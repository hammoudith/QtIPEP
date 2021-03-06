#include "imagemanip.h"
#include <cmath>

QImage ImageManip::zoomIn(){
    return QImage::scaled(QSize(this->size().width()*1.05,this->size().height()*1.05));
}
void ImageManip::zoomOut(){}

QImage ImageManip::negative(){
    //QColor rgb;
    //QImage negativeImage = QImage(QImage::size(),QImage::format());
    ;
//    for (int width = 0; width < QImage::size().width(); width++){
//        for (int height = 0; height < QImage::size().height(); height++){
//            rgb = QColor(QImage::pixel(width,height));
//            rgb.setBlue(255-rgb.blue());
//            rgb.setRed(255-rgb.red());
//            rgb.setGreen(255-rgb.green());
//            negativeImage.setPixel(width,height,rgb.rgb());
//        }
//    }


/*  this is a faster way to negate an image using using a combination of
    scanlines and bit manipulation*/

    QImage res = QImage(QImage::size(),QImage::format());
    for (int i = 0; i < QImage::size().height(); i++) {
        uint *q = (uint*)res.scanLine(i);
        const uint *p = (const uint*)QImage::constScanLine(i);
        const uint *end = p +  QImage::size().width();
        while (p < end) {
            uint c = *p;
            //*q = ((c << 16) & 0xff0000) | ((c >> 16) & 0xff) | (c & 0xff00ff00);
            *q = 0xffffffff - c;
            p++;
            q++;
        }
    }
    return res;
}

QImage ImageManip::increaseBrightness(const int brgtval){
    QImage res = QImage(QImage::size(),QImage::format());
    for (int i = 0; i < QImage::size().height(); i++) {
        uint *q = (uint*)res.scanLine(i);
        const uint *p = (const uint*)QImage::constScanLine(i);
        const uint *end = p +  QImage::size().width();
        while (p < end) {
            uint c = *p;
            int nr = qRed(c) + brgtval;
            if (nr>255) nr = 255;
            if (nr<0) nr=0;

            int ng = qGreen(c) + brgtval;
            if (ng>255) ng = 255;
            if (ng<0) ng=0;

            int nb = qBlue(c) + brgtval;
            if (nb>255) nb = 255;
            if (nb<0) nb=0;

            *q = qRgba(nr, ng, nb, 255);

            p++;
            q++;
        }
    }
    return res;
}

QImage ImageManip::decreaseBrightness(const int brgtval){
    QImage res = QImage(QImage::size(),QImage::format());
    for (int i = 0; i < QImage::size().height(); i++) {
        uint *q = (uint*)res.scanLine(i);
        const uint *p = (const uint*)QImage::constScanLine(i);
        const uint *end = p +  QImage::size().width();
        while (p < end) {
            uint c = *p;
            int nr = qRed(c) - brgtval;
            if (nr>255) nr = 255;
            if (nr<0) nr=0;

            int ng = qGreen(c) - brgtval;
            if (ng>255) ng = 255;
            if (ng<0) ng=0;

            int nb = qBlue(c) - brgtval;
            if (nb>255) nb = 255;
            if (nb<0) nb=0;

            *q = qRgba(nr, ng, nb, 255);

            p++;
            q++;
        }
    }
    return res;
}

QImage ImageManip::gammaCorrection(const double gamma){
    QImage res = QImage(QImage::size(),QImage::format());
    for (int i = 0; i < QImage::size().height(); i++) {
        uint *q = (uint*)res.scanLine(i);
        const uint *p = (const uint*)QImage::constScanLine(i);
        const uint *end = p +  QImage::size().width();
        while (p < end) {
            uint c = *p;
            int nr =pow(qRed(c),(1/gamma));
            if (nr>255) nr = 255;
            if (nr<0) nr=0;

            int ng = pow(qGreen(c),(1/gamma));
            if (ng>255) ng = 255;
            if (ng<0) ng=0;

            int nb = pow(qBlue(c),(1/gamma));
            if (nb>255) nb = 255;
            if (nb<0) nb=0;

            *q = qRgba(nr, ng, nb, 255);

            p++;
            q++;
        }
    }
    return res;
}

QImage ImageManip::modifyRGB(const int r, const int g, const int b){
    QImage res = QImage(QImage::size(),QImage::format());
    for (int i = 0; i < QImage::size().height(); i++) {
        uint *q = (uint*)res.scanLine(i);
        const uint *p = (const uint*)QImage::constScanLine(i);
        const uint *end = p +  QImage::size().width();
        while (p < end) {
            uint c = *p;
            int nr = qRed(c) + r;
            if (nr>255) nr = 255;
            if (nr<0) nr=0;

            int ng = qGreen(c) + g;
            if (ng>255) ng = 255;
            if (ng<0) ng=0;

            int nb = qBlue(c) + b;
            if (nb>255) nb = 255;
            if (nb<0) nb=0;

            *q = qRgba(nr, ng, nb, 255);
            p++;
            q++;
        }
    }
    return res;
}

QImage ImageManip::convertGrayScale(){
    QImage res = QImage(QImage::size(),QImage::format());
    for (int i = 1; i < QImage::size().height()-1; i++) {
        QRgb *q = (QRgb*) res.scanLine(i);
        QRgb *p = ( QRgb*)QImage::constScanLine(i);
        QRgb *end = p +  QImage::size().width()-1;
        while (p < end) {
            *q = qRgba((qRed(*p)+qGreen(*p)+qBlue(*p))/3,(qRed(*p)+qGreen(*p)+qBlue(*p))/3,(qRed(*p)+qGreen(*p)+qBlue(*p))/3,255);
            p++;
            q++;
        }
    }
    return res;
}

QImage ImageManip::horizontalEdgeDetection(){
    QImage res = QImage(QImage::size(),QImage::format());
    for (int i = 1; i < QImage::size().height()-1; i++) {
        QRgb *q = (QRgb*) res.scanLine(i);
        QRgb *pm1 = ( QRgb*)QImage::constScanLine(i-1);
        QRgb *p = ( QRgb*)QImage::constScanLine(i);
        QRgb *pp1 = ( QRgb*)QImage::constScanLine(i+1);
        pm1++;
        p++;
        pp1++;
        q++;
        QRgb *end = p +  QImage::size().width()-1;
        while (p < end) {

            int red = qRed(*(pm1-1))*hegmsk[0][0] + qRed(*(pm1))*hegmsk[0][1] + qRed(*(pm1+1))*hegmsk[0][2] +
                      qRed(*(p-1))*hegmsk[1][0] + qRed(*(p))*hegmsk[1][1] + qRed(*(p+1))*hegmsk[1][2] +
                      qRed(*(pp1)-1)*hegmsk[2][0] + qRed(*(pp1))*hegmsk[2][1] + qRed(*(pp1+1))*hegmsk[2][2];
            if (red>255) red=255;
            if (red<0) red=0;

            int green = qGreen(*(pm1-1))*hegmsk[0][0] + qGreen(*(pm1))*hegmsk[0][1] + qGreen(*(pm1+1))*hegmsk[0][2] +
                      qGreen(*(p-1))*hegmsk[1][0] + qGreen(*p)*hegmsk[1][1] + qGreen(*(p+1))*hegmsk[1][2] +
                      qGreen(*(pp1-1))*hegmsk[2][0] + qGreen(*pp1)*hegmsk[2][1] + qGreen(*(pp1+1))*hegmsk[2][2];
            if (green>255) green=255;
            if (green<0) green=0;

            int blue = qBlue(*(pm1-1))*hegmsk[0][0] + qBlue(*pm1)*hegmsk[0][1] + qBlue(*(pm1+1))*hegmsk[0][2] +
                      qBlue(*(p-1))*hegmsk[1][0] + qBlue(*p)*hegmsk[1][1] + qBlue(*(p+1))*hegmsk[1][2] +
                      qBlue(*(pp1-1))*hegmsk[2][0] + qBlue(*pp1)*hegmsk[2][1] + qBlue(*(pp1+1))*hegmsk[2][2];
            if (blue>255) blue=255;
            if (blue<0) blue=0;
            *q = qRgba(red,green,blue,255);
            pm1++;
            p++;
            pp1++;
            q++;
        }
    }
    return res;
}

// regarding the commented lines, it seems that casting the scanlines into a QRgb
// then manuplating the colors is faster than creating a QColor class for each pixel
QImage ImageManip::verticalEdgeDetection(){
    QImage res = QImage(QImage::size(),QImage::format());
    for (int i = 1; i < QImage::size().height()-1; i++) {
//        uint *q = (uint*)res.scanLine(i);
//        const uint *pm1 = (const uint*)QImage::constScanLine(i-1);
//        const uint *p = (const uint*)QImage::constScanLine(i);
//        const uint *pp1 = (const uint*)QImage::constScanLine(i+1);
//        const uint *end = p +  QImage::size().width()-1;
        QRgb *q = (QRgb*) res.scanLine(i);
        QRgb *pm1 = ( QRgb*)QImage::constScanLine(i-1);
        QRgb *p = ( QRgb*)QImage::constScanLine(i);
        QRgb *pp1 = ( QRgb*)QImage::constScanLine(i+1);
        pm1++;
        p++;
        pp1++;
        q++;
        QRgb *end = p +  QImage::size().width()-1;
        while (p < end) {

/*
            QColor c11 = QColor(*(pm1 - 1));
            QColor c12 = QColor(*pm1);
            QColor c13 = QColor(*(pm1 + 1));
            QColor c21 = QColor(*(p - 1));
            QColor c22 = QColor(*p);
            QColor c23 = QColor(*(p + 1));
            QColor c31 = QColor(*(pp1 - 1));
            QColor c32 = QColor(*pp1);
            QColor c33 = QColor(*(pp1 + 1));

            int red = c11.red()*vegmsk[0][0] + c12.red()*vegmsk[0][1] + c13.red()*vegmsk[0][2] +
                      c21.red()*vegmsk[1][0] + c22.red()*vegmsk[1][1] + c23.red()*vegmsk[1][2] +
                      c31.red()*vegmsk[2][0] + c32.red()*vegmsk[2][1] + c33.red()*vegmsk[2][2];
            if (red>255) red=255;
            if (red<0) red=0;

            int green = c11.green()*vegmsk[0][0] + c12.green()*vegmsk[0][1] + c13.green()*vegmsk[0][2] +
                        c21.green()*vegmsk[1][0] + c22.green()*vegmsk[1][1] + c23.green()*vegmsk[1][2] +
                        c31.green()*vegmsk[2][0] + c32.green()*vegmsk[2][1] + c33.green()*vegmsk[2][2];
            if (green>255) green=255;
            if (green<0) green=0;

            int blue = c11.blue()*vegmsk[0][0] + c12.blue()*vegmsk[0][1] + c13.blue()*vegmsk[0][2] +
                       c21.blue()*vegmsk[1][0] + c22.blue()*vegmsk[1][1] + c23.blue()*vegmsk[1][2] +
                       c31.blue()*vegmsk[2][0] + c32.blue()*vegmsk[2][1] + c33.blue()*vegmsk[2][2];
            if (blue>255) blue=255;
            if (blue<0) blue=0;
*/

            int red = qRed(*(pm1-1))*vegmsk[0][0] + qRed(*(pm1))*vegmsk[0][1] + qRed(*(pm1+1))*vegmsk[0][2] +
                      qRed(*(p-1))*vegmsk[1][0] + qRed(*(p))*vegmsk[1][1] + qRed(*(p+1))*vegmsk[1][2] +
                      qRed(*(pp1)-1)*vegmsk[2][0] + qRed(*(pp1))*vegmsk[2][1] + qRed(*(pp1+1))*vegmsk[2][2];
            if (red>255) red=255;
            if (red<0) red=0;

            int green = qGreen(*(pm1-1))*vegmsk[0][0] + qGreen(*(pm1))*vegmsk[0][1] + qGreen(*(pm1+1))*vegmsk[0][2] +
                      qGreen(*(p-1))*vegmsk[1][0] + qGreen(*p)*vegmsk[1][1] + qGreen(*(p+1))*vegmsk[1][2] +
                      qGreen(*(pp1-1))*vegmsk[2][0] + qGreen(*pp1)*vegmsk[2][1] + qGreen(*(pp1+1))*vegmsk[2][2];
            if (green>255) green=255;
            if (green<0) green=0;

            int blue = qBlue(*(pm1-1))*vegmsk[0][0] + qBlue(*pm1)*vegmsk[0][1] + qBlue(*(pm1+1))*vegmsk[0][2] +
                      qBlue(*(p-1))*vegmsk[1][0] + qBlue(*p)*vegmsk[1][1] + qBlue(*(p+1))*vegmsk[1][2] +
                      qBlue(*(pp1-1))*vegmsk[2][0] + qBlue(*pp1)*vegmsk[2][1] + qBlue(*(pp1+1))*vegmsk[2][2];
            if (blue>255) blue=255;
            if (blue<0) blue=0;


            *q = qRgba(red,green,blue,255);
            pm1++;
            p++;
            pp1++;
            q++;
        }
    }
    return res;
}

QImage ImageManip::sobelTransformation(){
    QImage horres = this->horizontalEdgeDetection();
    QImage verres = this->verticalEdgeDetection();
    QImage res = QImage(QImage::size(),QImage::format());
    for (int i = 0; i < QImage::size().height(); i++) {
        QRgb *q = (QRgb*) res.scanLine(i);
        //QRgb *p = ( QRgb*)QImage::constScanLine(i);
        QRgb *ph = ( QRgb*)horres.scanLine(i);
        QRgb *pv = ( QRgb*)verres.scanLine(i);
        QRgb *end = q +  QImage::size().width();
        while (q < end) {

            int red =(int) sqrt(pow(qRed(*ph),2)+pow(qRed(*pv),2));
            //int red =qRed(*ph)+qRed(*pv);
            if (red>255) red=255;
            if (red<0) red=0;

            int green =(int) sqrt(pow(qGreen(*ph),2)+pow(qGreen(*pv),2));
            //int green =qGreen(*ph)+qGreen(*pv);
            if (green>255) green=255;
            if (green<0) green=0;

            int blue =(int) sqrt(pow(qBlue(*ph),2)+pow(qBlue(*pv),2));
            //int blue =qBlue(*ph)+qBlue(*pv);
            if (blue>255) blue=255;
            if (blue<0) blue=0;
            *q = qRgba(red,green,blue,255);
            //p++;
            ph++;
            pv++;
            q++;
        }
    }
    return res;
    //ImageManip res = ImageManip(horres.bits(),horres.width(),horres.height(),horres.format());
    //return res.verticalEdgeDetection();
}

QImage ImageManip::laplacianTransformation(){

    QImage res = QImage(QImage::size(),QImage::format());
    for (int i = 2; i < QImage::size().height()-2; i++) {
        QRgb *q = (QRgb *)res.scanLine(i);
        QRgb *pm2 = (QRgb *)QImage::constScanLine(i-2);
        QRgb *pm1 = (QRgb *)QImage::constScanLine(i-1);
        QRgb *p = (QRgb *)QImage::constScanLine(i);
        QRgb *pp1 = (QRgb *)QImage::constScanLine(i+1);
        QRgb *pp2 = (QRgb *)QImage::constScanLine(i+2);
        QRgb *end = p +  QImage::size().width()-2;
        while (p < end) {
            int red =  qRed(*(pm2 - 2))*lapmsk[0][0] + qRed(*(pm2 - 1))*lapmsk[0][1] + qRed(*(pm2))*lapmsk[0][2] + qRed(*(pm2 + 1))*lapmsk[0][3] + qRed(*(pm2 + 2))*lapmsk[0][4] +
                       qRed(*(pm1 - 2))*lapmsk[1][0] + qRed(*(pm1 - 1))*lapmsk[1][1] + qRed(*(pm1))*lapmsk[1][2] + qRed(*(pm1 + 1))*lapmsk[1][3] + qRed(*(pm1 + 2))*lapmsk[1][4] +
                       qRed(*(  p - 2))*lapmsk[2][0] + qRed(*(  p - 1))*lapmsk[2][1] + qRed(*(  p))*lapmsk[2][2] + qRed(*(  p + 1))*lapmsk[2][3] + qRed(*(  p + 2))*lapmsk[2][4] +
                       qRed(*(pp1 - 2))*lapmsk[3][0] + qRed(*(pp1 - 1))*lapmsk[3][1] + qRed(*(pp1))*lapmsk[3][2] + qRed(*(pp1 + 1))*lapmsk[3][3] + qRed(*(pp1 + 2))*lapmsk[3][4] +
                       qRed(*(pp2 - 2))*lapmsk[4][0] + qRed(*(pp2 - 1))*lapmsk[4][1] + qRed(*(pp2))*lapmsk[4][2] + qRed(*(pp2 + 1))*lapmsk[4][3] + qRed(*(pp2 + 2))*lapmsk[4][4];
            if (red>255) red=255;
            if (red<0) red=0;

            int green =  qGreen(*(pm2 - 2))*lapmsk[0][0] + qGreen(*(pm2 - 1))*lapmsk[0][1] + qGreen(*(pm2))*lapmsk[0][2] + qGreen(*(pm2 + 1))*lapmsk[0][3] + qGreen(*(pm2 + 2))*lapmsk[0][4] +
                         qGreen(*(pm1 - 2))*lapmsk[1][0] + qGreen(*(pm1 - 1))*lapmsk[1][1] + qGreen(*(pm1))*lapmsk[1][2] + qGreen(*(pm1 + 1))*lapmsk[1][3] + qGreen(*(pm1 + 2))*lapmsk[1][4] +
                         qGreen(*(  p - 2))*lapmsk[2][0] + qGreen(*(  p - 1))*lapmsk[2][1] + qGreen(*(  p))*lapmsk[2][2] + qGreen(*(  p + 1))*lapmsk[2][3] + qGreen(*(  p + 2))*lapmsk[2][4] +
                         qGreen(*(pp1 - 2))*lapmsk[3][0] + qGreen(*(pp1 - 1))*lapmsk[3][1] + qGreen(*(pp1))*lapmsk[3][2] + qGreen(*(pp1 + 1))*lapmsk[3][3] + qGreen(*(pp1 + 2))*lapmsk[3][4] +
                         qGreen(*(pp2 - 2))*lapmsk[4][0] + qGreen(*(pp2 - 1))*lapmsk[4][1] + qGreen(*(pp2))*lapmsk[4][2] + qGreen(*(pp2 + 1))*lapmsk[4][3] + qGreen(*(pp2 + 2))*lapmsk[4][4];
            if (green>255) green=255;
            if (green<0) green=0;

            int blue =  qBlue(*(pm2 - 2))*lapmsk[0][0] + qBlue(*(pm2 - 1))*lapmsk[0][1] + qBlue(*(pm2))*lapmsk[0][2] + qBlue(*(pm2 + 1))*lapmsk[0][3] + qBlue(*(pm2 + 2))*lapmsk[0][4] +
                        qBlue(*(pm1 - 2))*lapmsk[1][0] + qBlue(*(pm1 - 1))*lapmsk[1][1] + qBlue(*(pm1))*lapmsk[1][2] + qBlue(*(pm1 + 1))*lapmsk[1][3] + qBlue(*(pm1 + 2))*lapmsk[1][4] +
                        qBlue(*(  p - 2))*lapmsk[2][0] + qBlue(*(  p - 1))*lapmsk[2][1] + qBlue(*(  p))*lapmsk[2][2] + qBlue(*(  p + 1))*lapmsk[2][3] + qBlue(*(  p + 2))*lapmsk[2][4] +
                        qBlue(*(pp1 - 2))*lapmsk[3][0] + qBlue(*(pp1 - 1))*lapmsk[3][1] + qBlue(*(pp1))*lapmsk[3][2] + qBlue(*(pp1 + 1))*lapmsk[3][3] + qBlue(*(pp1 + 2))*lapmsk[3][4] +
                        qBlue(*(pp2 - 2))*lapmsk[4][0] + qBlue(*(pp2 - 1))*lapmsk[4][1] + qBlue(*(pp2))*lapmsk[4][2] + qBlue(*(pp2 + 1))*lapmsk[4][3] + qBlue(*(pp2 + 2))*lapmsk[4][4];
            if (blue>255) blue=255;
            if (blue<0) blue=0;
            *q = qRgba(red,green,blue,255);
            pm2++;
            pm1++;
            p++;
            pp1++;
            pp2++;
            q++;
        }
    }
    return res;
}

QImage ImageManip::heavyBlur(){
    QImage res = QImage(QImage::size(),QImage::format());
    for (int i = 1; i < QImage::size().height()-1; i++) {
        QRgb *q = (QRgb*) res.scanLine(i);
        QRgb *pm1 = ( QRgb*)QImage::constScanLine(i-1);
        QRgb *p = ( QRgb*)QImage::constScanLine(i);
        QRgb *pp1 = ( QRgb*)QImage::constScanLine(i+1);
        QRgb *end = p +  QImage::size().width()-1;
        while (p < end) {

            int red = (qRed(*(pm1-1))*hblurmsk[0][0] + qRed(*(pm1))*hblurmsk[0][1] + qRed(*(pm1+1))*hblurmsk[0][2] +
                      qRed(*(p-1))*hblurmsk[1][0] + qRed(*(p))*hblurmsk[1][1] + qRed(*(p+1))*hblurmsk[1][2] +
                      qRed(*(pp1)-1)*hblurmsk[2][0] + qRed(*(pp1))*hblurmsk[2][1] + qRed(*(pp1+1))*hblurmsk[2][2])/9;
            if (red>255) red=255;
            if (red<0) red=0;

            int green = (qGreen(*(pm1-1))*hblurmsk[0][0] + qGreen(*(pm1))*hblurmsk[0][1] + qGreen(*(pm1+1))*hblurmsk[0][2] +
                      qGreen(*(p-1))*hblurmsk[1][0] + qGreen(*p)*hblurmsk[1][1] + qGreen(*(p+1))*hblurmsk[1][2] +
                      qGreen(*(pp1-1))*hblurmsk[2][0] + qGreen(*pp1)*hblurmsk[2][1] + qGreen(*(pp1+1))*hblurmsk[2][2])/9;
            if (green>255) green=255;
            if (green<0) green=0;

            int blue = (qBlue(*(pm1-1))*hblurmsk[0][0] + qBlue(*pm1)*hblurmsk[0][1] + qBlue(*(pm1+1))*hblurmsk[0][2] +
                      qBlue(*(p-1))*hblurmsk[1][0] + qBlue(*p)*hblurmsk[1][1] + qBlue(*(p+1))*hblurmsk[1][2] +
                      qBlue(*(pp1-1))*hblurmsk[2][0] + qBlue(*pp1)*hblurmsk[2][1] + qBlue(*(pp1+1))*hblurmsk[2][2])/9;
            if (blue>255) blue=255;
            if (blue<0) blue=0;
            *q = qRgba(red,green,blue,255);
            pm1++;
            p++;
            pp1++;
            q++;
        }
    }
    return res;
}

QImage ImageManip::lightBlur(){
    QImage res = QImage(QImage::size(),QImage::format());
    for (int i = 1; i < QImage::size().height()-1; i++) {
        QRgb *q = (QRgb*) res.scanLine(i);
        QRgb *pm1 = ( QRgb*)QImage::constScanLine(i-1);
        QRgb *p = ( QRgb*)QImage::constScanLine(i);
        QRgb *pp1 = ( QRgb*)QImage::constScanLine(i+1);
        QRgb *end = p +  QImage::size().width()-1;
        while (p < end) {

            int red = (qRed(*(pm1-1))*lblurmsk[0][0] + qRed(*(pm1))*lblurmsk[0][1] + qRed(*(pm1+1))*lblurmsk[0][2] +
                      qRed(*(p-1))*lblurmsk[1][0] + qRed(*(p))*lblurmsk[1][1] + qRed(*(p+1))*lblurmsk[1][2] +
                      qRed(*(pp1)-1)*lblurmsk[2][0] + qRed(*(pp1))*lblurmsk[2][1] + qRed(*(pp1+1))*lblurmsk[2][2])/9;
            if (red>255) red=255;
            if (red<0) red=0;

            int green = (qGreen(*(pm1-1))*lblurmsk[0][0] + qGreen(*(pm1))*lblurmsk[0][1] + qGreen(*(pm1+1))*lblurmsk[0][2] +
                      qGreen(*(p-1))*lblurmsk[1][0] + qGreen(*p)*lblurmsk[1][1] + qGreen(*(p+1))*lblurmsk[1][2] +
                      qGreen(*(pp1-1))*lblurmsk[2][0] + qGreen(*pp1)*lblurmsk[2][1] + qGreen(*(pp1+1))*lblurmsk[2][2])/9;
            if (green>255) green=255;
            if (green<0) green=0;

            int blue = (qBlue(*(pm1-1))*lblurmsk[0][0] + qBlue(*pm1)*lblurmsk[0][1] + qBlue(*(pm1+1))*lblurmsk[0][2] +
                      qBlue(*(p-1))*lblurmsk[1][0] + qBlue(*p)*lblurmsk[1][1] + qBlue(*(p+1))*lblurmsk[1][2] +
                      qBlue(*(pp1-1))*lblurmsk[2][0] + qBlue(*pp1)*lblurmsk[2][1] + qBlue(*(pp1+1))*lblurmsk[2][2])/9;
            if (blue>255) blue=255;
            if (blue<0) blue=0;
            *q = qRgba(red,green,blue,255);
            pm1++;
            p++;
            pp1++;
            q++;
        }
    }
    return res;
}

QImage ImageManip::highPass(){
    QImage res = QImage(QImage::size(),QImage::format());
    for (int i = 1; i < QImage::size().height()-1; i++) {
        QRgb *q = (QRgb*) res.scanLine(i);
        QRgb *pm1 = ( QRgb*)QImage::constScanLine(i-1);
        QRgb *p = ( QRgb*)QImage::constScanLine(i);
        QRgb *pp1 = ( QRgb*)QImage::constScanLine(i+1);
        QRgb *end = p +  QImage::size().width()-1;
        while (p < end) {

            int red = (qRed(*(pm1-1))*hpassmsk[0][0] + qRed(*(pm1))*hpassmsk[0][1] + qRed(*(pm1+1))*hpassmsk[0][2] +
                      qRed(*(p-1))*hpassmsk[1][0] + qRed(*(p))*hpassmsk[1][1] + qRed(*(p+1))*hpassmsk[1][2] +
                      qRed(*(pp1)-1)*hpassmsk[2][0] + qRed(*(pp1))*hpassmsk[2][1] + qRed(*(pp1+1))*hpassmsk[2][2])/9;
            if (red>255) red=255;
            if (red<0) red=0;

            int green = (qGreen(*(pm1-1))*hpassmsk[0][0] + qGreen(*(pm1))*hpassmsk[0][1] + qGreen(*(pm1+1))*hpassmsk[0][2] +
                      qGreen(*(p-1))*hpassmsk[1][0] + qGreen(*p)*hpassmsk[1][1] + qGreen(*(p+1))*hpassmsk[1][2] +
                      qGreen(*(pp1-1))*hpassmsk[2][0] + qGreen(*pp1)*hpassmsk[2][1] + qGreen(*(pp1+1))*hpassmsk[2][2])/9;
            if (green>255) green=255;
            if (green<0) green=0;

            int blue = (qBlue(*(pm1-1))*hpassmsk[0][0] + qBlue(*pm1)*hpassmsk[0][1] + qBlue(*(pm1+1))*hpassmsk[0][2] +
                      qBlue(*(p-1))*hpassmsk[1][0] + qBlue(*p)*hpassmsk[1][1] + qBlue(*(p+1))*hpassmsk[1][2] +
                      qBlue(*(pp1-1))*hpassmsk[2][0] + qBlue(*pp1)*hpassmsk[2][1] + qBlue(*(pp1+1))*hpassmsk[2][2])/9;
            if (blue>255) blue=255;
            if (blue<0) blue=0;
            *q = qRgba(red,green,blue,255);
            pm1++;
            p++;
            pp1++;
            q++;
        }
    }
    return res;
}

QImage ImageManip::lowPass(){
    QImage res = QImage(QImage::size(),QImage::format());
    for (int i = 1; i < QImage::size().height()-1; i++) {
        QRgb *q = (QRgb*) res.scanLine(i);
        QRgb *pm1 = ( QRgb*)QImage::constScanLine(i-1);
        QRgb *p = ( QRgb*)QImage::constScanLine(i);
        QRgb *pp1 = ( QRgb*)QImage::constScanLine(i+1);
        QRgb *end = p +  QImage::size().width()-1;
        while (p < end) {

            int red = (qRed(*(pm1-1))*hblurmsk[0][0] + qRed(*(pm1))*hblurmsk[0][1] + qRed(*(pm1+1))*hblurmsk[0][2] +
                      qRed(*(p-1))*hblurmsk[1][0] + qRed(*(p))*hblurmsk[1][1] + qRed(*(p+1))*hblurmsk[1][2] +
                      qRed(*(pp1)-1)*hblurmsk[2][0] + qRed(*(pp1))*hblurmsk[2][1] + qRed(*(pp1+1))*hblurmsk[2][2])/9;
            if (red>255) red=255;
            if (red<0) red=0;

            int green = (qGreen(*(pm1-1))*hblurmsk[0][0] + qGreen(*(pm1))*hblurmsk[0][1] + qGreen(*(pm1+1))*hblurmsk[0][2] +
                      qGreen(*(p-1))*hblurmsk[1][0] + qGreen(*p)*hblurmsk[1][1] + qGreen(*(p+1))*hblurmsk[1][2] +
                      qGreen(*(pp1-1))*hblurmsk[2][0] + qGreen(*pp1)*hblurmsk[2][1] + qGreen(*(pp1+1))*hblurmsk[2][2])/9;
            if (green>255) green=255;
            if (green<0) green=0;

            int blue = (qBlue(*(pm1-1))*hblurmsk[0][0] + qBlue(*pm1)*hblurmsk[0][1] + qBlue(*(pm1+1))*hblurmsk[0][2] +
                      qBlue(*(p-1))*hblurmsk[1][0] + qBlue(*p)*hblurmsk[1][1] + qBlue(*(p+1))*hblurmsk[1][2] +
                      qBlue(*(pp1-1))*hblurmsk[2][0] + qBlue(*pp1)*hblurmsk[2][1] + qBlue(*(pp1+1))*hblurmsk[2][2])/9;
            if (blue>255) blue=255;
            if (blue<0) blue=0;
            *q = qRgba(red,green,blue,255);
            pm1++;
            p++;
            pp1++;
            q++;
        }
    }
    return res;
}

QImage ImageManip::rotateClockwise(){
    //cos(90) = 0, sin(90) = 1
    //x2 = -i + y/2 + x/2
    //y2 = j - x/2 + y/2
    QImage res = QImage(QImage::size().height(), QImage::size().width(),QImage::format());
    for (int i = 0; i < QImage::size().height(); i++) {
        //uint *q = (uint*)res.scanLine(i);
        const uint *p = (const uint*)QImage::constScanLine(i);
        const uint *end = p +  QImage::size().width();
        int j = 0;
        while (p < end) {
            uint c = *p;
            int x = QImage::size().height() -i-1;
            int y = j;
            uint *q = (uint*)res.scanLine(y);
            q += x;
            *q = c;
            j++;
            p++;
            //q++;
        }
    }
    return res;
}

QImage ImageManip::rotateAntiClockwise(){
    //cos(-90) = 0, sin(-90) = -1
    QImage res = QImage(QImage::size().height(), QImage::size().width(),QImage::format());
    for (int i = 0; i < QImage::size().height(); i++) {
        //uint *q = (uint*)res.scanLine(i);
        const uint *p = (const uint*)QImage::constScanLine(i);
        const uint *end = p +  QImage::size().width();
        int j = 0;
        while (p < end) {
            uint c = *p;
            int x = i;
            int y = QImage::size().width() - j -1;
            uint *q = (uint*)res.scanLine(y);
            q += x;
            *q = c;
            j++;
            p++;
        }
    }
    return res;
}

void ImageManip::operator = (const QImage& base_)
{
  QImage::operator=(base_);
}

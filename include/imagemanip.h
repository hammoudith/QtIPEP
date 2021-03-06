#ifndef IMAGEMANIP_H
#define IMAGEMANIP_H

#include <QImage>
#include <QColor>
#include <QDebug>


//matrix mask for heavy blur
const int hblurmsk[3][3] = {{1, 1, 1},
                            { 1,  1,  1},
                            { 1,  1,  1}};

//matrix mask for light blur
const int lblurmsk[3][3] = {{1, 2, 1},
                            { 2,  4,  2},
                            { 1,  2,  1}};

//matrix mask for detecting horizontal edges
const int hegmsk[3][3] = {  { 1,  2,  1},
                            { 0,  0,  0},
                            {-1, -2, -1}};

//matrix mask for detecting vertical edges
const int vegmsk[3][3] = {   {-1,  0,  1},
                             {-2,  0,  2},
                             {-1,  0,  1}};

//matrix mask for sharpening an image
const int sharp[3][3] = {  {-1,  -1,  -1},
                           {-1,  9,   -1},
                           {-1,  -1,  -1}};

//matrix mask for high pass filter
const int hpassmsk[3][3] = {  {-1,  -1,  -1},
                           {-1,  8,   -1},
                           {-1,  -1,  -1}};

//matrix mask to apply laplacian transformation
const int lapmsk[5][5] = {  {-1,-1,-1,-1,-1},
                            {-1,-1,-1,-1,-1},
                            {-1,-1,24,-1,-1},
                            {-1,-1,-1,-1,-1},
                            {-1,-1,-1,-1,-1}};

class ImageManip : public QImage
{
    using QImage::QImage;



public:
    QImage zoomIn();
    void zoomOut();
    QImage negative();
    QImage increaseBrightness(const int brgtval);
    QImage decreaseBrightness(const int brgtval);
    QImage gammaCorrection(const double gamma);
    QImage horizontalEdgeDetection();
    QImage verticalEdgeDetection();
    QImage laplacianTransformation();
    QImage heavyBlur();
    QImage lightBlur();
    QImage highPass();
    QImage lowPass();
    QImage sobelTransformation();
    QImage convertGrayScale();
    QImage modifyRGB(const int r, const int g, const int b);
    QImage rotateClockwise();
    QImage rotateAntiClockwise();
    void operator = (const QImage& base_);



};

#endif // IMAGEMANIP_H

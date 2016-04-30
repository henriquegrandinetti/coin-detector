#include <stdio.h>
#include <math.h>

#include "list.h"
#include "edge.h"
#include "hough.h"

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;


int main( int argc, char** argv ){
    
    if(argc < 2) {
        printf( "No input image\n " );
        return -1;
    }
    
    char* imageName = argv[1];
    
    Mat image, imageC;
    image = imread( imageName, 0);
    imageC = imread( imageName, 1);
    
    if( !image.data ) {
        printf( "No image data \n " );
        return -1;
    }
    
    Mat xderivative = cv::Mat(image.rows, image.cols, CV_32FC1);
    Mat yderivative = cv::Mat(image.rows, image.cols, CV_32FC1);
    Mat gmagnitude  = cv::Mat(image.rows, image.cols, CV_32FC1);
    Mat gdirection  = cv::Mat(image.rows, image.cols, CV_32FC1);
    
    Mat gthreshold  = cv::Mat(image.rows, image.cols, CV_8UC1);
    
    int xkernel[3][3] = {{-1,-2,-1},
                         { 0, 0, 0},
                         { 1, 2, 1}};
    int ykernel[3][3] = {{-1, 0, 1},
                         {-2, 0, 2},
                         {-1, 0, 1}};
    
    Derive(image, xkernel, xderivative);
    Derive(image, ykernel, yderivative);
    Direction(xderivative, yderivative, gdirection);
    GradMagnitude(xderivative, yderivative, gmagnitude);
    
    Normalize(gmagnitude);
    GradientThreshold(gmagnitude, gthreshold, 60);
    Hough(gdirection, gthreshold, imageC, 4);
    
    Normalize(gdirection);
    NormalizeDerivative(xderivative, yderivative);
    
    imwrite( "xDerivative.jpg", xderivative);
    imwrite( "yDerivative.jpg", yderivative);
    imwrite( "gMagnitude.jpg", gmagnitude);
    imwrite( "gDirection.jpg", gdirection);
    
    return 0;
}
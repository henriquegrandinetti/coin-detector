#include "edge.h"
#include "hough.h"

#include <stdio.h>
#include <math.h>
#include <iostream>

using namespace cv;

void Derive (cv::Mat &input, int kernel[3][3], cv::Mat &output){
    //m and n helps when the kernel is outside the image, we are using the extend approach
    int y, x, m, n;
    float sum;
    
    for(y = 0; y < input.rows; y++){
        for(x = 0; x < input.cols; x++){
            
            sum = 0;
            
            for(int j = -1; j <= 1; j++){
                for(int i = -1; i <= 1; i++){
                    
                    //ajust when x is out of the image
                    if (x - i < 0)
                        m = 0;
                    else if (x - i > input.cols - 1)
                        m = input.cols - 1;
                    else
                        m = x - i;
                    
                    //ajust when y is out of the image
                    if (y - j < 0)
                        n = 0;
                    else if (y - j > input.rows - 1)
                        n = input.rows - 1;
                    else
                        n = y - j;
                    
                    sum += kernel[i+1][j+1] * input.at<uchar>(n, m);
                }
            }
            
            output.at<float>(y, x) = sum;
            
        }
    }
}

void GradMagnitude (cv::Mat &xderivative, cv::Mat &yderivative, cv::Mat &output){
    int y, x;
    
    for(y = 1; y < xderivative.rows - 1; y++){
        for(x = 1; x < yderivative.cols - 1; x++){
            output.at<float>(y, x) = sqrt(pow(xderivative.at<float>(y, x), 2) + pow(yderivative.at<float>(y, x), 2));
        }
    }
}

// Find max and min int both derivatives, in order to
// normalize with the same parameters
void NormalizeDerivative (cv::Mat &xderivative, cv::Mat &yderivative){
    int y, x;
    float max = -9999999;
    float min =  9999999;
    
    for(y = 0; y < xderivative.rows; y++){
        for(x = 0; x < xderivative.cols; x++){
            if (xderivative.at<float>(y, x) > yderivative.at<float>(y, x)) {
                if (xderivative.at<float>(y, x) > max)
                    max = xderivative.at<float>(y, x);
                if (yderivative.at<float>(y, x) < min)
                    min = yderivative.at<float>(y, x);
            }
            else {
                if (yderivative.at<float>(y, x) > max)
                    max = yderivative.at<float>(y, x);
                if (xderivative.at<float>(y, x) < min)
                    min = xderivative.at<float>(y, x);
            }
        }
    }
    
    for(y = 0; y < xderivative.rows; y++){
        for(x = 0; x < yderivative.cols; x++){
            //normalizes x
            xderivative.at<float>(y, x) -= min;
            xderivative.at<float>(y, x) /= (max - min)/255;
            
            //normalizes y
            yderivative.at<float>(y, x) -= min;
            yderivative.at<float>(y, x) /= (max - min)/255;
        }
    }
}

void Direction(cv::Mat &xderivative, cv::Mat &yderivative, cv::Mat &output){
    int x, y;
    
    for(y = 0; y < yderivative.rows; y++){
        for(x = 0; x < xderivative.cols; x++){
            output.at<float>(y, x) = atan(yderivative.at<float>(y, x)/xderivative.at<float>(y, x));
        }
    }
}
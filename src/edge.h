#ifndef __opencv__border__
#define __opencv__border__

#include <stdio.h>

#include "edge.h"
#include <stdio.h>
#include <math.h>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

void Derive (cv::Mat &input, int kernel[3][3], cv::Mat &output);
void GradMagnitude (cv::Mat &xderivative, cv::Mat &yderivative, cv::Mat &output);
void NormalizeDerivative (cv::Mat &xderivative, cv::Mat &yderivative);
void Direction(cv::Mat &xderivative, cv::Mat &yderivative, cv::Mat &output);
void SobelEdgeDetection (cv::Mat &image, cv::Mat &gdirection, cv::Mat &gmagnitude);

#endif
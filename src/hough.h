#ifndef __opencv__hough__
#define __opencv__hough__

#include <stdio.h>

#include "hough.h"
#include "list.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

int*** allocate3D(int l,int m,int n);

void deallocate3D(int*** arr3D,int l,int m);

void Normalize (cv::Mat &input);

void LowPassFilter (cv::Mat &input, int kernel[3][3], cv::Mat &output);

void GradientThreshold (cv::Mat &gmagnitude, cv::Mat &gthreshold, int t);
    
int WithinImage (int x, int y, float xshift, float yshift, cv::Mat image);

void PrintHoughSpace (int ***voting, cv::Mat &houghspace, int minradius, int maxradius);

void VoteCenters (cv::Mat &threshold, cv::Mat &direction, int ***voting, int minradius, int maxradius);

void ThresholdVotes (int ***voting, cv::Mat centers, int minradius, int maxradius, int t);

void FindCoinsCenters (cv::Mat centers, cv::Mat centerLPF, ListType *circles, int ***voting, int minradius, int maxradius);

void DrawCircles (cv::Mat image, ListType *circles, int coindistance, int minradius, int maxradius, int ***voting);

void Hough (cv::Mat &direction, cv::Mat &threshold, cv::Mat &image, int t);


#endif
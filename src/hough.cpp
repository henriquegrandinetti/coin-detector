#include "hough.h"
#include "list.h"

#include <math.h>

using namespace cv;

int*** allocate3D(int l,int m,int n){
    int ***arr3D;
    int i,j;
    
    arr3D = (int***)malloc(l * sizeof(int **));
    
    for(i=0;i<l;i++){
        arr3D[i] = (int**)malloc(m * sizeof(int*));
        for(j=0;j<m;j++){
            arr3D[i][j] = (int*)malloc(n*sizeof(int));
        }
    }
    
    return arr3D;
}

void deallocate3D(int*** arr3D,int l,int m)
{
    int i,j;
    
    for(i=0; i<l; i++){
        for(j=0; j<m; j++){
            free(arr3D[i][j]);
        }
        free(arr3D[i]);
    }
    free(arr3D);
}

void Normalize (cv::Mat &input){
    int y, x;
    float max = -9999999;
    float min =  9999999;
    
    for(y = 0; y < input.rows; y++){
        for(x = 0; x < input.cols; x++){
            if (input.at<float>(y, x) > max)
                max = input.at<float>(y, x);
            else if (input.at<float>(y, x) < min)
                min = input.at<float>(y, x);
        }
    }
    
    for(y = 0; y < input.rows; y++){
        for(x = 0; x < input.cols; x++){
            input.at<float>(y, x) -= min;
            input.at<float>(y, x) /= (max - min)/255;
        }
    }
}

void LowPassFilter (cv::Mat &input, int kernel[3][3], cv::Mat &output){
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
                    
                    sum += kernel[i+1][j+1] * input.at<float>(Point(m,n));
                }
            }
            
            output.at<float>(Point(x,y)) = sum;
            
        }
    }
    
    Normalize(output);
}

void GradientThreshold (cv::Mat &gmagnitude, cv::Mat &gthreshold, int t){
    int y, x;
    
    for(y = 0; y < gmagnitude.rows; y++){
        for(x = 0; x < gmagnitude.cols; x++){
            if (gmagnitude.at<float>(y, x) > t)
                gthreshold.at<uchar>(y, x) = 255;
            else
                gthreshold.at<uchar>(y, x) = 0;
        }
    }
}

int WithinImage (int x, int y, float xshift, float yshift, cv::Mat image){
    if ((int)(x + xshift) >= 0 && (int)(x + xshift) < image.cols && (int)(y + yshift) >= 0 && (int)(y + yshift) < image.rows)
        return 1;
    else
        return 0;
}

void PrintHoughSpace (int ***voting, cv::Mat &houghspace, int minradius, int maxradius){
    int x, y, r;
    
    for (y = 0; y < houghspace.rows; y++) {
        for (x = 0; x < houghspace.cols; x++) {
            for (r = minradius; r < maxradius; r++) {
                if (voting[x][y][r - minradius] > 0) {
                    houghspace.at<float>(y, x) += voting[x][y][r - minradius];
                }
            }
        }
    }
    
    Normalize(houghspace);
    imwrite("HoughSpace.jpg", houghspace);
}

void VoteCenters (cv::Mat &threshold, cv::Mat &direction, int ***voting, int minradius, int maxradius){
    int x, y, r;
    float rsin, rcos;
    
    for (y = 0; y < direction.rows; y++) {
        for (x = 0; x < direction.cols; x++) {
            if (threshold.at<uchar>(y, x) == 255) {
                for (r = minradius; r < maxradius; r++){
                    rcos = r * cos(direction.at<float>(y,x));
                    rsin = r * sin(direction.at<float>(y,x));
                    
                    if (WithinImage(x, y, rcos, rsin, direction))
                        voting[(int)(x + rcos)][(int)(y + rsin)][r - minradius] += 1;
                    if (WithinImage(x, y, -rcos, -rsin, direction))
                        voting[(int)(x - rcos)][(int)(y - rsin)][r - minradius] += 1;
                }
            }
        }
    }
}

void ThresholdVotes (int ***voting, cv::Mat centers, int minradius, int maxradius, int t){
    int x, y, r;
    for (y = 0; y < centers.rows; y++) {
        for (x = 0; x < centers.cols; x++) {
            for (r = minradius; r < maxradius; r++) {
                if (voting[x][y][r - minradius] > t) {
                    centers.at<float>(y, x) += voting[x][y][r - minradius];
                }
            }
        }
    }
    Normalize(centers);
}

void FindCoinsCenters (cv::Mat centers, cv::Mat centerLPF, ListType *circles, int ***voting, int minradius, int maxradius){
    int x, y;
    ItemType *point = (ItemType*) malloc(sizeof(ItemType));
    
    for (y = 0; y < centers.rows; y++) {
        for (x = 0; x < centers.cols; x++) {
            if (centerLPF.at<float>(y, x) == 255) {
                point->x0 = x;
                point->y0 = y;
                Insert(*point, circles, 30, voting, minradius, maxradius);
            }
        }
    }
    
    free(point);
}

void DrawCircles (cv::Mat image, ListType *circles, int coindistance, int minradius, int maxradius, int ***voting){
    Pointer q;
    q = circles->First->Next;
    
    while (q != NULL) {
        circle(image, Point(q->Item.x0, q->Item.y0), q->Item.radius, Scalar(0,255,0), 2, 8);
        q = q->Next;
    }
}

void Hough (cv::Mat &direction, cv::Mat &threshold, cv::Mat &image, int t){
    int minradius = 10;
    int maxradius = direction.rows/2;
    int x, y, r;
    
    int kernel[3][3] = {{1,1,1},{1,1,1},{1,1,1}};
    
    Mat houghspace = cv::Mat(direction.rows, direction.cols, CV_32FC1);
    Mat centers = cv::Mat(direction.rows, direction.cols, CV_32FC1);
    Mat centerLPF = cv::Mat(direction.rows, direction.cols, CV_32FC1);
    
    //x,y,r
    int ***voting = allocate3D(direction.cols, direction.rows, maxradius - minradius);
    
    //zero out matrix
    for (y = 0; y < threshold.rows; y++) {
        for (x = 0; x < threshold.cols; x++) {
            for (r = minradius; r < maxradius; r++){
                voting[x][y][r - minradius] = 0;
            }
        }
    }
    
    VoteCenters(threshold, direction, voting, minradius, maxradius);
    
    PrintHoughSpace(voting, houghspace, minradius, maxradius);
    
    ThresholdVotes(voting, centers, minradius, maxradius, t);
    
    LowPassFilter(centers, kernel, centerLPF);
    
    for (y = 0; y < centers.rows; y++) {
        for (x = 0; x < centers.cols; x++) {
            if (centers.at<float>(y, x) > 60)
                centerLPF.at<float>(y, x) = 255;
            else
                centerLPF.at<float>(y, x) = 0;
        }
    }
    
    ListType *circles = (ListType*) malloc(sizeof(ListType));
    MEList(circles);
    
    FindCoinsCenters(centers, centerLPF, circles, voting, minradius, maxradius);
    
    DrawCircles(image, circles, minradius, minradius, maxradius, voting);

    imwrite("CoinsDetected.jpg", image);
    
    deallocate3D(voting, direction.cols, direction.rows);
    
}

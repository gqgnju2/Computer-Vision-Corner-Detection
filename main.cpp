//
//  main.cpp
//  opencvTest
//
//  Created by gaoqingge on 6/03/16.
//  Copyright © 2016 gaoqingge. All rights reserved.
//
#include "global_utils.h"
#include "fastCornerDetector.h"

// Global variables
Mat src;
Mat src_gray;
Mat src_clone_h;
Mat src_clone_f;
Mat src_clone_f_cv;

int thresh_harris = 160;
int thresh_fast = 70;
int max_thresh = 255;

const char *source_window = "Source Image";
const char *corners_h_window = "Harris Corners detector";
const char *corners_f_window = "Fast Corners detector";
const char *corners_f_cv_window = "OpenCV Fast Corners detector";

/// Function header
void cornerFast(int, void*);
void cornerHarris(int, void*);
void cornerFast_opencv(int, void*);


void showStaticPics(String folder) {
    vector<String> filenames;
    glob(folder, filenames);
    
    for(size_t i = 0; i < filenames.size(); ++i)
    {
        if (filenames[i][strlen(filenames[i].c_str())-1] != 'g') {
            continue;
        }
        src = imread(filenames[i], IMREAD_COLOR);
        cvtColor(src, src_gray, CV_BGR2GRAY);
        
        namedWindow(corners_f_cv_window, CV_WINDOW_NORMAL);
        namedWindow(corners_h_window, CV_WINDOW_NORMAL);
        resizeWindow(corners_f_cv_window, 1000, 1500);
        resizeWindow(corners_h_window, 1000, 1500);
        
        createTrackbar("Threshold H: ", corners_h_window, &thresh_harris, max_thresh, cornerHarris);
        createTrackbar("Threshold F: ", corners_h_window, &thresh_fast, max_thresh, cornerFast_opencv);
        
        cornerHarris(0, 0);
        cornerFast_opencv(0, 0);
        
        waitKey(0);
    }
}

void showCamera() {
    Mat frame;
    VideoCapture cap;
    cap.open(0);
    if (!cap.isOpened())
    {
        cout << "Failed to open camera" << endl;
        return;
    }
    
    namedWindow("WebCam Harris:Red Fast:Green", 1);
    cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
    cap >> frame;
    printf("frame size %d %d \n", frame.rows, frame.cols);
    
    int key = 0;
    double fps = 0.0;
    
    while(1) {
        
        system_clock::time_point start = system_clock::now();
        cap >> frame;
        
        if(frame.empty()) {
            break;
        }
        
        //clear the output Mat
        cvtColor(frame, src_gray, CV_RGB2GRAY);
        
        //the code comment below is the fast algorithm implement by ourselves.
//        fastCornerDetector fastDet;
//        fastDet.setThreshold(thresh_fast);
//        vector<Point2i> vec = fastDet.detect(src_gray);
//
//        vector<Point2i>::iterator itr;
//        for (itr = vec.begin(); itr != vec.end(); itr++) {
//            int x = itr->x;
//            int y = itr->y;
//            circle(frame, Point(x, y), 6, Scalar(0, 255, 0), 1, 8, 0);
//        }
        
        //the code comment below is the fast algorithm implement by opencv.
        Ptr<FastFeatureDetector> myBlobDetector = FastFeatureDetector::create(thresh_fast);
        vector<KeyPoint> myBlobs;
        myBlobDetector->detect(src_gray, myBlobs);
        
        vector<KeyPoint>::iterator itr1;
        for (itr1 = myBlobs.begin(); itr1 != myBlobs.end(); itr1++) {
            int x = itr1->pt.x;
            int y = itr1->pt.y;
            circle(frame, Point(x, y), 3, Scalar(0, 255, 0), 1, 8, 0);
        }
        
        
        Mat dst_norm;
        Mat dst_norm_scaled;
        Mat dst = Mat::zeros(frame.size(), CV_32FC1);
        
        // Detector parameters
        int blockSize = 2;
        int apertureSize = 3;
        double k = 0.04;
        
        // Detecting corners
        cornerHarris(src_gray, dst, blockSize, apertureSize, k, BORDER_DEFAULT);
        
        // Normalizing
        normalize(dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
        convertScaleAbs(dst_norm, dst_norm_scaled);
        
        int iCount = 0;
        for(int j = 0; j < dst_norm.rows; j++) {
            for(int i = 0; i < dst_norm.cols; i++) {
                if((int) dst_norm.at<float>(j,i) > thresh_harris) {
                    iCount++;
                }
                if (iCount > dst_norm.rows/4*dst_norm.cols/4) {
                    break;
                }
            }
        }
        
        if (iCount <= dst_norm.rows/4*dst_norm.cols/4) {
            // Drawing a circle around corners
            for(int j = 0; j < dst_norm.rows; j++) {
                for(int i = 0; i < dst_norm.cols; i++) {
                    if((int) dst_norm.at<float>(j,i) > thresh_harris) {
                        circle(frame, Point(i, j), 3, Scalar(0, 0, 255), 1, 8, 0);
                    }
                }
            }
        }
        
        
        char str[256];
        sprintf(str, "%.1f", fps);
        putText(frame, str, Point2i(30, 40), CV_FONT_HERSHEY_PLAIN, 2, Scalar(255, 255, 255), 1);
        
        imshow("WebCam Harris:Red Fast:Green", frame);
        
        key = waitKey(1);
        //either esc or 'q'
        if(key == 113 || key == 27) {
            return;
        }
        
        system_clock::time_point end = system_clock::now();
        double seconds = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        fps = 1000000/seconds;
    }
}

void cornerHarris(int, void*)
{
    src_clone_h = src.clone();
    Mat dst_norm;
    Mat dst_norm_scaled;
    Mat dst = Mat::zeros(src.size(), CV_32FC1);
    
    // Detector parameters
    int blockSize = 2;
    int apertureSize = 3;
    double k = 0.04;
    
    // Detecting corners
    cornerHarris(src_gray, dst, blockSize, apertureSize, k, BORDER_DEFAULT);
    
    // Normalizing
    normalize(dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
    convertScaleAbs(dst_norm, dst_norm_scaled);
    
    int iCount = 0;
    // Drawing a circle around corners
    for(int j = 0; j < dst_norm.rows; j++) {
        for(int i = 0; i < dst_norm.cols; i++) {
            if((int) dst_norm.at<float>(j,i) > thresh_harris) {
                circle(src_clone_h, Point(i, j), 3, Scalar(0, 0, 255), 2, 8, 0);
                iCount++;
            }
        }
    }
    
    char str[256];
    sprintf(str, "threshold: %d  corner count: %d", thresh_harris, iCount);
    putText(src_clone_h, str, Point2i(20, 40), CV_FONT_HERSHEY_PLAIN, 2, Scalar(255, 0, 0), 1);
    
    
    imshow(corners_h_window, src_clone_h);
}


void cornerFast(int, void*)
{
    src_clone_f = src.clone();
    
    fastCornerDetector fastDet;
    fastDet.setThreshold(thresh_fast);
    vector<Point2i> vec = fastDet.detect(src_gray);
    
    int iCount = 0;
    vector<Point2i>::iterator itr;
    for (itr = vec.begin(); itr != vec.end(); itr++) {
        int x = itr->x;
        int y = itr->y;
        circle(src_clone_f, Point(x, y), 3,  Scalar(0, 255, 0), 2, 8, 0);
        iCount++;
    }
    
    char str[256];
    sprintf(str, "threshold: %d  corner count: %d", thresh_fast, iCount);
    putText(src_clone_f, str, Point2i(20, 40), CV_FONT_HERSHEY_PLAIN, 2, Scalar(255, 0, 0), 1);
    
    imshow(corners_f_window, src_clone_f);
}


void cornerFast_opencv(int, void*)
{
    src_clone_f_cv = src.clone();
    
    Ptr<FastFeatureDetector> myBlobDetector = FastFeatureDetector::create(thresh_fast);
    vector<KeyPoint> myBlobs;
    myBlobDetector->detect(src_clone_f_cv , myBlobs);
    
    int iCount = 0;
    vector<KeyPoint>::iterator itr;
    for (itr = myBlobs.begin(); itr != myBlobs.end(); itr++) {
        int x = itr->pt.x;
        int y = itr->pt.y;
        circle(src_clone_f_cv, Point(x, y), 3, Scalar(0, 255, 0), 2, 8, 0);
        iCount++;
    }
    
    /// Showing the result
    char str[256];
    sprintf(str, "threshold: %d  corner count: %d", thresh_fast, iCount);
    putText(src_clone_f_cv, str, Point2i(20, 40), CV_FONT_HERSHEY_PLAIN, 2, Scalar(255, 0, 0), 1);
    imshow(corners_f_cv_window, src_clone_f_cv);
}

void showUsage() {
    printf("Usage\n");
    printf("********************************************************************************************\n");
    printf("./main will start the camera and check every frame\n");
    printf("./main -load ./pictures will load all the images under pictures folder and show the result\n");
    printf("********************************************************************************************\n");
}

int main(int argc, char **argv)
{
    
    
    /* program usage:
     * ./main will start the camera and check every frame
     * ./main -load ./pictures will load all the images under pictures folder and show the result
     */
    
    if (argc == 1) {
        showCamera();
    }
    else if (argc == 3) {
        if( strcmp(argv[1], "-load") == 0) {
            showStaticPics(argv[2]);
        }
        else {
            showUsage();
        }
    }
    else {
        showUsage();
    }
    return 0;
    
}



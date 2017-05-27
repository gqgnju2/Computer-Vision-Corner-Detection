//
//  fastCornerDetector.cpp
//  opencvTest
//
//  Created by gaoqingge on 13/05/16.
//  Copyright © 2016 gaoqingge. All rights reserved.
//

#include "fastCornerDetector.h"
#include <list>

#define RESULT_BRIGHTER      1
#define RESULT_DARKER       -1
#define RESULT_SIMILAR       0

//the y offset of edge pixels
const int offset_y[] = {
    -3, -3, -2, -1,  0,
     1,  2,  3,  3,  3,
     2,  1,  0, -1, -2,
    -3
};

//the x offset of edge pixels
const int offset_x[] = {
     0,  1,  2,  3,  3,
     3,  2,  1,  0, -1,
    -2, -3, -3, -3, -2,
    -1
};

fastCornerDetector::fastCornerDetector() {
    //set the default value of threshold
    m_threshold = 30;
}

fastCornerDetector::~fastCornerDetector() {
    
}

void fastCornerDetector::setThreshold(int newValue) {
    m_threshold = newValue;
}

vector<Point2i> fastCornerDetector::detect(Mat &image) {
    
    vector<Point2i> vecResult;
    
    //the input image must be gray scaled
    if (image.channels() == 3) {
        cout << "input image with 3 channels, cannot parse" << endl;
        return vecResult;
    }
    
    for (int y = 3;  y < image.rows - 3; y++) {
        for (int x = 3;  x < image.cols - 3; x++) {
            
            //first check the key pixels
            if (false == checkKeyPixel(image, x, y)) {
                continue;
            }
            
            //continue check all the pixels if necessary
            if (checkContiguous(image, x, y)) {
                vecResult.push_back(Point2i(x, y));
            }
        }
    }
    
    return vecResult;
}

int fastCornerDetector::checkPixel(Mat &image, int x, int y, int inumber, int pValue) {
    
    int iResult = 0;
    
    //the edge pixel's value
    int nValue = Mpixel(image, x + offset_x[inumber], y + offset_y[inumber]);
    if (pValue + m_threshold < nValue) {
        iResult = RESULT_BRIGHTER;
    }
    else if (pValue - m_threshold > nValue) {
        iResult = RESULT_DARKER;
    }
    else {
        iResult = RESULT_SIMILAR;
    }
    return iResult;
}

bool fastCornerDetector::checkKeyPixel(Mat &image, int x, int y) {
    
    //the center pixel's value
    int pValue = Mpixel(image, x, y);
    
    int pixel0 = checkPixel(image, x, y, 0, pValue);
    int pixel8 = checkPixel(image, x, y, 8, pValue);
    
    //return false if any two of them is similar to center pixel
    if (pixel0 == RESULT_SIMILAR && pixel8 == RESULT_SIMILAR) {
        return false;
    }
    
    int pixel4 = checkPixel(image, x, y, 4, pValue);
    int pixel12 = checkPixel(image, x, y, 12, pValue);
    
    if (pixel4 == RESULT_SIMILAR && pixel12 == RESULT_SIMILAR) {
        return false;
    }
    
    // 1 5 9 13 1 5
    result[0] = pixel0;
    result[1] = pixel4;
    result[2] = pixel8;
    result[3] = pixel12;
    result[4] = pixel0;
    result[5] = pixel4;
    
    //at least three of them is the same status(brighter or darker)
    for (int i = 0; i < 4; i++) {
        if (result[i] == result[i+1] &&
            result[i+2] == result[i+1]) {
            //put these values to all pixel array, reusing them
            m_pixelValues[0] = pixel0;
            m_pixelValues[4] = pixel4;
            m_pixelValues[8] = pixel8;
            m_pixelValues[12] = pixel12;
            return true;
        }
    }
    return false;
}


bool fastCornerDetector::checkContiguous(Mat &image, int x, int y) {
    int iSum = 0;
    
    //the center pixel's value
    int pValue = Mpixel(image, x, y);
    
    //step1: get all the pixel's status value
    for (int i = 0; i < 16; i++) {
        if (i == 0 || i == 4 ||
            i == 8 || i == 12) {
            
        }
        else {
            m_pixelValues[i] = checkPixel(image, x, y, i, pValue);
        }
        iSum += m_pixelValues[i];
    }
    
    //there must be 12 contiguous pixels
    //if sum of pixel status value is smaller than 8
    //or bigger than -8, there's no possible
    if (iSum < 8 && iSum > -8) {
        return false;
    }
    
    //repeat the value to get one circle
    for (int i = 16; i < 27; i++) {
        m_pixelValues[i] = m_pixelValues[16-i];
    }
    
    iSum = 0;
    
    //get the sum of first 12 pixels
    for (int i = 0; i < 12; i++) {
        iSum += m_pixelValues[i];
    }
    
    if (iSum == 12 || iSum == -12) {
        return true;
    }
    
    for (int i = 12; i < 27; i++) {
        iSum -= m_pixelValues[i-12];
        iSum += m_pixelValues[i];
        if (iSum == 12 || iSum == -12) {
            return true;
        }
    }
    return false;
}
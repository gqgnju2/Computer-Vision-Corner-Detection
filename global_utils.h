//
//  global_utils.h
//  opencvTest
//
//  Created by gaoqingge on 21/03/16.
//  Copyright © 2016 gaoqingge. All rights reserved.
//

#ifndef global_utils_h
#define global_utils_h

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <iostream>
#include <chrono>

using namespace cv;
using namespace std;
using namespace chrono;

#define pixelB(image, x, y) \
image.data[image.step[0]*y+image.step[1]*x]

#define pixelG(image, x, y) \
image.data[image.step[0]*y+image.step[1]*x+1]

#define pixelR(image, x, y) \
image.data[image.step[0]*y+image.step[1]*x+2]

#define Mpixel(image,x,y) ( (uchar*) ( ((image).data) + (y)*((image).step) ) ) [(x)]
#define MpixelB(image,x,y) ( (uchar*) ( ((image).data) + (y)*((image).step) ) ) [(x)*((image).channels())]
#define MpixelG(image,x,y) ( (uchar*) ( ((image).data) + (y)*((image).step) ) ) [(x)*((image).channels())+1]
#define MpixelR(image,x,y) ( (uchar*) ( ((image).data) + (y)*((image).step) ) ) [(x)*((image).channels())+2]

#endif /* global_utils_h */

//
//  fastCornerDetector.hpp
//  opencvTest
//
//  Created by gaoqingge on 13/05/16.
//  Copyright © 2016 gaoqingge. All rights reserved.
//

#ifndef fastCornerDetector_h
#define fastCornerDetector_h

#include <stdio.h>
#include "global_utils.h"

class fastCornerDetector {
    
public:
    
    fastCornerDetector();
    ~fastCornerDetector();
    
    /**
     *  set the threshold with given value
     *
     *  @param newValue threshold value
     */
    void setThreshold(int newValue);
    
    /**
     *  check the given pixel's brightness
     *
     *  @param image   mat image
     *  @param x       x value of the center pixel
     *  @param y       y value of the center pixel
     *  @param inumber the number of the checking pixel
     *
     *  @return brighter, darker or similiar
     */
    int checkPixel(Mat &image, int x, int y, int inumber, int pValue);
    
    
    /**
     *  check key pixel brightness. 1, 9, 5, 13
     *
     *  @param image mat image
     *  @param x     x value of the center pixel
     *  @param y     y value of the center pixel
     *
     *  @return continue checking other pixel or not
     */
    bool checkKeyPixel(Mat &image, int x, int y);
    
    
    /**
     *  check if the pxiel is corner or not
     *
     *  @param image mat image
     *  @param x     x value of the center pixel
     *  @param y     y value of the center pixel
     *
     *  @return if the pixel is corner or not
     */
    bool checkContiguous(Mat &image, int x, int y);
    
    /**
     *  detect corners in the image
     *
     *  @param image mat image
     *
     *  @return corner pixel vector
     */
    vector<Point2i> detect(Mat &image);
    
    
private:
    int m_threshold;
    int m_pixelValues[28];
    int result[7];
};

#endif /* fastCornerDetector_h */

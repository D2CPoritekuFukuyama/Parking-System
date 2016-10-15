//
//  templateMatch.cpp
//  openCVtest1
//
//  Created by 石川諒 on 2016/10/15.
//  Copyright © 2016年 石川諒. All rights reserved.
//

#include "templateMatch.hpp"
#define NUMBER 10

using namespace cv;
using namespace std;

void TemplateMatch::Matching(){
    CvPoint min_loc;
    CvPoint max_loc;
    double min_val;
    double max_val;
    IplImage *templateImage;
    IplImage *src_bin = cvLoadImage("image/Nplate-down.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    IplImage *differenceMapImage;
    String filename;
    stringstream ss;
    cvThreshold(src_bin, src_bin, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
    for(int i = 0; i < NUMBER; i ++){
        ss << "image/" << i << ".jpg";
        filename = ss.str();
        templateImage  = cvLoadImage(filename.c_str(), CV_LOAD_IMAGE_GRAYSCALE);
        differenceMapImage = cvCreateImage(cvSize(src_bin -> width - templateImage -> width + 1, src_bin -> height - templateImage -> height + 1), IPL_DEPTH_32F, 1);
        
        cvMatchTemplate(src_bin, templateImage, differenceMapImage, CV_TM_CCOEFF_NORMED);
        cvMinMaxLoc(differenceMapImage, &min_val, &max_val, &min_loc, &max_loc, NULL);
        if(max_val >= 0.7)
        {
            cvRectangle(src_bin, max_loc, cvPoint(max_loc.x + templateImage->width, max_loc.y +templateImage  -> height),CV_RGB(255, 0, 0),-1,8,0);
            cvSaveImage("image/test.jpg", src_bin);
            --i;
        }
        
        ss.str("");

    }
}

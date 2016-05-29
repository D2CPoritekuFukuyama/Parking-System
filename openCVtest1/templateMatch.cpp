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

void TemplateMatch::location_sort(){
    int tmp1,tmp2;
    for(int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 4; j ++) {
            if(location[j][0] > location[j + 1][0])
            {
                tmp1 = location[j][0];
                tmp2 = location[j][1];
                location[j][0] = location[j + 1][0];
                location[j][1] = location[j + 1][1];
                location[j + 1][0] = tmp1;
                location[j + 1][1] = tmp2;
            }
        }

    }
}

void TemplateMatch::Matching(){
    CvPoint max_loc;
    double max_val;
    int count=0; //locationの代入場所指定用
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
        cvMinMaxLoc(differenceMapImage, NULL, &max_val, NULL, &max_loc, NULL);
        if(max_val >= 0.68)
        {
            cvRectangle(src_bin, max_loc, cvPoint(max_loc.x + templateImage->width, max_loc.y +templateImage  -> height),CV_RGB(255, 0, 0),-1,8,0);
            location[count][0] = max_loc.x;
            location[count][1] = i;
            cvSaveImage("image/test.jpg", src_bin);
            --i;
            ++ count;
        }
        ss.str("");
    }
    location_sort(); //各数字の位置を昇順に並べ替え
}

//
//  NumberMatching.cpp
//  openCVtest1
//
//  Created by 石川諒 on 2016/11/02.
//  Copyright © 2016年 石川諒. All rights reserved.
//

#include "NumberMatching.hpp"

#define NUMBER 10

using namespace std;
using namespace cv;


NumberMatching::NumberMatching(IplImage *target_img) : Matching(target_img){
    
}

string NumberMatching::Number_Matching(){
    CvPoint max_loc; //マッチした場所を格納
    double max_val; //マッチング率
    int count=0; //locationの代入場所指定用
    IplImage *templateImage; //テンプレート画像
    IplImage *differenceMapImage; //探索結果
    string filename;
    stringstream ss; //file名作成用
    
    //数字のマッチング
    for(int i = 0; i < NUMBER; i ++){
        ss << "image/Number/" << i << ".jpg";
        filename = ss.str();
        templateImage  = cvLoadImage(filename.c_str(), CV_LOAD_IMAGE_GRAYSCALE);
        differenceMapImage = cvCreateImage(cvSize(target_img -> width - templateImage -> width + 1, target_img -> height - templateImage -> height + 1), IPL_DEPTH_32F, 1);
        //テンプレートマッチング
        cvMatchTemplate(target_img, templateImage, differenceMapImage, CV_TM_CCOEFF_NORMED);
        //一番マッチした場所と値の格納
        cvMinMaxLoc(differenceMapImage, NULL, &max_val, NULL, &max_loc, NULL);
        if(max_val >= 0.68)
        {
            //マッチした場所を塗りつぶす
            cvRectangle(target_img, max_loc, cvPoint(max_loc.x + templateImage->width, max_loc.y +templateImage  -> height),CV_RGB(255, 0, 0),-1,8,0);
            num_map[max_loc.x] = i;
            cvSaveImage("image/test.jpg", target_img);
            //同じ数字をもう一度検索
            --i;
            ++ count;
        }
        ss.str("");
    }
    
    return get_num();
}
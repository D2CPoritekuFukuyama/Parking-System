//
//  HiraganaMatching.cpp
//  openCVtest1
//
//  Created by 石川諒 on 2016/11/04.
//  Copyright © 2016年 石川諒. All rights reserved.
//

#include "HiraganaMatching.hpp"
#define HIRAGANA_NUMBER 3

using namespace std;

HiraganaMatching::HiraganaMatching(IplImage *target_img) : Matching(target_img){

}

string HiraganaMatching::Hiragana_Matching(){
    CvPoint max_loc; //マッチした場所を格納
    double max_val; //マッチング率
    string template_name ="atowa"; //ひらがな
    string result;
    IplImage *templateImage; //テンプレート画像
    IplImage *differenceMapImage; //探索結果
    string filename;
    stringstream ss; //file名作成用
    for(int i = 0; i < HIRAGANA_NUMBER; i++)
    {
        ss << "image/hiragana/template_" << i << ".jpg";
        filename = ss.str();
        templateImage  = cvLoadImage(filename.c_str(), CV_LOAD_IMAGE_GRAYSCALE);
        if (templateImage == NULL) {
            break;
        }
        differenceMapImage = cvCreateImage(cvSize(target_img -> width - templateImage -> width + 1, target_img -> height - templateImage -> height + 1), IPL_DEPTH_32F, 1);
        //テンプレートマッチング
        cvMatchTemplate(target_img, templateImage, differenceMapImage, CV_TM_CCOEFF_NORMED);
        cvMinMaxLoc(differenceMapImage, NULL, &max_val, NULL, &max_loc, NULL);
        if(max_val >= 0.6)
        {
            cvRectangle(target_img, max_loc, cvPoint(max_loc.x + templateImage->width, max_loc.y +templateImage  -> height),CV_RGB(255, 0, 0),-1,8,0);
            cvSaveImage("image/test.jpg", target_img);
            if (i != 0){
                ss.str("");
                ss << template_name[i] << template_name[i+1];
                result = ss.str();
                i++;}
            else
                result = template_name[i];
            return result;
            
        }
        ss.str("");
    }
    return " ";
}
//
//  templateMatch.cpp
//  openCVtest1
//
//  Created by 石川諒 on 2016/10/15.
//  Copyright © 2016年 石川諒. All rights reserved.
//

#include "templateMatch.hpp"
#define NUMBER 10
#define HIRAGANA_NUMBER 31


using namespace cv;
using namespace std;


//マッチングした順に格納された数字をx座標をもとに並べ替え
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

int TemplateMatch::Number_Matching(IplImage *src_img){
    CvPoint max_loc; //マッチした場所を格納
    double max_val; //マッチング率
    int count=0; //locationの代入場所指定用
    IplImage *templateImage; //テンプレート画像
    IplImage *differenceMapImage; //探索結果
    String filename;
    stringstream ss; //file名作成用
    int result;
    cvThreshold(src_img, src_img, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU); //2値化
    //数字のマッチング
    for(int i = 0; i < NUMBER; i ++){
        ss << "image/Number/" << i << ".jpg";
        filename = ss.str();
        
        templateImage  = cvLoadImage(filename.c_str(), CV_LOAD_IMAGE_GRAYSCALE);
        differenceMapImage = cvCreateImage(cvSize(src_img -> width - templateImage -> width + 1, src_img -> height - templateImage -> height + 1), IPL_DEPTH_32F, 1);
        //テンプレートマッチング
        cvMatchTemplate(src_img, templateImage, differenceMapImage, CV_TM_CCOEFF_NORMED);
        //一番マッチした場所と値の格納
        cvMinMaxLoc(differenceMapImage, NULL, &max_val, NULL, &max_loc, NULL);
        if(max_val >= 0.68)
        {
            //マッチした場所を塗りつぶす
            cvRectangle(src_img, max_loc, cvPoint(max_loc.x + templateImage->width, max_loc.y +templateImage  -> height),CV_RGB(255, 0, 0),-1,8,0);
            location[count][0] = max_loc.x; //マッチした場所のx座標の格納
            location[count][1] = i; //ナンバーの格納
            cvSaveImage("image/test.jpg", src_img);
            //同じ数字をもう一度検索
            --i;
            ++ count;
        }
        ss.str("");
    }
    location_sort(); //各数字の位置を昇順に並べ替え
    ss << location[0][1] << location[1][1] << location[2][1] << location[3][1];
    ss >> result;
    return result;
}

char TemplateMatch::Hiragana_Matching(IplImage *src_img){
    CvPoint max_loc; //マッチした場所を格納
    double max_val; //マッチング率
    string template_name ="atowa"; //ひらがな
    char result;
    IplImage *templateImage; //テンプレート画像
    IplImage *differenceMapImage; //探索結果
    String filename;
    stringstream ss; //file名作成用
    cvThreshold(src_img, src_img, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU); //2値化
    for(int i = 0; i < HIRAGANA_NUMBER; i++)
    {
        ss << "image/hiragana/template_" << i << ".jpg";
        filename = ss.str();
        templateImage  = cvLoadImage(filename.c_str(), CV_LOAD_IMAGE_GRAYSCALE);
        if (templateImage == NULL) {
            break;
        }
        differenceMapImage = cvCreateImage(cvSize(src_img -> width - templateImage -> width + 1, src_img -> height - templateImage -> height + 1), IPL_DEPTH_32F, 1);
        //テンプレートマッチング
        cvMatchTemplate(src_img, templateImage, differenceMapImage, CV_TM_CCOEFF_NORMED);
        //一番マッチした場所と値の格納
        cvMinMaxLoc(differenceMapImage, NULL, &max_val, NULL, &max_loc, NULL);
        if(max_val >= 0.6)
        {
            cvRectangle(src_img, max_loc, cvPoint(max_loc.x + templateImage->width, max_loc.y +templateImage  -> height),CV_RGB(255, 0, 0),-1,8,0);
            cvSaveImage("image/test.jpg", src_img);
            
            result = template_name[i];
            return result;
            
        }
        ss.str("");
    }
    return NULL;
    
}

void TemplateMatch::Matching(){
    IplImage *src_img1 = cvLoadImage("image/Nplate-down.jpg", CV_LOAD_IMAGE_GRAYSCALE); //探索対象画像
    IplImage *src_img2 = src_img1;
    char hiragana;
    cvThreshold(src_img1, src_img1, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU); //2値化
    cvThreshold(src_img2, src_img2, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU); //2値化
    hiragana = Hiragana_Matching(src_img2);
    printf("%c %d\n", hiragana, Number_Matching(src_img1));

}

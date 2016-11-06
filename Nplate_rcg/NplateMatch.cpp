//
//  templateMatch.cpp
//  openCVtest1
//
//  Created by 石川諒 on 2016/10/15.
//  Copyright © 2016年 石川諒. All rights reserved.
//

#include "NplateMatch.hpp"
#include "Matching/NumberMatching.hpp"
#include "Matching/CateNumMatching.hpp"
#include "Matching/HiraganaMatching.hpp"

#define NUMBER 10
#define HIRAGANA_NUMBER 3


using namespace cv;
using namespace std;

string NplateMatch::Matching(){
    stringstream ss; //全結果を格納
    IplImage *src_img1 = cvLoadImage("image/Nplate-down.jpg", CV_LOAD_IMAGE_GRAYSCALE); //探索対象画像
    IplImage *category_img = cvLoadImage("image/Nplate-cate.jpg", CV_LOAD_IMAGE_GRAYSCALE); //探索対象画像
    NumberMatching numberMatching = NumberMatching(src_img1); //数字認識のNumberMatchingの初期化
    CateNumMatching cateNumMathing = CateNumMatching(category_img);//CateNumMatchingの初期化
    HiraganaMatching hiraganaMatching = HiraganaMatching(src_img1);//HiraganaMatchingの初期化
    number = numberMatching.Number_Matching();//ナンバーの取得
    cate_num = cateNumMathing.CategoryNum_Matching();//分類番号の取得
    hiragana = hiraganaMatching.Hiragana_Matching();
    
    ss << hiragana << " " << number << " " << cate_num;
    
    return ss.str();

}

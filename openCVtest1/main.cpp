//
//  main.cpp
//  openCVtest1
//
//  Created by 石川諒 on 2016/10/01.
//  Copyright © 2016年 石川諒. All rights reserved.
//

#include "cv.h"
#include "highgui.h"

using namespace cv;

void cv_Labelling(IplImage *src_img, IplImage *dst_img);
void GetContourFeature(CvSeq *Contour);

int main (int argc, char **argv)
{
    IplImage *img = cvLoadImage("image/nPlate3.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    IplImage *bin_img1 = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
    IplImage *bin_img2 = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
    IplImage *bin_img = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
    IplImage *resutl_img = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 3);
    
    Mat test = img;
    IplImage test1 = test;
    //cvErode(&test1, &test1);
    //GaussianBlur(test, test, Size(1,1), 0);
    cvShowImage("test", &test1);
    //cvLaplace(&test1, bin_img,3);
    cvThreshold(&test1, bin_img1, 165, 255, CV_THRESH_BINARY);
    cvAdaptiveThreshold(&test1, bin_img2, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY);
    //cvCanny(&test1, bin_img2, 50, 20);
    cvAnd(bin_img1, bin_img2, bin_img);
    //cvDilate(bin_img, bin_img);
    //cvMorphologyEx(bin_img, bin_img, NULL, NULL, CV_MOP_OPEN,2);
    cvShowImage("tes1", bin_img2);
    cvShowImage("test3", bin_img);
    cv_Labelling(bin_img, resutl_img);
    
    cvShowImage("test2", resutl_img);
    
    waitKey(0);
    return 0;
}

void DrawNextContour(
                     IplImage *img,//ラベリング結果を描画するIplImage(8Bit3chカラー）
                     CvSeq *Contour, //輪郭へのポインタ
                     int Level //輪郭のレベル（階層）
){
    // 輪郭を描画する色の設定
    CvScalar ContoursColor;
    
    if ((Level % 2) == 1){
        //白の輪郭の場合、赤で輪郭を描画
        ContoursColor = CV_RGB( 255, 0, 0 );
    }else{
        //黒の輪郭の場合、青で輪郭を描画
        ContoursColor = CV_RGB( 0, 0, 255 );
    }
    

    for (; Contour != 0; Contour = Contour ->h_next) {
       //輪郭のポリゴン近似
        CvSeq *approx = cvApproxPoly(Contour, sizeof(CvContour), NULL, CV_POLY_APPROX_DP, 20); //25
       /* cvDrawContours( img, approx, ContoursColor, ContoursColor, 0, 2);
        if (Contour -> h_next != NULL)
            DrawNextContour(img, approx->h_next, Level);
        */
        
        //面積
        double Area = fabs(cvContourArea(Contour, CV_WHOLE_SEQ));
        
        if(approx->total == 4 && Area >= 1000 && Area <= 6000) //頂点が4で面積が100以上なら描画
        {
            cvDrawContours( img, approx, ContoursColor, ContoursColor, 0, 2);
         
            if (Contour -> h_next != NULL)
                DrawNextContour(img, approx->h_next, Level);
        }
        
        

    }
 

    //輪郭の描画
    /*cvDrawContours( img, Contour, ContoursColor, ContoursColor, 0, 2);
    //各種輪郭の特徴量の取得
    GetContourFeature(Contour); //←オリジナル関数です。（詳細は後述）
    
    if (Contour->h_next != NULL)
        //次の輪郭がある場合は次の輪郭を描画
        DrawNextContour(img, Contour->h_next, Level);
    */
}



void cv_Labelling(IplImage *src_img, IplImage *dst_img){
    CvMemStorage *storage = cvCreateMemStorage(0);
    CvSeq *contours = NULL;
    
    if(src_img == NULL)
        return;
    
    int contour_count = cvFindContours(src_img,
                                       storage,
                                       &contours,
                                       sizeof(CvContour),
                                       CV_RETR_EXTERNAL,
                                       CV_CHAIN_APPROX_SIMPLE);
    if (contour_count <= 0)
        return ;
    
    if(contours != NULL){
        cvSet(dst_img, CV_RGB(0, 0, 0));
        DrawNextContour(dst_img, contours, 1);
    }
    
    cvReleaseMemStorage(&storage);
}

//各種輪郭の特徴量の取得
void GetContourFeature(CvSeq *Contour){
    //面積
    double Area = fabs(cvContourArea(Contour, CV_WHOLE_SEQ));
    //周囲長
    double Perimeter = cvArcLength(Contour);
    //円形度
    double CircleLevel = 4.0 * CV_PI * Area / (Perimeter * Perimeter);
    
    //傾いていない外接四角形領域（フィレ径）
    CvRect rect = cvBoundingRect(Contour);
    //輪郭を構成する頂点座標を取得
    for ( int i = 0; i < Contour->total; i++){
        CvPoint *point = CV_GET_SEQ_ELEM (CvPoint, Contour, i);
    }
}

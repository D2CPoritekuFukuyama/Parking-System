//
//  main.cpp
//  openCVtest1
//
//  Created by 石川諒 on 2016/10/01.
//  Copyright © 2016年 石川諒. All rights reserved.
#include "stdio.h"
#include <opencv/cv.hpp>
#include <opencv/highgui.h>
#include "labeling.hpp"
#include "Nplate_trim.hpp"
#include "trimming.hpp"

#define VISUAL
//#define DEBUG_LABELING
using namespace cv;

int main (int argc, char **argv)
{
    string result;
    Trimming trimming = Trimming(); //ナンバープレート検出のクラス

    //webカメラ用window生成
#ifdef VISUAL
    cvNamedWindow("webCamera", CV_WINDOW_AUTOSIZE);
#endif
    
    while (1) {


        trimming.get_plateData();
#ifdef DEBUG_LABELING
        if (cvWaitKey(1) == 0x71) {
            break;
        }
#endif
#if !defined DEBUG_LABELING
            //locationのゼロクリア
#endif
        
        
    }

    //cvReleaseCapture(&labeling.videoCapture);
    //cvDestroyAllWindows();
    return 0;
}

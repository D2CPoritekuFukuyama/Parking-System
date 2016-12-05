//
//  trimming.cpp
//  openCVtest1
//
//  Created by 石川諒 on 2016/12/04.
//  Copyright © 2016年 石川諒. All rights reserved.
//
#include "trimming.hpp"
//#define NPLATE_TRIM_DBUG

Trimming::Trimming(){
    nplate_trim = Nplate_trim(); //ナンバープレート検出のクラス
    elem_trimming = Elem_trimming();
}

int Trimming::get_plateData(){
    nplate_trim.frame = cvQueryFrame(nplate_trim.videoCapture);
    if(nplate_trim.videoCapture  == NULL )
        return -1;
    
    //cvShowImage("webCamera", nplate_trim.frame);
    if (nplate_trim.get_Nplate() != 0 || nplate_trim.result_img == NULL)
        return -1;
    //cvShowImage("nplate_down", nplate_trim.result_img);
#ifndef NPLATE_TRIM_DBUG
    
    elem_trimming.get_elem(nplate_trim.result_img);
#endif
    nplate_trim.frame = cvQueryFrame(nplate_trim.videoCapture);
    return 0;
}
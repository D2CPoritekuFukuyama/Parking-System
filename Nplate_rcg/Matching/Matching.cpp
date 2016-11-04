//
//  Matching.cpp
//  openCVtest1
//
//  Created by 石川諒 on 2016/11/01.
//  Copyright © 2016年 石川諒. All rights reserved.
//

#include "Matching.hpp"
#include "stdlib.h"
using namespace std;
string Matching::get_num(){
    stringstream ss; //結果の格納
    map<int, int>::iterator it = num_map.begin();
    while( it != num_map.end() )
    {
        ss << (*it).second;
        ++it;
    }
    
    return ss.str();
}

Matching::Matching(IplImage *src_img){
    target_img = src_img;
    cvThreshold(src_img, target_img, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU); //2値化
}


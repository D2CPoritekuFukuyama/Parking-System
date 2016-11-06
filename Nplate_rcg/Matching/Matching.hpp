//
//  Matching.hpp
//  openCVtest1
//
//  Created by 石川諒 on 2016/11/01.
//  Copyright © 2016年 石川諒. All rights reserved.
//

#ifndef Matching_hpp
#define Matching_hpp

#include <stdio.h>

#include <stdio.h>
#include "cv.h"
#include "highgui.h"

using namespace std;

class Matching{
protected:
    string get_num();
    map<int, int> num_map;
public:
    Matching(IplImage *src_img);
    IplImage *target_img;
};

#endif /* Matching_hpp */

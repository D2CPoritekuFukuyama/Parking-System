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

class Matching{
private:
    void binarization();
public:
    Matching(IplImage *src_img);
    IplImage *target_img;
};

#endif /* Matching_hpp */

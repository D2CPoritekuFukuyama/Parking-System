//
//  CateNumMatching.hpp
//  openCVtest1
//
//  Created by 石川諒 on 2016/11/02.
//  Copyright © 2016年 石川諒. All rights reserved.
//

#ifndef CateNumMatching_hpp
#define CateNumMatching_hpp

#include <stdio.h>
#include "Matching.hpp"

class CateNumMatching:public Matching{
private:
public:
    CateNumMatching(IplImage *target_img);
    string CategoryNum_Matching();
};

#endif /* CateNumMatching_hpp */

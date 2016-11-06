//
//  HiraganaMatching.hpp
//  openCVtest1
//
//  Created by 石川諒 on 2016/11/04.
//  Copyright © 2016年 石川諒. All rights reserved.
//

#ifndef HiraganaMatching_hpp
#define HiraganaMatching_hpp

#include <stdio.h>
#include "Matching.hpp"

class HiraganaMatching : public Matching{
public:
    HiraganaMatching(IplImage *src_img);
    std::string Hiragana_Matching();
};

#endif /* HiraganaMatching_hpp */

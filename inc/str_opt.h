/**
 * utils.h
 *
 * Author: wushiwei
 * Created on: 2015-02-15
 *
 * Copyright (c) 2015 wushiwei. All Rights Reserved
 *
 */

#ifndef __STR_OPT_H
#define __STR_OPT_H

#include "utils.h"

namespace semantic {

inline void split2chs(const char* uttr, std::vector<std::string>& charactors) {
    int len = strlen(uttr);
    int idx = 0;
    while (idx < len) {
        if (uttr[idx] > 0) {
            std::string str(uttr, idx, 1);
            ++idx;
            charactors.push_back(str);
        } else {
            if (idx + 2 > len) {
                break;
            }
            std::string str(uttr, idx, 2);
            charactors.push_back(str);
            idx += 2;
        }
    }
}

}

#endif // __STR_OPT_H

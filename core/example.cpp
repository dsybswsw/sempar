/**
 * example.cpp
 *
 * Author: wushiwei
 * Created on: 2015-02-15
 *
 * Copyright (c) Baidu.com, Inc. All Rights Reserved
 *
 */

#include "example.h"

namespace semantic {
using std::string;
using std::vector;

Example::Example(ULONG index, const string& uttr, 
        vector<string>& tokens, int nbest) {
    _index = index;
    _uttrance = uttr;
    _nbest = nbest;
    _tokens = tokens;
}

string Example::get_token(int idx) {
    return _tokens[idx];
}

}

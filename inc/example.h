/**
 * formula.h
 *
 * Author: wushiwei
 * Created on: 2015-12-15
 *
 * Copyright (c) Baidu.com, Inc. All Rights Reserved
 *
 */

#ifndef SEMANTIC_EXAMPLE_H
#define SEMANTIC_EXAMPLE_H

#include "formula.h"

namespace semantic {
class Example {
public:
    Example() : _index(0), _uttrance(""), _nbest(1) {}
    Example(ULONG index, const std::string& uttr, 
        std::vector<std::string>& tokens, int nbest);

    std::string get_token(int idx);
public:
    // input information.
    ULONG _index;
    std::string _uttrance;
    std::vector<std::string> _tokens;

    // result information.
    int _nbest;
    std::vector<Json::Value> _trees;
    std::vector<Derivation*> _parsing_derivs;

    // training infomation.
    float_t _target_val;
    size_t target_hash_;
    Json::Value _target_tree_json;
};
}

#endif // SEMANTIC_EXAMPLE_H


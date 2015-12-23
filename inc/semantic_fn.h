/**
 * semantic_fn.h
 *
 * Author: wushiwei
 * Created on: 2015-02-15
 *
 * Copyright (c) Baidu.com, Inc. All Rights Reserved
 *
 */

#ifndef SEMANTIC_SEMANTIC_FN_H 
#define SEMANTIC_SEMANTIC_FN_H

#include "example.h"

namespace semantic {
class SemanticFn {
public:
    SemanticFn(std::string& cat, int start, int end, Rule* rule, 
        std::vector<Derivation*>& children) :
            _category(cat), _start(start), _end(end), _rule(rule) {
        _children = children; 
    }

    ~SemanticFn() {
        _rule = NULL;
    }

    virtual std::vector<Derivation*> call(Example& ex);
public:
    std::string _category;
    int _start;
    int _end;
    Rule* _rule;
    std::vector<Derivation*> _children;
};
}

#endif // SEMANTIC_SEMANTIC_FN_H


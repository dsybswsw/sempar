/**
 * semantic_fn.cpp
 *
 * Author: wushiwei
 * Created on: 2015-12-15
 *
 * Copyright (c) Baidu.com, Inc. All Rights Reserved
 *
 */

#include "semantic_fn.h"

namespace semantic {
using std::string;
using std::vector;

vector<Derivation*> SemanticFn::call(Example& ex) {
    Derivation* deriv = new Derivation();
    deriv->_cat = _category;
    deriv->_start  = _start;
    deriv->_end = _end;
    deriv->_rule = _rule;
    deriv->_children = _children;
    vector<Derivation*> derivs;
    derivs.push_back(deriv);
    return derivs;
}

}


/**
 * trie.h
 *
 * Author: wushiwei
 * Created on: 2015-02-15
 *
 * Copyright (c) wushiwei 2015. All Rights Reserved
 *
 */

#include "trie.h"

namespace semantic {

using std::string;
using std::vector;

Trie::~Trie() {
    _rules.clear();
    _categories.clear();
    HashMap<string, Trie*>::iterator it = _children.begin();
    for (; it != _children.begin(); ++it) {
        if (it->second != NULL) {
            delete it->second;
            it->second = NULL;
        }
    }
    _children.clear();
}

void Trie::add(Rule* rule) {
    // add to root node.
    add(rule, 0);
}

void Trie::add(Rule* rule, int index) {
    if (rule == NULL) {return;}
    _categories.insert(rule->get_lhs());
    vector<string>& rhs = rule->get_rhs();
    if (index < 0 || index > rhs.size()) {return;}
    if ((int)rhs.size() == index) {
        _rules.push_back(rule);
        return;
    }
    string item = rhs[index];
    if (_children.count(item) == 0) {
        _children[item] = new Trie();
    }
    _children[item]->add(rule, index + 1);
}

Trie* Trie::next(const string& cat) {
    if (_children.count(cat) == 0) {
        return NULL;
    }
    return _children[cat];
}

}


/**
 * grammar.h
 *
 * Author: wushiwei
 * Created on: 2015-02-15
 *
 * Copyright (c) Baidu.com, Inc. All Rights Reserved
 *
 */

#ifndef __GRAMMAR_H_
#define __GRAMMAR_H_

#include "trie.h"

namespace semantic {

// set of rules.
class Grammar {
public:
    Grammar() : _trie(NULL) {}
    ~Grammar();
    int read(const char* gram_file);
    std::vector<Rule*>& get_all_rules();
    Trie* get_trie();
private:
    Trie* build_trie();

    std::vector<Rule*> _rules;
    Trie* _trie;
};

}

#endif


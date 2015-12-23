/**
 * trie.h
 *
 * Author: wushiwei
 * Created on: 2015-02-15
 *
 * Copyright (c) wushiwei. All Rights Reserved
 *
 */

#ifndef SEMANTIC_TRIE_H 
#define SEMANTIC_TRIE_H 

#include "formula.h"

namespace semantic {

class Trie {
public:
    Trie() {}
    ~Trie();

    void add(Rule* rule);
    void add(Rule* rule, int index);
    Trie* next(const std::string& cat);
public:
    // not the owner of rules.
    std::vector<Rule*> _rules;
    HashSet<std::string> _categories;

    // not the owner of sub-tries;
    HashMap<std::string, Trie*> _children;
};

}

#endif // SEMANTIC_TRIE_H


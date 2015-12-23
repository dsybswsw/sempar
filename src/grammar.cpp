/**
 * grammar.h
 *
 * Author: wushiwei
 * Created on: 2015-02-15
 *
 * Copyright (c) Baidu.com, Inc. All Rights Reserved
 *
 */

#include "grammar.h"

namespace semantic {

using std::vector;
using std::string;

Grammar::~Grammar() {
    if (_trie != NULL) {
        delete _trie;
        _trie = NULL;
    }
}

int Grammar::read(const char* gram_file) {
    FILE* fp = fopen(gram_file, "r");
    if (fp == NULL) {
        FATAL("Failed to open gram file %s", gram_file);
        return -1;
    }
    char buff[MAX_LINE_LEN];
    while (fgets(buff, MAX_LINE_LEN, fp)) {
        string line(buff);
        boost::algorithm::trim(line);
        Rule* rule = new Rule();
        int ret = rule->init(line);
        if (ret < 0) {
            FATAL("Failed to init grammar %s", line.c_str());
            delete rule;
            return -1;
        }
        _rules.push_back(rule);
    }
    LOG(INFO) << "succeed in loading grammar file, grammar size " 
        << _rules.size() << ".";
    _trie = build_trie();
    return 0;
}

Trie* Grammar::build_trie() {
    Trie* trie = new Trie();
    for (int i = 0; i < (int)_rules.size(); ++i) {
        trie->add(_rules[i]);
    }
    return trie;
}

Trie* Grammar::get_trie() {
    return _trie;
}

vector<Rule*>& Grammar::get_all_rules() {
    return _rules; 
}

}


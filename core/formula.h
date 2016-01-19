/**
 * formula.h
 *
 * Author: wushiwei
 * Created on: 2015-02-15
 *
 * Copyright (c) Baidu.com, Inc. All Rights Reserved
 *
 */

#ifndef __FORMULA_H_
#define __FORMULA_H_

#include "utils.h"
#include "feat_vec.h"
#include "params.h"

namespace semantic {
static const std::string ROOT = "ROOT";
class Rule {
public:
    Rule() : _lhs("") {}

    Rule(const std::string& lhs, const std::string& rhs) {
        _lhs = lhs;
        _rhs.push_back(rhs);
    }

    // binary grammar.
    Rule(const std::string& lhs, const std::string& rhs1, const std::string& rhs2) {
        _lhs = lhs;
        _rhs.push_back(rhs1);
        _rhs.push_back(rhs2);
    }

    int init(const std::string& line);
    
    std::string to_string();

    bool is_unary();
    std::string get_lhs();

    std::vector<std::string>& get_rhs();
private:
    // left-hand symbol.
    std::string _lhs;
    // right-hand symbols.
    std::vector<std::string> _rhs;
    std::vector<std::pair<std::string, double> > _info;
};

class Derivation {
public:
    Derivation();
    Derivation(std::string& cat, int start, int end, 
        float score, Rule* rule);
    ~Derivation() {
        _children.clear();
        _rule = NULL;
    }

    int get_start() const; 
    int get_end() const;

    Json::Value to_json();
    std::string get_rule_string();

    float_t compute_local_score(Params* model);

    size_t hashcode();
    static size_t hashcode(Json::Value& tree);
public:
    std::string _cat;
    int _start;
    int _end;
    float _score;
    Rule* _rule;
    std::vector<Derivation*> _children;
    FeatureVector local_feature_;
};
}

#endif // __FORMULA_H_ 


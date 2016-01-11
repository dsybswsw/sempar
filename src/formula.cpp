/**
 * formula.cpp
 *
 * Author: wushiwei
 * Created on: 2015-02-15
 *
 * Copyright (c) Baidu.com, Inc. All Rights Reserved
 *
 */

#include "str_opt.h"
#include "formula.h"

namespace semantic {

using std::string;
using std::pair;
using std::vector;

Derivation::Derivation() : _cat(""), _start(-1), 
    _end(-1), _score(0.0), _rule(NULL) {}

Derivation::Derivation(std::string& cat, int start, int end, 
        float score, Rule* rule) {
    _cat = cat;
    _start = start;
    _end = end;
    _score = score;
    _rule = rule;
}

int Derivation::get_start() const {
    return _start;
}

int Derivation::get_end() const {
    return _end;
}

string Derivation::get_rule_string() {
    if (_rule == NULL) {return "";}
    return _rule->to_string();
}

float_t Derivation::compute_local_score(Params* model) {
    _score = local_feature_.dot_product(model);
    for (int i = 0; i < (int)_children.size(); ++i) {
        _score += _children[i]->_score;
    }
    return _score;
}

// serailize the parsing tree to json.
Json::Value Derivation::to_json() {
    Json::Value tree;
    string cat = _cat;
    Json::Value array;
    if (_children.empty()) { // leaf node.
        vector<string>& rhs = _rule->get_rhs();
        for (int i = 0; i < (int)rhs.size(); ++i) {
            array.append(rhs[i]);
        }
        tree[cat] = array;
        return tree;
    }
    for (int i = 0; i < (int)_children.size(); ++i) {
        Json::Value child = _children[i]->to_json(); 
        array.append(child);
    }
    tree[cat] = array;
    return tree;
}

int Rule::init(const std::string& line) {
    string gram = line;
    boost::algorithm::trim(gram);
    vector<string> symbols;
    boost::split(symbols, gram, boost::is_any_of(" "));
    // check valid split number.
    if (symbols.size() < 2) {
        WARN("%s is not a valid grammar.", gram.c_str());
        return -1;
    }
    string cat_val = symbols[0];
    int fnd = cat_val.find("$");
    if (fnd != 0) {
        LOG(INFO) << "[rule:%s] left hand symbols must be a non-terminal," 
            << " and start with $" << gram.c_str();
        return -1;
    }
    _lhs = cat_val.substr(1);
    for (unsigned int i = 1; i < symbols.size(); ++i) {
        string val = symbols[i];
        boost::algorithm::trim(val);
        fnd = val.find("$");
        if (fnd == 0) { // start with $
            val = val.substr(1);
        }
        _rhs.push_back(val);
    }
    return 0;
}

string Rule::to_string() {
    std::string str = _lhs;
    str.append("->");
    for (int i = 0; i < (int)_rhs.size(); ++i) {
        str.append(_rhs[i]);
        str.append(" ");
    }
    return str;
}

bool Rule::is_unary() {
    if ((int)_rhs.size() == 1) {
        return true;
    }
    return false;
}

string Rule::get_lhs() {
    return _lhs;
}

vector<string>& Rule::get_rhs() {
    return _rhs;
}

size_t Derivation::hashcode() {
    std::hash<string> hash_str_func;
    std::hash<size_t> hash_int_func;
    size_t local_hash = hash_str_func(_cat);
    
    // handle leaf nodes.
    if (_children.empty()) { // leaf node.
        vector<string>& rhs = _rule->get_rhs();
        for (int i = 0; i < (int)rhs.size(); ++i) {
            size_t leaf_hash = hash_int_func(hash_str_func(rhs[i]));
            local_hash += hash_int_func(31 * local_hash + leaf_hash);
        }
        return hash_int_func(local_hash);
    }

    for (auto deriv : _children) {
        local_hash += hash_int_func(31 * local_hash + deriv->hashcode());
    }
    return hash_int_func(local_hash);
}

size_t Derivation::hashcode(Json::Value& tree) {
    std::hash<string> hash_str_func;
    std::hash<size_t> hash_int_func;
    if (!tree.isObject()) {
        string category = tree.asString();
        return hash_int_func(hash_str_func(category));
    }
    if (tree.getMemberNames().empty()) {
        LOG(INFO) << "wrong tree json " << tree;
        return hash_int_func(hash_str_func(""));
    }
    string key = tree.getMemberNames()[0];
    size_t local_hash = hash_str_func(key);
    Json::Value array = tree[key];
    for (int i = 0; i < array.size(); ++i) {
        local_hash += hash_int_func(hashcode(array[i]) + 31 * local_hash);
    }
    return hash_int_func(local_hash);
}

}


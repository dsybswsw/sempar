/**
 * wsg_and_pos.cpp
 *
 * Author: wushiwei
 * Created on: 2016-01-14
 *
 * Copyright (c) 2016 wushiwei. All Rights Reserved
 *
 */

#include "lex_data_struct.h"

namespace semantic {

using std::string;
using std::vector;
using std::map;
using std::set;

Term::~Term() {
}

Term::Term(const std::string& text, const std::string& type, int start, int end) {
    _text = text;
    _start = start;
    _end = end;
    _type = type;
    _norm_text = "";
    _score = 0.0;
}

Term::Term(const std::string& text, const std::string& type, \
        const std::string& norm_text, int start, int end) {
    _text = text;
    _start = start;
    _end = end;
    _type = type;
    _norm_text = norm_text;
    _score = 0.0;//default
}

Term::Term(const Term& term) {
    _text = term._text;
    _start = term._start;
    _end = term._end;
    _type = term._type;
    _norm_text = term._norm_text;
    _score = term._score;
}

Term& Term::operator= (const Term& term) {
    _text = term._text;
    _start = term._start;
    _end = term._end;
    _type = term._type;
    _norm_text = term._norm_text;
    _score = term._score;
    return *this;
}

bool Term::operator== (const Term& term) const {
    if (_text != term._text || _type != term._type || _norm_text != term._norm_text) {
        return false;
    }
    if (_start != term._start || _end != term._end) {
        return false;
    }
    /*if (_score - term._score > FC_EPSILON || _score - term._score < -FC_EPSILON) { //float a != b
        return false;
    }*/
    return true;
}

bool Term::operator!= (const Term& term) const {
    if ((*this) == term) {
        return false;
    }
    return true;
}

string Term::to_string() const {
    char log_line[MAX_LINE_LEN];
    snprintf(log_line, MAX_LINE_LEN, "{%s, %s, %s, %d, %d, %.6f}", _text.c_str(), 
        _type.c_str(), _norm_text.c_str(), _start, _end, _score);
    log_line[MAX_LINE_LEN - 1] = '\0';
    string str = log_line;
    return str;
}

int Term::combines(const Term& term1, const Term& term2) {
    if (term1._end != term2._start || term1._type != term2._type) {
        WARN("%s and %s cannot be combined", term1.to_string().c_str(), 
            term2.to_string().c_str());
        return -1;
    }
    _text.clear();
    _text.append(term1._text);
    _text.append(term2._text);
    _type = term1._type;
    _start = term1._start;
    _end = term2._end;
    _score = term1._score + term2._score;//multiple in real domain; add in log domain
    return 0;
}

string TermContainer::to_string() const {
    char score_str[MAX_LINE_LEN];
    snprintf(score_str, MAX_LINE_LEN, " score=%.2f, ", _score);
    score_str[MAX_LINE_LEN-1] = '\0';

    string str = "";
    str.append(score_str);
    str.append("[");
    for (int i = 0; i < (int)_terms.size(); ++i) {
        string term_str = _terms[i].to_string();
        str.append(term_str);
        if (i != (int)_terms.size() -1) {
            str.append(",");
        }
    }
    str.append("]");
    return str;
}

bool TermContainer::find(Term& term) const {
    vector<Term>::const_iterator it = _terms.begin();
    for (; it != _terms.end(); ++it) {
        if (*it == term) {
            return true;
        }
    }
    return false;
}

void TermContainer::clear() {
    _pos_base = -1;
    _terms.clear();
    _score = 0.0;
}

void TermContainer::add_term(Term& term) {
    if (find(term)) {
        return;
    }
    _terms.push_back(term);
}

void TermContainer::set_score(float score){
    
    _score = score;

    return;
}

float TermContainer::get_score(){

    return _score;
}

void TermContainer::remove_term(Term& term) {
    for (int i = 0; i < (int)_terms.size(); ++i) {
        if (_terms[i] == term) {
            _terms.erase(_terms.begin() + i);
            --i;
        }
    }
}

TermContainer& TermContainer::operator= (const TermContainer& term_cnt) {
    int term_size = static_cast<int>(term_cnt._terms.size());
    _terms.clear();
    _terms.resize(term_cnt._terms.size());
    for (int i = 0; i < term_size; i++) {
        _terms[i] = term_cnt._terms[i]; 
    }
    _pos_base = term_cnt._pos_base;
    _score = term_cnt._score;
    return *this;
}

LexForm::LexForm(const std::string& query) {
    _query = query;
    _actions_map.clear();
}

string LexForm::to_string() const {
    string str = "{";
    int idx = 0;
    HashMap<string, TermContainer>::const_iterator it = _slot_forms.begin();
    for (; it != _slot_forms.end(); ++it) {
        string key = it->first;
        string terms_str = it->second.to_string();
        str.append("{");
        str.append(key);
        str.append(",");
        str.append(terms_str);
        str.append("}");
        if (idx != (int)_slot_forms.size() - 1) {
            str.append(",");
        }
        ++idx;
    }
    str.append("}");
    return str;
}

const std::string& LexForm::get_query() {
    return _query;
}

void LexForm::clear(const std::string& key) {
    if (_slot_forms.count(key) != 0) {
        _slot_forms[key].clear();
    }
}

bool LexForm::contains(const std::string& key) {
    if (_slot_forms.count(key) == 0) {
        return false;
    }
    return true;
}

int LexForm::get_terms(const string& key, TermContainer& terms) {
    if (_slot_forms.count(key) == 0) {
        LOG(WARNING) << key << " does not exist!";
        return -1;
    }
    terms = _slot_forms[key];
    return 0;
}

int LexForm::get_terms(const string& key, vector<Term>& terms) {
    if (_slot_forms.count(key) == 0) {
        LOG(WARNING) << key << " does not exist!";
        return -1;
    }
    terms = _slot_forms[key]._terms;
    return 0;
}

int LexForm::add_term(const string& key, Term& term) {
    if (_slot_forms.count(key) == 0) {
        TermContainer term_container;
        _slot_forms[key] = term_container;
    }
    TermContainer& terms = _slot_forms[key];
    terms.add_term(term);
    
    return 0;
}

int LexForm::remove_term(const string& key, Term& term) {
    if (_slot_forms.count(key) == 0) {
        LOG(WARNING) << key << " does not exist!";
        return 0;
    }
    TermContainer& terms = _slot_forms[key];
    terms.remove_term(term);
    return 0;
}

void LexForm::add_term_container(const string& key, TermContainer& term_container) {
    if (_slot_forms.count(key) == 0) {
        _slot_forms[key] = term_container;
    } else {
        LOG(WARNING) << "the key of " << key << " is already exist," 
            << " remains the previous termcontainer";
    }
}

void LexForm::remove_term_container(const string& key) {
    if (_slot_forms.count(key) == 0) {
        LOG(WARNING) << key << " does not exist!";
    } else {
        _slot_forms.erase(_slot_forms.find(key));
    } 
}

bool compare_formula(const LexForm& form1, const LexForm& form2) {
    return (form1._confidance > form2._confidance);
}

}


/**
 * lex_data_struct.h
 *
 * Author: wushiwei
 * Created on: 2016-01-14
 *
 * Copyright (c) 2016 wushiwei. All Rights Reserved
 *
 */

#ifndef LEX_ALIGN_COMMON_DATA_STRUCT_H 
#define LEX_ALIGN_COMMON_DATA_STRUCT_H 

#include "utils.h"

namespace semantic {

static const std::string ORIGIN = "origin";
static const std::string WORDSEG = "wordseg";
static const std::string CHARACTER = "character";
static const std::string NE = "ne";

class Term {
public:
    ~Term();
    Term() : _text(""), _type(""), _norm_text(""), _score(0.0), _start(-1), _end(-1){}
    Term(const std::string& text, const std::string& type, int start, int end);
    Term(const std::string& text, const std::string& type, \
            const std::string& norm_text, int start, int end);
    Term(const Term& term);

    int combines(const Term& front, const Term& back);

    bool operator== (const Term& term) const;
    bool operator!= (const Term& term) const;
    Term& operator= (const Term& term);

    std::string to_string() const;
public:
    std::string _text;
    std::string _type;
    std::string _norm_text;
    float _score;//different module may be different meaning. xypeng
    // position in charactor sequence. 
    int _start;
    int _end;
};

class TermContainer {
public:
    TermContainer() : _pos_base(CHARACTER), _score(0) {}

    std::string to_string() const;
    
    bool find(Term& term) const;
    void add_term(Term& term);
    void remove_term(Term& term);
    void set_score(float score);
    float get_score();
    void clear();
    TermContainer& operator= (const TermContainer& term_cnt);
public:
    std::vector<Term> _terms;
    std::string _pos_base;
    float _score;
};

class Form {};

/**
 * Formula used in the whole query analysis pipeline.
 **/
class LexForm : public Form {
public:
    LexForm() : _confidance(0.0) {}
    LexForm(const std::string& query);

    virtual ~LexForm() {}

    virtual std::string to_string() const;

    const std::string& get_query();

    int get_terms(const std::string& key, TermContainer& container);
    int get_terms(const std::string& key, std::vector<Term>& terms);
    int add_term(const std::string& key, Term& term);
    int remove_term(const std::string& key, Term& term);
    void add_term_container(const std::string& key, TermContainer& term_contrainer);
    void remove_term_container(const std::string& key);

    void clear(const std::string& key);

    bool contains(const std::string& key); 
public:
    std::string _query;
    HashMap<std::string, TermContainer> _slot_forms;

    float _confidance;
private:
    std::map<std::string, HashSet<std::string> > _actions_map;
};

bool compare_formula(const LexForm& form1, const LexForm& form2);

} // end namespace semantic

#endif // LEX_ALIGN_COMMON_DATA_STRUCT_H


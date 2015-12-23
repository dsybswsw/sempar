/**
 * parser.h
 *
 * Author: wushiwei
 * Created on: 2015-02-15
 *
 * Copyright (c) wushiwei. All Rights Reserved
 *
 */

#ifndef SEMANTIC_PARSER_H 
#define SEMANTIC_PARSER_H 

#include "grammar.h"
#include "semantic_fn.h"
#include "feat_extractor.h"

namespace semantic {

class ParserState;

class Parser {
public:
    Parser() : _grammar(NULL), extractor_(NULL) {}
    ~Parser();

    int initialize(const char* gram_file);
    void parse(Example& ex);
    void parse(Params* model, Example& ex);
    
    // training procedures.
    ParserState* parse_with_state(Example& ex);
    ParserState* parse_with_state(Params* model, Example& ex);
private:
    Grammar* _grammar;
    FeatureExtractor* extractor_;
};

/**
 * cell (category, start, end) -> list of derivation.
 * sorted by score in decreasing order.
 **/
class Cell : public std::vector<std::pair<std::string, std::vector<Derivation*> > > {
public:
    void add_to_cell(const std::string& lhs, Derivation* deriv);
    void clear_cell();
    void sort();
    int count(const std::string& key);
    std::vector<Derivation*>& at(const std::string& lhs);
private:
    HashMap<std::string, int> _key_index_map;
};

class ParserState {
public:
    ParserState() : _example(NULL), _trie(NULL), 
        _grammar(NULL), extractor_(NULL), 
        model_(NULL), _token_num(0) {}
    ParserState(Example* ex, Grammar* grammar);
    ParserState(Example* ex, Grammar* grammar, 
        FeatureExtractor* extractor, Params* model);
    ~ParserState();
 
    void init(Example* ex, Grammar* grammar, 
        FeatureExtractor* extractor, Params* model); 
    void init_chart();
    void clear_chart();

    void inference();
    void extract_result();
    std::vector<Derivation*> get_parse_derivations();
protected:
    void build(int start, int end);
    void apply_non_unary_rules(int start, int end, int i, Trie* trie, 
        std::vector<Derivation*>& children);
    void apply_rule(int start, int end, Rule* rule, std::vector<Derivation*>& children);
    bool coarse_allows(const std::string& lhs, int start, int end);
    void featurize_and_score(Derivation* derivation);
protected:
    Example* _example;
    // not the owner.
    Trie* _trie;
    // not the owner.
    Grammar* _grammar;
    // feature extractor.
    FeatureExtractor* extractor_;
    // model params.
    Params* model_;

    int _token_num;
    std::vector<std::vector<Cell> > _chart;
};

}

#endif // SEMANTIC_PARSER_H 


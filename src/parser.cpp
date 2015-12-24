#include "parser.h"

namespace semantic {

using std::string;
using std::vector;
using std::pair;

void Cell::add_to_cell(const std::string& lhs, Derivation* deriv) {
    this->at(lhs).push_back(deriv);
}

vector<Derivation*>& Cell::at(const string& lhs) {
    if (count(lhs) == 0) {
        int cat_size = (int)this->size();
        _key_index_map[lhs] = cat_size;
        pair<string, vector<Derivation*> > lhs_derive_pair;
        lhs_derive_pair.first = lhs;
        this->push_back(lhs_derive_pair);
    }
    return (*this)[_key_index_map[lhs]].second;
}

int Cell::count(const string& key) {
    if (_key_index_map.count(key) > 0) {
        int idx = _key_index_map[key];
        if (idx >= 0 && idx < this->size()) {
            return _key_index_map.count(key);     
        }
    }
    return 0;
}

void Cell::clear_cell() {
    for (int i = 0; i < (int)this->size(); ++i) {
        vector<Derivation*>& derivs = (*this)[i].second;
        for (int i = 0; i < (int)derivs.size(); ++i) {
            if (derivs[i] != NULL) {
                delete derivs[i];
                derivs[i] = NULL;
            }
        }
        derivs.clear();
    }
    this->clear();
    _key_index_map.clear();
}

Parser::~Parser() {
    if (_grammar != NULL) {
        delete _grammar;
        _grammar = NULL;
    }
    if (extractor_ != NULL) {
        delete extractor_;
        extractor_ = NULL;
    }
}

int Parser::initialize(const char* gram_file) {
    _grammar = new Grammar();
    int ret = _grammar->read(gram_file);
    if (ret < 0) {
        FATAL("Failed to initialize grammars.");
        return -1;
    }
    extractor_ = new FeatureExtractor();
    return 0;
}

void Parser::parse(Example& ex) {
    ParserState state(&ex, _grammar);
    state.inference();
    LOG(INFO) << "Finish parsing inference.";
    state.extract_result();
}

void Parser::parse(Params* model, Example& ex) {
    if (model == NULL) {
        return parse(ex);
    }
    ParserState state(&ex, _grammar, extractor_, model);
    state.inference();
    LOG(INFO) << "Finish parsing inference.";
    state.extract_result();
}

ParserState* Parser::parse_with_state(Example& ex) {
    ParserState* state = new ParserState(&ex, _grammar);
    state->inference();
    // copy result pointers to examples.
    ex._parsing_derivs = state->get_parse_derivations();
    return state;
}

ParserState* Parser::parse_with_state(Params* model, Example& ex) {
    if (model == NULL) {
        LOG(WARNING)  << "model is null.";
        return NULL;
    }
    ParserState* state = new ParserState(&ex, _grammar, extractor_, model);
    state->inference();
    // copy result pointers to examples.
    ex._parsing_derivs = state->get_parse_derivations();
    return state;
}

ParserState::ParserState(Example* ex, Grammar* grammar) {
    init(ex, grammar, NULL, NULL);
}

ParserState::ParserState(Example* ex, Grammar* grammar, 
        FeatureExtractor* extractor, Params* model) {
    init(ex, grammar, extractor, model);
}

void ParserState::init(Example* ex, Grammar* grammar, 
        FeatureExtractor* extractor, Params* model) {
    _example = ex;
    _grammar = grammar;
    extractor_ = extractor;
    model_ = model;
    if (_example != NULL) {
        _token_num = _example->_tokens.size(); 
    }
    if (_grammar != NULL) {
        _trie = _grammar->get_trie();
    }
    init_chart();
}

ParserState::~ParserState() {
    _trie = NULL;
    _grammar = NULL;
    extractor_ = NULL;
    model_ = NULL;
    clear_chart();
}

void ParserState::init_chart() {
    _chart.resize(_token_num + 1);
    for (int i = 0; i <= _token_num; ++i) {
        _chart[i].resize(_token_num + 1);
        for (int j = i + 1; j <= _token_num; ++j) {
            _chart[i][j].clear();
        }
    }
}

void ParserState::clear_chart() {
    for (int i = 0; i < _token_num; ++i) {
        for (int j = i + 1; j <= _token_num; ++j) {
            _chart[i][j].clear_cell();
        }
    }
    _chart.clear();
}

bool ParserState::coarse_allows(const string& lhs, int start, int end) {
    if (_chart[start][end].count(lhs) == 0) {
        return false;
    } else {
        return true;
    }
}

void ParserState::inference() {
    if (_token_num == 0) {return;}
    if (!_trie || !_grammar) {return;}
    // build parsing chart.
    for (int len = 1; len <= _token_num; ++len) {
        for (int start = 0; start + len <= _token_num; ++start) {
            build(start, start + len);
        }
    }
}

// create all the derivations for the span [start, end).
void ParserState::build(int start, int end) {
    // apply non-unary rules.
    vector<Derivation*> children;
    apply_non_unary_rules(start, end, start, _trie, children);
    LOG(INFO) << "finish build " << start << " to " << end;
    // apply unary rules.
    // prune the cell.
}

bool compare(Derivation* deriv1, Derivation* deriv2) {
    return (deriv1->_score > deriv2->_score);
}

void ParserState::extract_result() {
    Cell& cell = _chart[0][_token_num];
    if (cell.count(ROOT) == 0) {return;}
    vector<Derivation*>& derivs = cell.at(ROOT);
    LOG(INFO) << "parsing tree number is " << derivs.size() << " and extract " 
        << _example->_nbest << " best results.";
    std::sort(derivs.begin(), derivs.end(), compare);
    if (derivs.size() > _example->_nbest) {
        derivs.resize(_example->_nbest);
    }
    for (int i = 0; i < derivs.size(); ++i) {
        Json::Value par_tree = derivs[i]->to_json();
        _example->_trees.push_back(par_tree);
        LOG(INFO) << i + 1 << " best result score is " << derivs[i]->_score;
    }
}

/**
 * real parsing actions : pattern match and chart building.
 * start, span start index.
 * end, span end index. left close and right open. 
 * i, current pattern match index between start and end. 
 * trie, pattern trie.
 * children, tree path. 
 **/
void ParserState::apply_non_unary_rules(int start, int end, int i, 
        Trie* trie, vector<Derivation*>& children) {
    if (trie == NULL) {return;}
    if (i == end) { // traverse to the end.
        // get specific rules from trie.
        vector<Rule*>& rules = trie->_rules;
        for (int k = 0; k < (int)rules.size(); ++k) {
            apply_rule(start, end, rules[k], children);
        }
        return;
    }

    // traverse terminal tokens.
    Trie* next = trie->next(_example->get_token(i));
    if (next != NULL) {
        apply_non_unary_rules(start, end, i + 1, next, children);
    }

    // traverse non-terminal tokens.
    for (int j = i + 1; j <= end; ++j) {
        for (int k = 0; k < _chart[i][j].size(); ++k) {
            Trie* subtrie = trie->next(_chart[i][j][k].first);
            if (subtrie == NULL) {continue;}
            vector<Derivation*> derivs = _chart[i][j][k].second;
            for (int dv = 0; dv < (int)derivs.size(); ++dv) {
                children.push_back(derivs[dv]);
                apply_non_unary_rules(start, end, j, subtrie, children);
                // deep first search, remove the traversed path.
                children.pop_back();
            }
        }
    }
}

void ParserState::apply_rule(int start, int end, Rule* rule, 
        vector<Derivation*>& children) {
    if (rule == NULL) {
        LOG(INFO) << "rule is null.";
        return;
    }
    char log_str[MAX_BUFF_LEN];
    snprintf(log_str, MAX_BUFF_LEN, "span [%d,%d] match rule %s\n",
        start, end, rule->to_string().c_str());
    log_str[MAX_BUFF_LEN - 1] = '\0';
    string lhs = rule->get_lhs();
    SemanticFn fn(lhs, start, end, rule, children);
    vector<Derivation*> derivs = fn.call(*_example);
    for (int i = 0; i < (int)derivs.size(); ++i) {
        Derivation* deriv = derivs[i];
        int start = deriv->get_start();
        int end = deriv->get_end();
        _chart[start][end].add_to_cell(lhs, deriv);
    }
    // featurize and score all the new derivations.
    if (extractor_ == NULL) {return;}
    for (int i = 0; i < (int)derivs.size(); ++i) {
        featurize_and_score(derivs[i]);
    }
}

void ParserState::featurize_and_score(Derivation* derivation) {
    if (derivation == NULL) {return;}
    extractor_->extract_local(*_example, derivation);
    derivation->compute_local_score(model_);
    LOG(INFO) << "feature : " << derivation->local_feature_.to_set_string();
}

vector<Derivation*> ParserState::get_parse_derivations() {
    Cell& cell = _chart[0][_token_num];
    if (cell.count(ROOT) == 0) {return vector<Derivation*>();}
    return cell.at(ROOT);
}

}


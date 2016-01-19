/**
 * learner.h
 *
 * Author: wushiwei
 * Created on: 2015-12-14
 *
 * Copyright (c) wushiwei. All Rights Reserved
 *
 */
#ifndef SEMANTIC_LEARNER_H 
#define SEMANTIC_LEARNER_H 

#include "math_utils.h"
#include "parser.h"
#include "params.h"
#include "dataset.h"

namespace semantic {

class Evaluation {
public:
    Evaluation() {}
    
    void add(const std::string& name, int val);
    void add(const std::string& name, float_t val);

    int get_int(const std::string& name);
    float_t get_float(const std::string& name);
public:
    HashMap<std::string, int> intval_map_;
    HashMap<std::string, float_t> fval_map_;
};

class Learner {
public:
    Learner() : parser_(NULL), model_(NULL), data_set_(NULL), 
        iters_(200), batch_size_(10), relative_func_thres_(1e-3), 
        max_pass_converge_(3) {}
    
    void init(Parser* parser, DataSet* dataset);
    int learn();
    int process_group(int iter, const std::string& group_key, 
            std::vector<Example>& examples, Evaluation& eval);
    ParserState* parse_example(Example& ex);
    float_t reward(float_t tgt, float_t val);

    void update_counts(Example& ex, HashMap<std::string, float_t>& counts);

    void compute_gradients_and_obj(Example& ex, 
                                   HashMap<std::string, float_t>& grads, 
                                   float_t& obj);
    
    /**
     * Abstract interface for computing the objective funtion and parameter gradients.
     * Flexible to change cost function or parsing algorithms and their expected counts.
     * For batch learning, function value and gradients are accumulated after
     * going through each training sample.
     */ 
    virtual void compute_gradients_and_obj(
            Example& ex, 
            Derivation* deriv, 
            HashMap<std::string, float_t>& grads, 
            float_t& obj);

    /**
     * Abstract interface for updating parameters with function gradients.
     * default optimization algorithm is SGD and can be overloaded with other
     * algorithm like L-BFGS etc...
     */ 
    virtual void update_weights(HashMap<std::string, float_t>& counts);

    void compute_expected_counts(
            Example& ex, 
            std::vector<Derivation*>& derivs, 
            HashMap<std::string, float_t>& counts);

    void increment_deriv(
            Derivation* deriv, 
            float_t factor, 
            HashMap<std::string, float_t>& grads);

    void increment(
            float_t factor, 
            HashMap<std::string, float_t>& grad_map, 
            HashMap<std::string, float_t>& feat_map);

    void accumulate_grads(
            HashMap<std::string, float_t>& grads, 
            HashMap<std::string, float_t>& incomes);

    void collect_statistics(std::vector<Evaluation>& evals);
    void collect_cost(std::vector<Evaluation>& evals, Evaluation& eval);

protected:
    Parser* parser_;
    Params* model_;
    DataSet* data_set_;
    int iters_;
    int batch_size_;
    float_t relative_func_thres_;
    int max_pass_converge_;

    std::string dest_path_;
};

}

#endif // SEMANTIC_LEARNER_H 

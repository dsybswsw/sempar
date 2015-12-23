#include "learner.h"

namespace semantic {

using std::string;
using std::vector;

void Evaluation::add(const string& name, int val) {
    if (intval_map_.count(name) == 0) {
        intval_map_[name] = val;
    }
}

void Evaluation::add(const string& name, float_t val) {
    if (fval_map_.count(name) == 0) {
        fval_map_[name] = val;
    }
}

int Evaluation::get_int(const string& name) {
    if (intval_map_.count(name) == 0) {
        intval_map_[name] = 0;
    }
    return intval_map_[name];
}

float_t Evaluation::get_float(const string& name) {
    if (fval_map_.count(name) == 0) {
        fval_map_[name] = 0.0;
    }
    return fval_map_[name];
}

int Learner::learn() {
    vector<Evaluation> evals;
    for (int iter = 0; iter < iters_; ++iter) {
        vector<Evaluation> iter_evals;
        for (auto grp_iter : data_set_->groups()) {
            string grp_key = grp_iter.first;
            vector<Example>& examples = grp_iter.second;
            Evaluation tmp_eval;
            process_group(iter, grp_key, examples, tmp_eval);
            iter_evals.push_back(tmp_eval);
        }
        Evaluation eval;
        collect_cost(iter_evals, eval);
        evals.push_back(eval);
        print_statistic(evals);
        // print statiscal information.
        string tmp_model = dest_path_ + "/" + "param_" 
            + boost::lexical_cast<string>(iter) + ".mdl.txt";
        model_->consist(tmp_model.c_str());
        LOG(INFO) << "----training the " << iter << " 'st iteration.";

    }
    return 0;
}

// minimize the cost function.
int Learner::process_group(int iter, const string& grp_key, 
        vector<Example>& examples, Evaluation& eval) {
    if (examples.empty()) {return 0;}
    HashMap<std::string, float_t> gradients;
    int batch_size = 0;
    float_t accu_cost = 0.0;
    for (auto ex : examples) {
        ParserState* state = parse_example(ex);
        float cost = 0.0;
        HashMap<string, float> grads;
        compute_gradients_and_obj(ex, grads, cost);
        accumulate_grads(gradients, grads);
        accu_cost += cost;
        ++batch_size;
        if (batch_size > batch_size_) {
            // update params when gathered enough examples.
            update_weights(gradients);
            batch_size = 0;
        }
        delete state;
    }
    if (batch_size > 0) {
        update_weights(gradients);
    }
    accu_cost += model_->compulte_l2norm();
    LOG(INFO) << "cost function val is " << accu_cost;
    eval.add("cost", accu_cost);
    int example_size = examples.size();
    eval.add("num", example_size);
    return 0;
}

void Learner::update_counts(Example& ex, HashMap<string, float_t>& counts) {
    compute_expected_counts(ex, ex._parsing_derivs, counts);
}

void Learner::compute_gradients_and_obj(Example& ex, HashMap<string, float_t>& gradients, float_t& acc_obj) {
    vector<Derivation*>& derivs = ex._parsing_derivs;
    for (auto deriv : derivs) {
        compute_gradients_and_obj(ex, deriv, gradients, acc_obj);
    }
}

void Learner::compute_gradients_and_obj(Example& ex, Derivation* deriv, 
        HashMap<string, float_t>& gradients, float_t& acc_obj) {
    float_t true_score;
    float_t pred_score;
    size_t hash = deriv->hashcode();
    size_t tar_hash = ex.target_hash_;
    float_t r = (hash == tar_hash ? 1.0 : 0.0);
    LOG(INFO) << "parse hash " << hash << ", target hash " 
        << tar_hash << ", reward " << r;
    true_score = r;
    pred_score = math::sigmoid(deriv->_score);
    LOG(INFO) << "true score " << true_score << ", predict score " << pred_score;
    float_t dec = pred_score - true_score;
    // collect gradients.
    increment_deriv(deriv, dec, gradients);
    float jv = -(true_score * log(pred_score + math::EPS) + (1 - true_score) * log(1 - pred_score + math::EPS));
    LOG(INFO) << "objective score of [" << ex._index << ":" << ex._uttrance << "] is " << jv;
    acc_obj += jv; // accumulate objective score.
}

void Learner::update_weights(HashMap<string, float_t>& gradients) {
    model_->update(gradients);
    gradients.clear();
}

void Learner::compute_expected_counts(Example& ex, vector<Derivation*>& derivs, 
        HashMap<string, float_t>& counts) {
    int size = derivs.size();
    vector<float_t> true_scores(size);
    vector<float_t> pred_scores(size);
    for (int i = 0; i < size; ++i) {
        Derivation* deriv = derivs[i];
        size_t hash = deriv->hashcode();
        size_t tar_hash = ex.target_hash_;
        float_t r = (hash == tar_hash ? 1.0 : 0.0);
        LOG(INFO) << "parse hash " << hash << ", target hash " 
            << tar_hash << ", reward " << r;
        // keep reward between 0 to 1.
        // float_t r = reward(ex._target_val, deriv->_score);
        true_scores[i] = r;
        pred_scores[i] = math::sigmoid(deriv->_score);
    }
    // update parameters.
    for (int i = 0; i < size; ++i) {
        // log-linear model with cross-entropy cost function.
        float_t dev = pred_scores[i] - true_scores[i];
        LOG(INFO) << "true score " << true_scores[i] << ", predict score " << pred_scores[i];
        increment_deriv(derivs[i], dev, counts);
    }
}

void Learner::increment_deriv(Derivation* deriv, float_t factor, 
        HashMap<string, float_t>& grad_map) {
    if (deriv == NULL) {return;}
    FeatureVector& feat_vec = deriv->local_feature_;
    // update feature factors.
    increment(factor, grad_map, feat_vec.feat_vec_);
    // update each derivation.
    vector<Derivation*>::iterator it = deriv->_children.begin();
    for (; it != deriv->_children.end(); ++it) {
        increment_deriv(*it, factor, grad_map);
    }
}

void Learner::increment(float_t factor, HashMap<string, float_t>& grad_map, 
        HashMap<string, float_t>& feat_map) {
    for (auto entry : feat_map) {
        if (grad_map.count(entry.first) > 0) {
            grad_map[entry.first] += factor * entry.second;
        } else {
            grad_map[entry.first] = factor * entry.second;
        }
    }
}

ParserState* Learner::parse_example(Example& ex) {
    return parser_->parse_with_state(model_, ex);
}

float_t Learner::reward(float_t target, float_t val) {
    return  (fabs(target - val) < math::EPS ? 1.0 : 0.0);
}

void Learner::init(Parser* parser, DataSet* dataset) {
    parser_ = parser;
    data_set_ = dataset;
    model_ = new Params();
    dest_path_ = "data/model/";
}

void Learner::accumulate_grads(HashMap<string, float_t>& grads, HashMap<string, float_t>& incomes) {
    for (auto entry : incomes) {
        if (grads.count(entry.first) == 0) {
            grads[entry.first] = entry.second;
        } else {
            grads[entry.first] += entry.second;
        }
    }
}

void Learner::print_statistic(vector<Evaluation>& evals) {
    if (evals.empty()) {return;}
    // step 1, calculate mean cost.
    int size = (int)evals.size();
    Evaluation& last = evals[size - 1];
    int sample_size = last.get_int("num");
    if (sample_size == 0) {return;}
    float_t cost = last.get_float("cost");
    float_t mean_cost = cost / sample_size;
    evals[size - 1].add("mean_cost", mean_cost);
    float_t diff = 0.0;
    if (evals.size() > 1) {
        Evaluation& one_bf_last = evals[size - 2];
        float_t bf_mc = one_bf_last.get_float("mean_cost");
        diff = fabs(mean_cost - bf_mc) / (bf_mc + math::EPS);
    }
    char info[MAX_BUFF_LEN];
    snprintf(info, MAX_BUFF_LEN, "iter=%d obj=%f diff=%f", size, mean_cost, diff);
    info[MAX_BUFF_LEN - 1] = '\0';
    std::cout << info << std::endl; 
}

void Learner::collect_cost(vector<Evaluation>& evals, Evaluation& eval) {
    int num = 0;
    float_t cost = 0.0;
    for (Evaluation e : evals) {
        num += e.get_int("num");
        cost += e.get_float("cost");
    }
    eval.add("num", num);
    eval.add("cost", cost);
}

}


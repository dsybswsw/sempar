#include "params.h"
#include "math_utils.h"
#include "file_utils.h"

namespace semantic {

using std::string;
using std::vector;

void Params::update(HashMap<string, float_t>& gradients) {
    ++update_num_;
    float_t step_size = 1.33;
    for (auto entry : gradients) {
        string feature = entry.first;
        float_t grad = entry.second;
        float_t w = get_weight(feature);
        weights_[feature] = w - grad * step_size;
    }
    // udpate L2 normalization.
    for (auto entry : weights_) {
        weights_[entry.first] -= mu_ * entry.second * step_size;
    }
}

float_t Params::compulte_l2norm() {
    float_t sum = 0.0;
    for (auto entry : weights_) {
        sum += entry.second * entry.second;
    }
    return 0.5 * mu_ * sum;
}

bool Params::contains(const string& feature) {
    return (weights_.count(feature) > 0);
}

float_t Params::get_weight(const string& feature) {
    if (!contains(feature)) {
        weights_[feature] = gen_init();
    }
    return weights_[feature];
}

// TODO(shiwei) : initialize the parameters randomly.
float_t Params::gen_init() {
    return 0.0;
}

int Params::consist(const char* path) {
    std::ofstream out(path);
    if (!out.is_open()) {
        LOG(WARNING) << "Failed to open " << path;
        return -1;
    }
    for (auto entry : weights_) {
        out << entry.first << "\t" << entry.second << std::endl;
    }
    out.close();
    return 0;
}

int Params::read(const char* path) {
    string file_path(path);
    vector<string> contents;
    if (fs::read_lines(file_path, contents) < 0) {
        LOG(INFO) << "Failed to read model " << path;
        return -1;
    }
    for (string line : contents) {
        boost::algorithm::trim(line);
        vector<string> tokens;
        boost::split(tokens, line, boost::is_any_of(" \t"));
        if (tokens.size() != 2) {continue;}
        string feat = tokens[0];
        float_t weight = boost::lexical_cast<float_t>(tokens[1]);
        weights_[feat] = weight;
    }
    return 0;
}

}

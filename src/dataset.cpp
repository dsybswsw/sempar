/**
 * dataset.cpp
 *
 * Author: wushiwei
 * Created on: 2015-12-14
 *
 * Copyright (c) wushiwei. All Rights Reserved
 *
 */

#include "dataset.h"
#include "str_opt.h"

namespace semantic {

using std::string;
using std::vector;

HashMap<string, vector<Example> >& DataSet::groups() {
    return all_samples_;
}

bool DataSet::contains(const string& grp_key) {
    return (all_samples_.count(grp_key) > 0);
}

vector<Example>& DataSet::get_examples(const string& grp_key) {
    return all_samples_[grp_key];
}

vector<string>& DataSet::get_group_keys() {
    if (group_keys_.size() == all_samples_.size()) {
        return group_keys_;
    }
    for (auto entry : all_samples_) {
        group_keys_.push_back(entry.first);
    }
    return group_keys_;
}

int DataSet::read() {
    if (opt_.data_format_ == 1) {
        return read_answers();
    } else if (opt_.data_format_ == 2) {
        return read_trees();
    } else {
        LOG(INFO) << "unknown data format " << opt_.data_format_;
        return 0;
    }
}

int DataSet::read_trees() {
    vector<string> files = fs::extract_paths_from_folder(opt_.folder_path_);
    if (files.empty()) {
        LOG(FATAL) << "Nothing in folder " << opt_.folder_path_;
        return -1;
    }
    
    for (auto file : files) {
        vector<string> contents;
        if (fs::read_lines(file, contents) < 0) {
            LOG(FATAL) << "Failed to read contents from " << file;
            return -1;
        }
        string grp_name = fs::extract_file_prefix(file);
        if (grp_name.empty()) {
            LOG(FATAL) << file << " name is strange, can not make file prefix.";
            return -1;
        }
        if (read_trees(grp_name, contents) < 0) {
            LOG(FATAL) << "Failed to make contents for examples in group " << grp_name;
            return -1;
        }
    }
    return 0;
}

int DataSet::read_trees(const std::string& group_name, vector<string>& contents) {
    int num_line = 0;
    if (all_samples_.count(group_name) == 0) {
        all_samples_[group_name] = vector<Example>();
    }
    for (auto line : contents) {
        ++num_line;
        boost::algorithm::trim(line);
        if (line.empty()) {continue;}
        vector<string> tokens;
        boost::split(tokens, line, boost::is_any_of(" \t"));
        if (tokens.size() != 2) {
            LOG(INFO) << "format error in line " << num_line;
            continue;
        }
        string query = tokens[0];
        string tree_json = tokens[1];
        LOG(INFO) << query << " and " << tree_json;
        Json::Reader reader;
        Json::Value value;
        if (!reader.parse(tree_json, value)) {continue;}
        vector<string> words;
        split2chs(query.c_str(), words);
        Example ex(num_line, query, words, 1);
        ex._target_tree_json = value;
        ex.target_hash_ = Derivation::hashcode(value);
        // LOG(INFO) << "example hash " << ex.target_hash_;
        all_samples_[group_name].push_back(ex);
    }
    LOG(INFO) << "group " << group_name << " has " 
        << all_samples_[group_name].size() << " samples.";
    return 0;
}

int DataSet::read_answers() {
    LOG(INFO) << "<q,a> format has not been supported.";
    return -1;
}

}


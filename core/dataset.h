/**
 * dataset.h
 *
 * Author: wushiwei
 * Created on: 2015-12-14
 *
 * Copyright (c) wushiwei. All Rights Reserved
 *
 */
#ifndef SEMANTIC_DATASET_H 
#define SEMANTIC_DATASET_H 

#include "example.h"
#include "file_utils.h"

namespace semantic {

/**
 * Dataset objects, read from data set.
 **/ 
class DataSet {
public:
    class Option {
    public:
        Option() : data_format_(2), max_num_(1000000) {
            folder_path_ = "data/corpus/"; // default path.
            LOG(INFO) << "set default training path " << folder_path_;
        }

        /** 1 for <uttr, answer>, 2 for <uttr, tree>
         *  trees are represented in json format.
         **/
        int data_format_;
        int max_num_;
        std::string folder_path_;
    };

    DataSet() {}
    int read();
    HashMap<std::string, std::vector<Example> >& groups();
    bool contains(const std::string& group_key);
    std::vector<Example>& get_examples(const std::string& group);
    std::vector<std::string>& get_group_keys();
protected:
    int read_trees();
    int read_answers();

    int read_trees(const std::string& group_name, std::vector<std::string>& contents);

    HashMap<std::string, std::vector<Example> > all_samples_;
    std::vector<std::string> group_keys_;
    Option opt_;
};

}

#endif // SEMANTIC_DATASET_H

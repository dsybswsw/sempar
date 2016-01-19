/**
 * file_utils.cpp
 *
 * Author: wushiwei
 * Created on: 2015-12-15
 *
 * Copyright (c) 2015 wushiwei. All Rights Reserved
 *
 */

#include "file_utils.h"

namespace semantic {
namespace fs {

using std::string;
using std::vector;
using boost::filesystem::directory_iterator;
using boost::filesystem::is_directory;

vector<string> extract_paths_from_folder(const string& folder_path) {
    vector<string> paths;
    boost::filesystem::path path(folder_path);
    for (auto it = directory_iterator(path); it != directory_iterator(); ++it) {
        if (is_directory(it->path())) {continue;}
        paths.push_back(it->path().string());
        LOG(INFO) << "file path: " << it->path().string();
    }
    return paths;
}

int read_lines(const std::string& file_path, vector<string>& content) {
    std::ifstream txt_file(file_path);
    if (!txt_file.is_open()) {
        LOG(INFO) << "Failed to open file " << file_path;
        return -1;
    }
    int num_line = 0;
    while(txt_file.good()) {
        string tmp_str;
        ++num_line;
        getline(txt_file, tmp_str);
        boost::algorithm::trim(tmp_str);
        if (tmp_str.empty()) {continue;}
        content.push_back(tmp_str);
    }
    LOG(INFO) << "Finish read " << content.size() << " lines.";
    txt_file.close();
    return num_line;
}

string extract_file_prefix(const string& file_path) {
    LOG(INFO) << "extract key file name from " << file_path;
    string path_str = file_path;
    boost::algorithm::trim(path_str);
    vector<string> file_tokens;
    boost::split(file_tokens, path_str, boost::is_any_of("/\\"));
    if (file_tokens.empty()) {return "";}
    string filename = file_tokens[file_tokens.size() - 1];
    vector<string> sub_toks;
    boost::split(sub_toks, filename, boost::is_any_of(""));
    if (sub_toks.empty()) {return "";}
    return sub_toks[0];
}

}
}

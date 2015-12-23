/**
 * file_utils.h
 *
 * Author: wushiwei
 * Created on: 2015-12-15
 *
 * Copyright (c) 2015 wushiwei. All Rights Reserved
 *
 */

#ifndef SEMANTIC_FILE_UTILS_H
#define SEMANTIC_FILE_UTILS_H

#include "utils.h"

namespace semantic {
namespace fs {

std::vector<std::string> extract_paths_from_folder(const std::string& folder_path);

int read_lines(const std::string& file_path, std::vector<std::string>& contents);

std::string extract_file_prefix(const std::string& file_path);

}
}

#endif // SEMANTIC_FILE_UTILS_H

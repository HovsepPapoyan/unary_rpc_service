/**
 * @file helper.cc
 *
 * @brief loadStringFromFile function is responsible for loading string from the file mentioned by path.
 *
 * @author Hovsep Papoyan
 * Contact: papoyanhovsep93@gmail.com
 * @Date 2025-08-30
 *
 */

#include "helper.h"

#include <fstream>
#include <iostream>
#include <sstream>

namespace helper
{

::std::string loadStringFromFile(const ::std::string& path)
{
    ::std::ifstream file(path);
    if (!file.is_open())
    {
        ::std::cerr << "Failed to open " << path << '\n';
        ::std::abort();
    }
    ::std::stringstream sstr;
    sstr << file.rdbuf();
    return sstr.str();
}

} // namespace helper


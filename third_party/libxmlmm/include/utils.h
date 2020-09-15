 //
// Copyright (c) 2008-2012 Sean Farrell
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of 
// this software and associated documentation files (the "Software"), to deal in 
// the Software without restriction, including without limitation the rights to use, 
// copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the 
// Software, and to permit persons to whom the Software is furnished to do so, 
// subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all 
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
// PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT 
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION 
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
// 

#ifndef _LIBXMLMM_UTILS_H_
#define _LIBXMLMM_UTILS_H_

#include <iosfwd>
#include <string>
#include <sstream>
#include <libxml/tree.h>

namespace xml
{    
    /**
     * Get the last error as string from libxml.
     **/
    std::string get_last_error();
    
    /**
     * Wrap a node.
     *
     * @note This function is used as callback to libxml.
     **/
    void wrap_node(xmlNode* const node);
    
    /**
     * Free the wrapper of a node.
     *
     * @note This function is used as callback to libxml.
     **/
    void free_wrapper(xmlNode* node);
    
    /** Read from a stream until EOF. **/    
    std::string read_until_eof(std::istream& is);
    
    /**
     * Convert arbitrary value to string.
     **/
    template <typename T>
    std::string to_string(T value)
    {
        std::stringstream buff;
        buff << value;
        return buff.str();
    }
    
    /**
     * Convert arbitrary value from string.
     **/
    template <typename T>
    T from_string(const std::string& str)
    {
        std::stringstream buff(str);
        T value;
        buff >> value;
        return value;
    }
}

#endif

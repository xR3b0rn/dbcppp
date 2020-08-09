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

#ifndef _LIBXMLMM_EXCEPTIONS_H_
#define _LIBXMLMM_EXCEPTIONS_H_

#include <stdexcept>

namespace xml
{
    struct Exception : std::runtime_error
    {
        explicit Exception(const std::string &what)
        : runtime_error("xmlmm: " + what) {}
    };

    struct NoRootElement : Exception
    {
        NoRootElement() 
        : Exception("No root element") {}
    };

    struct EmptyDocument : Exception
    {
        EmptyDocument() 
        : Exception("Document is empty") {}
    };

    struct InvalidXPath : Exception
    {
        explicit InvalidXPath(const std::string &xpath)
        : Exception("Invalid XPath: " + xpath) {}
    };

    struct NoSuchAttribute : Exception
    {
        NoSuchAttribute(const std::string &attribute, const std::string &nodeName)
        : Exception("There is no attribute '" + attribute + "' on the element '" + nodeName + "'.") {}
    };

    struct ValidationError : Exception
    {
        explicit ValidationError(const std::string& what) 
            : Exception(what)
        {}
    };
}

#endif

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

#include "Content.h"
#include "utils.h"

namespace xml
{
//------------------------------------------------------------------------------
    Content::Content(xmlNode* cobj) 
    : Node(cobj) {}
    
//------------------------------------------------------------------------------
    std::string Content::get_value() const
    {
        if (cobj->content != NULL)
        {
            return reinterpret_cast<const char*>(cobj->content);
        }
        return std::string();
    }

//------------------------------------------------------------------------------    
    std::string Content::get_content() const    
    {
        return this->get_value(); 
    }
    
//------------------------------------------------------------------------------
    void Content::set_content(const std::string& value) 
    {
        xmlNodeSetContent(cobj, reinterpret_cast<const xmlChar*>(value.c_str()));
    }
    
//------------------------------------------------------------------------------
    bool Content::is_blank() const 
    {
        return xmlIsBlankNode(const_cast<xmlNode*>(cobj));   
    }

}

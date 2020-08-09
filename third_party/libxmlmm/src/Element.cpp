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

#include "Element.h"
#include "exceptions.h"
#include "utils.h"

#include <cassert>
#include <algorithm>
#include <typeinfo> 

namespace xml
{
//------------------------------------------------------------------------------
    Element::Element(xmlNode* const cobj)
    : Node(cobj) {}
    
//------------------------------------------------------------------------------    
    std::string Element::get_name() const
    {
        assert(cobj != NULL);
        if (cobj->name != NULL)
        {
            return reinterpret_cast<const char*>(cobj->name);
        }
        return std::string();
    }

//------------------------------------------------------------------------------        
    void Element::set_name(const std::string& value)
    {
        xmlNodeSetName(cobj, reinterpret_cast<const xmlChar*>(value.c_str()));
    }
    
//------------------------------------------------------------------------------
    bool Element::has_attribute(const std::string& key) const
    {
        return xmlGetProp(cobj, reinterpret_cast<const xmlChar*>(key.c_str())) != NULL;
    }
    
//------------------------------------------------------------------------------
    std::string Element::get_attribute(const std::string& key) const
    {
        const xmlChar* const value = xmlGetProp(cobj, reinterpret_cast<const xmlChar*>(key.c_str()));
        if (!value)
        {
            throw NoSuchAttribute(key, get_name());
        }
        return reinterpret_cast<const char*>(value);
    }
    
//------------------------------------------------------------------------------
    void Element::set_attribute(const std::string& key, const std::string& value)
    {
        xmlSetProp(cobj, reinterpret_cast<const xmlChar*>(key.c_str()), reinterpret_cast<const xmlChar*>(value.c_str()));
    }
        
//------------------------------------------------------------------------------
    void Element::remove_attribute(const std::string& key)
    {
        xmlUnsetProp(cobj, reinterpret_cast<const xmlChar*>(key.c_str()));    
    }

//------------------------------------------------------------------------------
    std::string Element::get_value() const
    {        
        const Content* const content = get_text_node();
        if (content != NULL)
        {
          return content->get_value();
        }
        return std::string();
    }

//------------------------------------------------------------------------------    
    std::string Element::get_text() const
    {
        return this->get_value(); 
    }

//------------------------------------------------------------------------------
    Content* Element::get_text_node() const
    {
        for(xmlNode* child = cobj->children; child; child = child->next)
        {
            if (child->type == XML_TEXT_NODE)
            {
                return reinterpret_cast<Content*>(child->_private);
            }
            if (child->type == XML_CDATA_SECTION_NODE)
            {
                return reinterpret_cast<Content*>(child->_private);
            }            
        }
        return NULL;
    }

//------------------------------------------------------------------------------
    void Element::set_text(const std::string& text)
    {
        Content* node = get_text_node();
        if(node)
        {
            node->set_content(text);
        }
        else
        {
            add_text(text);
        }
    }

//------------------------------------------------------------------------------
    void Element::add_text(const std::string& text)
    {
        xmlNode* node = xmlNewText(reinterpret_cast<const xmlChar*>(text.c_str()));
        xmlAddChild(cobj, node); 
    }

//------------------------------------------------------------------------------    
    Element* Element::add_element(const std::string& name)
    {
        xmlNode* node = xmlNewNode(NULL, reinterpret_cast<const xmlChar*>(name.c_str()));
        xmlAddChild(cobj, node); 
        return reinterpret_cast<Element*>(node->_private);
    }
    
//------------------------------------------------------------------------------    
    std::vector<Node*> Element::get_children()
    {
        std::vector<Node*> children;
        xmlNode* child = cobj->children;
        while (child != NULL)
        {
            children.push_back(reinterpret_cast<Node*>(child->_private));
            child = child->next;
        }
        return children;
    }

//------------------------------------------------------------------------------        
    std::vector<const Node*> Element::get_children() const
    {
        std::vector<const Node*> children;
        const xmlNode* child = cobj->children;
        while (child != NULL)
        {
            children.push_back(reinterpret_cast<const Node*>(child->_private));
            child = child->next;
        }
        return children;
    }
    
//------------------------------------------------------------------------------        
    Element* Element::find_element(const std::string& xpath)
    {
        return this->find<Element*>(xpath);
    }
    
//------------------------------------------------------------------------------
    const Element* Element::find_element(const std::string& xpath) const
    {
        return this->find<const Element*>(xpath);
    }
    
//------------------------------------------------------------------------------        
    std::vector<Element*> Element::find_elements(const std::string& xpath)
    {
        return this->find_all<Element*>(xpath);
    }

//------------------------------------------------------------------------------
    std::vector<const Element*> Element::find_elements(const std::string& xpath) const
    {
        return this->find_all<const Element*>(xpath);
    }
}

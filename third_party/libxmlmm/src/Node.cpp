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

#include "Node.h"

#include <cassert>

#include "utils.h"
#include "exceptions.h"
#include "Element.h"
#include "Content.h"

namespace xml
{
//------------------------------------------------------------------------------
    Node::Node(xmlNode* const co)
    : cobj(co)
    {
        assert(cobj != NULL);
    }
    
//------------------------------------------------------------------------------
    Node::~Node() 
    {
        assert(cobj != NULL);
    }

//------------------------------------------------------------------------------    
    std::string Node::get_path() const
    {
        xmlChar* path = xmlGetNodePath(cobj);
        if (path == NULL)
        {
            throw Exception("xml::Node::get_path(): failed to allocated path");
        }
            
        std::string value(reinterpret_cast<const char*>(path));
        xmlFree(path);
        return value;
    }
    
//------------------------------------------------------------------------------
    Element* Node::get_parent()
    {
        if (cobj->parent != NULL)
        {
            return reinterpret_cast<Element*>(cobj->parent->_private);
        }
        else
        {
            // Ok, this never happens! 
            // A xmlNode only has no parent if it it is a document node 
            // (not root element) and this is not wraped by Node.
            throw Exception("no parent");
        }
    }
    
//------------------------------------------------------------------------------
    const Element* Node::get_parent() const
    {
        return const_cast<Node*>(this)->get_parent();
    }

//------------------------------------------------------------------------------    
    Node* Node::find_node(const std::string& xpath)
    {
        return this->find<Node*>(xpath);
    }
    
//------------------------------------------------------------------------------
    const Node* Node::find_node(const std::string& xpath) const
    {
        return this->find<const Node*>(xpath);
    }
    
//------------------------------------------------------------------------------
    std::vector<Node*> Node::find_nodes(const std::string& xpath)
    {
        return this->find_all<Node*>(xpath);
    }
    
//------------------------------------------------------------------------------
    std::vector<const Node*> Node::find_nodes(const std::string& xpath) const
    {
        return this->find_all<const Node*>(xpath);
    }

//------------------------------------------------------------------------------    
    std::string Node::query_string(const std::string& xpath) const
    {
        FindNodeset search(cobj, xpath);
        const xmlXPathObject* result = search;
        
        std::string value;
        if (result->type == XPATH_STRING)
        {
            value = reinterpret_cast<const char*>(result->stringval);                            
        }
        else if (result->type == XPATH_NUMBER)
        {
            value = to_string(result->floatval);                            
        }
        else if (result->type == XPATH_NODESET)
        {
            const xmlNodeSet* nodeset = result->nodesetval;
            if (nodeset)
            {
                // Concatenate all the text from all the text nodes we have.  
                // NOTE: we technically shouldn't have to do this
                // since all adjacent text nodes are supposed to merge to
                // a single node, but that doesn't always happen in
                // libxml2.  Most notably, when CDATA nodes are adjacent
                // to other text nodes. 
                for (int i = 0; i != nodeset->nodeNr; i++)
                {
                    const Node* node = reinterpret_cast<const Node*>(nodeset->nodeTab[i]->_private);
                    value.append(node->get_value());
                }
            }
        }
        
        return value;
    }
    
//------------------------------------------------------------------------------    
    double Node::query_number(const std::string& xpath) const
    {
        FindNodeset search(cobj, xpath);
        const xmlXPathObject* result = search;
        
        double value = 0.0;
        if (result->type == XPATH_NUMBER)
        {
            value = result->floatval;                            
        }
        else if (result->type == XPATH_STRING)
        {
            value = from_string<double>(reinterpret_cast<const char*>(result->stringval));
        }
        else if (result->type == XPATH_NODESET)
        {
            const xmlNodeSet* nodeset = result->nodesetval;
            if (! xmlXPathNodeSetIsEmpty(nodeset))
            {
                const Node* const node = reinterpret_cast<const Node*>(nodeset->nodeTab[0]->_private);
                value = from_string<double>(node->get_value());
            }
        }
        
        return value;
    }
                

//------------------------------------------------------------------------------
    Node::FindNodeset::FindNodeset(xmlNode *const cobj, const std::string &xpath, const xmlXPathObjectType type)
    {
        ctxt = xmlXPathNewContext(cobj->doc);
        ctxt->node = cobj;

        result = xmlXPathEval(reinterpret_cast<const xmlChar*>(xpath.c_str()), ctxt);
        if (!result)
        {
            xmlXPathFreeContext(ctxt);
            throw InvalidXPath(xpath);
        }
                
        if (type != XPATH_UNDEFINED && result->type != type)
        {
            xmlXPathFreeObject(result);
            xmlXPathFreeContext(ctxt);
            throw Exception("Unsuported query.");
        }
    }

//------------------------------------------------------------------------------
    Node::FindNodeset::~FindNodeset()
    {
        xmlXPathFreeObject(result);
        xmlXPathFreeContext(ctxt);
    }
}

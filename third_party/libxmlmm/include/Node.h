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

#pragma once

#include <string>
#include <vector>
#include <libxml/tree.h>
#include <libxml/xpath.h>

#include "defines.h"

namespace xml
{
    class Element;

    /**
     * XML DOM Node
     *
     * The Node class is the base class for all DOM elements (except Document). 
     * It is effectively a wrapper for xmlNode.
     **/    
    class LIBXMLMM_EXPORT Node
    {
    protected:
        /**
         * Construct Wrapper
         *
         * @param cobj The xmlNode object to wrap.
         **/
        explicit Node(xmlNode* const cobj);
        
    public:
        /**
         * Destructor
         **/
        virtual ~Node();       
        
        /**
         * Get the node's path
         *
         * @return The XPath of this node.
         **/
        std::string get_path() const;

        /**
         * Get the node's parent.
         *
         * @return The node's parent.
         *
         * @throw Exception If the node has no parent node.
         *
         * @note It is safe to assume that all nodes have a parent.
         * If this is not the case something is really broken.
         *
         * @todo What about the root element? Yes it has a document as
         * parent and that is a xmlNode in libxml2, but not in libxmlmm.
         * Wrapping a document xmlNode into a Element may not be the right
         * thing to do.
         *
         * @{
         **/
        Element* get_parent(); 
        const Element* get_parent() const;  
        /** @} **/
        
        /**
         * Find a given node.
         *
         * @param xpath the XPath relative to this node
         *
         * @return the node found
         *
         * @{
         **/
        Node* find_node(const std::string& xpath);
        const Node* find_node(const std::string& xpath) const;
        /** @} **/
        
        /**
         * Find a set of nodes.
         *
         * @param xpath the XPath relative to this node
         *
         * @return the nodes found
         *
         * @todo What about empy result?
         *
         * @{
         **/
        std::vector<Node*> find_nodes(const std::string& xpath);
        std::vector<const Node*> find_nodes(const std::string& xpath) const;
        /** @} **/
        
        /**
         * Query a value.
         *
         * @param xpath the xpath
         *
         * @return the value
         *
         * @todo What about empy result?
         *
         * @{
         **/
        std::string query_string(const std::string& xpath) const;
        double query_number(const std::string& xpath) const;
        /** @} **/
    
        /**
         * Get the value of this node.  Empty if not found.
         **/
        virtual std::string get_value() const = 0;

    protected:    
        /** The wrapped xmlNode object. **/
        xmlNode* cobj;
    
        // Helper object to keep our xpath search context.
        struct FindNodeset 
        {
            FindNodeset(xmlNode *const cobj, const std::string &xpath, const xmlXPathObjectType type = XPATH_UNDEFINED);
            ~FindNodeset();

            operator xmlXPathObject* ()
            { 
                return result; 
            }

            operator xmlNodeSet* ()
            { 
                return result->nodesetval; 
            }

        private:
            xmlXPathContext* ctxt;
            xmlXPathObject* result;
        };

        template <typename NodeType>
        NodeType find(const std::string &xpath) const 
        {
            FindNodeset search(cobj, xpath, XPATH_NODESET);
            const xmlNodeSet* nodeset = search;
            if (!nodeset || nodeset->nodeNr == 0)
            {
                return NULL;
            }
            return reinterpret_cast<NodeType>(nodeset->nodeTab[0]->_private);
        }

        template <typename NodeType>
        std::vector<NodeType> find_all(const std::string &xpath) const 
        {
            FindNodeset search(cobj, xpath, XPATH_NODESET);
            const xmlNodeSet* nodeset = search;
            std::vector<NodeType> nodes;
            if (nodeset != NULL)
            {
                for (int i = 0; i != nodeset->nodeNr; i++)
                {
                    nodes.push_back(reinterpret_cast<NodeType>(nodeset->nodeTab[i]->_private));
                }
            }
            return nodes;
        }

    private:
        Node(const Node&);
        Node& operator = (const Node&);
    };    
}

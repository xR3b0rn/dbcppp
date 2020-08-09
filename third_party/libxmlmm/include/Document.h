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
#include <iosfwd>
#include <libxml/tree.h>

#include "defines.h"
#include "LibXmlSentry.h"
#include "Element.h"

namespace xml
{
    /**
     * XML DOM Document
     *
     * This class is the root class for the XML DOM. 
     *
     * @todo Rename write_to_file and friends to write_file          
     **/    
    class LIBXMLMM_EXPORT Document
    {
    public:    
        /** 
         * Default Constructor
         **/
        Document();
        
        /** 
         * Construct and populate from the specified xml string.
         *
         * @exception exception Throws exception if the xml is invalid.
         *
         * @deprecated This constructor will be fully removed, use read_from_string
         **/
        explicit Document(const std::string &xml);

        /**
         * Destructor
         **/
        ~Document();
        
        /**
         * Check if the document has a root element.
         *
         * @return true if the document has a root element.
         **/
        bool has_root_element() const;
        
        /**
         * Get the root element.
         *
         * @return The root element.
         *
         * @exception NoRootElement Throws NoRootElement if the document has
         * no root element.
         *
         * @{
         **/
        Element* get_root_element();
        const Element* get_root_element() const;
        /** @} **/        
    
        /**
         * Create the root element of a document.
         *
         * @param name The name of the root node.
         *
         * @return The root element.
         *
         * @exception Exception Throws Exception if the document already
         * has a root element.
         **/
        Element* create_root_element(const std::string& name);
        
        /**
         * Write document to string.
         **/
        std::string write_to_string() const;
        
        /**
         * Write document to string.
         **/
        std::string write_to_string(const std::string& encoding) const;
        
        /**
         * Write document to stream.
         **/
        void write_to_stream(std::ostream& os) const;
        
        /**
         * Write document to stream.
         **/
        void write_to_stream(std::ostream& os, const std::string& encoding) const;
        
        /**
         * Write document to file.
         **/
        void write_to_file(const std::string& file) const;
        
        /**
         * Write document to file.
         **/
        void write_to_file(const std::string& file, const std::string& encoding) const;
        
        /**
         * Read document from string.
         *
         * @todo Add error handling.
         **/
        void read_from_string(const std::string& xml);
        
        /**
         * Read document from stream.
         *
         * @todo Add error handling.
         **/
        void read_from_stream(std::istream& is);
        
        /**
         * Read the XML document from file.
         **/
        void read_from_file(const std::string& file);
        
        /**
         * Find a given node.
         *
         * @param xpath the xpath relative to this node
         *
         * @return the node found
         *
         * @{
         **/
        Node* find_node(const std::string& xpath);
        const Node* find_node(const std::string& xpath) const;
        /** @} **/
        
        /**
         * Find a given set of nodes.
         *
         * @param xpath the xpath
         *
         * @return the nodes found
         *
         * @{
         **/
        std::vector<Node*> find_nodes(const std::string& xpath);
        std::vector<const Node*> find_nodes(const std::string& xpath) const;
        /** @} **/
        
        /**
         * Find a given element.
         *
         * @param xpath the xpath relative to this element
         *
         * @return the element found
         *
         * @{
         **/
        Element* find_element(const std::string& xpath);
        const Element* find_element(const std::string& xpath) const;
        /** @} **/
        
        /**
         * Find a given set of elements.
         *
         * @param xpath the xpath relative to this element
         *
         * @return the elements found
         *
         * @{
         **/
        std::vector<Element*> find_elements(const std::string& xpath);
        std::vector<const Element*> find_elements(const std::string& xpath) const;
        /** @} **/ 
        
        /**
         * Query a value.
         *
         * @param xpath the xpath
         *
         * @return the value
         *
         * @{
         **/
        std::string query_string(const std::string& xpath) const;
        double query_number(const std::string& xpath) const;
        /** @} **/

        void validate(const char* mem_kcd, int len) const;
        
    private:
        xmlDoc* cobj;
        
        LibXmlSentry libxml_sentry;
        
        Document(const Document&);
        Document& operator = (const Document&);
    };   
    
    /** 
     * Stream insert operator.
     **/
    LIBXMLMM_EXPORT 
    std::ostream& operator << (std::ostream& os, const Document& doc);
    
    /** 
     * Stream extract operator.
     **/
    LIBXMLMM_EXPORT 
    std::istream& operator >> (std::istream& is, Document& doc);
}

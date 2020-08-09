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
#include <sstream>

#include "Node.h"
#include "Text.h"
#include "exceptions.h"

namespace xml
{
    /**
     * XML Element Wrapper
     **/    
    class LIBXMLMM_EXPORT Element : public Node
    {
    public: 
        /** 
         * Construct the Wrapper
         **/
        explicit Element(xmlNode* const cobj);
        
        /**
         * Get the node's name.  Empty if not found.
         **/
        std::string get_name() const;

        /**
         * Set a node's name.
         **/
        void set_name(const std::string& value);
        
        /**
         * Check if a given attribute exists.
         **/
        bool has_attribute(const std::string& key) const;
                
        /**
         * Get a given attribute in given type.
         *
         * This method will retrive an attribute and try to convert it
         * to the given type with the help of stream operators.
         *
         * @param id the attribtue id
         * @return the attribute value
         *
         * @throws no_such_attribute if the attibute does not exist on
         * this element.
         *
         * @todo add check if conversion to value worked
         **/
        template <typename T>
        T get_attribute(const std::string& id) const
        {
            std::stringstream conv(get_attribute(id));
            
            T value;
            conv >> value;
            if (! conv.eof())
            {
                throw xml::Exception("xml::Element::get_attribute<>: Type conversion failed.");
            }
            
            return value;
        }

        /** 
         * Get a given attribute. 
         *         
         * This method will retrive an attribute.
         *
         * @param id the attribtue id
         * @return the attribute value
         *
         * @throws no_such_attribute if the attibute does not exist on
         * this element.
         **/
        std::string get_attribute(const std::string& key) const;
        
        /**
         * Set an attribute.
         **/
        void set_attribute(const std::string& id, const std::string& value);
               
        /**
         * Set an attribute with generic type.
         **/
        template <typename T>
        void set_attribute(const std::string& id, T value)
        {
            std::stringstream conv;
            conv << value;
            set_attribute(id, conv.str());
        }
        
        /**
         * Remove a given attribute.
         **/
        void remove_attribute(const std::string& key);
        
        /**
         * Get the value of this node.  Empty if not found.
         **/
        virtual std::string get_value() const;
        
        /**
         * Get the element's text.  Empty if not found.
         * 
         * @tood Merge multiple text bits?          
         **/
        std::string get_text() const;

        /**
         * Get the element's text node.
         *
         * @todo Is this funciton really needed?
         **/
        Content* get_text_node() const;
        
        /**
         * Set the 
         **/
        void set_text(const std::string& text);
        
        /**
         * Add/append text to this element.
         **/
        void add_text(const std::string& text);
        
        /**
         * Add a element.
         **/
        Element* add_element(const std::string& name);
        
        /**
         * Get all children of this element.
         *
         * @return All children of this element. 
         **/
        std::vector<Node*> get_children();

        /**
         * Get all children of this element.
         *
         * @return All children of this element. 
         **/
        std::vector<const Node*> get_children() const;

        /**
         * Find a given element.
         *
         * @param xpath the xpath relative to this element
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
         * @return the elements found
         *
         * @{
         **/
        std::vector<Element*> find_elements(const std::string& xpath);
        std::vector<const Element*> find_elements(const std::string& xpath) const;
        /** @} **/  
        
    private:             
        
    };    
}

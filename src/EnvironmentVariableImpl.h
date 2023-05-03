#pragma once

#include "dbcppp/EnvironmentVariable.h"
#include "NodeImpl.h"
#include "AttributeImpl.h"
#include "ValueEncodingDescriptionImpl.h"

namespace dbcppp
{
    class EnvironmentVariableImpl final
        : public IEnvironmentVariable
    {
    public:
        EnvironmentVariableImpl(
              std::string&& name
            , EVarType var_type
            , double minimum
            , double maximum
            , std::string&& unit
            , double initial_value
            , uint64_t ev_id
            , EAccessType access_type
            , std::vector<std::string>&& access_nodes
            , std::vector<ValueEncodingDescriptionImpl>&& value_encoding_descriptions
            , uint64_t data_size
            , std::vector<AttributeImpl>&& attribute_values
            , std::string&& comment);
            
        virtual std::unique_ptr<IEnvironmentVariable> Clone() const override;
        
        virtual const std::string& Name() const override;
        virtual EVarType VarType() const override;
        virtual double Minimum() const override;
        virtual double Maximum() const override;
        virtual const std::string& Unit() const override;
        virtual double InitialValue() const override;
        virtual uint64_t EvId() const override;
        virtual EAccessType AccessType() const override;
        virtual const std::string& AccessNodes_Get(std::size_t i) const override;
        virtual uint64_t AccessNodes_Size() const override;
        virtual const IValueEncodingDescription& ValueEncodingDescriptions_Get(std::size_t i) const override;
        virtual uint64_t ValueEncodingDescriptions_Size() const override;
        virtual uint64_t DataSize() const override;
        virtual const IAttribute& AttributeValues_Get(std::size_t i) const override;
        virtual uint64_t AttributeValues_Size() const override;
        virtual const std::string& Comment() const override;
        
        virtual bool operator==(const IEnvironmentVariable& rhs) const override;
        virtual bool operator!=(const IEnvironmentVariable& rhs) const override;

    private:
        std::string _name;
        EVarType _var_type;
        double _minimum;
        double _maximum;
        std::string _unit;
        double _initial_value;
        uint64_t _ev_id;
        EAccessType _access_type;
        std::vector<std::string> _access_nodes;
        std::vector<ValueEncodingDescriptionImpl> _value_encoding_descriptions;
        uint64_t _data_size;
        std::vector<AttributeImpl> _attribute_values;
        std::string _comment;
    };
}
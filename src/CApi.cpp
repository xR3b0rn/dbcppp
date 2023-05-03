#include "dbcppp/CApi.h"
#include "NetworkImpl.h"
#include "EnvironmentVariableImpl.h"

using namespace dbcppp;

extern "C"
{
    DBCPPP_API const dbcppp_Attribute* dbcppp_AttributeCreate(
        const char* name,
        dbcppp_EObjectType object_type,
        dbcppp_EAttributeValueType value_type,
        const void* value)
    {
        IAttributeDefinition::EObjectType ot;
        switch (object_type)
        {
        case dbcppp_EObjectType::dbcppp_ObjectType_Network:  ot = IAttributeDefinition::EObjectType::Network; break;
        case dbcppp_EObjectType::dbcppp_ObjectType_Node:     ot = IAttributeDefinition::EObjectType::Node; break;
        case dbcppp_EObjectType::dbcppp_ObjectType_Message:  ot = IAttributeDefinition::EObjectType::Message; break;
        case dbcppp_EObjectType::dbcppp_ObjectType_Signal:   ot = IAttributeDefinition::EObjectType::Signal; break;
        default: ot = IAttributeDefinition::EObjectType::EnvironmentVariable; break;
        }
        std::unique_ptr<IAttribute> attribute;
        switch (value_type)
        {
        case dbcppp_EAttributeValueType::dbcppp_AttributeValueType_Int:
            attribute = IAttribute::Create(std::string(name), ot, *reinterpret_cast<const int64_t*>(value));
            break;
        case dbcppp_EAttributeValueType::dbcppp_AttributeValueType_Double:
            attribute = IAttribute::Create(std::string(name), ot, *reinterpret_cast<const double*>(value));
            break;
        case dbcppp_EAttributeValueType::dbcppp_AttributeValueType_String:
            attribute = IAttribute::Create(std::string(name), ot, std::string(reinterpret_cast<const char*>(value)));
            break;
        }
        return reinterpret_cast<const dbcppp_Attribute*>(attribute.release());
    }
    DBCPPP_API void dbcppp_AttributeFree(const dbcppp_Attribute* attribute)
    {
        std::unique_ptr<const AttributeImpl>(reinterpret_cast<const AttributeImpl*>(attribute));
    }
    DBCPPP_API const char* dbcppp_AttributeName(const dbcppp_Attribute* attribute)
    {
        auto ai = reinterpret_cast<const AttributeImpl*>(attribute);
        return ai->Name().c_str();
    }
    DBCPPP_API dbcppp_EObjectType dbcppp_AttributeObjectType(const dbcppp_Attribute* attribute)
    {
        auto ai = reinterpret_cast<const AttributeImpl*>(attribute);
        switch (ai->ObjectType())
        {
        case IAttributeDefinition::EObjectType::Network: return dbcppp_EObjectType::dbcppp_ObjectType_Network;
        case IAttributeDefinition::EObjectType::Node: return dbcppp_EObjectType::dbcppp_ObjectType_Node;
        case IAttributeDefinition::EObjectType::Message: return dbcppp_EObjectType::dbcppp_ObjectType_Message;
        case IAttributeDefinition::EObjectType::Signal: return dbcppp_EObjectType::dbcppp_ObjectType_Signal;
        default: return dbcppp_EObjectType::dbcppp_ObjectType_EnvironmentVariable;
        }
    }
    DBCPPP_API dbcppp_EAttributeValueType dbcppp_AttributeValueType(const dbcppp_Attribute* attribute)
    {
        auto ai = reinterpret_cast<const AttributeImpl*>(attribute);
        if (std::get_if<int64_t>(&ai->Value()))
        {
            return dbcppp_EAttributeValueType::dbcppp_AttributeValueType_Int;
        }
        else if (std::get_if<double>(&ai->Value()))
        {
            return dbcppp_EAttributeValueType::dbcppp_AttributeValueType_Double;
        }
        else
        {
            return dbcppp_EAttributeValueType::dbcppp_AttributeValueType_String;
        }
    }
    DBCPPP_API int64_t dbcppp_AttributeValueAsInt(const dbcppp_Attribute* attribute)
    {
        auto ai = reinterpret_cast<const AttributeImpl*>(attribute);
        return std::get<int64_t>(ai->Value());
    }
    DBCPPP_API double dbcppp_AttributeValueAsDouble(const dbcppp_Attribute* attribute)
    {
        auto ai = reinterpret_cast<const AttributeImpl*>(attribute);
        return std::get<double>(ai->Value());
    }
    DBCPPP_API const char* dbcppp_AttributeValueAsString(const dbcppp_Attribute* attribute)
    {
        auto ai = reinterpret_cast<const AttributeImpl*>(attribute);
        return std::get<std::string>(ai->Value()).c_str();
    }

    DBCPPP_API dbcppp_AttributeDefinition* dbcppp_AttributeDefinitionCreate(
          const char* name
        , dbcppp_EObjectType object_type
        , dbcppp_EAttributeDefinitionValueType value_type
        , const void* value)
    {
        IAttributeDefinition::EObjectType ot;
        switch (object_type)
        {
        case dbcppp_EObjectType::dbcppp_ObjectType_Network:  ot = IAttributeDefinition::EObjectType::Network; break;
        case dbcppp_EObjectType::dbcppp_ObjectType_Node:     ot = IAttributeDefinition::EObjectType::Node; break;
        case dbcppp_EObjectType::dbcppp_ObjectType_Message:  ot = IAttributeDefinition::EObjectType::Message; break;
        case dbcppp_EObjectType::dbcppp_ObjectType_Signal:   ot = IAttributeDefinition::EObjectType::Signal; break;
        default: ot = IAttributeDefinition::EObjectType::EnvironmentVariable; break;
        }
        IAttributeDefinition::value_type_t vt;
        switch (value_type)
        {
        case dbcppp_EAttributeDefinitionValueType::dbcppp_AttributeDefinitionValueTypeInt:
        {
            IAttributeDefinition::ValueTypeInt vti;
            vti.minimum = reinterpret_cast<const int64_t*>(value)[0];
            vti.maximum = reinterpret_cast<const int64_t*>(value)[1];
            vt = vti;
            break;
        }
        case dbcppp_EAttributeDefinitionValueType::dbcppp_AttributeDefinitionValueTypeHex:
        {
            IAttributeDefinition::ValueTypeHex vtui;
            vtui.minimum = reinterpret_cast<const uint64_t*>(value)[0];
            vtui.maximum = reinterpret_cast<const uint64_t*>(value)[1];
            vt = vtui;
            break;
        }
        case dbcppp_EAttributeDefinitionValueType::dbcppp_AttributeDefinitionValueTypeFloat:
        {
            IAttributeDefinition::ValueTypeFloat vtd;
            vtd.minimum = reinterpret_cast<const double*>(value)[0];
            vtd.maximum = reinterpret_cast<const double*>(value)[1];
            vt = vtd;
            break;
        }
        case dbcppp_EAttributeDefinitionValueType::dbcppp_AttributeDefinitionValueTypeString:
        {
            IAttributeDefinition::ValueTypeString vts;
            vt = vts;
            break;
        }
        case dbcppp_EAttributeDefinitionValueType::dbcppp_AttributeDefinitionValueTypeEnum:
        {
            IAttributeDefinition::ValueTypeEnum vte;
            const char* const* values = reinterpret_cast<const char* const*>(value);
            for (; *values; values++)
            {
                vte.values.push_back(*values);
            }
            vt = std::move(vte);
            break;
        }
        }
        auto result = IAttributeDefinition::Create(std::string(name), ot, std::move(vt));
        return reinterpret_cast<dbcppp_AttributeDefinition*>(result.release());
    }
    DBCPPP_API void dbcppp_AttributeDefinitionFree(const dbcppp_AttributeDefinition* attribute_defintion)
    {
        std::unique_ptr<const AttributeDefinitionImpl>(reinterpret_cast<const AttributeDefinitionImpl*>(attribute_defintion));
    }
    DBCPPP_API dbcppp_EObjectType dbcppp_AttributeDefinitionObjectType(const dbcppp_AttributeDefinition* attribute_definition)
    {
        auto ad = reinterpret_cast<const AttributeDefinitionImpl*>(attribute_definition);
        switch (ad->ObjectType())
        {
        case IAttributeDefinition::EObjectType::Network:  return dbcppp_EObjectType::dbcppp_ObjectType_Network;
        case IAttributeDefinition::EObjectType::Node:     return dbcppp_EObjectType::dbcppp_ObjectType_Node;
        case IAttributeDefinition::EObjectType::Message:  return dbcppp_EObjectType::dbcppp_ObjectType_Message;
        case IAttributeDefinition::EObjectType::Signal:   return dbcppp_EObjectType::dbcppp_ObjectType_Signal;
        default: return dbcppp_EObjectType::dbcppp_ObjectType_EnvironmentVariable;
        }
    }
    DBCPPP_API const char* dbcppp_AttributeDefinitionName(const dbcppp_AttributeDefinition* attribute_definition)
    {
        auto ad = reinterpret_cast<const AttributeDefinitionImpl*>(attribute_definition);
        return ad->Name().c_str();
    }
    DBCPPP_API dbcppp_EAttributeDefinitionValueType dbcppp_AttributeDefinitionValueType(const dbcppp_AttributeDefinition* attribute_definition)
    {
        auto ad = reinterpret_cast<const AttributeDefinitionImpl*>(attribute_definition);
        if (std::get_if<IAttributeDefinition::ValueTypeInt>(&ad->ValueType()))
        {
            return dbcppp_EAttributeDefinitionValueType::dbcppp_AttributeDefinitionValueTypeInt;
        }
        else if (std::get_if<IAttributeDefinition::ValueTypeHex>(&ad->ValueType()))
        {
            return dbcppp_EAttributeDefinitionValueType::dbcppp_AttributeDefinitionValueTypeHex;
        }
        else if (std::get_if<IAttributeDefinition::ValueTypeFloat>(&ad->ValueType()))
        {
            return dbcppp_EAttributeDefinitionValueType::dbcppp_AttributeDefinitionValueTypeFloat;
        }
        else if (std::get_if<IAttributeDefinition::ValueTypeString>(&ad->ValueType()))
        {
            return dbcppp_EAttributeDefinitionValueType::dbcppp_AttributeDefinitionValueTypeString;
        }
        else
        {
            return dbcppp_EAttributeDefinitionValueType::dbcppp_AttributeDefinitionValueTypeEnum;
        }
    }
    DBCPPP_API int64_t dbcppp_AttributeDefinitionValueTypeAsInt_Minimum(const dbcppp_AttributeDefinition* attribute_definition)
    {
        auto ad = reinterpret_cast<const AttributeDefinitionImpl*>(attribute_definition);
        return std::get<IAttributeDefinition::ValueTypeInt>(ad->ValueType()).minimum;
    }
    DBCPPP_API int64_t dbcppp_AttributeDefinitionValueTypeAsInt_Maximum(const dbcppp_AttributeDefinition* attribute_definition)
    {
        auto ad = reinterpret_cast<const AttributeDefinitionImpl*>(attribute_definition);
        return std::get<IAttributeDefinition::ValueTypeInt>(ad->ValueType()).maximum;
    }
    DBCPPP_API uint64_t dbcppp_AttributeDefinitionValueTypeAsHex_Minimum(const dbcppp_AttributeDefinition* attribute_definition)
    {
        auto ad = reinterpret_cast<const AttributeDefinitionImpl*>(attribute_definition);
        return std::get<IAttributeDefinition::ValueTypeHex>(ad->ValueType()).minimum;
    }
    DBCPPP_API uint64_t dbcppp_AttributeDefinitionValueTypeAsHex_Maximum(const dbcppp_AttributeDefinition* attribute_definition)
    {
        auto ad = reinterpret_cast<const AttributeDefinitionImpl*>(attribute_definition);
        return std::get<IAttributeDefinition::ValueTypeHex>(ad->ValueType()).maximum;
    }
    DBCPPP_API double dbcppp_AttributeDefinitionValueTypeAsFloat_Minimum(const dbcppp_AttributeDefinition* attribute_definition)
    {
        auto ad = reinterpret_cast<const AttributeDefinitionImpl*>(attribute_definition);
        return std::get<IAttributeDefinition::ValueTypeFloat>(ad->ValueType()).minimum;
    }
    DBCPPP_API double dbcppp_AttributeDefinitionValueTypeAsFloat_Maximum(const dbcppp_AttributeDefinition* attribute_definition)
    {
        auto ad = reinterpret_cast<const AttributeDefinitionImpl*>(attribute_definition);
        return std::get<IAttributeDefinition::ValueTypeFloat>(ad->ValueType()).maximum;
    }
    DBCPPP_API const char* dbcppp_AttributeDefinitionValueTypeAsEnum_Get(const dbcppp_AttributeDefinition* attribute_definition, uint64_t i)
    {
        auto ad = reinterpret_cast<const AttributeDefinitionImpl*>(attribute_definition);
        const auto& vte = std::get<IAttributeDefinition::ValueTypeEnum>(ad->ValueType());
        return vte.values[i].c_str();
    }
    DBCPPP_API uint64_t dbcppp_AttributeDefinitionValueTypeAsEnum_Size(const dbcppp_AttributeDefinition* attribute_definition)
    {
        auto ad = reinterpret_cast<const AttributeDefinitionImpl*>(attribute_definition);
        const auto& vte = std::get<IAttributeDefinition::ValueTypeEnum>(ad->ValueType());
        return vte.values.size();
    }

    DBCPPP_API const dbcppp_BitTiming* dbcppp_BitTimingCreate(uint64_t baudrate, uint64_t BTR1, uint64_t BTR2)
    {
        auto bit_timing = IBitTiming::Create(baudrate, BTR1, BTR2);
        return reinterpret_cast<const dbcppp_BitTiming*>(bit_timing.release());
    }
    DBCPPP_API void dbcppp_BitTimingFree(const dbcppp_BitTiming* bit_timing)
    {
        std::unique_ptr<const BitTimingImpl>(reinterpret_cast<const BitTimingImpl*>(bit_timing));
    }
    DBCPPP_API uint64_t dbcppp_BitTimingBaudrate(const dbcppp_BitTiming* bit_timing)
    {
        auto bti = reinterpret_cast<const BitTimingImpl*>(bit_timing);
        return bti->Baudrate();
    }
    DBCPPP_API uint64_t dbcppp_BitTimingBTR1(const dbcppp_BitTiming* bit_timing)
    {
        auto bti = reinterpret_cast<const BitTimingImpl*>(bit_timing);
        return bti->BTR1();
    }
    DBCPPP_API uint64_t dbcppp_BitTimingBTR2(const dbcppp_BitTiming* bit_timing)
    {
        auto bti = reinterpret_cast<const BitTimingImpl*>(bit_timing);
        return bti->BTR2();
    }

    DBCPPP_API const dbcppp_EnvironmentVariable* dbcppp_EnvironmentVariableCreate(
          const char* name
        , dbcppp_EEnvironmentVariableVarType var_type
        , double minimum
        , double maximum
        , const char* unit
        , double initial_value
        , uint64_t ev_id
        , dbcppp_EEnvironmentVariableAccessType access_type
        , const char** access_nodes
        , dbcppp_ValueEncodingDescription** value_encoding_descriptions
        , uint64_t data_size
        , dbcppp_Attribute** attribute_values
        , const char* comment)
    {
        IEnvironmentVariable::EVarType vt;
        IEnvironmentVariable::EAccessType at;
        std::vector<std::string> ans;
        std::vector<std::unique_ptr<IValueEncodingDescription>> vds;
        std::vector<std::unique_ptr<IAttribute>> avs;
        switch (var_type)
        {
        case dbcppp_EEnvironmentVariableVarType::dbcppp_EnvironmentVariableVarTypeInteger:
            vt = IEnvironmentVariable::EVarType::Integer;
            break;
        case dbcppp_EEnvironmentVariableVarType::dbcppp_EnvironmentVariableVarTypeFloat:
            vt = IEnvironmentVariable::EVarType::Float;
            break;
        case dbcppp_EEnvironmentVariableVarType::dbcppp_EnvironmentVariableVarTypeString:
            vt = IEnvironmentVariable::EVarType::String;
            break;
        default:
            vt = IEnvironmentVariable::EVarType::Data;
            break;
        }
        switch (access_type)
        {
        case dbcppp_EEnvironmentVariableAccessType::dbcppp_EnvironmentVariableAccessTypeUnrestricted:
            at = IEnvironmentVariable::EAccessType::Unrestricted;
            break;
        case dbcppp_EEnvironmentVariableAccessType::dbcppp_EnvironmentVariableAccessTypeRead:
            at = IEnvironmentVariable::EAccessType::Read;
            break;
        case dbcppp_EEnvironmentVariableAccessType::dbcppp_EnvironmentVariableAccessTypeWrite:
            at = IEnvironmentVariable::EAccessType::Write;
            break;
        default:
            at = IEnvironmentVariable::EAccessType::ReadWrite;
            break;
        }
        for (; *access_nodes; access_nodes++)
        {
            ans.push_back(*access_nodes);
            *access_nodes = nullptr;
        }
        for (; *value_encoding_descriptions; value_encoding_descriptions++)
        {
            ValueEncodingDescriptionImpl* ved = reinterpret_cast<ValueEncodingDescriptionImpl*>(*value_encoding_descriptions);
            vds.push_back(std::unique_ptr<ValueEncodingDescriptionImpl>(ved));
            *value_encoding_descriptions = nullptr;
        }
        for (; *attribute_values; attribute_values++)
        {
            AttributeImpl* attr = reinterpret_cast<AttributeImpl*>(*attribute_values);
            avs.push_back(std::unique_ptr<AttributeImpl>(attr));
            *attribute_values = nullptr;
        }
        auto environment_variable = IEnvironmentVariable::Create(
              std::string(name)
            , vt
            , minimum
            , maximum
            , std::string(unit)
            , initial_value
            , ev_id
            , at
            , std::move(ans)
            , std::move(vds)
            , data_size
            , std::move(avs)
            , std::string(comment));
        return reinterpret_cast<const dbcppp_EnvironmentVariable*>(environment_variable.release());
    }
    DBCPPP_API void dbcppp_EnvironmentVariableFree(const dbcppp_EnvironmentVariable* env_var)
    {
        std::unique_ptr<const EnvironmentVariableImpl>(reinterpret_cast<const EnvironmentVariableImpl*>(env_var));
    }
    DBCPPP_API const char* dbcppp_EnvironmentVariableName(const dbcppp_EnvironmentVariable* env_var)
    {
        auto env_var_i = reinterpret_cast<const EnvironmentVariableImpl*>(env_var);
        return env_var_i->Name().c_str();
    }
    DBCPPP_API dbcppp_EEnvironmentVariableVarType dbcppp_EnvironmentVariableVarType(const dbcppp_EnvironmentVariable* env_var)
    {
        auto env_var_i = reinterpret_cast<const EnvironmentVariableImpl*>(env_var);
        switch (env_var_i->VarType())
        {
        case IEnvironmentVariable::EVarType::Integer: return dbcppp_EEnvironmentVariableVarType::dbcppp_EnvironmentVariableVarTypeInteger;
        case IEnvironmentVariable::EVarType::Float: return dbcppp_EEnvironmentVariableVarType::dbcppp_EnvironmentVariableVarTypeFloat;
        case IEnvironmentVariable::EVarType::String: return dbcppp_EEnvironmentVariableVarType::dbcppp_EnvironmentVariableVarTypeString;
        default: return dbcppp_EEnvironmentVariableVarType::dbcppp_EnvironmentVariableVarTypeData;
        }
    }
    DBCPPP_API double dbcppp_EnvironmentVariableMinimum(const dbcppp_EnvironmentVariable* env_var)
    {
        auto env_var_i = reinterpret_cast<const EnvironmentVariableImpl*>(env_var);
        return env_var_i->Minimum();
    }
    DBCPPP_API double dbcppp_EnvironmentVariableMaximum(const dbcppp_EnvironmentVariable* env_var)
    {
        auto env_var_i = reinterpret_cast<const EnvironmentVariableImpl*>(env_var);
        return env_var_i->Maximum();
    }
    DBCPPP_API const char* dbcppp_EnvironmentVariableUnit(const dbcppp_EnvironmentVariable* env_var)
    {
        auto env_var_i = reinterpret_cast<const EnvironmentVariableImpl*>(env_var);
        return env_var_i->Unit().c_str();
    }
    DBCPPP_API double dbcppp_EnvironmentVariableInitialValue(const dbcppp_EnvironmentVariable* env_var)
    {
        auto env_var_i = reinterpret_cast<const EnvironmentVariableImpl*>(env_var);
        return env_var_i->InitialValue();
    }
    DBCPPP_API uint64_t dbcppp_EnvironmentVariableEvId(const dbcppp_EnvironmentVariable* env_var)
    {
        auto env_var_i = reinterpret_cast<const EnvironmentVariableImpl*>(env_var);
        return env_var_i->EvId();
    }
    DBCPPP_API dbcppp_EEnvironmentVariableAccessType dbcppp_EnvironmentVariableAccessType(const dbcppp_EnvironmentVariable* env_var)
    {
        auto env_var_i = reinterpret_cast<const EnvironmentVariableImpl*>(env_var);
        switch (env_var_i->AccessType())
        {
        case IEnvironmentVariable::EAccessType::Unrestricted: return dbcppp_EEnvironmentVariableAccessType::dbcppp_EnvironmentVariableAccessTypeUnrestricted;
        case IEnvironmentVariable::EAccessType::Read: return dbcppp_EEnvironmentVariableAccessType::dbcppp_EnvironmentVariableAccessTypeRead;
        case IEnvironmentVariable::EAccessType::Write: return dbcppp_EEnvironmentVariableAccessType::dbcppp_EnvironmentVariableAccessTypeWrite;
        default: return dbcppp_EEnvironmentVariableAccessType::dbcppp_EnvironmentVariableAccessTypeReadWrite;
        }
    }
    DBCPPP_API const char* dbcppp_EnvironmentVariableAccessNodes_Get(const dbcppp_EnvironmentVariable* env_var, uint64_t i)
    {
        auto env_var_i = reinterpret_cast<const EnvironmentVariableImpl*>(env_var);
        return env_var_i->AccessNodes_Get(i).c_str();
    }
    DBCPPP_API uint64_t dbcppp_EnvironmentVariableAccessNodes_Size(const dbcppp_EnvironmentVariable* env_var)
    {
        auto env_var_i = reinterpret_cast<const EnvironmentVariableImpl*>(env_var);
        return env_var_i->AccessNodes_Size();
    }
    DBCPPP_API uint64_t dbcppp_EnvironmentVariableValueEncodingDescriptions_GetValue(const dbcppp_EnvironmentVariable* env_var, int64_t i)
    {
        auto env_var_i = reinterpret_cast<const EnvironmentVariableImpl*>(env_var);
        return env_var_i->ValueEncodingDescriptions_Get(i).Value();
    }
    DBCPPP_API const char* dbcppp_EnvironmentVariableValueEncodingDescriptions_GetDescription(const dbcppp_EnvironmentVariable* env_var, int64_t i)
    {
        auto env_var_i = reinterpret_cast<const EnvironmentVariableImpl*>(env_var);
        return env_var_i->ValueEncodingDescriptions_Get(i).Description().c_str();
    }
    DBCPPP_API uint64_t dbcppp_EnvironmentVariableValueEncodingDescriptions_Size(const dbcppp_EnvironmentVariable* env_var)
    {
        auto env_var_i = reinterpret_cast<const EnvironmentVariableImpl*>(env_var);
        return env_var_i->ValueEncodingDescriptions_Size();
    }
    DBCPPP_API uint64_t dbcppp_EnvironmentVariableDataSize(const dbcppp_EnvironmentVariable* env_var)
    {
        auto env_var_i = reinterpret_cast<const EnvironmentVariableImpl*>(env_var);
        return env_var_i->DataSize();
    }
    DBCPPP_API const dbcppp_Attribute* dbcppp_EnvironmentVariableAttributeValues_Get(const dbcppp_EnvironmentVariable* env_var, uint64_t i)
    {
        auto env_var_i = reinterpret_cast<const EnvironmentVariableImpl*>(env_var);
        return reinterpret_cast<const dbcppp_Attribute*>(&env_var_i->AttributeValues_Get(i));
    }
    DBCPPP_API uint64_t dbcppp_EnvironmentVariableAttributeValues_Size(const dbcppp_EnvironmentVariable* env_var)
    {
        auto env_var_i = reinterpret_cast<const EnvironmentVariableImpl*>(env_var);
        return env_var_i->AttributeValues_Size();
    }
    DBCPPP_API const char* dbcppp_EnvironmentVariableComment(const dbcppp_EnvironmentVariable* env_var)
    {
        auto env_var_i = reinterpret_cast<const EnvironmentVariableImpl*>(env_var);
        return env_var_i->Comment().c_str();
    }

    DBCPPP_API const dbcppp_Message* dbcppp_MessageCreate(
          uint64_t id
        , const char* name
        , uint64_t message_size
        , const char* transmitter
        , const char** message_transmitters
        , dbcppp_Signal** signals
        , dbcppp_Attribute** attribute_values
        , const char* comment)
    {
        std::vector<std::string> msg_trans;
        std::vector<std::unique_ptr<ISignal>> sigs;
        std::vector<std::unique_ptr<IAttribute>> attrs;
        for (; *message_transmitters; message_transmitters++)
        {
            msg_trans.push_back(*message_transmitters);
            *message_transmitters = nullptr;
        }
        for (; *signals; signals++)
        {
            SignalImpl* si = reinterpret_cast<SignalImpl*>(*signals);
            sigs.push_back(std::unique_ptr<SignalImpl>(si));
            *signals = nullptr;
        }
        for (; *attribute_values; attribute_values++)
        {
            AttributeImpl* attr = reinterpret_cast<AttributeImpl*>(*attribute_values);
            attrs.push_back(std::unique_ptr<AttributeImpl>(attr));
            *attribute_values = nullptr;
        }
        auto result = IMessage::Create(
              id
            , std::string(name)
            , message_size
            , std::string(transmitter)
            , std::move(msg_trans)
            , std::move(sigs)
            , std::move(attrs)
            , std::string(comment)
            , {});
        return reinterpret_cast<const dbcppp_Message*>(result.release());
    }
    DBCPPP_API void dbcppp_MessageFree(const dbcppp_Message* msg)
    {
        std::unique_ptr<const MessageImpl>(reinterpret_cast<const MessageImpl*>(msg));
    }
    DBCPPP_API uint64_t dbcppp_MessageId(const dbcppp_Message* msg)
    {
        auto msgi = reinterpret_cast<const MessageImpl*>(msg);
        return msgi->Id();
    }
    DBCPPP_API const char* dbcppp_MessageName(const dbcppp_Message* msg)
    {
        auto msgi = reinterpret_cast<const MessageImpl*>(msg);
        return msgi->Name().c_str();
    }
    DBCPPP_API uint64_t dbcppp_MessageMessageSize(const dbcppp_Message* msg)
    {
        auto msgi = reinterpret_cast<const MessageImpl*>(msg);
        return msgi->MessageSize();
    }
    DBCPPP_API const char* dbcppp_MessageTransmitter(const dbcppp_Message* msg)
    {
        auto msgi = reinterpret_cast<const MessageImpl*>(msg);
        return msgi->Transmitter().c_str();
    }
    DBCPPP_API const char* dbcppp_MessageTransmitters_Get(const dbcppp_Message* msg, uint64_t i)
    {
        auto msgi = reinterpret_cast<const MessageImpl*>(msg);
        return msgi->MessageTransmitters_Get(i).c_str();
    }
    DBCPPP_API uint64_t dbcppp_MessageTransmitters_Size(const dbcppp_Message* msg)
    {
        auto msgi = reinterpret_cast<const MessageImpl*>(msg);
        return msgi->MessageTransmitters_Size();
    }
    DBCPPP_API const dbcppp_Signal* dbcppp_MessageSignals_Get(const dbcppp_Message* msg, uint64_t i)
    {
        auto msgi = reinterpret_cast<const MessageImpl*>(msg);
        return reinterpret_cast<const dbcppp_Signal*>(&msgi->Signals_Get(i));
    }
    DBCPPP_API uint64_t dbcppp_MessageSignals_Size(const dbcppp_Message* msg)
    {
        auto msgi = reinterpret_cast<const MessageImpl*>(msg);
        return msgi->Signals_Size();
    }
    DBCPPP_API const dbcppp_Attribute* dbcppp_MessageAttributeValues_Get(const dbcppp_Message* msg, uint64_t i)
    {
        auto msgi = reinterpret_cast<const MessageImpl*>(msg);
        return reinterpret_cast<const dbcppp_Attribute*>(&msgi->AttributeValues_Get(i));
    }
    DBCPPP_API uint64_t dbcppp_MessageAttributeValues_Size(const dbcppp_Message* msg)
    {
        auto msgi = reinterpret_cast<const MessageImpl*>(msg);
        return msgi->AttributeValues_Size();
    }
    DBCPPP_API const char* dbcppp_MessageComment(const dbcppp_Message* msg)
    {
        auto msgi = reinterpret_cast<const MessageImpl*>(msg);
        return msgi->Comment().c_str();
    }

    DBCPPP_API const dbcppp_Network* dbcppp_NetworkCreate(
          const char* version
        , const char** new_symbols
        , dbcppp_BitTiming* bit_timing
        , dbcppp_Node** nodes
        , dbcppp_ValueTable** value_tables
        , dbcppp_Message** messages
        , dbcppp_EnvironmentVariable** environment_variables
        , dbcppp_AttributeDefinition** attribute_definitions
        , dbcppp_Attribute** attribute_defaults
        , dbcppp_Attribute** attribute_values
        , const char* comment)
    {
        std::string v(version);
        std::vector<std::string> ns;
        for (; *new_symbols; new_symbols++)
        {
            ns.push_back(*new_symbols);
            *new_symbols = nullptr;
        }
        std::unique_ptr<IBitTiming> bt(reinterpret_cast<BitTimingImpl*>(bit_timing));
        std::vector<std::unique_ptr<INode>> n;
        for (; *nodes; nodes++)
        {
            NodeImpl* ni = reinterpret_cast<NodeImpl*>(*nodes);
            n.push_back(std::unique_ptr<INode>(ni));
            *nodes = nullptr;
        }
        std::vector<std::unique_ptr<IValueTable>> vt;
        for (; *value_tables; value_tables++)
        {
            ValueTableImpl* vti = reinterpret_cast<ValueTableImpl*>(*value_tables);
            vt.push_back(std::unique_ptr<IValueTable>(vti));
            *value_tables = nullptr;
        }
        std::vector<std::unique_ptr<IMessage>> m;
        for (; *messages; messages++)
        {
            MessageImpl* mi = reinterpret_cast<MessageImpl*>(*messages);
            m.push_back(std::unique_ptr<IMessage>(mi));
            *messages = nullptr;
        }
        std::vector<std::unique_ptr<IEnvironmentVariable>> ev;
        for (; *environment_variables; environment_variables++)
        {
            EnvironmentVariableImpl* evi = reinterpret_cast<EnvironmentVariableImpl*>(*environment_variables);
            ev.push_back(std::unique_ptr<IEnvironmentVariable>(evi));
            *environment_variables = nullptr;
        }
        std::vector<std::unique_ptr<IAttributeDefinition>> adef;
        for (; *attribute_definitions; attribute_definitions++)
        {
            AttributeDefinitionImpl* adefi = reinterpret_cast<AttributeDefinitionImpl*>(*attribute_definitions);
            adef.push_back(std::unique_ptr<IAttributeDefinition>(adefi));
            *attribute_definitions = nullptr;
        }
        std::vector<std::unique_ptr<IAttribute>> ad;
        for (; *attribute_defaults; attribute_defaults++)
        {
            AttributeImpl* adi = reinterpret_cast<AttributeImpl*>(*attribute_defaults);
            ad.push_back(std::unique_ptr<IAttribute>(adi));
            *attribute_defaults = nullptr;
        }
        std::vector<std::unique_ptr<IAttribute>> av;
        for (; *attribute_values; attribute_values++)
        {
            AttributeImpl* avi = reinterpret_cast<AttributeImpl*>(*attribute_values);
            ad.push_back(std::unique_ptr<IAttribute>(avi));
            *attribute_values = nullptr;
        }
        std::string c(comment);
        auto net = INetwork::Create(
              std::move(v), std::move(ns)
            , std::move(bt), std::move(n)
            , std::move(vt), std::move(m)
            , std::move(ev), std::move(adef)
            , std::move(ad), std::move(av)
            , std::move(c));
        INetwork* result = net.release();
        return reinterpret_cast<const dbcppp_Network*>(result);
    }
    DBCPPP_API void dbcppp_NetworkFree(const dbcppp_Network* network)
    {
        std::unique_ptr<const NetworkImpl>(reinterpret_cast<const NetworkImpl*>(network));
    }
    DBCPPP_API const char* dbcppp_NetworkVersion(const dbcppp_Network* net)
    {
        auto neti = reinterpret_cast<const NetworkImpl*>(net);
        return neti->Version().c_str();
    }
    DBCPPP_API const char* dbcppp_NetworkNewSymbols_Get(const dbcppp_Network* net, uint64_t i)
    {
        auto neti = reinterpret_cast<const NetworkImpl*>(net);
        return neti->NewSymbols_Get(i).c_str();
    }
    DBCPPP_API uint64_t dbcppp_NetworkNewSymbols_Size(const dbcppp_Network* net)
    {
        auto neti = reinterpret_cast<const NetworkImpl*>(net);
        return neti->NewSymbols_Size();
    }
    DBCPPP_API const dbcppp_BitTiming* dbcppp_NetworkBitTiming(const dbcppp_Network* net)
    {
        auto neti = reinterpret_cast<const NetworkImpl*>(net);
        const IBitTiming& bt = neti->BitTiming();
        return reinterpret_cast<const dbcppp_BitTiming*>(&bt);
    }
    DBCPPP_API const dbcppp_Node* dbcppp_NetworkNodes_Get(const dbcppp_Network* net, uint64_t i)
    {
        auto neti = reinterpret_cast<const NetworkImpl*>(net);
        return reinterpret_cast<const dbcppp_Node*>(&neti->Nodes_Get(i));
    }
    DBCPPP_API uint64_t dbcppp_NetworkNodes_Size(const dbcppp_Network* net)
    {
        auto neti = reinterpret_cast<const NetworkImpl*>(net);
        return neti->Nodes_Size();
    }
    DBCPPP_API const dbcppp_ValueTable* dbcppp_NetworkValueTables_Get(const dbcppp_Network* net, uint64_t i)
    {
        auto neti = reinterpret_cast<const NetworkImpl*>(net);
        return reinterpret_cast<const dbcppp_ValueTable*>(&neti->ValueTables_Get(i));
    }
    DBCPPP_API uint64_t dbcppp_NetworkValueTables_Size(const dbcppp_Network* net)
    {
        auto neti = reinterpret_cast<const NetworkImpl*>(net);
        return neti->ValueTables_Size();
    }
    DBCPPP_API const dbcppp_Message* dbcppp_NetworkMessages_Get(const dbcppp_Network* net, uint64_t i)
    {
        auto neti = reinterpret_cast<const NetworkImpl*>(net);
        return reinterpret_cast<const dbcppp_Message*>(&neti->Messages_Get(i));
    }
    DBCPPP_API uint64_t dbcppp_NetworkMessages_Size(const dbcppp_Network* net)
    {
        auto neti = reinterpret_cast<const NetworkImpl*>(net);
        return neti->Messages_Size();
    }
    DBCPPP_API const dbcppp_EnvironmentVariable* dbcppp_NetworkEnvironmentVariables_Get(const dbcppp_Network* net, uint64_t i)
    {
        auto neti = reinterpret_cast<const NetworkImpl*>(net);
        return reinterpret_cast<const dbcppp_EnvironmentVariable*>(&neti->EnvironmentVariables_Get(i));
    }
    DBCPPP_API uint64_t dbcppp_NetworkEnvironmentVariables_Size(const dbcppp_Network* net)
    {
        auto neti = reinterpret_cast<const NetworkImpl*>(net);
        return neti->EnvironmentVariables_Size();
    }
    DBCPPP_API const dbcppp_AttributeDefinition* dbcppp_NetworkAttributeDefinitions_Get(const dbcppp_Network* net, uint64_t i)
    {
        auto neti = reinterpret_cast<const NetworkImpl*>(net);
        return reinterpret_cast<const dbcppp_AttributeDefinition*>(&neti->AttributeDefinitions_Get(i));
    }
    DBCPPP_API uint64_t dbcppp_NetworkAttributeDefinitions_Size(const dbcppp_Network* net)
    {
        auto neti = reinterpret_cast<const NetworkImpl*>(net);
        return neti->AttributeDefinitions_Size();
    }
    DBCPPP_API const dbcppp_Attribute* dbcppp_NetworkAttributeDefaults_Get(const dbcppp_Network* net, uint64_t i)
    {
        auto neti = reinterpret_cast<const NetworkImpl*>(net);
        return reinterpret_cast<const dbcppp_Attribute*>(&neti->AttributeDefaults_Get(i));
    }
    DBCPPP_API uint64_t dbcppp_NetworkAttributeDefaults_Size(const dbcppp_Network* net)
    {
        auto neti = reinterpret_cast<const NetworkImpl*>(net);
        return neti->AttributeDefaults_Size();
    }
    DBCPPP_API const dbcppp_Attribute* dbcppp_NetworkAttributeValues_Get(const dbcppp_Network* net, uint64_t i)
    {
        auto neti = reinterpret_cast<const NetworkImpl*>(net);
        return reinterpret_cast<const dbcppp_Attribute*>(&neti->AttributeValues_Get(i));
    }
    DBCPPP_API uint64_t dbcppp_NetworkAttributeValues_Size(const dbcppp_Network* net)
    {
        auto neti = reinterpret_cast<const NetworkImpl*>(net);
        return neti->AttributeValues_Size();
    }
    DBCPPP_API const char* dbcppp_NetworkComment(const dbcppp_Network* net)
    {
        auto neti = reinterpret_cast<const NetworkImpl*>(net);
        return neti->Comment().c_str();
    }

    DBCPPP_API const dbcppp_Node* dbcppp_NodeCreate(const char* name, const char* comment, dbcppp_Attribute** attributes)
    {
        std::vector<std::unique_ptr<IAttribute>> attrs;
        for (; *attributes; attributes++)
        {
            AttributeImpl* ai = reinterpret_cast<AttributeImpl*>(*attributes);
            attrs.push_back(std::unique_ptr<IAttribute>(ai));
            *attributes = nullptr;
        }
        auto result = INode::Create(std::string(name), std::string(comment), std::move(attrs));
        return reinterpret_cast<const dbcppp_Node*>(result.release());
    }
    DBCPPP_API void dbcppp_NodeFree(const dbcppp_Node* node)
    {
        std::unique_ptr<const NodeImpl>(reinterpret_cast<const NodeImpl*>(node));
    }
    DBCPPP_API const char* dbcppp_NodeName(const dbcppp_Node* node)
    {
        auto ni = reinterpret_cast<const NodeImpl*>(node);
        return ni->Name().c_str();
    }
    DBCPPP_API const dbcppp_Attribute* dbcppp_NodeAttributeValues_Get(const dbcppp_Network* net, uint64_t i)
    {
        auto ni = reinterpret_cast<const NodeImpl*>(net);
        return reinterpret_cast<const dbcppp_Attribute*>(&ni->AttributeValues_Get(i));
    }
    DBCPPP_API uint64_t dbcppp_NodeAttributeValues_Size(const dbcppp_Network* net)
    {
        auto ni = reinterpret_cast<const NodeImpl*>(net);
        return ni->AttributeValues_Size();
    }
    DBCPPP_API const char* dbcppp_NodeComment(const dbcppp_Node* node)
    {
        auto ni = reinterpret_cast<const NodeImpl*>(node);
        return ni->Comment().c_str();
    }

    DBCPPP_API const dbcppp_Signal* dbcppp_SignalCreate(
          uint64_t message_size
        , const char* name
        , dbcppp_ESignalMultiplexer multiplexer_indicator
        , uint64_t multiplexer_switch_value
        , uint64_t start_bit
        , uint64_t bit_size
        , dbcppp_ESignalByteOrder byte_order
        , dbcppp_ESignalValueType value_type
        , double factor
        , double offset
        , double minimum
        , double maximum
        , const char* unit
        , const char** receivers
        , dbcppp_Attribute** attribute_values
        , dbcppp_ValueEncodingDescription** value_encoding_descriptions
        , const char* comment
        , dbcppp_ESignalExtendedValueType extended_value_type)
    {
        ISignal::EMultiplexer m;
        ISignal::EByteOrder bo;
        ISignal::EValueType vt;
        std::vector<std::string> rs;
        std::vector<std::unique_ptr<IAttribute>> avs;
        std::vector<std::unique_ptr<IValueEncodingDescription>> veds;
        ISignal::EExtendedValueType evt;
        switch (multiplexer_indicator)
        {
        case dbcppp_ESignalMultiplexer::dbcppp_SignalMultiplexerNoMux: m = ISignal::EMultiplexer::NoMux; break;
        case dbcppp_ESignalMultiplexer::dbcppp_SignalMultiplexerMuxSwitch: m = ISignal::EMultiplexer::MuxSwitch; break;
        default: m = ISignal::EMultiplexer::MuxValue; break;
        }
        switch (byte_order)
        {
        case dbcppp_ESignalByteOrder::dbcppp_SignalByteOrderLittleEndian: bo = ISignal::EByteOrder::LittleEndian; break;
        default: bo = ISignal::EByteOrder::BigEndian; break;
        }
        switch (value_type)
        {
        case dbcppp_ESignalValueType::dbcppp_SignalValueTypeSigned: vt = ISignal::EValueType::Signed; break;
        default: vt = ISignal::EValueType::Unsigned; break;
        }
        for (; *receivers; receivers++)
        {
            rs.push_back(*receivers);
            *receivers = nullptr;
        }
        for (; *attribute_values; attribute_values++)
        {
            auto av = reinterpret_cast<AttributeImpl*>(*attribute_values);
            avs.push_back(std::unique_ptr<AttributeImpl>(av));
            *attribute_values = nullptr;
        }
        for (; *value_encoding_descriptions; value_encoding_descriptions++)
        {
            auto ved = reinterpret_cast<ValueEncodingDescriptionImpl*>(*value_encoding_descriptions);
            veds.push_back(std::unique_ptr<ValueEncodingDescriptionImpl>(ved));
            *value_encoding_descriptions = nullptr;
        }
        switch (extended_value_type)
        {
        case dbcppp_ESignalExtendedValueType::dbcppp_SignalExtendedValueTypeInteger: evt = ISignal::EExtendedValueType::Integer; break;
        case dbcppp_ESignalExtendedValueType::dbcppp_SignalExtendedValueTypeFloat: evt = ISignal::EExtendedValueType::Float; break;
        default: evt = ISignal::EExtendedValueType::Double; break;
        }
        auto result = ISignal::Create(
              message_size
            , std::string(name)
            , m
            , multiplexer_switch_value
            , start_bit
            , bit_size
            , bo
            , vt
            , factor
            , offset
            , minimum
            , maximum
            , std::string(unit)
            , std::move(rs)
            , std::move(avs)
            , std::move(veds)
            , std::string(comment)
            , evt
            , {});
        return reinterpret_cast<const dbcppp_Signal*>(result.release());
    }
    DBCPPP_API const char* dbcppp_SignalName(const dbcppp_Signal* sig)
    {
        auto sigi = reinterpret_cast<const SignalImpl*>(sig);
        return sigi->Name().c_str();
    }
    DBCPPP_API dbcppp_ESignalMultiplexer dbcppp_SignalMultiplexerIndicator(const dbcppp_Signal* sig)
    {
        auto sigi = reinterpret_cast<const SignalImpl*>(sig);
        switch (sigi->MultiplexerIndicator())
        {
        case ISignal::EMultiplexer::NoMux: return dbcppp_ESignalMultiplexer::dbcppp_SignalMultiplexerNoMux;
        case ISignal::EMultiplexer::MuxSwitch: return dbcppp_ESignalMultiplexer::dbcppp_SignalMultiplexerMuxSwitch;
        default: return dbcppp_ESignalMultiplexer::dbcppp_SignalMultiplexerMuxValue;
        }
    }
    DBCPPP_API uint64_t dbcppp_SignalMultiplexerSwitchValue(const dbcppp_Signal* sig)
    {
        auto sigi = reinterpret_cast<const SignalImpl*>(sig);
        return sigi->MultiplexerSwitchValue();
    }
    DBCPPP_API uint64_t dbcppp_SignalMultiplexerValues_Size(const dbcppp_Signal* sig)
    {
        auto sigi = reinterpret_cast<const SignalImpl*>(sig);
        return sigi->SignalMultiplexerValues_Size();
    }
    DBCPPP_API const dbcppp_SignalMultiplexerValue* dbcppp_SignalMultiplexerValues_Get(const dbcppp_Signal* sig, uint64_t i)
    {
        auto sigi = reinterpret_cast<const SignalImpl*>(sig);
        return reinterpret_cast<const dbcppp_SignalMultiplexerValue*>(&sigi->SignalMultiplexerValues_Get(i));
    }
    DBCPPP_API const char* dbcppp_SignalMultiplexerValue_SwitchName(const dbcppp_SignalMultiplexerValue* mux_val)
    {
        auto sigi = reinterpret_cast<const ISignalMultiplexerValue*>(mux_val);
        return sigi->SwitchName().c_str();
    }
    DBCPPP_API uint64_t dbcppp_SignalMultiplexerValue_ValueRanges_Size(const dbcppp_SignalMultiplexerValue* mux_val)
    {
        auto sigi = reinterpret_cast<const ISignalMultiplexerValue*>(mux_val);
        return sigi->ValueRanges_Size();
    }
    DBCPPP_API const dbcppp_ValueRange* dbcppp_SignalMultiplexerValue_ValueRanges_Get(const dbcppp_SignalMultiplexerValue* mux_val, uint64_t i)
    {
        auto sigi = reinterpret_cast<const ISignalMultiplexerValue*>(mux_val);
        return reinterpret_cast<const dbcppp_ValueRange*>(&sigi->ValueRanges_Get(i));
    }
    DBCPPP_API uint64_t dbcppp_SignalStartBit(const dbcppp_Signal* sig)
    {
        auto sigi = reinterpret_cast<const SignalImpl*>(sig);
        return sigi->StartBit();
    }
    DBCPPP_API uint64_t dbcppp_SignalBitSize(const dbcppp_Signal* sig)
    {
        auto sigi = reinterpret_cast<const SignalImpl*>(sig);
        return sigi->BitSize();
    }
    DBCPPP_API dbcppp_ESignalByteOrder dbcppp_SignalByteOrder(const dbcppp_Signal* sig)
    {
        auto sigi = reinterpret_cast<const SignalImpl*>(sig);
        switch (sigi->ByteOrder())
        {
        case ISignal::EByteOrder::LittleEndian: return dbcppp_ESignalByteOrder::dbcppp_SignalByteOrderLittleEndian;
        default: return dbcppp_ESignalByteOrder::dbcppp_SignalByteOrderBigEndian;
        }
    }
    DBCPPP_API dbcppp_ESignalValueType dbcppp_SignalValueType(const dbcppp_Signal* sig)
    {
        auto sigi = reinterpret_cast<const SignalImpl*>(sig);
        switch (sigi->ValueType())
        {
        case ISignal::EValueType::Signed: return dbcppp_ESignalValueType::dbcppp_SignalValueTypeSigned;
        default: return dbcppp_ESignalValueType::dbcppp_SignalValueTypeUnsigned;
        }
    }
    DBCPPP_API double dbcppp_SignalFactor(const dbcppp_Signal* sig)
    {
        auto sigi = reinterpret_cast<const SignalImpl*>(sig);
        return sigi->Factor();
    }
    DBCPPP_API double dbcppp_SignalOffset(const dbcppp_Signal* sig)
    {
        auto sigi = reinterpret_cast<const SignalImpl*>(sig);
        return sigi->Offset();
    }
    DBCPPP_API double dbcppp_SignalMinimum(const dbcppp_Signal* sig)
    {
        auto sigi = reinterpret_cast<const SignalImpl*>(sig);
        return sigi->Minimum();
    }
    DBCPPP_API double dbcppp_SignalMaximum(const dbcppp_Signal* sig)
    {
        auto sigi = reinterpret_cast<const SignalImpl*>(sig);
        return sigi->Maximum();
    }
    DBCPPP_API const char* dbcppp_SignalUnit(const dbcppp_Signal* sig)
    {
        auto sigi = reinterpret_cast<const SignalImpl*>(sig);
        return sigi->Unit().c_str();
    }
    DBCPPP_API const char* dbcppp_SignalReceivers_Get(const dbcppp_Signal* signal, uint64_t i)
    {
        auto sigi = reinterpret_cast<const SignalImpl*>(signal);
        return sigi->Receivers_Get(i).c_str();
    }
    DBCPPP_API uint64_t dbcppp_SignalReceivers_Size(const dbcppp_Signal* signal)
    {
        auto sigi = reinterpret_cast<const SignalImpl*>(signal);
        return sigi->Receivers_Size();
    }
    DBCPPP_API const dbcppp_ValueEncodingDescription* dbcppp_SignalValueEncodingDescriptions_Get(const dbcppp_Signal* signal, uint64_t i)
    {
        auto sigi = reinterpret_cast<const SignalImpl*>(signal);
        return reinterpret_cast<const dbcppp_ValueEncodingDescription*>(&sigi->ValueEncodingDescriptions_Get(i));
    }
    DBCPPP_API uint64_t dbcppp_SignalValueEncodingDescriptions_Size(const dbcppp_Signal* signal)
    {
        auto sigi = reinterpret_cast<const SignalImpl*>(signal);
        return sigi->ValueEncodingDescriptions_Size();
    }
    DBCPPP_API const dbcppp_Attribute* dbcppp_SignalAttributeValues_Get(const dbcppp_Signal* signal, uint64_t i)
    {
        auto sigi = reinterpret_cast<const SignalImpl*>(signal);
        return reinterpret_cast<const dbcppp_Attribute*>(&sigi->AttributeValues_Get(i));
    }
    DBCPPP_API uint64_t dbcppp_SignalAttributeValues_Size(const dbcppp_Signal* signal)
    {
        auto sigi = reinterpret_cast<const SignalImpl*>(signal);
        return sigi->AttributeValues_Size();
    }
    DBCPPP_API const char* dbcppp_SignalComment(const dbcppp_Signal* sig)
    {
        auto sigi = reinterpret_cast<const SignalImpl*>(sig);
        return sigi->Comment().c_str();
    }
    DBCPPP_API dbcppp_ESignalExtendedValueType dbcppp_SignalExtended_ValueType(const dbcppp_Signal* sig)
    {
        auto sigi = reinterpret_cast<const SignalImpl*>(sig);
        switch (sigi->ExtendedValueType())
        {
        case ISignal::EExtendedValueType::Integer: return dbcppp_ESignalExtendedValueType::dbcppp_SignalExtendedValueTypeInteger;
        case ISignal::EExtendedValueType::Float: return dbcppp_ESignalExtendedValueType::dbcppp_SignalExtendedValueTypeFloat;
        default: return dbcppp_ESignalExtendedValueType::dbcppp_SignalExtendedValueTypeDouble;
        }
    }
    DBCPPP_API uint64_t dbcppp_SignalDecode(const dbcppp_Signal* sig, const void* bytes)
    {
        auto sigi = reinterpret_cast<const SignalImpl*>(sig);
        return sigi->Decode(bytes);
    }
    DBCPPP_API void dbcppp_SignalEncode(const dbcppp_Signal* sig, uint64_t raw, void* buffer)
    {
        auto sigi = reinterpret_cast<const SignalImpl*>(sig);
        sigi->Encode(raw, buffer);
    }
    DBCPPP_API double dbcppp_SignalRawToPhys(const dbcppp_Signal* sig, uint64_t raw)
    {
        auto sigi = reinterpret_cast<const SignalImpl*>(sig);
        return sigi->RawToPhys(raw);
    }
    DBCPPP_API uint64_t dbcppp_SignalPhysToRaw(const dbcppp_Signal* sig, double phys)
    {
        auto sigi = reinterpret_cast<const SignalImpl*>(sig);
        return sigi->PhysToRaw(phys);
    }

    DBCPPP_API const dbcppp_SignalType* dbcppp_SignalTypeCreate(
          const char* name
        , uint64_t signal_size
        , dbcppp_ESignalByteOrder byte_order
        , dbcppp_ESignalValueType value_type
        , double factor
        , double offset
        , double minimum
        , double maximum
        , const char* unit
        , double default_value
        , const char* value_table)
    {
        ISignal::EByteOrder bo;
        ISignal::EValueType vt;
        switch (byte_order)
        {
        case dbcppp_ESignalByteOrder::dbcppp_SignalByteOrderLittleEndian: bo = ISignal::EByteOrder::LittleEndian; break;
        default: bo = ISignal::EByteOrder::BigEndian; break;
        }
        switch (value_type)
        {
        case dbcppp_ESignalValueType::dbcppp_SignalValueTypeSigned: vt = ISignal::EValueType::Signed; break;
        default: vt = ISignal::EValueType::Unsigned; break;
        }
        auto result = ISignalType::Create(
              std::string(name)
            , signal_size
            , bo
            , vt
            , factor
            , offset
            , minimum
            , maximum
            , std::string(unit)
            , default_value
            , std::string(value_table));
        return reinterpret_cast<const dbcppp_SignalType*>(result.release());
    }
    DBCPPP_API const char* dbcppp_SignalTypeName(const dbcppp_SignalType* sig_type)
    {
        auto sti = reinterpret_cast<const SignalTypeImpl*>(sig_type);
        return sti->Name().c_str();
    }
    DBCPPP_API uint64_t dbcppp_SignalTypeSignalSize(const dbcppp_SignalType* sig_type)
    {
        auto sti = reinterpret_cast<const SignalTypeImpl*>(sig_type);
        return sti->SignalSize();
    }
    DBCPPP_API dbcppp_ESignalByteOrder dbcppp_SignalTypeByteOrder(const dbcppp_SignalType* sig_type)
    {
        auto sti = reinterpret_cast<const SignalTypeImpl*>(sig_type);
        switch (sti->ByteOrder())
        {
        case ISignal::EByteOrder::LittleEndian: return dbcppp_ESignalByteOrder::dbcppp_SignalByteOrderLittleEndian;
        default: return dbcppp_ESignalByteOrder::dbcppp_SignalByteOrderBigEndian;
        }
    }
    DBCPPP_API dbcppp_ESignalValueType dbcppp_SignalTypeValueType(const dbcppp_SignalType* sig_type)
    {
        auto sti = reinterpret_cast<const SignalTypeImpl*>(sig_type);
        switch (sti->ValueType())
        {
        case ISignal::EValueType::Signed: return dbcppp_ESignalValueType::dbcppp_SignalValueTypeSigned;
        default: return dbcppp_ESignalValueType::dbcppp_SignalValueTypeUnsigned;
        }
    }
    DBCPPP_API double dbcppp_SignalTypeFactor(const dbcppp_SignalType* sig_type)
    {
        auto sti = reinterpret_cast<const SignalTypeImpl*>(sig_type);
        return sti->Factor();
    }
    DBCPPP_API double dbcppp_SignalTypeOffset(const dbcppp_SignalType* sig_type)
    {
        auto sti = reinterpret_cast<const SignalTypeImpl*>(sig_type);
        return sti->Offset();
    }
    DBCPPP_API double dbcppp_SignalTypeMinimum(const dbcppp_SignalType* sig_type)
    {
        auto sti = reinterpret_cast<const SignalTypeImpl*>(sig_type);
        return sti->Minimum();
    }
    DBCPPP_API double dbcppp_SignalTypeMaximum(const dbcppp_SignalType* sig_type)
    {
        auto sti = reinterpret_cast<const SignalTypeImpl*>(sig_type);
        return sti->Maximum();
    }
    DBCPPP_API const char* dbcppp_SignalTypeUnit(const dbcppp_SignalType* sig_type)
    {
        auto sti = reinterpret_cast<const SignalTypeImpl*>(sig_type);
        return sti->Unit().c_str();
    }
    DBCPPP_API double dbcppp_SignalTypeDefaultValue(const dbcppp_SignalType* sig_type)
    {
        auto sti = reinterpret_cast<const SignalTypeImpl*>(sig_type);
        return sti->DefaultValue();
    }
    DBCPPP_API const char* dbcppp_SignalTypeValueTable(const dbcppp_SignalType* sig_type)
    {
        auto sti = reinterpret_cast<const SignalTypeImpl*>(sig_type);
        return sti->ValueTable().c_str();
    }

    DBCPPP_API const dbcppp_ValueTable* dbcppp_ValueTableCreate(
          const char* name
        , dbcppp_SignalType* signal_type
        , dbcppp_ValueEncodingDescription** value_encoding_descriptions)
    {
        std::vector<std::unique_ptr<IValueEncodingDescription>> veds;
        for (; *value_encoding_descriptions; value_encoding_descriptions++)
        {
            auto ved = std::unique_ptr<ValueEncodingDescriptionImpl>(reinterpret_cast<ValueEncodingDescriptionImpl*>(*value_encoding_descriptions));
            veds.push_back(std::move(ved));
        }
        std::optional<std::unique_ptr<ISignalType>> st;
        if (signal_type)
        {
            st = std::unique_ptr<SignalTypeImpl>(reinterpret_cast<SignalTypeImpl*>(signal_type));
        }
        auto result = IValueTable::Create(std::string(name), std::move(st), std::move(veds));
        return reinterpret_cast<dbcppp_ValueTable*>(result.release());
    }
    DBCPPP_API void dbcppp_ValueTableFree(const dbcppp_ValueTable* value_table)
    {
        std::unique_ptr<const ValueTableImpl>(reinterpret_cast<const ValueTableImpl*>(value_table));
    }
    DBCPPP_API const char* dbcppp_ValueTableName(const dbcppp_ValueTable* value_table)
    {
        auto vti = reinterpret_cast<const ValueTableImpl*>(value_table);
        return vti->Name().c_str();
    }
    DBCPPP_API const dbcppp_SignalType* dbcppp_ValueTableSignalType(const dbcppp_ValueTable* value_table)
    {
        auto vti = reinterpret_cast<const ValueTableImpl*>(value_table);
        const dbcppp_SignalType* result = nullptr;
        if (vti->SignalType())
        {
            result = reinterpret_cast<const dbcppp_SignalType*>(&vti->SignalType()->get());
        }
        return result;
    }
    DBCPPP_API const dbcppp_ValueEncodingDescription* dbcppp_ValueTableValueEncodingDescription_Get(const dbcppp_ValueTable* value_table, uint64_t i)
    {
        auto vti = reinterpret_cast<const ValueTableImpl*>(value_table);
        return reinterpret_cast<const dbcppp_ValueEncodingDescription*>(&vti->ValueEncodingDescriptions_Get(i));
    }
    DBCPPP_API uint64_t dbcppp_ValueTableValueEncodingDescription_Size(const dbcppp_ValueTable* value_table)
    {
        auto vti = reinterpret_cast<const ValueTableImpl*>(value_table);
        return vti->ValueEncodingDescriptions_Size();
    }
    DBCPPP_API const dbcppp_ValueEncodingDescription* dbcppp_ValueEncodingDescriptionCreate(uint64_t value, const char* desc)
    {
        auto ved = IValueEncodingDescription::Create(value, desc);
        const dbcppp_ValueEncodingDescription* cved = reinterpret_cast<dbcppp_ValueEncodingDescription*>(ved.release());
        return cved;
    }
    DBCPPP_API void dbcppp_ValueEncodingDescriptionFree(const dbcppp_ValueEncodingDescription* cved)
    {
        auto ved = reinterpret_cast<const ValueEncodingDescriptionImpl*>(cved);
        std::unique_ptr<const ValueEncodingDescriptionImpl> upved(ved);
    }
    DBCPPP_API uint64_t dbcppp_ValueEncodingDescriptionValue(const dbcppp_ValueEncodingDescription* cved)
    {
        auto ved = reinterpret_cast<const ValueEncodingDescriptionImpl*>(cved);
        return ved->Value();
    }
    DBCPPP_API const char* dbcppp_ValueEncodingDescriptionDescription(const dbcppp_ValueEncodingDescription* cved)
    {
        auto ved = reinterpret_cast<const ValueEncodingDescriptionImpl*>(cved);
        return ved->Description().c_str();
    }
}
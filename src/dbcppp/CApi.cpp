
#include "../../include/dbcppp/CApi.h"
#include "NetworkImpl.h"
#include "EnvironmentVariableImpl.h"

using namespace dbcppp;

extern "C"
{
    DBCPPP_API const dbcppp_Attribute* dbcppp_AttributeCreate(
        const char* name,
        dbcppp_ObjectType object_type,
        dbcppp_AttributeValueType value_type,
        const void* value)
    {
        AttributeDefinition::ObjectType ot;
        switch (object_type)
        {
        case dbcppp_ObjectType::dbcppp_ObjectType_Network:  ot = AttributeDefinition::ObjectType::Network; break;
        case dbcppp_ObjectType::dbcppp_ObjectType_Node:     ot = AttributeDefinition::ObjectType::Node; break;
        case dbcppp_ObjectType::dbcppp_ObjectType_Message:  ot = AttributeDefinition::ObjectType::Message; break;
        case dbcppp_ObjectType::dbcppp_ObjectType_Signal:   ot = AttributeDefinition::ObjectType::Signal; break;
        default: ot = AttributeDefinition::ObjectType::EnvironmentVariable; break;
        }
        std::unique_ptr<Attribute> attribute;
        switch (value_type)
        {
        case dbcppp_AttributeValueType::dbcppp_AttributeValueType_Int:
            attribute = Attribute::create(std::string(name), ot, *reinterpret_cast<const int64_t*>(value));
            break;
        case dbcppp_AttributeValueType::dbcppp_AttributeValueType_Double:
            attribute = Attribute::create(std::string(name), ot, *reinterpret_cast<const double*>(value));
            break;
        case dbcppp_AttributeValueType::dbcppp_AttributeValueType_String:
            attribute = Attribute::create(std::string(name), ot, std::string(reinterpret_cast<const char*>(value)));
            break;
        }
        return reinterpret_cast<const dbcppp_Attribute*>(attribute.release());
    }
    DBCPPP_API void dbcppp_AttributeFree(const dbcppp_Attribute* attribute)
    {
        std::unique_ptr<const AttributeImpl>(reinterpret_cast<const AttributeImpl*>(attribute));
    }
    DBCPPP_API const char* dbcppp_AttributeGetName(const dbcppp_Attribute* attribute)
    {
        auto ai = reinterpret_cast<const AttributeImpl*>(attribute);
        return ai->getName().c_str();
    }
    DBCPPP_API dbcppp_ObjectType dbcppp_AttributeGetObjectType(const dbcppp_Attribute* attribute)
    {
        auto ai = reinterpret_cast<const AttributeImpl*>(attribute);
        switch (ai->getObjectType())
        {
        case AttributeDefinition::ObjectType::Network: return dbcppp_ObjectType::dbcppp_ObjectType_Network;
        case AttributeDefinition::ObjectType::Node: return dbcppp_ObjectType::dbcppp_ObjectType_Node;
        case AttributeDefinition::ObjectType::Message: return dbcppp_ObjectType::dbcppp_ObjectType_Message;
        case AttributeDefinition::ObjectType::Signal: return dbcppp_ObjectType::dbcppp_ObjectType_Signal;
        default: return dbcppp_ObjectType::dbcppp_ObjectType_EnvironmentVariable; 
        }
    }
    DBCPPP_API dbcppp_AttributeValueType dbcppp_AttributeGetValueType(const dbcppp_Attribute* attribute)
    {
        auto ai = reinterpret_cast<const AttributeImpl*>(attribute);
        if (ai->getValue().type() == typeid(int64_t))
        {
            return dbcppp_AttributeValueType::dbcppp_AttributeValueType_Int;
        }
        else if (ai->getValue().type() == typeid(double))
        {
            return dbcppp_AttributeValueType::dbcppp_AttributeValueType_Double;
        }
        else
        {
            return dbcppp_AttributeValueType::dbcppp_AttributeValueType_String;
        }
    }
    DBCPPP_API int64_t dbcppp_AttributeGetValueAsInt(const dbcppp_Attribute* attribute)
    {
        auto ai = reinterpret_cast<const AttributeImpl*>(attribute);
        return boost::get<int64_t>(ai->getValue());
    }
    DBCPPP_API double dbcppp_AttributeGetValueAsDouble(const dbcppp_Attribute* attribute)
    {
        auto ai = reinterpret_cast<const AttributeImpl*>(attribute);
        return boost::get<double>(ai->getValue());
    }
    DBCPPP_API const char* dbcppp_AttributeGetValueAsString(const dbcppp_Attribute* attribute)
    {
        auto ai = reinterpret_cast<const AttributeImpl*>(attribute);
        return boost::get<std::string>(ai->getValue()).c_str();
    }
    
    DBCPPP_API dbcppp_AttributeDefinition* dbcppp_AttributeDefinitionCreate(
          const char* name
        , dbcppp_ObjectType object_type
        , dbcppp_AttributeDefinitionValueType value_type
        , const void* value)
    {
        AttributeDefinition::ObjectType ot;
        switch (object_type)
        {
        case dbcppp_ObjectType::dbcppp_ObjectType_Network: ot = AttributeDefinition::ObjectType::Network; break;
        case dbcppp_ObjectType::dbcppp_ObjectType_Node: ot = AttributeDefinition::ObjectType::Node; break;
        case dbcppp_ObjectType::dbcppp_ObjectType_Message: ot = AttributeDefinition::ObjectType::Message; break;
        case dbcppp_ObjectType::dbcppp_ObjectType_Signal: ot = AttributeDefinition::ObjectType::Signal; break;
        default: ot = AttributeDefinition::ObjectType::EnvironmentVariable; break;
        }
        AttributeDefinition::value_type_t vt;
        switch (value_type)
        {
        case dbcppp_AttributeDefinitionValueType::dbcppp_AttributeDefinitionValueTypeInt:
        {
            AttributeDefinition::ValueTypeInt vti;
            vti.minimum = reinterpret_cast<const int64_t*>(value)[0];
            vti.maximum = reinterpret_cast<const int64_t*>(value)[1];
            vt = vti;
            break;
        }
        case dbcppp_AttributeDefinitionValueType::dbcppp_AttributeDefinitionValueTypeHex:
        {
            AttributeDefinition::ValueTypeHex vtui;
            vtui.minimum = reinterpret_cast<const uint64_t*>(value)[0];
            vtui.maximum = reinterpret_cast<const uint64_t*>(value)[1];
            vt = vtui;
            break;
        }
        case dbcppp_AttributeDefinitionValueType::dbcppp_AttributeDefinitionValueTypeFloat:
        {
            AttributeDefinition::ValueTypeFloat vtd;
            vtd.minimum = reinterpret_cast<const double*>(value)[0];
            vtd.maximum = reinterpret_cast<const double*>(value)[1];
            vt = vtd;
            break;
        }
        case dbcppp_AttributeDefinitionValueType::dbcppp_AttributeDefinitionValueTypeString:
        {
            AttributeDefinition::ValueTypeString vts;
            vt = vts;
            break;
        }
        case dbcppp_AttributeDefinitionValueType::dbcppp_AttributeDefinitionValueTypeEnum:
        {
            AttributeDefinition::ValueTypeEnum vte;
            const char* const* values = reinterpret_cast<const char* const*>(value);
            for (; *values; values++)
            {
                vte.values.push_back(*values);
            }
            vt = std::move(vte);
            break;
        }
        }
        auto result = AttributeDefinition::create(std::string(name), ot, std::move(vt));
        return reinterpret_cast<dbcppp_AttributeDefinition*>(result.release());
    }
    DBCPPP_API void dbcppp_AttributeDefinitionFree(const dbcppp_AttributeDefinition* attribute_defintion)
    {
        std::unique_ptr<const AttributeDefinitionImpl>(reinterpret_cast<const AttributeDefinitionImpl*>(attribute_defintion));
    }
    DBCPPP_API dbcppp_ObjectType dbcppp_AttributeDefinitionGetObjectType(const dbcppp_AttributeDefinition* attribute_definition)
    {
        auto ad = reinterpret_cast<const AttributeDefinitionImpl*>(attribute_definition);
        switch (ad->getObjectType())
        {
        case AttributeDefinition::ObjectType::Network:  return dbcppp_ObjectType::dbcppp_ObjectType_Network;
        case AttributeDefinition::ObjectType::Node:     return dbcppp_ObjectType::dbcppp_ObjectType_Node;
        case AttributeDefinition::ObjectType::Message:  return dbcppp_ObjectType::dbcppp_ObjectType_Message;
        case AttributeDefinition::ObjectType::Signal:   return dbcppp_ObjectType::dbcppp_ObjectType_Signal;
        default: return dbcppp_ObjectType::dbcppp_ObjectType_EnvironmentVariable;
        }
    }
    DBCPPP_API const char* dbcppp_AttributeDefinitionGetName(const dbcppp_AttributeDefinition* attribute_definition)
    {
        auto ad = reinterpret_cast<const AttributeDefinitionImpl*>(attribute_definition);
        return ad->getName().c_str();
    }
    DBCPPP_API dbcppp_AttributeDefinitionValueType dbcppp_AttributeDefinitionGetValueType(const dbcppp_AttributeDefinition* attribute_definition)
    {
        auto ad = reinterpret_cast<const AttributeDefinitionImpl*>(attribute_definition);
        if (ad->getValueType().type() == typeid(AttributeDefinition::ValueTypeInt))
        {
            return dbcppp_AttributeDefinitionValueType::dbcppp_AttributeDefinitionValueTypeInt;
        }
        else if (ad->getValueType().type() == typeid(AttributeDefinition::ValueTypeHex))
        {
            return dbcppp_AttributeDefinitionValueType::dbcppp_AttributeDefinitionValueTypeHex;
        }
        else if (ad->getValueType().type() == typeid(AttributeDefinition::ValueTypeHex))
        {
            return dbcppp_AttributeDefinitionValueType::dbcppp_AttributeDefinitionValueTypeFloat;
        }
        else if (ad->getValueType().type() == typeid(AttributeDefinition::ValueTypeHex))
        {
            return dbcppp_AttributeDefinitionValueType::dbcppp_AttributeDefinitionValueTypeString;
        }
        else
        {
            return dbcppp_AttributeDefinitionValueType::dbcppp_AttributeDefinitionValueTypeEnum;
        }
    }
    DBCPPP_API int64_t dbcppp_AttributeDefinitionGetValueTypeAsInt_Minimum(const dbcppp_AttributeDefinition* attribute_definition)
    {
        auto ad = reinterpret_cast<const AttributeDefinitionImpl*>(attribute_definition);
        return boost::get<AttributeDefinition::ValueTypeInt>(ad->getValueType()).minimum;
    }
    DBCPPP_API int64_t dbcppp_AttributeDefinitionGetValueTypeAsInt_Maximum(const dbcppp_AttributeDefinition* attribute_definition)
    {
        auto ad = reinterpret_cast<const AttributeDefinitionImpl*>(attribute_definition);
        return boost::get<AttributeDefinition::ValueTypeInt>(ad->getValueType()).maximum;
    }
    DBCPPP_API uint64_t dbcppp_AttributeDefinitionGetValueTypeAsHex_Minimum(const dbcppp_AttributeDefinition* attribute_definition)
    {
        auto ad = reinterpret_cast<const AttributeDefinitionImpl*>(attribute_definition);
        return boost::get<AttributeDefinition::ValueTypeHex>(ad->getValueType()).minimum;
    }
    DBCPPP_API uint64_t dbcppp_AttributeDefinitionGetValueTypeAsHex_Maximum(const dbcppp_AttributeDefinition* attribute_definition)
    {
        auto ad = reinterpret_cast<const AttributeDefinitionImpl*>(attribute_definition);
        return boost::get<AttributeDefinition::ValueTypeHex>(ad->getValueType()).maximum;
    }
    DBCPPP_API double dbcppp_AttributeDefinitionGetValueTypeAsFloat_Minimum(const dbcppp_AttributeDefinition* attribute_definition)
    {
        auto ad = reinterpret_cast<const AttributeDefinitionImpl*>(attribute_definition);
        return boost::get<AttributeDefinition::ValueTypeFloat>(ad->getValueType()).minimum;
    }
    DBCPPP_API double dbcppp_AttributeDefinitionGetValueTypeAsFloat_Maximum(const dbcppp_AttributeDefinition* attribute_definition)
    {
        auto ad = reinterpret_cast<const AttributeDefinitionImpl*>(attribute_definition);
        return boost::get<AttributeDefinition::ValueTypeFloat>(ad->getValueType()).maximum;
    }
    DBCPPP_API void dbcppp_AttributeDefinitionForEachValueTypeEnum(const dbcppp_AttributeDefinition* attribute_definition, void(*cb)(const char*, void*), void* data)
    {
        auto ad = reinterpret_cast<const AttributeDefinitionImpl*>(attribute_definition);
        ad->forEachValueTypeEnum(
            [&](const std::string& v)
            {
                cb(v.c_str(), data);
            });
    }
    DBCPPP_API void dbcppp_AttributeDefinitionFindValueTypeEnum(const dbcppp_AttributeDefinition* attribute_definition, bool(*pred)(const char*, void*), void* data)
    {
        auto ad = reinterpret_cast<const AttributeDefinitionImpl*>(attribute_definition);
        ad->findValueTypeEnum(
            [&](const std::string& v)
            {
                return pred(v.c_str(), data);
            });
    }

    DBCPPP_API const dbcppp_BitTiming* dbcppp_BitTimingCreate(uint64_t baudrate, uint64_t BTR1, uint64_t BTR2)
    {
        auto bit_timing = BitTiming::create(baudrate, BTR1, BTR2);
        return reinterpret_cast<const dbcppp_BitTiming*>(bit_timing.release());
    }
    DBCPPP_API void dbcppp_BitTimingFree(const dbcppp_BitTiming* bit_timing)
    {
        std::unique_ptr<const BitTimingImpl>(reinterpret_cast<const BitTimingImpl*>(bit_timing));
    }
    DBCPPP_API uint64_t dbcppp_BitTimingGetBaudrate(const dbcppp_BitTiming* bit_timing)
    {
        auto bti = reinterpret_cast<const BitTimingImpl*>(bit_timing);
        return bti->getBaudrate();
    }
    DBCPPP_API uint64_t dbcppp_BitTimingGetBTR1(const dbcppp_BitTiming* bit_timing)
    {
        auto bti = reinterpret_cast<const BitTimingImpl*>(bit_timing);
        return bti->getBTR1();
    }
    DBCPPP_API uint64_t dbcppp_BitTimingGetBTR2(const dbcppp_BitTiming* bit_timing)
    {
        auto bti = reinterpret_cast<const BitTimingImpl*>(bit_timing);
        return bti->getBTR2();
    }

    DBCPPP_API const dbcppp_EnvironmentVariable* dbcppp_EnvironmentVariableCreate(
          const char* name
        , dbcppp_EnvironmentVariableVarType var_type
        , double minimum
        , double maximum
        , const char* unit
        , double initial_value
        , uint64_t ev_id
        , dbcppp_EnvironmentVariableAccessType access_type
        , const char** access_nodes
        , dbcppp_ValueDescriptionPair** value_descriptions
        , uint64_t data_size
        , dbcppp_Attribute** attribute_values
        , const char* comment)
    {
        EnvironmentVariable::VarType vt;
        EnvironmentVariable::AccessType at;
        std::set<std::string> ans;
        std::map<double, std::string> vds;
        std::map<std::string, std::unique_ptr<Attribute>> avs;
        switch (var_type)
        {
        case dbcppp_EnvironmentVariableVarType::dbcppp_EnvironmentVariableVarTypeInteger:
            vt = EnvironmentVariable::VarType::Integer;
            break;
        case dbcppp_EnvironmentVariableVarType::dbcppp_EnvironmentVariableVarTypeFloat:
            vt = EnvironmentVariable::VarType::Float;
            break;
        case dbcppp_EnvironmentVariableVarType::dbcppp_EnvironmentVariableVarTypeString:
            vt = EnvironmentVariable::VarType::String;
            break;
        default:
            vt = EnvironmentVariable::VarType::Data;
            break;
        }
        switch (access_type)
        {
        case dbcppp_EnvironmentVariableAccessType::dbcppp_EnvironmentVariableAccessTypeUnrestricted:
            at = EnvironmentVariable::AccessType::Unrestricted;
            break;
        case dbcppp_EnvironmentVariableAccessType::dbcppp_EnvironmentVariableAccessTypeRead:
            at = EnvironmentVariable::AccessType::Read;
            break;
        case dbcppp_EnvironmentVariableAccessType::dbcppp_EnvironmentVariableAccessTypeWrite:
            at = EnvironmentVariable::AccessType::Write;
            break;
        default:
            at = EnvironmentVariable::AccessType::ReadWrite;
            break;
        }
        for (; *access_nodes; access_nodes++)
        {
            ans.insert(*access_nodes);
            *access_nodes = nullptr;
        }
        for (; *value_descriptions; value_descriptions++)
        {
            vds.insert(std::make_pair((*value_descriptions)->value, (*value_descriptions)->description));
            *value_descriptions = nullptr;
        }
        for (; *attribute_values; attribute_values++)
        {
            AttributeImpl* attr = reinterpret_cast<AttributeImpl*>(*attribute_values);
            avs.insert(std::make_pair(attr->getName(), std::unique_ptr<AttributeImpl>(attr)));
            *attribute_values = nullptr;
        }
        auto result = EnvironmentVariable::create(
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
        return reinterpret_cast<const dbcppp_EnvironmentVariable*>(result.release());
    }
    DBCPPP_API void dbcppp_EnvironmentVariableFree(const dbcppp_EnvironmentVariable* env_var)
    {
        std::unique_ptr<const EnvironmentVariableImpl>(reinterpret_cast<const EnvironmentVariableImpl*>(env_var));
    }
    DBCPPP_API const char* dbcppp_EnvironmentVariableGetName(const dbcppp_EnvironmentVariable* env_var)
    {
        auto env_var_i = reinterpret_cast<const EnvironmentVariableImpl*>(env_var);
        return env_var_i->getName().c_str();
    }
    DBCPPP_API dbcppp_EnvironmentVariableVarType dbcppp_EnvironmentVariableGetVarType(const dbcppp_EnvironmentVariable* env_var)
    {
        auto env_var_i = reinterpret_cast<const EnvironmentVariableImpl*>(env_var);
        switch (env_var_i->getVarType())
        {
        case EnvironmentVariable::VarType::Integer: return dbcppp_EnvironmentVariableVarType::dbcppp_EnvironmentVariableVarTypeInteger;
        case EnvironmentVariable::VarType::Float: return dbcppp_EnvironmentVariableVarType::dbcppp_EnvironmentVariableVarTypeFloat;
        case EnvironmentVariable::VarType::String: return dbcppp_EnvironmentVariableVarType::dbcppp_EnvironmentVariableVarTypeString;
        default: return dbcppp_EnvironmentVariableVarType::dbcppp_EnvironmentVariableVarTypeData;
        }
    }
    DBCPPP_API double dbcppp_EnvironmentVariableGetMinimum(const dbcppp_EnvironmentVariable* env_var)
    {
        auto env_var_i = reinterpret_cast<const EnvironmentVariableImpl*>(env_var);
        return env_var_i->getMinimum();
    }
    DBCPPP_API double dbcppp_EnvironmentVariableGetMaximum(const dbcppp_EnvironmentVariable* env_var)
    {
        auto env_var_i = reinterpret_cast<const EnvironmentVariableImpl*>(env_var);
        return env_var_i->getMaximum();
    }
    DBCPPP_API const char* dbcppp_EnvironmentVariableGetUnit(const dbcppp_EnvironmentVariable* env_var)
    {
        auto env_var_i = reinterpret_cast<const EnvironmentVariableImpl*>(env_var);
        return env_var_i->getUnit().c_str();
    }
    DBCPPP_API double dbcppp_EnvironmentVariableGetInitialValue(const dbcppp_EnvironmentVariable* env_var)
    {
        auto env_var_i = reinterpret_cast<const EnvironmentVariableImpl*>(env_var);
        return env_var_i->getInitialValue();
    }
    DBCPPP_API uint64_t dbcppp_EnvironmentVariableGetEvId(const dbcppp_EnvironmentVariable* env_var)
    {
        auto env_var_i = reinterpret_cast<const EnvironmentVariableImpl*>(env_var);
        return env_var_i->getEvId();
    }
    DBCPPP_API dbcppp_EnvironmentVariableAccessType dbcppp_EnvironmentVariableGetAccessType(const dbcppp_EnvironmentVariable* env_var)
    {
        auto env_var_i = reinterpret_cast<const EnvironmentVariableImpl*>(env_var);
        switch (env_var_i->getAccessType())
        {
        case EnvironmentVariable::AccessType::Unrestricted: return dbcppp_EnvironmentVariableAccessType::dbcppp_EnvironmentVariableAccessTypeUnrestricted;
        case EnvironmentVariable::AccessType::Read: return dbcppp_EnvironmentVariableAccessType::dbcppp_EnvironmentVariableAccessTypeRead;
        case EnvironmentVariable::AccessType::Write: return dbcppp_EnvironmentVariableAccessType::dbcppp_EnvironmentVariableAccessTypeWrite;
        default: return dbcppp_EnvironmentVariableAccessType::dbcppp_EnvironmentVariableAccessTypeReadWrite;
        }
    }
    DBCPPP_API uint64_t dbcppp_EnvironmentVariableHasNode(const dbcppp_EnvironmentVariable* env_var, const char* node_name)
    {
        auto env_var_i = reinterpret_cast<const EnvironmentVariableImpl*>(env_var);
        return env_var_i->hasAccessNode(node_name);
    }
    DBCPPP_API void dbcppp_EnvironmentVariableForEachAccessNode(const dbcppp_EnvironmentVariable* env_var, void(*cb)(const char*, void*), void* data)
    {
        auto env_var_i = reinterpret_cast<const EnvironmentVariableImpl*>(env_var);
        env_var_i->forEachAccessNode(
            [&](const std::string& name)
            {
                cb(name.c_str(), data);
            });
    }
    DBCPPP_API void dbcppp_EnvironmentVariableForEachValueDescription(const dbcppp_EnvironmentVariable* env_var, void(*cb)(double, const char*, void*), void* data)
    {
        auto env_var_i = reinterpret_cast<const EnvironmentVariableImpl*>(env_var);
        env_var_i->forEachValueDescription(
            [&](double value, const std::string& name)
            {
                cb(value, name.c_str(), data);
            });
    }
    DBCPPP_API uint64_t dbcppp_EnvironmentVariableGetDataSize(const dbcppp_EnvironmentVariable* env_var)
    {
        auto env_var_i = reinterpret_cast<const EnvironmentVariableImpl*>(env_var);
        return env_var_i->getDataSize();
    }
    DBCPPP_API dbcppp_Attribute* dbcppp_EnvironmentVariableGetAttributeValueByName(const dbcppp_EnvironmentVariable* env_var, const char* attribute_name)
    {
        auto env_var_i = reinterpret_cast<const EnvironmentVariableImpl*>(env_var);
        return const_cast<dbcppp_Attribute*>(reinterpret_cast<const dbcppp_Attribute*>(env_var_i->getAttributeValueByName(attribute_name)));
    }
    DBCPPP_API dbcppp_Attribute* dbcppp_EnvironmentVariableFindAttributeValue(const dbcppp_EnvironmentVariable* env_var, bool(*pred)(dbcppp_Attribute*, void*), void* data)
    {
        auto env_var_i = reinterpret_cast<const EnvironmentVariableImpl*>(env_var);
        auto result = env_var_i->findAttributeValue(
            [&](const Attribute& attr)
            {
                return pred(reinterpret_cast<dbcppp_Attribute*>(&const_cast<Attribute&>(attr)), data);
            });
        return reinterpret_cast<dbcppp_Attribute*>(const_cast<Attribute*>(result));
    }
    DBCPPP_API void dbcppp_EnvironmentVariableForEachAttributeValue(const dbcppp_EnvironmentVariable* env_var, void(*cb)(dbcppp_Attribute*, void*), void* data)
    {
        auto env_var_i = reinterpret_cast<const EnvironmentVariableImpl*>(env_var);
        env_var_i->forEachAttributeValue(
            [&](const Attribute& attr)
            {
                cb(reinterpret_cast<dbcppp_Attribute*>(&const_cast<Attribute&>(attr)), data);
            });
    }
    DBCPPP_API const char* dbcppp_EnvironmentVariableGetComment(const dbcppp_EnvironmentVariable* env_var)
    {
        auto env_var_i = reinterpret_cast<const EnvironmentVariableImpl*>(env_var);
        return env_var_i->getComment().c_str();
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
        std::set<std::string> msg_trans;
        std::map<std::string, std::unique_ptr<Signal>> sigs;
        std::map<std::string, std::unique_ptr<Attribute>> attrs;
        for (; *message_transmitters; message_transmitters++)
        {
            msg_trans.insert(*message_transmitters);
            *message_transmitters = nullptr;
        }
        for (; *signals; signals++)
        {
            SignalImpl* si = reinterpret_cast<SignalImpl*>(*signals);
            sigs.insert(std::make_pair(si->getName(), std::unique_ptr<SignalImpl>(si)));
            *signals = nullptr;
        }
        for (; *attribute_values; attribute_values++)
        {
            AttributeImpl* attr = reinterpret_cast<AttributeImpl*>(*attribute_values);
            attrs.insert(std::make_pair(attr->getName(), std::unique_ptr<AttributeImpl>(attr)));
            *attribute_values = nullptr;
        }
        auto result = Message::create(
              id
            , std::string(name)
            , message_size
            , std::string(transmitter)
            , std::move(msg_trans)
            , std::move(sigs)
            , std::move(attrs)
            , std::string(comment));
        return reinterpret_cast<const dbcppp_Message*>(result.release());
    }
    DBCPPP_API void dbcppp_MessageFree(const dbcppp_Message* msg)
    {
        std::unique_ptr<const MessageImpl>(reinterpret_cast<const MessageImpl*>(msg));
    }
    DBCPPP_API uint64_t dbcppp_MessageGetId(const dbcppp_Message* msg)
    {
        auto msgi = reinterpret_cast<const MessageImpl*>(msg);
        return msgi->getId();
    }
    DBCPPP_API const char* dbcppp_MessageGetName(const dbcppp_Message* msg)
    {
        auto msgi = reinterpret_cast<const MessageImpl*>(msg);
        return msgi->getName().c_str();
    }
    DBCPPP_API uint64_t dbcppp_MessageGetMessageSize(const dbcppp_Message* msg)
    {
        auto msgi = reinterpret_cast<const MessageImpl*>(msg);
        return msgi->getId();
    }
    DBCPPP_API const char* dbcppp_MessageGetTransmitter(const dbcppp_Message* msg)
    {
        auto msgi = reinterpret_cast<const MessageImpl*>(msg);
        return msgi->getTransmitter().c_str();
    }
    DBCPPP_API bool dbcppp_MessageHasMessageTransmitter(const dbcppp_Message* msg, const char* name)
    {
        auto msgi = reinterpret_cast<const MessageImpl*>(msg);
        return msgi->hasMessageTransmitter(name);
    }
    DBCPPP_API void dbcppp_MessageForEachMessageTransmitter(const dbcppp_Message* msg, void(*cb)(const char*, void*), void* data)
    {
        auto msgi = reinterpret_cast<const MessageImpl*>(msg);
        msgi->forEachMessageTransmitter(
            [&](const std::string& trans)
            {
                cb(trans.c_str(), data);
            });
    }
    DBCPPP_API const dbcppp_Signal* dbcppp_MessageGetSignalByName(const dbcppp_Message* msg, const char* name)
    {
        auto msgi = reinterpret_cast<const MessageImpl*>(msg);
        auto result = msgi->getSignalByName(name);
        return reinterpret_cast<const dbcppp_Signal*>(result);
    }
    DBCPPP_API const dbcppp_Signal* dbcppp_MessageFindSignal(const dbcppp_Message* msg, bool(*pred)(const dbcppp_Signal*, void*), void* data)
    {
        auto msgi = reinterpret_cast<const MessageImpl*>(msg);
        auto result = msgi->findSignal(
            [&](const Signal& sig)
            {
                return pred(reinterpret_cast<const dbcppp_Signal*>(&sig), data);
            });
        return reinterpret_cast<const dbcppp_Signal*>(result);
    }
    DBCPPP_API void dbcppp_MessageForEachSignal(const dbcppp_Message* msg, void(*cb)(const dbcppp_Signal*, void*), void* data)
    {
        auto msgi = reinterpret_cast<const MessageImpl*>(msg);
        msgi->forEachSignal(
            [&](const Signal& sig)
            {
                cb(reinterpret_cast<const dbcppp_Signal*>(&sig), data);
            });
    }
    DBCPPP_API const dbcppp_Attribute* dbcppp_MessageGetAttributeValueByName(const dbcppp_Message* msg, const char* name)
    {
        auto msgi = reinterpret_cast<const MessageImpl*>(msg);
        auto result = msgi->getAttributeValueByName(name);
        return reinterpret_cast<const dbcppp_Attribute*>(result);
    }
    DBCPPP_API const dbcppp_Attribute* dbcppp_MessageFindAttributeValue(dbcppp_Message* msg, bool(*pred)(const dbcppp_Attribute*, void*), void* data)
    {
        auto msgi = reinterpret_cast<const MessageImpl*>(msg);
        auto result = msgi->findAttributeValue(
            [&](const Attribute& sig)
            {
                return pred(reinterpret_cast<const dbcppp_Attribute*>(&sig), data);
            });
       return reinterpret_cast<const dbcppp_Attribute*>(result);
    }
    DBCPPP_API void dbcppp_MessageForEachAttributeValue(dbcppp_Message* msg, void(*cb)(const dbcppp_Attribute*, void*), void* data)
    {
        auto msgi = reinterpret_cast<const MessageImpl*>(msg);
        msgi->forEachAttributeValue(
            [&](const Attribute& sig)
            {
                cb(reinterpret_cast<const dbcppp_Attribute*>(&sig), data);
            });
    }
    DBCPPP_API const char* dbcppp_MessageGetComment(const dbcppp_Message* msg)
    {
        auto msgi = reinterpret_cast<const MessageImpl*>(msg);
        return msgi->getComment().c_str();
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
        std::set<std::string> ns;
        for (; *new_symbols; new_symbols++)
        {
            ns.insert(*new_symbols);
            *new_symbols = nullptr;
        }
        std::unique_ptr<BitTiming> bt(reinterpret_cast<BitTimingImpl*>(bit_timing));
        std::map<std::string, std::unique_ptr<Node>> n;
        for (; *nodes; nodes++)
        {
            NodeImpl* ni = reinterpret_cast<NodeImpl*>(*nodes);
            n.insert(std::make_pair(ni->getName(), std::unique_ptr<Node>(ni)));
            *nodes = nullptr;
        }
        std::map<std::string, std::unique_ptr<ValueTable>> vt;
        for (; *value_tables; value_tables++)
        {
            ValueTableImpl* vti = reinterpret_cast<ValueTableImpl*>(*value_tables);
            vt.insert(std::make_pair(vti->getName(), std::unique_ptr<ValueTable>(vti)));
            *value_tables = nullptr;
        }
        std::unordered_map<uint64_t, std::unique_ptr<Message>> m;
        for (; *messages; messages++)
        {
            MessageImpl* mi = reinterpret_cast<MessageImpl*>(*messages);
            m.insert(std::make_pair(mi->getId(), std::unique_ptr<Message>(mi)));
            *messages = nullptr;
        }
        std::map<std::string, std::unique_ptr<EnvironmentVariable>> ev;
        for (; *environment_variables; environment_variables++)
        {
            EnvironmentVariableImpl* evi = reinterpret_cast<EnvironmentVariableImpl*>(*environment_variables);
            ev.insert(std::make_pair(evi->getName(), std::unique_ptr<EnvironmentVariable>(evi)));
            *environment_variables = nullptr;
        }
        std::map<std::string, std::unique_ptr<AttributeDefinition>> adef;
        for (; *attribute_definitions; attribute_definitions++)
        {
            AttributeDefinitionImpl* adefi = reinterpret_cast<AttributeDefinitionImpl*>(*attribute_definitions);
            adef.insert(std::make_pair(adefi->getName(), std::unique_ptr<AttributeDefinition>(adefi)));
            *attribute_definitions = nullptr;
        }
        std::map<std::string, std::unique_ptr<Attribute>> ad;
        for (; *attribute_defaults; attribute_defaults++)
        {
            AttributeImpl* adi = reinterpret_cast<AttributeImpl*>(*attribute_defaults);
            ad.insert(std::make_pair(adi->getName(), std::unique_ptr<Attribute>(adi)));
            *attribute_defaults = nullptr;
        }
        std::map<std::string, std::unique_ptr<Attribute>> av;
        for (; *attribute_values; attribute_values++)
        {
            AttributeImpl* avi = reinterpret_cast<AttributeImpl*>(*attribute_values);
            ad.insert(std::make_pair(avi->getName(), std::unique_ptr<Attribute>(avi)));
            *attribute_values = nullptr;
        }
        std::string c(comment);
        auto net = Network::create(
              std::move(v), std::move(ns)
            , std::move(bt), std::move(n)
            , std::move(vt), std::move(m)
            , std::move(ev), std::move(adef)
            , std::move(ad), std::move(av)
            , std::move(c));
        Network* result = net.release();
        return reinterpret_cast<const dbcppp_Network*>(result);
    }
    DBCPPP_API void dbcppp_NetworkFree(const dbcppp_Network* network)
    {
        std::unique_ptr<const NetworkImpl>(reinterpret_cast<const NetworkImpl*>(network));
    }
    DBCPPP_API const char* dbcppp_NetworkGetVersion(const dbcppp_Network* net)
    {
        auto neti = reinterpret_cast<const NetworkImpl*>(net);
        return neti->getVersion().c_str();
    }
    DBCPPP_API bool dbcppp_NetworkHasNewSymbol(const dbcppp_Network* net, const char* new_symbol)
    {
        auto neti = reinterpret_cast<const NetworkImpl*>(net);
        return neti->hasNewSymbol(new_symbol);
    }
    DBCPPP_API void dbcppp_NetworkForEachNewSymbol(const dbcppp_Network* net, void(*cb)(const char*, void*), void* data)
    {
        auto neti = reinterpret_cast<const NetworkImpl*>(net);
        neti->forEachNewSymbol(
            [&](const std::string& n)
            {
                cb(n.c_str(), data);
            });
    }
    DBCPPP_API const dbcppp_BitTiming* dbcppp_NetworkGetBitTiming(const dbcppp_Network* net)
    {
        auto neti = reinterpret_cast<const NetworkImpl*>(net);
        const BitTiming& bt = neti->getBitTiming();
        return reinterpret_cast<const dbcppp_BitTiming*>(&bt);
    }
    DBCPPP_API const dbcppp_Node* dbcppp_NetworkGetNodeByName(const dbcppp_Network* net, const char* name)
    {
        auto neti = reinterpret_cast<const NetworkImpl*>(net);
        const Node* node = neti->getNodeByName(name);
        return reinterpret_cast<const dbcppp_Node*>(node);
    }
    DBCPPP_API void dbcppp_NetworkForEachNode(const dbcppp_Network* net, void(*cb)(const dbcppp_Node*, void*), void* data)
    {
        auto neti = reinterpret_cast<const NetworkImpl*>(net);
        neti->forEachNode(
            [&](const Node& node)
            {
                cb(reinterpret_cast<const dbcppp_Node*>(&node), data);
            });
    }
    DBCPPP_API const dbcppp_ValueTable* dbcppp_NetworkGetValueTableByName(const dbcppp_Network* net, const char* name)
    {
        auto neti = reinterpret_cast<const NetworkImpl*>(net);
        const ValueTable* vt = neti->getValueTableByName(name);
        return reinterpret_cast<const dbcppp_ValueTable*>(vt);
    }
    DBCPPP_API const dbcppp_ValueTable* dbcppp_NetworkFindValueTable(const dbcppp_Network* net, bool(*pred)(const dbcppp_ValueTable*, void*), void* data)
    {
        auto neti = reinterpret_cast<const NetworkImpl*>(net);
        const ValueTable* vt = neti->findValueTable(
            [&](const ValueTable& vt)
            {
                return pred(reinterpret_cast<const dbcppp_ValueTable*>(&vt), data);
            });
        return reinterpret_cast<dbcppp_ValueTable*>(const_cast<ValueTable*>(vt));
    }
    DBCPPP_API void dbcppp_NetworkForEachValueTable(const dbcppp_Network* net, void(*cb)(const dbcppp_ValueTable*, void*), void* data)
    {
        auto neti = reinterpret_cast<const NetworkImpl*>(net);
        neti->forEachValueTable(
            [&](const ValueTable& vt)
            {
                cb(reinterpret_cast<const dbcppp_ValueTable*>(&vt), data);
            });
    }
    DBCPPP_API const dbcppp_Message* dbcppp_NetworkGetMessageById(const dbcppp_Network* net, uint64_t id)
    {
        auto neti = reinterpret_cast<const NetworkImpl*>(net);
        const Message* m = neti->getMessageById(id);
        return reinterpret_cast<const dbcppp_Message*>(m);
    }
    DBCPPP_API const dbcppp_Message* dbcppp_NetworkFindMessage(const dbcppp_Network* net, bool(*pred)(const dbcppp_Message*, void*), void* data)
    {
        auto neti = reinterpret_cast<const NetworkImpl*>(net);
        const Message* m = neti->findMessage(
            [&](const Message& m)
            {
                return pred(reinterpret_cast<const dbcppp_Message*>(&m), data);
            });
        return reinterpret_cast<const dbcppp_Message*>(m);
    }
    DBCPPP_API void dbcppp_NetworkForEachMessage(const dbcppp_Network* net, void(*cb)(const dbcppp_Message*))
    {
        auto neti = reinterpret_cast<const NetworkImpl*>(net);
        neti->forEachMessage(
            [&](const Message& m)
            {
                cb(reinterpret_cast<const dbcppp_Message*>(&m));
            });
    }
    DBCPPP_API const dbcppp_EnvironmentVariable* dbcppp_NetworkGetEnvironmentVariableByName(const dbcppp_Network* net, const char* name)
    {
        auto neti = reinterpret_cast<const NetworkImpl*>(net);
        const EnvironmentVariable* ev = neti->getEnvironmentVariableByName(name);
        return reinterpret_cast<const dbcppp_EnvironmentVariable*>(ev);
    }
    DBCPPP_API dbcppp_EnvironmentVariable* dbcppp_NetworkFindEnvironmentVariable(const dbcppp_Network* net, bool(*pred)(const dbcppp_EnvironmentVariable*, void*), void* data)
    {
        auto neti = reinterpret_cast<const NetworkImpl*>(net);
        const EnvironmentVariable* ev = neti->findEnvironmentVariable(
            [&](const EnvironmentVariable& ev)
            {
                return pred(reinterpret_cast<const dbcppp_EnvironmentVariable*>(&ev), data);
            });
        return reinterpret_cast<dbcppp_EnvironmentVariable*>(const_cast<EnvironmentVariable*>(ev));
    }
    DBCPPP_API void dbcppp_NetworkForEachEnvironmentVariable(const dbcppp_Network* net, void(*cb)(const dbcppp_EnvironmentVariable*, void*), void* data)
    {
        auto neti = reinterpret_cast<const NetworkImpl*>(net);
        neti->forEachEnvironmentVariable(
            [&](const EnvironmentVariable& ev)
            {
                cb(reinterpret_cast<const dbcppp_EnvironmentVariable*>(&ev), data);
            });
    }
    DBCPPP_API const dbcppp_AttributeDefinition* dbcppp_NetworkGetAttributeDefinitionByName(const dbcppp_Network* net, const char* name)
    {
        auto neti = reinterpret_cast<const NetworkImpl*>(net);
        const AttributeDefinition* ad = neti->getAttributeDefinitionByName(name);
        return reinterpret_cast<const dbcppp_AttributeDefinition*>(ad);
    }
    DBCPPP_API dbcppp_AttributeDefinition* dbcppp_NetworkFindAttributeDefinition(const dbcppp_Network* net, bool(*pred)(const dbcppp_AttributeDefinition*, void*), void* data)
    {
        auto neti = reinterpret_cast<const NetworkImpl*>(net);
        const AttributeDefinition* ad = neti->findAttributeDefinition(
            [&](const AttributeDefinition& ad)
            {
                return pred(reinterpret_cast<const dbcppp_AttributeDefinition*>(&ad), data);
            });
        return reinterpret_cast<dbcppp_AttributeDefinition*>(const_cast<AttributeDefinition*>(ad));
    }
    DBCPPP_API void dbcppp_NetworkForEachAttributeDefinition(const dbcppp_Network* net, void(*cb)(const dbcppp_AttributeDefinition*, void*), void* data)
    {
        auto neti = reinterpret_cast<const NetworkImpl*>(net);
        neti->forEachAttributeDefinition(
            [&](const AttributeDefinition& ad)
            {
                cb(reinterpret_cast<const dbcppp_AttributeDefinition*>(&ad), data);
            });
    }
    DBCPPP_API const dbcppp_Attribute* dbcppp_NetworkGetAttributeDefaultByName(const dbcppp_Network* net, const char* name)
    {
        auto neti = reinterpret_cast<const NetworkImpl*>(net);
        const Attribute* ad = neti->getAttributeDefaultByName(name);
        return reinterpret_cast<const dbcppp_Attribute*>(ad);
    }
    DBCPPP_API const dbcppp_Attribute* dbcppp_NetworkFindAttributeDefault(const dbcppp_Network* net, bool(*pred)(const dbcppp_Attribute*, void*), void* data)
    {
        auto neti = reinterpret_cast<const NetworkImpl*>(net);
        const Attribute* ad = neti->findAttributeDefault(
            [&](const Attribute& ad)
            {
                return pred(reinterpret_cast<const dbcppp_Attribute*>(&ad), data);
            });
        return reinterpret_cast<const dbcppp_Attribute*>(ad);
    }
    DBCPPP_API void dbcppp_NetworkForEachAttributeDefault(const dbcppp_Network* net, void(*cb)(const dbcppp_Attribute*, void*), void* data)
    {
        auto neti = reinterpret_cast<const NetworkImpl*>(net);
        neti->forEachAttributeDefault(
            [&](const Attribute& ad)
            {
                cb(reinterpret_cast<const dbcppp_Attribute*>(&ad), data);
            });
    }
    DBCPPP_API const dbcppp_Attribute* dbcppp_NetworkGetAttributeValueByName(const dbcppp_Network* net, const char* name)
    {
        auto neti = reinterpret_cast<const NetworkImpl*>(net);
        const Attribute* ad = neti->getAttributeValueByName(name);
        return reinterpret_cast<const dbcppp_Attribute*>(ad);
    }
    DBCPPP_API const dbcppp_Attribute* dbcppp_NetworkFindAttributeValue(const dbcppp_Network* net, bool(*pred)(const dbcppp_Attribute*, void*), void* data)
    {
        auto neti = reinterpret_cast<const NetworkImpl*>(net);
        const Attribute* ad = neti->findAttributeValue(
            [&](const Attribute& ad)
            {
                return pred(reinterpret_cast<const dbcppp_Attribute*>(&ad), data);
            });
        return reinterpret_cast<const dbcppp_Attribute*>(ad);
    }
    DBCPPP_API void dbcppp_NetworkForEachAttributeValue(const dbcppp_Network* net, void(*cb)(const dbcppp_Attribute*, void*), void* data)
    {
        auto neti = reinterpret_cast<const NetworkImpl*>(net);
        neti->forEachAttributeValue(
            [&](const Attribute& ad)
            {
                cb(reinterpret_cast<const dbcppp_Attribute*>(&ad), data);
            });
    }
    DBCPPP_API const char* dbcppp_NetworkGetComment(const dbcppp_Network* net)
    {
        auto neti = reinterpret_cast<const NetworkImpl*>(net);
        return neti->getComment().c_str();
    }

    DBCPPP_API const dbcppp_Node* dbcppp_NodeCreate(const char* name, const char* comment, dbcppp_Attribute** attributes)
    {
        std::map<std::string, std::unique_ptr<Attribute>> attrs;
        for (; *attributes; attributes++)
        {
            AttributeImpl* ai = reinterpret_cast<AttributeImpl*>(*attributes);
            attrs.insert(std::make_pair(ai->getName(), std::unique_ptr<Attribute>(ai)));
            *attributes = nullptr;
        }
        auto result = Node::create(std::string(name), std::string(comment), std::move(attrs));
        return reinterpret_cast<const dbcppp_Node*>(result.release());
    }
    DBCPPP_API void dbcppp_NodeFree(const dbcppp_Node* node)
    {
        std::unique_ptr<const NodeImpl>(reinterpret_cast<const NodeImpl*>(node));
    }
    DBCPPP_API const char* dbcppp_NodeGetName(const dbcppp_Node* node)
    {
        auto ni = reinterpret_cast<const NodeImpl*>(node);
        return ni->getName().c_str();
    }
    DBCPPP_API const dbcppp_Attribute* dbcppp_NodeGetAttributeValueByName(const dbcppp_Node* node, const char* attribute_name)
    {
        auto ni = reinterpret_cast<const NodeImpl*>(node);
        return reinterpret_cast<const dbcppp_Attribute*>(ni->getAttributeValueByName(attribute_name));
    }
    DBCPPP_API const dbcppp_Attribute* dbcppp_NodeFindAttributeValue(const dbcppp_Node* node, bool(*pred)(const dbcppp_Attribute*, void*), void* data)
    {
        auto ni = reinterpret_cast<const NodeImpl*>(node);
        const Attribute* a = ni->findAttributeValue(
            [&](const Attribute& a)
            {
                return pred(reinterpret_cast<const dbcppp_Attribute*>(&a), data);
            });
        return reinterpret_cast<const dbcppp_Attribute*>(a);
    }
    DBCPPP_API void dbcppp_NodeForEachAttributeValue(const dbcppp_Node* node, void(*cb)(const dbcppp_Attribute*, void*), void* data)
    {
        auto ni = reinterpret_cast<const NodeImpl*>(node);
        ni->forEachAttributeValue(
            [&](const Attribute& a)
            {
                cb(reinterpret_cast<const dbcppp_Attribute*>(&a), data);
            });
    }
    DBCPPP_API const char* dbcppp_NodeGetComment(const dbcppp_Node* node)
    {
        auto ni = reinterpret_cast<const NodeImpl*>(node);
        return ni->getComment().c_str();
    }

    DBCPPP_API const dbcppp_Signal* dbcppp_SignalCreate(
          uint64_t message_size
        , const char* name
        , dbcppp_SignalMultiplexer multiplexer_indicator
        , uint64_t multiplexer_switch_value
        , uint64_t start_bit
        , uint64_t bit_size
        , dbcppp_SignalByteOrder byte_order
        , dbcppp_SignalValueType value_type
        , double factor
        , double offset
        , double minimum
        , double maximum
        , const char* unit
        , const char** receivers
        , dbcppp_Attribute** attribute_values
        , dbcppp_ValueDescriptionPair** value_descriptions
        , const char* comment
        , dbcppp_SignalExtendedValueType extended_value_type)
    {
        Signal::Multiplexer m;
        Signal::ByteOrder bo;
        Signal::ValueType vt;
        std::set<std::string> rs;
        std::map<std::string, std::unique_ptr<Attribute>> avs;
        std::map<double, std::string> vds;
        Signal::ExtendedValueType evt;
        switch (multiplexer_indicator)
        {
        case dbcppp_SignalMultiplexer::dbcppp_SignalMultiplexerNoMux: m = Signal::Multiplexer::NoMux; break;
        case dbcppp_SignalMultiplexer::dbcppp_SignalMultiplexerMuxSwitch: m = Signal::Multiplexer::MuxSwitch; break;
        default: m = Signal::Multiplexer::MuxValue; break;
        }
        switch (byte_order)
        {
        case dbcppp_SignalByteOrder::dbcppp_SignalByteOrderLittleEndian: bo = Signal::ByteOrder::LittleEndian; break;
        default: bo = Signal::ByteOrder::BigEndian; break;
        }
        switch (value_type)
        {
        case dbcppp_SignalValueType::dbcppp_SignalValueTypeSigned: vt = Signal::ValueType::Signed; break;
        default: vt = Signal::ValueType::Unsigned; break;
        }
        for (; *receivers; receivers++)
        {
            rs.insert(*receivers);
            *receivers = nullptr;
        }
        for (; *attribute_values; attribute_values++)
        {
            auto av = reinterpret_cast<AttributeImpl*>(*attribute_values);
            avs.insert(std::make_pair(av->getName(), std::unique_ptr<AttributeImpl>(av)));
            *attribute_values = nullptr;
        }
        for (; *value_descriptions; value_descriptions++)
        {
            vds.insert(std::make_pair((*value_descriptions)->value, (*value_descriptions)->description));
            *value_descriptions = nullptr;
        }
        switch (extended_value_type)
        {
        case dbcppp_SignalExtendedValueType::dbcppp_SignalExtendedValueTypeInteger: evt = Signal::ExtendedValueType::Integer; break;
        case dbcppp_SignalExtendedValueType::dbcppp_SignalExtendedValueTypeFloat: evt = Signal::ExtendedValueType::Float; break;
        default: evt = Signal::ExtendedValueType::Double; break;
        }
        auto result = Signal::create(
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
            , std::move(vds)
            , std::string(comment)
            , evt);
        return reinterpret_cast<const dbcppp_Signal*>(result.release());
    }
    DBCPPP_API const char* dbcppp_SignalGetName(const dbcppp_Signal* sig)
    {
        auto sigi = reinterpret_cast<const SignalImpl*>(sig);
        return sigi->getName().c_str();
    }
    DBCPPP_API dbcppp_SignalMultiplexer dbcppp_SignalGetMultiplexerIndicator(const dbcppp_Signal* sig)
    {
        auto sigi = reinterpret_cast<const SignalImpl*>(sig);
        switch (sigi->getMultiplexerIndicator())
        {
        case Signal::Multiplexer::NoMux: return dbcppp_SignalMultiplexer::dbcppp_SignalMultiplexerNoMux;
        case Signal::Multiplexer::MuxSwitch: return dbcppp_SignalMultiplexer::dbcppp_SignalMultiplexerMuxSwitch;
        default: return dbcppp_SignalMultiplexer::dbcppp_SignalMultiplexerMuxValue;
        }
    }
    DBCPPP_API uint64_t dbcppp_SignalGetMultiplexerSwitchValue(const dbcppp_Signal* sig)
    {
        auto sigi = reinterpret_cast<const SignalImpl*>(sig);
        return sigi->getMultiplexerSwitchValue();
    }
    DBCPPP_API uint64_t dbcppp_SignalGetStartBit(const dbcppp_Signal* sig)
    {
        auto sigi = reinterpret_cast<const SignalImpl*>(sig);
        return sigi->getStartBit();
    }
    DBCPPP_API uint64_t dbcppp_SignalGetBitSize(const dbcppp_Signal* sig)
    {
        auto sigi = reinterpret_cast<const SignalImpl*>(sig);
        return sigi->getBitSize();
    }
    DBCPPP_API dbcppp_SignalByteOrder dbcppp_SignalGetByteOrder(const dbcppp_Signal* sig)
    {
        auto sigi = reinterpret_cast<const SignalImpl*>(sig);
        switch (sigi->getByteOrder())
        {
        case Signal::ByteOrder::LittleEndian: return dbcppp_SignalByteOrder::dbcppp_SignalByteOrderLittleEndian;
        default: return dbcppp_SignalByteOrder::dbcppp_SignalByteOrderBigEndian;
        }
    }
    DBCPPP_API dbcppp_SignalValueType dbcppp_SignalGetValueType(const dbcppp_Signal* sig)
    {
        auto sigi = reinterpret_cast<const SignalImpl*>(sig);
        switch (sigi->getValueType())
        {
        case Signal::ValueType::Signed: return dbcppp_SignalValueType::dbcppp_SignalValueTypeSigned;
        default: return dbcppp_SignalValueType::dbcppp_SignalValueTypeUnsigned;
        }
    }
    DBCPPP_API double dbcppp_SignalGetFactor(const dbcppp_Signal* sig)
    {
        auto sigi = reinterpret_cast<const SignalImpl*>(sig);
        return sigi->getFactor();
    }
    DBCPPP_API double dbcppp_SignalGetOffset(const dbcppp_Signal* sig)
    {
        auto sigi = reinterpret_cast<const SignalImpl*>(sig);
        return sigi->getOffset();
    }
    DBCPPP_API double dbcppp_SignalGetMinimum(const dbcppp_Signal* sig)
    {
        auto sigi = reinterpret_cast<const SignalImpl*>(sig);
        return sigi->getMinimum();
    }
    DBCPPP_API double dbcppp_SignalGetMaximum(const dbcppp_Signal* sig)
    {
        auto sigi = reinterpret_cast<const SignalImpl*>(sig);
        return sigi->getMaximum();
    }
    DBCPPP_API const char* dbcppp_SignalGetUnit(const dbcppp_Signal* sig)
    {
        auto sigi = reinterpret_cast<const SignalImpl*>(sig);
        return sigi->getUnit().c_str();
    }
    DBCPPP_API bool dbcppp_SignalHasReceiver(const dbcppp_Signal* sig, const char* name)
    {
        auto sigi = reinterpret_cast<const SignalImpl*>(sig);
        return sigi->hasReceiver(name);
    }
    DBCPPP_API void dbcppp_SignalForEachReceiver(const dbcppp_Signal* sig, void(*cb)(const char*, void*), void* data)
    {
        auto sigi = reinterpret_cast<const SignalImpl*>(sig);
        sigi->forEachReceiver(
            [&](const std::string& recv)
            {
                cb(recv.c_str(), data);
            });
    }
    DBCPPP_API void dbcppp_SignalForEachValueDescription(const dbcppp_Signal* sig, void(*cb)(double, const char*, void*), void* data)
    {
        auto sigi = reinterpret_cast<const SignalImpl*>(sig);
        sigi->forEachValueDescription(
            [&](double value, const std::string& desc)
            {
                cb(value, desc.c_str(), data);
            });
    }
    DBCPPP_API const dbcppp_Attribute* dbcppp_SignalGetAttributeValueByName(const dbcppp_Signal* sig, const char* name)
    {
        auto sigi = reinterpret_cast<const SignalImpl*>(sig);
        auto result = sigi->getAttributeValueByName(name);
        return reinterpret_cast<const dbcppp_Attribute*>(result);
    }
    DBCPPP_API const dbcppp_Attribute* dbcppp_SignalFindAttributeValue(const dbcppp_Signal* sig, bool(*pred)(const dbcppp_Attribute*, void*), void* data)
    {
        auto sigi = reinterpret_cast<const SignalImpl*>(sig);
        auto result = sigi->findAttributeValue(
            [&](const Attribute& attr)
            {
                return pred(reinterpret_cast<const dbcppp_Attribute*>(&attr), data);
            });
        return reinterpret_cast<const dbcppp_Attribute*>(result);
    }
    DBCPPP_API void dbcppp_SignalForEachAttributeValue(const dbcppp_Signal* sig, void(*cb)(const dbcppp_Attribute*, void*), void* data)
    {
        auto sigi = reinterpret_cast<const SignalImpl*>(sig);
        sigi->forEachAttributeValue(
            [&](const Attribute& attr)
            {
                cb(reinterpret_cast<const dbcppp_Attribute*>(&attr), data);
            });
    }
    DBCPPP_API const char* dbcppp_SignalGetComment(const dbcppp_Signal* sig)
    {
        auto sigi = reinterpret_cast<const SignalImpl*>(sig);
        return sigi->getComment().c_str();
    }
    DBCPPP_API dbcppp_SignalExtendedValueType dbcppp_SignalGetGetExtendedValueType(const dbcppp_Signal* sig)
    {
        auto sigi = reinterpret_cast<const SignalImpl*>(sig);
        switch (sigi->getExtendedValueType())
        {
        case Signal::ExtendedValueType::Integer: return dbcppp_SignalExtendedValueType::dbcppp_SignalExtendedValueTypeInteger;
        case Signal::ExtendedValueType::Float: return dbcppp_SignalExtendedValueType::dbcppp_SignalExtendedValueTypeFloat;
        default: return dbcppp_SignalExtendedValueType::dbcppp_SignalExtendedValueTypeDouble;
        }
    }
    DBCPPP_API double dbcppp_SignalDecode(const dbcppp_Signal* sig, const void* bytes)
    {
        auto sigi = reinterpret_cast<const SignalImpl*>(sig);
        return sigi->decode(bytes);
    }
    DBCPPP_API void dbcppp_SignalEncode(const dbcppp_Signal* sig, double raw, void* buffer)
    {
        auto sigi = reinterpret_cast<const SignalImpl*>(sig);
        sigi->encode(raw, buffer);
    }
    DBCPPP_API double dbcppp_SignalRawToPhys(const dbcppp_Signal* sig, double raw)
    {
        auto sigi = reinterpret_cast<const SignalImpl*>(sig);
        return sigi->rawToPhys(raw);
    }
    DBCPPP_API double dbcppp_SignalPhysToRaw(const dbcppp_Signal* sig, double phys)
    {
        auto sigi = reinterpret_cast<const SignalImpl*>(sig);
        return sigi->physToRaw(phys);
    }

    DBCPPP_API const dbcppp_SignalType* dbcppp_SignalTypeCreate(
          const char* name
        , uint64_t signal_size
        , dbcppp_SignalByteOrder byte_order
        , dbcppp_SignalValueType value_type
        , double factor
        , double offset
        , double minimum
        , double maximum
        , const char* unit
        , double default_value
        , const char* value_table)
    {
        Signal::ByteOrder bo;
        Signal::ValueType vt;
        switch (byte_order)
        {
        case dbcppp_SignalByteOrder::dbcppp_SignalByteOrderLittleEndian: bo = Signal::ByteOrder::LittleEndian; break;
        default: bo = Signal::ByteOrder::BigEndian; break;
        }
        switch (value_type)
        {
        case dbcppp_SignalValueType::dbcppp_SignalValueTypeSigned: vt = Signal::ValueType::Signed; break;
        default: vt = Signal::ValueType::Unsigned; break;
        }
        auto result = SignalType::create(
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
    DBCPPP_API const char* dbcppp_SignalTypeGetName(const dbcppp_SignalType* sig_type)
    {
        auto sti = reinterpret_cast<const SignalTypeImpl*>(sig_type);
        return sti->getName().c_str();
    }
    DBCPPP_API uint64_t dbcppp_SignalTypeGetSignalSize(const dbcppp_SignalType* sig_type)
    {
        auto sti = reinterpret_cast<const SignalTypeImpl*>(sig_type);
        return sti->getSignalSize();
    }
    DBCPPP_API dbcppp_SignalByteOrder dbcppp_SignalTypeGetByteOrder(const dbcppp_SignalType* sig_type)
    {
        auto sti = reinterpret_cast<const SignalTypeImpl*>(sig_type);
        switch (sti->getByteOrder())
        {
        case Signal::ByteOrder::LittleEndian: return dbcppp_SignalByteOrder::dbcppp_SignalByteOrderLittleEndian;
        default: return dbcppp_SignalByteOrder::dbcppp_SignalByteOrderBigEndian;
        }
    }
    DBCPPP_API dbcppp_SignalValueType dbcppp_SignalTypeGetValueType(const dbcppp_SignalType* sig_type)
    {
        auto sti = reinterpret_cast<const SignalTypeImpl*>(sig_type);
        switch (sti->getValueType())
        {
        case Signal::ValueType::Signed: return dbcppp_SignalValueType::dbcppp_SignalValueTypeSigned;
        default: return dbcppp_SignalValueType::dbcppp_SignalValueTypeUnsigned;
        }
    }
    DBCPPP_API double dbcppp_SignalTypeGetFactor(const dbcppp_SignalType* sig_type)
    {
        auto sti = reinterpret_cast<const SignalTypeImpl*>(sig_type);
        return sti->getFactor();
    }
    DBCPPP_API double dbcppp_SignalTypeGetOffset(const dbcppp_SignalType* sig_type)
    {
        auto sti = reinterpret_cast<const SignalTypeImpl*>(sig_type);
        return sti->getOffset();
    }
    DBCPPP_API double dbcppp_SignalTypeGetMinimum(const dbcppp_SignalType* sig_type)
    {
        auto sti = reinterpret_cast<const SignalTypeImpl*>(sig_type);
        return sti->getMinimum();
    }
    DBCPPP_API double dbcppp_SignalTypeGetMaximum(const dbcppp_SignalType* sig_type)
    {
        auto sti = reinterpret_cast<const SignalTypeImpl*>(sig_type);
        return sti->getMaximum();
    }
    DBCPPP_API const char* dbcppp_SignalTypeGetUnit(const dbcppp_SignalType* sig_type)
    {
        auto sti = reinterpret_cast<const SignalTypeImpl*>(sig_type);
        return sti->getUnit().c_str();
    }
    DBCPPP_API double dbcppp_SignalTypeGetDefaultValue(const dbcppp_SignalType* sig_type)
    {
        auto sti = reinterpret_cast<const SignalTypeImpl*>(sig_type);
        return sti->getDefaultValue();
    }
    DBCPPP_API const char* dbcppp_SignalTypeGetValueTable(const dbcppp_SignalType* sig_type)
    {
        auto sti = reinterpret_cast<const SignalTypeImpl*>(sig_type);
        return sti->getValueTable().c_str();
    }

    DBCPPP_API const dbcppp_ValueTable* dbcppp_ValueTableCreate(const char* name, dbcppp_SignalType* signal_type, dbcppp_ValueDescriptionPair** pairs)
    {
        std::map<double, std::string> descs;
        for (; *pairs; pairs++)
        {
            descs.insert(std::make_pair((*pairs)->value, (*pairs)->description));
        }
        boost::optional<std::unique_ptr<SignalType>> st;
        if (signal_type)
        {
            st = std::unique_ptr<SignalTypeImpl>(reinterpret_cast<SignalTypeImpl*>(signal_type));
        }
        auto result = ValueTable::create(std::string(name), std::move(st), std::move(descs));
        return reinterpret_cast<dbcppp_ValueTable*>(result.release());
    }
    DBCPPP_API void dbcppp_ValueTableFree(const dbcppp_ValueTable* value_table)
    {
        std::unique_ptr<const ValueTableImpl>(reinterpret_cast<const ValueTableImpl*>(value_table));
    }
    DBCPPP_API const char* dbcppp_ValueTableGetName(const dbcppp_ValueTable* value_table)
    {
        auto vti = reinterpret_cast<const ValueTableImpl*>(value_table);
        return vti->getName().c_str();
    }
    DBCPPP_API const dbcppp_SignalType* dbcppp_ValueTableGetSignalType(const dbcppp_ValueTable* value_table)
    {
        auto vti = reinterpret_cast<const ValueTableImpl*>(value_table);
        const dbcppp_SignalType* result = nullptr;
        if (vti->getSignalType())
        {
            result = reinterpret_cast<const dbcppp_SignalType*>(&*vti->getSignalType());
        }
        return result;
    }
    DBCPPP_API void dbcppp_ValueTableForEachValueEncodingDescription(const dbcppp_ValueTable* value_table, void(*cb)(double, const char*, void*), void* data)
    {
        auto vti = reinterpret_cast<const ValueTableImpl*>(value_table);
        vti->forEachValueEncodingDescription(
            [&](double value, const std::string& desc)
            {
                cb(value, desc.c_str(), data);
            });
    }
}

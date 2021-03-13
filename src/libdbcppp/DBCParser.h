
#pragma once

#include <cstdlib>
#include <type_traits>
#include <charconv>
#include <string_view>
#include <stdexcept>
#include <vector>
#include <optional>
#include <variant>
#include <map>

namespace dbcppp
{
    using variant_attr_value_t = std::variant<int64_t, double, std::string>;

    struct G_Version
    {
        std::string version;
    };
    struct G_NewSymbols
    {
        std::vector<std::string> new_symbols;
    };
    struct G_BitTiming
    {
        uint64_t baudrate;
        uint64_t BTR1;
        uint64_t BTR2;
    };
    struct G_Node
    {
        std::string name;
    };
    struct G_ValueTable
    {
        std::string name;
        std::vector<std::tuple<int64_t, std::string>> value_encoding_descriptions;
    };
    struct G_Signal
    {
        std::string name;
        std::optional<std::string> multiplexer_indicator;
        uint64_t start_bit;
        uint64_t signal_size;
        char byte_order;
        char value_type;
        double factor;
        double offset;
        double minimum;
        double maximum;
        std::string unit;
        std::vector<std::string> receivers;
    };
    struct G_Message
    {
        uint64_t id;
        std::string name;
        uint64_t size;
        std::string transmitter;
        std::vector<G_Signal> signals;
    };
    struct G_MessageTransmitter
    {
        uint64_t id;
        std::vector<std::string> transmitters;
    };
    struct G_EnvironmentVariable
    {
        std::string name;
        uint64_t var_type;
        double minimum;
        double maximum;
        std::string unit;
        double initial_value;
        uint64_t id;
        std::string access_type;
        std::vector<std::string> access_nodes;
    };
    struct G_EnvironmentVariableData
    {
        std::string name;
        uint64_t size;
    };
    struct G_SignalType
    {
        std::string name;
        uint64_t size;
        char byte_order;
        char value_type;
        double factor;
        double offset;
        double minimum;
        double maximum;
        std::string unit;
        double default_value;
        std::string value_table_name;
    };
    struct G_CommentNetwork
    {
        std::string comment;
    };
    struct G_CommentNode
    {
        std::string node_name;
        std::string comment;
    };
    struct G_CommentMessage
    {
        uint64_t message_id;
        std::string comment;
    };
    struct G_CommentSignal
    {
        uint64_t message_id;
        std::string signal_name;
        std::string comment;
    };
    struct G_CommentEnvVar
    {
        std::string env_var_name;
        std::string comment;
    };
    using variant_comment_t = std::variant<G_CommentNetwork, G_CommentNode, G_CommentMessage, G_CommentSignal, G_CommentEnvVar>;
    struct G_Comment
    {
        variant_comment_t comment;
    };
    struct G_AttributeValueTypeInt
    {
        int64_t minimum;
        int64_t maximum;
    };
    struct G_AttributeValueTypeHex
    {
        int64_t minimum;
        int64_t maximum;
    };
    struct G_AttributeValueTypeFloat
    {
        double minimum;
        double maximum;
    };
    struct G_AttributeValueTypeString
    {
    };
    struct G_AttributeValueTypeEnum
    {
        std::vector<std::string> values;
    };
    using variant_attribute_value_t =
        std::variant<
            G_AttributeValueTypeInt, G_AttributeValueTypeHex,
            G_AttributeValueTypeFloat, G_AttributeValueTypeString,
            G_AttributeValueTypeEnum>;
    struct G_AttributeValue
    {
        variant_attribute_value_t value;
    };
    struct G_AttributeDefinition
    {
        std::optional<std::string> object_type;
        std::string name;
        G_AttributeValue value_type;
    };
    struct G_Attribute
    {
        std::string name;
        variant_attr_value_t value;
    };
    struct G_AttributeNetwork
    {
        std::string attribute_name;
        variant_attr_value_t value;
    };
    struct G_AttributeNode
    {
        std::string attribute_name;
        std::string node_name;
        variant_attr_value_t value;
    };
    struct G_AttributeMessage
    {
        std::string attribute_name;
        uint64_t message_id;
        variant_attr_value_t value;
    };
    struct G_AttributeSignal
    {
        std::string attribute_name;
        uint64_t message_id;
        std::string signal_name;
        variant_attr_value_t value;
    };
    struct G_AttributeEnvVar
    {
        std::string attribute_name;
        std::string env_var_name;
        variant_attr_value_t value;
    };
    using variant_attribute_t = std::variant<G_AttributeNetwork, G_AttributeNode, G_AttributeMessage, G_AttributeSignal, G_AttributeEnvVar>;
    struct G_ValueDescriptionSignal
    {
        uint64_t message_id;
        std::string signal_name;
        std::vector<std::tuple<int64_t, std::string>> value_descriptions;
    };
    struct G_ValueDescriptionEnvVar
    {
        std::string env_var_name;
        std::vector<std::tuple<int64_t, std::string>> value_descriptions;
    };
    struct G_ValueDescription
    {
        std::variant<G_ValueDescriptionSignal, G_ValueDescriptionEnvVar> description;
    };
    struct G_SignalExtendedValueType
    {
        uint64_t message_id;
        std::string signal_name;
        uint64_t value;
    };
    struct G_Network
    {
        G_Version version;
        std::vector<std::string> new_symbols;
        std::optional<G_BitTiming> bit_timing;
        std::vector<G_Node> nodes;
        std::vector<G_ValueTable> value_tables;
        std::vector<G_Message> messages;
        std::vector<G_MessageTransmitter> message_transmitters;
        std::vector<G_EnvironmentVariable> environment_variables;
        std::vector<G_EnvironmentVariableData> environment_variable_datas;
        std::vector<G_SignalType> signal_types;
        std::vector<variant_comment_t> comments;
        std::vector<G_AttributeDefinition> attribute_definitions;
        std::vector<G_Attribute> attribute_defaults;
        std::vector<variant_attribute_t> attribute_values;
        std::vector<G_ValueDescription> value_descriptions;
        std::vector<G_SignalExtendedValueType> signal_extended_value_types;
    };
}

class DBCIterator
{
public:
    struct Pos
    {
        std::size_t line;
        std::size_t column;
    };

    DBCIterator(const char* begin)
        : _begin(begin)
        , _cur(begin)
    {}
    DBCIterator(const DBCIterator& it)
        : _begin(it._begin)
        , _cur(it._cur)
    {}
    DBCIterator(DBCIterator&& it)
        : _begin(it._begin)
        , _cur(it._cur)
    {}
    const char* operator++()
    {
        return ++_cur;
    }
    const char* operator++(int)
    {
        auto result = _cur;
        _cur++;
        return result;
    }
    const char* operator+=(std::size_t n)
    {
        _cur += n;
        return _cur;
    }
    const char* operator+(std::size_t n) const
    {
        return _cur + n;
    }
    auto operator*() const
    {
        return *_cur;
    }
    auto Data() const
    {
        return _cur;
    }
    operator const char*() const
    {
        return _cur;
    }
    Pos GetPos() const
    {
        Pos result;
        auto iter = _begin;
        while (iter != _cur)
        {
            if (*iter == '\n')
            {
                result.line++;
                result.column = 0;
            }
            else
            {
                result.column++;
            }
            iter++;
        }
        return result;
    }

private:
    const char* _begin;
    const char* _cur;
};

class ParserError
{
public:
    ParserError(DBCIterator iter, const std::string& what)
        : _iter(iter)
        , _what(what)
    {}

private:
    std::string _what;
    DBCIterator _iter;
};

#define ExpectLit(In, Lit) if (!ParseLit(In, Lit)) throw ParserError(In, "Expected ':'");
#define ExpectCharLit(In, Lit) if (!ParseCharLit(In, Lit)) throw ParserError(In, "Expected ':'");
#define Expect(In, Func, Var, Err) if (!Func(In, Var)) throw ParserError(In, Err);
#define ParseCommaSeperatedCIdentifiers(in, Var)                                                        \
    {                                                                                                   \
        std::string identifier;                                                                         \
        if (ParseCIdentifier(in, identifier))                                                           \
        {                                                                                               \
            Var.push_back(identifier);                                                                  \
            Skip(in);                                                                                   \
            while (ParseLit(in, ","))                                                                   \
            {                                                                                           \
                Skip(in);                                                                               \
                Expect(in, ParseCIdentifier, identifier, "Expected C_identifier (" #Var ")"); Skip(in); \
                Var.push_back(identifier);                                                              \
            }                                                                                           \
        }                                                                                               \
    }
#define ParseCommaSeperatedCIdentifiersSkipSpace(in, Var)                                                       \
    {                                                                                                           \
        std::string identifier;                                                                                 \
        if (ParseCIdentifier(in, identifier))                                                                   \
        {                                                                                                       \
            Var.push_back(identifier);                                                                          \
            SkipSpace(in);                                                                                      \
            while (ParseLit(in, ","))                                                                           \
            {                                                                                                   \
                SkipSpace(in);                                                                                  \
                Expect(in, ParseCIdentifier, identifier, "Expected C_identifier (" #Var ")"); SkipSpace(in);    \
                Var.push_back(identifier);                                                                      \
            }                                                                                                   \
        }                                                                                                       \
    }

class DBCParser
{
public:
    static bool SkipSpace(DBCIterator& in)
    {
        bool result = false;
        while (*in == ' ' || *in == '\t')
        {
            result = true;
            in++;
        }
        return result;
    }
    static bool SkipNewLine(DBCIterator& in)
    {
        bool result = false;
        while (*in == '\r' || *in == '\n')
        {
            result = true;
            in++;
        }
        return result;
    }
    static bool SkipSingleLineComment(DBCIterator& in)
    {
        bool result = false;
        if (*in != '\0' && *in == '/' && *(in + 1) == '/')
        {
            result = true;
            in += 2;
            while (*in != '\0' && *in != '\n')
            {
                in++;
            }
        }
        return result;
    }
    static bool SkipBlockComment(DBCIterator& in)
    {
        bool result = false;
        if (*in != '\0' && *in == '/' && *(in + 1) == '*')
        {
            result = true;
            while (*in != '\0' && !(*in == '*' && *(in + 1) == '/'))
            {
                in++;
            }
            if (*in == '\0')
            {
                throw ParserError(in, "Could not find closing '*/'");
            }
            // Skip '*/'
            in += 2;
        }
        return result;
    }
    static bool Skip(DBCIterator& in)
    {
        bool result = false;
        while (SkipSpace(in) || SkipNewLine(in) || SkipSingleLineComment(in) || SkipBlockComment(in))
        {
            result = true;
        }
        return result;
    }
    static bool ParseChar(DBCIterator& in, char& chr)
    {
        bool result = false;
        if (*in != '\0')
        {
            result = true;
            chr = *in;
            in++;
        }
        return result;
    }
    static bool NextIsSpace(DBCIterator& in)
    {
        return *in == ' ' || *in == '\t' || *in == '\n' || *in == '\r';
    }
    static bool ParseCharLit(DBCIterator& in, char lit)
    {
        bool result = false;
        if (*in == lit)
        {
            result = true;
            in += 1;
        }
        return result;
    }
    static bool ParseLit(DBCIterator& in, std::string_view lit)
    {
        bool result = false;
        if (std::strncmp(in, lit.data(), lit.size()) == 0 && NextIsSpace(DBCIterator(in + lit.size())))
        {
            result = true;
            in += lit.size();
        }
        return result;
    }
    static bool ParseDouble(DBCIterator& in, double& r)
    {
        bool result = false;
        char* end;
        r = std::strtod(in, &end);
        if (in != end)
        {
            result = true;
            in = end;
        }
        return result;
    }
    static bool ParseInt(DBCIterator& in, int64_t& r)
    {
        bool result = false;
        char* end;
        r = std::strtol(in, &end, 10);
        if (in != end)
        {
            result = true;
            in = end;
        }
        return result;
    }
    static bool ParseUint(DBCIterator& in, uint64_t& r)
    {
        bool result = false;
        char* end;
        r = std::strtoul(in, &end, 10);
        if (in != end)
        {
            result = true;
            in = end;
        }
        return result;
    }
    static bool ParseHex(DBCIterator& in, uint64_t& r)
    {
        bool result = false;
        char* tmp, * end;
        if (*tmp == '0' && (*tmp == 'x' || *tmp == 'X'))
        {
            r = std::strtoul(tmp, &end, 16);
            if (in != end)
            {
                result = true;
                in = end;
            }
        }
        return result;
    }
    static bool ParseCharString(DBCIterator& in, std::string& str)
    {
        bool result = false;
        auto beg = in;
        if (*in == '\"')
        {
            in++;
            while (*in != '\0' && *in != '\"')
            {
                in++;
                if (*in == '\\' && *(in + 1) == '"')
                {
                    in += 2;
                }
            }
            if (*in == '\0')
            {
                throw ParserError(in, "Could not find closing '\"'");
            }
            // Skip "
            in++;
        }
        if (beg != in)
        {
            result = true;
            str = {beg + 1, in - 1};
        }
        return result;
    }
    static bool ParseCIdentifier(DBCIterator& in, std::string& identifier)
    {
        identifier = "";
        auto beg = in;
        if (('a' <= *in && *in <= 'z') ||
            ('A' <= *in && *in <= 'Z') ||
             '_' == *in)
        {
            in++;
            while (('a' <= *in && *in <= 'z') ||
                 ('A' <= *in && *in <= 'Z') ||
                 ('0' <= *in && *in <= '9') ||
                  '_' == *in)
            {
                in++;
            }
            identifier = {beg.Data(), in.Data()};
        }
        return beg != in;
    }
    static bool ParseVersion(DBCIterator& in, dbcppp::G_Version& version)
    {
        bool result = false;
        if (ParseLit(in, "VERSION"))
        {
            result = true;
            SkipSpace(in);
            Expect(in, ParseCharString, version.version, "Expected char_string (version.version)");
        }
        return result;
    }
    static bool ParseBitTiming(DBCIterator& in, dbcppp::G_BitTiming& bit_timing)
    {
        bool result = false;
        if (ParseLit(in, "BS_"))
        {
            result = true;
            Skip(in);
            ExpectCharLit(in, ':') Skip(in);
            if (ParseUint(in, bit_timing.baudrate))
            {
                Skip(in);
                ExpectCharLit(in, ':'); Skip(in);
                Expect(in, ParseUint, bit_timing.BTR1, "Expected Uint (bit_timing.BTR1)"); Skip(in);
                ExpectCharLit(in, ','); Skip(in);
                Expect(in, ParseUint, bit_timing.BTR2, "Expected Uint (bit_timing.BTR2)")
            }
        }
        return result;
    }
    static bool ParseNodes(DBCIterator& in, std::vector<dbcppp::G_Node>& nodes)
    {
        bool result = false;
        if (ParseLit(in, "BU_"))
        {
            Skip(in);
            ExpectCharLit(in, ':'); SkipSpace(in);
            dbcppp::G_Node node;
            while (ParseCIdentifier(in, node.name))
            {
                nodes.push_back(node);
                SkipSpace(in);
            }
        }
    }
    static bool ParseNewSymbol(DBCIterator& in)
    {
        static constexpr std::string_view new_symbols[] =
            {
                {"SIGTYPE_VALTYPE_"},
                {"BA_DEF_DEF_REL_"},
                {"BA_DEF_SGTYPE_"},
                {"SIG_TYPE_REF_"},
                {"ENVVAR_DATA_"},
                {"SIG_VALTYPE_"},
                {"SG_MUL_VAL_"},
                {"BA_DEF_DEF_"},
                {"ENVVAR_DTA_"},
                {"BA_DEF_REL_"},
                {"SGTYPE_VAL_"},
                {"VAL_TABLE_"},
                {"BA_SGTYPE_"},
                {"SIG_GROUP_"},
                {"BU_SG_REL_"},
                {"BU_EV_REL_"},
                {"BU_BO_REL_"},
                {"BO_TX_BU_"},
                {"NS_DESC_"},
                {"CAT_DEF_"},
                {"EV_DATA_"},
                {"BA_REL_"},
                {"SGTYPE_"},
                {"BA_DEF_"},
                {"FILTER"},
                {"DEF_"},
                {"VAL_"},
                {"CAT_"},
                {"BA_"},
                {"CM_"}
            };
        static constexpr std::size_t num_syms = sizeof(new_symbols) / sizeof(std::string_view);
        bool result = false;
        for (std::size_t i = 0; i < num_syms; i++)
        {
            if (ParseLit(in, new_symbols[i]))
            {
                result = true;
                break;
            }
        }
        return result;
    }
    static bool ParseNewSymbols(DBCIterator& in)
    {
        bool result = false;
        if (ParseLit(in, "NS_"))
        {
            Skip(in);
            ExpectCharLit(in, ':'); Skip(in);
            while (ParseNewSymbol(in))
            {
                Skip(in);
            }
        }
    }
    static bool ParseValueEncodingDescription(DBCIterator& in, std::tuple<int64_t, std::string>& ved)
    {
        bool result = false;
        if (ParseInt(in, std::get<0>(ved)))
        {
            result = true;
            Skip(in);
            Expect(in, ParseCharString, std::get<1>(ved), "Error: Expected encoding description (char_string)");
        }
        return result;
    }
    static bool ParseValueEncodingDescriptions(DBCIterator& in, std::vector<std::tuple<int64_t, std::string>>& veds)
    {
        bool result = false;
        std::tuple<int64_t, std::string> ved;
        while (ParseValueEncodingDescription(in, ved))
        {
            result = true;
            veds.push_back(ved);
            Skip(in);
        }
        return result;
    }
    static bool ParseValueTable(DBCIterator& in, dbcppp::G_ValueTable& value_table)
    {
        bool result = false;
        if (ParseLit(in, "VAL_TABLE_"))
        {
            result = true;
            Skip(in);
            Expect(in, ParseCIdentifier, value_table.name, "Error: Expected VAL_TABLE_ name (C_Identifier)");
            Skip(in);
            ParseValueEncodingDescriptions(in, value_table.value_encoding_descriptions);
        }
        return result;
    }
    static bool ParseValueTables(DBCIterator& in, std::vector<dbcppp::G_ValueTable>& value_tables)
    {
        bool result = false;
        dbcppp::G_ValueTable value_table;
        while (ParseValueTable(in, value_table))
        {
            result = true;
            value_tables.push_back(value_table);
            Skip(in);
        }
        return result;
    }
    static bool ParseSignal(DBCIterator& in, dbcppp::G_Signal& signal)
    {
        bool result = false;
        if (ParseLit(in, "SG_"))
        {
            result = true;
            Skip(in);
            Expect(in, ParseCIdentifier, signal.name, "Expected C_identifier (signal.name)"); Skip(in);
            std::string multiplexer_indicator;
            if (ParseCIdentifier(in, multiplexer_indicator))
            {
                signal.multiplexer_indicator = std::move(multiplexer_indicator);
            }
            Skip(in);
            ExpectCharLit(in, ':'); Skip(in);
            Expect(in, ParseUint, signal.start_bit, "Expected Uint (signal.start_bit)"); Skip(in);
            ExpectCharLit(in, '|'); Skip(in);
            Expect(in, ParseUint, signal.signal_size, "Expected Uint (signal.signal_size)"); Skip(in);
            ExpectCharLit(in, '@'); Skip(in);
            Expect(in, ParseChar, signal.byte_order, "Expected char (signal.byte_order)"); Skip(in);
            Expect(in, ParseChar, signal.value_type, "Expected char (signal.value_type)"); Skip(in);
            ExpectCharLit(in, '('); Skip(in);
            Expect(in, ParseDouble, signal.factor, "Expected double (signal.factor)"); Skip(in);
            ExpectCharLit(in, ','); Skip(in);
            Expect(in, ParseDouble, signal.offset, "Expected double (signal.offset)"); Skip(in);
            ExpectCharLit(in, ')'); Skip(in);
            ExpectCharLit(in, '['); Skip(in);
            Expect(in, ParseDouble, signal.minimum, "Expected double (signal.minimum)"); Skip(in);
            ExpectCharLit(in, '|'); Skip(in);
            Expect(in, ParseDouble, signal.maximum, "Expected double (signal.maximum)"); Skip(in);
            ExpectCharLit(in, ']'); Skip(in);
            Expect(in, ParseCharString, signal.unit, "Expected char_string (signal.unit)"); Skip(in);
            ParseCommaSeperatedCIdentifiersSkipSpace(in, signal.receivers);
        }
        return result;
    }
    static bool ParseSignals(DBCIterator& in, std::vector<dbcppp::G_Signal>& signals)
    {
        bool result = false;
        dbcppp::G_Signal signal;
        while (ParseSignal(in, signal))
        {
            result = true;
            signals.push_back(signal);
            Skip(in);
        }
        return result;
    }
    static bool ParseMessage(DBCIterator& in, dbcppp::G_Message& message)
    {
        bool result = false;
        if (ParseLit(in, "BO_"))
        {
            result = true;
            Skip(in);
            Expect(in, ParseUint, message.id, "Expected Uint (message.id)"); Skip(in);
            Expect(in, ParseCIdentifier, message.name, "Expected C_identifier (message.name)"); Skip(in);
            ExpectCharLit(in, ':'); Skip(in);
            Expect(in, ParseUint, message.size, "Expected Uint (message.size)"); Skip(in);
            Expect(in, ParseCIdentifier, message.transmitter, "Expected C_identifier (message.transmitter)"); Skip(in);
            ParseSignals(in, message.signals);
        }
        return result;
    }
    static bool ParseMessages(DBCIterator& in, std::vector<dbcppp::G_Message>& messages)
    {
        bool result = false;
        dbcppp::G_Message message;
        while (ParseMessage(in, message))
        {
            result = true;
            messages.push_back(message);
            Skip(in);
        }
        return result;
    }
    static bool ParseMessageTransmitter(DBCIterator& in, dbcppp::G_MessageTransmitter& message_transmitter)
    {
        bool result = false;
        if (ParseLit(in, "BO_TX_BU_"))
        {
            result = true;
            Skip(in);
            Expect(in, ParseUint, message_transmitter.id, "Expected Uint (message_transmitter.id)"); Skip(in);
            ExpectCharLit(in, ':'); Skip(in);
            ParseCommaSeperatedCIdentifiers(in, message_transmitter.transmitters); Skip(in);
            ExpectCharLit(in, ';'); Skip(in);
        }
        return result;
    }
    static bool ParseMessageTransmitters(DBCIterator& in, std::vector<dbcppp::G_MessageTransmitter>& message_transmitters)
    {
        bool result = false;
        dbcppp::G_MessageTransmitter message_transmitter;
        while (ParseMessageTransmitter(in, message_transmitter))
        {
            result = true;
            message_transmitters.push_back(message_transmitter);
            Skip(in);
        }
        return result = true;
    }
    static bool ParseEnvironmentVariable(DBCIterator& in, dbcppp::G_EnvironmentVariable& environment_variable)
    {
        bool result = false;
        if (ParseLit(in, "EV_"))
        {
            result = true;
            Skip(in);
            Expect(in, ParseCIdentifier, environment_variable.name, "Expected C_identifier (environment_variable.name)"); Skip(in);
            ExpectCharLit(in, ':'); Skip(in);
            Expect(in, ParseUint, environment_variable.var_type, "Expect Uint (environment_variable.var_type)"); Skip(in);
            ExpectCharLit(in, '['); Skip(in);
            Expect(in, ParseDouble, environment_variable.minimum, "Expected Double (environment_variable.minimum)"); Skip(in);
            ExpectCharLit(in, '|'); Skip(in);
            Expect(in, ParseDouble, environment_variable.maximum, "Expected Double (environment_variable.maximum)"); Skip(in);
            ExpectCharLit(in, ']'); Skip(in);
            Expect(in, ParseCharString, environment_variable.unit, "Expected CharString (environment_variable.unit)"); Skip(in);
            Expect(in, ParseDouble, environment_variable.initial_value, "Expected Double (environment_variable.initial_value)"); Skip(in);
            Expect(in, ParseUint, environment_variable.id, "Expected Uint (environment_variable.id)"); Skip(in);
            Expect(in, ParseCharString, environment_variable.access_type, "Expected Uint (environment_variable.access_type)"); Skip(in);
            ParseCommaSeperatedCIdentifiers(in, environment_variable.access_nodes); Skip(in);
            ExpectCharLit(in, ';');
        }
        return result;
    }
    static bool ParseEnvironmentVariables(DBCIterator& in, std::vector<dbcppp::G_EnvironmentVariable>& environment_variables)
    {
        bool result = false;
        dbcppp::G_EnvironmentVariable environment_variable;
        while (ParseEnvironmentVariable(in, environment_variable))
        {
            result = true;
            environment_variables.push_back(environment_variable);
            Skip(in);
        }
        return result = true;
    }
    static bool ParseEnvironmentVariableData(DBCIterator& in, dbcppp::G_EnvironmentVariableData& environment_variable_data)
    {
        bool result = false;
        if (ParseLit(in, "ENVVAR_DATA_"))
        {
            result = true;
            Skip(in);
            Expect(in, ParseCIdentifier, environment_variable_data.name, "Expected C_identifier (environment_variable_data.name)"); Skip(in);
            ExpectCharLit(in, ':'); Skip(in);
            Expect(in, ParseUint, environment_variable_data.size, "Expect Uint (environment_variable_data.size)"); Skip(in);
            ExpectCharLit(in, ';');
        }
        return result;
    }
    static bool ParseEnvironmentVariableDatas(DBCIterator& in, std::vector<dbcppp::G_EnvironmentVariableData>& environment_variable_datas)
    {
        bool result = false;
        dbcppp::G_EnvironmentVariableData environment_variable_data;
        while (ParseEnvironmentVariableData(in, environment_variable_data))
        {
            result = true;
            environment_variable_datas.push_back(environment_variable_data);
            Skip(in);
        }
        return result = true;
    }
    static bool ParseSignalType(DBCIterator& in, dbcppp::G_SignalType& signal_type)
    {
        bool result = false;
        if (ParseLit(in, "SGTYPE_"))
        {
            result = true;
            Skip(in);
            Expect(in, ParseCIdentifier, signal_type.name, "Expected C_identifier (signal_type.name)"); Skip(in);
            ExpectCharLit(in, ':'); Skip(in);
            Expect(in, ParseUint, signal_type.size, "Expected Uint (signal_type.size)"); Skip(in);
            ExpectCharLit(in, '@'); Skip(in);
            Expect(in, ParseChar, signal_type.byte_order, "Expected Char (signal_type.byte_order)"); Skip(in);
            Expect(in, ParseChar, signal_type.value_type, "Expected Char (signal_type.value_type)"); Skip(in);
            ExpectCharLit(in, '('); Skip(in);
            Expect(in, ParseDouble, signal_type.factor, "Expected Double (signal_type.factor)"); Skip(in);
            ExpectCharLit(in, ','); Skip(in);
            Expect(in, ParseDouble, signal_type.offset, "Expected Double (signal_type.offset)"); Skip(in);
            ExpectCharLit(in, ')'); Skip(in);
            ExpectCharLit(in, '['); Skip(in);
            Expect(in, ParseDouble, signal_type.minimum, "Expected Double (signal_type.minimum)"); Skip(in);
            ExpectCharLit(in, '|'); Skip(in);
            Expect(in, ParseDouble, signal_type.maximum, "Expected Double (signal_type.maximum)"); Skip(in);
            ExpectCharLit(in, ']'); Skip(in);
            Expect(in, ParseCharString, signal_type.unit, "Expected CharString (signal_type.unit)"); Skip(in);
            Expect(in, ParseDouble, signal_type.default_value, "Expected Double (signal_type.default_value)"); Skip(in);
            ExpectCharLit(in, ','); Skip(in);
            Expect(in, ParseCIdentifier, signal_type.value_table_name, "Expected C_identifier (signal_type.value_table_name)"); Skip(in);
            ExpectCharLit(in, ';');
        }
        return result;
    }
    static bool ParseSignalTypes(DBCIterator& in, std::vector<dbcppp::G_SignalType>& signal_types)
    {
        bool result = false;
        dbcppp::G_SignalType signal_type;
        while (ParseSignalType(in, signal_type))
        {
            result = true;
            signal_types.push_back(signal_type);
            Skip(in);
        }
        return result = true;
    }
    static bool ParseComment(DBCIterator& in, dbcppp::G_Comment& comment)
    {
        bool result = false;
        if (ParseLit(in, "CM_"))
        {
            result = true;
            Skip(in);
            if (std::string com; ParseCharString(in, com))
            {
                comment.comment = dbcppp::G_CommentNetwork{com};
            }
            else if (ParseLit(in, "BU_"))
            {
                std::string node_name;
                std::string com;
                Expect(in, ParseCIdentifier, node_name, "Expected C_identifier (node_name)"); Skip(in);
                Expect(in, ParseCharString, com, "Expect Uint (comment)"); Skip(in);
                comment.comment = dbcppp::G_CommentNode{node_name, com};
            }
            else if (ParseLit(in, "BO_"))
            {
                uint64_t message_id;
                std::string com;
                Expect(in, ParseUint, message_id, "Expected Uint (message_id)"); Skip(in);
                Expect(in, ParseCharString, com, "Expect Uint (comment)"); Skip(in);
                comment.comment = dbcppp::G_CommentMessage{message_id, com};
            }
            else if (ParseLit(in, "SG_"))
            {
                uint64_t message_id;
                std::string signal_name;
                std::string com;
                Expect(in, ParseUint, message_id, "Expected Uint (message_id)"); Skip(in);
                Expect(in, ParseCIdentifier, signal_name, "Expect C_identifier (signal_name)"); Skip(in);
                Expect(in, ParseCharString, com, "Expect Uint (comment)"); Skip(in);
                comment.comment = dbcppp::G_CommentSignal{message_id, signal_name, com};
            }
            else if (ParseLit(in, "EV_"))
            {
                std::string env_var_name;
                std::string com;
                Expect(in, ParseCIdentifier, env_var_name, "Expected C_identifier (env_var_name)"); Skip(in);
                Expect(in, ParseCharString, com, "Expect Uint (comment)"); Skip(in);
                comment.comment = dbcppp::G_CommentEnvVar{env_var_name, com};
            }
            else
            {
                throw ParserError(in, "Expected BU_, BO_, SG_, EV_ or CharString (comment)");
            }
            ExpectCharLit(in, ';');
        }
        return result;
    }
    static bool ParseComments(DBCIterator& in, std::vector<dbcppp::G_Comment>& comments)
    {
        bool result = false;
        dbcppp::G_Comment comment;
        while (ParseComment(in, comment))
        {
            result = true;
            comments.push_back(comment);
            Skip(in);
        }
        return result = true;
    }
};

#pragma once

#include <string>
#include <vector>

#include <boost/variant.hpp>
#include <boost/optional.hpp>
#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>

namespace dbcppp
{
    namespace DBCX3
    {
        namespace AST
        {
            using variant_attr_value_t = boost::variant<int64_t, double, std::string>;
            struct G_Version
                : boost::spirit::x3::position_tagged
            {
                std::string version;
            };
            struct G_NewSymbols
                : boost::spirit::x3::position_tagged
            {
                boost::optional<std::vector<std::string>> new_symbols;
            };
            struct G_BitTiming
                : boost::spirit::x3::position_tagged
            {
                uint64_t baudrate;
                uint64_t BTR1;
                uint64_t BTR2;
            };
            struct G_Node
                : boost::spirit::x3::position_tagged
            {
                std::string name;
            };
            struct G_ValueEncodingDescription
                : boost::spirit::x3::position_tagged
            {
                int64_t value;
                std::string description;
            };
            struct G_ValueTable
                : boost::spirit::x3::position_tagged
            {
                std::string name;
                std::vector<G_ValueEncodingDescription> value_encoding_descriptions;
            };
            struct G_Signal
                : boost::spirit::x3::position_tagged
            {
                std::string name;
                boost::optional<std::string> multiplexer_indicator;
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
                : boost::spirit::x3::position_tagged
            {
                uint64_t id;
                std::string name;
                uint64_t size;
                std::string transmitter;
                std::vector<G_Signal> signals;
            };
            struct G_MessageTransmitter
                : boost::spirit::x3::position_tagged
            {
                uint64_t id;
                std::vector<std::string> transmitters;
            };
            struct G_EnvironmentVariable
                : boost::spirit::x3::position_tagged
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
                : boost::spirit::x3::position_tagged
            {
                std::string name;
                uint64_t size;
            };
            struct G_SignalType
                : boost::spirit::x3::position_tagged
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
                : boost::spirit::x3::position_tagged
            {
                std::string comment;
            };
            struct G_CommentNode
                : boost::spirit::x3::position_tagged
            {
                std::string node_name;
                std::string comment;
            };
            struct G_CommentMessage
                : boost::spirit::x3::position_tagged
            {
                uint64_t message_id;
                std::string comment;
            };
            struct G_CommentSignal
                : boost::spirit::x3::position_tagged
            {
                uint64_t message_id;
                std::string signal_name;
                std::string comment;
            };
            struct G_CommentEnvVar
                : boost::spirit::x3::position_tagged
            {
                std::string env_var_name;
                std::string comment;
            };
            using variant_comment_t = boost::variant<G_CommentNetwork, G_CommentNode, G_CommentMessage, G_CommentSignal, G_CommentEnvVar>;
            struct G_Comment
                : boost::spirit::x3::position_tagged
            {
                variant_comment_t comment;
            };
            struct G_AttributeValueTypeInt
                : boost::spirit::x3::position_tagged
            {
                int64_t minimum;
                int64_t maximum;
            };
            struct G_AttributeValueTypeHex
                : boost::spirit::x3::position_tagged
            {
                int64_t minimum;
                int64_t maximum;
            };
            struct G_AttributeValueTypeFloat
                : boost::spirit::x3::position_tagged
            {
                double minimum;
                double maximum;
            };
            struct G_AttributeValueTypeString
                : boost::spirit::x3::position_tagged
            {
            };
            struct G_AttributeValueTypeEnum
                : boost::spirit::x3::position_tagged
            {
                std::vector<std::string> values;
            };
            using variant_attribute_value_t =
                boost::variant<
                    G_AttributeValueTypeInt, G_AttributeValueTypeHex,
                    G_AttributeValueTypeFloat, G_AttributeValueTypeString,
                    G_AttributeValueTypeEnum>;
            struct G_AttributeValue
                : boost::spirit::x3::position_tagged
            {
                variant_attribute_value_t value;
            };
            struct G_AttributeDefinition
                : boost::spirit::x3::position_tagged
            {
                boost::optional<std::string> object_type;
                std::string name;
                G_AttributeValue value_type;
            };
            struct G_Attribute
                : boost::spirit::x3::position_tagged
            {
                std::string name;
                variant_attr_value_t value;
            };
            struct G_AttributeNetwork
                : boost::spirit::x3::position_tagged
            {
                std::string attribute_name;
                variant_attr_value_t value;
            };
            struct G_AttributeNode
                : boost::spirit::x3::position_tagged
            {
                std::string attribute_name;
                std::string node_name;
                variant_attr_value_t value;
            };
            struct G_AttributeMessage
                : boost::spirit::x3::position_tagged
            {
                std::string attribute_name;
                uint64_t message_id;
                variant_attr_value_t value;
            };
            struct G_AttributeSignal
                : boost::spirit::x3::position_tagged
            {
                std::string attribute_name;
                uint64_t message_id;
                std::string signal_name;
                variant_attr_value_t value;
            };
            struct G_AttributeEnvVar
                : boost::spirit::x3::position_tagged
            {
                std::string attribute_name;
                std::string env_var_name;
                variant_attr_value_t value;
            };
            struct G_ValueDescription
                : boost::spirit::x3::position_tagged
            {
                int64_t value;
                std::string description;
            };
            using variant_attribute_t = boost::variant<G_AttributeNetwork, G_AttributeNode, G_AttributeMessage, G_AttributeSignal, G_AttributeEnvVar>;
            struct G_ValueDescriptionSignal
                : boost::spirit::x3::position_tagged
            {
                uint64_t message_id;
                std::string signal_name;
                std::vector<G_ValueDescription> value_descriptions;
            };
            struct G_ValueDescriptionEnvVar
                : boost::spirit::x3::position_tagged
            {;
                std::string env_var_name;
                std::vector<G_ValueDescription> value_descriptions;
            };
            struct G_ValueDescriptionSigEnvVar
                : boost::spirit::x3::position_tagged
            {
                boost::variant<G_ValueDescriptionSignal, G_ValueDescriptionEnvVar> description;
            };
            struct G_SignalExtendedValueType
                : boost::spirit::x3::position_tagged
            {
                uint64_t message_id;
                std::string signal_name;
                uint64_t value;
            };
            struct G_Range
                : boost::spirit::x3::position_tagged
            {
                uint64_t from;
                uint64_t to;
            };
            struct G_SignalMultiplexerValue
                : boost::spirit::x3::position_tagged
            {
                uint64_t message_id;
                std::string signal_name;
                std::string switch_name;
                std::vector<G_Range> value_ranges;
            };
            struct G_SignalGroup
            {
                uint64_t message_id;
                std::string signal_group_name;
                uint64_t repetitions;
                std::vector<std::string> signal_names;
            };

            struct G_Network
                : boost::spirit::x3::position_tagged
            {
                G_Version version;
                std::vector<std::string> new_symbols;
                boost::optional<G_BitTiming> bit_timing;
                std::vector<G_Node> nodes;
                std::vector<G_ValueTable> value_tables;
                std::vector<G_Message> messages;
                std::vector<G_MessageTransmitter> message_transmitters;
                std::vector<G_EnvironmentVariable> environment_variables;
                std::vector<G_EnvironmentVariableData> environment_variable_datas;
                std::vector<G_SignalType> signal_types;
                std::vector<G_Comment> comments;
                std::vector<G_AttributeDefinition> attribute_definitions;
                std::vector<G_Attribute> attribute_defaults;
                std::vector<variant_attribute_t> attribute_values;
                std::vector<G_ValueDescriptionSigEnvVar> value_descriptions_sig_env_var;
                std::vector<G_SignalGroup> signal_groups;
                std::vector<G_SignalExtendedValueType> signal_extended_value_types;
                std::vector<G_SignalMultiplexerValue> signal_multiplexer_values;
            };
        }
        std::optional<AST::G_Network> DBCPPP_API ParseFromMemory(const char* begin, const char* end);
    }
}
#pragma once

#include <string>
#include <vector>

#include <boost/variant.hpp>
#include <boost/optional.hpp>

namespace dbcppp
{
    namespace DBCX3
    {
        namespace AST
        {
            using variant_attr_value_t = boost::variant<int64_t, double, std::string>;

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
            using variant_comment_t = boost::variant<G_CommentNetwork, G_CommentNode, G_CommentMessage, G_CommentSignal, G_CommentEnvVar>;
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
                boost::variant<
                    G_AttributeValueTypeInt, G_AttributeValueTypeHex,
                    G_AttributeValueTypeFloat, G_AttributeValueTypeString,
                    G_AttributeValueTypeEnum>;
            struct G_AttributeValue
            {
                variant_attribute_value_t value;
            };
            struct G_AttributeDefinition
            {
                boost::optional<std::string> object_type;
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
            using variant_attribute_t = boost::variant<G_AttributeNetwork, G_AttributeNode, G_AttributeMessage, G_AttributeSignal, G_AttributeEnvVar>;
            struct G_ValueDescriptionSignal
            {
                uint64_t message_id;
                std::string signal_name;
                std::vector<std::tuple<int64_t, std::string>> value_descriptions;
            };
            struct G_ValueDescriptionEnvVar
            {;
                std::string env_var_name;
                std::vector<std::tuple<int64_t, std::string>> value_descriptions;
            };
            struct G_ValueDescription
            {
                boost::variant<G_ValueDescriptionSignal, G_ValueDescriptionEnvVar> description;
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
                boost::optional<G_BitTiming> bit_timing;
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
        std::optional<AST::G_Network> DBCPPP_API ParseFromMemory(const char* begin, const char* end);
    }
}
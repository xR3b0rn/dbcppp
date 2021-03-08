
#include <vector>
#include <optional>
#include <variant>
#include <charconv>
#include <utility>

#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/parse_tree.hpp>

template <class TLhs, class Rhs>
struct samett
{
    static constexpr bool value = false;
};
template <template <class> class TLhs, template <class> class TRhs, class Rhs>
struct samett<TLhs<Rhs>, TRhs<Rhs>>
{
    static constexpr bool value = std::is_same_v<TLhs<Rhs>, TRhs<Rhs>>;
};
template <template <class> class Tag>
constexpr auto get_tagged()
{
  return std::tuple<>();
}
template <template <class> class Tag, class T, class... Args>
constexpr auto get_tagged(const T& t, const Args&... args)
{
  if constexpr (samett<Tag, T>)
  {
    return std::tuple_cat(std::tuple<T>(), get_tagged<Tag>(args...));
  }
  else
  {
    return get_tagged<Tag>(args...);
  }
}

using variant_attr_value_t = std::variant<int64_t, double, std::string>;

struct G_Version
{
  std::string::iterator position;
  std::string version;
};
struct G_NewSymbols
{
  std::string::iterator position;
  std::vector<std::string> new_symbols;
};
struct G_BitTiming
{
  std::string::iterator position;
  uint64_t baudrate;
  uint64_t BTR1;
  uint64_t BTR2;
};
struct G_Node
{
  std::string::iterator position;
  std::string name;
};
struct G_ValueTable
{
  std::string::iterator position;
  std::string name;
  std::vector<std::tuple<int64_t, std::string>> value_encoding_descriptions;
};
struct G_Signal
{
  std::string::iterator position;
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
  std::string::iterator position;
  uint64_t id;
  std::string name;
  uint64_t size;
  std::string transmitter;
  std::vector<G_Signal> signals;
};
struct G_MessageTransmitter
{
  std::string::iterator position;
  uint64_t id;
  std::vector<std::string> transmitters;
};
struct G_EnvironmentVariable
{
  std::string::iterator position;
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
  std::string::iterator position;
  std::string name;
  uint64_t size;
};
struct G_SignalType
{
  std::string::iterator position;
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
  std::string::iterator position;
  std::string comment;
};
struct G_CommentNode
{
  std::string::iterator position;
  std::string node_name;
  std::string comment;
};
struct G_CommentMessage
{
  std::string::iterator position;
  uint64_t message_id;
  std::string comment;
};
struct G_CommentSignal
{
  std::string::iterator position;
  uint64_t message_id;
  std::string signal_name;
  std::string comment;
};
struct G_CommentEnvVar
{
  std::string::iterator position;
  std::string env_var_name;
  std::string comment;
};
using variant_comment_t = std::variant<G_CommentNetwork, G_CommentNode, G_CommentMessage, G_CommentSignal, G_CommentEnvVar>;
struct G_Comment
{
  std::string::iterator position;
  variant_comment_t comment;
};
struct G_AttributeValueTypeInt
{
  std::string::iterator position;
  int64_t minimum;
  int64_t maximum;
};
struct G_AttributeValueTypeHex
{
  std::string::iterator position;
  int64_t minimum;
  int64_t maximum;
};
struct G_AttributeValueTypeFloat
{
  std::string::iterator position;
  double minimum;
  double maximum;
};
struct G_AttributeValueTypeString
{
  std::string::iterator position;
};
struct G_AttributeValueTypeEnum
{
    std::string::iterator position;
    std::vector<std::string> values;
};
using variant_attribute_value_t =
  std::variant<
      G_AttributeValueTypeInt, G_AttributeValueTypeHex
    , G_AttributeValueTypeFloat, G_AttributeValueTypeString
    , G_AttributeValueTypeEnum>;
struct G_AttributeValue
{
  std::string::iterator position;
  variant_attribute_value_t value;
};
struct G_AttributeDefinition
{
  std::string::iterator position;
  std::optional<std::string> object_type;
  std::string name;
  G_AttributeValue value_type;
};
struct G_Attribute
{
  std::string::iterator position;
  std::string name;
  variant_attr_value_t value;
};
struct G_AttributeNetwork
{
  std::string::iterator position;
  std::string attribute_name;
  variant_attr_value_t value;
};
struct G_AttributeNode
{
  std::string::iterator position;
  std::string attribute_name;
  std::string node_name;
  variant_attr_value_t value;
};
struct G_AttributeMessage
{
  std::string::iterator position;
  std::string attribute_name;
  uint64_t message_id;
  variant_attr_value_t value;
};
struct G_AttributeSignal
{
  std::string::iterator position;
  std::string attribute_name;
  uint64_t message_id;
  std::string signal_name;
  variant_attr_value_t value;
};
struct G_AttributeEnvVar
{
  std::string::iterator position;
  std::string attribute_name;
  std::string env_var_name;
  variant_attr_value_t value;
};
using variant_attribute_t = std::variant<G_AttributeNetwork, G_AttributeNode, G_AttributeMessage, G_AttributeSignal, G_AttributeEnvVar>;
struct G_ValueDescriptionSignal
{
  std::string::iterator position;
  uint64_t message_id;
  std::string signal_name;
  std::vector<std::tuple<int64_t, std::string>> value_descriptions;
};
struct G_ValueDescriptionEnvVar
{
  std::string::iterator position;
  std::string env_var_name;
  std::vector<std::tuple<int64_t, std::string>> value_descriptions;
};
struct G_ValueDescription
{
  std::string::iterator position;
  std::variant<G_ValueDescriptionSignal, G_ValueDescriptionEnvVar> description;
};
struct G_SignalExtendedValueType
{
  std::string::iterator position;
  uint64_t message_id;
  std::string signal_name;
  uint64_t value;
};
struct G_Network
{
  std::string::iterator position;
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

using namespace tao::pegtl;

struct _blank : sor<one<' ', '\t'>> {};
struct _space : sor<_blank, eol> {};
struct _single_line_comment : seq<two<'/'>, until<eolf>> {};
struct _block_comment : seq<string<'/', '*'>, must<until<TAO_PEGTL_STRING("*/")>>> {};
template <class R>
struct _ss : seq<star<sor<_space, _single_line_comment, _block_comment>>, R>{};
template <class R>
struct _sb : seq<star<sor<_blank, _block_comment>>, R>{};
struct _hex : seq<one<'0'>, sor<one<'x'>, one<'X'>>, plus<xdigit>> {};
struct _unsigned_integer : seq<opt<one<'+'>>, sor<_hex, plus<digit>>> {};
struct _signed_integer : seq<opt<one<'-', '+'>>, sor<_hex, plus<digit>>> {};
struct _double : sor<seq<one<'.'>, plus<digit>>, seq<plus<digit>, one<'.'>, star<digit>>> {};
template <class T> struct _char_string_tag { T& value; };
struct _char_string : seq<one<'"'>, star<sor<seq<one<'\\'>, one<'"'>>, not_one<'"'>>>, one<'"'>> {};
struct _C_identifier : identifier {};
struct _start_bit : _unsigned_integer {};
struct _signal_size : _unsigned_integer {};
struct _factor : _double {};
struct _offset : _double {};
struct _minimum  : _double {};
struct _maximum : _double {};
struct _byte_order : one<'0', '1'> {};
struct _value_type : one<'-', '+'> {};
struct _version
{
  using rule_t = seq<TAO_PEGTL_ISTRING("VERSION"), must<_ss<_char_string>>>;
  template <
      apply_mode A
    , rewind_mode M
    , template <class...> class Action
    , template <class...> class Control
    , class ParseInput
    , class... States>
  static bool
  match(ParseInput& in, G_Network& net)
  {
    auto result = tao::pegtl::match<rule_t, A, M, Action, Control, ParseInput>(in, net.version.version);
    return result;
  }
};
struct _new_symbol
  : sor<
      TAO_PEGTL_STRING("SIGTYPE_VALTYPE_")
    , TAO_PEGTL_STRING("BA_DEF_DEF_REL_")
    , TAO_PEGTL_STRING("BA_DEF_SGTYPE_")
    , TAO_PEGTL_STRING("SIG_TYPE_REF_")
    , TAO_PEGTL_STRING("ENVVAR_DATA_")
    , TAO_PEGTL_STRING("SIG_VALTYPE_")
    , TAO_PEGTL_STRING("SG_MUL_VAL_")
    , TAO_PEGTL_STRING("BA_DEF_DEF_")
    , TAO_PEGTL_STRING("ENVVAR_DTA_")
    , TAO_PEGTL_STRING("BA_DEF_REL_")
    , TAO_PEGTL_STRING("SGTYPE_VAL_")
    , TAO_PEGTL_STRING("VAL_TABLE_")
    , TAO_PEGTL_STRING("BA_SGTYPE_")
    , TAO_PEGTL_STRING("SIG_GROUP_")
    , TAO_PEGTL_STRING("BU_SG_REL_")
    , TAO_PEGTL_STRING("BU_EV_REL_")
    , TAO_PEGTL_STRING("BU_BO_REL_")
    , TAO_PEGTL_STRING("BO_TX_BU_")
    , TAO_PEGTL_STRING("NS_DESC_")
    , TAO_PEGTL_STRING("CAT_DEF_")
    , TAO_PEGTL_STRING("EV_DATA_")
    , TAO_PEGTL_STRING("BA_REL_")
    , TAO_PEGTL_STRING("SGTYPE_")
    , TAO_PEGTL_STRING("BA_DEF_")
    , TAO_PEGTL_STRING("FILTER")
    , TAO_PEGTL_STRING("DEF_")
    , TAO_PEGTL_STRING("VAL_")
    , TAO_PEGTL_STRING("CAT_")
    , TAO_PEGTL_STRING("BA_")
    , TAO_PEGTL_STRING("CM_")>
{};
struct _new_symbols : seq<TAO_PEGTL_STRING("NS_"), must<seq<_ss<one<':'>>, star<_ss<_new_symbol>>>>> {};
struct _baudrate
{
  using rule_t = _unsigned_integer;
  template <
      apply_mode A
    , rewind_mode M
    , template <class...> class Action
    , template <class...> class Control
    , class ParseInput>
  static bool
  match(ParseInput& in, G_BitTiming& bit_timing)
  {
    return tao::pegtl::match<rule_t, A, M, Action, Control, ParseInput>(in, bit_timing.baudrate);
  }
};
struct _BTR1 : _unsigned_integer
{
  using rule_t = _unsigned_integer;
  template <
      apply_mode A
    , rewind_mode M
    , template <class...> class Action
    , template <class...> class Control
    , class ParseInput>
  static bool
  match(ParseInput& in, G_BitTiming& bit_timing)
  {
    return tao::pegtl::match<rule_t, A, M, Action, Control, ParseInput>(in, bit_timing.BTR1);
  }
};
struct _BTR2
{
  using rule_t = _unsigned_integer;
  template <
      apply_mode A
    , rewind_mode M
    , template <class...> class Action
    , template <class...> class Control
    , class ParseInput>
  static bool
  match(ParseInput& in, G_BitTiming& bit_timing)
  {
    return tao::pegtl::match<rule_t, A, M, Action, Control, ParseInput>(in, bit_timing.BTR2);
  }
};
struct _bit_timing
{
  using rule_t = seq<TAO_PEGTL_STRING("BS_"), must<seq<_ss<one<':'>>, _ss<_baudrate>, _ss<one<':'>>, _ss<_BTR1>, _ss<one<','>>, _ss<_BTR2>>>>;
  template <
      apply_mode A
    , rewind_mode M
    , template <class...> class Action
    , template <class...> class Control
    , class ParseInput>
  static bool
  match(ParseInput& in, G_Network& net)
  {
    G_BitTiming bit_timing;
    auto result = tao::pegtl::match<rule_t, A, M, Action, Control, ParseInput>(in, bit_timing);
    if (result)
    {
      net.bit_timing = bit_timing;
    }
    return result;
  }
};
struct _node_name : _C_identifier {};
struct _node
{
  using rule_t = _node_name;
  template <
      apply_mode A
    , rewind_mode M
    , template <class...> class Action
    , template <class...> class Control
    , class ParseInput>
  static bool
  match(ParseInput& in, std::vector<G_Node>& nodes)
  {
    auto cur = in.current();
    G_Node node;
    auto result = tao::pegtl::match<rule_t, A, M, Action, Control, ParseInput>(in, node.name);
    if (result)
    {
      nodes.emplace_back(std::move(node));
    }
    return result;
  }
};
struct _nodes
{
  using rule_t = seq<TAO_PEGTL_STRING("BU_"), must<seq<_ss<one<':'>>, _sb<seq<star<_sb<_node>>, _sb<eolf>>>>>>;
  template <
      apply_mode A
    , rewind_mode M
    , template <class...> class Action
    , template <class...> class Control
    , class ParseInput>
  static bool
  match(ParseInput& in, G_Network& net)
  {
    return tao::pegtl::match<rule_t, A, M, Action, Control, ParseInput>(in, net.nodes);
  }
};
struct _value_table_name : _C_identifier {};
struct _value_encoding_description
{

};
struct _value_encoding_descriptions
{

};
struct _value_table
{
  //using rule_t = seq<TAO_PEGTL_STRING("VAL_TABLE_"), _ss<_value_table_name>, _ss<_value_ecoding_descriptions>, _ss<one<';'>>;
};
struct _network
  : seq<
      _ss<_version>
    , _ss<_new_symbols>
    , _ss<_bit_timing>
    , _ss<_nodes>
  >
{};
template <class R>
struct _action : nothing<R> {};
template <>
struct _action<_unsigned_integer>
{
  template <class ActionInput>
  static void
  apply(const ActionInput& in, uint64_t& result)
  {
    auto sv = in.string_view();
    const char* begin = &*sv.begin();
    const char* end = begin + sv.size();
    if (sv.size() >= 2 && begin[0] == '0' && (begin[1] == 'x' || begin[1] == 'X'))
    {
      std::from_chars(begin + 2, end, result, 16);
    }
    else
    {
      std::from_chars(begin, end, result);
    }
  }
};
template <class T, class ActionInput, class Tuple, std::size_t... I>
auto call_apply_helper(const ActionInput& in, Tuple&& t, std::index_sequence<I...>)
{
  (T::applyer(in, std::get<I>(t).value), ...);
}
template <class T, class ActionInput, class Tuple, std::size_t... I>
auto call_apply(const ActionInput& in, Tuple&& t)
{
  call_apply_helper<T>(in, t, std::make_index_sequence<std::tuple_size_v<Tuple>>());
}
template <>
struct _action<_char_string>
{
  //template <class ActionInput>
  //static void
  //apply(const ActionInput& in, std::string& result)
  //{
  //  result = in.string();
  //}
  template <class ActionInput, class... Args>
  static void
  apply(const ActionInput& in, Args&&... args)
  {
    auto t = get_tagged<_char_string_tag>(args...);
    call_apply<_action<_char_string>>(in, t);
  }
  template <class ActionInput, class T>
  static void
  applyer(const ActionInput& in, T& result)
  {
    result = in.string();
  }
};
template <>
struct _action<_C_identifier>
{
  template <class ActionInput>
  static void
  apply(const ActionInput& in, std::string& result)
  {
    result = in.string();
  }
  template <class ActionInput, class... Args>
  static void
  apply(const ActionInput& in, Args&&... args) {}
};

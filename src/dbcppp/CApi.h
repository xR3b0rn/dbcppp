
#pragma once

extern "C"
{
	struct dbcppp_Network;
	struct dbcppp_Node;
	struct dbcppp_ValueTable;
	struct dbcppp_Message;
	struct dbcppp_Transmitter;
	struct dbcppp_Signal;
	struct dbcppp_Attribute;

	struct dbcppp_Network* dbcppp_create_network();

	void dbcppp_free_network(struct dbcppp_Network* net);
	void dbcppp_parse_dbc_from_file(struct dbcppp_Network* net, const char* filename);
	void dbcppp_parse_dbc_from_memory(struct dbcppp_Network* net, const char* dbc);

	const char* dbcppp_network_get_version(struct dbcppp_Network* net);
	int dbcppp_network_get_num_new_symbols(struct dbcppp_Network* net);
	int dbcppp_network_get_new_symbol(struct dbcppp_Network* net, int index);
	struct dbcppp_BitTiming* dbcppp_network_get_bit_timing(struct dbcppp_Network* net);
	int dbcppp_network_get_num_nodes(struct dbcppp_Network* net);
	struct dbcppp_Node* dbcppp_network_get_node_by_index(struct dbcppp_Network* net, int index);
	struct dbcppp_Node* dbcppp_network_get_node_by_name(struct dbcppp_Network* net, const char* name);

	int dbcppp_network_get_num_value_tables(struct dbcppp_Network* net);
	struct dbcppp_ValueTable* dbcppp_network_get_value_table_by_index(struct dbcppp_Network* net, int index);
	struct dbcppp_ValueTable* dbcppp_network_get_value_table_by_name(struct dbcppp_Network* net, const char* name);

	int dbcppp_network_get_num_messages(struct dbcppp_Network* net);
	struct dbcppp_Message* dbcppp_network_get_message_by_index(struct dbcppp_Network* net, int index);
	struct dbcppp_Message* dbcppp_network_get_message_by_id(struct dbcppp_Network* net, int id);

	
	unsigned long int dbcppp_message_get_id(struct dbcppp_Message* msg);
	const char* dbcppp_message_get_name(struct dbcppp_Message* msg);
	unsigned long int dbcppp_message_get_message_size(struct dbcppp_Message* msg);
	struct dbcppp_Transmitter* dbcppp_message_get_transmitter(struct dbcppp_Message* msg);
	int dbcppp_message_get_num_transmitters(struct dbcppp_Message* msg);
	struct dbcppp_Transmitter* dbcppp_message_get_transmitters_by_idnex(struct dbcppp_Message* msg, int index);
	bool dbcppp_message_transmitters_contains(struct dbcppp_Message* msg, dbcppp_Transmitter* transmitter);
	int dbcppp_message_get_num_signals(struct dbcppp_Message* msg);
	struct dbcppp_Signal* dbcppp_message_get_signal_by_index(struct dbcppp_Message* msg, int index);
	struct dbcppp_Signal* dbcppp_message_get_signal_by_name(struct dbcppp_Message* msg, const char* name);
	int dbcppp_message_get_num_attribute_values(struct dbcppp_Message* msg);
	struct dbcppp_Attribute* dbcppp_message_get_attribute_value_by_index(struct dbcppp_Message* msg, int index);
	struct dbcppp_Attribute* dbcppp_message_get_attribute_value_by_name(struct dbcppp_Message* msg, const char* name);
	const char* dbcppp_message_get_comment(struct dbcppp_Message* msg);


}

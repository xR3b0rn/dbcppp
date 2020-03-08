
#pragma once

extern "C"
{
#include "Export.h"
#include <stdint.h>

	struct dbcppp_Network;
	struct dbcppp_BitTiming;
	struct dbcppp_Node;
	struct dbcppp_ValueTable;
	struct dbcppp_Message;
	struct dbcppp_EnvironmentVariable;
	struct dbcppp_AttributeDefinition;
	struct dbcppp_Attribute;

	DBCPPP_API struct dbcppp_Network* dbcppp_LoadFromFile(const char* filename);
	DBCPPP_API void dbcppp_FreeNetwork(struct dbcppp_Network* net);

	DBCPPP_API const char* dbcppp_NetworkGetVersion(struct dbcppp_Network* net);
	DBCPPP_API bool dbcppp_NetworkHasNewSymbol(struct dbcppp_Network* net, const char* new_symbol);
	DBCPPP_API void dbcppp_NetworkForEachNewSymbol(struct dbcppp_Network* net, void(*cb)(const char*));
	DBCPPP_API struct dbcppp_BitTiming* dbcppp_NetworkGetBitTiming(struct dbcppp_Network* net);
	DBCPPP_API struct dbcppp_Node* dbcppp_NetworkGetNodeByName(struct dbcppp_Network* net, const char* name);
	DBCPPP_API void dbcppp_network_for_each_node(struct dbcppp_Network* net, void(*cb)(dbcppp_Node*));
	DBCPPP_API struct dbcppp_ValueTable* dbcppp_NetworkGetValueTableByName(struct dbcppp_Network* net, const char* name);
	DBCPPP_API struct dbcppp_ValueTable* dbcppp_NetworkFindValueTable(struct dbcppp_Network* net, bool(*pred)(struct dbcppp_ValueTable*));
	DBCPPP_API void dbcppp_NetworkForEachValueTable(struct dbcppp_Network* net, void(*cb)(struct dbcppp_ValueTable*));
	DBCPPP_API struct dbcppp_Message* dbcppp_NetworkGetMessageById(struct dbcppp_Network* net, uint64_t id);
	DBCPPP_API struct dbcppp_Message* dbcppp_NetworkFindMessage(struct dbcppp_Network* net, bool(*pred)(struct dbcppp_Message*));
	DBCPPP_API void dbcppp_NetworkForEachMessage(struct dbcppp_Network* net, void(*cb)(struct dbcppp_Message*));
	DBCPPP_API struct dbcppp_EnvironmentVariable* dbcppp_NetworkGetEnvironmentVariableByName(struct dbcppp_Network* net, const char* name);
	DBCPPP_API struct dbcppp_EnvironmentVariable* dbcppp_NetworkFindEnvironmentVariable(struct dbcppp_Network* net, bool(*pred)(struct dbcppp_EnvironmentVariable*));
	DBCPPP_API void dbcppp_NetworkForEachEnvironmentVariable(struct dbcppp_Network* net, void(*cb)(struct dbcppp_EnvironmentVariable*));
	DBCPPP_API struct dbcppp_AttributeDefinition* dbcppp_NetworkGetAttributeDefinitionByName(struct dbcppp_Network* net, const char* name);
	DBCPPP_API struct dbcppp_AttributeDefinition* dbcppp_NetworkFindAttributeDefinition(struct dbcppp_Network* net, bool(*pred)(struct dbcppp_AttributeDefinition*));
	DBCPPP_API void dbcppp_NetworkForEachAttributeDefinition(struct dbcppp_Network* net, void(*cb)(struct dbcppp_AttributeDefinition*));
	DBCPPP_API struct dbcppp_Attribute* dbcppp_NetworkGetAttributeDefaultByName(struct dbcppp_Network* net, const char* name);
	DBCPPP_API struct dbcppp_Attribute* dbcppp_NetworkFindAttributeDefault(struct dbcppp_Network* net, bool(*pred)(struct dbcppp_Attribute*));
	DBCPPP_API void dbcppp_NetworkForEachAttributeDefault(struct dbcppp_Network* net, void(*cb)(struct dbcppp_Attribute*));
	DBCPPP_API struct dbcppp_Attribute* dbcppp_NetworkGetAttributeValueByName(struct dbcppp_Network* net, const char* name);
	DBCPPP_API struct dbcppp_Attribute* dbcppp_NetworkFindAttributeValue(struct dbcppp_Network* net, bool(*pred)(struct dbcppp_Attribute*));
	DBCPPP_API void dbcppp_NetworkForEachAttributeValue(struct dbcppp_Network* net, void(*cb)(struct dbcppp_Attribute*));
	DBCPPP_API const char* dbcppp_NetworkGetComment(struct dbcppp_Network* net);
}

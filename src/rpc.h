

#ifndef JUST_HASH_IT_ASSIGNMENT_2_RPC_H
#define JUST_HASH_IT_ASSIGNMENT_2_RPC_H

char* rpc_call(const char* method, const char* params);
char* rpc_get_result(const char* json);
char* rpc_get_hex(const char* json);

#endif //JUST_HASH_IT_ASSIGNMENT_2_RPC_H

#include "bitcoin.h"
#include <stdio.h>
#include <stdlib.h>
#include "rpc.h"


char* btc_get_new_address_legacy() {

    char *raw = rpc_call("getnewaddress", "\"\",\"legacy\"");
    char *result = rpc_get_result(raw);

    free(raw);
    return result;
}

char* btc_send_to_address(const char* address, double amount) {

    char params[256];
    sprintf(params, "\"%s\", %f", address, amount);

    char *raw = rpc_call("sendtoaddress", params);
    char *result = rpc_get_result(raw);

    free(raw);
    return result;
}

char* btc_list_unspent() {

    char *raw = rpc_call("listunspent", "");
    char *result = rpc_get_result(raw);

    free(raw);
    return result;
}

char* btc_decode_raw_transaction(const char* txhex) {

    char params[512];
    sprintf(params, "\"%s\"", txhex);

    char *raw = rpc_call("decoderawtransaction", params);
    char *result = rpc_get_result(raw);

    free(raw);
    return result;
}

char* btc_create_raw_tx(const char* txid, int vout, const char* address, double amount) {

    char params[512];

    sprintf(params,
        "[{\"txid\":\"%s\",\"vout\":%d}],{\"%s\":%f}",
        txid, vout, address, amount);

    char *raw = rpc_call("createrawtransaction", params);
    char *result = rpc_get_result(raw);

    free(raw);
    return result;
}

char* btc_fund_tx(const char* rawtx) {

    char params[1024];
    sprintf(params, "\"%s\"", rawtx);

    char *raw = rpc_call("fundrawtransaction", params);
    char *result = rpc_get_hex(raw);

    free(raw);
    return result;
}

char* btc_sign_tx(const char* rawtx) {

    char params[1024];
    sprintf(params, "\"%s\"", rawtx);

    char *raw = rpc_call("signrawtransactionwithwallet", params);
    char *result = rpc_get_hex(raw);

    free(raw);
    return result;
}

char* btc_send_tx(const char* rawtx) {

    char params[1024];
    sprintf(params, "\"%s\"", rawtx);

    char *raw = rpc_call("sendrawtransaction", params);
    char *result = rpc_get_result(raw);
    free(raw);
    return result;
}
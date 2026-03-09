
#include "bitcoin.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rpc.h"
#include <cjson/cJSON.h>


char* btc_get_new_address_legacy() {

    char *raw = rpc_call("getnewaddress", "\"\",\"legacy\"");
    char *result = rpc_get_result(raw);

    free(raw);
    return result;
}

char* btc_get_new_address_segwit() {

    char *raw = rpc_call("getnewaddress", "\"\",\"p2sh-segwit\"");
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

void btc_generate_blocks(int n, const char* address) {

    char params[256];

    sprintf(params, "%d,\"%s\"", n, address);

    char *raw = rpc_call("generatetoaddress", params);

    free(raw);
}

UTXO btc_find_utxo(const char* address) {

    UTXO utxo;
    utxo.txid[0] = '\0';
    utxo.vout = -1;

    char *raw = rpc_call("listunspent", "");

    cJSON *root = cJSON_Parse(raw);
    if (!root) {
        free(raw);
        return utxo;
    }

    cJSON *result = cJSON_GetObjectItem(root, "result");

    int size = cJSON_GetArraySize(result);

    for (int i = 0; i < size; i++) {

        cJSON *item = cJSON_GetArrayItem(result, i);

        cJSON *addr = cJSON_GetObjectItem(item, "address");

        if (addr && strcmp(addr->valuestring, address) == 0) {

            cJSON *txid = cJSON_GetObjectItem(item, "txid");
            cJSON *vout = cJSON_GetObjectItem(item, "vout");

            strcpy(utxo.txid, txid->valuestring);
            utxo.vout = vout->valueint;

            break;
        }
    }

    cJSON_Delete(root);
    free(raw);

    return utxo;
}


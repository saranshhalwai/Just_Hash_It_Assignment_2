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

void btc_print_scripts(const char *txid)
{
    char params[256];
    sprintf(params, "\"%s\", true", txid);

    char *raw = rpc_call("getrawtransaction", params);

    cJSON *root = cJSON_Parse(raw);
    cJSON *tx = root ? cJSON_GetObjectItem(root, "result") : NULL;
    if (!tx) {
        printf("Failed to parse transaction JSON\n");
        free(raw);
        cJSON_Delete(root);
        return;
    }

    cJSON *vin = cJSON_GetObjectItem(tx, "vin");
    if (!vin || !cJSON_IsArray(vin)) {
        printf("Could not read inputs\n");
        cJSON_Delete(root);
        free(raw);
        return;
    }

    cJSON *input = cJSON_GetArrayItem(vin, 0);

    printf("\n--- Response Script (scriptSig / witness) ---\n");

    /* Legacy scriptSig */

    cJSON *scriptSig = cJSON_GetObjectItem(input, "scriptSig");
    if (scriptSig) {
        cJSON *asm_field = cJSON_GetObjectItem(scriptSig, "asm");
        if (asm_field && strlen(asm_field->valuestring) > 0)
            printf("%s\n", asm_field->valuestring);
    }

    /* SegWit witness */

    cJSON *witness = cJSON_GetObjectItem(input, "txinwitness");
    if (witness && cJSON_IsArray(witness)) {
        printf("Witness:\n");
        int n = cJSON_GetArraySize(witness);

        for (int i = 0; i < n; i++) {
            cJSON *item = cJSON_GetArrayItem(witness, i);
            if (item)
                printf("%s\n", item->valuestring);
        }
    }

    printf("\n--- Challenge Script (scriptPubKey) ---\n");

    /* Get previous tx info */

    cJSON *prev_txid = cJSON_GetObjectItem(input, "txid");
    cJSON *prev_vout = cJSON_GetObjectItem(input, "vout");

    if (!prev_txid || !prev_vout) {
        printf("Could not locate previous output\n");
        goto cleanup;
    }

    char prev_params[256];
    sprintf(prev_params, "\"%s\", true", prev_txid->valuestring);

    char *prev_raw = rpc_call("getrawtransaction", prev_params);

    cJSON *prev_root = cJSON_Parse(prev_raw);
    cJSON *prev_tx = prev_root ? cJSON_GetObjectItem(prev_root, "result") : NULL;

    if (!prev_tx) {
        printf("Failed to parse previous transaction\n");
        free(prev_raw);
        cJSON_Delete(prev_root);
        goto cleanup;
    }

    cJSON *vouts = cJSON_GetObjectItem(prev_tx, "vout");
    if (vouts && cJSON_IsArray(vouts)) {

        cJSON *spent = cJSON_GetArrayItem(vouts, prev_vout->valueint);

        if (spent) {
            cJSON *script = cJSON_GetObjectItem(spent, "scriptPubKey");
            cJSON *asm_field = cJSON_GetObjectItem(script, "asm");

            if (asm_field)
                printf("%s\n", asm_field->valuestring);
        }
    }

    cJSON_Delete(prev_root);
    free(prev_raw);

cleanup:
    cJSON_Delete(root);
    free(raw);
}

void btc_print_tx_sizes(const char *txid) {
    char params[256];
    sprintf(params, "\"%s\", true", txid);

    char *raw = rpc_call("getrawtransaction", params);
    cJSON *root = cJSON_Parse(raw);
    cJSON *tx = root ? cJSON_GetObjectItem(root, "result") : NULL;

    if (!tx) {
        printf("Failed to get tx sizes\n");
        free(raw);
        cJSON_Delete(root);
        return;
    }

    cJSON *size   = cJSON_GetObjectItem(tx, "size");
    cJSON *vsize  = cJSON_GetObjectItem(tx, "vsize");
    cJSON *weight = cJSON_GetObjectItem(tx, "weight");

    printf("  Size:   %d bytes\n",  size   ? size->valueint   : -1);
    printf("  VSize:  %d vbytes\n", vsize  ? vsize->valueint  : -1);
    printf("  Weight: %d WU\n",     weight ? weight->valueint : -1);

    cJSON_Delete(root);
    free(raw);
}
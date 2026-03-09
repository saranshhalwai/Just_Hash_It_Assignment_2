#ifndef JUST_HASH_IT_ASSIGNMENT_2_BITCOIN_H
#define JUST_HASH_IT_ASSIGNMENT_2_BITCOIN_H

typedef struct {
    char txid[128];
    int vout;
} UTXO;

char* btc_get_new_address_legacy();
char* btc_get_new_address_segwit();

char* btc_send_to_address(const char* address, double amount);

char* btc_list_unspent();

char* btc_decode_raw_transaction(const char* txhex);

char* btc_create_raw_tx(const char* txid, int vout, const char* address, double amount);
char* btc_fund_tx(const char* rawtx);
char* btc_sign_tx(const char* rawtx);
char* btc_send_tx(const char* rawtx);

void btc_generate_blocks(int n, const char* address);

UTXO btc_find_utxo(const char* address);

#endif
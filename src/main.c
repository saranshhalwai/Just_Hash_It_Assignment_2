#include <stdio.h>
#include <stdlib.h>
#include "bitcoin.h"

int main() {

    printf("===== LEGACY TEST =====\n");

    char *A = btc_get_new_address_legacy();
    char *B = btc_get_new_address_legacy();
    char *C = btc_get_new_address_legacy();

    printf("A: %s\n", A);
    printf("B: %s\n", B);
    printf("C: %s\n", C);

    /* Fund wallet so A has coins */

    printf("Mining blocks to fund wallet...\n");
    btc_generate_blocks(101, A);

    /* A → B */

    char *txid1 = btc_send_to_address(B, 1.0);
    printf("A → B txid: %s\n", txid1);

    /* Confirm transaction */

    btc_generate_blocks(1, A);

    /* Find UTXO for B */

    UTXO utxo = btc_find_utxo(B);

    printf("UTXO for B:\n");
    printf("txid: %s\n", utxo.txid);
    printf("vout: %d\n", utxo.vout);

    /* B → C raw transaction */

    char *raw = btc_create_raw_tx(utxo.txid, utxo.vout, C, 0.9);
    printf("Raw tx: %s\n", raw);

    char *funded = btc_fund_tx(raw);
    printf("Funded tx: %s\n", funded);

    char *signed_tx = btc_sign_tx(funded);
    printf("Signed tx: %s\n", signed_tx);

    char *txid2 = btc_send_tx(signed_tx);
    printf("B → C txid: %s\n", txid2);

    btc_generate_blocks(1, A);

    printf("\nScripts for transaction:\n");
    btc_print_scripts(txid2);

    printf("\nLegacy B→C Transaction Sizes:\n");
    btc_print_tx_sizes(txid2);

    free(A);
    free(B);
    free(C);
    free(txid1);
    free(raw);
    free(funded);
    free(signed_tx);
    free(txid2);

    printf("\n===== SEGWIT TEST =====\n");

    char *A2 = btc_get_new_address_segwit();
    char *B2 = btc_get_new_address_segwit();
    char *C2 = btc_get_new_address_segwit();

    printf("A': %s\n", A2);
    printf("B': %s\n", B2);
    printf("C': %s\n", C2);

    btc_generate_blocks(101, A2);

    /* A' → B' */

    char *txid3 = btc_send_to_address(B2, 1.0);
    printf("A' → B' txid: %s\n", txid3);

    btc_generate_blocks(1, A2);

    /* Find UTXO for B' */

    UTXO utxo2 = btc_find_utxo(B2);

    printf("UTXO for B':\n");
    printf("txid: %s\n", utxo2.txid);
    printf("vout: %d\n", utxo2.vout);

    /* B' → C' */

    char *raw2 = btc_create_raw_tx(utxo2.txid, utxo2.vout, C2, 0.9);
    printf("Raw tx: %s\n", raw2);

    char *funded2 = btc_fund_tx(raw2);
    printf("Funded tx: %s\n", funded2);

    char *signed_tx2 = btc_sign_tx(funded2);
    printf("Signed tx: %s\n", signed_tx2);

    char *txid4 = btc_send_tx(signed_tx2);
    printf("B' → C' txid: %s\n", txid4);

    btc_generate_blocks(1, A2);          // ← mine FIRST

    printf("\nScripts for transaction:\n");
    btc_print_scripts(txid4);            // ← now confirmed, readable

    printf("\nSegWit B'→C' Transaction Sizes:\n");
    btc_print_tx_sizes(txid4);

    free(A2);
    free(B2);
    free(C2);
    free(txid3);
    free(raw2);
    free(funded2);
    free(signed_tx2);
    free(txid4);

    return 0;
}
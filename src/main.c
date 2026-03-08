#include <stdio.h>
#include <stdlib.h>
#include "bitcoin.h"

int main() {

    char *C = btc_get_new_address_legacy();

    printf("Address C: %s\n", C);

    char *raw = btc_create_raw_tx(
        "476a7d0147df361984332ca4e6e30038891860a335b0b4d9115cab2b6e7e43dd",
        1,
        C,
        0.9
    );

    printf("Raw tx: %s\n", raw);

    char *funded = btc_fund_tx(raw);
    printf("Funded: %s\n", funded);

    char *signed_tx = btc_sign_tx(funded);
    printf("Signed: %s\n", signed_tx);

    char *txid = btc_send_tx(signed_tx);
    printf("Broadcasted txid: %s\n", txid);

    free(C);
    free(raw);
    free(funded);
    free(signed_tx);
    free(txid);

    return 0;
}
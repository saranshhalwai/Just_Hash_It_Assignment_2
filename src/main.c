#include <stdio.h>
#include <stdlib.h>
#include "rpc.h"

int main() {

    char *raw = rpc_call("getblockchaininfo", "");
    char *result = rpc_get_result(raw);

    printf("Result:\n%s\n", result);

    free(raw);
    free(result);

    return 0;
}
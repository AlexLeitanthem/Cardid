#include <stdio.h>
#include <string.h>
#include "cardid.h"

static const char* network_name(cardid_network n) {
    switch (n) {
        case CARD_VISA: return "VISA";
        case CARD_MASTERCARD: return "MASTERCARD";
        case CARD_AMEX: return "AMEX";
        case CARD_DISCOVER: return "DISCOVER";
        default: return "UNKNOWN";
    }
}

int main(int argc, char** argv) {
    char input[256] = {0};
    if (argc >= 2) {
        // Concatenate all args with spaces to allow dashed/spaced inputs
        size_t pos = 0;
        for (int i = 1; i < argc; ++i) {
            size_t len = strlen(argv[i]);
            if (pos + len + 1 >= sizeof(input)) break;
            memcpy(input + pos, argv[i], len);
            pos += len;
            if (i + 1 < argc && pos + 1 < sizeof(input)) input[pos++] = ' ';
        }
        input[pos] = '\0';
    } else {
        fputs("Number: ", stdout);
        if (!fgets(input, sizeof input, stdin)) return 1;
    }

    cardid_result res;
    cardid_extract_result meta;
    cardid_analyze(input, &res, &meta);

    if (meta.overflowed || res.length == 0 || !res.luhn_valid) {
        puts("INVALID");
        return 0;
    }

    const char* name = network_name(res.network);
    if (res.network == CARD_UNKNOWN) {
        puts("INVALID");
    } else {
        puts(name);
    }
    return 0;
}


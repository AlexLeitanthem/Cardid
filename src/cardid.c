#include "cardid.h"
#include <ctype.h>
#include <string.h>

bool cardid_luhn_digits(const char* d, int n) {
    int sum = 0, pos = 0;
    for (int i = n - 1; i >= 0; --i, ++pos) {
        int v = d[i] - '0';
        if ((unsigned)v > 9) return false;
        if (pos & 1) { int x = v * 2; sum += x > 9 ? x - 9 : x; }
        else sum += v;
    }
    return (sum % 10) == 0;
}

static int prefix_n(const char* s, int n, int total_len) {
    if (total_len < n) return -1;
    int v = 0;
    for (int i = 0; i < n; ++i) v = v * 10 + (s[i] - '0');
    return v;
}

static int len_in(int len, const int* lens, int count) {
    for (int i = 0; i < count; ++i) if (lens[i] == len) return 1;
    return 0;
}

cardid_extract_result cardid_extract_digits(const char* input, char* out_digits, int out_capacity) {
    int idx = 0;
    bool found_non_digit = false, overflowed = false;
    
    // Security: Validate input parameters
    if (!input || !out_digits || out_capacity <= 0) {
        cardid_extract_result r = {0, true, true};
        return r;
    }
    
    // Security: Initialize output buffer to prevent information leakage
    memset(out_digits, 0, out_capacity);
    
    for (int i = 0; input[i] != '\0'; ++i) {
        unsigned char c = (unsigned char)input[i];
        
        // Security: Prevent buffer overflow
        if (idx >= out_capacity - 1) {
            overflowed = true;
            break;
        }
        
        if (isdigit(c)) {
            out_digits[idx++] = (char)c;
        } else if (!isspace(c) && c != '-') {
            found_non_digit = true;
        }
    }
    out_digits[idx] = '\0';
    
    cardid_extract_result r = { idx, found_non_digit, overflowed };
    return r;
}

cardid_network cardid_detect_network(const char* s, int len) {
    // American Express: 34/37, length 15
    if (len == 15) {
        int p2 = prefix_n(s, 2, len);
        if (p2 == 34 || p2 == 37) return CARD_AMEX;
    }

    // Visa: prefix 4, lengths 13,16,19
    if (len == 13 || len == 16 || len == 19) {
        int p1 = prefix_n(s, 1, len);
        if (p1 == 4) return CARD_VISA;
    }

    // Mastercard: 51–55 or 2221–2720; length 16
    if (len == 16) {
        int p2 = prefix_n(s, 2, len);
        if (p2 >= 51 && p2 <= 55) return CARD_MASTERCARD;
        int p4 = prefix_n(s, 4, len);
        if (p4 >= 2221 && p4 <= 2720) return CARD_MASTERCARD;
    }

    // Discover: 6011, 622126–622925, 644–649, 65; lengths 16 or 19
    if (len == 16 || len == 19) {
        int p2 = prefix_n(s, 2, len);
        int p3 = prefix_n(s, 3, len);
        int p4 = prefix_n(s, 4, len);
        int p6 = prefix_n(s, 6, len);
        if (p4 == 6011) return CARD_DISCOVER;
        if (p2 == 65) return CARD_DISCOVER;
        if (p3 >= 644 && p3 <= 649) return CARD_DISCOVER;
        if (p6 >= 622126 && p6 <= 622925) return CARD_DISCOVER;
    }

    return CARD_UNKNOWN;
}

void cardid_analyze(const char* input, cardid_result* out, cardid_extract_result* extract_meta) {
    // Security: Validate input parameters
    if (!input || !out) {
        if (out) {
            out->length = 0;
            out->luhn_valid = false;
            out->network = CARD_UNKNOWN;
        }
        return;
    }
    
    char digits[ CARDID_MAX_DIGITS + 1 ];
    cardid_extract_result r = cardid_extract_digits(input, digits, (int)sizeof(digits));
    if (extract_meta) *extract_meta = r;

    out->length = r.digit_count;
    if (r.digit_count == 0 || r.overflowed) {
        out->luhn_valid = false;
        out->network = CARD_UNKNOWN;
        return;
    }

    // Security: Validate length bounds
    if (r.digit_count < 13 || r.digit_count > 19) {
        out->luhn_valid = false;
        out->network = CARD_UNKNOWN;
        return;
    }

    // Quick length filter: allow only common lengths 13,15,16,19
    static const int allowed[] = {13, 15, 16, 19};
    if (!len_in(r.digit_count, allowed, 4)) {
        out->luhn_valid = false;
        out->network = CARD_UNKNOWN;
        return;
    }

    out->luhn_valid = cardid_luhn_digits(digits, r.digit_count);
    out->network = out->luhn_valid ? cardid_detect_network(digits, r.digit_count) : CARD_UNKNOWN;
}

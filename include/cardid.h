#pragma once
#include <stdbool.h>

typedef enum {
  CARD_UNKNOWN = 0,
  CARD_VISA,
  CARD_MASTERCARD,
  CARD_AMEX,
  CARD_DISCOVER,
} cardid_network;

typedef struct {
  cardid_network network;
  bool luhn_valid;
  int length;
} cardid_result;

bool cardid_luhn_digits(const char* digits, int len);

// Maximum number of PAN digits supported by this library (ISO allows up to 19)
#define CARDID_MAX_DIGITS 19

typedef struct {
  int digit_count;
  bool found_non_digit;
  bool overflowed;
} cardid_extract_result;

// Extract only digits from an arbitrary input string (spaces/dashes allowed).
// Writes up to out_capacity-1 digits and NUL-terminates. Returns extraction metadata.
cardid_extract_result cardid_extract_digits(const char* input,
                                            char* out_digits,
                                            int out_capacity);

// Detect network from a clean digits string and its length.
cardid_network cardid_detect_network(const char* digits, int len);

// High-level analysis from raw input; optionally returns extraction metadata.
void cardid_analyze(const char* input,
                    cardid_result* out,
                    cardid_extract_result* extract_meta);

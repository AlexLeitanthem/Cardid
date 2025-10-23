#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../include/cardid.h"

#define TEST_ASSERT(condition, message) \
    do { \
        if (!(condition)) { \
            printf("FAIL: %s:%d - %s\n", __FILE__, __LINE__, message); \
            return 1; \
        } \
    } while(0)

#define TEST_PASS(message) \
    printf("PASS: %s\n", message)

// Test data structure
typedef struct {
    const char* input;
    const char* expected_output;
    bool should_be_valid;
} test_case_t;

// Valid test cases
static const test_case_t valid_cases[] = {
    // Visa
    {"4111111111111111", "VISA", true},
    {"4111-1111-1111-1111", "VISA", true},
    {"4111 1111 1111 1111", "VISA", true},
    {"4532015112830366", "VISA", true},
    {"4532-0151-1283-0366", "VISA", true},
    
    // Mastercard
    {"5555555555554444", "MASTERCARD", true},
    {"5555-5555-5555-4444", "MASTERCARD", true},
    {"2223003122003222", "MASTERCARD", true},
    {"2223-0031-2200-3222", "MASTERCARD", true},
    {"5105105105105100", "MASTERCARD", true},
    
    // American Express
    {"378282246310005", "AMEX", true},
    {"3782-822463-10005", "AMEX", true},
    {"371449635398431", "AMEX", true},
    {"3714-496353-98431", "AMEX", true},
    
    // Discover
    {"6011111111111117", "DISCOVER", true},
    {"6011-1111-1111-1117", "DISCOVER", true},
    {"6011000990139424", "DISCOVER", true},
    {"6221260000000000", "DISCOVER", true},
    {"6445644564456445", "DISCOVER", true},
    {"6500000000000000", "DISCOVER", true},
};

// Invalid test cases
static const test_case_t invalid_cases[] = {
    {"1234567890123456", "INVALID", false},
    {"0000000000000000", "INVALID", false},
    {"4111111111111112", "INVALID", false}, // Wrong checksum
    {"1234", "INVALID", false}, // Too short
    {"12345678901234567890", "INVALID", false}, // Too long
    {"abcd1234567890", "INVALID", false}, // Non-numeric
    {"", "INVALID", false}, // Empty
    {"1234-5678-9012-3456-7890", "INVALID", false}, // Too long with separators
};

static int test_luhn_validation() {
    printf("\n=== Testing Luhn Validation ===\n");
    
    // Test valid Luhn numbers
    const char* valid_luhn[] = {
        "4111111111111111",
        "5555555555554444",
        "378282246310005",
        "6011111111111117"
    };
    
    for (int i = 0; i < 4; i++) {
        int len = (int)strlen(valid_luhn[i]);
        bool result = cardid_luhn_digits(valid_luhn[i], len);
        TEST_ASSERT(result, "Valid Luhn number should pass validation");
    }
    
    // Test invalid Luhn numbers
    const char* invalid_luhn[] = {
        "4111111111111112",
        "5555555555554445",
        "378282246310006",
        "6011111111111118"
    };
    
    for (int i = 0; i < 4; i++) {
        int len = (int)strlen(invalid_luhn[i]);
        bool result = cardid_luhn_digits(invalid_luhn[i], len);
        TEST_ASSERT(!result, "Invalid Luhn number should fail validation");
    }
    
    TEST_PASS("Luhn validation tests");
    return 0;
}

static int test_digit_extraction() {
    printf("\n=== Testing Digit Extraction ===\n");
    
    char output[32];
    cardid_extract_result result;
    
    // Test normal extraction
    result = cardid_extract_digits("4111-1111-1111-1111", output, sizeof(output));
    TEST_ASSERT(result.digit_count == 16, "Should extract 16 digits");
    TEST_ASSERT(strcmp(output, "4111111111111111") == 0, "Should extract correct digits");
    TEST_ASSERT(!result.overflowed, "Should not overflow");
    
    // Test with spaces
    result = cardid_extract_digits("4111 1111 1111 1111", output, sizeof(output));
    TEST_ASSERT(result.digit_count == 16, "Should extract 16 digits with spaces");
    TEST_ASSERT(strcmp(output, "4111111111111111") == 0, "Should extract correct digits with spaces");
    
    // Test overflow
    result = cardid_extract_digits("123456789012345678901234567890", output, 10);
    TEST_ASSERT(result.overflowed, "Should detect overflow");
    
    // Test empty input
    result = cardid_extract_digits("", output, sizeof(output));
    TEST_ASSERT(result.digit_count == 0, "Should handle empty input");
    
    TEST_PASS("Digit extraction tests");
    return 0;
}

static int test_network_detection() {
    printf("\n=== Testing Network Detection ===\n");
    
    // Test Visa
    cardid_network network = cardid_detect_network("4111111111111111", 16);
    TEST_ASSERT(network == CARD_VISA, "Should detect Visa");
    
    // Test Mastercard
    network = cardid_detect_network("5555555555554444", 16);
    TEST_ASSERT(network == CARD_MASTERCARD, "Should detect Mastercard");
    
    // Test Amex
    network = cardid_detect_network("378282246310005", 15);
    TEST_ASSERT(network == CARD_AMEX, "Should detect Amex");
    
    // Test Discover
    network = cardid_detect_network("6011111111111117", 16);
    TEST_ASSERT(network == CARD_DISCOVER, "Should detect Discover");
    
    // Test unknown
    network = cardid_detect_network("1234567890123456", 16);
    TEST_ASSERT(network == CARD_UNKNOWN, "Should detect unknown network");
    
    TEST_PASS("Network detection tests");
    return 0;
}

static int test_full_analysis() {
    printf("\n=== Testing Full Analysis ===\n");
    
    // Test valid cases
    for (int i = 0; i < sizeof(valid_cases)/sizeof(valid_cases[0]); i++) {
        cardid_result result;
        cardid_extract_result meta;
        cardid_analyze(valid_cases[i].input, &result, &meta);
        
        TEST_ASSERT(result.luhn_valid == valid_cases[i].should_be_valid, 
                   "Luhn validation should match expected");
        TEST_ASSERT(result.length > 0, "Should have valid length");
        TEST_ASSERT(!meta.overflowed, "Should not overflow");
    }
    
    // Test invalid cases
    for (int i = 0; i < sizeof(invalid_cases)/sizeof(invalid_cases[0]); i++) {
        cardid_result result;
        cardid_extract_result meta;
        cardid_analyze(invalid_cases[i].input, &result, &meta);
        
        TEST_ASSERT(!result.luhn_valid || result.network == CARD_UNKNOWN, 
                   "Invalid cases should fail validation or be unknown");
    }
    
    TEST_PASS("Full analysis tests");
    return 0;
}

static int test_edge_cases() {
    printf("\n=== Testing Edge Cases ===\n");
    
    // Test NULL input
    char output[32];
    cardid_extract_result result = cardid_extract_digits(NULL, output, sizeof(output));
    TEST_ASSERT(result.digit_count == 0, "Should handle NULL input");
    
    // Test zero capacity
    result = cardid_extract_digits("1234567890", output, 0);
    TEST_ASSERT(result.overflowed, "Should detect overflow with zero capacity");
    
    // Test very long input
    char long_input[1000];
    memset(long_input, '1', 999);
    long_input[999] = '\0';
    result = cardid_extract_digits(long_input, output, sizeof(output));
    TEST_ASSERT(result.overflowed, "Should detect overflow with very long input");
    
    TEST_PASS("Edge case tests");
    return 0;
}

int main() {
    printf("Starting CardID Test Suite\n");
    printf("==========================\n");
    
    int failures = 0;
    failures += test_luhn_validation();
    failures += test_digit_extraction();
    failures += test_network_detection();
    failures += test_full_analysis();
    failures += test_edge_cases();
    
    printf("\n==========================\n");
    if (failures == 0) {
        printf("All tests PASSED! ✅\n");
        return 0;
    } else {
        printf("Tests FAILED: %d failures ❌\n", failures);
        return 1;
    }
}

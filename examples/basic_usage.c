/**
 * @file basic_usage.c
 * @brief Basic usage examples for the CardID library
 * @author CardID Team
 * @date 2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/cardid.h"

/**
 * @brief Print card network name
 * @param network The card network enum value
 * @return String representation of the network
 */
static const char* network_to_string(cardid_network network) {
    switch (network) {
        case CARD_VISA: return "Visa";
        case CARD_MASTERCARD: return "Mastercard";
        case CARD_AMEX: return "American Express";
        case CARD_DISCOVER: return "Discover";
        default: return "Unknown";
    }
}

/**
 * @brief Example: Basic card validation
 */
void example_basic_validation() {
    printf("=== Basic Card Validation ===\n");
    
    const char* test_cards[] = {
        "4111111111111111",  // Valid Visa
        "5555555555554444",  // Valid Mastercard
        "378282246310005",   // Valid Amex
        "6011111111111117",  // Valid Discover
        "1234567890123456",  // Invalid
        NULL
    };
    
    for (int i = 0; test_cards[i] != NULL; i++) {
        cardid_result result;
        cardid_analyze(test_cards[i], &result, NULL);
        
        printf("Card: %s\n", test_cards[i]);
        printf("  Valid: %s\n", result.luhn_valid ? "Yes" : "No");
        printf("  Network: %s\n", network_to_string(result.network));
        printf("  Length: %d\n", result.length);
        printf("\n");
    }
}

/**
 * @brief Example: Input with separators
 */
void example_with_separators() {
    printf("=== Cards with Separators ===\n");
    
    const char* test_cards[] = {
        "4111-1111-1111-1111",
        "5555 5555 5555 4444",
        "3782-822463-10005",
        "6011-1111-1111-1117",
        NULL
    };
    
    for (int i = 0; test_cards[i] != NULL; i++) {
        cardid_result result;
        cardid_extract_result meta;
        cardid_analyze(test_cards[i], &result, &meta);
        
        printf("Input: %s\n", test_cards[i]);
        printf("  Extracted digits: %d\n", meta.digit_count);
        printf("  Non-digit chars: %s\n", meta.found_non_digit ? "Yes" : "No");
        printf("  Valid: %s\n", result.luhn_valid ? "Yes" : "No");
        printf("  Network: %s\n", network_to_string(result.network));
        printf("\n");
    }
}

/**
 * @brief Example: Manual digit extraction
 */
void example_manual_extraction() {
    printf("=== Manual Digit Extraction ===\n");
    
    const char* input = "4111-1111-1111-1111";
    char digits[32];
    cardid_extract_result result;
    
    result = cardid_extract_digits(input, digits, sizeof(digits));
    
    printf("Input: %s\n", input);
    printf("Extracted: %s\n", digits);
    printf("Digit count: %d\n", result.digit_count);
    printf("Overflowed: %s\n", result.overflowed ? "Yes" : "No");
    printf("Found non-digits: %s\n", result.found_non_digit ? "Yes" : "No");
    printf("\n");
}

/**
 * @brief Example: Network detection only
 */
void example_network_detection() {
    printf("=== Network Detection ===\n");
    
    const char* test_cards[] = {
        "4111111111111111",  // Visa
        "5555555555554444",  // Mastercard
        "378282246310005",   // Amex
        "6011111111111117",  // Discover
        "1234567890123456",  // Unknown
        NULL
    };
    
    for (int i = 0; test_cards[i] != NULL; i++) {
        int len = (int)strlen(test_cards[i]);
        cardid_network network = cardid_detect_network(test_cards[i], len);
        
        printf("Card: %s\n", test_cards[i]);
        printf("Network: %s\n", network_to_string(network));
        printf("\n");
    }
}

/**
 * @brief Example: Luhn validation only
 */
void example_luhn_validation() {
    printf("=== Luhn Validation ===\n");
    
    const char* test_cards[] = {
        "4111111111111111",  // Valid Luhn
        "4111111111111112",  // Invalid Luhn
        "5555555555554444",  // Valid Luhn
        "5555555555554445",  // Invalid Luhn
        NULL
    };
    
    for (int i = 0; test_cards[i] != NULL; i++) {
        int len = (int)strlen(test_cards[i]);
        bool valid = cardid_luhn_digits(test_cards[i], len);
        
        printf("Card: %s\n", test_cards[i]);
        printf("Luhn valid: %s\n", valid ? "Yes" : "No");
        printf("\n");
    }
}

/**
 * @brief Example: Error handling
 */
void example_error_handling() {
    printf("=== Error Handling ===\n");
    
    // Test NULL input
    cardid_result result;
    cardid_analyze(NULL, &result, NULL);
    printf("NULL input - Valid: %s, Network: %s\n", 
           result.luhn_valid ? "Yes" : "No", 
           network_to_string(result.network));
    
    // Test empty input
    cardid_analyze("", &result, NULL);
    printf("Empty input - Valid: %s, Network: %s\n", 
           result.luhn_valid ? "Yes" : "No", 
           network_to_string(result.network));
    
    // Test too short
    cardid_analyze("1234", &result, NULL);
    printf("Too short - Valid: %s, Network: %s\n", 
           result.luhn_valid ? "Yes" : "No", 
           network_to_string(result.network));
    
    // Test too long
    char long_input[100];
    memset(long_input, '1', 99);
    long_input[99] = '\0';
    cardid_analyze(long_input, &result, NULL);
    printf("Too long - Valid: %s, Network: %s\n", 
           result.luhn_valid ? "Yes" : "No", 
           network_to_string(result.network));
    
    printf("\n");
}

/**
 * @brief Main function demonstrating all examples
 */
int main() {
    printf("CardID Library Examples\n");
    printf("=======================\n\n");
    
    example_basic_validation();
    example_with_separators();
    example_manual_extraction();
    example_network_detection();
    example_luhn_validation();
    example_error_handling();
    
    printf("All examples completed!\n");
    return 0;
}

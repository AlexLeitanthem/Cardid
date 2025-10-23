/**
 * @file benchmark_cardid.c
 * @brief Performance benchmarks for CardID library
 * @author CardID Team
 * @date 2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include "../include/cardid.h"

#define BENCHMARK_ITERATIONS 1000000
#define BENCHMARK_WARMUP 10000

// Test data for benchmarks
static const char* benchmark_cards[] = {
    "4111111111111111",  // Visa
    "5555555555554444",  // Mastercard
    "378282246310005",   // Amex
    "6011111111111117",  // Discover
    "4111-1111-1111-1111",  // Visa with separators
    "5555 5555 5555 4444",  // Mastercard with spaces
    "3782-822463-10005",     // Amex with separators
    "6011-1111-1111-1117",  // Discover with separators
    NULL
};

/**
 * @brief Get current time in microseconds
 */
static long long get_time_us() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000000LL + tv.tv_usec;
}

/**
 * @brief Benchmark Luhn validation
 */
static void benchmark_luhn() {
    printf("=== Luhn Validation Benchmark ===\n");
    
    const char* test_card = "4111111111111111";
    int len = (int)strlen(test_card);
    
    // Warmup
    for (int i = 0; i < BENCHMARK_WARMUP; i++) {
        cardid_luhn_digits(test_card, len);
    }
    
    // Benchmark
    long long start = get_time_us();
    for (int i = 0; i < BENCHMARK_ITERATIONS; i++) {
        cardid_luhn_digits(test_card, len);
    }
    long long end = get_time_us();
    
    long long total_time = end - start;
    double avg_time = (double)total_time / BENCHMARK_ITERATIONS;
    
    printf("Iterations: %d\n", BENCHMARK_ITERATIONS);
    printf("Total time: %lld μs\n", total_time);
    printf("Average time: %.3f μs per validation\n", avg_time);
    printf("Validations per second: %.0f\n", 1000000.0 / avg_time);
    printf("\n");
}

/**
 * @brief Benchmark digit extraction
 */
static void benchmark_extraction() {
    printf("=== Digit Extraction Benchmark ===\n");
    
    const char* test_input = "4111-1111-1111-1111";
    char output[32];
    
    // Warmup
    for (int i = 0; i < BENCHMARK_WARMUP; i++) {
        cardid_extract_digits(test_input, output, sizeof(output));
    }
    
    // Benchmark
    long long start = get_time_us();
    for (int i = 0; i < BENCHMARK_ITERATIONS; i++) {
        cardid_extract_digits(test_input, output, sizeof(output));
    }
    long long end = get_time_us();
    
    long long total_time = end - start;
    double avg_time = (double)total_time / BENCHMARK_ITERATIONS;
    
    printf("Iterations: %d\n", BENCHMARK_ITERATIONS);
    printf("Total time: %lld μs\n", total_time);
    printf("Average time: %.3f μs per extraction\n", avg_time);
    printf("Extractions per second: %.0f\n", 1000000.0 / avg_time);
    printf("\n");
}

/**
 * @brief Benchmark network detection
 */
static void benchmark_network_detection() {
    printf("=== Network Detection Benchmark ===\n");
    
    const char* test_card = "4111111111111111";
    int len = (int)strlen(test_card);
    
    // Warmup
    for (int i = 0; i < BENCHMARK_WARMUP; i++) {
        cardid_detect_network(test_card, len);
    }
    
    // Benchmark
    long long start = get_time_us();
    for (int i = 0; i < BENCHMARK_ITERATIONS; i++) {
        cardid_detect_network(test_card, len);
    }
    long long end = get_time_us();
    
    long long total_time = end - start;
    double avg_time = (double)total_time / BENCHMARK_ITERATIONS;
    
    printf("Iterations: %d\n", BENCHMARK_ITERATIONS);
    printf("Total time: %lld μs\n", total_time);
    printf("Average time: %.3f μs per detection\n", avg_time);
    printf("Detections per second: %.0f\n", 1000000.0 / avg_time);
    printf("\n");
}

/**
 * @brief Benchmark complete analysis
 */
static void benchmark_analysis() {
    printf("=== Complete Analysis Benchmark ===\n");
    
    const char* test_input = "4111-1111-1111-1111";
    cardid_result result;
    
    // Warmup
    for (int i = 0; i < BENCHMARK_WARMUP; i++) {
        cardid_analyze(test_input, &result, NULL);
    }
    
    // Benchmark
    long long start = get_time_us();
    for (int i = 0; i < BENCHMARK_ITERATIONS; i++) {
        cardid_analyze(test_input, &result, NULL);
    }
    long long end = get_time_us();
    
    long long total_time = end - start;
    double avg_time = (double)total_time / BENCHMARK_ITERATIONS;
    
    printf("Iterations: %d\n", BENCHMARK_ITERATIONS);
    printf("Total time: %lld μs\n", total_time);
    printf("Average time: %.3f μs per analysis\n", avg_time);
    printf("Analyses per second: %.0f\n", 1000000.0 / avg_time);
    printf("\n");
}

/**
 * @brief Benchmark different card types
 */
static void benchmark_card_types() {
    printf("=== Card Type Performance Comparison ===\n");
    
    for (int i = 0; benchmark_cards[i] != NULL; i++) {
        const char* card = benchmark_cards[i];
        cardid_result result;
        
        // Warmup
        for (int j = 0; j < 1000; j++) {
            cardid_analyze(card, &result, NULL);
        }
        
        // Benchmark
        long long start = get_time_us();
        for (int j = 0; j < 100000; j++) {
            cardid_analyze(card, &result, NULL);
        }
        long long end = get_time_us();
        
        long long total_time = end - start;
        double avg_time = (double)total_time / 100000;
        
        printf("Card: %s\n", card);
        printf("  Average time: %.3f μs\n", avg_time);
        printf("  Network: %s\n", 
               result.network == CARD_VISA ? "Visa" :
               result.network == CARD_MASTERCARD ? "Mastercard" :
               result.network == CARD_AMEX ? "Amex" :
               result.network == CARD_DISCOVER ? "Discover" : "Unknown");
        printf("  Valid: %s\n", result.luhn_valid ? "Yes" : "No");
        printf("\n");
    }
}

/**
 * @brief Memory usage benchmark
 */
static void benchmark_memory() {
    printf("=== Memory Usage Benchmark ===\n");
    
    // Test stack usage
    char digits[32];
    cardid_result result;
    cardid_extract_result meta;
    
    printf("Stack usage test:\n");
    printf("  cardid_result: %zu bytes\n", sizeof(cardid_result));
    printf("  cardid_extract_result: %zu bytes\n", sizeof(cardid_extract_result));
    printf("  digits buffer: %zu bytes\n", sizeof(digits));
    printf("  Total stack usage: %zu bytes\n", 
           sizeof(cardid_result) + sizeof(cardid_extract_result) + sizeof(digits));
    printf("\n");
    
    // Test with different input sizes
    const char* inputs[] = {
        "1234",                    // 4 digits
        "1234567890",              // 10 digits
        "1234567890123456",        // 16 digits
        "1234567890123456789",     // 19 digits
        NULL
    };
    
    for (int i = 0; inputs[i] != NULL; i++) {
        cardid_analyze(inputs[i], &result, &meta);
        printf("Input: %s\n", inputs[i]);
        printf("  Length: %d\n", result.length);
        printf("  Valid: %s\n", result.luhn_valid ? "Yes" : "No");
        printf("  Network: %s\n", 
               result.network == CARD_UNKNOWN ? "Unknown" : "Detected");
        printf("\n");
    }
}

/**
 * @brief Main benchmark function
 */
int main() {
    printf("CardID Performance Benchmarks\n");
    printf("==============================\n\n");
    
    // Run all benchmarks
    benchmark_luhn();
    benchmark_extraction();
    benchmark_network_detection();
    benchmark_analysis();
    benchmark_card_types();
    benchmark_memory();
    
    printf("Benchmark completed!\n");
    return 0;
}

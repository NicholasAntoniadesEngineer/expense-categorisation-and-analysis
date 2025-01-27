#pragma once

#include <string>
#include <chrono>

namespace finance {

// Represents currency types
enum class Currency {
    GBP,    // British Pound
    EUR,    // Euro
    USD,    // US Dollar
    UNKNOWN // Default/Unknown currency
};

// Helper function to convert currency string to enum
inline Currency stringToCurrency(const std::string& symbol) {
    if (symbol == "£" || symbol == "GBP" || symbol == "GBR") return Currency::GBP;
    if (symbol == "€" || symbol == "EUR") return Currency::EUR;
    if (symbol == "$" || symbol == "USD") return Currency::USD;
    return Currency::UNKNOWN;
}

// Helper function to get currency symbol
inline std::string currencyToSymbol(Currency currency) {
    switch (currency) {
        case Currency::GBP: return "GBP";
        case Currency::EUR: return "EUR";
        case Currency::USD: return "USD";
        default: return "UNKNOWN";
    }
}

// Represents a single financial expense entry
struct Expense {
    std::chrono::system_clock::time_point date;  // Transaction date
    std::string month;                           // YYYY-MM format for grouping
    std::string file_origin;                     // Source of the expense data
    std::string description;                     // Transaction description
    double amount;                               // Transaction amount
    Currency currency;                           // Currency of the transaction
    std::string category;                        // Expense category
    std::string name;                            // Additional info
};

// Represents column indices in CSV files
struct CSVColumns {
    int date_col = -1;          // Date column index
    int description_col = -1;    // Description/merchant column index
    int amount_col = -1;        // Amount/value column index
    int name_col = -1;          // Name column index
};

} // namespace finance 
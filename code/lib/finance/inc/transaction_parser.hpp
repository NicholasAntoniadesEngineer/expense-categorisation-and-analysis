#pragma once

#include "finance_types.hpp"
#include <string>
#include <chrono>
#include <utility>

namespace finance {

// Handles parsing and formatting of financial values
class TransactionParser {
public:
    // Parse date string to time_point (format: DD/MM/YYYY)
    static std::chrono::system_clock::time_point parseDate(const std::string& date_str);
    
    // Extract YYYY-MM format from time_point
    static std::string extractMonth(const std::chrono::system_clock::time_point& date);
    
    // Parse amount string to double and detect currency
    // Returns pair of (amount, currency)
    static std::pair<double, Currency> parseAmount(const std::string& amount_str);

private:
    // Parse currency type from amount string (symbols and codes)
    static Currency parseCurrencyType(const std::string& amount_str);
    
    // Remove currency symbols from amount string
    static std::string removeCurrencySymbols(const std::string& amount_str);
    
    // Remove numeric formatting from amount string
    static std::string removeNumericFormatting(const std::string& amount_str);
};

} // namespace finance 
#include "transaction_parser.hpp"
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <iostream>
#include <regex>

namespace finance {

using TimePoint = std::chrono::system_clock::time_point;

TimePoint TransactionParser::parseDate(
    const std::string& date_str) {
    std::istringstream ss(date_str);
    std::tm tm = {};  // Zero-initialize
    
    // Parse date using format specifier
    ss >> std::get_time(&tm, "%d/%m/%Y");
    
    if (ss.fail()) {
        throw std::runtime_error("Failed to parse date: " + date_str);
    }
    
    return std::chrono::system_clock::from_time_t(std::mktime(&tm));
}

std::string TransactionParser::extractMonth(
    const std::chrono::system_clock::time_point& date) {
    auto time = std::chrono::system_clock::to_time_t(date);
    std::tm tm = *std::localtime(&time);
    char buffer[8];
    std::strftime(buffer, sizeof(buffer), "%Y-%m", &tm);
    return std::string(buffer);
}

Currency TransactionParser::parseCurrencyType(const std::string& amount_str) {
    // Look for currency symbols at the start or end of the string
    std::string str = amount_str;
    str.erase(std::remove_if(str.begin(), str.end(), ::isspace), str.end());
    
    if (str.find("£") != std::string::npos) return Currency::GBP;
    if (str.find("€") != std::string::npos) return Currency::EUR;
    if (str.find("$") != std::string::npos) return Currency::USD;
    
    // Look for currency codes
    if (str.find("GBP") != std::string::npos || str.find("GBR") != std::string::npos) return Currency::GBP;
    if (str.find("EUR") != std::string::npos) return Currency::EUR;
    if (str.find("USD") != std::string::npos) return Currency::USD;
    
    return Currency::GBP;
}

std::pair<double, Currency> TransactionParser::parseAmount(const std::string& amount_str) {
    // First parse the currency type
    Currency currency = parseCurrencyType(amount_str);
    
    // Remove currency symbols and clean numeric formatting
    std::string cleaned = removeNumericFormatting(removeCurrencySymbols(amount_str));
    
    // Handle empty or invalid strings
    if (cleaned.empty() || cleaned == "-") {
        return {0.0, currency};
    }
    
    try {
        double amount = std::stod(cleaned);
        return {amount, currency};
    } catch (const std::exception& e) {
        std::cerr << "Failed to parse amount: " << amount_str 
                  << " (cleaned: " << cleaned << ")" << std::endl;
        return {0.0, Currency::UNKNOWN};
    }
}

std::string TransactionParser::removeCurrencySymbols(const std::string& amount_str) {
    std::string cleaned = amount_str;
    
    // Remove single-byte currency symbols and whitespace
    cleaned.erase(std::remove_if(cleaned.begin(), cleaned.end(),
        [](unsigned char c) { return c == '$' || std::isspace(c); }),
        cleaned.end());
    
    // Remove multi-byte currency symbols
    size_t pos;
    while ((pos = cleaned.find("£")) != std::string::npos) {
        cleaned.erase(pos, strlen("£"));
    }
    while ((pos = cleaned.find("€")) != std::string::npos) {
        cleaned.erase(pos, strlen("€"));
    }
    
    // Remove currency codes
    std::regex currency_code(R"(\b(GBP|GBR|EUR|USD)\b)");
    cleaned = std::regex_replace(cleaned, currency_code, "");
    
    return cleaned;
}

std::string TransactionParser::removeNumericFormatting(const std::string& amount_str) {
    std::string cleaned = amount_str;
    
    // Remove quotes that might enclose the amount
    cleaned.erase(std::remove(cleaned.begin(), cleaned.end(), '"'),
                 cleaned.end());
    
    // Remove thousands separators (commas)
    cleaned.erase(std::remove(cleaned.begin(), cleaned.end(), ','),
                 cleaned.end());
    
    return cleaned;
}

} // namespace finance 
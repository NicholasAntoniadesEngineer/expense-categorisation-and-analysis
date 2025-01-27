#pragma once

#include "finance_types.hpp"
#include <string>
#include <vector>

namespace finance {

// Handles CSV file parsing and field extraction
class CSVParser {
public:
    // Parse CSV header to identify column positions
    static CSVColumns parseHeader(const std::string& header_line);
    
    // Parse a CSV line into fields, handling quoted values
    static std::vector<std::string> parseLine(const std::string& line);
    
    // Clean and standardize field values
    static std::string cleanField(const std::string& field);

private:
    // Remove quotes and whitespace from a field
    static std::string removeQuotesAndWhitespace(const std::string& field);
};

} // namespace finance 
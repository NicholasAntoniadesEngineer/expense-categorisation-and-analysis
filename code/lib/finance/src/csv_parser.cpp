#include "csv_parser.hpp"
#include <algorithm>
#include <sstream>

namespace finance {

CSVColumns CSVParser::parseHeader(const std::string& header_line) {
    CSVColumns cols;
    std::vector<std::string> fields = parseLine(header_line);
    
    // Find column indices based on header names
    for (size_t i = 0; i < fields.size(); ++i) {
        std::string field = cleanField(fields[i]);
        
        if (field.find("date") != std::string::npos) {
            cols.date_col = i;
        } else if (field.find("description") != std::string::npos || 
                  field.find("merchant") != std::string::npos ||
                  field.find("details") != std::string::npos) {
            cols.description_col = i;
        } else if (field.find("amount") != std::string::npos ||
                  field.find("value") != std::string::npos) {
            cols.amount_col = i;
        } else if (field.find("name") != std::string::npos) {
            cols.name_col = i;
        }
    }
    
    return cols;
}

std::vector<std::string> CSVParser::parseLine(const std::string& line) {
    std::vector<std::string> fields;
    bool in_quotes = false;
    std::string current_field;
    
    // Parse character by character, handling quoted fields
    for (size_t i = 0; i < line.length(); ++i) {
        char c = line[i];
        
        if (c == '"') {
            in_quotes = !in_quotes;
            // Skip adding the quote character
            continue;
        }
        
        if (c == ',' && !in_quotes) {
            // Remove leading/trailing whitespace
            current_field.erase(0, current_field.find_first_not_of(" \t\r\n"));
            current_field.erase(current_field.find_last_not_of(" \t\r\n") + 1);
            fields.push_back(current_field);
            current_field.clear();
        } else {
            current_field += c;
        }
    }
    
    // Add the last field after trimming whitespace
    current_field.erase(0, current_field.find_first_not_of(" \t\r\n"));
    current_field.erase(current_field.find_last_not_of(" \t\r\n") + 1);
    fields.push_back(current_field);
    
    return fields;
}

std::string CSVParser::cleanField(const std::string& field) {
    // Convert to lowercase for case-insensitive matching
    std::string cleaned = field;
    std::transform(cleaned.begin(), cleaned.end(), cleaned.begin(), ::tolower);
    
    return removeQuotesAndWhitespace(cleaned);
}

std::string CSVParser::removeQuotesAndWhitespace(const std::string& field) {
    std::string cleaned = field;
    
    // Remove quotes
    cleaned.erase(std::remove(cleaned.begin(), cleaned.end(), '"'), 
                 cleaned.end());
    
    // Remove whitespace
    cleaned.erase(std::remove(cleaned.begin(), cleaned.end(), ' '), 
                 cleaned.end());
    
    return cleaned;
}

} // namespace finance 
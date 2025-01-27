#include "keyword_loader.hpp"
#include "csv_parser.hpp"
#include <fstream>
#include <iostream>

namespace finance {

KeywordLoader::KeywordLoader(const std::string& filepath)
    : filepath_(filepath) {}

std::map<std::string, std::string> KeywordLoader::loadKeywords() {
    std::map<std::string, std::string> keyword_map;
    std::ifstream file(filepath_);
    
    if (!file.is_open()) {
        throw std::runtime_error("Could not open keyword mapping file");
    }
    
    try {
        std::string line;
        // Skip header
        std::getline(file, line);
        
        // Process each line
        while (std::getline(file, line)) {
            auto fields = CSVParser::parseLine(line);
            if (fields.size() >= 2) {
                std::string category = fields[0];
                std::string keyword = fields[1];
                
                // Convert keyword to lowercase for case-insensitive matching
                std::transform(keyword.begin(), keyword.end(), keyword.begin(),
                             [](unsigned char c) { return std::tolower(c); });
                
                keyword_map[keyword] = category;
            }
        }
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to load keyword mapping: " + 
                               std::string(e.what()));
    }
    
    if (keyword_map.empty()) {
        throw std::runtime_error("No keywords loaded from file");
    }
    
    return keyword_map;
}

} // namespace finance 
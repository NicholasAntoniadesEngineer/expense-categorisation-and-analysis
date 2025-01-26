#include "finance_types.hpp" 
#include "data_loader.hpp"
#include "csv_parser.hpp"
#include "transaction_parser.hpp"
#include <filesystem>   
#include <fstream>      
#include <sstream>      
#include <iostream>    
#include <algorithm>   
#include <iomanip>     
#include <regex>        
#include <numeric>      

namespace finance {

namespace fs = std::filesystem;

// Constructor implementation
DataLoader::DataLoader(const std::string& directory)
    : directory_(directory) {}  list

std::string DataLoader::getFileOrigin(const std::string& basename) {
    std::istringstream iss(basename);
    std::string part;
    std::vector<std::string> parts;
    
    // Split filename by spaces and filter out common words
    while (std::getline(iss, part, ' ')) {
        if (part != "-" && part != "Data" && part != "Export") {
            // Remove .csv extension if present
            if (part.length() > 4 && part.substr(part.length() - 4) == ".csv") {
                part = part.substr(0, part.length() - 4);
            }
            parts.push_back(part);
        }
    }
    
    // Join remaining parts with spaces using std::accumulate
    return parts.empty() ? "" : 
           std::accumulate(std::next(parts.begin()), parts.end(), parts[0],
                         [](const std::string& a, const std::string& b) {
                             return a + " " + b;
                         });
}

Expense DataLoader::createExpense(
    const std::vector<std::string>& fields,
    const CSVColumns& cols,
    const std::string& file_origin) {
    
    if (fields.size() <= static_cast<size_t>(
        std::max({cols.date_col, cols.description_col, cols.amount_col}))) {
        throw std::runtime_error("Invalid number of fields");
    }
    
    Expense expense;
    expense.date = TransactionParser::parseDate(fields[cols.date_col]);
    expense.month = TransactionParser::extractMonth(expense.date);
    expense.file_origin = file_origin;
    
    // Clean up description field
    std::string description = fields[cols.description_col];
    // Remove any remaining quotes
    description.erase(std::remove(description.begin(), description.end(), '"'), description.end());
    
    // Remove everything after and including the first comma
    size_t comma_pos = description.find(',');
    if (comma_pos != std::string::npos) {
        description = description.substr(0, comma_pos);
    }
    
    // Clean up any extra spaces
    description = std::regex_replace(description, std::regex(R"(\s+)"), " ");
    description = std::regex_replace(description, std::regex(R"(^\s+|\s+$)"), "");
    
    // Extract currency code from description if present
    std::regex currency_pattern(R"(\b(GBR|GBP|EUR|USD)\b)");
    std::smatch match;
    if (std::regex_search(description, match, currency_pattern)) {
        expense.currency = stringToCurrency(match[1].str());
        // Remove the currency code from description
        description = std::regex_replace(description, currency_pattern, "");
        // Clean up any extra spaces again
        description = std::regex_replace(description, std::regex(R"(\s+)"), " ");
        description = std::regex_replace(description, std::regex(R"(^\s+|\s+$)"), "");
    }
    expense.description = description;
    
    // Parse amount and currency together
    auto [amount, detected_currency] = TransactionParser::parseAmount(fields[cols.amount_col]);
    expense.amount = amount;
    // Only use detected currency if we didn't find one in the description
    if (expense.currency == Currency::UNKNOWN) {
        expense.currency = detected_currency;
    }
    
    // Handle AMEX amount sign
    bool is_amex = file_origin.find("amex") != std::string::npos || 
                   file_origin.find("american express") != std::string::npos;
    if (is_amex) {
        expense.amount = -expense.amount;
    }
    
    // Handle optional name field
    if (cols.name_col != -1 && 
        fields.size() > static_cast<size_t>(cols.name_col)) {
        expense.name = fields[cols.name_col];
        
        // Use name as description if description is empty
        if (expense.description.empty()) {
            expense.description = expense.name;
        }
    }
    
    return expense;
}

std::vector<Expense> DataLoader::processFile(const std::string& filepath) {
    std::vector<Expense> expenses;
    std::ifstream file(filepath);
    
    if (!file.is_open()) {
        std::cerr << "Could not open file: " << filepath << std::endl;
        return expenses;
    }
    
    try {
        // Read and parse header
        std::string header_line;
        if (!std::getline(file, header_line)) {
            std::cerr << "Empty file: " << filepath << std::endl;
            return expenses;
        }
        
        CSVColumns cols = CSVParser::parseHeader(header_line);
        if (cols.date_col == -1 || cols.description_col == -1 || 
            cols.amount_col == -1) {
            std::cerr << "Required columns not found in file: " << filepath << std::endl;
            return expenses;
        }
        
        std::string file_origin = getFileOrigin(fs::path(filepath).filename().string());
        
        // Process each line
        std::string line;
        while (std::getline(file, line)) {
            try {
                auto fields = CSVParser::parseLine(line);
                expenses.push_back(createExpense(fields, cols, file_origin));
            } catch (const std::exception& e) {
                std::cerr << "Error processing line in " << filepath 
                         << ": " << e.what() << std::endl;
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error processing file " << filepath 
                  << ": " << e.what() << std::endl;
    }
    
    return expenses;
}

// Main function to load and process all expense data
std::vector<Expense> DataLoader::loadAndPreprocessData() {
    std::vector<Expense> all_expenses;
    
    try {
        // Process each CSV file in the directory
        for (const auto& entry : fs::directory_iterator(directory_)) {
            if (entry.path().extension() != ".csv") continue;
            
            auto file_expenses = processFile(entry.path().string());
            all_expenses.insert(all_expenses.end(), 
                              file_expenses.begin(), 
                              file_expenses.end());
        }
    } catch (const std::exception& e) {
        std::cerr << "Error loading data: " << e.what() << std::endl;
        return std::vector<Expense>();
    }
    
    if (all_expenses.empty()) {
        std::cerr << "No data found in the CSV files." << std::endl;
    }
    
    return all_expenses;
}

} // namespace finance 
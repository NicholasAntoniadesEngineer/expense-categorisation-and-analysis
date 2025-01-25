#pragma once  // Modern header guard, prevents multiple inclusions

// Standard C++ library includes
#include <string>    // For std::string
#include <vector>    // For std::vector (dynamic array)
#include <memory>    // For smart pointers (not used here but good practice)
#include <chrono>    // For time-related functionality

// All code is wrapped in a namespace to prevent naming conflicts
namespace finance {

// Main class for loading and preprocessing financial data from CSV files
class DataLoader {
public:
    // Constructor - takes directory containing CSV files
    explicit DataLoader(const std::string& directory);

    // Load and preprocess all expense data from CSV files
    std::vector<Expense> loadAndPreprocessData();

private:
    // Extract meaningful origin name from filename
    std::string getFileOrigin(const std::string& basename);
    
    // Process a single CSV file
    std::vector<Expense> processFile(const std::string& filepath);
    
    // Create expense object from CSV fields
    Expense createExpense(const std::vector<std::string>& fields, 
                         const CSVColumns& cols,
                         const std::string& file_origin);

    // Directory containing CSV files
    std::string directory_;
};

} // namespace finance 
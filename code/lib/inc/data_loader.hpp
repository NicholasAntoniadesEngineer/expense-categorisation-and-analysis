#pragma once

#include <string>    
#include <vector>   
#include <memory>  
#include <chrono>   

namespace finance {

class DataLoader {
public:
    explicit DataLoader(const std::string& directory);

    std::vector<Expense> loadAndPreprocessData();

private:
    std::string getFileOrigin(const std::string& basename);
    
    // Process a single CSV file
    std::vector<Expense> processFile(const std::string& filepath);
    
    // Create expense object from CSV fields
    Expense createExpense(const std::vector<std::string>& fields, 
                         const CSVColumns& cols,
                         const std::string& file_origin);

    std::string directory_;
};

} // namespace finance 
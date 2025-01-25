#include "finance_types.hpp"
#include "keyword_loader.hpp"
#include "data_loader.hpp"
#include "transaction_categorisation.hpp"
#include "report_generator.hpp"
#include "data_exporter.hpp"
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

// Function to ensure directory exists
void ensureDirectoryExists(const std::string& path) {
    if (!fs::exists(path)) {
        fs::create_directories(path);
    }
}

// Main class to handle the finance categorisation process
class FinanceProcessor {
public:
    // Constructor takes input and output directories
    FinanceProcessor(const std::string& directory,
                    const std::string& output_dir,
                    const std::string& keyword_file)
        : directory_(directory)
        , output_dir_(output_dir)
        , keyword_file_(keyword_file) {}
    
    // Main processing function
    void run() {
        try {
            // Load keyword mapping
            finance::KeywordLoader loader(keyword_file_);
            auto keyword_map = loader.loadKeywords();
            if (keyword_map.empty()) {
                throw std::runtime_error("Failed to load keyword mapping");
            }
            
            // Load and preprocess expense data
            finance::DataLoader data_loader(directory_);
            auto all_expenses = data_loader.loadAndPreprocessData();
            if (all_expenses.empty()) {
                throw std::runtime_error("No expense data found");
            }
            
            // categorise expenses
            finance::TransactionCategorisation categoriser(keyword_map);
            categoriser.categoriseExpenses(all_expenses);
            
            // Generate reports and export data
            finance::ReportGenerator report_gen(output_dir_);
            report_gen.generateReports(all_expenses);
            
            // Export data (monthly summaries and entire dataset)
            finance::DataExporter exporter(output_dir_, true, false, true);
            exporter.exportData(all_expenses);
            
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            throw;
        }
    }
    
private:
    std::string directory_;
    std::string output_dir_;
    std::string keyword_file_;
};

int main() {
    try {
        std::cout << "Starting the finance categorisation script..." << std::endl;
        
        // Create processor with configured paths
        FinanceProcessor processor(
            "files_to_categorise",
            "files_categorised",
            "code/categorisation_keywords.csv"
        );
        
        // Run the processing
        processor.run();
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
} 
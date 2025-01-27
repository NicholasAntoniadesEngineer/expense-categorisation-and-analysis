#include "finance_processor.hpp"
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
static void ensureDirectoryExists(const std::string& path) {
    if (!fs::exists(path)) {
        fs::create_directories(path);
    }
}

FinanceProcessor::FinanceProcessor(const std::string& directory,
                                 const std::string& output_dir,
                                 const std::string& keyword_file,
                                 bool export_monthly_summary,
                                 bool export_weekly_summary,
                                 bool export_full_dataset)
    : directory_(directory)
    , output_dir_(output_dir)
    , keyword_file_(keyword_file)
    , export_monthly_summary_(export_monthly_summary)
    , export_weekly_summary_(export_weekly_summary)
    , export_full_dataset_(export_full_dataset) {}

void FinanceProcessor::run() {
    try {
        // Ensure directories exist
        ensureDirectoryExists(directory_);
        ensureDirectoryExists(output_dir_);
        
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
        
        // Export data with user-specified options
        finance::DataExporter exporter(output_dir_, 
                                     export_monthly_summary_,
                                     export_weekly_summary_,
                                     export_full_dataset_);
        exporter.exportData(all_expenses);
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        throw;
    }
} 
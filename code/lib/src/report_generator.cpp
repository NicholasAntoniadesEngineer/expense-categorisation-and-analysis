#include "report_generator.hpp"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>

namespace finance {

namespace fs = std::filesystem;

ReportGenerator::ReportGenerator(const std::string& output_dir)
    : output_dir_(output_dir) {
    // Create output directory if it doesn't exist
    fs::create_directories(output_dir);
}

void ReportGenerator::generateReports(const std::vector<Expense>& expenses) {
    // Generate full report with all expenses
    generateFullReport(expenses);
}

void ReportGenerator::generateFullReport(const std::vector<Expense>& expenses) {
    // Create output file with fixed name
    std::string filepath = fs::path(output_dir_) / "categorised_transactions.csv";
    std::ofstream file(filepath);
    
    if (!file.is_open()) {
        throw std::runtime_error("Could not create full report file: " + filepath);
    }
    
    // Write header
    file << "Date,Month,FileOrigin,Description,Amount,Currency,Category\n";
    
    // Write expenses
    for (const auto& expense : expenses) {
        // Format date
        auto time = std::chrono::system_clock::to_time_t(expense.date);
        std::tm tm = *std::localtime(&time);
        char date_str[11];
        std::strftime(date_str, sizeof(date_str), "%d/%m/%Y", &tm);
        
        file << date_str << ","
             << expense.month << ","
             << expense.file_origin << ","
             << expense.description << ","
             << std::fixed << std::setprecision(2) << std::abs(expense.amount) << ","
             << currencyToSymbol(expense.currency) << ","
             << expense.category << "\n";
    }
}

std::map<std::string, double> ReportGenerator::calculateCategoryTotals(
    const std::vector<Expense>& expenses) {
    
    std::map<std::string, double> totals;
    
    for (const auto& expense : expenses) {
        // Skip expenses with unknown currency
        if (expense.currency == Currency::UNKNOWN) continue;
        
        // Convert to GBP if necessary (simplified conversion)
        double amount_gbp = expense.amount;
        if (expense.currency == Currency::EUR) {
            amount_gbp *= 0.86;  // Approximate EUR to GBP conversion
        } else if (expense.currency == Currency::USD) {
            amount_gbp *= 0.79;  // Approximate USD to GBP conversion
        }
        
        totals[expense.category] += amount_gbp;
    }
    
    return totals;
}

} // namespace finance 
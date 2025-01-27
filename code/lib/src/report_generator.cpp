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
    // Group expenses by month
    std::map<std::string, std::vector<Expense>> monthly_expenses;
    for (const auto& expense : expenses) {
        monthly_expenses[expense.month].push_back(expense);
    }
    
    // Generate monthly reports
    for (const auto& [month, month_expenses] : monthly_expenses) {
        generateMonthlyReport(month_expenses, month);
    }
    
    // Generate full report with all expenses
    generateFullReport(expenses);
}

void ReportGenerator::generateMonthlyReport(
    const std::vector<Expense>& expenses,
    const std::string& month) {
    
    // Calculate totals by category
    auto category_totals = calculateCategoryTotals(expenses);
    
    // Create output file
    std::string filepath = fs::path(output_dir_) / (month + "_monthly_summary.csv");
    std::ofstream file(filepath);
    
    if (!file.is_open()) {
        throw std::runtime_error("Could not create monthly report file: " + filepath);
    }
    
    // Write header
    file << "Category,Total (GBP)\n";
    
    // Write category totals
    for (const auto& [category, total] : category_totals) {
        file << category << "," << std::fixed << std::setprecision(2) << total << "\n";
    }
}

void ReportGenerator::generateFullReport(const std::vector<Expense>& expenses) {
    // Get current date for filename
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::tm tm = *std::localtime(&time);
    char date_buffer[11];
    std::strftime(date_buffer, sizeof(date_buffer), "%Y-%m-%d", &tm);
    
    // Create output file
    std::string filepath = fs::path(output_dir_) / (std::string(date_buffer) + "_finance.csv");
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
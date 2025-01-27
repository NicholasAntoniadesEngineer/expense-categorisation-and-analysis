#include "data_exporter.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <chrono>
#include <iomanip>
#include <sstream>

namespace finance {

namespace fs = std::filesystem;

DataExporter::DataExporter(const std::string& output_dir,
                         bool export_monthly,
                         bool export_weekly,
                         bool export_entire)
    : output_dir_(output_dir)
    , export_monthly_(export_monthly)
    , export_weekly_(export_weekly)
    , export_entire_(export_entire) {
    // Create output directory if it doesn't exist
    fs::create_directories(output_dir);
}

void DataExporter::exportData(const std::vector<Expense>& expenses) {
    if (!export_monthly_ && !export_weekly_ && !export_entire_) {
        std::cerr << "Warning: No export flags set. No files will be generated.\n";
        return;
    }

    if (export_monthly_) {
        exportMonthlyData(expenses);
    }
    if (export_weekly_) {
        exportWeeklyData(expenses);
    }
    if (export_entire_) {
        exportEntireData(expenses);
    }
}

void DataExporter::exportMonthlyData(const std::vector<Expense>& expenses) {
    // Group expenses by month
    std::map<std::string, std::vector<Expense>> monthly_expenses;
    for (const auto& expense : expenses) {
        monthly_expenses[expense.month].push_back(expense);
    }
    
    // Export each month's expenses
    for (const auto& [month, month_expenses] : monthly_expenses) {
        std::string filename = month + "_monthly_summary.csv";
        std::string filepath = fs::path(output_dir_) / filename;
        
        std::ofstream file(filepath);
        if (!file.is_open()) {
            throw std::runtime_error("Could not create file: " + filepath);
        }
        
        // Write header
        file << "Category,Total (GBP)\n";
        
        // Calculate totals by category
        std::map<std::string, double> category_totals;
        for (const auto& expense : month_expenses) {
            // Convert to GBP if necessary
            double amount_gbp = expense.amount;
            if (expense.currency == Currency::EUR) {
                amount_gbp *= 0.86;  // Approximate EUR to GBP conversion
            } else if (expense.currency == Currency::USD) {
                amount_gbp *= 0.79;  // Approximate USD to GBP conversion
            }
            
            // Use "Uncategorised" for empty categories
            std::string category = expense.category.empty() ? "Uncategorised" : expense.category;
            category_totals[category] += amount_gbp;
        }
        
        // Write category totals
        for (const auto& [category, total] : category_totals) {
            file << category << "," << std::fixed << std::setprecision(2) << total << "\n";
        }
    }
}

void DataExporter::exportWeeklyData(const std::vector<Expense>& expenses) {
    // Group expenses by week
    std::map<std::string, std::vector<Expense>> weekly_expenses;
    
    for (const auto& expense : expenses) {
        // Get the start of the week (Monday) for this expense
        auto time = std::chrono::system_clock::to_time_t(expense.date);
        std::tm tm = *std::localtime(&time);
        
        // Calculate days to subtract to get to Monday (tm_wday is 0-based, Sunday = 0)
        int days_to_monday = (tm.tm_wday == 0) ? 6 : tm.tm_wday - 1;
        
        // Subtract days to get to Monday
        tm.tm_mday -= days_to_monday;
        std::mktime(&tm);  // Normalize the time
        
        // Format the week start date
        char buffer[11];
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%d", &tm);
        std::string week_key = buffer;
        
        weekly_expenses[week_key].push_back(expense);
    }
    
    // Export each week's expenses
    for (const auto& [week, week_expenses] : weekly_expenses) {
        std::string filename = week + "_weekly_summary.csv";
        std::string filepath = fs::path(output_dir_) / filename;
        
        std::ofstream file(filepath);
        if (!file.is_open()) {
            throw std::runtime_error("Could not create file: " + filepath);
        }
        
        // Write header
        file << "Category,Total (GBP)\n";
        
        // Calculate totals by category
        std::map<std::string, double> category_totals;
        for (const auto& expense : week_expenses) {
            if (expense.currency == Currency::UNKNOWN) continue;
            
            // Convert to GBP if necessary
            double amount_gbp = expense.amount;
            if (expense.currency == Currency::EUR) {
                amount_gbp *= 0.86;
            } else if (expense.currency == Currency::USD) {
                amount_gbp *= 0.79;
            }
            
            category_totals[expense.category] += amount_gbp;
        }
        
        // Write category totals
        for (const auto& [category, total] : category_totals) {
            file << category << "," << std::fixed << std::setprecision(2) << total << "\n";
        }
    }
}

void DataExporter::exportEntireData(const std::vector<Expense>& expenses) {
    // Get current date for filename
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::tm tm = *std::localtime(&time);
    char date_buffer[11];
    std::strftime(date_buffer, sizeof(date_buffer), "%Y-%m-%d", &tm);
    
    std::string filepath = fs::path(output_dir_) / (std::string(date_buffer) + "_finance.csv");
    std::ofstream file(filepath);
    
    if (!file.is_open()) {
        throw std::runtime_error("Could not create file: " + filepath);
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

} // namespace finance 
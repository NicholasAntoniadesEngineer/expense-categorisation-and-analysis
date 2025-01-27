#include "data_exporter.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <set>
#include <algorithm>

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
    // Get all unique categories and months
    std::set<std::string> categories;
    std::map<std::string, std::map<std::string, double>> category_month_totals;
    std::vector<std::string> months;
    
    for (const auto& expense : expenses) {
        // Use "Uncategorised" for empty categories
        std::string category = expense.category.empty() ? "Uncategorised" : expense.category;
        categories.insert(category);
        
        // Convert to GBP if necessary
        double amount_gbp = expense.amount;
        if (expense.currency == Currency::EUR) {
            amount_gbp *= 0.86;  // Approximate EUR to GBP conversion
        } else if (expense.currency == Currency::USD) {
            amount_gbp *= 0.79;  // Approximate USD to GBP conversion
        }
        
        category_month_totals[category][expense.month] += amount_gbp;
        
        // Keep track of months in order of appearance
        if (std::find(months.begin(), months.end(), expense.month) == months.end()) {
            months.push_back(expense.month);
        }
    }
    
    // Sort months chronologically
    std::sort(months.begin(), months.end());
    
    // Create the monthly summary file
    std::string filepath = fs::path(output_dir_) / "monthly_summary.csv";
    std::ofstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not create file: " + filepath);
    }
    
    // Write header with months
    file << "Category";
    for (const auto& month : months) {
        file << "," << month;
    }
    file << "\n";
    
    // Write data for each category
    for (const auto& category : categories) {
        file << category;
        for (const auto& month : months) {
            double total = category_month_totals[category][month];
            file << "," << std::fixed << std::setprecision(2) << total;
        }
        file << "\n";
    }
}

void DataExporter::exportWeeklyData(const std::vector<Expense>& expenses) {
    // Get all unique categories and weeks
    std::set<std::string> categories;
    std::map<std::string, std::map<std::string, double>> category_week_totals;
    std::vector<std::string> weeks;
    
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
        
        // Use "Uncategorised" for empty categories
        std::string category = expense.category.empty() ? "Uncategorised" : expense.category;
        categories.insert(category);
        
        // Convert to GBP if necessary
        double amount_gbp = expense.amount;
        if (expense.currency == Currency::EUR) {
            amount_gbp *= 0.86;
        } else if (expense.currency == Currency::USD) {
            amount_gbp *= 0.79;
        }
        
        category_week_totals[category][week_key] += amount_gbp;
        
        // Keep track of weeks in order of appearance
        if (std::find(weeks.begin(), weeks.end(), week_key) == weeks.end()) {
            weeks.push_back(week_key);
        }
    }
    
    // Sort weeks chronologically
    std::sort(weeks.begin(), weeks.end());
    
    // Create the weekly summary file
    std::string filepath = fs::path(output_dir_) / "weekly_summary.csv";
    std::ofstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not create file: " + filepath);
    }
    
    // Write header with weeks
    file << "Category";
    for (const auto& week : weeks) {
        file << "," << week;
    }
    file << "\n";
    
    // Write data for each category
    for (const auto& category : categories) {
        file << category;
        for (const auto& week : weeks) {
            double total = category_week_totals[category][week];
            file << "," << std::fixed << std::setprecision(2) << total;
        }
        file << "\n";
    }
}

void DataExporter::exportEntireData(const std::vector<Expense>& expenses) {
    std::string filepath = fs::path(output_dir_) / "categorised_transactions.csv";
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
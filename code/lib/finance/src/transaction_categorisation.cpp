#include "transaction_categorisation.hpp"
#include <algorithm>
#include <cctype>

namespace finance {

TransactionCategorisation::TransactionCategorisation(
    const std::map<std::string, std::string>& keyword_map)
    : keyword_map_(keyword_map) {}

void TransactionCategorisation::categoriseExpense(Expense& expense) const {
    // Find matching category based on description
    std::string category = findMatchingCategory(expense.description);
    
    // If no match found and name is available, try matching on name
    if (category.empty() && !expense.name.empty()) {
        category = findMatchingCategory(expense.name);
    }
    
    // Handle credit card repayments
    if (category == "Credit card") {
        std::string lower_desc = toLower(expense.description);
        if (lower_desc.find("amex") != std::string::npos || 
            lower_desc.find("payment received") != std::string::npos) {
            // Invert the amount for credit card repayments
            expense.amount = -expense.amount;
        }
    }
    
    // Set the category (use "Uncategorised" if no match found)
    expense.category = category.empty() ? "Uncategorised" : category;
}

void TransactionCategorisation::categoriseExpenses(
    std::vector<Expense>& expenses) const {
    for (auto& expense : expenses) {
        categoriseExpense(expense);
    }
}

std::string TransactionCategorisation::toLower(const std::string& str) {
    std::string lower = str;
    std::transform(lower.begin(), lower.end(), lower.begin(), 
                  [](unsigned char c) { return std::tolower(c); });
    return lower;
}

std::string TransactionCategorisation::findMatchingCategory(
    const std::string& description) const {
    // Convert description to lowercase for case-insensitive matching
    std::string lower_desc = toLower(description);
    
    // Check each keyword
    for (const auto& [keyword, category] : keyword_map_) {
        if (lower_desc.find(toLower(keyword)) != std::string::npos) {
            return category;
        }
    }
    
    return "";  // No match found
}

} // namespace finance 
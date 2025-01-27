#pragma once

#include "finance_types.hpp"
#include <string>
#include <vector>
#include <map>

namespace finance {

class TransactionCategorisation {
public:
    // Constructor takes a map of keywords to categories
    explicit TransactionCategorisation(const std::map<std::string, std::string>& keyword_map);
    
    // categorise a single expense based on its description
    void categoriseExpense(Expense& expense) const;
    
    // categorise a vector of expenses
    void categoriseExpenses(std::vector<Expense>& expenses) const;
    
private:
    std::map<std::string, std::string> keyword_map_;
    
    // Helper function to convert description to lowercase for matching
    static std::string toLower(const std::string& str);
    
    // Helper function to find matching category based on keywords
    std::string findMatchingCategory(const std::string& description) const;
};

} // namespace finance 
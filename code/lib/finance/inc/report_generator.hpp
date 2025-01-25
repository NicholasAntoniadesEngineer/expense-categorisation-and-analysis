#pragma once

#include "finance_types.hpp"
#include <string>
#include <vector>
#include <map>

namespace finance {

class ReportGenerator {
public:
    // Constructor takes the output directory path
    explicit ReportGenerator(const std::string& output_dir);
    
    // Generate reports for the given expenses
    void generateReports(const std::vector<Expense>& expenses);
    
private:
    std::string output_dir_;
    
    // Helper functions for report generation
    void generateMonthlyReport(const std::vector<Expense>& expenses, 
                             const std::string& month);
    void generateFullReport(const std::vector<Expense>& expenses);
    
    // Calculate monthly totals by category
    std::map<std::string, double> calculateCategoryTotals(
        const std::vector<Expense>& expenses);
};

} // namespace finance 
#pragma once

#include "finance_types.hpp"
#include "report_generator.hpp"
#include <string>
#include <vector>

namespace finance {

class DataExporter {
public:
    // Constructor takes output directory and export options
    DataExporter(const std::string& output_dir,
                bool export_monthly = false,
                bool export_weekly = false,
                bool export_entire = false);
    
    // Export data to files
    void exportData(const std::vector<Expense>& expenses);
    
private:
    std::string output_dir_;
    bool export_monthly_;
    bool export_weekly_;
    bool export_entire_;
    
    // Helper functions
    void exportMonthlyData(const std::vector<Expense>& expenses);
    void exportWeeklyData(const std::vector<Expense>& expenses);
    void exportEntireData(const std::vector<Expense>& expenses);
};

} // namespace finance 
#pragma once

#include <string>

class FinanceProcessor {
public:
    // Constructor takes input and output directories, and export options
    FinanceProcessor(const std::string& directory,
                    const std::string& output_dir,
                    const std::string& keyword_file,
                    bool export_monthly_summary = true,
                    bool export_weekly_summary = false,
                    bool export_full_dataset = true);
    
    // Main processing function
    void run();
    
private:
    std::string directory_;
    std::string output_dir_;
    std::string keyword_file_;
    bool export_monthly_summary_;
    bool export_weekly_summary_;
    bool export_full_dataset_;
}; 
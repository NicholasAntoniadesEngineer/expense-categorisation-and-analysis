"""
personal-finance/finance_categorization.py

This script processes financial data from CSV files, categorizes expenses based on keywords,
removes duplicates, and generates detailed and summary reports. The categorization is based
on a keyword mapping provided in a separate CSV file.

Author: Nicholas Antoniades
Date: 2024-10-26
"""

from lib import keyword_loader
from lib import data_loader
from lib import categorizer
from lib import report_preparer
from lib import result_saver

SAVE_MONTHLY = True
SAVE_WEEKLY = False
SAVE_ENTIRE = True

DIRECTORY = 'expense-categorization-and-analysis/files_to_categorize'
KEYWORD_FILE = 'expense-categorization-and-analysis/categorization_keywords.csv'
OUTPUT_DIRECTORY = 'expense-categorization-and-analysis/files_categorized'

class FinanceCategorizer:
    def __init__(self, directory, keyword_file, output_directory, save_monthly=True, save_weekly=False, save_entire=True):
        """
        Initialize the FinanceCategorizer with directory paths and save options.
        """
        self.data_loader = data_loader.DataLoader(directory)
        self.keyword_loader = keyword_loader.KeywordLoader(keyword_file)
        self.keyword_mapping = self.keyword_loader.load_keyword_mapping()
        self.categorizer = categorizer.Categorizer(self.keyword_mapping)
        self.report_preparer = report_preparer.ReportPreparer()
        self.result_saver = result_saver.ResultSaver(output_directory, save_monthly, save_weekly, save_entire)

    def run(self):
        """
        Execute the finance categorization process.
        """
        all_expenses = self.data_loader.load_and_preprocess_data()
        if all_expenses is None or self.keyword_mapping is None:
            return
        all_expenses = self.categorizer.finance_categorization(all_expenses)
        detailed_expenses = self.report_preparer.prepare_output(all_expenses)
        self.result_saver.save_results(detailed_expenses)

if __name__ == "__main__":
    print("Starting the finance categorization script...")
    categorizer = FinanceCategorizer(
        directory=DIRECTORY,
        keyword_file=KEYWORD_FILE,
        output_directory=OUTPUT_DIRECTORY,
        save_monthly=SAVE_MONTHLY,
        save_weekly=SAVE_WEEKLY,
        save_entire=SAVE_ENTIRE
    )
    categorizer.run()

"""
personal-finance/categorizer.py

This module contains the Categorizer class which categorizes financial
data based on keyword mappings.

Author: Nicholas Antoniades
Date: 2024-10-26
"""

class Categorizer:
    def __init__(self, keyword_mapping):
        self.keyword_mapping = keyword_mapping

    def match_keyword(self, text):
        """
        Match a text against the keyword mapping to find a category.
        """
        if text and text.lower() != 'nan':
            for keyword, category in self.keyword_mapping.items():
                if keyword.lower() in text.lower():
                    return category
        return None

    def categorize_row(self, row):
        """
        Categorize a single row based on its description, name, or transfers.
        """
        try:
            for field in ['Description', 'Name', 'Transfers']:
                category = self.match_keyword(str(row.get(field, '')).lower())
                if category:
                    return category
            return 'Uncategorized'
        except Exception as e:
            print(f"Error categorizing row: {row}\nError: {str(e)}")
            return 'Error'

    def finance_categorization(self, all_expenses):
        """
        Apply categorization to all expenses.
        """
        all_expenses['Category'] = all_expenses.apply(self.categorize_row, axis=1)
        error_rows = all_expenses[all_expenses['Category'] == 'Error']
        if not error_rows.empty:
            print(f"There were {len(error_rows)} rows that couldn't be categorized.")
        return all_expenses 
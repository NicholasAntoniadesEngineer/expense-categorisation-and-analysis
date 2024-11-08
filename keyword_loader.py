"""
personal-finance/keyword_loader.py

This module contains the KeywordLoader class which loads keyword mappings
from a CSV file for categorizing financial data.

Author: Nicholas Antoniades
Date: 2024-10-26
"""

import pandas as pd

class KeywordLoader:
    def __init__(self, keyword_file):
        self.keyword_file = keyword_file

    def load_keyword_mapping(self):
        """
        Load keyword mapping from a CSV file into a dictionary.
        """
        try:
            df = pd.read_csv(self.keyword_file)
            if df.empty:
                print("Keyword mapping file is empty.")
                return None
            return dict(zip(df['Keyword'].astype(str).str.lower(), df['Category']))
        except Exception as e:
            print(f"Error loading keyword mapping: {str(e)}")
            return None 
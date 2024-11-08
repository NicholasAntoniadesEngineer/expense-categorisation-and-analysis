"""
personal-finance/data_loader.py

This module contains the DataLoader class which loads and preprocesses
financial data from CSV files.

Author: Nicholas Antoniades
Date: 2024-10-26
"""

import os
from glob import glob
import pandas as pd

class DataLoader:
    def __init__(self, directory):
        self.directory = directory

    def get_file_origin(self, basename):
        """
        Extract the origin of the file from its basename.
        """
        parts = basename.split(' ')
        origin = [part[:-4] if part.lower().endswith('.csv') else part for part in parts if part not in ['-', 'Data', 'Export']]
        return ' '.join(origin)

    def load_and_preprocess_data(self):
        """
        Load and preprocess data from CSV files in the specified directory.
        """
        all_expenses = pd.DataFrame()
        csv_files = glob(os.path.join(self.directory, '*.csv'))

        for file in csv_files:
            df = pd.read_csv(file)
            df['FileOrigin'] = self.get_file_origin(os.path.basename(file))
            empty_desc_mask = df['Description'].isna() | (df['Description'] == '')
            numeric_desc_mask = df['Description'].str.replace(' ', '', regex=False).str.replace('.', '', regex=False).str.isnumeric().fillna(False)
            if 'Category' in df.columns:
                df.loc[empty_desc_mask, 'Description'] = df.loc[empty_desc_mask, 'Category']
            if 'amex' in file.lower() or 'american express' in file.lower():
                df['Amount'] = df['Amount'].apply(lambda x: -x)
            if 'Name' in df.columns:
                alphanumeric_pattern_mask = df['Description'].str.match(r'^[A-Za-z0-9]{7,}$', na=False)
                numeric_or_long_numbers_mask = numeric_desc_mask | df['Description'].str.count('\d').gt(5) | alphanumeric_pattern_mask
                df.loc[numeric_or_long_numbers_mask, 'Description'] = df.loc[numeric_or_long_numbers_mask, 'Name']
            all_expenses = pd.concat([all_expenses, df], ignore_index=True)

        if all_expenses.empty:
            print("No data found in the CSV files.")
            return None

        all_expenses['Date'] = pd.to_datetime(all_expenses['Date'], format='%d/%m/%Y', errors='coerce')
        all_expenses['Month'] = all_expenses['Date'].dt.to_period('M')
        all_expenses['Description'] = all_expenses['Description'].fillna('')
        all_expenses['Name'] = all_expenses['Name'].fillna('') if 'Name' in all_expenses.columns else ''
        savings_mask = all_expenses['Description'].str.lower() == 'savings'
        all_expenses.loc[savings_mask, 'Category'] = 'Savings'
        return all_expenses 
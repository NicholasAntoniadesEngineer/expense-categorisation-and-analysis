"""
personal-finance/finance.py

This script processes financial data from CSV files, categorizes expenses based on keywords,
removes duplicates, and generates detailed and summary reports. The categorization is based
on a keyword mapping provided in a separate CSV file.

Author: Nicholas Antoniades
Date: 2024-10-26
"""

import os
from glob import glob
import pandas as pd

SAVE_MONTHLY = True
SAVE_WEEKLY = False
SAVE_ENTIRE = True

DIRECTORY = 'personal-finance/files_to_categorize'
KEYWORD_FILE = 'personal-finance/categorization_keywords.csv'
OUTPUT_DIRECTORY = 'personal-finance/files_categorized'

class FinanceCategorizer:
    def __init__(self, directory, keyword_file, output_directory, save_monthly=True, save_weekly=False, save_entire=True):
        """
        Initialize the FinanceCategorizer with directory paths and save options.
        """
        self.directory = directory
        self.keyword_file = keyword_file
        self.output_directory = output_directory
        self.save_monthly = save_monthly
        self.save_weekly = save_weekly
        self.save_entire = save_entire
        self.keyword_mapping = self.load_keyword_mapping()

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

    def get_file_origin(self, basename):
        """
        Extract the origin of the file from its basename.
        """
        parts = basename.split(' ')
        origin = [part[:-4] if part.lower().endswith('.csv') else part for part in parts if part not in ['-', 'Data', 'Export']]
        return ' '.join(origin)

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

    def prepare_output(self, all_expenses):
        """
        Prepare the output data for saving.
        """
        output_columns = ['Date', 'Month', 'FileOrigin', 'Description', 'Amount', 'Category']
        detailed_expenses = all_expenses[output_columns].sort_values(by=['Date', 'Category'])
        detailed_expenses['Amount'] = detailed_expenses['Amount'].map(lambda x: f"{x:.2f}")
        return detailed_expenses

    def save_results(self, detailed_expenses):
        """
        Save the results to CSV files based on the save options.
        """
        if self.save_entire:
            output_filename = os.path.join(self.output_directory, f"{pd.Timestamp.now().strftime('%Y-%m-%d')}_finance.csv")
            detailed_expenses['Amount'] = pd.to_numeric(detailed_expenses['Amount'], errors='coerce')
            detailed_expenses.to_csv(output_filename, index=False)
            print(f"Results have been saved to {output_filename}")

        if self.save_monthly:
            self.save_monthly_summary(detailed_expenses)

        if self.save_weekly:
            self.save_weekly_summary(detailed_expenses)

    def save_monthly_summary(self, detailed_expenses):
        """
        Save monthly summary reports.
        """
        detailed_expenses['Month'] = detailed_expenses['Date'].dt.to_period('M')
        unique_months = detailed_expenses['Month'].unique()

        for month in unique_months:
            monthly_data = detailed_expenses[detailed_expenses['Month'] == month]
            monthly_summary = pd.pivot_table(
                monthly_data,
                values='Amount',
                index=['Month'],
                columns=['Category'],
                aggfunc='sum'
            ).reset_index().melt(
                id_vars=['Month'],
                var_name='Category',
                value_name='Amount'
            ).dropna()

            monthly_summary = monthly_summary[(monthly_summary['Category'] != 'Nothing') & (monthly_summary['Amount'] != 0)]
            total_amount = monthly_summary['Amount'].astype(float).sum()
            total_row = pd.DataFrame({'Month': [month], 'Category': ['Total'], 'Amount': [f"{total_amount:.2f}"]})
            monthly_summary = pd.concat([monthly_summary, total_row], ignore_index=True)
            monthly_summary['Amount'] = pd.to_numeric(monthly_summary['Amount'], errors='coerce')
            monthly_summary['Amount'] = monthly_summary['Amount'].map(lambda x: f"{x:.2f}")
            monthly_summary_filename = os.path.join(self.output_directory, f"{month}_monthly_summary.csv")
            monthly_summary.to_csv(monthly_summary_filename, index=False)
            print(f"Monthly summary for {month} has been saved to {monthly_summary_filename}")

    def save_weekly_summary(self, detailed_expenses):
        """
        Save weekly summary reports.
        """
        detailed_expenses['Week'] = detailed_expenses['Date'].dt.to_period('W').apply(lambda r: r.start_time)
        unique_weeks = detailed_expenses['Week'].unique()

        for week in unique_weeks:
            weekly_data = detailed_expenses[detailed_expenses['Week'] == week]
            weekly_summary = pd.pivot_table(
                weekly_data,
                values='Amount',
                index=['Week'],
                columns=['Category'],
                aggfunc='sum'
            ).reset_index().melt(
                id_vars=['Week'],
                var_name='Category',
                value_name='Amount'
            ).dropna()

            weekly_summary = weekly_summary[(weekly_summary['Category'] != 'Nothing') & (weekly_summary['Amount'] != 0)]
            weekly_summary['Amount'] = pd.to_numeric(weekly_summary['Amount'], errors='coerce')
            total_amount = weekly_summary['Amount'].sum()
            total_row = pd.DataFrame({'Week': [week], 'Category': ['Total'], 'Amount': [f"{total_amount:.2f}"]})
            weekly_summary = pd.concat([weekly_summary, total_row], ignore_index=True)
            weekly_summary['Amount'] = pd.to_numeric(weekly_summary['Amount'], errors='coerce')
            weekly_summary['Amount'] = weekly_summary['Amount'].map(lambda x: f"{x:.2f}")
            week_start_date = pd.to_datetime(week).strftime('%Y-%m-%d')
            weekly_summary_filename = os.path.join(self.output_directory, f"{week_start_date}_weekly_summary.csv")
            weekly_summary.to_csv(weekly_summary_filename, index=False)
            print(f"Weekly summary for {week_start_date} has been saved to {weekly_summary_filename}")

    def run(self):
        """
        Execute the finance categorization process.
        """
        all_expenses = self.load_and_preprocess_data()
        if all_expenses is None or self.keyword_mapping is None:
            return
        all_expenses = self.finance_categorization(all_expenses)
        detailed_expenses = self.prepare_output(all_expenses)
        self.save_results(detailed_expenses)


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

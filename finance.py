"""
personal-finance/finance.py

This script processes financial data from CSV files, categorizes expenses based on keywords,
removes duplicates, and generates detailed and summary reports. The categorization is based
on a keyword mapping provided in a separate CSV file.

Author: Nicholas Antoniades
Date: 2024-10-26
"""

import os
import logging
from glob import glob
import pandas as pd


def load_keyword_mapping(file_path):
    df = pd.read_csv(file_path)
    return dict(zip(df['Keyword'].astype(str).str.lower(), df['Category']))


def get_file_origin(basename):
    parts = basename.split(' ')
    origin = []
    i = 0
    while i < len(parts):
        if parts[i] not in ['-', 'Data', 'Export']:
            if parts[i].lower().endswith('.csv'):
                parts[i] = parts[i][:-4]
            origin.append(parts[i])
        i += 1
    return ' '.join(origin)


def match_keyword(text, keyword_mapping):
    if text and text.lower() != 'nan':
        for keyword, category in keyword_mapping.items():
            if keyword.lower() in text.lower():
                return category
    return None


def categorize_row(row, keyword_mapping):
    try:
        PRINT_TRUE = False 

        if PRINT_TRUE: print("====================================")
        
        # Try to categorize using Description
        description = (str(row.get('Description', '')).lower())
        if PRINT_TRUE:print(f"Checking Description:{description}")
        category = match_keyword(description, keyword_mapping)
        if category:
            if PRINT_TRUE: print(f"Matched Description with category: {category}")
            return category
        
        # Try to categorize using Name
        name = str(row.get('Name', '')).lower()
        if PRINT_TRUE: print(f"Checking Name: {name}")
        category = match_keyword(name, keyword_mapping)
        if category:
            if PRINT_TRUE: print(f"Matched Name with category: {category}")
            return category

        # Try to categorize using Transfers
        transfers = str(row.get('Transfers', '')).lower()
        if PRINT_TRUE: print(f"Checking Transfers: {transfers}")
        category = match_keyword(transfers, keyword_mapping)
        if category:
            if PRINT_TRUE: print(f"Matched Transfers with category: {category}")
            return category

        # Default to Uncategorized
        if PRINT_TRUE:    
            print(f"Checking Description:{description}")
            print(f"Checking Name: {name}")
            print(f"Checking Transfers: {transfers}")
            print("No category found, returning Uncategorized")
        return 'Uncategorized'
    except Exception as e:
        print(f"Error categorizing row: {row}\nError: {str(e)}")
        return 'Error'


def categorize_expenses(all_expenses, keyword_mapping):

    all_expenses['Category'] = all_expenses.apply(categorize_row, axis=1)

    error_rows = all_expenses[all_expenses['Category'] == 'Error']
    
    if not error_rows.empty:
        print(f"There were {len(error_rows)} rows that couldn't be categorized.")

    return all_expenses


def load_and_preprocess_data(directory, keyword_file):

    all_expenses = pd.DataFrame()
    
    csv_files = glob(os.path.join(directory, '*.csv'))

    for file in csv_files:
        
        df = pd.read_csv(file)
        
        file_basename = os.path.basename(file)
        
        df['FileOrigin'] = get_file_origin(file_basename)
        
        # Create mask for empty descriptions (either NaN or empty string)
        empty_desc_mask = df['Description'].isna() | (df['Description'] == '')
        
        numeric_desc_mask = df['Description'].str.replace(' ', '', regex=False).str.replace('.', '', regex=False).str.isnumeric()
        
        numeric_desc_mask = numeric_desc_mask.fillna(False)
                
        if 'Category' in df.columns:
            
            df.loc[empty_desc_mask, 'Description'] = df.loc[empty_desc_mask, 'Category']

        if 'amex' in file_basename.lower() or 'american express' in file_basename.lower():
            
            df['Amount'] = df['Amount'].apply(lambda x: -x)
        
        if 'Name' in df.columns:
            # Update the alphanumeric pattern to capture strings like 'T1253000070'
            alphanumeric_pattern_mask = df['Description'].str.match(r'^[A-Za-z0-9]{7,}$', na=False)
            
            numeric_or_long_numbers_mask = numeric_desc_mask | df['Description'].str.count('\d').gt(5) | alphanumeric_pattern_mask

            df.loc[numeric_or_long_numbers_mask, 'Description'] = df.loc[numeric_or_long_numbers_mask, 'Name']
            
        all_expenses = pd.concat([all_expenses, df], ignore_index=True)
        

    if all_expenses.empty:
        print("No data found in the CSV files.")
        return None

    date_column = 'Date'
    all_expenses[date_column] = pd.to_datetime(all_expenses[date_column], format='%d/%m/%Y', errors='coerce')
    all_expenses['Month'] = all_expenses[date_column].dt.to_period('M')
    all_expenses['Description'] = all_expenses['Description'].fillna('')
    all_expenses['Name'] = all_expenses['Name'].fillna('') if 'Name' in all_expenses.columns else ''

    savings_mask = all_expenses['Description'].str.lower() == 'savings'
    all_expenses.loc[savings_mask, 'Category'] = 'Savings'

    return all_expenses


# Prepare output data
def prepare_output(all_expenses):
    output_columns = ['Date', 'Month', 'FileOrigin', 'Description', 'Amount', 'Category']
    detailed_expenses = all_expenses[output_columns]
    detailed_expenses = detailed_expenses.sort_values(by=['Date', 'Category'])
    detailed_expenses['Amount'] = detailed_expenses['Amount'].map(lambda x: f"{x:.2f}")
    return detailed_expenses


# Save results to CSV files
def save_results(detailed_expenses):
    output_filename = f"{pd.Timestamp.now().strftime('%Y-%m-%d')}_finance.csv"
    detailed_expenses['Amount'] = pd.to_numeric(detailed_expenses['Amount'], errors='coerce')
    detailed_expenses.to_csv(output_filename, index=False)
    print(f"Results have been saved to {output_filename}")

    summary = pd.pivot_table(
        detailed_expenses,
        values='Amount',
        index=['Month'],
        columns=['Category'],
        aggfunc='sum'
    ).reset_index().melt(
        id_vars=['Month'],
        var_name='Category',
        value_name='Amount'
    ).dropna()

    # Remove the 'Nothing' category from the summary
    summary = summary[summary['Category'] != 'Nothing']

    summary['Amount'] = summary['Amount'].map(lambda x: f"{x:.2f}")
    summary_filename = f"{pd.Timestamp.now().strftime('%Y-%m-%d')}_summary.csv"
    summary.to_csv(summary_filename, index=False)
    print(f"Summary has been saved to {summary_filename}")


def main():
    directory = 'finance_files'
    keyword_file = 'keyword_mapping.csv'

    all_expenses = load_and_preprocess_data(directory, keyword_file)
    
    if all_expenses is None:
        print("No data found in the CSV files.")
        return
    
    keyword_mapping = load_keyword_mapping(keyword_file)

    all_expenses = categorize_expenses(all_expenses, keyword_mapping)
    
    detailed_expenses = prepare_output(all_expenses)
    
    save_results(detailed_expenses)

if __name__ == "__main__":
    print("Starting the finance categorization script...")
    main()

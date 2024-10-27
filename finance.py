import pandas as pd
import os
from glob import glob
import logging

def load_keyword_mapping(file_path):
    # Load the keyword mapping from a CSV file
    df = pd.read_csv(file_path)
    # Convert all keywords to strings to avoid type issues
    return dict(zip(df['Keyword'].astype(str).str.lower(), df['Category']))

# Define a helper function to check for keyword matches
def match_keyword(text, keyword_mapping):
    if text and text.lower() != 'nan':
        for keyword, category in keyword_mapping.items():
            if keyword.lower() in text.lower():
                return category
    return None

# Apply categorization with error handling
def safe_categorize(row):
    try:
        return categorize_expense(row)
    except Exception as e:
        logging.error(f"Error categorizing row: {row}\nError: {str(e)}")
        print(f"Error categorizing row: {row}\nError: {str(e)}")
        return 'Error'

# Function to categorize expenses
def categorize_expense(row):
    PRINT_TRUE = False  # Set to True to enable prints, False to disable

    if PRINT_TRUE:
        print("====================================")
        # print(f"Processing row: {row}")
    
    # Try to categorize using Description
    description = str(row.get('Description', '')).lower()
    if PRINT_TRUE: print(f"Checking Description: {description}")
    category = match_keyword(description, keyword_mapping)
    if category:
        if PRINT_TRUE: print(f"Matched Description with category: {category}")
        return category

    # Try to categorize using Transfers
    transfers = str(row.get('Transfers', '')).lower()
    if PRINT_TRUE: print(f"Checking Transfers: {transfers}")
    category = match_keyword(transfers, keyword_mapping)
    if category:
        if PRINT_TRUE: print(f"Matched Transfers with category: {category}")
        return category

    # Try to categorize using Name
    name = str(row.get('Name', '')).lower()
    if PRINT_TRUE: print(f"Checking Name: {name}")
    category = match_keyword(name, keyword_mapping)
    if category:
        if PRINT_TRUE: print(f"Matched Name with category: {category}")
        return category

    # If no category found, check for specific conditions
    if 'pot transfer' in description.lower() or 'pot transfer' in name.lower():
        if PRINT_TRUE: print("Matched 'pot transfer' condition")
        return 'Transfers'

    # If still no category found, return Uncategorized
    if PRINT_TRUE: print("No category found, returning Uncategorized")
    return 'Uncategorized'


def get_file_origin(basename):
    parts = basename.split(' ')
    origin = []
    i = 0
    while i < len(parts):
        if (parts[i] != '-' and 
            parts[i] != 'Data' and 
            parts[i] != 'Export'):
            # Keep 'july' but remove '.csv' extension
            if parts[i].lower().endswith('.csv'):
                parts[i] = parts[i][:-4]
            origin.append(parts[i])
        i += 1
    return ' '.join(origin)



def load_and_preprocess_data(directory, keyword_file):
    # Load keyword mapping from CSV
    global keyword_mapping
    keyword_mapping = load_keyword_mapping(keyword_file)

    # Add custom extra keyword mappings
    custom_mappings = {
        'nan': 'Transfers',
        # '': 'Transfers Category 2'
        # Add more custom mappings as needed
    }
    keyword_mapping.update(custom_mappings)

    # Initialize a DataFrame to store all expenses
    all_expenses = pd.DataFrame()

    # List and read all CSV files in the directory
    csv_files = glob(os.path.join(directory, '*.csv'))

    
    for file in csv_files:
        df = pd.read_csv(file)
        # Skip rows where Description is "PAYMENT RECEIVED - THANK YOU"
        df = df[df['Description'] != 'PAYMENT RECEIVED - THANK YOU']
        
        file_basename = os.path.basename(file)
        df['FileOrigin'] = get_file_origin(file_basename)
        
        # If Description is empty or just numbers/spaces, use Category or Name as Description
        empty_desc_mask = df['Description'].isna() | (df['Description'] == '')
        numeric_desc_mask = df['Description'].str.replace(' ', '', regex=False).str.replace('.', '', regex=False).str.isnumeric()

        # Fill NaN values in the mask with False
        numeric_desc_mask = numeric_desc_mask.fillna(False)
        if 'Category' in df.columns:
            df.loc[empty_desc_mask, 'Description'] = df.loc[empty_desc_mask, 'Category']
        
        if 'Name' in df.columns:
            # Check for numeric descriptions or descriptions with more than 8 numbers
            numeric_or_long_numbers_mask = numeric_desc_mask | df['Description'].str.count('\d').gt(8)
            df.loc[numeric_or_long_numbers_mask, 'Description'] = df.loc[numeric_or_long_numbers_mask, 'Name']
            
        # Make Amex/American Express values negative since they are expenses
        if 'amex' in file_basename.lower() or 'american express' in file_basename.lower():
            df['Amount'] = df['Amount'].apply(lambda x: -x)
        all_expenses = pd.concat([all_expenses, df], ignore_index=True)

    if all_expenses.empty:
        print("No data found in the CSV files.")
        return None

    # Preprocess the data
    date_column = 'Date'  # Default column name
    all_expenses[date_column] = pd.to_datetime(all_expenses[date_column], format='%d/%m/%Y', errors='coerce')
    all_expenses['Month'] = all_expenses[date_column].dt.to_period('M')
    all_expenses['Description'] = all_expenses['Description'].fillna('')
    all_expenses['Name'] = all_expenses['Name'].fillna('') if 'Name' in all_expenses.columns else ''

    # Ensure transactions with Description "Savings" keep their Category as "Savings"
    savings_mask = all_expenses['Description'].str.lower() == 'savings'
    all_expenses.loc[savings_mask, 'Category'] = 'Savings'

    return all_expenses
def categorize_expenses(all_expenses):
    logging.basicConfig(filename='finance_log.txt', level=logging.ERROR)
    all_expenses['Category'] = all_expenses.apply(safe_categorize, axis=1)

    error_rows = all_expenses[all_expenses['Category'] == 'Error']
    if not error_rows.empty:
        print(f"There were {len(error_rows)} rows that couldn't be categorized. Check finance_log.txt for details.")

    return all_expenses

def remove_duplicates(all_expenses):
    duplicates = all_expenses.duplicated(subset=['Date', 'Amount', 'Description'], keep=False)
    duplicate_entries = all_expenses[duplicates]
    all_expenses = all_expenses.drop_duplicates(subset=['Date', 'Amount', 'Description'])

    if not duplicate_entries.empty:
        removed_filename = f"{pd.Timestamp.now().strftime('%Y-%m-%d')}_removed_items.csv"
        duplicate_entries.to_csv(removed_filename, index=False)
        print(f"{len(duplicate_entries)} duplicate entries were removed. Details saved to {removed_filename}")

    return all_expenses

def prepare_output(all_expenses):
    output_columns = ['Date', 'Month', 'FileOrigin', 'Description', 'Amount', 'Category']
    detailed_expenses = all_expenses[output_columns]
    detailed_expenses = detailed_expenses.sort_values(by=['Date', 'Category'])
    detailed_expenses['Amount'] = detailed_expenses['Amount'].map(lambda x: f"{x:.2f}")
    return detailed_expenses

def save_results(detailed_expenses):
    # Save the detailed expenses without monthly totals
    output_filename = f"{pd.Timestamp.now().strftime('%Y-%m-%d')}_finance.csv"
    
    # Convert Amount to numeric for calculations
    detailed_expenses['Amount'] = pd.to_numeric(detailed_expenses['Amount'], errors='coerce')
    
    # Save the detailed expenses directly to CSV
    detailed_expenses.to_csv(output_filename, index=False)
    print(f"Results have been saved to {output_filename}")

    # Create a summary of total amounts per category for each month
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

    # Format the summary for better readability
    summary['Amount'] = summary['Amount'].map(lambda x: f"{x:.2f}")

    # Save the summary to a new CSV file
    summary_filename = f"{pd.Timestamp.now().strftime('%Y-%m-%d')}_summary.csv"
    summary.to_csv(summary_filename, index=False)
    print(f"Summary has been saved to {summary_filename}")

def main():
    directory = 'finance_files'
    keyword_file = 'keyword_mapping.csv'

    all_expenses = load_and_preprocess_data(directory, keyword_file)
    if all_expenses is None:
        return

    all_expenses = categorize_expenses(all_expenses)
    # all_expenses = remove_duplicates(all_expenses)
    detailed_expenses = prepare_output(all_expenses)

    print(detailed_expenses)
    save_results(detailed_expenses)

if __name__ == "__main__":
    print("Starting the finance categorization script...")
    main()

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
    PRINT_TRUE = True  # Set to True to enable prints, False to disable

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



def load_and_preprocess_data(directory, keyword_file):
    # Load keyword mapping from CSV
    global keyword_mapping
    keyword_mapping = load_keyword_mapping(keyword_file)

    # Add custom extra keyword mappings
    custom_mappings = {
        'nan': 'Transfers Category 1',
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
        file_origin = os.path.basename(file).split(' ')[0]
        df['FileOrigin'] = file_origin
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
    output_filename = f"{pd.Timestamp.now().strftime('%Y-%m-%d')}_finance.csv"
    detailed_expenses.to_csv(output_filename, index=False)
    print(f"Results have been saved to {output_filename}")

def main():
    directory = 'finance_files'
    keyword_file = 'keyword_mapping.csv'

    all_expenses = load_and_preprocess_data(directory, keyword_file)
    if all_expenses is None:
        return

    all_expenses = categorize_expenses(all_expenses)
    all_expenses = remove_duplicates(all_expenses)
    detailed_expenses = prepare_output(all_expenses)

    print(detailed_expenses)
    save_results(detailed_expenses)

if __name__ == "__main__":
    print("Starting the finance categorization script...")
    main()

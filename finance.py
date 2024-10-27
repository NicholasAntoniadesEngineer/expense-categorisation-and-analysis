import pandas as pd
import os
from glob import glob
import logging

def load_keyword_mapping(file_path):
    # Load the keyword mapping from a CSV file
    df = pd.read_csv(file_path)
    # Convert all keywords to strings to avoid type issues
    return dict(zip(df['Keyword'].astype(str).str.lower(), df['Category']))

# Function to categorize expenses
def categorize_expense(row):
            
    # Define a helper function to check for keyword matches
    def match_keyword(text, keyword_mapping):
        if text and text.lower() != 'nan':
            for keyword, category in keyword_mapping.items():
                if keyword.lower() in text.lower():
                    return category
        return None

    # Try to categorize using Description
    description = str(row.get('Description', '')).lower()
    category = match_keyword(description, keyword_mapping)
    if category:
        return category

    # Try to categorize using Transfers
    transfers = str(row.get('Transfers', '')).lower()
    category = match_keyword(transfers, keyword_mapping)
    if category:
        return category

    # Try to categorize using Name
    name = str(row.get('Name', '')).lower()
    category = match_keyword(name, keyword_mapping)
    if category:
        return category

    # If no category found, check for specific conditions
    if 'pot transfer' in description.lower() or 'pot transfer' in name.lower():
        return 'Transfers'

    # If still no category found, return Uncategorized
    return 'Uncategorized'

# Apply categorization with error handling
def safe_categorize(row):
    try:
        return categorize_expense(row)
    except Exception as e:
        logging.error(f"Error categorizing row: {row}\nError: {str(e)}")
        print(f"Error categorizing row: {row}\nError: {str(e)}")
        return 'Error'

def main():
    # Define the directory containing the CSV files
    directory = 'finance_files'
    keyword_file = 'keyword_mapping.csv'

    # Load keyword mapping from CSV
    global keyword_mapping
    keyword_mapping = load_keyword_mapping(keyword_file)

    # Add custom extra keyword mappings
    custom_mappings = {
        'nan': 'Transfers Category 1',
        # '': 'Transfers Category 2'
        # Add more custom mappings as needed
    }

    # Update the keyword_mapping with custom mappings
    keyword_mapping.update(custom_mappings)

    # Initialize a DataFrame to store all expenses
    all_expenses = pd.DataFrame()

    # List all CSV files in the directory
    csv_files = glob(os.path.join(directory, '*.csv'))

    # Read all CSV files in the directory
    for file in csv_files:
        df = pd.read_csv(file)
        
        # Extract the first word of the file name (without extension) for the FileOrigin column
        file_origin = os.path.basename(file).split(' ')[0]
        df['FileOrigin'] = file_origin
        
        all_expenses = pd.concat([all_expenses, df], ignore_index=True)

    # Check if the DataFrame is empty
    if all_expenses.empty:
        print("No data found in the CSV files.")
        return

    # Check for the correct column name for date
    date_column = 'Date'  # Default column name

    # Convert the date column to datetime with a specified format
    all_expenses[date_column] = pd.to_datetime(all_expenses[date_column], format='%d/%m/%Y', errors='coerce')

    # Add a 'Month' column for grouping
    all_expenses['Month'] = all_expenses[date_column].dt.to_period('M')

    # Ensure 'Description' column is filled with empty strings where there are NaN values
    all_expenses['Description'] = all_expenses['Description'].fillna('')
    # Ensure 'Name' column exists and fill NaN values with empty strings
    if 'Name' in all_expenses.columns:
        all_expenses['Name'] = all_expenses['Name'].fillna('')
    else:
        all_expenses['Name'] = ''

    # Set up logging
    logging.basicConfig(filename='finance_log.txt', level=logging.ERROR)

    all_expenses['Category'] = all_expenses.apply(safe_categorize, axis=1)

    # After categorization, check for any errors
    error_rows = all_expenses[all_expenses['Category'] == 'Error']
    if not error_rows.empty:
        print(f"There were {len(error_rows)} rows that couldn't be categorized. Check finance_log.txt for details.")

    # Remove duplicate entries
    duplicates = all_expenses.duplicated(subset=['Date', 'Amount', 'Description'], keep=False)
    duplicate_entries = all_expenses[duplicates]
    all_expenses = all_expenses.drop_duplicates(subset=['Date', 'Amount', 'Description'])

    # Save removed duplicates to a CSV file
    if not duplicate_entries.empty:
        removed_filename = f"{pd.Timestamp.now().strftime('%Y-%m-%d')}_removed_items.csv"
        duplicate_entries.to_csv(removed_filename, index=False)
        print(f"{len(duplicate_entries)} duplicate entries were removed. Details saved to {removed_filename}")

    # Define the order of columns for the output
    output_columns = ['Date', 'Month', 'FileOrigin', 'Description', 'Amount', 'Category']

    # Merge the individual entries with their categories and months
    detailed_expenses = all_expenses[output_columns]

    # Sort the DataFrame by Date and Category
    detailed_expenses = detailed_expenses.sort_values(by=['Date', 'Category'])

    # Format the Amount column to two decimal places
    detailed_expenses['Amount'] = detailed_expenses['Amount'].map(lambda x: f"{x:.2f}")

    # Display the result
    print(detailed_expenses)

    # Output the results to a CSV file with the name finance_{the_date}.csv
    output_filename = f"{pd.Timestamp.now().strftime('%Y-%m-%d')}_finance.csv"
    detailed_expenses.to_csv(output_filename, index=False)
    print(f"Results have been saved to {output_filename}")

if __name__ == "__main__":
    print("Starting the finance categorization script...")
    main()

import pandas as pd
import os
from glob import glob

def load_keyword_mapping(file_path):
    # Load the keyword mapping from a CSV file
    df = pd.read_csv(file_path)
    return dict(zip(df['Keyword'].str.lower(), df['Category']))

def main():
    # Define the directory containing the CSV files
    directory = 'finance_files'
    keyword_file = 'keyword_mapping.csv'

    # Load keyword mapping from CSV
    keyword_mapping = load_keyword_mapping(keyword_file)

    # Add custom extra keyword mappings
    custom_mappings = {
        'nan': 'Transfers Category 1',
        '': 'Transfers Category 2'
        # Add more custom mappings as needed
    }

    # Update the keyword_mapping with custom mappings
    keyword_mapping.update(custom_mappings)

    # Initialize a DataFrame to store all expenses
    all_expenses = pd.DataFrame()

    # List all CSV files in the directory
    csv_files = glob(os.path.join(directory, '*.csv'))
    # print("CSV files found:", csv_files)

    # Read all CSV files in the directory
    for file in csv_files:
        df = pd.read_csv(file)
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

    # Function to categorize expenses
    def categorize_expense(row):
        # Convert the description to a string and handle NaN values
        description = str(row['Description']).lower()
        for keyword, category in keyword_mapping.items():
            if keyword in description:
                return category
        return 'Uncategorized'

    # Apply categorization
    all_expenses['Category'] = all_expenses.apply(categorize_expense, axis=1)

    # Define the order of columns for the output
    output_columns = ['Date', 'Description', 'Amount', 'Category', 'Month']
 
    # Merge the individual entries with their categories and months
    detailed_expenses = all_expenses[output_columns]

    # Sort the DataFrame by Date and Category
    detailed_expenses = detailed_expenses.sort_values(by=['Date', 'Category'])

    # Format the Amount column to two decimal places
    detailed_expenses['Amount'] = detailed_expenses['Amount'].map(lambda x: f"{x:.2f}")

    # Display the result
    print(detailed_expenses)

    # Output the results to a CSV file with the name finance_{the_date}.csv
    output_filename = f"finance_{pd.Timestamp.now().strftime('%Y-%m-%d')}.csv"
    detailed_expenses.to_csv(output_filename, index=False)
    print(f"Results have been saved to {output_filename}")

if __name__ == "__main__":
    print("Starting the finance categorization script...")
    main()
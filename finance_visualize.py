import os
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np

# Constants for file paths
CATEGORIZED_DIRECTORY = 'personal-finance/files_categorized'

def load_categorized_data(file_path):
    return pd.read_csv(file_path)

def visualize_data(data, file_name):
    # Ensure 'Amount' is numeric
    data['Amount'] = pd.to_numeric(data['Amount'], errors='coerce')

    # Group by category and sum the amounts
    category_totals = data.groupby('Category')['Amount'].sum().reset_index()

    # Exclude the "Nothing" category
    category_totals = category_totals[category_totals['Category'] != 'Nothing']

    # Check if "Total" is already in the data
    if 'Total' in category_totals['Category'].values:
        total_amount = category_totals.loc[category_totals['Category'] == 'Total', 'Amount'].values[0]
        category_totals = category_totals[category_totals['Category'] != 'Total']
    else:
        # Calculate total amount if not present
        total_amount = category_totals['Amount'].sum()

    # Sort categories by total amount
    category_totals = category_totals.sort_values(by='Amount', ascending=False)

    # Add a row for the total amount at the end
    total_row = pd.DataFrame({'Category': ['Total'], 'Amount': [total_amount]})
    category_totals = pd.concat([category_totals, total_row], ignore_index=True)

    # Print calculated values
    print(f"Category Totals for {file_name}:")
    print(category_totals)

    # Plot
    plt.figure(figsize=(12, 8))
    sns.barplot(x='Amount', y='Category', data=category_totals, palette='viridis')
    plt.title(f'Total Amount by Category for {file_name}')
    plt.xlabel('Total Amount')
    plt.ylabel('Category')

    # Set detailed increments on the x-axis
    max_amount = category_totals['Amount'].max()
    min_amount = category_totals['Amount'].min()
    increment = round(max_amount * 0.1, -1)  # Round to the nearest 10

    # Set x-axis limits based on actual min and max values
    plt.xlim(min_amount - increment, max_amount + increment)
    plt.xticks(np.arange(min_amount, max_amount + increment, increment))

    # Rotate x-axis labels to be more perpendicular
    plt.xticks(rotation=45)

    plt.tight_layout()
    plt.show()

def main():
    csv_files = [f for f in os.listdir(CATEGORIZED_DIRECTORY) if f.endswith('.csv')]
    
    if not csv_files:
        print("No categorized data found.")
        return
    
    for file in csv_files:
        file_path = os.path.join(CATEGORIZED_DIRECTORY, file)
        categorized_data = load_categorized_data(file_path)
        visualize_data(categorized_data, file)

if __name__ == "__main__":
    print("Starting the finance visualization script...")
    main()

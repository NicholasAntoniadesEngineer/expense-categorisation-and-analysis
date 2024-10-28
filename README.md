# Finance Categorization Script

This Python script processes finance CSV files to categorize expenses based on keywords, removes duplicates, and generates categorized summaries. The script is designed for financial data preprocessing, including categorization, error handling, and reporting.

##Bugs
- The implementation needs to be able to handle monzo and amex, monzo only and amex only.
  Currently monzo payments to amex, and actual amex expenses clash
  - Amex repayments should cancel each other out if included
  - if only monzo amex repayment indicate all amex expenses

## Features

- **Expense Categorization**: Matches keywords from the `keyword_mapping.csv` file to categorize transactions.
- **Data Preprocessing**: Handles missing descriptions, converts American Express transactions to negative values, and merges all files from a directory.
- **Duplicate Removal**: Optionally identifies and removes duplicate entries.
- **Summarized Output**: Generates a monthly summary report of total expenses per category and saves categorized details.

## Prerequisites

- Python 3.x
- `pandas` library (install via `pip install pandas`)

## Setup

1. **Directory Structure**:
   - Place all finance CSV files in a directory named `finance_files`.
   - Include a `keyword_mapping.csv` file with two columns: `Keyword` and `Category`.

2. **Configure Keyword Mapping**:
   - `keyword_mapping.csv` should contain keywords (e.g., "rent", "utilities") with associated categories.

3. **Optional Logging**:
   - Errors are logged in `finance_log.txt` for rows that cannot be categorized.

## Usage

Run the script using:

```bash
python finance_categorization.py

# Finance Categorisation and Visualization Scripts (2024) ARCHIVED 

This project consists of Python scripts designed to categorise expenses based on keywords, remove duplicates, generate categorised summaries, and visualise the categorized data.

## Features

- **Expense Categorization**: Matches keywords from the `categorization_keywords.csv` file to categorize transactions.
- **Data Preprocessing**: Handles missing descriptions, converts American Express transactions to negative values, and merges all files from a directory.
- **Summarized Output**: Generates monthly and weekly summary reports of total expenses per category and saves categorized details.
- **Data Visualization**: visualises categorized data using bar plots to show total amounts by category.

## Prerequisites

- Python 3.x
- `pandas` library (install via `pip install pandas`)
- `matplotlib` library (install via `pip install matplotlib`)
- `seaborn` library (install via `pip install seaborn`)
- `numpy` library (install via `pip install numpy`)

## Setup

1. **Directory Structure**:
   - Place all finance CSV files in a directory named `files_to_categorize`.
   - Include a `categorization_keywords.csv` file with two columns: `Keyword` and `Category`.

2. **Configure Keyword Mapping**:
   - `categorization_keywords.csv` should contain keywords (e.g., "rent", "utilities") with associated categories.

3. **Optional Logging**:
   - Errors are logged in `finance_log.txt` for rows that cannot be categorized.

## Usage

### Categorization

Run the categorization script using:

```bash
python finance_categorization.py
```

### Visualization

Run the visualization script using:

```bash
python finance_visualise.py
```

## Notes

- The categorization script processes all CSV files in the `files_to_categorize` directory and outputs categorized files in the `files_categorized` directory.
- The visualization script generates bar plots for each categorized file, excluding the "Nothing" category, and displays the total amount by category.

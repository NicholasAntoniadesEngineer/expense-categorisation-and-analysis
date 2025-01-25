# Expense Categorisation and Analysis (2025)

A C++ application for automated expense categorisation and financial analysis. This tool processes financial transaction data from CSV files, automatically categorises expenses using keyword matching, and generates detailed financial reports.

## Features

- **Transaction Processing**
  - Supports multiple CSV formats with automatic column detection
  - Handles multiple currencies (GBP, EUR, USD)
  - Robust date and amount parsing
  - Smart transaction categorisation using keyword matching

- **Analysis & Reporting**
  - Monthly and weekly expense summaries
  - Full dataset exports
  - Category-based analysis
  - Support for multiple input sources

- **User Interface**
  - Modern Qt-based GUI
  - Directory selection for input/output
  - Configurable export options
  - Command-line interface available

## Project Structure

```
.
├── code/                # Source code
│   ├── app/             # Main application
│   ├── lib/             # Core libraries
│   │   ├── finance/     # Finance processing logic
│   │   └── gui/         # Qt GUI components
│   ├── resources/       # Application resources
│   └── build/           # Build artifacts
├── config/              # Configuration files
├── input_files/         # Input transaction files
├── output_files/        # Generated reports
└── scripts/             # Build and run scripts
```

## Dependencies

- C++17 or later
- Qt Framework
- CMake (3.x or later)

## Building

```bash
# Clone the repository
git clone [repository-url]
cd expense-categorisation-and-analysis-cpp

# Build the project
./scripts/build.sh

# Run the application
./scripts/run.sh
```

## Usage

1. Place your transaction CSV files in the `input_files` directory
2. Configure categorisation keywords in `config/categorisation_keywords.csv`
3. Run the application and select:
   - Input directory (containing CSV files)
   - Output directory (for reports)
   - Export options (monthly/weekly summaries, full dataset)

## Transaction Categorisation

The application uses keyword-based matching to categorise transactions. Categories include:
- Activities
- Credit Card
- Entertainment
- Groceries
- Shopping
- Subscriptions
- Transportation
- Utilities
- And more...

Keywords can be configured in `config/categorisation_keywords.csv`.

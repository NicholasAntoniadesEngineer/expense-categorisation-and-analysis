# Expense Categorisation and Analysis (2025)

A desktop application for automated expense categorisation and financial analysis. This tool processes financial transaction data from CSV files, automatically categorises expenses using keyword matching, and provides interactive visualizations and detailed financial reports.

## Features

- **Transaction Processing**
  - CSV file processing with automatic column detection
  - Intelligent transaction categorisation using keyword matching
  - Support for multiple input files
  - Robust error handling and validation

- **Analysis & Visualization**
  - Interactive weekly and monthly expense summaries
  - Category-based line charts with real-time data points
  - Detailed transaction tables with sorting and filtering
  - Multiple visualization windows for comparative analysis

- **Modern User Interface**
  - Dark-themed Qt-based GUI
  - Intuitive directory and file selection
  - Configurable export options
  - Interactive plot controls with category toggles
  - Responsive window management

## Project Structure

```
.
├── code/
│   ├── app/                   # Application code
│   │   ├── inc/               # Headers for UI components
│   │   │   ├── *_window.hpp   # Window components (main, plot, table)
│   │   │   ├── *_manager.hpp  # Component managers (UI, visualization, chart)
│   │   │   └── app_config.hpp # Application configuration
│   │   ├── src/               # Component implementations
│   │   └── styles/            # QSS stylesheets
│   ├── lib/                   # Core library
│   │   ├── inc/               # Processing and data handling headers
│   │   │   ├── finance_*.hpp  # Financial processing components
│   │   │   └── data_*.hpp     # Data handling components
│   │   └── src/               # Library implementations
│   └── resources/             # Application resources
├── scripts/                   # Build and utility scripts
├── config/                    # Configuration files
├── input_files/               # Input data directory
└── output_files/              # Generated reports and data
```

## Scripts

The application includes several utility scripts in the `scripts/` directory:

- **build.sh**
  ```bash
  ./scripts/build.sh [debug|release]  # Build with debug/release configuration
  ./scripts/build.sh clean            # Clean and rebuild
  ./scripts/build.sh test             # Build and run tests
  ```

- **clean.sh**
  ```bash
  ./scripts/clean.sh          # Remove build artifacts
  ./scripts/clean.sh all      # Remove build and generated files
  ```

- **run.sh**
  ```bash
  ./scripts/run.sh            # Run the application
  ./scripts/run.sh debug      # Run with debug output
  ```

- **format.sh**
  ```bash
  ./scripts/format.sh         # Format all source files
  ./scripts/format.sh check   # Check formatting only
  ```

## Key Components

- **Main Window**: Central interface for file selection and processing
- **Plot Window**: Interactive charts for financial data visualization
- **Table Window**: Detailed transaction and summary views
- **Finance Processor**: Core logic for transaction processing
- **Visualization Manager**: Handles multiple visualization windows
- **UI Manager**: Consistent styling and window management

## Dependencies

- C++17 or later
- Qt 6 (Core, Widgets, Charts)
- CMake 3.16 or later

## Usage

1. Launch the application
2. Configure input/output locations:
   - Select input directory containing CSV files
   - Choose output directory for reports
   - Select keyword file for categorisation
3. Choose export options:
   - Weekly Summary
   - Monthly Summary
   - Full Dataset
4. Click "Process Files" to analyze data
5. Use visualization tools:
   - Plot weekly/monthly summaries
   - View detailed transaction tables
   - Toggle category visibility
   - Interact with data points

## Transaction Categories

The application supports flexible category definitions through the keyword configuration file. Default categories include:
- Activities & Entertainment
- Bills & Utilities
- Credit Card Payments
- Groceries & Food
- Shopping
- Subscriptions
- Transportation
- And more...

Categories can be customized by editing `config/categorisation_keywords.csv`.

## Window Management

The application supports multiple visualization windows:
- Main control window
- Weekly and monthly summary plots
- Transaction tables
- Category visibility panels

Windows can be positioned and managed independently, with automatic cleanup when closed.

## Future Enhancements

- Currency conversion support
- Custom category creation
- Data export formats
- Advanced filtering options
- Report templates

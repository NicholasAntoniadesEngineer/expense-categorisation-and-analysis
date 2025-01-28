/**
 * @file main_window.cpp
 * @brief Implementation of the Finance Manager main window
 * @author Nicholas Antoniades
 * @date 2024-01-24
 * 
 * This file implements the main GUI window functionality for the
 * Finance Manager application, handling user interactions and file processing.
 */

#include "main_window.hpp"
#include "finance_processor.hpp"
#include "chart_manager.hpp"
#include "window_manager.hpp"
#include "plot_manager.hpp"
#include "table_window.hpp"
#include <functional>
#include "ui_manager.hpp"
#include "file_dialog_manager.hpp"
#include "visualization_manager.hpp"

namespace FinanceManager {

// Constructor and Initialization
MainWindow::MainWindow(AppConfig& config, QWidget *parent)
    : QMainWindow(parent)
    , config(config)
{
    // Initialize window-specific strings
    config.strings = {
        .INPUT_DIR_TITLE = "Input Directory",
        .OUTPUT_DIR_TITLE = "Output Directory",
        .KEYWORD_FILE_TITLE = "Keyword File",
        .EXPORT_OPTIONS_TITLE = "Export Options",
        .BROWSE_BUTTON_TEXT = "Browse",
        .PROCESS_BUTTON_TEXT = "Process Files",
        .MONTHLY_SUMMARY_TEXT = "Export Monthly Summary",
        .WEEKLY_SUMMARY_TEXT = "Export Weekly Summary",
        .FULL_DATASET_TEXT = "Export Full Dataset",
        .SELECT_INPUT_DIR_TEXT = "Select Input Directory",
        .SELECT_OUTPUT_DIR_TEXT = "Select Output Directory",
        .SELECT_KEYWORD_FILE_TEXT = "Select Keyword File",
        .CSV_FILE_FILTER = "CSV Files (*.csv);;All Files (*)",
        .ERROR_TITLE = "Error",
        .SUCCESS_TITLE = "Success",
        .FIELDS_REQUIRED_ERROR = "All fields must be filled",
        .PROCESS_SUCCESS = "Files processed successfully!",
        .PLOT_WEEKLY_TEXT = "Plot Weekly Summary",
        .PLOT_MONTHLY_TEXT = "Plot Monthly Summary"
    };

    setupUi();
    createConnections();
}

// Window Setup and Configuration
bool MainWindow::initializeApplicationInfo() noexcept {
    return UIManager::initializeApplicationInfo(config);
}

bool MainWindow::initializeAppearance() noexcept {
    return UIManager::initializeAppearance(config);
}

bool MainWindow::setupWindow() noexcept {
    return UIManager::setupMainWindow(this, config);
}

// UI Setup
void MainWindow::setupUi() {
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    mainLayout = new QVBoxLayout(centralWidget);
    
    // Create input directory group using UIManager
    auto inputGroup = UIManager::createDirectoryGroup(
        config.strings.INPUT_DIR_TITLE,
        config.strings.BROWSE_BUTTON_TEXT,
        this
    );
    inputDirEdit = inputGroup.pathEdit;
    inputBrowseButton = inputGroup.browseButton;
    mainLayout->addWidget(inputGroup.group);
    
    // Create output directory group using UIManager
    auto outputGroup = UIManager::createDirectoryGroup(
        config.strings.OUTPUT_DIR_TITLE,
        config.strings.BROWSE_BUTTON_TEXT,
        this
    );
    outputDirEdit = outputGroup.pathEdit;
    outputBrowseButton = outputGroup.browseButton;
    mainLayout->addWidget(outputGroup.group);
    
    // Create keyword file group using UIManager
    auto keywordGroup = UIManager::createDirectoryGroup(
        config.strings.KEYWORD_FILE_TITLE,
        config.strings.BROWSE_BUTTON_TEXT,
        this
    );
    keywordFileEdit = keywordGroup.pathEdit;
    keywordBrowseButton = keywordGroup.browseButton;
    mainLayout->addWidget(keywordGroup.group);
    
    // Create export options group using UIManager
    auto exportGroup = UIManager::createExportGroup(
        config.strings.EXPORT_OPTIONS_TITLE,
        config,
        this
    );
    exportMonthlySummaryCheck = exportGroup.monthlyCheck;
    exportWeeklySummaryCheck = exportGroup.weeklyCheck;
    exportFullDatasetCheck = exportGroup.fullDatasetCheck;
    mainLayout->addWidget(exportGroup.group);
    
    // Create action buttons using UIManager
    processButton = UIManager::createActionButton(config.strings.PROCESS_BUTTON_TEXT, this);
    processButton->setFixedSize(600, 30); 
    
    plotWeeklyButton = UIManager::createActionButton(config.strings.PLOT_WEEKLY_TEXT, this);
    plotMonthlyButton = UIManager::createActionButton(config.strings.PLOT_MONTHLY_TEXT, this);
    
    // Add process button to layout
    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch(1);  
    buttonLayout->addWidget(processButton);
    buttonLayout->addStretch(1);  
    mainLayout->addLayout(buttonLayout);
    mainLayout->addStretch();
    
    // Create visualization buttons
    VisualizationManager::VisualizationButtons visualizationButtons{
        .plotWeeklyButton = plotWeeklyButton,
        .plotMonthlyButton = plotMonthlyButton,
        .viewWeeklySummaryButton = new QPushButton("View Weekly Summary", this),
        .viewMonthlySummaryButton = new QPushButton("View Monthly Summary", this),
        .viewAllTransactionsButton = new QPushButton("View All Transactions", this)
    };
    
    // Create visualization group using VisualizationManager
    QGroupBox* visualizationGroup = VisualizationManager::createVisualizationGroup(
        this, visualizationButtons);
    mainLayout->addWidget(visualizationGroup);
    
    // Set default paths and states
    setupDefaultPaths();
    setupDefaultStates();

    // Connect summary buttons
    connect(visualizationButtons.viewWeeklySummaryButton, &QPushButton::clicked, 
            this, &MainWindow::viewWeeklySummary);
    connect(visualizationButtons.viewMonthlySummaryButton, &QPushButton::clicked, 
            this, &MainWindow::viewMonthlySummary);
    connect(visualizationButtons.viewAllTransactionsButton, &QPushButton::clicked, 
            this, &MainWindow::viewAllTransactions);
}

void MainWindow::setupDefaultPaths() {
    // Navigate from source file location to repository root
    QDir appDir(QString::fromUtf8(__FILE__));
    for (int i = 0; i < 4; ++i) {
        appDir.cdUp();
    }
    
    // Set default values with paths relative to repository root
    inputDirEdit->setText(appDir.absoluteFilePath("input_files"));
    outputDirEdit->setText(appDir.absoluteFilePath("output_files"));
    keywordFileEdit->setText(appDir.absoluteFilePath("config/categorisation_keywords.csv"));
}

void MainWindow::setupDefaultStates() {
    exportMonthlySummaryCheck->setChecked(true);
    exportWeeklySummaryCheck->setChecked(true);
    exportFullDatasetCheck->setChecked(true);
}

void MainWindow::createConnections() {
    connect(inputBrowseButton, &QPushButton::clicked, this, &MainWindow::browseInputDirectory);
    connect(outputBrowseButton, &QPushButton::clicked, this, &MainWindow::browseOutputDirectory);
    connect(keywordBrowseButton, &QPushButton::clicked, this, &MainWindow::browseKeywordFile);
    connect(processButton, &QPushButton::clicked, this, &MainWindow::processFiles);
    connect(plotWeeklyButton, &QPushButton::clicked, this, &MainWindow::plotWeeklySummary);
    connect(plotMonthlyButton, &QPushButton::clicked, this, &MainWindow::plotMonthlySummary);
}

// Event Handlers
void MainWindow::browseInputDirectory() {
    FileDialogManager::browseForDirectory(
        this,
        config.strings.SELECT_INPUT_DIR_TEXT,
        inputDirEdit->text(),
        inputDirectory,
        inputDirEdit
    );
}

void MainWindow::browseOutputDirectory() {
    FileDialogManager::browseForDirectory(
        this,
        config.strings.SELECT_OUTPUT_DIR_TEXT,
        outputDirEdit->text(),
        outputDirectory,
        outputDirEdit
    );
}

void MainWindow::browseKeywordFile() {
    FileDialogManager::browseForFile(
        this,
        config.strings.SELECT_KEYWORD_FILE_TEXT,
        keywordFileEdit->text(),
        config.strings.CSV_FILE_FILTER,
        keywordFileEdit
    );
}

void MainWindow::processFiles() {
    try {
        QString inputDir = inputDirEdit->text();
        QString outputDir = outputDirEdit->text();
        QString keywordFile = keywordFileEdit->text();
        
        if (inputDir.isEmpty() || outputDir.isEmpty() || keywordFile.isEmpty()) {
            QMessageBox::warning(this, config.strings.ERROR_TITLE, config.strings.FIELDS_REQUIRED_ERROR);
            return;
        }
        
        FinanceProcessor processor(
            inputDir.toStdString(),
            outputDir.toStdString(),
            keywordFile.toStdString(),
            exportMonthlySummaryCheck->isChecked(),
            exportWeeklySummaryCheck->isChecked(),
            exportFullDatasetCheck->isChecked()
        );
        
        processor.run();
        QMessageBox::information(this, config.strings.SUCCESS_TITLE, config.strings.PROCESS_SUCCESS);
        
    } catch (const std::exception& e) {
        QMessageBox::critical(this, config.strings.ERROR_TITLE, 
                            QString("Processing failed: %1").arg(e.what()));
    }
}

// Utility Functions
void MainWindow::showErrorMessage(const QString& message, const char* title) noexcept {
    QMessageBox::critical(nullptr, title, message, QMessageBox::Ok);
}

}  // namespace FinanceManager 
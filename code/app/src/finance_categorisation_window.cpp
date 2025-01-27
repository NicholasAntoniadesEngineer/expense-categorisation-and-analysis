/**
 * @file finance_categorisation_window.cpp
 * @brief Implementation of the Finance Manager main window
 * @author Nicholas Antoniades
 * @date 2024-01-24
 * 
 * This file implements the main GUI window functionality for the
 * Finance Manager application, handling user interactions and file processing.
 */

#include "finance_categorisation_window.hpp"
#include "finance_processor.hpp"
#include <QMessageBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QDir>
#include <QDebug>
#include <QApplication>
#include <QStyle>
#include <QStyleFactory>
#include <QIcon>
#include <QFont>

namespace FinanceManager {

// Constructor and Initialization
FinanceCategorisationWindow::FinanceCategorisationWindow(AppConfig& config, QWidget *parent)
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
        .PROCESS_SUCCESS = "Files processed successfully!"
    };

    setupUi();
    createConnections();
}

// Window Setup and Configuration
bool FinanceCategorisationWindow::initializeApplicationInfo() noexcept {
    QApplication::setApplicationName(config.app_name);
    QApplication::setOrganizationName(config.org_name);
    QApplication::setOrganizationDomain(config.org_domain);
    QApplication::setApplicationVersion(config.app_version);
    QApplication::setApplicationDisplayName(config.app_name);
    return true;
}

bool FinanceCategorisationWindow::initializeAppearance() noexcept {
    QStyle* style = QStyleFactory::create(config.style_name);
    if (!style) {
        return false;
    }
    QApplication::setStyle(style);

    QFont appFont(config.font_family, config.default_font_size);
    QApplication::setFont(appFont);
    
    QIcon appIcon(":/icons/app_icon.png");
    if (!appIcon.isNull()) {
        QApplication::setWindowIcon(appIcon);
    }
    
    return true;
}

bool FinanceCategorisationWindow::setupWindow() noexcept {
    setWindowTitle(config.app_name);
    resize(config.default_window_width, config.default_window_height);
    show();
    return true;
}

// UI Setup
void FinanceCategorisationWindow::setupUi() {
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    mainLayout = new QVBoxLayout(centralWidget);
    
    // Create input directory group
    QGroupBox *inputGroup = new QGroupBox(config.strings.INPUT_DIR_TITLE, this);
    QHBoxLayout *inputLayout = new QHBoxLayout;
    inputDirEdit = new QLineEdit(this);
    inputBrowseButton = new QPushButton(config.strings.BROWSE_BUTTON_TEXT, this);
    inputLayout->addWidget(inputDirEdit);
    inputLayout->addWidget(inputBrowseButton);
    inputGroup->setLayout(inputLayout);
    
    // Create output directory group
    QGroupBox *outputGroup = new QGroupBox(config.strings.OUTPUT_DIR_TITLE, this);
    QHBoxLayout *outputLayout = new QHBoxLayout;
    outputDirEdit = new QLineEdit(this);
    outputBrowseButton = new QPushButton(config.strings.BROWSE_BUTTON_TEXT, this);
    outputLayout->addWidget(outputDirEdit);
    outputLayout->addWidget(outputBrowseButton);
    outputGroup->setLayout(outputLayout);
    
    // Create keyword file group
    QGroupBox *keywordGroup = new QGroupBox(config.strings.KEYWORD_FILE_TITLE, this);
    QHBoxLayout *keywordLayout = new QHBoxLayout;
    keywordFileEdit = new QLineEdit(this);
    keywordBrowseButton = new QPushButton(config.strings.BROWSE_BUTTON_TEXT, this);
    keywordLayout->addWidget(keywordFileEdit);
    keywordLayout->addWidget(keywordBrowseButton);
    keywordGroup->setLayout(keywordLayout);
    
    // Create export options group
    QGroupBox *exportGroup = new QGroupBox(config.strings.EXPORT_OPTIONS_TITLE, this);
    QVBoxLayout *exportLayout = new QVBoxLayout;
    exportMonthlySummaryCheck = new QCheckBox(config.strings.MONTHLY_SUMMARY_TEXT, this);
    exportWeeklySummaryCheck = new QCheckBox(config.strings.WEEKLY_SUMMARY_TEXT, this);
    exportFullDatasetCheck = new QCheckBox(config.strings.FULL_DATASET_TEXT, this);
    exportLayout->addWidget(exportMonthlySummaryCheck);
    exportLayout->addWidget(exportWeeklySummaryCheck);
    exportLayout->addWidget(exportFullDatasetCheck);
    exportGroup->setLayout(exportLayout);
    
    // Create process button
    processButton = new QPushButton(config.strings.PROCESS_BUTTON_TEXT, this);
    
    // Add all widgets to main layout
    mainLayout->addWidget(inputGroup);
    mainLayout->addWidget(outputGroup);
    mainLayout->addWidget(keywordGroup);
    mainLayout->addWidget(exportGroup);
    mainLayout->addWidget(processButton);
    mainLayout->addStretch();
    
    // Set default paths and states
    setupDefaultPaths();
    setupDefaultStates();
}

void FinanceCategorisationWindow::setupDefaultPaths() {
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

void FinanceCategorisationWindow::setupDefaultStates() {
    exportMonthlySummaryCheck->setChecked(true);
    exportWeeklySummaryCheck->setChecked(false);
    exportFullDatasetCheck->setChecked(true);
}

void FinanceCategorisationWindow::createConnections() {
    connect(inputBrowseButton, &QPushButton::clicked, this, &FinanceCategorisationWindow::browseInputDirectory);
    connect(outputBrowseButton, &QPushButton::clicked, this, &FinanceCategorisationWindow::browseOutputDirectory);
    connect(keywordBrowseButton, &QPushButton::clicked, this, &FinanceCategorisationWindow::browseKeywordFile);
    connect(processButton, &QPushButton::clicked, this, &FinanceCategorisationWindow::processFiles);
}

// Event Handlers
void FinanceCategorisationWindow::browseInputDirectory() {
    QString dir = QFileDialog::getExistingDirectory(
        this, config.strings.SELECT_INPUT_DIR_TEXT,
        inputDirEdit->text(),
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
    );
    
    if (!dir.isEmpty()) {
        inputDirEdit->setText(dir);
        inputDirectory = dir;
    }
}

void FinanceCategorisationWindow::browseOutputDirectory() {
    QString dir = QFileDialog::getExistingDirectory(
        this, config.strings.SELECT_OUTPUT_DIR_TEXT,
        outputDirEdit->text(),
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
    );
    
    if (!dir.isEmpty()) {
        outputDirEdit->setText(dir);
        outputDirectory = dir;
    }
}

void FinanceCategorisationWindow::browseKeywordFile() {
    QString file = QFileDialog::getOpenFileName(
        this, config.strings.SELECT_KEYWORD_FILE_TEXT,
        keywordFileEdit->text(),
        config.strings.CSV_FILE_FILTER
    );
    
    if (!file.isEmpty()) {
        keywordFileEdit->setText(file);
        keywordFile = file;
    }
}

void FinanceCategorisationWindow::processFiles() {
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
void FinanceCategorisationWindow::showErrorMessage(const QString& message, const char* title) noexcept {
    QMessageBox::critical(nullptr, title, message, QMessageBox::Ok);
}

}  // namespace FinanceManager 
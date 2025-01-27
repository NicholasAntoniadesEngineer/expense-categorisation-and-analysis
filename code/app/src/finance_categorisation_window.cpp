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

namespace FinanceManager {

/**
 * @brief Window configuration constants
 */
struct WindowConfig {
    static const char* const INPUT_GROUP_TITLE;
    static const char* const OUTPUT_GROUP_TITLE;
    static const char* const KEYWORD_GROUP_TITLE;
    static const char* const EXPORT_GROUP_TITLE;
    static const char* const BROWSE_BUTTON_TEXT;
    static const char* const PROCESS_BUTTON_TEXT;
};

const char* const WindowConfig::INPUT_GROUP_TITLE = "Input Directory";
const char* const WindowConfig::OUTPUT_GROUP_TITLE = "Output Directory";
const char* const WindowConfig::KEYWORD_GROUP_TITLE = "Keyword File";
const char* const WindowConfig::EXPORT_GROUP_TITLE = "Export Options";
const char* const WindowConfig::BROWSE_BUTTON_TEXT = "Browse";
const char* const WindowConfig::PROCESS_BUTTON_TEXT = "Process Files";

/**
 * @brief Constructs the main window and initializes UI components
 * @param parent Parent widget (optional)
 */
FinanceCategorisationWindow::FinanceCategorisationWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi();
    createConnections();
}

/**
 * @brief Sets up the user interface components and layout
 */
void FinanceCategorisationWindow::setupUi()
{
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    mainLayout = new QVBoxLayout(centralWidget);
    
    // Create input directory group
    QGroupBox *inputGroup = new QGroupBox(WindowConfig::INPUT_GROUP_TITLE, this);
    QHBoxLayout *inputLayout = new QHBoxLayout;
    inputDirEdit = new QLineEdit(this);
    inputBrowseButton = new QPushButton(WindowConfig::BROWSE_BUTTON_TEXT, this);
    inputLayout->addWidget(inputDirEdit);
    inputLayout->addWidget(inputBrowseButton);
    inputGroup->setLayout(inputLayout);
    
    // Create output directory group
    QGroupBox *outputGroup = new QGroupBox(WindowConfig::OUTPUT_GROUP_TITLE, this);
    QHBoxLayout *outputLayout = new QHBoxLayout;
    outputDirEdit = new QLineEdit(this);
    outputBrowseButton = new QPushButton(WindowConfig::BROWSE_BUTTON_TEXT, this);
    outputLayout->addWidget(outputDirEdit);
    outputLayout->addWidget(outputBrowseButton);
    outputGroup->setLayout(outputLayout);
    
    // Create keyword file group
    QGroupBox *keywordGroup = new QGroupBox(WindowConfig::KEYWORD_GROUP_TITLE, this);
    QHBoxLayout *keywordLayout = new QHBoxLayout;
    keywordFileEdit = new QLineEdit(this);
    keywordBrowseButton = new QPushButton(WindowConfig::BROWSE_BUTTON_TEXT, this);
    keywordLayout->addWidget(keywordFileEdit);
    keywordLayout->addWidget(keywordBrowseButton);
    keywordGroup->setLayout(keywordLayout);
    
    // Create export options group
    QGroupBox *exportGroup = new QGroupBox(WindowConfig::EXPORT_GROUP_TITLE, this);
    QVBoxLayout *exportLayout = new QVBoxLayout;
    exportMonthlySummaryCheck = new QCheckBox("Export Monthly Summary", this);
    exportWeeklySummaryCheck = new QCheckBox("Export Weekly Summary", this);
    exportFullDatasetCheck = new QCheckBox("Export Full Dataset", this);
    exportLayout->addWidget(exportMonthlySummaryCheck);
    exportLayout->addWidget(exportWeeklySummaryCheck);
    exportLayout->addWidget(exportFullDatasetCheck);
    exportGroup->setLayout(exportLayout);
    
    // Create process button
    processButton = new QPushButton(WindowConfig::PROCESS_BUTTON_TEXT, this);
    
    // Add all widgets to main layout
    mainLayout->addWidget(inputGroup);
    mainLayout->addWidget(outputGroup);
    mainLayout->addWidget(keywordGroup);
    mainLayout->addWidget(exportGroup);
    mainLayout->addWidget(processButton);
    mainLayout->addStretch();
    
    // Navigate from source file location to repository root
    QDir appDir(QString::fromUtf8(__FILE__));
    for (int i = 0; i < 4; ++i) {
        appDir.cdUp();
    }
    
    // Set default values with paths relative to repository root
    inputDirEdit->setText(appDir.absoluteFilePath("input_files"));
    outputDirEdit->setText(appDir.absoluteFilePath("output_files"));


    keywordFileEdit->setText(appDir.absoluteFilePath("config/categorisation_keywords.csv"));
    
    // Set default checkbox states
    exportMonthlySummaryCheck->setChecked(true);
    exportWeeklySummaryCheck->setChecked(false);
    exportFullDatasetCheck->setChecked(true);
}

/**
 * @brief Creates signal/slot connections for UI components
 */
void FinanceCategorisationWindow::createConnections()
{
    connect(inputBrowseButton, &QPushButton::clicked, this, &FinanceCategorisationWindow::browseInputDirectory);
    connect(outputBrowseButton, &QPushButton::clicked, this, &FinanceCategorisationWindow::browseOutputDirectory);
    connect(keywordBrowseButton, &QPushButton::clicked, this, &FinanceCategorisationWindow::browseKeywordFile);
    connect(processButton, &QPushButton::clicked, this, &FinanceCategorisationWindow::processFiles);
}

/**
 * @brief Opens a directory dialog for selecting the input directory
 */
void FinanceCategorisationWindow::browseInputDirectory()
{
    QString dir = QFileDialog::getExistingDirectory(this, "Select Input Directory",
                                                  inputDirEdit->text(),
                                                  QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (!dir.isEmpty()) {
        inputDirEdit->setText(dir);
        inputDirectory = dir;
    }
}

/**
 * @brief Opens a directory dialog for selecting the output directory
 */
void FinanceCategorisationWindow::browseOutputDirectory()
{
    QString dir = QFileDialog::getExistingDirectory(this, "Select Output Directory",
                                                  outputDirEdit->text(),
                                                  QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (!dir.isEmpty()) {
        outputDirEdit->setText(dir);
        outputDirectory = dir;
    }
}

/**
 * @brief Opens a file dialog for selecting the keyword mapping file
 */
void FinanceCategorisationWindow::browseKeywordFile()
{
    QString file = QFileDialog::getOpenFileName(this, "Select Keyword File",
                                              keywordFileEdit->text(),
                                              "CSV Files (*.csv);;All Files (*)");
    if (!file.isEmpty()) {
        keywordFileEdit->setText(file);
        keywordFile = file;
    }
}

/**
 * @brief Processes the input files using the selected options
 * 
 * Validates input fields and initiates the file processing using
 * the FinanceProcessor class. Displays appropriate success/error
 * messages to the user.
 */
void FinanceCategorisationWindow::processFiles()
{
    try {
        QString inputDir = inputDirEdit->text();
        QString outputDir = outputDirEdit->text();
        QString keywordFile = keywordFileEdit->text();
        
        if (inputDir.isEmpty() || outputDir.isEmpty() || keywordFile.isEmpty()) {
            QMessageBox::warning(this, "Error", "All fields must be filled");
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
        
        QMessageBox::information(this, "Success", "Files processed successfully!");
        
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", QString("Processing failed: %1").arg(e.what()));
    }
}

}  // namespace FinanceManager 
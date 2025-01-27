/**
 * @file finance_categorisation_window.hpp
 * @brief Main window class for the Finance Manager application
 * @author Nicholas Antoniades
 * @date 2024-01-24
 * 
 * This class implements the main GUI window for the Finance Manager application,
 * providing interfaces for directory selection and processing options.
 */

#pragma once

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QFileDialog>
#include <QString>
#include <QCheckBox>

namespace FinanceManager {

/**
 * @brief Application configuration settings
 * 
 * Configuration for window appearance and application metadata.
 */
struct AppConfig {
    const char* app_name;
    const char* org_name;
    const char* org_domain;
    const char* app_version;
    const char* style_name;
    const char* font_family;
    int default_window_width;
    int default_window_height;
    int default_font_size;
};

/**
 * @brief Main window class for the Finance Manager application
 * 
 * Provides the user interface for selecting input/output directories,
 * keyword files, and processing options for financial data analysis.
 */
class FinanceCategorisationWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit FinanceCategorisationWindow(QWidget *parent = nullptr);
    ~FinanceCategorisationWindow() = default;

private slots:
    // Directory and file selection dialogs
    void browseInputDirectory();
    void browseOutputDirectory();
    void browseKeywordFile();
    
    void processFiles();

private:
    void setupUi();
    void createConnections();

    // UI Components
    QWidget *centralWidget;
    QVBoxLayout *mainLayout;
    
    QLineEdit *inputDirEdit;
    QLineEdit *outputDirEdit;
    QLineEdit *keywordFileEdit;
    
    QPushButton *inputBrowseButton;
    QPushButton *outputBrowseButton;
    QPushButton *keywordBrowseButton;
    QPushButton *processButton;

    QCheckBox *exportMonthlySummaryCheck;
    QCheckBox *exportWeeklySummaryCheck;
    QCheckBox *exportFullDatasetCheck;

    // File paths
    QString inputDirectory;
    QString outputDirectory;
    QString keywordFile;
};

}  // namespace FinanceManager 
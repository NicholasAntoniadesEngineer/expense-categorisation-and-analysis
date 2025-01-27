/**
 * @file finance_categorisation_window.hpp
 * @brief Main window for the Finance Manager application
 * @author Nicholas Antoniades
 * @date 2024-01-24
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
#include <QtCharts>

namespace FinanceManager {

/**
 * @brief Application configuration settings
 */
struct AppConfig {
    // Application metadata
    const char* app_name;
    const char* org_name;
    const char* org_domain;
    const char* app_version;
    const char* style_name;
    const char* font_family;
    int default_window_width;
    int default_window_height;
    int default_font_size;

    // UI strings
    struct UIStrings {
        const char* INPUT_DIR_TITLE;
        const char* OUTPUT_DIR_TITLE;
        const char* KEYWORD_FILE_TITLE;
        const char* EXPORT_OPTIONS_TITLE;
        const char* BROWSE_BUTTON_TEXT;
        const char* PROCESS_BUTTON_TEXT;
        const char* MONTHLY_SUMMARY_TEXT;
        const char* WEEKLY_SUMMARY_TEXT;
        const char* FULL_DATASET_TEXT;
        const char* SELECT_INPUT_DIR_TEXT;
        const char* SELECT_OUTPUT_DIR_TEXT;
        const char* SELECT_KEYWORD_FILE_TEXT;
        const char* CSV_FILE_FILTER;
        const char* ERROR_TITLE;
        const char* SUCCESS_TITLE;
        const char* FIELDS_REQUIRED_ERROR;
        const char* PROCESS_SUCCESS;
        const char* PLOT_WEEKLY_TEXT;
        const char* PLOT_MONTHLY_TEXT;
    } strings;
};

/**
 * @brief Main window for selecting and processing financial data
 */
class FinanceCategorisationWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit FinanceCategorisationWindow(AppConfig& config, QWidget *parent = nullptr);
    ~FinanceCategorisationWindow() = default;

    // Window Setup and Configuration
    bool initializeAppearance() noexcept;
    bool initializeApplicationInfo() noexcept;
    bool setupWindow() noexcept;
    
    // Utility Functions
    static void showErrorMessage(const QString& message, const char* title) noexcept;

private slots:
    // Event Handlers
    void browseInputDirectory();
    void browseOutputDirectory();
    void browseKeywordFile();
    void processFiles();
    void plotWeeklySummary();
    void plotMonthlySummary();
    void updateSeriesVisibility(const QString& category, bool visible);

private:
    // UI Setup
    void setupUi();
    void setupDefaultPaths();
    void setupDefaultStates();
    void createConnections();
    void setupPlotWindow(QChart* chart, const QString& title);
    void setupCategoryPanel(QMainWindow* plotWindow, const QStringList& categories, const QMap<QString, QLineSeries*>& series);

    AppConfig& config;

    QWidget *centralWidget;
    QVBoxLayout *mainLayout;
    
    QLineEdit *inputDirEdit;
    QLineEdit *outputDirEdit;
    QLineEdit *keywordFileEdit;
    
    QPushButton *inputBrowseButton;
    QPushButton *outputBrowseButton;
    QPushButton *keywordBrowseButton;
    QPushButton *processButton;
    QPushButton *plotWeeklyButton;
    QPushButton *plotMonthlyButton;

    QCheckBox *exportMonthlySummaryCheck;
    QCheckBox *exportWeeklySummaryCheck;
    QCheckBox *exportFullDatasetCheck;

    QString inputDirectory;
    QString outputDirectory;
    QString keywordFile;
};

}  // namespace FinanceManager 
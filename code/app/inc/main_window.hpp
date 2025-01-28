/**
 * @file main_window.hpp
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
#include "app_config.hpp"
#include "plot_window.hpp"
#include "table_window.hpp"

namespace FinanceManager {

/**
 * @brief Main window for selecting and processing financial data
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(AppConfig& config, QWidget *parent = nullptr);
    ~MainWindow() = default;

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
    void viewAllTransactions();
    void viewWeeklySummary();
    void viewMonthlySummary();
    void updateSeriesVisibility(const QString& category, bool visible);

private:
    // UI Setup
    void setupUi();
    void setupDefaultPaths();
    void setupDefaultStates();
    void createConnections();
    void plotData(const QString& filePattern, const QString& title, const QString& xAxisTitle);

    // Window tracking
    PlotWindow* weeklyPlotWindow = nullptr;
    PlotWindow* monthlyPlotWindow = nullptr;
    TableWindow* allTransactionsWindow = nullptr;
    TableWindow* weeklySummaryWindow = nullptr;
    TableWindow* monthlySummaryWindow = nullptr;

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
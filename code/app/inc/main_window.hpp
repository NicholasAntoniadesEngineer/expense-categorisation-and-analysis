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
#include "visualization_manager.hpp"

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
    void plotWeeklySummary() {
        VisualizationManager::plotWeeklySummary(windows, outputDirEdit->text(), this);
    }
    void plotMonthlySummary() {
        VisualizationManager::plotMonthlySummary(windows, outputDirEdit->text(), this);
    }
    void viewAllTransactions() {
        VisualizationManager::viewAllTransactions(windows, outputDirEdit->text(), this);
    }
    void viewWeeklySummary() {
        VisualizationManager::viewWeeklySummary(windows, outputDirEdit->text(), this);
    }
    void viewMonthlySummary() {
        VisualizationManager::viewMonthlySummary(windows, outputDirEdit->text(), this);
    }

private:
    // UI Setup
    void setupUi();
    void setupDefaultPaths();
    void setupDefaultStates();
    void createConnections();

    // Member variables
    AppConfig& config;
    QString inputDirectory;
    QString outputDirectory;
    QString keywordFile;
    
    // UI Components
    QWidget* centralWidget;
    QVBoxLayout* mainLayout;
    QLineEdit* inputDirEdit;
    QLineEdit* outputDirEdit;
    QLineEdit* keywordFileEdit;
    QPushButton* inputBrowseButton;
    QPushButton* outputBrowseButton;
    QPushButton* keywordBrowseButton;
    QPushButton* processButton;
    QPushButton* plotWeeklyButton;
    QPushButton* plotMonthlyButton;
    QCheckBox* exportMonthlySummaryCheck;
    QCheckBox* exportWeeklySummaryCheck;
    QCheckBox* exportFullDatasetCheck;
    
    // Visualization windows
    VisualizationManager::Windows windows;
};

}  // namespace FinanceManager 
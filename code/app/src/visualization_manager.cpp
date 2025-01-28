#include "visualization_manager.hpp"
#include <QObject>

namespace FinanceManager {

// Window Management Methods
void VisualizationManager::plotData(Windows& windows,
                                  const QString& outputDir,
                                  const QString& filePattern,
                                  const QString& title,
                                  const QString& xAxisTitle,
                                  QWidget* parent) {
    PlotWindow*& plotWindow = (title.contains("Weekly") ? windows.weeklyPlotWindow : windows.monthlyPlotWindow);
    
    PlotManager::createPlotFromFile(plotWindow, outputDir, filePattern, title, xAxisTitle, parent,
        [&windows](const QString& category, bool visible) {
            updateSeriesVisibility(windows, category, visible);
        });
}

void VisualizationManager::plotWeeklySummary(Windows& windows,
                                           const QString& outputDir,
                                           QWidget* parent) {
    if (windows.weeklyPlotWindow) {
        windows.weeklyPlotWindow->activateWindow();
        windows.weeklyPlotWindow->raise();
        return;
    }
    plotData(windows, outputDir, "weekly_summary.csv", "Weekly Expense Summary Over Time", "Week Number", parent);
}

void VisualizationManager::plotMonthlySummary(Windows& windows,
                                            const QString& outputDir,
                                            QWidget* parent) {
    if (windows.monthlyPlotWindow) {
        windows.monthlyPlotWindow->activateWindow();
        windows.monthlyPlotWindow->raise();
        return;
    }
    plotData(windows, outputDir, "monthly_summary.csv", "Monthly Expense Summary Over Time", "Month Number", parent);
}

void VisualizationManager::viewAllTransactions(Windows& windows,
                                             const QString& outputDir,
                                             QWidget* parent) {
    TableManager::TableConfig config{
        .title = "All Categorised Transactions",
        .fileName = "categorised_transactions.csv",
        .width = 1200,
        .height = 800
    };
    
    windows.allTransactionsWindow = TableManager::showTableFromFile(
        windows.allTransactionsWindow, outputDir, config, parent);

    if (windows.allTransactionsWindow) {
        setupWindowDestroyHandler(windows, windows.allTransactionsWindow);
    }
}

void VisualizationManager::viewWeeklySummary(Windows& windows,
                                           const QString& outputDir,
                                           QWidget* parent) {
    TableManager::TableConfig config{
        .title = "Weekly Summary",
        .fileName = "weekly_summary.csv",
        .width = 1000,
        .height = 600
    };
    
    windows.weeklySummaryWindow = TableManager::showTableFromFile(
        windows.weeklySummaryWindow, outputDir, config, parent);

    if (windows.weeklySummaryWindow) {
        setupWindowDestroyHandler(windows, windows.weeklySummaryWindow);
    }
}

void VisualizationManager::viewMonthlySummary(Windows& windows,
                                            const QString& outputDir,
                                            QWidget* parent) {
    TableManager::TableConfig config{
        .title = "Monthly Summary",
        .fileName = "monthly_summary.csv",
        .width = 1000,
        .height = 600
    };
    
    windows.monthlySummaryWindow = TableManager::showTableFromFile(
        windows.monthlySummaryWindow, outputDir, config, parent);

    if (windows.monthlySummaryWindow) {
        setupWindowDestroyHandler(windows, windows.monthlySummaryWindow);
    }
}

void VisualizationManager::updateSeriesVisibility(Windows& windows,
                                                const QString& category,
                                                bool visible) {
    PlotManager::updateSeriesVisibility(windows.weeklyPlotWindow, category, visible);
    PlotManager::updateSeriesVisibility(windows.monthlyPlotWindow, category, visible);
}

// UI Group Creation Methods
QGroupBox* VisualizationManager::createVisualizationGroup(QWidget* parent, VisualizationButtons& buttons) {
    QGroupBox* visualizationGroup = new QGroupBox("Visualization", parent);
    QVBoxLayout* visualizationLayout = new QVBoxLayout;
    
    // Create and add plot group
    QGroupBox* plotGroup = createPlotGroup(parent, buttons.plotWeeklyButton, buttons.plotMonthlyButton);
    visualizationLayout->addWidget(plotGroup);
    
    // Create and add summary group
    QGroupBox* summaryGroup = createSummaryGroup(parent,
                                               buttons.viewWeeklySummaryButton,
                                               buttons.viewMonthlySummaryButton,
                                               buttons.viewAllTransactionsButton);
    visualizationLayout->addWidget(summaryGroup);
    
    visualizationGroup->setLayout(visualizationLayout);
    return visualizationGroup;
}

QGroupBox* VisualizationManager::createPlotGroup(QWidget* parent,
                                               QPushButton* plotWeeklyButton,
                                               QPushButton* plotMonthlyButton) {
    QGroupBox* plotGroup = new QGroupBox("Plots", parent);
    QHBoxLayout* plotLayout = new QHBoxLayout;
    
    plotLayout->addWidget(plotWeeklyButton);
    plotLayout->addWidget(plotMonthlyButton);
    
    plotGroup->setLayout(plotLayout);
    return plotGroup;
}

QGroupBox* VisualizationManager::createSummaryGroup(QWidget* parent,
                                                  QPushButton* viewWeeklySummaryButton,
                                                  QPushButton* viewMonthlySummaryButton,
                                                  QPushButton* viewAllTransactionsButton) {
    QGroupBox* summaryGroup = new QGroupBox("View Summaries", parent);
    QHBoxLayout* summaryLayout = new QHBoxLayout;
    
    summaryLayout->addWidget(viewWeeklySummaryButton);
    summaryLayout->addWidget(viewMonthlySummaryButton);
    summaryLayout->addWidget(viewAllTransactionsButton);
    
    summaryGroup->setLayout(summaryLayout);
    return summaryGroup;
}

} // namespace FinanceManager 
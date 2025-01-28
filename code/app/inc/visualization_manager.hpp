#pragma once

#include <QString>
#include <QWidget>
#include "plot_manager.hpp"
#include "table_manager.hpp"
#include "table_window.hpp"

namespace FinanceManager {

class VisualizationManager {
public:
    struct Windows {
        PlotWindow* weeklyPlotWindow = nullptr;
        PlotWindow* monthlyPlotWindow = nullptr;
        TableWindow* allTransactionsWindow = nullptr;
        TableWindow* weeklySummaryWindow = nullptr;
        TableWindow* monthlySummaryWindow = nullptr;
    };

    static void plotData(Windows& windows, 
                        const QString& outputDir,
                        const QString& filePattern, 
                        const QString& title, 
                        const QString& xAxisTitle,
                        QWidget* parent);

    static void plotWeeklySummary(Windows& windows, 
                                 const QString& outputDir,
                                 QWidget* parent);

    static void plotMonthlySummary(Windows& windows, 
                                  const QString& outputDir,
                                  QWidget* parent);

    static void viewAllTransactions(Windows& windows,
                                  const QString& outputDir,
                                  QWidget* parent);

    static void viewWeeklySummary(Windows& windows,
                                 const QString& outputDir,
                                 QWidget* parent);

    static void viewMonthlySummary(Windows& windows,
                                 const QString& outputDir,
                                 QWidget* parent);

    static void updateSeriesVisibility(Windows& windows,
                                     const QString& category,
                                     bool visible);

private:
    template<typename T>
    static void setupWindowDestroyHandler(Windows& windows, T*& window) {
        (void)windows;  // Silence unused parameter warning
        QObject::connect(window, &QObject::destroyed, [&window]() {
            window = nullptr;
        });
    }
};

} // namespace FinanceManager 
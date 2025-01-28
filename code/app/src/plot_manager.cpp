/**
 * @file plot_manager.cpp
 * @brief Implementation of plot window and chart visualization management
 * @author Nicholas Antoniades
 * @date 2024-01-28
 */

#include "plot_manager.hpp"
#include "window_manager.hpp"
#include <QDir>
#include <QChartView>

namespace FinanceManager {

PlotWindow* PlotManager::createPlotFromFile(PlotWindow*& currentWindow,
                                          const QString& outputDir,
                                          const QString& filePattern,
                                          const QString& title,
                                          const QString& xAxisTitle,
                                          QWidget* parent,
                                          const std::function<void(const QString&, bool)>& visibilityCallback) {
    QString filePath;
    if (!validatePlotData(parent, outputDir, filePattern, title, filePath)) {
        return nullptr;
    }
    
    return showPlotWindow(currentWindow, filePath, title, xAxisTitle, parent, visibilityCallback);
}

PlotWindow* PlotManager::showPlotWindow(PlotWindow*& currentWindow,
                                      const QString& filePath,
                                      const QString& title,
                                      const QString& xAxisTitle,
                                      QWidget* parent,
                                      const std::function<void(const QString&, bool)>& visibilityCallback) {
    try {
        double maxValue, minValue;
        QMap<QString, QLineSeries*> categorySeries;
        
        QChart* chart = ChartManager::createSummaryChart(
            filePath, title, xAxisTitle,
            maxValue, minValue, categorySeries
        );

        if (!currentWindow) {
            currentWindow = new PlotWindow(title, parent);
            if (visibilityCallback) {
                QObject::connect(currentWindow, &PlotWindow::categoryVisibilityChanged,
                               parent, visibilityCallback);
            }
            QObject::connect(currentWindow, &QObject::destroyed, [&currentWindow]() {
                currentWindow = nullptr;
            });
        }
        
        currentWindow->setChart(chart);
        currentWindow->setupCategoryPanel(categorySeries.keys(), categorySeries);
        currentWindow->show();
        currentWindow->raise();
        currentWindow->activateWindow();

        return currentWindow;

    } catch (const std::exception& e) {
        WindowManager::showErrorMessage(parent, 
            QString("Plot generation failed: %1").arg(e.what()));
        return nullptr;
    }
}

void PlotManager::updateSeriesVisibility(PlotWindow* window,
                                       const QString& category,
                                       bool visible) {
    if (!window) return;

    QChartView* chartView = qobject_cast<QChartView*>(window->centralWidget());
    if (!chartView) return;

    QChart* chart = chartView->chart();
    for (QAbstractSeries* abstractSeries : chart->series()) {
        QLineSeries* lineSeries = qobject_cast<QLineSeries*>(abstractSeries);
        if (lineSeries && lineSeries->name() == category) {
            lineSeries->setVisible(visible);
            lineSeries->setPointsVisible(visible);
        }
    }
}

bool PlotManager::validatePlotData(QWidget* parent,
                                 const QString& outputDir,
                                 const QString& filePattern,
                                 const QString& title,
                                 QString& filePath) {
    if (!WindowManager::checkOutputDirectory(parent, outputDir)) {
        return false;
    }

    QDir dir(outputDir);
    QStringList files = dir.entryList(QStringList() << filePattern, QDir::Files);
    
    if (files.isEmpty()) {
        WindowManager::showErrorMessage(parent, 
            QString("No %1 files found").arg(title.toLower()));
        return false;
    }

    filePath = dir.filePath(files.first());
    return true;
}

} // namespace FinanceManager 
/**
 * @file plot_manager.hpp
 * @brief Manager class for plot windows and chart visualization
 * @author Nicholas Antoniades
 * @date 2024-01-28
 */

#pragma once

#include <QString>
#include <QWidget>
#include "plot_window.hpp"
#include "chart_manager.hpp"

namespace FinanceManager {

class PlotManager {
public:
    static PlotWindow* createPlotFromFile(PlotWindow*& currentWindow,
                                        const QString& outputDir,
                                        const QString& filePattern,
                                        const QString& title,
                                        const QString& xAxisTitle,
                                        QWidget* parent,
                                        const std::function<void(const QString&, bool)>& visibilityCallback);

    static PlotWindow* showPlotWindow(PlotWindow*& currentWindow,
                                    const QString& filePath,
                                    const QString& title,
                                    const QString& xAxisTitle,
                                    QWidget* parent,
                                    const std::function<void(const QString&, bool)>& visibilityCallback);

    static void updateSeriesVisibility(PlotWindow* window,
                                     const QString& category,
                                     bool visible);

private:
    static bool validatePlotData(QWidget* parent,
                               const QString& outputDir,
                               const QString& filePattern,
                               const QString& title,
                               QString& filePath);
};

} // namespace FinanceManager 
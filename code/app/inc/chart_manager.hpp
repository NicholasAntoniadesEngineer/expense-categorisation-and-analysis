/**
 * @file chart_manager.hpp
 * @brief Manager class for creating and configuring charts
 * @author Nicholas Antoniades
 * @date 2024-01-28
 */

#pragma once

#include <QtCharts>
#include <QString>
#include <QMap>
#include <QStringList>

namespace FinanceManager {

class ChartManager {
public:
    static QChart* createSummaryChart(const QString& filePath,
                                    const QString& title,
                                    const QString& xAxisTitle,
                                    double& maxValue,
                                    double& minValue,
                                    QMap<QString, QLineSeries*>& categorySeries);

private:
    static void setupAxes(QChart* chart,
                         const QStringList& dates,
                         double maxValue,
                         double minValue,
                         const QString& xAxisTitle);
                         
    static QColor getCategoryColor(int index);
    static void setupSeries(QLineSeries* series, int colorIndex);
};

} // namespace FinanceManager 
/**
 * @file chart_manager.cpp
 * @brief Implementation of the chart manager for creating and configuring charts
 * @author Nicholas Antoniades
 * @date 2024-01-28
 */

#include "chart_manager.hpp"
#include <QFile>
#include <QTextStream>
#include <limits>

namespace FinanceManager {

QChart* ChartManager::createSummaryChart(const QString& filePath,
                                       const QString& title,
                                       const QString& xAxisTitle,
                                       double& maxValue,
                                       double& minValue,
                                       QMap<QString, QLineSeries*>& categorySeries) {
    QChart* chart = new QChart();
    chart->setTitle(title);
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // Initialize min/max values
    maxValue = 0;
    minValue = std::numeric_limits<double>::max();

    // Read the CSV file
    QFile csvFile(filePath);
    if (!csvFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return chart;

    QTextStream in(&csvFile);
    
    // Read header to get dates
    QString header = in.readLine();
    QStringList dates = header.split(',');
    dates.removeFirst(); // Remove "Category" column header
    
    // Read each line (category)
    int colorIndex = 0;
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(',');
        if (fields.size() >= 2) {
            QString category = fields[0];
            QLineSeries* series = new QLineSeries();
            series->setName(category);
            
            setupSeries(series, colorIndex++);

            // Add data points
            for (int i = 1; i < fields.size(); ++i) {
                double value = fields[i].toDouble();
                series->append(i, value);
                maxValue = qMax(maxValue, value);
                minValue = qMin(minValue, value);
            }

            categorySeries[category] = series;
            chart->addSeries(series);
        }
    }
    csvFile.close();

    setupAxes(chart, dates, maxValue, minValue, xAxisTitle);
    chart->legend()->setVisible(false);

    return chart;
}

void ChartManager::setupAxes(QChart* chart,
                           const QStringList& dates,
                           double maxValue,
                           double minValue,
                           const QString& xAxisTitle) {
    // Create and setup Y axis
    QValueAxis* axisY = new QValueAxis();
    double maxRounded = std::ceil(maxValue / 400.0) * 400;
    double minRounded = std::min(0.0, std::floor(minValue / 400.0) * 400);
    axisY->setRange(minRounded, maxRounded);
    axisY->setTickCount((maxRounded - minRounded) / 400 + 1);
    axisY->setLabelFormat("%d");
    axisY->setTitleText("Amount (£)");
    axisY->setGridLineVisible(true);
    chart->addAxis(axisY, Qt::AlignLeft);

    // Create custom X axis with date labels
    QCategoryAxis* axisX = new QCategoryAxis();
    axisX->setRange(1, dates.size());
    axisX->setTitleText(xAxisTitle);
    axisX->setGridLineVisible(true);
    
    // Add date labels to x-axis
    for (int i = 0; i < dates.size(); ++i) {
        axisX->append(dates[i], i + 1);
    }
    axisX->setLabelsPosition(QCategoryAxis::AxisLabelsPositionOnValue);
    axisX->setLabelsAngle(-65);
    chart->addAxis(axisX, Qt::AlignBottom);

    // Attach axes to all series
    for (auto series : chart->series()) {
        series->attachAxis(axisX);
        series->attachAxis(axisY);
    }
}

QColor ChartManager::getCategoryColor(int index) {
    static const QColor colors[] = {
        QColor("#1f77b4"), // Blue
        QColor("#ff7f0e"), // Orange
        QColor("#2ca02c"), // Green
        QColor("#d62728"), // Red
        QColor("#9467bd"), // Purple
        QColor("#8c564b"), // Brown
        QColor("#e377c2"), // Pink
        QColor("#7f7f7f"), // Gray
        QColor("#bcbd22"), // Yellow-green
        QColor("#17becf")  // Cyan
    };
    return colors[index % 10];
}

void ChartManager::setupSeries(QLineSeries* series, int colorIndex) {
    QPen pen = series->pen();
    pen.setColor(getCategoryColor(colorIndex));
    pen.setWidth(1);
    series->setPen(pen);
    series->setPointsVisible(true);
    series->setMarkerSize(2);
    series->setPointLabelsVisible(false);
}

} // namespace FinanceManager 
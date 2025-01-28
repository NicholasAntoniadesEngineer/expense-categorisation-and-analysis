/**
 * @file plot_window.hpp
 * @brief Plot window for displaying financial data charts
 * @author Nicholas Antoniades
 * @date 2024-01-24
 */

#pragma once

#include <QMainWindow>
#include <QtCharts>
#include <QLabel>
#include <QDockWidget>
#include <QCheckBox>
#include <QVBoxLayout>

namespace FinanceManager {

class PlotWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit PlotWindow(const QString& title, QWidget* parent = nullptr);
    ~PlotWindow() = default;

    void setChart(QChart* chart);
    void setupCategoryPanel(const QStringList& categories, const QMap<QString, QLineSeries*>& series);

signals:
    void categoryVisibilityChanged(const QString& category, bool visible);

private:
    void setupChartView(QChart* chart);
    void createCoordinateLabel();
    void setupMouseTracking(QChartView* chartView, QChart* chart);

    QLabel* coordLabel;
};

// Mouse event filter for chart interaction
class MouseEventFilter : public QObject {
public:
    MouseEventFilter(QChart* chart, QLabel* label);

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;

private:
    QChart* m_chart;
    QLabel* m_label;
};

} // namespace FinanceManager 
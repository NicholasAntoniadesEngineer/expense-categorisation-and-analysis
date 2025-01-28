/**
 * @file plot_window.cpp
 * @brief Implementation of plot window for displaying financial data charts
 * @author Nicholas Antoniades
 * @date 2024-01-24
 */

#include "plot_window.hpp"
#include <QStatusBar>

namespace FinanceManager {

MouseEventFilter::MouseEventFilter(QChart* chart, QLabel* label) 
    : QObject(chart), m_chart(chart), m_label(label) {}

bool MouseEventFilter::eventFilter(QObject* obj, QEvent* event) {
    if (event->type() == QEvent::MouseMove) {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        QPoint point = mouseEvent->pos();
        QPointF chartPoint = m_chart->mapToValue(point);
        m_label->setText(QString("X: %1, Y: £%2")
            .arg(chartPoint.x(), 0, 'f', 1)
            .arg(chartPoint.y(), 0, 'f', 2));
    }
    return QObject::eventFilter(obj, event);
}

PlotWindow::PlotWindow(const QString& title, QWidget* parent)
    : QMainWindow(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle(title);
    resize(1200, 800);
    createCoordinateLabel();
}

void PlotWindow::createCoordinateLabel() {
    coordLabel = new QLabel(this);
    coordLabel->setAlignment(Qt::AlignLeft);
    coordLabel->setStyleSheet(
        "QLabel { color: black; background-color: rgba(255, 255, 255, 0.8); "
        "padding: 5px; border-radius: 3px; margin: 5px; font-weight: bold; }"
    );
    statusBar()->addWidget(coordLabel);
}

void PlotWindow::setupMouseTracking(QChartView* chartView, QChart* chart) {
    chartView->setMouseTracking(true);
    chartView->viewport()->installEventFilter(new MouseEventFilter(chart, coordLabel));
}

void PlotWindow::setupChartView(QChart* chart) {
    QChartView* chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    setCentralWidget(chartView);
    setupMouseTracking(chartView, chart);
}

void PlotWindow::setChart(QChart* chart) {
    setupChartView(chart);
}

void PlotWindow::setupCategoryPanel(const QStringList& categories, 
    const QMap<QString, QLineSeries*>& series) {
    
    QDockWidget* dock = new QDockWidget("Categories", this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    
    QWidget* categoryWidget = new QWidget(dock);
    QVBoxLayout* layout = new QVBoxLayout(categoryWidget);
    
    // Add "Select All" checkbox
    QCheckBox* selectAllBox = new QCheckBox("Select All", categoryWidget);
    selectAllBox->setChecked(true);
    layout->addWidget(selectAllBox);
    
    // Add line separator
    QFrame* line = new QFrame(categoryWidget);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    layout->addWidget(line);
    
    // Create map to store checkboxes
    QMap<QString, QCheckBox*> categoryBoxes;
    
    // Add checkbox for each category
    for (const QString& category : categories) {
        QCheckBox* box = new QCheckBox(category, categoryWidget);
        box->setChecked(true);
        
        // Get the color of the corresponding series
        QLineSeries* lineSeries = series[category];
        if (lineSeries) {
            QColor color = lineSeries->pen().color();
            QPixmap pixmap(16, 16);
            pixmap.fill(color);
            box->setIcon(QIcon(pixmap));
        }
        
        layout->addWidget(box);
        categoryBoxes[category] = box;
        
        // Connect checkbox to visibility update
        connect(box, &QCheckBox::toggled, this, [this, category](bool checked) {
            emit categoryVisibilityChanged(category, checked);
        });
    }
    
    // Connect select all checkbox
    connect(selectAllBox, &QCheckBox::toggled, this, [categoryBoxes](bool checked) {
        for (QCheckBox* box : categoryBoxes) {
            box->setChecked(checked);
        }
    });
    
    layout->addStretch();
    categoryWidget->setLayout(layout);
    
    dock->setWidget(categoryWidget);
    addDockWidget(Qt::RightDockWidgetArea, dock);
}

} // namespace FinanceManager 
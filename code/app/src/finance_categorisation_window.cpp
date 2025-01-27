/**
 * @file finance_categorisation_window.cpp
 * @brief Implementation of the Finance Manager main window
 * @author Nicholas Antoniades
 * @date 2024-01-24
 * 
 * This file implements the main GUI window functionality for the
 * Finance Manager application, handling user interactions and file processing.
 */

#include "finance_categorisation_window.hpp"
#include "finance_processor.hpp"
#include <QMessageBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QDir>
#include <QDebug>
#include <QApplication>
#include <QStyle>
#include <QStyleFactory>
#include <QIcon>
#include <QFont>
#include <QtCharts>
#include <QToolTip>
#include <QLabel>

namespace FinanceManager {

// Add this class definition at the top of the file, after the includes
class MouseEventFilter : public QObject {
public:
    MouseEventFilter(QChart* chart, QLabel* label) 
        : QObject(chart), m_chart(chart), m_label(label) {}

protected:
    bool eventFilter(QObject* obj, QEvent* event) override {
        if (event->type() == QEvent::MouseMove) {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
            QPoint point = mouseEvent->pos();
            // Convert pixel coordinates to chart coordinates
            QPointF chartPoint = m_chart->mapToValue(point);
            m_label->setText(QString("X: %1, Y: £%2")
                .arg(chartPoint.x(), 0, 'f', 1)
                .arg(chartPoint.y(), 0, 'f', 2));
        }
        return QObject::eventFilter(obj, event);
    }

private:
    QChart* m_chart;
    QLabel* m_label;
};

// Constructor and Initialization
FinanceCategorisationWindow::FinanceCategorisationWindow(AppConfig& config, QWidget *parent)
    : QMainWindow(parent)
    , config(config)
{
    // Initialize window-specific strings
    config.strings = {
        .INPUT_DIR_TITLE = "Input Directory",
        .OUTPUT_DIR_TITLE = "Output Directory",
        .KEYWORD_FILE_TITLE = "Keyword File",
        .EXPORT_OPTIONS_TITLE = "Export Options",
        .BROWSE_BUTTON_TEXT = "Browse",
        .PROCESS_BUTTON_TEXT = "Process Files",
        .MONTHLY_SUMMARY_TEXT = "Export Monthly Summary",
        .WEEKLY_SUMMARY_TEXT = "Export Weekly Summary",
        .FULL_DATASET_TEXT = "Export Full Dataset",
        .SELECT_INPUT_DIR_TEXT = "Select Input Directory",
        .SELECT_OUTPUT_DIR_TEXT = "Select Output Directory",
        .SELECT_KEYWORD_FILE_TEXT = "Select Keyword File",
        .CSV_FILE_FILTER = "CSV Files (*.csv);;All Files (*)",
        .ERROR_TITLE = "Error",
        .SUCCESS_TITLE = "Success",
        .FIELDS_REQUIRED_ERROR = "All fields must be filled",
        .PROCESS_SUCCESS = "Files processed successfully!",
        .PLOT_WEEKLY_TEXT = "Plot Weekly Summary",
        .PLOT_MONTHLY_TEXT = "Plot Monthly Summary"
    };

    setupUi();
    createConnections();
}

// Window Setup and Configuration
bool FinanceCategorisationWindow::initializeApplicationInfo() noexcept {
    QApplication::setApplicationName(config.app_name);
    QApplication::setOrganizationName(config.org_name);
    QApplication::setOrganizationDomain(config.org_domain);
    QApplication::setApplicationVersion(config.app_version);
    QApplication::setApplicationDisplayName(config.app_name);
    return true;
}

bool FinanceCategorisationWindow::initializeAppearance() noexcept {
    QStyle* style = QStyleFactory::create(config.style_name);
    if (!style) {
        return false;
    }
    QApplication::setStyle(style);

    // Use ".AppleSystemUIFont" which is guaranteed to be available on macOS
    QFont appFont(".AppleSystemUIFont", config.default_font_size);
    QApplication::setFont(appFont);
    
    QIcon appIcon(":/icons/app_icon.png");
    if (!appIcon.isNull()) {
        QApplication::setWindowIcon(appIcon);
    }
    
    return true;
}

bool FinanceCategorisationWindow::setupWindow() noexcept {
    setWindowTitle(config.app_name);
    resize(config.default_window_width, config.default_window_height);
    show();
    return true;
}

// UI Setup
void FinanceCategorisationWindow::setupUi() {
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    mainLayout = new QVBoxLayout(centralWidget);
    
    // Create input directory group
    QGroupBox *inputGroup = new QGroupBox(config.strings.INPUT_DIR_TITLE, this);
    QHBoxLayout *inputLayout = new QHBoxLayout;
    inputDirEdit = new QLineEdit(this);
    inputBrowseButton = new QPushButton(config.strings.BROWSE_BUTTON_TEXT, this);
    inputLayout->addWidget(inputDirEdit);
    inputLayout->addWidget(inputBrowseButton);
    inputGroup->setLayout(inputLayout);
    
    // Create output directory group
    QGroupBox *outputGroup = new QGroupBox(config.strings.OUTPUT_DIR_TITLE, this);
    QHBoxLayout *outputLayout = new QHBoxLayout;
    outputDirEdit = new QLineEdit(this);
    outputBrowseButton = new QPushButton(config.strings.BROWSE_BUTTON_TEXT, this);
    outputLayout->addWidget(outputDirEdit);
    outputLayout->addWidget(outputBrowseButton);
    outputGroup->setLayout(outputLayout);
    
    // Create keyword file group
    QGroupBox *keywordGroup = new QGroupBox(config.strings.KEYWORD_FILE_TITLE, this);
    QHBoxLayout *keywordLayout = new QHBoxLayout;
    keywordFileEdit = new QLineEdit(this);
    keywordBrowseButton = new QPushButton(config.strings.BROWSE_BUTTON_TEXT, this);
    keywordLayout->addWidget(keywordFileEdit);
    keywordLayout->addWidget(keywordBrowseButton);
    keywordGroup->setLayout(keywordLayout);
    
    // Create export options group
    QGroupBox *exportGroup = new QGroupBox(config.strings.EXPORT_OPTIONS_TITLE, this);
    QVBoxLayout *exportLayout = new QVBoxLayout;
    exportMonthlySummaryCheck = new QCheckBox(config.strings.MONTHLY_SUMMARY_TEXT, this);
    exportWeeklySummaryCheck = new QCheckBox(config.strings.WEEKLY_SUMMARY_TEXT, this);
    exportFullDatasetCheck = new QCheckBox(config.strings.FULL_DATASET_TEXT, this);
    exportLayout->addWidget(exportMonthlySummaryCheck);
    exportLayout->addWidget(exportWeeklySummaryCheck);
    exportLayout->addWidget(exportFullDatasetCheck);
    exportGroup->setLayout(exportLayout);
    
    // Create process button
    processButton = new QPushButton(config.strings.PROCESS_BUTTON_TEXT, this);
    
    // Create plot buttons
    plotWeeklyButton = new QPushButton(config.strings.PLOT_WEEKLY_TEXT, this);
    plotMonthlyButton = new QPushButton(config.strings.PLOT_MONTHLY_TEXT, this);
    
    // Create visualization group
    QGroupBox *visualizationGroup = new QGroupBox("Visualization", this);
    QHBoxLayout *visualizationLayout = new QHBoxLayout;
    visualizationLayout->addWidget(plotWeeklyButton);
    visualizationLayout->addWidget(plotMonthlyButton);
    visualizationGroup->setLayout(visualizationLayout);
    
    // Add all widgets to main layout
    mainLayout->addWidget(inputGroup);
    mainLayout->addWidget(outputGroup);
    mainLayout->addWidget(keywordGroup);
    mainLayout->addWidget(exportGroup);
    mainLayout->addWidget(processButton);
    mainLayout->addWidget(visualizationGroup);
    mainLayout->addStretch();
    
    // Set default paths and states
    setupDefaultPaths();
    setupDefaultStates();
}

void FinanceCategorisationWindow::setupDefaultPaths() {
    // Navigate from source file location to repository root
    QDir appDir(QString::fromUtf8(__FILE__));
    for (int i = 0; i < 4; ++i) {
        appDir.cdUp();
    }
    
    // Set default values with paths relative to repository root
    inputDirEdit->setText(appDir.absoluteFilePath("input_files"));
    outputDirEdit->setText(appDir.absoluteFilePath("output_files"));
    keywordFileEdit->setText(appDir.absoluteFilePath("config/categorisation_keywords.csv"));
}

void FinanceCategorisationWindow::setupDefaultStates() {
    exportMonthlySummaryCheck->setChecked(true);
    exportWeeklySummaryCheck->setChecked(false);
    exportFullDatasetCheck->setChecked(true);
}

void FinanceCategorisationWindow::createConnections() {
    connect(inputBrowseButton, &QPushButton::clicked, this, &FinanceCategorisationWindow::browseInputDirectory);
    connect(outputBrowseButton, &QPushButton::clicked, this, &FinanceCategorisationWindow::browseOutputDirectory);
    connect(keywordBrowseButton, &QPushButton::clicked, this, &FinanceCategorisationWindow::browseKeywordFile);
    connect(processButton, &QPushButton::clicked, this, &FinanceCategorisationWindow::processFiles);
    connect(plotWeeklyButton, &QPushButton::clicked, this, &FinanceCategorisationWindow::plotWeeklySummary);
    connect(plotMonthlyButton, &QPushButton::clicked, this, &FinanceCategorisationWindow::plotMonthlySummary);
}

// Event Handlers
void FinanceCategorisationWindow::browseInputDirectory() {
    QString dir = QFileDialog::getExistingDirectory(
        this, config.strings.SELECT_INPUT_DIR_TEXT,
        inputDirEdit->text(),
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
    );
    
    if (!dir.isEmpty()) {
        inputDirEdit->setText(dir);
        inputDirectory = dir;
    }
}

void FinanceCategorisationWindow::browseOutputDirectory() {
    QString dir = QFileDialog::getExistingDirectory(
        this, config.strings.SELECT_OUTPUT_DIR_TEXT,
        outputDirEdit->text(),
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
    );
    
    if (!dir.isEmpty()) {
        outputDirEdit->setText(dir);
        outputDirectory = dir;
    }
}

void FinanceCategorisationWindow::browseKeywordFile() {
    QString file = QFileDialog::getOpenFileName(
        this, config.strings.SELECT_KEYWORD_FILE_TEXT,
        keywordFileEdit->text(),
        config.strings.CSV_FILE_FILTER
    );
    
    if (!file.isEmpty()) {
        keywordFileEdit->setText(file);
        keywordFile = file;
    }
}

void FinanceCategorisationWindow::processFiles() {
    try {
        QString inputDir = inputDirEdit->text();
        QString outputDir = outputDirEdit->text();
        QString keywordFile = keywordFileEdit->text();
        
        if (inputDir.isEmpty() || outputDir.isEmpty() || keywordFile.isEmpty()) {
            QMessageBox::warning(this, config.strings.ERROR_TITLE, config.strings.FIELDS_REQUIRED_ERROR);
            return;
        }
        
        FinanceProcessor processor(
            inputDir.toStdString(),
            outputDir.toStdString(),
            keywordFile.toStdString(),
            exportMonthlySummaryCheck->isChecked(),
            exportWeeklySummaryCheck->isChecked(),
            exportFullDatasetCheck->isChecked()
        );
        
        processor.run();
        QMessageBox::information(this, config.strings.SUCCESS_TITLE, config.strings.PROCESS_SUCCESS);
        
    } catch (const std::exception& e) {
        QMessageBox::critical(this, config.strings.ERROR_TITLE, 
                            QString("Processing failed: %1").arg(e.what()));
    }
}

void FinanceCategorisationWindow::setupPlotWindow(QChart* chart, const QString& title) {
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    
    QMainWindow *plotWindow = new QMainWindow(this);
    plotWindow->setCentralWidget(chartView);
    plotWindow->resize(1000, 600);
    plotWindow->setWindowTitle(title);

    // Create coordinate label
    QLabel* coordLabel = new QLabel(plotWindow);
    coordLabel->setAlignment(Qt::AlignLeft);
    coordLabel->setStyleSheet("QLabel { color: black; background-color: rgba(255, 255, 255, 0.8); padding: 5px; border-radius: 3px; margin: 5px; font-weight: bold; }");
    plotWindow->statusBar()->addWidget(coordLabel);

    // Connect mouse tracking
    chartView->setMouseTracking(true);
    chartView->viewport()->installEventFilter(new MouseEventFilter(chart, coordLabel));

    // Get list of categories from chart series
    QStringList categories;
    QMap<QString, QLineSeries*> seriesMap;
    for (QAbstractSeries* series : chart->series()) {
        QLineSeries* lineSeries = qobject_cast<QLineSeries*>(series);
        if (lineSeries) {
            categories << lineSeries->name();
            seriesMap[lineSeries->name()] = lineSeries;
        }
    }

    // Setup category panel
    setupCategoryPanel(plotWindow, categories, seriesMap);
    
    plotWindow->show();
}

void FinanceCategorisationWindow::setupCategoryPanel(QMainWindow* plotWindow, 
    const QStringList& categories, const QMap<QString, QLineSeries*>& series) {
    
    // Create dock widget for categories
    QDockWidget* dock = new QDockWidget("Categories", plotWindow);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    
    // Create widget to hold checkboxes
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
            
            // Create colored square icon
            QPixmap pixmap(16, 16);
            pixmap.fill(color);
            box->setIcon(QIcon(pixmap));
        }
        
        layout->addWidget(box);
        categoryBoxes[category] = box;
        
        // Connect checkbox to visibility update
        connect(box, &QCheckBox::toggled, this, [this, category](bool checked) {
            updateSeriesVisibility(category, checked);
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
    plotWindow->addDockWidget(Qt::RightDockWidgetArea, dock);
}

void FinanceCategorisationWindow::updateSeriesVisibility(const QString& category, bool visible) {
    // Find all plot windows and update the series in each
    for (QWidget* widget : QApplication::topLevelWidgets()) {
        QMainWindow* mainWindow = qobject_cast<QMainWindow*>(widget);
        if (mainWindow && mainWindow != this) {
            QChartView* chartView = qobject_cast<QChartView*>(mainWindow->centralWidget());
            if (chartView) {
                QChart* chart = chartView->chart();
                // Update series visibility
                for (QAbstractSeries* abstractSeries : chart->series()) {
                    QLineSeries* lineSeries = qobject_cast<QLineSeries*>(abstractSeries);
                    if (lineSeries && lineSeries->name() == category) {
                        lineSeries->setVisible(visible);
                        lineSeries->setPointsVisible(visible);
                    }
                }
            }
        }
    }
}

void FinanceCategorisationWindow::plotData(const QString& filePattern, const QString& title, const QString& xAxisTitle) {
    try {
        QString outputDir = outputDirEdit->text();
        if (outputDir.isEmpty()) {
            QMessageBox::warning(this, config.strings.ERROR_TITLE, "Output directory must be specified");
            return;
        }

        QDir dir(outputDir);
        QStringList files = dir.entryList(QStringList() << filePattern, QDir::Files);
        
        if (files.isEmpty()) {
            QMessageBox::warning(this, config.strings.ERROR_TITLE, QString("No %1 files found").arg(title.toLower()));
            return;
        }

        QChart *chart = new QChart();
        chart->setTitle(title);
        chart->setAnimationOptions(QChart::SeriesAnimations);

        // Track overall min/max values for axis scaling
        double maxValue = 0;
        double minValue = std::numeric_limits<double>::max();

        // Create a map to store category-wise series
        QMap<QString, QLineSeries*> categorySeries;

        // Read the single summary file
        QFile csvFile(dir.filePath(files.first()));
        if (!csvFile.open(QIODevice::ReadOnly | QIODevice::Text))
            return;

        QTextStream in(&csvFile);
        
        // Read header to get dates
        QString header = in.readLine();
        QStringList dates = header.split(',');
        dates.removeFirst(); // Remove "Category" column header
        
        // Read each line (category)
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList fields = line.split(',');
            if (fields.size() >= 2) {
                QString category = fields[0];
                QLineSeries *series = new QLineSeries();
                series->setName(category);
                
                // Assign a unique color from a predefined palette
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
                static int colorIndex = 0;
                
                QPen pen = series->pen();
                pen.setColor(colors[colorIndex % 10]);
                pen.setWidth(1);  // Make lines thicker
                series->setPen(pen);
                colorIndex++;

                // Make points visible and set their size
                series->setPointsVisible(true);
                series->setMarkerSize(2);  // Make points larger
                series->setPointLabelsVisible(false);

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

        // Create and setup axes
        QValueAxis *axisY = new QValueAxis();
        double maxRounded = std::ceil(maxValue / 500.0) * 500;
        double minRounded = std::min(0.0, std::floor(minValue / 500.0) * 500);
        axisY->setRange(minRounded, maxRounded);
        axisY->setTickCount((maxRounded - minRounded) / 500 + 1);
        axisY->setLabelFormat("%d");
        axisY->setTitleText("Amount (£)");
        axisY->setGridLineVisible(true);
        chart->addAxis(axisY, Qt::AlignLeft);

        // Create custom x-axis with date labels
        QCategoryAxis *axisX = new QCategoryAxis();
        axisX->setRange(1, dates.size());
        axisX->setTitleText(xAxisTitle);
        axisX->setGridLineVisible(true);
        
        // Add date labels to x-axis
        for (int i = 0; i < dates.size(); ++i) {
            axisX->append(dates[i], i + 1);
        }
        axisX->setLabelsPosition(QCategoryAxis::AxisLabelsPositionOnValue);
        axisX->setLabelsAngle(-65);  // Angle the date labels for better readability
        chart->addAxis(axisX, Qt::AlignBottom);

        // Attach axes to all series
        for (auto series : categorySeries) {
            series->attachAxis(axisX);
            series->attachAxis(axisY);
            
            // Connect hover signals for tooltips
            connect(series, &QLineSeries::hovered, this, [series, dates](const QPointF &point, bool state) {
                if (state) {
                    int index = qRound(point.x()) - 1;
                    if (index >= 0 && index < dates.size()) {
                        QToolTip::showText(QCursor::pos(), 
                            QString("%1\n%2: £%3")
                                .arg(dates[index])
                                .arg(series->name())
                                .arg(point.y(), 0, 'f', 2));
                    }
                }
            });
        }

        chart->legend()->setVisible(true);
        chart->legend()->setAlignment(Qt::AlignRight);

        setupPlotWindow(chart, title);

    } catch (const std::exception& e) {
        QMessageBox::critical(this, config.strings.ERROR_TITLE, 
                            QString("Plot generation failed: %1").arg(e.what()));
    }
}

void FinanceCategorisationWindow::plotWeeklySummary() {
    plotData("weekly_summary.csv", "Weekly Expense Summary Over Time", "Week Number");
}

void FinanceCategorisationWindow::plotMonthlySummary() {
    plotData("monthly_summary.csv", "Monthly Expense Summary Over Time", "Month Number");
}

// Utility Functions
void FinanceCategorisationWindow::showErrorMessage(const QString& message, const char* title) noexcept {
    QMessageBox::critical(nullptr, title, message, QMessageBox::Ok);
}

}  // namespace FinanceManager 
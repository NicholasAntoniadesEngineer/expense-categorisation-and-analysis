/**
 * @file table_window.cpp
 * @brief Implementation of table window for displaying financial data
 * @author Nicholas Antoniades
 * @date 2024-01-24
 */

#include "table_window.hpp"
#include <QFile>
#include <QTextStream>
#include <QHeaderView>
#include <QDir>
#include <QMessageBox>

namespace FinanceManager {

TableWindow::TableWindow(const QString& title, QWidget* parent)
    : QMainWindow(parent)
    , table(new QTableWidget(this))
{
    setWindowTitle(title);
    setupTable();
    styleTable();
}

void TableWindow::setupTable() {
    setCentralWidget(table);
    table->setAlternatingRowColors(true);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->horizontalHeader()->setStretchLastSection(true);
    table->verticalHeader()->setVisible(false);
}

void TableWindow::styleTable() {
    table->setStyleSheet(
        "QTableWidget {"
        "    background-color: white;"
        "    alternate-background-color: #f6f8fa;"
        "    selection-background-color: #0366d6;"
        "    selection-color: white;"
        "    color: #24292e;"  
        "}"
        "QTableWidget::item {"
        "    color: #24292e;"  
        "    padding: 4px;"
        "}"
        "QHeaderView::section {"
        "    background-color: #24292e;"
        "    color: white;"
        "    padding: 5px;"
        "    border: none;"
        "}"

        "QScrollBar:vertical {"
        "    background-color: #24292e;"
        "    width: 12px;"
        "    margin: 0px;"
        "}"
        "QScrollBar::handle:vertical {"
        "    background-color: #6e7681;"  
        "    min-height: 20px;"
        "    border-radius: 6px;"
        "}"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
        "    height: 0px;"
        "}"
        "QScrollBar:horizontal {"
        "    background-color: #24292e;"
        "    height: 12px;"
        "    margin: 0px;"
        "}"
        "QScrollBar::handle:horizontal {"
        "    background-color: #6e7681;"  
        "    min-width: 20px;"
        "    border-radius: 6px;"
        "}"
        "QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal {"
        "    width: 0px;"
        "}"
        "QScrollBar::handle:vertical:hover, QScrollBar::handle:horizontal:hover {"
        "    background-color: #8b949e;" 
        "}"
    );
}

void TableWindow::loadFromCSV(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "Could not open file: " + filePath);
        return;
    }

    QTextStream in(&file);
    QString headerLine = in.readLine();
    QStringList headers = headerLine.split(',');
    
    table->setColumnCount(headers.size());
    table->setHorizontalHeaderLabels(headers);
    
    int row = 0;
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(',');
        
        table->insertRow(row);
        for (int col = 0; col < fields.size(); ++col) {
            QTableWidgetItem* item = new QTableWidgetItem(fields[col]);
            table->setItem(row, col, item);
        }
        ++row;
    }
    
    calculateWindowSize(row, headers.size());
}

void TableWindow::calculateWindowSize(int rowCount, int columnCount) {
    int totalWidth = columnCount * DEFAULT_COLUMN_WIDTH;
    int totalHeight = rowCount * 30 + 100;  
    
    totalWidth = qMin(totalWidth, 1200);
    totalHeight = qMin(totalHeight, 800);
    
    resize(totalWidth, totalHeight);
}

void TableWindow::setInitialSize(int width, int height) {
    resize(width, height);
}

TableWindow* TableWindow::showTableFromFile(TableWindow*& currentWindow,
                                          const QString& outputDir,
                                          const TableConfig& config,
                                          QWidget* parent) {
    QString filePath;
    if (!validateTableData(parent, outputDir, config.fileName, config.title, filePath)) {
        return nullptr;
    }

    if (currentWindow) {
        currentWindow->activateWindow();
        currentWindow->raise();
        return currentWindow;
    }

    currentWindow = new TableWindow(config.title, parent);
    currentWindow->setAttribute(Qt::WA_DeleteOnClose);
    currentWindow->setInitialSize(config.width, config.height);
    currentWindow->loadFromCSV(filePath);
    currentWindow->show();

    return currentWindow;
}

bool TableWindow::validateTableData(QWidget* parent,
                                  const QString& outputDir,
                                  const QString& fileName,
                                  const QString& title,
                                  QString& filePath) {
    filePath = QDir(outputDir).filePath(fileName);
    if (!QFile::exists(filePath)) {
        QMessageBox::warning(parent, "Error",
                           QString("Cannot show %1: File not found at %2")
                               .arg(title)
                               .arg(filePath));
        return false;
    }
    return true;
}

} // namespace FinanceManager 
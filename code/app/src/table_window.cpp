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
#include <QScrollBar>

namespace FinanceManager {

TableWindow::TableWindow(const QString& title, QWidget* parent)
    : QMainWindow(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle(title);
    setupTable();
}

void TableWindow::setupTable() {
    table = new QTableWidget(this);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);  // Read-only
    table->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    table->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setCentralWidget(table);
    styleTable();
}

void TableWindow::styleTable() {
    table->setAlternatingRowColors(true);
    table->setStyleSheet(
        "QTableWidget { gridline-color: #d0d0d0; color: black; background-color: white; }"
        "QHeaderView::section { background-color: #f0f0f0; color: black; padding: 4px; }"
        "QTableWidget::item { padding: 4px; color: black; }"
        "QTableWidget::item:alternate { background-color: #f9f9f9; }"
    );
}

void TableWindow::calculateWindowSize(int rowCount, int columnCount) {
    const int totalWidth = (columnCount * DEFAULT_COLUMN_WIDTH) + 
                          table->verticalHeader()->width() + 
                          table->verticalScrollBar()->sizeHint().width() + 4;
    
    const int totalHeight = (rowCount * table->rowHeight(0)) + 
                           table->horizontalHeader()->height() +
                           table->horizontalScrollBar()->sizeHint().height() + 4;
    
    setMaximumSize(totalWidth, totalHeight);
}

void TableWindow::setInitialSize(int width, int height) {
    resize(qMin(width, maximumWidth()), qMin(height, maximumHeight()));
}

void TableWindow::loadFromCSV(const QString& filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    
    // Read header
    QString header = in.readLine();
    QStringList headers = header.split(',');
    table->setColumnCount(headers.size());
    table->setHorizontalHeaderLabels(headers);
    
    // Read data
    QVector<QStringList> rows;
    while (!in.atEnd()) {
        QString line = in.readLine();
        rows.append(line.split(','));
    }
    
    table->setRowCount(rows.size());
    
    // Populate table
    for (int i = 0; i < rows.size(); ++i) {
        const QStringList &row = rows[i];
        for (int j = 0; j < row.size() && j < headers.size(); ++j) {
            QTableWidgetItem *item = new QTableWidgetItem();
            if (j == 0) {  // Category column
                item->setText(row[j]);
                item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
            } else {  // Value columns
                double value = row[j].toDouble();
                item->setText(QString("£%1").arg(value, 0, 'f', 2));
                item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            }
            table->setItem(i, j, item);
        }
    }
    
    // Set column widths
    for (int i = 0; i < headers.size(); ++i) {
        table->setColumnWidth(i, DEFAULT_COLUMN_WIDTH);
    }
    
    // Enable sorting
    table->setSortingEnabled(true);
    
    // Calculate and set window size
    calculateWindowSize(rows.size(), headers.size());
    
    file.close();
}

} // namespace FinanceManager 
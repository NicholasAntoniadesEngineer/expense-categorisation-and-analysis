/**
 * @file table_window.hpp
 * @brief Table window for displaying financial data in tabular format
 * @author Nicholas Antoniades
 * @date 2024-01-24
 */

#pragma once

#include <QMainWindow>
#include <QTableWidget>
#include <QString>

namespace FinanceManager {

class TableWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit TableWindow(const QString& title, QWidget* parent = nullptr);
    ~TableWindow() = default;

    void loadFromCSV(const QString& filePath);
    void setInitialSize(int width, int height);

private:
    void setupTable();
    void styleTable();
    void calculateWindowSize(int rowCount, int columnCount);

    QTableWidget* table;
    static constexpr int DEFAULT_COLUMN_WIDTH = 120;
}; 

} // namespace FinanceManager 
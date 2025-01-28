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
#include <QWidget>

namespace FinanceManager {

class TableWindow : public QMainWindow {
    Q_OBJECT

public:
    struct TableConfig {
        QString title;
        QString fileName;
        int width;
        int height;
    };

    explicit TableWindow(const QString& title, QWidget* parent = nullptr);
    ~TableWindow() = default;

    void loadFromCSV(const QString& filePath);
    void setInitialSize(int width, int height);

    // Static manager methods
    static TableWindow* showTableFromFile(TableWindow*& currentWindow,
                                        const QString& outputDir,
                                        const TableConfig& config,
                                        QWidget* parent);

private:
    void setupTable();
    void styleTable();
    void calculateWindowSize(int rowCount, int columnCount);

    // Static manager helper methods
    static bool validateTableData(QWidget* parent,
                                const QString& outputDir,
                                const QString& fileName,
                                const QString& title,
                                QString& filePath);

    QTableWidget* table;
    static constexpr int DEFAULT_COLUMN_WIDTH = 120;
}; 

} // namespace FinanceManager 
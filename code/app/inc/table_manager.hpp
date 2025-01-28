/**
 * @file table_manager.hpp
 * @brief Manager class for table windows and data visualization
 * @author Nicholas Antoniades
 * @date 2024-01-28
 */

#pragma once

#include <QString>
#include <QWidget>
#include "table_window.hpp"
#include "window_manager.hpp"

namespace FinanceManager {

class TableManager {
public:
    struct TableConfig {
        QString title;
        QString fileName;
        int width;
        int height;
    };

    static TableWindow* showTableFromFile(TableWindow*& currentWindow,
                                        const QString& outputDir,
                                        const TableConfig& config,
                                        QWidget* parent);

private:
    static bool validateTableData(QWidget* parent,
                                const QString& outputDir,
                                const QString& fileName,
                                const QString& title,
                                QString& filePath);
};

} // namespace FinanceManager 
/**
 * @file table_manager.cpp
 * @brief Implementation of table window and data visualization management
 * @author Nicholas Antoniades
 * @date 2024-01-28
 */

#include "table_manager.hpp"
#include "window_manager.hpp"
#include <QDir>
#include <functional>

namespace FinanceManager {

TableWindow* TableManager::showTableFromFile(TableWindow*& currentWindow,
                                           const QString& outputDir,
                                           const TableConfig& config,
                                           QWidget* parent) {
    QString filePath;
    if (!validateTableData(parent, outputDir, config.fileName, config.title, filePath)) {
        return nullptr;
    }

    return WindowManager::showWindow(currentWindow, config.title, parent,
        std::function<void(TableWindow*)>([filePath, config](TableWindow* window) {
            window->loadFromCSV(filePath);
            window->setInitialSize(config.width, config.height);
            window->setAttribute(Qt::WA_DeleteOnClose);
        }));
}

bool TableManager::validateTableData(QWidget* parent,
                                   const QString& outputDir,
                                   const QString& fileName,
                                   const QString& title,
                                   QString& filePath) {
    if (!WindowManager::checkOutputDirectory(parent, outputDir)) {
        return false;
    }

    filePath = QDir(outputDir).filePath(fileName);
    if (!WindowManager::checkFileExists(parent, filePath, title)) {
        return false;
    }

    return true;
}

} // namespace FinanceManager 
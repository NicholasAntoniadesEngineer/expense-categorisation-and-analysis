/**
 * @file window_manager.cpp
 * @brief Implementation of generic window management functionality
 * @author Nicholas Antoniades
 * @date 2024-01-28
 */

#include "window_manager.hpp"
#include <QMessageBox>
#include <QFile>

namespace FinanceManager {

bool WindowManager::checkOutputDirectory(QWidget* parent, const QString& outputDir) {
    if (outputDir.isEmpty()) {
        showErrorMessage(parent, "Output directory must be specified");
        return false;
    }
    return true;
}

bool WindowManager::checkFileExists(QWidget* parent, const QString& filePath, const QString& fileDescription) {
    if (!QFile::exists(filePath)) {
        showErrorMessage(parent, fileDescription + " file not found");
        return false;
    }
    return true;
}

void WindowManager::showErrorMessage(QWidget* parent, const QString& message) {
    QMessageBox::warning(parent, "Error", message);
}

} // namespace FinanceManager 
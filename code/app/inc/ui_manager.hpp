/**
 * @file ui_manager.hpp
 * @brief Manager class for UI setup and configuration
 * @author Nicholas Antoniades
 * @date 2024-01-28
 */

#pragma once

#include <QString>
#include <QWidget>
#include <QGroupBox>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include "app_config.hpp"

namespace FinanceManager {

class UIManager {
public:
    struct DirectoryGroup {
        QGroupBox* group;
        QLineEdit* pathEdit;
        QPushButton* browseButton;
    };

    struct ExportGroup {
        QGroupBox* group;
        QCheckBox* monthlyCheck;
        QCheckBox* weeklyCheck;
        QCheckBox* fullDatasetCheck;
    };

    struct FileDialogConfig {
        QString title;
        QString currentPath;
        QString filter;
        bool isDirectory;
    };

    static bool initializeApplicationInfo(const AppConfig& config) noexcept;
    static bool initializeAppearance(const AppConfig& config) noexcept;
    static bool setupMainWindow(QWidget* window, const AppConfig& config) noexcept;
    static void showErrorMessage(const QString& message, const char* title) noexcept;

    // UI Component Creation
    static DirectoryGroup createDirectoryGroup(const QString& title, 
                                             const QString& buttonText,
                                             QWidget* parent);
    static ExportGroup createExportGroup(const QString& title,
                                       const AppConfig& config,
                                       QWidget* parent);
    static QPushButton* createActionButton(const QString& text,
                                         QWidget* parent);

    // File Dialog Operations
    static QString showFileDialog(QWidget* parent, const FileDialogConfig& config);

private:
    static bool setupStyle(const AppConfig& config) noexcept;
    static bool setupFont(const AppConfig& config) noexcept;
    static bool setupIcon() noexcept;
};

} // namespace FinanceManager 
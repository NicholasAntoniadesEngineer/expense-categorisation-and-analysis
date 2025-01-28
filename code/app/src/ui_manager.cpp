/**
 * @file ui_manager.cpp
 * @brief Implementation of UI setup and configuration management
 * @author Nicholas Antoniades
 * @date 2024-01-28
 */

#include "ui_manager.hpp"
#include <QApplication>
#include <QStyle>
#include <QStyleFactory>
#include <QMessageBox>
#include <QFont>
#include <QIcon>
#include <QMainWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>

namespace FinanceManager {

bool UIManager::initializeApplicationInfo(const AppConfig& config) noexcept {
    QApplication::setApplicationName(config.app_name);
    QApplication::setOrganizationName(config.org_name);
    QApplication::setOrganizationDomain(config.org_domain);
    QApplication::setApplicationVersion(config.app_version);
    QApplication::setApplicationDisplayName(config.app_name);
    return true;
}

bool UIManager::initializeAppearance(const AppConfig& config) noexcept {
    return setupStyle(config) && setupFont(config) && setupIcon();
}

bool UIManager::setupMainWindow(QWidget* window, const AppConfig& config) noexcept {
    if (!window) return false;
    
    window->setWindowTitle(config.app_name);
    window->resize(config.default_window_width, config.default_window_height);
    window->show();
    return true;
}

bool UIManager::setupStyle(const AppConfig& config) noexcept {
    QStyle* style = QStyleFactory::create(config.style_name);
    if (!style) {
        return false;
    }
    QApplication::setStyle(style);
    return true;
}

bool UIManager::setupFont(const AppConfig& config) noexcept {
    // Use ".AppleSystemUIFont" which is guaranteed to be available on macOS
    QFont appFont(".AppleSystemUIFont", config.default_font_size);
    QApplication::setFont(appFont);
    return true;
}

bool UIManager::setupIcon() noexcept {
    QIcon appIcon(":/icons/app_icon.png");
    if (appIcon.isNull()) {
        return false;
    }
    QApplication::setWindowIcon(appIcon);
    return true;
}

void UIManager::showErrorMessage(const QString& message, const char* title) noexcept {
    QMessageBox::critical(nullptr, title, message, QMessageBox::Ok);
}

UIManager::DirectoryGroup UIManager::createDirectoryGroup(const QString& title,
                                                        const QString& buttonText,
                                                        QWidget* parent) {
    DirectoryGroup group;
    
    group.group = new QGroupBox(title, parent);
    QHBoxLayout* layout = new QHBoxLayout;
    
    group.pathEdit = new QLineEdit(parent);
    group.browseButton = new QPushButton(buttonText, parent);
    
    layout->addWidget(group.pathEdit);
    layout->addWidget(group.browseButton);
    
    group.group->setLayout(layout);
    return group;
}

UIManager::ExportGroup UIManager::createExportGroup(const QString& title,
                                                  const AppConfig& config,
                                                  QWidget* parent) {
    ExportGroup group;
    
    group.group = new QGroupBox(title, parent);
    QVBoxLayout* layout = new QVBoxLayout;
    
    group.weeklyCheck = new QCheckBox(config.strings.WEEKLY_SUMMARY_TEXT, parent);
    group.monthlyCheck = new QCheckBox(config.strings.MONTHLY_SUMMARY_TEXT, parent);
    group.fullDatasetCheck = new QCheckBox(config.strings.FULL_DATASET_TEXT, parent);
    
    layout->addWidget(group.weeklyCheck);
    layout->addWidget(group.monthlyCheck);
    layout->addWidget(group.fullDatasetCheck);
    
    group.group->setLayout(layout);
    return group;
}

QPushButton* UIManager::createActionButton(const QString& text, QWidget* parent) {
    QPushButton* button = new QPushButton(text, parent);
    button->setMinimumWidth(100);  // Ensure reasonable minimum width
    return button;
}

QString UIManager::showFileDialog(QWidget* parent, const FileDialogConfig& config) {
    if (config.isDirectory) {
        return QFileDialog::getExistingDirectory(
            parent,
            config.title,
            config.currentPath,
            QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
        );
    } else {
        return QFileDialog::getOpenFileName(
            parent,
            config.title,
            config.currentPath,
            config.filter
        );
    }
}

} // namespace FinanceManager 
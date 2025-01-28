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
#include <QPalette>
#include <QColor>
#include <QGuiApplication>
#include <QScreen>

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
    window->show();  // Show the window first
    
    // Now position the window after it's shown and has its final size
    QScreen* screen = QGuiApplication::primaryScreen();
    if (screen) {
        QRect screenGeometry = screen->availableGeometry();  // Use available geometry instead of full geometry
        
        // Calculate position to place window at bottom center
        int x = (screenGeometry.width() - window->frameGeometry().width()) / 2;
        int y = screenGeometry.height() - window->frameGeometry().height() - 50;  // 50 pixels from bottom
        
        window->move(x, y);  // Use move instead of setGeometry since we already set the size
    }
    
    return true;
}

bool UIManager::setupStyle(const AppConfig& config) noexcept {
    QStyle* style = QStyleFactory::create(config.style_name);
    if (!style) {
        return false;
    }
    QApplication::setStyle(style);
    
    // Apply dark theme palette
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(60, 60, 60));            // 20% lighter dark background
    darkPalette.setColor(QPalette::WindowText, QColor(230, 230, 230));     // Lighter text
    darkPalette.setColor(QPalette::Base, QColor(70, 70, 70));             // Lighter base
    darkPalette.setColor(QPalette::AlternateBase, QColor(65, 65, 65));
    darkPalette.setColor(QPalette::ToolTipBase, QColor(70, 70, 70));
    darkPalette.setColor(QPalette::ToolTipText, QColor(230, 230, 230));
    darkPalette.setColor(QPalette::Text, QColor(230, 230, 230));
    darkPalette.setColor(QPalette::Button, QColor(80, 80, 80));           // Lighter buttons
    darkPalette.setColor(QPalette::ButtonText, QColor(230, 230, 230));
    darkPalette.setColor(QPalette::Link, QColor(180, 180, 180));          // Grey instead of blue
    darkPalette.setColor(QPalette::Highlight, QColor(180, 180, 180));
    darkPalette.setColor(QPalette::HighlightedText, QColor(255, 255, 255));

    QApplication::setPalette(darkPalette);
    
    // Apply stylesheet for custom styling
    QString styleSheet = R"(
        QMainWindow {
            background-color: #3C3C3C;
        }
        QGroupBox {
            background-color: #484848;
            border: 1px solid #606060;
            border-radius: 6px;
            margin-top: 1em;
            padding: 12px;
            color: #E6E6E6;
        }
        QGroupBox::title {
            color: #E6E6E6;
            subcontrol-origin: margin;
            left: 10px;
            padding: 0 3px;
            background-color: #484848;
        }
        QPushButton {
            background-color: #525252;
            color: #E6E6E6;
            border: 1px solid #606060;
            border-radius: 4px;
            padding: 6px 12px;
            min-width: 80px;
        }
        QPushButton#processButton {
            width: 600px;
            min-width: 600px;
            max-width: 600px;
        }
        QPushButton:hover {
            background-color: #5E5E5E;
            border: 1px solid #707070;
        }
        QPushButton:pressed {
            background-color: #484848;
            border: 1px solid #606060;
        }
        QLineEdit {
            padding: 6px;
            border: 1px solid #606060;
            border-radius: 4px;
            background-color: #484848;
            color: #E6E6E6;
        }
        QLineEdit:focus {
            border: 1px solid #B4B4B4;
            background-color: #525252;
        }
        QCheckBox {
            spacing: 8px;
            color: #E6E6E6;
        }
        QCheckBox::indicator {
            width: 18px;
            height: 18px;
        }
        QCheckBox::indicator:unchecked {
            border: 2px solid #606060;
            background-color: #484848;
            border-radius: 3px;
        }
        QCheckBox::indicator:checked {
            border: 2px solid #B4B4B4;
            background-color: #484848;
            border-radius: 3px;
            image: url(data:image/svg+xml,%3Csvg xmlns='http://www.w3.org/2000/svg' width='14' height='14' viewBox='0 0 14 14'%3E%3Cpath d='M11.7,2.3L8.7,5.3L5.7,2.3C5.3,1.9,4.7,1.9,4.3,2.3C3.9,2.7,3.9,3.3,4.3,3.7l3,3l-3,3c-0.4,0.4-0.4,1,0,1.4 c0.2,0.2,0.5,0.3,0.7,0.3s0.5-0.1,0.7-0.3l3-3l3,3c0.2,0.2,0.5,0.3,0.7,0.3s0.5-0.1,0.7-0.3c0.4-0.4,0.4-1,0-1.4l-3-3l3-3 c0.4-0.4,0.4-1,0-1.4C12.7,1.9,12.1,1.9,11.7,2.3z' fill='%23B4B4B4'/%3E%3C/svg%3E);
        }
    )";
    
    qApp->setStyleSheet(styleSheet);
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
    if (text == "Process Files") {  // Using string literal instead of config
        button->setObjectName("processButton");
    }
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
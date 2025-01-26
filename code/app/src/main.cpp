/**
 * @file main.cpp
 * @brief Main entry point for the Finance Manager application
 * @author Nicholas Antoniades
 * @date 2024-01-24
 * 
 * This Qt-based application provides a GUI interface for categorizing
 * and analyzing financial transactions.
 * 
 */

#include "finance_categorisation_window.hpp"
#include <QApplication>
#include <QGuiApplication>
#include <QIcon>
#include <QFont>
#include <QStyle>
#include <QStyleFactory>
#include <QString>
#include <QMessageBox>

namespace FinanceManager {

/**
 * @brief Application configuration constants
 */
struct AppConfig {
    static constexpr int DEFAULT_WINDOW_WIDTH = 600;
    static constexpr int DEFAULT_WINDOW_HEIGHT = 400;
    static constexpr int DEFAULT_FONT_SIZE = 12;
    static const char* const APP_NAME;
    static const char* const ORG_NAME;
    static const char* const ORG_DOMAIN;
    static const char* const APP_VERSION;
    static const char* const STYLE_NAME;
    static const char* const FONT_FAMILY;
};

const char* const AppConfig::APP_NAME = "Finance Manager";
const char* const AppConfig::ORG_NAME = "Finance";
const char* const AppConfig::ORG_DOMAIN = "finance.manager";
const char* const AppConfig::APP_VERSION = "1.0.0";
const char* const AppConfig::STYLE_NAME = "fusion";
const char* const AppConfig::FONT_FAMILY = "SF Pro Display";

/**
 * @brief Initialize application appearance settings
 * @return true if initialization successful, false otherwise
 */
bool initializeAppearance() noexcept {
    // Set application style
    QStyle* style = QStyleFactory::create(AppConfig::STYLE_NAME);
    if (!style) {
        return false;
    }
    QApplication::setStyle(style);

    // Set application font
    QFont appFont(AppConfig::FONT_FAMILY, AppConfig::DEFAULT_FONT_SIZE);
    QApplication::setFont(appFont);
    
    // Set application icon
    QIcon appIcon(":/icons/app_icon.png");
    if (!appIcon.isNull()) {
        QApplication::setWindowIcon(appIcon);
    }
    
    return true;
}

/**
 * @brief Initialize application metadata and information
 * @return true if initialization successful, false otherwise
 */
bool initializeApplicationInfo() noexcept {
    QApplication::setApplicationName(AppConfig::APP_NAME);
    QApplication::setOrganizationName(AppConfig::ORG_NAME);
    QApplication::setOrganizationDomain(AppConfig::ORG_DOMAIN);
    QApplication::setApplicationVersion(AppConfig::APP_VERSION);
    QApplication::setApplicationDisplayName(AppConfig::APP_NAME);
    return true;
}

/**
 * @brief Configure and display the main application window
 * @param window Reference to the main window instance
 * @return true if setup successful, false otherwise
 */
bool setupMainWindow(FinanceCategorisationWindow& window) noexcept {
    window.setWindowTitle(AppConfig::APP_NAME);
    window.resize(AppConfig::DEFAULT_WINDOW_WIDTH, 
                 AppConfig::DEFAULT_WINDOW_HEIGHT);
    window.show();
    return true;
}

/**
 * @brief Display an error message to the user
 * @param message The error message to display
 */
void showErrorMessage(const QString& message) noexcept {
    QMessageBox::critical(nullptr, 
                         AppConfig::APP_NAME, 
                         message,
                         QMessageBox::Ok);
}

}  // namespace FinanceManager

/**
 * @brief Application entry point
 * @param argc Number of command line arguments
 * @param argv Array of command line arguments
 * @return EXIT_SUCCESS on successful execution, EXIT_FAILURE otherwise
 */
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Initialize application components
    if (!FinanceManager::initializeApplicationInfo() || 
        !FinanceManager::initializeAppearance()) {
        FinanceManager::showErrorMessage("Failed to initialize application");
        return EXIT_FAILURE;
    }

    // Create and setup main window
    FinanceManager::FinanceCategorisationWindow window;
    if (!FinanceManager::setupMainWindow(window)) {
        FinanceManager::showErrorMessage("Failed to setup main window");
        return EXIT_FAILURE;
    }

    // Start the application event loop
    return app.exec();
} 
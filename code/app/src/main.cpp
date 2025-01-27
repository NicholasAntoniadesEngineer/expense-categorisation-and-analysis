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
 * @brief Initialize application appearance settings
 * @param config Application configuration
 * @return true if initialization successful, false otherwise
 */
bool initializeAppearance(const AppConfig& config) noexcept {
    // Set application style
    QStyle* style = QStyleFactory::create(config.style_name);
    if (!style) {
        return false;
    }
    QApplication::setStyle(style);

    // Set application font
    QFont appFont(config.font_family, config.default_font_size);
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
 * @param config Application configuration
 * @return true if initialization successful, false otherwise
 */
bool initializeApplicationInfo(const AppConfig& config) noexcept {
    QApplication::setApplicationName(config.app_name);
    QApplication::setOrganizationName(config.org_name);
    QApplication::setOrganizationDomain(config.org_domain);
    QApplication::setApplicationVersion(config.app_version);
    QApplication::setApplicationDisplayName(config.app_name);
    return true;
}

/**
 * @brief Configure and display the main application window
 * @param window Reference to the main window instance
 * @param config Application configuration
 * @return true if setup successful, false otherwise
 */
bool setupMainWindow(FinanceCategorisationWindow& window, const AppConfig& config) noexcept {
    window.setWindowTitle(config.app_name);
    window.resize(config.default_window_width, config.default_window_height);
    window.show();
    return true;
}

/**
 * @brief Display an error message to the user
 * @param message The error message to display
 * @param config Application configuration
 */
void showErrorMessage(const QString& message, const AppConfig& config) noexcept {
    QMessageBox::critical(nullptr, config.app_name, message, QMessageBox::Ok);
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

    // Initialize application configuration
    const FinanceManager::AppConfig config{
        .app_name = "Finance Manager",
        .org_name = "Finance",
        .org_domain = "finance.manager",
        .app_version = "1.0.0",
        .style_name = "fusion",
        .font_family = "SF Pro Display",
        .default_window_width = 600,
        .default_window_height = 400,
        .default_font_size = 12
    };

    // Initialize application components
    if (!FinanceManager::initializeApplicationInfo(config) || 
        !FinanceManager::initializeAppearance(config)) {
        FinanceManager::showErrorMessage("Failed to initialize application", config);
        return EXIT_FAILURE;
    }

    // Create and setup main window
    FinanceManager::FinanceCategorisationWindow window;
    if (!FinanceManager::setupMainWindow(window, config)) {
        FinanceManager::showErrorMessage("Failed to setup main window", config);
        return EXIT_FAILURE;
    }

    // Start the application event loop
    return app.exec();
} 
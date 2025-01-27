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

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Initialize application configuration
    FinanceManager::AppConfig config{
        .app_name = "Finance Manager",
        .org_name = "Finance",
        .org_domain = "finance.manager",
        .app_version = "0.1.1",
        .style_name = "fusion",
        .font_family = "SF Pro Display",
        .default_window_width = 600,
        .default_window_height = 400,
        .default_font_size = 12
    };

    // Create and initialize main window
    FinanceManager::FinanceCategorisationWindow window(config);
    
    if (!window.initializeApplicationInfo() || !window.initializeAppearance()) {
        FinanceManager::FinanceCategorisationWindow::showErrorMessage(
            "Failed to initialize application", 
            config.app_name
        );
        return EXIT_FAILURE;
    }

    if (!window.setupWindow()) {
        FinanceManager::FinanceCategorisationWindow::showErrorMessage(
            "Failed to setup main window",
            config.app_name
        );
        return EXIT_FAILURE;
    }

    // Start the application event loop
    return app.exec();
} 
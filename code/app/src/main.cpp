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

#include "main_window.hpp"
#include "app_config.hpp"
#include <QApplication>

using namespace FinanceManager;

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Initialize application configuration
    AppConfig config = AppConfig::getDefaultConfig();

    // Create and initialize main window
    MainWindow window(config);
    
    if (!window.initializeApplicationInfo() || !window.initializeAppearance()) {
        MainWindow::showErrorMessage(
            "Failed to initialize application", 
            config.app_name
        );
        return EXIT_FAILURE;
    }

    if (!window.setupWindow()) {
        MainWindow::showErrorMessage(
            "Failed to setup main window",
            config.app_name
        );
        return EXIT_FAILURE;
    }

    // Start the application event loop
    return app.exec();
} 
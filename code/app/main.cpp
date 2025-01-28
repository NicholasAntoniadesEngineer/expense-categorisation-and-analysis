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

    AppConfig config = AppConfig::getDefaultConfig();

    MainWindow mainWindow(config);
    
    if (!mainWindow.initializeApplicationInfo() || !mainWindow.initializeAppearance()) {
        MainWindow::showErrorMessage(
            "Failed to initialize application", 
            config.app_name
        );
        return EXIT_FAILURE;
    }

    if (!mainWindow.setupWindow()) {
        MainWindow::showErrorMessage(
            "Failed to setup main window",
            config.app_name
        );
        return EXIT_FAILURE;
    }

    return app.exec();
} 
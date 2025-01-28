/**
 * @file window_manager.hpp
 * @brief Generic window management functionality
 * @author Nicholas Antoniades
 * @date 2024-01-28
 */

#pragma once

#include <QString>
#include <QWidget>
#include "plot_window.hpp"
#include "table_window.hpp"

namespace FinanceManager {

class WindowManager {
public:
    static bool checkOutputDirectory(QWidget* parent, const QString& outputDir);
    static bool checkFileExists(QWidget* parent, const QString& filePath, const QString& fileDescription);
    
    template<typename WindowType>
    static WindowType* showWindow(WindowType* currentWindow,
                                const QString& title,
                                QWidget* parent,
                                const std::function<void(WindowType*)>& setupFunc = nullptr) {
        if (currentWindow) {
            currentWindow->activateWindow();
            currentWindow->raise();
            return currentWindow;
        }

        WindowType* window = new WindowType(title, parent);
        if (setupFunc) {
            setupFunc(window);
        }
        window->show();
        return window;
    }

    static void showErrorMessage(QWidget* parent, const QString& message);
};

} // namespace FinanceManager 
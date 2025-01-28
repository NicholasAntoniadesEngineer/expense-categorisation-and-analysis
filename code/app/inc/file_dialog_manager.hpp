#pragma once

#include <QString>
#include <QWidget>
#include <QLineEdit>
#include "app_config.hpp"

namespace FinanceManager {

class FileDialogManager {
public:
    struct DialogConfig {
        QString title;
        QString currentPath;
        QString filter;
        bool isDirectory;
    };

    static QString browseForPath(QWidget* parent, 
                               const DialogConfig& config, 
                               QString& targetPath,
                               QLineEdit* targetEdit);

    static QString browseForDirectory(QWidget* parent,
                                    const QString& title,
                                    const QString& currentPath,
                                    QString& targetPath,
                                    QLineEdit* targetEdit);

    static QString browseForFile(QWidget* parent,
                               const QString& title,
                               const QString& currentPath,
                               const QString& filter,
                               QLineEdit* targetEdit);
};

} // namespace FinanceManager 
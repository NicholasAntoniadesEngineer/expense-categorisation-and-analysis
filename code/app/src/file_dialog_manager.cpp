#include "file_dialog_manager.hpp"
#include <QFileDialog>
#include <QLineEdit>

namespace FinanceManager {

QString FileDialogManager::browseForPath(QWidget* parent,
                                       const DialogConfig& config,
                                       QString& targetPath,
                                       QLineEdit* targetEdit) {
    QString path;
    if (config.isDirectory) {
        path = QFileDialog::getExistingDirectory(
            parent,
            config.title,
            config.currentPath,
            QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
        );
    } else {
        path = QFileDialog::getOpenFileName(
            parent,
            config.title,
            config.currentPath,
            config.filter
        );
    }

    if (!path.isEmpty()) {
        targetPath = path;
        if (targetEdit) {
            targetEdit->setText(path);
        }
    }

    return path;
}

QString FileDialogManager::browseForDirectory(QWidget* parent,
                                            const QString& title,
                                            const QString& currentPath,
                                            QString& targetPath,
                                            QLineEdit* targetEdit) {
    DialogConfig config{
        .title = title,
        .currentPath = currentPath,
        .filter = "",
        .isDirectory = true
    };
    return browseForPath(parent, config, targetPath, targetEdit);
}

QString FileDialogManager::browseForFile(QWidget* parent,
                                       const QString& title,
                                       const QString& currentPath,
                                       const QString& filter,
                                       QLineEdit* targetEdit) {
    DialogConfig config{
        .title = title,
        .currentPath = currentPath,
        .filter = filter,
        .isDirectory = false
    };
    QString dummyPath;  
    return browseForPath(parent, config, dummyPath, targetEdit);
}

} // namespace FinanceManager 
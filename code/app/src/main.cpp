#include "main_window.hpp"
#include <QApplication>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // Set application-wide properties
    QApplication::setApplicationName("Finance Categorisation");
    QApplication::setOrganizationName("Finance");
    QApplication::setOrganizationDomain("finance.categorisation");
    
    // Set application icon early
    QIcon appIcon(":/icons/app_icon.png");
    if (!appIcon.isNull()) {
        QApplication::setWindowIcon(appIcon);
    }
    
    MainWindow window;
    window.resize(600, 400);
    window.show();
    
    return app.exec();
} 
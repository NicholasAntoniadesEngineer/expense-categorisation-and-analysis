#pragma once

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QFileDialog>
#include <QString>
#include <QCheckBox>

namespace FinanceManager {

class FinanceCategorisationWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit FinanceCategorisationWindow(QWidget *parent = nullptr);
    ~FinanceCategorisationWindow() = default;

private slots:
    void browseInputDirectory();
    void browseOutputDirectory();
    void browseKeywordFile();
    void processFiles();

private:
    void setupUi();
    void createConnections();

    QWidget *centralWidget;
    QVBoxLayout *mainLayout;
    
    QLineEdit *inputDirEdit;
    QLineEdit *outputDirEdit;
    QLineEdit *keywordFileEdit;
    
    QPushButton *inputBrowseButton;
    QPushButton *outputBrowseButton;
    QPushButton *keywordBrowseButton;
    QPushButton *processButton;

    QCheckBox *exportMonthlySummaryCheck;
    QCheckBox *exportWeeklySummaryCheck;
    QCheckBox *exportFullDatasetCheck;

    QString inputDirectory;
    QString outputDirectory;
    QString keywordFile;
};

}  // namespace FinanceManager 
#pragma once

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QFileDialog>
#include <QString>
#include <QCheckBox>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() = default;

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
#include "finance_categorisation_window.hpp"
#include "finance_processor.hpp"
#include <QMessageBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QDir>
#include <QIcon>
#include <QDebug>
#include <QApplication>
#include <QStyle>

namespace FinanceManager {

FinanceCategorisationWindow::FinanceCategorisationWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi();
    createConnections();
    setWindowTitle(QApplication::applicationDisplayName());
    
    // Set window icon - try multiple approaches
    QIcon appIcon(":/icons/app_icon.png");
    if (appIcon.isNull()) {
        qDebug() << "Failed to load application icon from resources";
        // Try loading with absolute path
        QString iconPath = QDir(QCoreApplication::applicationDirPath())
                          .absoluteFilePath("../resources/icons/app_icon.png");
        appIcon = QIcon(iconPath);
        if (appIcon.isNull()) {
            qDebug() << "Failed to load icon from:" << iconPath;
        }
    } else {
        qDebug() << "Successfully loaded application icon from resources";
    }
    
    if (!appIcon.isNull()) {
        // Set icon in multiple ways
        setWindowIcon(appIcon);
        QApplication::setWindowIcon(appIcon);
        
#ifdef Q_OS_MAC
        // macOS specific icon settings
        QIcon::setThemeName("macos");
#endif
    }
}

void FinanceCategorisationWindow::setupUi()
{
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    mainLayout = new QVBoxLayout(centralWidget);
    
    // Create input directory group
    QGroupBox *inputGroup = new QGroupBox("Input Directory", this);
    QHBoxLayout *inputLayout = new QHBoxLayout;
    inputDirEdit = new QLineEdit(this);
    inputBrowseButton = new QPushButton("Browse", this);
    inputLayout->addWidget(inputDirEdit);
    inputLayout->addWidget(inputBrowseButton);
    inputGroup->setLayout(inputLayout);
    
    // Create output directory group
    QGroupBox *outputGroup = new QGroupBox("Output Directory", this);
    QHBoxLayout *outputLayout = new QHBoxLayout;
    outputDirEdit = new QLineEdit(this);
    outputBrowseButton = new QPushButton("Browse", this);
    outputLayout->addWidget(outputDirEdit);
    outputLayout->addWidget(outputBrowseButton);
    outputGroup->setLayout(outputLayout);
    
    // Create keyword file group
    QGroupBox *keywordGroup = new QGroupBox("Keyword File", this);
    QHBoxLayout *keywordLayout = new QHBoxLayout;
    keywordFileEdit = new QLineEdit(this);
    keywordBrowseButton = new QPushButton("Browse", this);
    keywordLayout->addWidget(keywordFileEdit);
    keywordLayout->addWidget(keywordBrowseButton);
    keywordGroup->setLayout(keywordLayout);
    
    // Create export options group
    QGroupBox *exportGroup = new QGroupBox("Export Options", this);
    QVBoxLayout *exportLayout = new QVBoxLayout;
    exportMonthlySummaryCheck = new QCheckBox("Export Monthly Summary", this);
    exportWeeklySummaryCheck = new QCheckBox("Export Weekly Summary", this);
    exportFullDatasetCheck = new QCheckBox("Export Full Dataset", this);
    exportLayout->addWidget(exportMonthlySummaryCheck);
    exportLayout->addWidget(exportWeeklySummaryCheck);
    exportLayout->addWidget(exportFullDatasetCheck);
    exportGroup->setLayout(exportLayout);
    
    // Create process button
    processButton = new QPushButton("Process Files", this);
    
    // Add all widgets to main layout
    mainLayout->addWidget(inputGroup);
    mainLayout->addWidget(outputGroup);
    mainLayout->addWidget(keywordGroup);
    mainLayout->addWidget(exportGroup);
    mainLayout->addWidget(processButton);
    mainLayout->addStretch();
    
    // Navigate from source file location to repository root
    QDir appDir(QString::fromUtf8(__FILE__));
    for (int i = 0; i < 5; ++i) {
        appDir.cdUp();
    }
    
    // Set default values with paths relative to repository root
    inputDirEdit->setText(appDir.absoluteFilePath("input_files"));
    outputDirEdit->setText(appDir.absoluteFilePath("output_files"));


    keywordFileEdit->setText(appDir.absoluteFilePath("config/categorisation_keywords.csv"));
    
    // Set default checkbox states
    exportMonthlySummaryCheck->setChecked(true);
    exportWeeklySummaryCheck->setChecked(false);
    exportFullDatasetCheck->setChecked(true);
}

void FinanceCategorisationWindow::createConnections()
{
    connect(inputBrowseButton, &QPushButton::clicked, this, &FinanceCategorisationWindow::browseInputDirectory);
    connect(outputBrowseButton, &QPushButton::clicked, this, &FinanceCategorisationWindow::browseOutputDirectory);
    connect(keywordBrowseButton, &QPushButton::clicked, this, &FinanceCategorisationWindow::browseKeywordFile);
    connect(processButton, &QPushButton::clicked, this, &FinanceCategorisationWindow::processFiles);
}

void FinanceCategorisationWindow::browseInputDirectory()
{
    QString dir = QFileDialog::getExistingDirectory(this, "Select Input Directory",
                                                  inputDirEdit->text(),
                                                  QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (!dir.isEmpty()) {
        inputDirEdit->setText(dir);
        inputDirectory = dir;
    }
}

void FinanceCategorisationWindow::browseOutputDirectory()
{
    QString dir = QFileDialog::getExistingDirectory(this, "Select Output Directory",
                                                  outputDirEdit->text(),
                                                  QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (!dir.isEmpty()) {
        outputDirEdit->setText(dir);
        outputDirectory = dir;
    }
}

void FinanceCategorisationWindow::browseKeywordFile()
{
    QString file = QFileDialog::getOpenFileName(this, "Select Keyword File",
                                              keywordFileEdit->text(),
                                              "CSV Files (*.csv);;All Files (*)");
    if (!file.isEmpty()) {
        keywordFileEdit->setText(file);
        keywordFile = file;
    }
}

void FinanceCategorisationWindow::processFiles()
{
    try {
        QString inputDir = inputDirEdit->text();
        QString outputDir = outputDirEdit->text();
        QString keywordFile = keywordFileEdit->text();
        
        if (inputDir.isEmpty() || outputDir.isEmpty() || keywordFile.isEmpty()) {
            QMessageBox::warning(this, "Error", "All fields must be filled");
            return;
        }
        
        FinanceProcessor processor(
            inputDir.toStdString(),
            outputDir.toStdString(),
            keywordFile.toStdString(),
            exportMonthlySummaryCheck->isChecked(),
            exportWeeklySummaryCheck->isChecked(),
            exportFullDatasetCheck->isChecked()
        );
        
        processor.run();
        
        QMessageBox::information(this, "Success", "Files processed successfully!");
        
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", QString("Processing failed: %1").arg(e.what()));
    }
}

}  // namespace FinanceManager 
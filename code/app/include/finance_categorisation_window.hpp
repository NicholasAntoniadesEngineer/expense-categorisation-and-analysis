    private:
        void setupUi();
        void setupDefaultPaths();
        void setupDefaultStates();
        void createConnections();
        void setupPlotWindow(QChart* chart, const QString& title);
        void setupCategoryPanel(QMainWindow* plotWindow, const QStringList& categories, const QMap<QString, QLineSeries*>& series);
        void updateSeriesVisibility(const QString& category, bool visible);
        void plotData(const QString& filePattern, const QString& title, const QString& xAxisTitle);

        // Member variables
        AppConfig& config; 
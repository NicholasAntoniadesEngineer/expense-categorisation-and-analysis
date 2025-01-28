/**
 * @file app_config.hpp
 * @brief Application configuration and initialization
 * @author Nicholas Antoniades
 * @date 2024-01-24
 */

#pragma once

namespace FinanceManager {

/**
 * @brief Application configuration settings
 */
struct AppConfig {
    // Application metadata
    const char* app_name;
    const char* org_name;
    const char* org_domain;
    const char* app_version;
    const char* style_name;
    const char* font_family;
    int default_window_width;
    int default_window_height;
    int default_font_size;

    // UI strings
    struct UIStrings {
        const char* INPUT_DIR_TITLE;
        const char* OUTPUT_DIR_TITLE;
        const char* KEYWORD_FILE_TITLE;
        const char* EXPORT_OPTIONS_TITLE;
        const char* BROWSE_BUTTON_TEXT;
        const char* PROCESS_BUTTON_TEXT;
        const char* MONTHLY_SUMMARY_TEXT;
        const char* WEEKLY_SUMMARY_TEXT;
        const char* FULL_DATASET_TEXT;
        const char* SELECT_INPUT_DIR_TEXT;
        const char* SELECT_OUTPUT_DIR_TEXT;
        const char* SELECT_KEYWORD_FILE_TEXT;
        const char* CSV_FILE_FILTER;
        const char* ERROR_TITLE;
        const char* SUCCESS_TITLE;
        const char* FIELDS_REQUIRED_ERROR;
        const char* PROCESS_SUCCESS;
        const char* PLOT_WEEKLY_TEXT;
        const char* PLOT_MONTHLY_TEXT;
    } strings;

    /**
     * @brief Get default application configuration
     * @return Default AppConfig instance
     */
    static AppConfig getDefaultConfig();
};

} // namespace FinanceManager 
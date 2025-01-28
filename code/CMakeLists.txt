cmake_minimum_required(VERSION 3.16)
project(FinanceManager)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# macOS specific settings
if(APPLE)
    set(MACOSX_BUNDLE TRUE)
    set(MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/resources/Info.plist)
    set(APP_TYPE MACOSX_BUNDLE)
else()
    set(APP_TYPE "")
endif()

# Find Qt6
find_package(Qt6 REQUIRED COMPONENTS Widgets Charts)

# Enable automoc for Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Add resource files
set(RESOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/icons/resources.qrc
)

# Add source files
set(SOURCES
    app/src/main.cpp
    lib/src/finance_processor.cpp
    lib/src/keyword_loader.cpp
    lib/src/data_loader.cpp
    lib/src/transaction_categorisation.cpp
    lib/src/report_generator.cpp
    lib/src/data_exporter.cpp
    lib/src/csv_parser.cpp
    lib/src/transaction_parser.cpp
    app/src/main_window.cpp
    app/src/app_config.cpp
    app/src/plot_window.cpp
    app/src/table_window.cpp
    app/src/chart_manager.cpp
    ${RESOURCES}
)

# Add header files
set(HEADERS
    lib/inc/finance_processor.hpp
    lib/inc/finance_types.hpp
    lib/inc/keyword_loader.hpp
    lib/inc/data_loader.hpp
    lib/inc/transaction_categorisation.hpp
    lib/inc/report_generator.hpp
    lib/inc/data_exporter.hpp
    lib/inc/csv_parser.hpp
    app/inc/app_config.hpp
    app/inc/main_window.hpp
    app/inc/plot_window.hpp
    app/inc/table_window.hpp
    app/inc/chart_manager.hpp
)

# Create executable
add_executable(FinanceManager ${APP_TYPE} ${SOURCES} ${HEADERS})

# Add include directories
target_include_directories(FinanceManager PRIVATE
    lib/inc
    app/inc
)

# Link Qt libraries
target_link_libraries(FinanceManager PRIVATE
    Qt6::Widgets
    Qt6::Charts
)

# Set warning flags
if(MSVC)
    target_compile_options(FinanceManager PRIVATE /W4)
else()
    target_compile_options(FinanceManager PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Install targets
install(TARGETS FinanceManager
    RUNTIME DESTINATION bin
    BUNDLE DESTINATION .
) 
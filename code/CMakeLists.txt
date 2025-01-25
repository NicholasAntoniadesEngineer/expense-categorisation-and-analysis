cmake_minimum_required(VERSION 3.16)
project(finance_categorisation)

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
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Enable automoc for Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Add resource files
set(RESOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/resources.qrc
)

# Add source files
set(SOURCES
    app/src/main.cpp
    lib/finance/src/finance_processor.cpp
    lib/finance/src/keyword_loader.cpp
    lib/finance/src/data_loader.cpp
    lib/finance/src/transaction_categorisation.cpp
    lib/finance/src/report_generator.cpp
    lib/finance/src/data_exporter.cpp
    lib/finance/src/csv_parser.cpp
    lib/finance/src/transaction_parser.cpp
    lib/gui/src/main_window.cpp
    ${RESOURCES}
)

# Add header files
set(HEADERS
    lib/finance/inc/finance_processor.hpp
    lib/finance/inc/finance_types.hpp
    lib/finance/inc/keyword_loader.hpp
    lib/finance/inc/data_loader.hpp
    lib/finance/inc/transaction_categorisation.hpp
    lib/finance/inc/report_generator.hpp
    lib/finance/inc/data_exporter.hpp
    lib/finance/inc/csv_parser.hpp
    lib/gui/inc/main_window.hpp
)

# Create executable
add_executable(finance_categorisation ${APP_TYPE} ${SOURCES} ${HEADERS})

# Add include directories
target_include_directories(finance_categorisation PRIVATE
    lib/finance/inc
    lib/gui/inc
)

# Link Qt libraries
target_link_libraries(finance_categorisation PRIVATE
    Qt6::Widgets
)

# Set warning flags
if(MSVC)
    target_compile_options(finance_categorisation PRIVATE /W4)
else()
    target_compile_options(finance_categorisation PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Install targets
install(TARGETS finance_categorisation
    RUNTIME DESTINATION bin
    BUNDLE DESTINATION .
) 
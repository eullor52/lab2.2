cmake_minimum_required(VERSION 3.16)
project(ui VERSION 0.1 LANGUAGES CXX)

# Автоматическая обработка Qt-файлов
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Поиск Qt (сначала Qt6, если нет — Qt5)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# Исходные файлы
set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
)

# Создание исполняемого файла (WIN32 убирает консольное окно)
add_executable(ui WIN32 ${PROJECT_SOURCES})

# Линковка с Qt Widgets
target_link_libraries(ui PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
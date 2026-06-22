cmake_minimum_required(VERSION 3.16)
project(lab2_ui)

# Устанавливаем стандарт C++ (обязательно для Qt)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Включаем автоматическую обработку MOC для классов с Q_OBJECT
set(CMAKE_AUTOMOC ON)
# Для корректной работы AUTOMOC добавляем текущую директорию в include-путь
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Пытаемся найти Qt6, если не получается – ищем Qt5
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# Создаём исполняемый файл
add_executable(lab2_ui main.cpp)

# Подключаем библиотеки QtWidgets
target_link_libraries(lab2_ui Qt${QT_VERSION_MAJOR}::Widgets)

# Опционально: указываем директорию с заголовками (если они не в папке проекта)
target_include_directories(lab2_ui PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
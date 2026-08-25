cmake_minimum_required(VERSION 3.16)
project(lab2)

# Общие настройки C++
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Включаем тестирование (нужно для ctest и gtest_discover_tests)
enable_testing()

# Библиотека с заголовками, которую используют и UI, и тесты
add_library(project_core INTERFACE)
target_include_directories(project_core INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/headers)

# Добавляем подпроекты
add_subdirectory(ui)      # Исполняемый файл UI (Windows GUI)
add_subdirectory(tests)   # Модульные тесты
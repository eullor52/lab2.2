find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_executable(lab_ui
    main.cpp
    mainwindow.h
    mainwindow.cpp
    resources.hpp
)

target_link_libraries(lab_ui PRIVATE Qt6::Widgets)
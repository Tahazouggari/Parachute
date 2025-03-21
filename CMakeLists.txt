cmake_minimum_required(VERSION 3.10)
project(ParachuteEncoder)

set(CMAKE_CXX_STANDARD 17)

# Find Qt5
find_package(Qt5 COMPONENTS Widgets REQUIRED)

# Enable automatic MOC for Qt's Meta-Object System
set(CMAKE_AUTOMOC ON)

# Add include directories
include_directories(${CMAKE_SOURCE_DIR}/src)

# Add executable
add_executable(ParachuteEncoder 
    main.cpp
    src/model/MessageEncoder.cpp
    src/model/ParachuteModel.cpp
    src/view/MainWindow.cpp
    src/view/ParachuteView.cpp
    src/presenter/ParachutePresenter.cpp
    src/view/BinaryWidget.cpp
)

# Link Qt5 Libraries
target_link_libraries(ParachuteEncoder Qt5::Widgets)

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
    src/utils/LanguageManager.cpp # Add LanguageManager.cpp here
)

# Add translations
file(GLOB TRANSLATION_FILES resources/translations/*.qm)
add_custom_target(translations ALL DEPENDS ${TRANSLATION_FILES})

# Add resources
qt5_add_resources(RESOURCES resources/resources.qrc)
target_sources(ParachuteEncoder PRIVATE ${RESOURCES})

# Link Qt5 Libraries
target_link_libraries(ParachuteEncoder Qt5::Widgets)

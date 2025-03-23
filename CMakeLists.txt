cmake_minimum_required(VERSION 3.10)
project(ParachuteEncoder)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)   
set(CMAKE_AUTORCC ON)   

set(CMAKE_PREFIX_PATH "/opt/homebrew/Cellar/qt@5/5.15.16_1")

find_package(Qt5 COMPONENTS Widgets REQUIRED)

include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src 
    ${Qt5Widgets_INCLUDE_DIRS}
)

file(GLOB_RECURSE HEADER_FILES 
    ${CMAKE_SOURCE_DIR}/include/model/*.h
    ${CMAKE_SOURCE_DIR}/include/view/*.h
    ${CMAKE_SOURCE_DIR}/include/presenter/*.h
    ${CMAKE_SOURCE_DIR}/include/utils/*.h
)

add_executable(ParachuteEncoder 
    main.cpp
    src/model/MessageEncoder.cpp
    src/model/ParachuteModel.cpp
    src/view/MainWindow.cpp
    src/view/ParachuteView.cpp
    src/presenter/ParachutePresenter.cpp
    src/view/BinaryWidget.cpp
    src/utils/LanguageManager.cpp
    src/view/HexView.cpp
    ${HEADER_FILES} 
)

file(GLOB TRANSLATION_FILES resources/translations/*.qm)
add_custom_target(translations ALL DEPENDS ${TRANSLATION_FILES})

qt5_add_resources(RESOURCES resources/resources.qrc)
target_sources(ParachuteEncoder PRIVATE ${RESOURCES})

target_link_libraries(ParachuteEncoder Qt5::Widgets)

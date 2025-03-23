cmake_minimum_required(VERSION 3.10)
project(ParachuteEncoder)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)   # ← Important pour les fichiers .ui (si tu en as)
set(CMAKE_AUTORCC ON)   # ← Important si tu utilises des ressources Qt (.qrc)

# Indiquer explicitement le dossier d'installation de Qt5
set(CMAKE_PREFIX_PATH "/opt/homebrew/Cellar/qt@5/5.15.16_1")

# Chercher Qt5 Widgets
find_package(Qt5 COMPONENTS Widgets REQUIRED)

# Ajouter explicitement les chemins d'entête de Qt5 et le nouveau dossier include
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src 
    ${Qt5Widgets_INCLUDE_DIRS}
)

# Collecter les fichiers d'en-tête pour les inclure explicitement
file(GLOB_RECURSE HEADER_FILES 
    ${CMAKE_SOURCE_DIR}/include/model/*.h
    ${CMAKE_SOURCE_DIR}/include/view/*.h
    ${CMAKE_SOURCE_DIR}/include/presenter/*.h
    ${CMAKE_SOURCE_DIR}/include/utils/*.h
)

# Déclarer explicitement tes sources
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
    ${HEADER_FILES}  # Inclure explicitement les fichiers d'en-tête
)

# Add translations
file(GLOB TRANSLATION_FILES resources/translations/*.qm)
add_custom_target(translations ALL DEPENDS ${TRANSLATION_FILES})

# Add resources
qt5_add_resources(RESOURCES resources/resources.qrc)
target_sources(ParachuteEncoder PRIVATE ${RESOURCES})

# Lier explicitement ton exécutable avec Qt5::Widgets
target_link_libraries(ParachuteEncoder Qt5::Widgets)

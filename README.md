#  Qt-ParachuteEncoder

##  Project Overview
ParachuteEncoder is a **Qt-based application** designed for encoding messages and simulating a parachute model. The project follows the **Model-View-Presenter (MVP)** architectural pattern for better separation of concerns.

---

# Professor

- **karim-eric ziad**: karim-eric.ziad-forest@ensicaen.fr

  
# Student 

- **ZOUGGARI Taha**: taha.zouggari@ensicaen.fr
- **KHALI Omar**: omar.khali@ensicaen.fr

## Project Structure
The project is organized following the MVP architecture.
```
PARACHUTE/
├── vscode/ 
├── resources/ 
├── src/
│ ├── model/ 
│ │     ├── MessageEncoder.cpp
│ │     ├── MessageEncoder.h
│ │     ├── ParachuteModel.cpp
│ │     └── ParachuteModel.h
│ ├── presenter/
│ │     ├── ParachutePresenter.cpp
│ │     └── ParachutePresenter.h
│ └── view/
│       ├── MainWindow.cpp
│       ├── MainWindow.h
│       ├── ParachuteView.cpp
│       └── ParachuteView.h
├── CMakeLists.txt 
├── main.cpp 
└── README.md 
```

## generate translation files :



```
lrelease resources/translations/en.ts resources/translations/fr.ts resources/translations/ar.ts
lupdate . -ts resources/translations/en.ts resources/translations/fr.ts resources/translations/ar.ts
```
open ui and modifie translation 
```
linguist resources/translations/en.ts
linguist resources/translations/fr.ts
linguist resources/translations/ar.ts
```



## To run project 

```
uic src/view/MainWindow.ui -o src/view/ui_MainWindow.h // for the ui  

rm -rf build
mkdir build && cd build
cmake ..
make
./ParachuteEncoder
```

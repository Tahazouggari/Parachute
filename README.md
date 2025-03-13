#  ParachuteEncoder

##  Project Overview
ParachuteEncoder is a **Qt-based application** designed for encoding messages and simulating a parachute model. The project follows the **Model-View-Presenter (MVP)** architectural pattern for better separation of concerns.

---

# Professor

- **karim-eric ziad**: karim-eric.ziad-forest@ensicaen.fr

## Project Structure
The project is organized into multiple directories to maintain a **clean and modular structure**.
```
PARACHUTE/
├── vscode/ 
├── resources/ 
├── src/
│ ├── model/ 
│ │ ├── MessageEncoder.cpp
│ │ ├── MessageEncoder.h
│ │ ├── ParachuteModel.cpp
│ │ └── ParachuteModel.h
│ ├── presenter/
│ │ ├── ParachutePresenter.cpp
│ │ └── ParachutePresenter.h
│ └── view/
│ ├── MainWindow.cpp
│ ├── MainWindow.h
│ ├── ParachuteView.cpp
│ └── ParachuteView.h
├── CMakeLists.txt 
├── main.cpp 
└── README.md 
```

#  ParachuteEncoder

##  Project Overview
ParachuteEncoder is a **Qt-based application** designed for encoding messages and simulating a parachute model. The project follows the **Model-View-Presenter (MVP)** architectural pattern for better separation of concerns.

---

# Professor

- **Anthony Roger**: karim-eric.ziad-forest@ensicaen.fr

## Project Structure
The project is organized into multiple directories to maintain a **clean and modular structure**.

PARACHUTE/
├── vscode/ 
├── resources/ 
├── src/
│ ├── model/ # Model components
│ │ ├── MessageEncoder.cpp
│ │ ├── MessageEncoder.h
│ │ ├── ParachuteModel.cpp
│ │ └── ParachuteModel.h
│ ├── presenter/ # Presenter components
│ │ ├── ParachutePresenter.cpp
│ │ └── ParachutePresenter.h
│ └── view/ # View components
│ ├── MainWindow.cpp
│ ├── MainWindow.h
│ ├── ParachuteView.cpp
│ └── ParachuteView.h
├── CMakeLists.txt # CMake build configuration
├── main.cpp # Main application entry point
└── README.md # Project documentation


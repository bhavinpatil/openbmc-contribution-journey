# Makefile-Based C++ Project: Greeter

This project demonstrates how to structure and build a simple modular C++ application using a custom Makefile — similar to real-world embedded or firmware projects like OpenBMC.

## 🧠 What It Does

A basic C++ class `Greeter` is created that prints a personalized greeting. The project is organized with proper headers, source files, and uses a Makefile to compile and link the components.

## 📁 Project Structure

```
makefile-project/
├── src/ # C++ source files
│ ├── main.cpp
│ └── greeter.cpp
├── include/ # Header files
│ └── greeter.hpp
├── obj/ # (auto-created) Object files
├── bin/ # (auto-created) Final executable
├── Makefile
└── README.md
```


## 🛠 Build & Run Instructions

```bash
# Build the project
make

# Run the executable
make run

# Clean build files
make clean
```
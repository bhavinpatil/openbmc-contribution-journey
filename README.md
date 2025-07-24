# 🔧 OpenBMC Contribution Journey — A Learning Path for New Contributors

This repository serves as a practical and structured learning path for developers who want to start contributing to **C++-driven open-source firmware and embedded Linux projects** like [OpenBMC](https://github.com/openbmc).

It is especially useful for those without access to real hardware — just a Linux machine or VM is enough to follow along.

---

## 🎯 Why This Repository?

Projects like OpenBMC are large and complex, often built on top of:
- C++
- Linux system APIs
- D-Bus (inter-process communication)
- Build systems like Make, CMake, and Yocto

This repository offers a step-by-step approach to build the foundational skills required to explore, understand, and eventually contribute to such ecosystems.

---

## 📚 What You'll Learn

This learning path is divided into focused stages, each with example code, explanations, and hands-on exercises:

### 🧱 Stage 1 — Core Skills
- Basics of `Makefile` and `CMake` build systems
- Writing Linux system programs using `fork`, `exec`, file I/O, and signals
- Shell scripting to automate common developer/system tasks
- Intro to Python scripting for testing and tooling
- D-Bus fundamentals: services, signals, method calls

### 🔍 Stage 2 — Exploring OpenBMC-Like Architecture
- Setting up and running OpenBMC on QEMU (virtual machine)
- Understanding OpenBMC services and D-Bus interfaces
- Exploring C++ services like `phosphor-logging`, `phosphor-time-manager`
- Inspecting D-Bus communication with `busctl` and writing mock services

### 🧩 Stage 3 — Making Real-World Contributions
- Finding beginner-friendly issues (`good first issue`) in OpenBMC or related repos
- Submitting documentation or test improvements
- Writing patches for small bugs, cleanup, or logging enhancements
- Contributing tools (log parsers, service watchers, test scripts)

---

## 🗂️ Repository Structure
```
openbmc-contribution-journey/
├── makefile-project/ # C++ project using Makefile
├── cmake-project/ # C++ project built with CMake
├── system-programming/ # Fork, exec, signals, file I/O examples
├── dbus/ # Basic D-Bus service + client
├── shell-scripts/ # Bash scripts for system automation
├── python-tools/ # Python CLI tools and utilities
├── README.md # This file
```


---

## 🤝 Contributing

If you're learning alongside this or want to improve the examples, feel free to:
- Open issues or pull requests
- Suggest improvements or add your own learning examples

This repo is intentionally minimal and beginner-friendly — anyone is welcome to join the journey.

---

## 📫 Contact

Maintained by [@bhavinpatil](https://github.com/bhavinpatil)  
Pull requests and discussions are always welcome.

---

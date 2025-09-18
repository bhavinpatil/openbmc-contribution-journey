# OpenBMC Contribution Journey

This repository is a structured collection of examples and exercises designed to help developers build the skills needed to explore and contribute to OpenBMC and other C++-driven embedded Linux projects.

It is particularly suited for learners without dedicated hardware — a Linux machine or virtual environment is sufficient.

---

## What You’ll Learn

This repository focuses on practical, hands-on learning with examples covering:

- C++ fundamentals and system-level programming
- Build systems: Makefile and CMake
- Linux programming concepts: fork, exec, signals, file I/O
- Shell scripting for automation and system utilities
- Python scripting for tooling and testing
- D-Bus basics: services, properties, signals, and method calls
- Exploring and simulating OpenBMC-like services in a QEMU environment

This repository offers a step-by-step approach to build the foundational skills required to explore, understand, and eventually contribute to such ecosystems.

---

## Repository Structure
```
openbmc-contribution-journey/
├── cmake-projects/              # C++ projects using CMake
├── cplusplus/                   # C++ examples (e.g., bit manipulation)
├── dbus/                        # Basic and extended D-Bus services & clients
├── docs/                        # Documentation (e.g., QEMU usage guides)
├── makefile-projects/           # C++ projects using Makefiles
├── shell-scripts/               # Bash scripts for automation
├── system-programming/          # Fork, exec, signals, file I/O examples
├── xyz.openbmc_project.Bhavin.* # Custom D-Bus services (demo, virtual switch)
├── README.md                    # This file

```


---

## Contribution

The repository is designed to be minimal, practical, and beginner-friendly. Contributions are welcome in the form of:
- Additional examples or exercises
- Improvements to existing code or documentation
- Issues and suggestions for clarity and learning flow

This repo is intentionally minimal and beginner-friendly — anyone is welcome to join the journey.

---

## Maintainer

Maintained by @bhavinpatil

Feedback and pull requests are encouraged.

---

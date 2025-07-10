## 🔥 STAGE 1: 2-Week Learning Plan

Focus: **Makefiles, CMake, Linux system programming, D-Bus, and scripting**

### 📅 Week 1: *Build System + System Programming*

---

### ✅ **DAY 1–2: Makefiles**

* **Goal**: Understand how projects are compiled using `make`
* **Topics**:

  * Variables
  * Targets & dependencies
  * `make clean`, `make all`
  * Include paths and linking flags

**Resources**:

* [https://makefiletutorial.com/](https://makefiletutorial.com/)
* [This GitHub project to practice](https://github.com/mbcrawfo/simple-makefile-example)

🛠 **Task**:

* Write a `Makefile` for a small C++ project (2-3 files using a class).
* Add `make run`, `make clean` targets.

---

### ✅ **DAY 3–4: CMake**

* **Goal**: Learn modern cross-platform builds with CMake
* **Topics**:

  * `CMakeLists.txt` structure
  * `target_link_libraries`, `add_executable`, `include_directories`

**Resources**:

* [Official CMake Tutorial](https://cmake.org/cmake/help/latest/guide/tutorial/index.html)
* Practice repo: [CMake Examples](https://github.com/ttroy50/cmake-examples)

🛠 **Task**:

* Convert the same Makefile-based C++ project into a CMake-based build.
* Add one external dependency (e.g., fmtlib or nlohmann/json via `FetchContent`)

---

### ✅ **DAY 5–7: Linux System Programming (Basics)**

* **Goal**: Learn to interact with OS-level APIs
* **Topics**:

  * File I/O (`open`, `read`, `write`, `close`)
  * Process management (`fork`, `exec`, `wait`)
  * Signals (`signal`, `kill`)

**Resources**:

* Book (Free PDF): [Linux System Programming (Love)](https://github.com/s-matyukevich/Linux-System-Programming)
* [This repo with simple system programming examples](https://github.com/brenns10/lss)

🛠 **Task**:

* Write a small program that forks a child process.
* In child, run a command using `execvp` (e.g., `ls`).
* In parent, wait and print child's exit status.

---

### 📅 Week 2: *D-Bus, Shell/Python Scripting*

---

### ✅ **DAY 8–10: D-Bus Fundamentals**

* **Goal**: Understand the basics of inter-process communication on Linux
* **Topics**:

  * What is D-Bus (system vs session)
  * Using `busctl` to inspect services
  * Writing a basic D-Bus service/client

**Resources**:

* [Beginner-friendly D-Bus tutorial in Python](https://dbus.freedesktop.org/doc/dbus-python/tutorial.html)
* CLI tool: `busctl --user list`

🛠 **Task**:

* Write a D-Bus server in Python or C++ that exposes a simple method (e.g., "SayHello").
* Write a client to call it.

---

### ✅ **DAY 11–12: Shell Scripting Practice**

* **Goal**: Be able to write quick tools to automate tasks
* **Topics**:

  * Variables, conditionals, loops
  * `awk`, `grep`, `cut`, `xargs`
  * Script arguments

**Resources**:

* [Shell scripting crash course](https://devhints.io/bash)
* Practice: [Shell scripting examples](https://github.com/dylanaraps/pure-bash-bible)

🛠 **Task**:

* Write a script to:

  * List files > 100MB in a directory
  * Monitor a log file for the word “ERROR” and alert you

---

### ✅ **DAY 13–14: Python Practice (for testing/tools)**

* **Goal**: Brush up Python basics so you can write test scripts and tools
* **Topics**:

  * File I/O, functions, classes
  * CLI tools using `argparse`
  * JSON/YAML handling

🛠 **Task**:

* Write a Python script that reads a JSON config and prints system info (like `os`, `platform`, etc).
* Write another script to ping a list of IPs from a text file.

---

## 📦 Final Outcome of Stage 1

By the end of 2 weeks, you’ll have:

* Created and built a small C++ project with both Make and CMake
* Written basic system programs using Linux syscalls
* Created a working D-Bus service and client
* Built basic Python + Shell scripts
* Gained confidence with tools used by OpenBMC contributors

---

## 📘 Optional Template Repo for Practice

I'll prepare a **template GitHub repo** for this 2-week challenge if you want to track it publicly and use it as a portfolio. Let me know!

---

## 👉 What Next?

You can start **Day 1 today**. Just reply here with any help you need while doing the task or understanding the material.

Also let me know:

* Do you want a public GitHub repo for your learning projects?
* Do you want me to track your progress as you go and give feedback?

I’ll be here throughout the journey.

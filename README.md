# Simple Scheduler
## A Process Scheduler in C from Scratch

[![Language: C](https://img.shields.io/badge/language-C-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))

---

## Table of Contents

1. [Overview](#overview)
2. [Features](#features)
3. [Implementation Details](#implementation-details)

---

## Overview

Simple Scheduler is a lightweight process scheduler implemented in C that manages user-submitted commands using a **priority-based round-robin** algorithm. The project features a custom shell interface for command input and a scheduler that efficiently handles process queues, inter-process communication via pipes, and robust signal handling. This project is ideal for understanding process management and system-level programming in C.

---

## Features

- **Priority-Based Round-Robin Scheduling:**  
  Allocates CPU time fairly by scheduling processes based on their assigned priorities.
- **Custom Shell Interface:**  
  Captures and processes user commands seamlessly.
- **Inter-Process Communication:**  
  Utilizes pipes (FIFO) for effective communication between the shell and scheduler.
- **Signal Handling:**  
  Implements handlers for SIGINT, SIGCHLD, and SIGTERM to manage process lifecycle and termination.
- **Queue Management:**  
  Maintains separate queues for ready, running, and completed processes.
- **Command History & Logging:**  
  Records command execution history, process IDs, and execution times for performance monitoring.

---

## Implementation Details

For an in-depth explanation of the project architecture, design choices, and code implementation, please refer to the [Implementation](implementation.md) document.

---

Happy scheduling and enjoy exploring process management in C!

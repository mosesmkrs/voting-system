# SONU Electronic Voting System

An industrial-grade, terminal-based electronic voting application developed in **C**. This project was designed to facilitate elections for the **Students Organization of Nairobi University (SONU)**, featuring multi-race support, concurrency protection, and automated tallying.

## 🚀 Features

- **Dynamic Race Management**: Administrators can define multiple positions (e.g., Chairman, Secretary General)
- **Voter & Candidate Registration**: Secure registration for students and aspirants with unique ID generation
- **Relational Ballot System**: Voters can cast one ballot per race/position, preventing double-voting in a single category
- **Thread-Safe Concurrency**: Uses `pthread` mutex locks to prevent race conditions during simultaneous voting
- **Automated Tallying**: Implements a Bubble Sort algorithm to rank candidates and automatically identify winners or ties
- **Data Persistence**: All records are stored in binary `.dat` files for efficiency and durability

---

## 🏗️ Architectural Design

The project follows a modular C structure to separate logic from data and interface:

```
VotingSystem/
├── bin/          # Compiled executable (sonu_vote.exe)
├── include/      # Header files (.h) defining data structures
├── src/          # Source code (.c) implementation modules
├── obj/          # Intermediate object files (.o)
├── data/         # Binary database files (.dat)
└── Makefile      # Automated build system
```

## 🛠️ Technical Specifications

| Aspect | Details |
|--------|---------|
| **Language** | C (GCC 14.2.0+) |
| **Concurrency** | POSIX Threads (lpthread) |
| **File I/O** | Binary stream handling (fread, fwrite, fseek) |
| **Algorithm** | Bubble Sort for vote tallying |

## 📥 Installation & Setup

1. Clone the project into your VS Code workspace
2. Ensure you have MinGW-w64 (with msys64) installed and added to your path

**Compile the project using the provided Makefile:**
```bash
mingw32-make
```

**Run the application:**
```bash
./bin/sonu_vote.exe
```

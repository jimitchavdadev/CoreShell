# MyShell - A Custom Linux Shell in C

**MyShell** is a custom command-line interpreter for Unix-like operating systems, written entirely in C. This project is an exploration of fundamental operating system concepts, including process management, inter-process communication (IPC), signal handling, and file descriptor manipulation.

## 🎯 Project Objectives

The primary goal of this project is to build a shell that replicates core functionalities of standard shells like `bash` or `zsh`. This demonstrates a deep understanding of how an operating system manages and executes user commands. Key objectives include:

  * **Process Management:** Mastering the `fork`, `exec`, and `waitpid` system calls to create and manage child processes.
  * **Inter-Process Communication:** Implementing piping (`|`) to channel the output of one process into the input of another.
  * **I/O Redirection:** Handling input (`<`) and output (`>`) redirection to and from files.
  * **Job Control:** Implementing features to manage background (`&`) and foreground processes, including stopping (`Ctrl+Z`), resuming (`fg`, `bg`), and terminating (`kill`) jobs.
  * **Custom Built-in Commands:** Creating commands that are part of the shell itself, such as `my_cd` and `my_exit`.

## ✨ Features

  * **Command Execution:** Executes any command found in the system's `$PATH`.
  * **Piping (`|`):** Supports single pipes to connect two commands.
  * **Redirection (`<`, `>`):** Supports redirecting standard input and standard output.
  * **Job Control:**
      * Run processes in the background using `&`.
      * List active jobs with the `jobs` command.
      * Bring a job to the foreground with `fg`.
      * Resume a stopped job in the background with `bg`.
      * Terminate a job with `kill`.
  * **Built-in Commands:** Includes `my_cd`, `my_exit`, `jobs`, `fg`, `bg`, `kill`, `my_echo`, `history`, `sysinfo`, and `countdown`.
  * **Command History:** Remembers previous commands, which can be viewed with the `history` command.

## 📂 Project Structure

The project is organized into modular source files for clarity and maintainability.

```
.
├── Makefile          # Automates the compilation process.
├── my_shell          # The final compiled executable.
├── src/
│   ├── shell.h       # Header file with shared structures and prototypes.
│   ├── shell.c       # The main entry point and command loop.
│   ├── parser.c      # Logic for parsing the user's command line input.
│   ├── executor.c    # Handles command execution, forking, and redirection.
│   ├── builtins.c    # Implementation of all built-in commands.
│   ├── job_control.c # Functions for managing the background job list.
│   ├── history.c     # Manages the command history list.
└── test_my_shell.sh  # An automated script to test all shell features.
```

## ⚙️ How It Works

The shell operates on a simple but powerful **Read-Parse-Execute** loop.

1.  **Read:** The shell displays a prompt and reads a full line of input from the user using `fgets()`.
2.  **Parse:** The input string is parsed using `strtok()`. The parser identifies the command, its arguments, and any special operators like `|`, `<`, `>`, or `&`.
3.  **Execute:** This is the most critical step:
      * **Built-in Check:** The shell first checks if the command is a **parent-managed built-in** (`my_cd`, `my_exit`, `fg`, `bg`, `kill`, `jobs`). These commands must modify the shell's own state (e.g., its current directory or job list) and are executed directly.
      * **Forking:** If the command is not a parent built-in, the shell calls `fork()` to create a new child process.
      * **Child Process:** The child process handles I/O redirection using `dup2()` if necessary. It then checks if the command is a **child-runnable built-in** (`my_echo`, `history`, etc.). If so, it runs the C function and exits. Otherwise, it uses `execvp()` to replace its own code with the code of the external command (e.g., `ls`).
      * **Parent Process:** If the command was run in the foreground, the parent waits for the child to complete using `waitpid()`. If it was a background command (`&`), the parent adds the child to its job list and immediately loops back to the prompt.

## 🛠️ How to Compile and Run

1.  **Compile the Project:** Use the provided `Makefile`.

    ```bash
    make
    ```

    This will create the `my_shell` executable in the root directory.

2.  **Run the Shell:**

    ```bash
    ./my_shell
    ```

## 🧪 How to Test

An automated test script is included to verify all core functionality.

1.  **Make the script executable:**

    ```bash
    chmod +x test_my_shell.sh
    ```

2.  **Run the tests:**

    ```bash
    ./test_my_shell.sh
    ```

    The script will run a series of automated checks and then provide instructions for manually testing interactive features like `fg` and `Ctrl+Z`.
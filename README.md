# Minishell – A Simple Shell Implementation (42 Project)

Minishell is a minimal Unix shell built as part of the 42 curriculum.  
It reproduces the core behavior of **bash**, implementing command parsing, pipes, redirections, environment variable expansion, process execution, and builtins — all developed in C without external libraries other than `readline`.

---

## 🚀 Features

### ✔ Command execution
- Execute binary programs using PATH resolution  
- Support for absolute and relative paths  
- Error handling identical to bash (`command not found`, permissions, etc.)

### ✔ Builtins implemented
- `echo` (with `-n`)
- `cd`
- `pwd`
- `export`
- `unset`
- `env`
- `exit`

### ✔ Environment variable management
- Internal environment list replicated from `envp`
- `$VARIABLE` expansion
- `$?` support (last exit status)

### ✔ Pipes and Redirections
- `|`
- `<`
- `>`
- `>>`
- Combination of multiple pipes and nested redirections
- File descriptor duplication and correct closing strategy

### ✔ Quoting rules
- Single quotes `' '` preserve literal content  
- Double quotes `" "` allow expansion  
- Escape character handling (if implemented)

### ✔ Signals
- `Ctrl-C` → interrupts current input, redisplays prompt  
- `Ctrl-\` → ignored (like bash in interactive mode)  
- Proper signal management inside child processes  

### ✔ Memory Management
- No leaks (validated with `valgrind`)  
- Full cleanup between commands  
- Tokenized structures freed on each iteration  

---

## 🧠 Architecture Overview

### **1. Lexer**
Converts the raw user input into tokens:
- Words  
- Pipes  
- Redirection operators  
- Quoted blocks  

Handles escape rules and space trimming.

### **2. Parser**
Builds a command table:
- Command name  
- Arguments  
- Input/output redirections  
- Pipe sequence structure  

Detects syntax errors such as:
- `|` at start/end
- `||`
- `>>>`
- Missing filenames after redirections

### **3. Executor**
Executes the parsed command structure:
- Handles builtins in parent process (when required)
- Fork/execve for external commands
- Pipe creation and wiring
- Redirection using `dup2`
- Proper closing of fds to avoid leaks

### **4. Environment Handling**
Maintains an internal copy of `envp`:
- Linked list or array  
- Synced with export/unset  
- Passed to execve as char**  

### **5. Builtins**
Each builtin has its own module and follows bash behavior and return codes.

---

## ⛓ Dependencies
Your minishell uses:

- `readline()`, `add_history()`
- Standard C library
- POSIX functions: `fork`, `execve`, `pipe`, `dup2`, `waitpid`, `signal`, etc.

Install readline on Linux:

```bash
sudo apt install libreadline-dev

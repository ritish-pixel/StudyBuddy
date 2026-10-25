
# StudyTrack — Student Task Planner

StudyTrack is a beginner-friendly, menu-driven task management application developed in C. It helps students organize academic tasks, track completion progress, and save their tasks for future use.

## Features

- **Add Tasks:** Create tasks with a title, subject, and priority.
- **View Tasks:** Display all saved tasks with their details and status.
- **Unique Task IDs:** Assign a unique ID to each new task.
- **Mark as Completed:** Update the status of a task when it is finished.
- **Search Tasks:** Find a task using its exact title.
- **Progress Summary:** View total, completed, and pending tasks along with the completion percentage.
- **Delete Tasks:** Remove tasks that are no longer needed.
- **File Handling:** Save tasks to `tasks.txt` and load them when the program starts.
- **Input Validation:** Handle invalid menu choices, numeric inputs, and task priorities.

## Technologies Used

- C Programming Language
- Standard C Library
- File Handling
- Visual Studio Code

## Project Structure

```text
StudyTrack/
├── main.c
├── tasks.txt
└── README.md
```

`tasks.txt` is created by the program when tasks are saved. It stores task information so it can be loaded during a future run.

## How to Run

### 1. Install a C compiler

Install GCC or another compatible C compiler and make sure it is available in your terminal.

### 2. Open the project folder

Open the StudyTrack folder in Visual Studio Code.

### 3. Compile the program

Using GCC, run:

```bash
gcc main.c -o StudyTrack
```

### 4. Run the program

On Windows:

```powershell
.\StudyTrack.exe
```

On Linux:

```bash
./StudyTrack
```

## Concepts Practiced

This project helped me practice:

- Structures (`struct`)
- Arrays and strings
- Functions and pointers
- Loops and conditional statements
- String comparison using `strcmp()`
- Input validation
- File operations using `fopen()`, `fgets()`, `fprintf()`, and `fclose()`
- Basic data validation and task management logic

## Future Improvements

- Add task due dates and deadlines.
- Sort tasks by priority.
- Search tasks by subject.
- Add overdue-task reminders.
- Improve the interface and task filtering options.

## Learning Outcome

StudyTrack is a practical project for applying fundamental C programming concepts to a useful student-focused application. It provides experience in program structure, input handling, data management, and persistent file storage.

---

**Project:** StudyTrack — Student Task Planner  
**Language:** C  
**Level:** Beginner

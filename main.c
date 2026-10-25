
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define MAX_TASKS 100

int nextTaskId = 1;

// Structure to store task details
struct Task
{
    int id;
    char title[100];
    char subject[50];
    int priority;
    int completed;
};

// Read a line of text safely
int readLine(const char *prompt, char text[], size_t size)
{
    printf("%s", prompt);

    if (fgets(text, size, stdin) == NULL)
    {
        return 0;
    }

    size_t length = strlen(text);

    if (length > 0 && text[length - 1] == '\n')
    {
        text[length - 1] = '\0';
    }
    else
    {
        int ch;

        while ((ch = getchar()) != '\n' && ch != EOF)
        {
            // Clear extra characters
        }
    }

    // Reject empty input
    if (text[0] == '\0')
    {
        printf("Input cannot be empty.\n");
        return 0;
    }

    return 1;
}

// Read an integer safely
int readInteger(const char *prompt, int *number)
{
    char input[100];
    char *end;
    long value;

    while (1)
    {
        if (!readLine(prompt, input, sizeof(input)))
        {
            if (feof(stdin))
            {
                return 0;
            }

            continue;
        }

        errno = 0;
        value = strtol(input, &end, 10);

        while (*end == ' ' || *end == '\t')
        {
            end++;
        }

        if (end == input || *end != '\0' ||
            errno == ERANGE ||
            value < -2147483647L - 1 ||
            value > 2147483647L)
        {
            printf("Invalid input! Please enter a whole number.\n");
            continue;
        }

        *number = (int)value;
        return 1;
    }
}

// Function to add a new task
void addTask(struct Task tasks[], int *taskCount)
{
    if (*taskCount >= MAX_TASKS)
    {
        printf("\nTask limit reached!\n");
        return;
    }

    struct Task newTask;

    if (!readLine("Enter task title: ",
                  newTask.title, sizeof(newTask.title)))
    {
        return;
    }

    if (!readLine("Enter subject: ",
                  newTask.subject, sizeof(newTask.subject)))
    {
        return;
    }

    while (1)
    {
        if (!readInteger(
                "Select priority (1=Low, 2=Medium, 3=High): ",
                &newTask.priority))
        {
            return;
        }

        if (newTask.priority >= 1 && newTask.priority <= 3)
        {
            break;
        }

        printf("Priority must be 1, 2, or 3.\n");
    }

    newTask.id = nextTaskId++;
    newTask.completed = 0;

    tasks[*taskCount] = newTask;
    (*taskCount)++;

    printf("\nTask added successfully! Task ID: %d\n",
           newTask.id);
}

// Function to display one task
void displayTask(struct Task task)
{
    printf("\nTask ID: %d\n", task.id);
    printf("Title: %s\n", task.title);
    printf("Subject: %s\n", task.subject);

    printf("Priority: ");

    switch (task.priority)
    {
        case 1:
            printf("Low\n");
            break;

        case 2:
            printf("Medium\n");
            break;

        case 3:
            printf("High\n");
            break;
    }

    printf("Status: %s\n",
           task.completed ? "Completed" : "Pending");
}

// Function to display all tasks
void viewTasks(struct Task tasks[], int taskCount)
{
    if (taskCount == 0)
    {
        printf("\nNo tasks added yet!\n");
        return;
    }

    printf("\n========== YOUR TASKS ==========\n");

    for (int i = 0; i < taskCount; i++)
    {
        displayTask(tasks[i]);
    }

    printf("\n================================\n");
}

// Function to mark a task as completed
void completeTask(struct Task tasks[], int taskCount)
{
    if (taskCount == 0)
    {
        printf("\nNo tasks available to complete!\n");
        return;
    }

    int id;

    if (!readInteger("Enter the Task ID to complete: ", &id))
    {
        return;
    }

    for (int i = 0; i < taskCount; i++)
    {
        if (tasks[i].id == id)
        {
            if (tasks[i].completed)
            {
                printf("This task is already completed!\n");
            }
            else
            {
                tasks[i].completed = 1;
                printf("Task marked as completed!\n");
            }

            return;
        }
    }

    printf("Task ID not found!\n");
}

// Function to search for a task by exact title
void searchTask(struct Task tasks[], int taskCount)
{
    if (taskCount == 0)
    {
        printf("\nNo tasks available to search!\n");
        return;
    }

    char searchTitle[100];

    if (!readLine("Enter the exact task title to search: ",
                  searchTitle, sizeof(searchTitle)))
    {
        return;
    }

    for (int i = 0; i < taskCount; i++)
    {
        if (strcmp(tasks[i].title, searchTitle) == 0)
        {
            printf("\nTask found!\n");
            displayTask(tasks[i]);
            return;
        }
    }

    printf("\nTask not found! Check the title and try again.\n");
}

// Function to display progress
void showProgress(struct Task tasks[], int taskCount)
{
    int completed = 0;
    int pending = 0;

    for (int i = 0; i < taskCount; i++)
    {
        if (tasks[i].completed == 1)
        {
            completed++;
        }
        else
        {
            pending++;
        }
    }

    float percentage = 0.0f;

    if (taskCount > 0)
    {
        percentage = (completed * 100.0f) / taskCount;
    }

    printf("\n========== PROGRESS SUMMARY ==========\n");
    printf("Total Tasks      : %d\n", taskCount);
    printf("Completed Tasks  : %d\n", completed);
    printf("Pending Tasks    : %d\n", pending);
    printf("Completion       : %.2f%%\n", percentage);
    printf("======================================\n");
}

// Function to save tasks to a file
void saveTasks(struct Task tasks[], int taskCount)
{
    FILE *file = fopen("tasks.txt", "w");

    if (file == NULL)
    {
        printf("Error: Could not save tasks!\n");
        return;
    }

    for (int i = 0; i < taskCount; i++)
    {
        if (fprintf(file, "%d\n%s\n%s\n%d\n%d\n",
                    tasks[i].id,
                    tasks[i].title,
                    tasks[i].subject,
                    tasks[i].priority,
                    tasks[i].completed) < 0)
        {
            printf("Error while writing tasks to file!\n");
            fclose(file);
            return;
        }
    }

    if (fclose(file) != 0)
    {
        printf("Error while closing the task file!\n");
        return;
    }

    printf("Tasks saved successfully!\n");
}

// Read an integer from one line of a file
int readFileInteger(FILE *file, int *number)
{
    char line[100];
    char *end;
    long value;

    if (fgets(line, sizeof(line), file) == NULL)
    {
        return 0;
    }

    errno = 0;
    value = strtol(line, &end, 10);

    while (*end == ' ' || *end == '\t' ||
           *end == '\n' || *end == '\r')
    {
        end++;
    }

    if (end == line || *end != '\0' ||
        errno == ERANGE ||
        value < -2147483647L - 1 ||
        value > 2147483647L)
    {
        return 0;
    }

    *number = (int)value;
    return 1;
}

// Function to load tasks from a file
void loadTasks(struct Task tasks[], int *taskCount)
{
    FILE *file = fopen("tasks.txt", "r");

    if (file == NULL)
    {
        // No saved file yet
        return;
    }

    *taskCount = 0;
    nextTaskId = 1;

    while (*taskCount < MAX_TASKS)
    {
        struct Task task;
        char extra[2];
        int valid = 1;

        // Read task ID
        if (!readFileInteger(file, &task.id))
        {
            if (feof(file))
            {
                break;
            }

            printf("Warning: Invalid task ID in saved file.\n");
            break;
        }

        // Read title
        if (fgets(task.title, sizeof(task.title), file) == NULL)
        {
            valid = 0;
        }
        else if (strchr(task.title, '\n') == NULL &&
                 !feof(file))
        {
            // Title is longer than the allowed size
            valid = 0;
        }

        if (!valid)
        {
            printf("Warning: Invalid or incomplete task title.\n");
            break;
        }

        // Remove newline from title
        task.title[strcspn(task.title, "\r\n")] = '\0';

        // Read subject
        if (fgets(task.subject, sizeof(task.subject), file) == NULL)
        {
            printf("Warning: Missing task subject.\n");
            break;
        }

        if (strchr(task.subject, '\n') == NULL &&
            !feof(file))
        {
            printf("Warning: Task subject is too long.\n");
            break;
        }

        // Remove newline from subject
        task.subject[strcspn(task.subject, "\r\n")] = '\0';

        // Read priority and completion status
        if (!readFileInteger(file, &task.priority) ||
            !readFileInteger(file, &task.completed))
        {
            printf("Warning: Saved task data is incomplete.\n");
            break;
        }

        // Validate loaded values
        if (task.id <= 0 ||
            task.title[0] == '\0' ||
            task.subject[0] == '\0' ||
            task.priority < 1 ||
            task.priority > 3 ||
            (task.completed != 0 && task.completed != 1))
        {
            printf("Warning: Invalid task data found.\n");
            break;
        }

        // Check for duplicate IDs
        for (int i = 0; i < *taskCount; i++)
        {
            if (tasks[i].id == task.id)
            {
                valid = 0;
                break;
            }
        }

        if (!valid)
        {
            printf("Warning: Duplicate task ID found.\n");
            break;
        }

        // Store the validated task
        tasks[*taskCount] = task;
        (*taskCount)++;

        if (task.id >= nextTaskId)
        {
            nextTaskId = task.id + 1;
        }
    }

    // Check whether the file contains more tasks than allowed
    if (*taskCount == MAX_TASKS)
    {
        int ch;

        while ((ch = fgetc(file)) != EOF)
        {
            if (!feof(file))
            {
                printf("Warning: Maximum task limit reached while loading.\n");
                break;
            }
        }
    }

    fclose(file);

    printf("%d task(s) loaded successfully!\n", *taskCount);
}

// Function to delete a task
void deleteTask(struct Task tasks[], int *taskCount)
{
    if (*taskCount == 0)
    {
        printf("\nNo tasks available to delete!\n");
        return;
    }

    int id;

    if (!readInteger("Enter the Task ID to delete: ", &id))
    {
        return;
    }

    for (int i = 0; i < *taskCount; i++)
    {
        if (tasks[i].id == id)
        {
            // Shift remaining tasks one position left
            for (int j = i; j < *taskCount - 1; j++)
            {
                tasks[j] = tasks[j + 1];
            }

            (*taskCount)--;

            printf("Task deleted successfully!\n");
            saveTasks(tasks, *taskCount);
            return;
        }
    }

    printf("Task ID not found!\n");
}

// Main function
int main(void)
{
    struct Task tasks[MAX_TASKS];
    int taskCount = 0;
    int choice;

    loadTasks(tasks, &taskCount);

    while (1)
    {
        printf("\n================================\n");
        printf("           STUDYTRACK\n");
        printf("       Student Task Planner\n");
        printf("================================\n");

        printf("1. Add a new task\n");
        printf("2. View all tasks\n");
        printf("3. Mark task as completed\n");
        printf("4. Search for a task\n");
        printf("5. View progress summary\n");
        printf("6. Delete a task\n");
        printf("7. Exit\n");

        printf("================================\n");

        if (!readInteger("Enter your choice: ", &choice))
        {
            printf("\nInput ended. Saving tasks before exit.\n");
            saveTasks(tasks, taskCount);
            break;
        }

        switch (choice)
        {
            case 1:
            {
                int oldCount = taskCount;

                addTask(tasks, &taskCount);

                if (taskCount > oldCount)
                {
                    saveTasks(tasks, taskCount);
                }

                break;
            }

            case 2:
                viewTasks(tasks, taskCount);
                break;

            case 3:
            {
                int oldCompleted[MAX_TASKS];

                for (int i = 0; i < taskCount; i++)
                {
                    oldCompleted[i] = tasks[i].completed;
                }

                completeTask(tasks, taskCount);

                // Save only if a task's status changed
                for (int i = 0; i < taskCount; i++)
                {
                    if (oldCompleted[i] != tasks[i].completed)
                    {
                        saveTasks(tasks, taskCount);
                        break;
                    }
                }

                break;
            }

            case 4:
                searchTask(tasks, taskCount);
                break;

            case 5:
                showProgress(tasks, taskCount);
                break;

            case 6:
                deleteTask(tasks, &taskCount);
                break;

            case 7:
                saveTasks(tasks, taskCount);
                printf("Thank you for using StudyTrack!\n");
                return 0;

            default:
                printf("Invalid choice. Please select 1 to 7.\n");
        }
    }

    return 0;
}


#include <stdio.h>
#include <string.h>

#define MAX_HABITS 50
#define NAME_LENGTH 100

typedef struct {
    char name[NAME_LENGTH];
    int completed;
} Habit;

Habit habits[MAX_HABITS];
int count = 0;

void saveHabits() {
    FILE *file = fopen("habits.txt", "w");

    if (file == NULL) {
        printf("Error saving habits!\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%d|%s\n",
                habits[i].completed, habits[i].name);
    }

    fclose(file);
}

void loadHabits() {
    FILE *file = fopen("habits.txt", "r");

    if (file == NULL) {
        return;
    }

    char line[NAME_LENGTH + 10];

    while (count < MAX_HABITS &&
           fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';

        if (sscanf(line, "%d|%99[^\n]",
                   &habits[count].completed,
                   habits[count].name) == 2) {
            count++;
        }
    }

    fclose(file);
}

void addHabit() {
    if (count >= MAX_HABITS) {
        printf("Habit list is full!\n");
        return;
    }

    printf("Enter habit name: ");

    if (fgets(habits[count].name, NAME_LENGTH, stdin)
        == NULL) {
        return;
    }

    habits[count].name[
        strcspn(habits[count].name, "\n")
    ] = '\0';

    if (habits[count].name[0] == '\0' ||
        strchr(habits[count].name, '|') != NULL) {
        printf("Invalid habit name!\n");
        return;
    }

    habits[count].completed = 0;
    count++;

    saveHabits();
    printf("Habit added successfully!\n");
}

void viewHabits() {
    if (count == 0) {
        printf("No habits yet. Add one first!\n");
        return;
    }

    printf("\n------ YOUR HABITS ------\n");

    for (int i = 0; i < count; i++) {
        printf("%d. [%c] %s\n",
               i + 1,
               habits[i].completed ? 'X' : ' ',
               habits[i].name);
    }
}

void completeHabit() {
    int number;

    viewHabits();

    if (count == 0) {
        return;
    }

    printf("\nEnter habit number to complete: ");

    if (scanf("%d", &number) != 1) {
        printf("Please enter a valid number.\n");

        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) {}
        return;
    }

    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {}

    if (number < 1 || number > count) {
        printf("Invalid habit number!\n");
        return;
    }

    habits[number - 1].completed = 1;
    saveHabits();

    printf("Great work! Habit completed!\n");
}

int main() {
    int choice;

    loadHabits();

    do {
        printf("\n===== HABIT TRACKER =====\n");
        printf("1. Add a habit\n");
        printf("2. View habits\n");
        printf("3. Mark habit as completed\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Please enter a valid number.\n");

            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) {}
            continue;
        }

        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) {}

        switch (choice) {
            case 1:
                addHabit();
                break;

            case 2:
                viewHabits();
                break;

            case 3:
                completeHabit();
                break;

            case 4:
                saveHabits();
                printf("Habits saved. Goodbye!\n");
                break;

            default:
                printf("Invalid choice. Try again!\n");
        }

    } while (choice != 4);

    return 0;
}

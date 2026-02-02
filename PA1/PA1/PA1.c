#include <stdio.h>

#define MAX_SIZE 100

typedef struct {
    int data[MAX_SIZE];
    int size;
} IntList;

/* Function prototypes */
void initList(IntList *list);
int insertEnd(IntList *list, int value);
int insertAt(IntList *list, int index, int value);
int removeAt(IntList *list, int index);
int search(const IntList *list, int value);
void printList(const IntList *list);
void printMenu(void);

int main(void) {
    IntList list;
    int choice;
    int value;
    int index;
    int result;

    initList(&list);

    do {
        printMenu();
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter value to insert at end: ");
                scanf("%d", &value);
                result = insertEnd(&list, value);
                if (result == -1) {
                    printf("List is full.\n");
                }
                break;

            case 2:
                printf("Enter index: ");
                scanf("%d", &index);
                printf("Enter value: ");
                scanf("%d", &value);
                result = insertAt(&list, index, value);
                if (result == -1) {
                    printf("Invalid index or list is full.\n");
                }
                break;

            case 3:
                printf("Enter index to remove: ");
                scanf("%d", &index);
                result = removeAt(&list, index);
                if (result == -1) {
                    printf("Invalid index.\n");
                }
                break;

            case 4:
                printf("Enter value to search: ");
                scanf("%d", &value);
                result = search(&list, value);
                if (result == -1) {
                    printf("Value not found.\n");
                } else {
                    printf("Value found at index %d.\n", result);
                }
                break;

            case 5:
                printList(&list);
                break;

            case 0:
                printf("Exiting program.\n");
                break;

            default:
                printf("Invalid choice.\n");
        }

    } while (choice != 0);

    return 0;
}

// Initialize list to empty
void initList(IntList *list) {
    list->size = 0;
}

// Add element at end
int insertEnd(IntList *list, int value) {
    if (list->size >= MAX_SIZE) {
        return -1;
    }

    list->data[list->size] = value;
    list->size++;

    return 0;
}

// Insert at specific index, shift elements right
int insertAt(IntList *list, int index, int value) {
    if (index < 0 || index > list->size || list->size >= MAX_SIZE) {
        return -1;
    }

    // Shift elements right to make room
    for (int i = list->size; i > index; i--) {
        list->data[i] = list->data[i - 1];
    }

    list->data[index] = value;
    list->size++;

    return 0;
}

// Remove element at index, shift left
int removeAt(IntList *list, int index) {
    if (index < 0 || index >= list->size) {
        return -1;
    }

    // Shift elements left
    for (int i = index; i < list->size - 1; i++) {
        list->data[i] = list->data[i + 1];
    }

    list->size--;

    return 0;
}

// Search for value, return index or -1
int search(const IntList *list, int value) {
    for (int i = 0; i < list->size; i++) {
        if (list->data[i] == value) {
            return i;
        }
    }

    return -1;
}

// Display all elements
void printList(const IntList *list) {
    if (list->size == 0) {
        printf("List is empty.\n");
        return;
    }

    printf("List: ");
    for (int i = 0; i < list->size; i++) {
        printf("%d", list->data[i]);
        if (i < list->size - 1) {
            printf(" ");
        }
    }
    printf("\n");
}

// Show menu options
void printMenu(void) {
    printf("\nMenu:\n");
    printf("1. Insert at end\n");
    printf("2. Insert at index\n");
    printf("3. Remove at index\n");
    printf("4. Search\n");
    printf("5. Print list\n");
    printf("0. Exit\n");
}

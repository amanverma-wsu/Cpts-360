# PA1 - Integer List

A menu-driven program that implements a fixed-size list using an array. Supports basic operations like insert, remove, search, and display.

## Compile & Run

```bash
gcc PA1.c -o PA1
./PA1
```

## Operations

1. **Insert at end** - Add element to the end of the list
2. **Insert at index** - Insert element at a specific position, shifts remaining elements right
3. **Remove at index** - Remove element at a specific position, shifts remaining elements left
4. **Search** - Find the index of a value in the list
5. **Print list** - Display all current elements

## Assumptions

- User inputs are valid integers when prompted
- List indices are 0-based (first element is at index 0)
- For `insertAt`, valid indices range from 0 to current size (inclusive)
- For `removeAt`, valid indices range from 0 to size-1 (inclusive)
- Search returns the index of the first occurrence if duplicates exist
- Maximum list capacity is 100 elements (defined by MAX_SIZE)

## Example Input and Output

```
Menu:
1. Insert at end
2. Insert at index
3. Remove at index
4. Search
5. Print list
0. Exit
Enter choice: 1
Enter value to insert at end: 10

Menu:
1. Insert at end
2. Insert at index
3. Remove at index
4. Search
5. Print list
0. Exit
Enter choice: 1
Enter value to insert at end: 20

Menu:
1. Insert at end
2. Insert at index
3. Remove at index
4. Search
5. Print list
0. Exit
Enter choice: 5
List: 10 20

Menu:
1. Insert at end
2. Insert at index
3. Remove at index
4. Search
5. Print list
0. Exit
Enter choice: 2
Enter index: 1
Enter value: 15

Menu:
1. Insert at end
2. Insert at index
3. Remove at index
4. Search
5. Print list
0. Exit
Enter choice: 5
List: 10 15 20

Menu:
1. Insert at end
2. Insert at index
3. Remove at index
4. Search
5. Print list
0. Exit
Enter choice: 4
Enter value to search: 15
Value found at index 1.

Menu:
1. Insert at end
2. Insert at index
3. Remove at index
4. Search
5. Print list
0. Exit
Enter choice: 3
Enter index to remove: 1

Menu:
1. Insert at end
2. Insert at index
3. Remove at index
4. Search
5. Print list
0. Exit
Enter choice: 5
List: 10 20

Menu:
1. Insert at end
2. Insert at index
3. Remove at index
4. Search
5. Print list
0. Exit
Enter choice: 0
Exiting program.
```
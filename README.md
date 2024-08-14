# C++ and C Suite
## Introduction
The C++ and C Suite is a collection of four console applications written in C and C++, each demonstrating unique functionalities ranging from weather simulation to file manipulation and worker management.

C++
- Weather Simulator (weather_sim)
- Array Accumulator (arracc)
  
C
- Line Flipper (reverse)
- Vineyard Workers Management (vineyard)
  
## Weather Simulator
Folder: `weather_sim`
### Description
This program simulates the hydrological cycle of the Earth, focusing on how different types of land areas affect and are affected by weather changes. Each land area has a name, type (plain, green, lake), and stored water amount (in cubic kilometers). The common air above these land areas has a humidity level (in percentage).

### Weather Determination
The weather for the day depends on the current day's humidity:
- If the humidity exceeds 70%, it will rain, and the humidity will drop to 30%.
- If the humidity is below 40%, it will be sunny.
- If the humidity is between 40% and 70%, the weather has a chance of being rainy or cloudy. The probability of rain is (humidity - 40) * 3.3 percent. Otherwise, the weather will be cloudy.
### Land Area Reactions
Each land area reacts to different weather conditions in the order they are defined:
1. Plain (P):
    - Sunny: Water amount decreases by 3 km³.
    - Cloudy: Water amount decreases by 1 km³.
    - Rainy: Water amount increases by 5 km³.
    - Increases air humidity by 3%.
    - Changes to green if water amount exceeds 15 km³.
2. Green (Z):
    - Sunny: Water amount decreases by 6 km³.
    - Cloudy: Water amount decreases by 2 km³.
    - Rainy: Water amount increases by 10 km³.
    - Increases air humidity by 7%.
    - Changes to lake if water amount exceeds 50 km³.
    - Changes to plain if water amount falls below 16 km³.
3. Lake (T):
    - Sunny: Water amount decreases by 10 km³.
    - Cloudy: Water amount decreases by 3 km³.
    - Rainy: Water amount increases by 15 km³.
    - Increases air humidity by 10%.
    - Changes to green if water amount falls below 51 km³.

The simulation runs in cycles until all land areas become the same type. At each cycle, the properties of each land area are displayed.

### Input File Format
The program reads the initial data from a text file. The format of the file is:
- The first line contains the number of land areas.
- The subsequent lines contain the properties of each land area: owner name (string), type (p, z, t), and initial water amount (integer).
- The last line contains the initial humidity level.

Example Input File:
   ```txt
   4
   Bean t 86
   Green z 26
   Dean p 12
   Teen z 35
   98
   ```

## Array Accumlator
Folder: `arracc`
### Description
This program involves creating an array_accumulator class template in C++. This class will transform a given array so that each element at index i contains the cumulative sum (or other specified binary operation result) of all elements from the start up to index i. Additionally, when the array_accumulator object is destroyed, the original array will be restored to its initial state. The class will also support adding additional arrays, where the accumulation will continue from where the previous array left off.
### Class Specification
#### Template Parameters
- T: Type of elements stored in the arrays.
- BinPred: Binary operation to apply to accumulate the values (default is std::plus<T>).
#### Public Methods
- Constructor: Accepts a reference to an array and transforms it as described.
- Destructor: Restores the array to its original state.
- add: Accepts additional arrays, continuing the accumulation from the end of the last transformed array.
- size: Returns the total number of original elements stored.

## Line Flipper
Folder: `reverse`
### Description
This program involves implementing a reverse command in C. The reverse command processes the contents of files provided as arguments, or reads lines from the console if no files are specified. It then outputs the lines in reverse order, with each line reversed, and numbered from the end to the beginning.
### Features
- Reads lines from one or more files or from standard input.
- Reverses the order of the lines.
- Reverses the content of each line.
- Numbers the lines starting from the end.
- Press Ctrl+D (on Unix) or Ctrl+Z (on Windows) to indicate the end of input.

### Example
Given a file test.txt with the following content:

```txt
apple
banana
cherry
```
Running ./a.out test.txt will produce:
```txt
3 yrrehc
2 ananab
1 elppa
```
If test.txt is specified twice, the output will be:
```txt
3 yrrehc
2 ananab
1 elppa
3 yrrehc
2 ananab
1 elppa
```
### File Structure
The project consists of the following files:
- main.c: Contains the main function and the reverseThem function.
- reverse.c: Contains the implementations of helper functions.
- reverse.h: Contains the declarations of helper functions.

## Vineyard Workers Management
`Folder: vineyard`
### Description
This is a C/C++ command-line application designed for managing workers at a vineyard. The primary function of this application is to handle the scheduling and transportation of vineyard workers who apply to work on different days of the week during the spring season. The application facilitates adding new workers, listing their details, modifying records, deleting them, and organizing transportation using minibuses.

### Features
#### Worker Management
- Add New Worker: Allows users to enter a new worker's name and the days they are available to work. The details are stored in a file.
- List Workers: Displays all workers and their available days from the file, providing a comprehensive overview of the current list of applicants.
- Modify Records: Enables users to update an existing worker's information, such as changing the days they are available to work.
- Delete Records: Removes a worker's information from the list, effectively deleting their record from the file.
#### Transportation Management
- Start Buses: Organizes the transportation of workers using two minibuses, each with a capacity of 5 workers. The application coordinates bus operations based on the available workers for a specific day.
- Inter-Process Communication (IPC): Utilizes pipes and message queues to handle communication between the main application and the child processes responsible for managing bus operations. This allows for real-time coordination and status updates.
#### File Operations
- Data Persistence: Workers' data is saved to and retrieved from a file, ensuring that the list of applicants persists between program runs.
- File Handling: Implements robust file handling operations, including creating, reading, writing, and appending to files.
#### User Interface
- Menu-Driven Interface: Provides a simple text-based menu that allows users to navigate through various functionalities such as adding new workers, listing records, and managing transportation.
- Error Handling: Includes error handling for file operations and IPC mechanisms, ensuring that the application can gracefully handle issues and provide feedback to the user.

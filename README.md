In this project, there are several submissions grouped by programming language and functionality. Here's an overview of the submissions:
## C Projects:
- In beadando0 there is a program that expects a text file (.txt) and reverses the order of the words in the file.
- In beadando1 there is a program for managing workers and their workdays.
- In beadando2 there is a program for managing workers and organizing transportation for them, including bus scheduling.
## C++ Projects:
- In weather_sim there is a weather simulation program.
- In arracc there is a implementation of the array_accumulator class.

# Weather Simulator
## Project Description
This program simulates the hydrological cycle of the Earth, focusing on how different types of land areas affect and are affected by weather changes. Each land area has a name, type (plain, green, lake), and stored water amount (in cubic kilometers). The common air above these land areas has a humidity level (in percentage).

## Weather Determination
The weather for the day depends on the current day's humidity:
- If the humidity exceeds 70%, it will rain, and the humidity will drop to 30%.
- If the humidity is below 40%, it will be sunny.
- If the humidity is between 40% and 70%, the weather has a chance of being rainy or cloudy. The probability of rain is (humidity - 40) * 3.3 percent. Otherwise, the weather will be cloudy.
## Land Area Reactions
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

## Input File Format
The program reads the initial data from a text file. The format of the file is:
- The first line contains the number of land areas.
- The subsequent lines contain the properties of each land area: owner name (string), type (p, z, t), and initial water amount (integer).
- The last line contains the initial humidity level.

Example Input File:
    ```bash
    4
    Bean t 86
    Green z 26
    Dean p 12
    Teen z 35
    98
    ```

# Array Accumlator
This program involves creating an array_accumulator class template in C++. This class will transform a given array so that each element at index i contains the cumulative sum (or other specified binary operation result) of all elements from the start up to index i. Additionally, when the array_accumulator object is destroyed, the original array will be restored to its initial state. The class will also support adding additional arrays, where the accumulation will continue from where the previous array left off.
## Class Specification
### Template Parameters
- T: Type of elements stored in the arrays.
- BinPred: Binary operation to apply to accumulate the values (default is std::plus<T>).
### Public Methods
- Constructor: Accepts a reference to an array and transforms it as described.
- Destructor: Restores the array to its original state.
- add: Accepts additional arrays, continuing the accumulation from the end of the last transformed array.
- size: Returns the total number of original elements stored.
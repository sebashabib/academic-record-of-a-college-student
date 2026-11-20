# Student Academic Record System

A C++ console program for managing students' academic records. It calculates each student's grades in every course, works out their GPA, ranks them against the other students, and flags anyone who's on academic warning.

## How grades are calculated

Each student takes 8 courses. The grade for each course is calculated like this:

| Component   | Weight |
|-------------|--------|
| Quizzes     | 20%    |
| Assignments | 20%    |
| Midterm     | 25%    |
| Final exam  | 35%    |

## Features

The program runs from a menu with these options:

1. Add a new student with their grades
2. Update a student's name
3. Delete a student
4. Search for a student and view their transcript (GPA + academic standing)
5. Edit a student's course grades
6. Sort students by GPA and show the top 5
7. Course statistics and an overall summary
8. List students with academic warnings
9. Normalize grades (mean 75, SD 10) and compare rankings before and after
10. Swap two students' records

A student gets a warning if their GPA is below 2.0 or if they failed two or more courses.

## Running it

You can run it directly on OnlineGDB:
https://onlinegdb.com/qlVErUkXi

Or compile it locally:

    g++ main.cpp -o main
    ./main

## Notes

- Data is stored in parallel arrays and accessed with pointers.
- User input is validated, so invalid values are rejected.
- Data isn't saved to a file. Everything resets when the program closes.

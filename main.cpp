#include <iostream>
#include <string>
#include <iomanip>
#include <cmath>
using namespace std;


const int MAX_STUDENTS = 100;
const int NUM_COURSES = 8;
int studentCount = 4;

// Parallel arrays: the same index belongs to the same student.
int studentID[MAX_STUDENTS] = {101, 102, 103, 104};
string studentName[MAX_STUDENTS] = {"Seba", "Haneen", "Lana", "Atheer"};

// Each row is a student and each column is a course.
double quizzes[MAX_STUDENTS][NUM_COURSES] = {
    {90, 85, 88, 92, 95, 80, 87, 90},
    {75, 80, 78, 85, 82, 88, 79, 90},
    {95, 92, 90, 96, 94, 91, 93, 97},
    {60, 65, 70, 68, 72, 75, 66, 70}
};
double assignments[MAX_STUDENTS][NUM_COURSES] = {
    {85, 90, 88, 95, 92, 87, 90, 93},
    {80, 85, 75, 90, 88, 82, 85, 87},
    {95, 90, 94, 98, 96, 92, 95, 97},
    {65, 70, 68, 72, 75, 70, 73, 68}
};
double midterm[MAX_STUDENTS][NUM_COURSES] = {
    {88, 85, 90, 92, 87, 89, 91, 90},
    {75, 78, 80, 82, 85, 79, 81, 83},
    {96, 94, 92, 95, 98, 93, 97, 96},
    {60, 65, 62, 68, 70, 66, 64, 63}
};
double finalExam[MAX_STUDENTS][NUM_COURSES] = {
    {90, 92, 88, 95, 93, 87, 91, 94},
    {80, 82, 78, 85, 87, 83, 81, 86},
    {98, 95, 96, 97, 94, 99, 93, 98},
    {65, 68, 70, 72, 66, 69, 71, 67}
};
double finalGrades[MAX_STUDENTS][NUM_COURSES] = {0};

// 1. Validate a number and reject extra text on its line. Return -1 if input ends.
double readNumber(string prompt, double minimum, double maximum, bool integerOnly) {
    while (true) {
        cout << prompt;
        double value = 0;
        string remaining;

        if (cin >> value) {
            getline(cin, remaining);
            if (remaining.find_first_not_of(" \t\r") == string::npos &&
                isfinite(value) && value >= minimum && value <= maximum &&
                (!integerOnly || value == floor(value))) {
                return value;
            }
        } else {
            cin.clear();
            if (!getline(cin, remaining)) return -1;
        }

        cout << "Invalid input. Enter ";
        if (integerOnly) cout << "a whole number ";
        else cout << "a number ";
        cout << "from " << minimum << " to " << maximum << ".\n";
    }
}

// 2. Find a student by unique ID.
int findStudent(int id) {
    for (int i = 0; i < studentCount; i++) {
        if (studentID[i] == id) return i;
    }
    return -1;
}

// 3. Assign a course letter grade.
char getLetterGrade(double mark) {
    if (mark >= 90) return 'A';
    if (mark >= 80) return 'B';
    if (mark >= 70) return 'C';
    if (mark >= 60) return 'D';
    return 'F';
}

// 4. Calculate GPA using pointer arithmetic on a row of course grades.
double calculateGPA(const double* marks) {
    double points = 0;
    for (int c = 0; c < NUM_COURSES; c++) {
        char grade = getLetterGrade(*(marks + c));
        if (grade == 'A') points += 4;
        else if (grade == 'B') points += 3;
        else if (grade == 'C') points += 2;
        else if (grade == 'D') points += 1;
    }
    return points / NUM_COURSES;
}

// 5. Modify grades through pointer parameters and pointer arithmetic.
// course = -1 enters all eight courses; 0..7 changes one course.
bool modifyGrades(double* q, double* a, double* m, double* f, double* result, int course) {
    if (course < -1 || course >= NUM_COURSES) return false;
    int start = 0;
    int end = NUM_COURSES;
    if (course >= 0) {
        start = course;
        end = course + 1;
    }
    for (int c = start; c < end; c++) {
        cout << "\nCourse " << c + 1 << " (enter each component out of 100)\n";
        double quiz = readNumber("Quizzes: ", 0, 100, false);
        if (quiz < 0) return false;
        double assignment = readNumber("Assignments: ", 0, 100, false);
        if (assignment < 0) return false;
        double exam1 = readNumber("Midterm: ", 0, 100, false);
        if (exam1 < 0) return false;
        double exam2 = readNumber("Final exam: ", 0, 100, false);
        if (exam2 < 0) return false;

        *(q + c) = quiz;
        *(a + c) = assignment;
        *(m + c) = exam1;
        *(f + c) = exam2;
        *(result + c) = quiz * 0.20 + assignment * 0.20 + exam1 * 0.25 + exam2 * 0.35;
    }
    return true;
}

// 6. Exchange complete records. Array parameters are pointers to their rows.
void swapStudents(int first, int second, double q[][NUM_COURSES],
                  double a[][NUM_COURSES], double m[][NUM_COURSES],
                  double f[][NUM_COURSES], double grades[][NUM_COURSES]) {
    if (first < 0 || second < 0 || first >= studentCount || second >= studentCount) return;
    int tempID = studentID[first];
    studentID[first] = studentID[second];
    studentID[second] = tempID;
    string tempName = studentName[first];
    studentName[first] = studentName[second];
    studentName[second] = tempName;
    for (int c = 0; c < NUM_COURSES; c++) {
        double temp = q[first][c];
        q[first][c] = q[second][c];
        q[second][c] = temp;
        temp = a[first][c];
        a[first][c] = a[second][c];
        a[second][c] = temp;
        temp = m[first][c];
        m[first][c] = m[second][c];
        m[second][c] = temp;
        temp = f[first][c];
        f[first][c] = f[second][c];
        f[second][c] = temp;
        temp = grades[first][c];
        grades[first][c] = grades[second][c];
        grades[second][c] = temp;
    }
}

// 7. Display an individual transcript and academic standing.
void displayTranscript(int index) {
    if (index < 0 || index >= studentCount) {
        cout << "Student not found.\n";
        return;
    }
    cout << "\nStudent: " << studentName[index] << " | ID: " << studentID[index] << "\n";
    cout << left << setw(9) << "Course" << setw(10) << "Quizzes"
         << setw(13) << "Assignments" << setw(10) << "Midterm"
         << setw(12) << "Final exam" << setw(10) << "Total" << "Grade\n";
    int failed = 0;
    for (int c = 0; c < NUM_COURSES; c++) {
        char grade = getLetterGrade(finalGrades[index][c]);
        if (grade == 'F') failed++;
        cout << left << setw(9) << c + 1 << setw(10) << quizzes[index][c]
             << setw(13) << assignments[index][c] << setw(10) << midterm[index][c]
             << setw(12) << finalExam[index][c] << setw(10) << finalGrades[index][c]
             << grade << "\n";
    }
    double gpa = calculateGPA(finalGrades[index]);
    cout << "GPA: " << gpa << " / 4.00\nAcademic standing: ";
    if (gpa >= 3.5) cout << "Excellent\n";
    else if (gpa >= 3.0) cout << "Very Good\n";
    else if (gpa >= 2.5) cout << "Good\n";
    else if (gpa >= 2.0) cout << "Satisfactory\n";
    else cout << "Probation\n";
    cout << "Failed courses: " << failed << "\n";
    if (gpa < 2.0 || failed >= 2) cout << "Academic warning.\n";
}

// 8. Student management: 1 add, 2 update name, 3 delete, 4 search.
void manageStudent(int action) {
    if (action < 1 || action > 4) return;
    if (action == 1 && studentCount == MAX_STUDENTS) {
        cout << "The maximum of 100 students has been reached.\n";
        return;
    }
    if (action != 1 && studentCount == 0) {
        cout << "No students.\n";
        return;
    }
    int id = static_cast<int>(readNumber("Student ID: ", 1, 2147483647, true));
    if (id < 0) return;
    int index = findStudent(id);
    if (action == 1) {
        if (index != -1) {
            cout << "Duplicate ID. The student was not added.\n";
            return;
        }
        index = studentCount;
    } else if (index == -1) {
        cout << "Student not found.\n";
        return;
    }

    if (action == 1 || action == 2) {
        string name;
        do {
            cout << "Student name: ";
            if (!getline(cin, name)) return;
            if (name.find_first_not_of(" \t\r") == string::npos)
                cout << "The name cannot be empty.\n";
        } while (name.find_first_not_of(" \t\r") == string::npos);

        if (action == 1) {
            if (!modifyGrades(quizzes[index], assignments[index], midterm[index],
                              finalExam[index], finalGrades[index], -1)) return;
            studentID[index] = id;
            studentName[index] = name;
            studentCount++;
            cout << "Student added.\n";
        } else {
            studentName[index] = name;
            cout << "Student name updated.\n";
        }
    } else if (action == 3) {
        for (int i = index; i < studentCount - 1; i++) {
            swapStudents(i, i + 1, quizzes, assignments, midterm, finalExam, finalGrades);
        }
        studentCount--;
        studentID[studentCount] = 0;
        studentName[studentCount] = "";
        cout << "Student deleted.\n";
    } else {
        displayTranscript(index);
    }
}

// 9. Sort complete records by descending GPA and display up to five students.
void displayTopFive() {
    if (studentCount == 0) {
        cout << "No students.\n";
        return;
    }
    for (int i = 0; i < studentCount - 1; i++) {
        for (int j = 0; j < studentCount - i - 1; j++) {
            if (calculateGPA(finalGrades[j]) < calculateGPA(finalGrades[j + 1])) {
                swapStudents(j, j + 1, quizzes, assignments, midterm, finalExam, finalGrades);
            }
        }
    }
    int limit = studentCount;
    if (limit > 5) limit = 5;
    cout << "\nTop students by GPA\n";
    for (int i = 0; i < limit; i++) {
        cout << i + 1 << ". " << studentName[i] << " | ID: " << studentID[i]
             << " | GPA: " << calculateGPA(finalGrades[i]) << "\n";
    }
}

// 10. Course statistics and a university-wide academic summary.
void displayStatistics() {
    if (studentCount == 0) {
        cout << "No students.\n";
        return;
    }
    for (int c = 0; c < NUM_COURSES; c++) {
        double sum = 0;
        double highest = finalGrades[0][c];
        double lowest = finalGrades[0][c];
        int passed = 0;
        int distribution[5] = {0};
        for (int i = 0; i < studentCount; i++) {
            double mark = finalGrades[i][c];
            sum += mark;
            if (mark > highest) highest = mark;
            if (mark < lowest) lowest = mark;
            if (mark >= 60) passed++;
            char grade = getLetterGrade(mark);
            if (grade == 'A') distribution[0]++;
            else if (grade == 'B') distribution[1]++;
            else if (grade == 'C') distribution[2]++;
            else if (grade == 'D') distribution[3]++;
            else distribution[4]++;
        }
        cout << "\nCourse " << c + 1 << " statistics\n"
             << "Average: " << sum / studentCount << "\n"
             << "Highest: " << highest << " | Lowest: " << lowest << "\n"
             << "Pass: " << passed * 100.0 / studentCount << "%"
             << " | Fail: " << (studentCount - passed) * 100.0 / studentCount << "%\n"
             << "A: " << distribution[0] << " | B: " << distribution[1]
             << " | C: " << distribution[2] << " | D: " << distribution[3]
             << " | F: " << distribution[4] << "\n";
    }
    double totalGPA = 0;
    int warnings = 0;
    for (int i = 0; i < studentCount; i++) {
        double gpa = calculateGPA(finalGrades[i]);
        totalGPA += gpa;
        int failed = 0;
        for (int c = 0; c < NUM_COURSES; c++) {
            if (finalGrades[i][c] < 60) failed++;
        }
        if (gpa < 2.0 || failed >= 2) warnings++;
    }
    cout << "\nUniversity academic summary\n"
         << "Students: " << studentCount << "\n"
         << "Average GPA: " << totalGPA / studentCount << "\n"
         << "Students with warnings: " << warnings << "\n"
         << "Students without warnings: " << studentCount - warnings << "\n";
}

// 11. Identify students with GPA below 2.0 OR at least two failed courses.
void displayWarnings() {
    int warnings = 0;
    cout << "\nAcademic warnings\n";
    for (int i = 0; i < studentCount; i++) {
        double gpa = calculateGPA(finalGrades[i]);
        int failed = 0;
        for (int c = 0; c < NUM_COURSES; c++) {
            if (finalGrades[i][c] < 60) failed++;
        }
        if (gpa < 2.0 || failed >= 2) {
            warnings++;
            cout << studentName[i] << " | ID: " << studentID[i]
                 << " | GPA: " << gpa << " | Failed courses: " << failed << "\n";
        }
    }
    if (warnings == 0) cout << "No students with academic warnings.\n";
}

// 12. Normalize course grades and compare GPA rankings before and after.
void normalizeGrades() {
    if (studentCount == 0) {
        cout << "No students.\n";
        return;
    }
    double normalized[MAX_STUDENTS][NUM_COURSES] = {0};
    cout << "\nNormalization: target mean = 75, target standard deviation = 10.\n"
         << "Formula: 75 + 10 * (mark - course mean) / course standard deviation.\n"
         << "Results are limited to 0..100. Zero deviation keeps the original grades.\n"
         << "This comparison report keeps the original student records.\n";
    for (int c = 0; c < NUM_COURSES; c++) {
        double sum = 0;
        for (int i = 0; i < studentCount; i++) sum += finalGrades[i][c];
        double mean = sum / studentCount;
        double variance = 0;
        for (int i = 0; i < studentCount; i++) {
            double difference = finalGrades[i][c] - mean;
            variance += difference * difference;
        }
        double deviation = sqrt(variance / studentCount);
        cout << "Course " << c + 1 << " | Mean: " << mean << " | SD: " << deviation << "\n";
        for (int i = 0; i < studentCount; i++) {
            double mark = finalGrades[i][c];
            if (deviation > 0.000000001) mark = 75 + 10 * (mark - mean) / deviation;
            if (mark < 0) mark = 0;
            if (mark > 100) mark = 100;
            normalized[i][c] = mark;
        }
    }
    double beforeGPA[MAX_STUDENTS];
    double afterGPA[MAX_STUDENTS];
    int beforeOrder[MAX_STUDENTS];
    int afterOrder[MAX_STUDENTS];
    int beforeRank[MAX_STUDENTS];
    for (int i = 0; i < studentCount; i++) {
        beforeGPA[i] = calculateGPA(finalGrades[i]);
        afterGPA[i] = calculateGPA(normalized[i]);
        beforeOrder[i] = i;
        afterOrder[i] = i;
    }
    for (int i = 0; i < studentCount - 1; i++) {
        for (int j = 0; j < studentCount - i - 1; j++) {
            if (beforeGPA[beforeOrder[j]] < beforeGPA[beforeOrder[j + 1]]) {
                int temp = beforeOrder[j];
                beforeOrder[j] = beforeOrder[j + 1];
                beforeOrder[j + 1] = temp;
            }
            if (afterGPA[afterOrder[j]] < afterGPA[afterOrder[j + 1]]) {
                int temp = afterOrder[j];
                afterOrder[j] = afterOrder[j + 1];
                afterOrder[j + 1] = temp;
            }
        }
    }
    for (int i = 0; i < studentCount; i++) beforeRank[beforeOrder[i]] = i + 1;
    cout << "\nID | Name | GPA before | GPA after | Rank before | Rank after\n";
    for (int rank = 0; rank < studentCount; rank++) {
        int index = afterOrder[rank];
        cout << studentID[index] << " | " << studentName[index]
             << " | " << beforeGPA[index] << " | " << afterGPA[index]
             << " | " << beforeRank[index] << " | " << rank + 1 << "\n";
        cout << "Normalized grades: ";
        for (int c = 0; c < NUM_COURSES; c++) cout << normalized[index][c] << " ";
        cout << "\n";
    }
}

int main() {
    cout << fixed << setprecision(2);
    for (int i = 0; i < studentCount; i++) {
        for (int c = 0; c < NUM_COURSES; c++) {
            finalGrades[i][c] = quizzes[i][c] * 0.20 + assignments[i][c] * 0.20
                              + midterm[i][c] * 0.25 + finalExam[i][c] * 0.35;
        }
    }
    int choice;
    do {
        cout << "\n===== Student Academic Management =====\n"
             << "1. Add student and grades\n"
             << "2. Update student name\n"
             << "3. Delete student\n"
             << "4. Search student and display transcript\n"
             << "5. Modify course grades\n"
             << "6. Sort by GPA and display top five\n"
             << "7. Course statistics and university summary\n"
             << "8. Academic warnings\n"
             << "9. Normalize grades and compare rankings\n"
             << "10. Exchange two student records\n"
             << "0. Exit\n";
        choice = static_cast<int>(readNumber("Choose: ", 0, 10, true));
        if (choice < 0) break;
        switch (choice) {
            case 1:
            case 2:
            case 3:
            case 4:
                manageStudent(choice);
                break;
            case 5: {
                int id = static_cast<int>(readNumber("Student ID: ", 1, 2147483647, true));
                if (id < 0) break;
                int index = findStudent(id);
                if (index == -1) {
                    cout << "Student not found.\n";
                    break;
                }
                int course = static_cast<int>(readNumber("Course number (1-8): ", 1, NUM_COURSES, true));
                if (course < 0) break;
                if (modifyGrades(quizzes[index], assignments[index], midterm[index],
                                 finalExam[index], finalGrades[index], course - 1)) {
                    cout << "Course grades updated.\n";
                }
                break;
            }
            case 6:
                displayTopFive();
                break;
            case 7:
                displayStatistics();
                break;
            case 8:
                displayWarnings();
                break;
            case 9:
                normalizeGrades();
                break;
            case 10: {
                if (studentCount < 2) {
                    cout << "At least two students are needed.\n";
                    break;
                }
                int firstID = static_cast<int>(readNumber("First student ID: ", 1, 2147483647, true));
                if (firstID < 0) break;
                int first = findStudent(firstID);
                if (first == -1) {
                    cout << "Student not found.\n";
                    break;
                }
                int secondID = static_cast<int>(readNumber("Second student ID: ", 1, 2147483647, true));
                if (secondID < 0) break;
                int second = findStudent(secondID);
                if (second == -1) cout << "Student not found.\n";
                else if (first == second) cout << "Choose two different students.\n";
                else {
                    swapStudents(first, second, quizzes, assignments, midterm, finalExam, finalGrades);
                    cout << "Complete student records exchanged.\n";
                }
                break;
            }
            case 0:
                break;
        }
    } while (choice != 0);
    cout << "Goodbye.\n";
    return 0;
}

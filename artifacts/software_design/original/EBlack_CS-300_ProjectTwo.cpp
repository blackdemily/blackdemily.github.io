//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Emily Black
// Version     : 1.0
// Description : Advising Assistance Program
//============================================================================

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

// data structure used to hold course information
struct Course {
    string courseNumber;            // course number
    string courseTitle;             // course title
    vector<string> prerequisites;   // prerequisites for the applicable courses
};

// vector data structure used to store course objects
vector<Course> courses;

// future TODO: Create class CourseRepository to manage loading and storing courses
// future TODO: Create class CourseService to handle business logic
// future TODO: Create class MenuUI to manage UI and menu loop

/*
Future TODO: replace CSV with DB
Example: 

CREATE TABLE courses(
  course_id VARCHAR(10) PRIMARY KEY,
  title     VARCHAR(255),
  prereqs   VARCHAR(255)
);
SELECT * FROM courses WHERE course_id = ?;
*/

// function used to load data from a file into the courses vector data structure
void loadDataStructure(const string& fileName) {
    ifstream file(fileName);        // open the file 
    if (!file.is_open()) {          // check to see if the file failed while opening
        cout << "Unable to open file: " << fileName << endl;  // error message for failed file opening
        return;
    }

    // string used to store each line of the file
    string line;
    while (getline(file, line)) {                   // read each line of the file
        Course course;                              // creation of course object to store course information
        size_t pos = line.find(",");                // find the position of the first comma
        course.courseNumber = line.substr(0, pos);  // extract the course number
        line.erase(0, pos + 1);                     // erase the extracted course number
        pos = line.find(",");                       // find the position of the next comma
        course.courseTitle = line.substr(0, pos);   // extract course title
        line.erase(0, pos + 1);                     // erase the extracted course title

        // extract and store prerequisites
        while (!line.empty()) {
            pos = line.find(",");                           // find the position of the next comma
            string prerequisite = line.substr(0, pos);      // extract the prerequisite
            course.prerequisites.push_back(prerequisite);   // store the prerequisite
            if (pos == string::npos) break;      // if not more commas exist, exit (break) from the loop
            line.erase(0, pos + 1);                         // erase the extracted prerequisite
        }

        // add the course object to the courses vector
        courses.push_back(course);
    }

    // close the file
    file.close();
}

// function used to print a list of courses in alphanumerical order
void printCourseList() {
    sort(courses.begin(), courses.end(), [](const Course& a, const Course& b) {
        return a.courseNumber < b.courseNumber;
        });

    cout << "Here is a list of courses:" << endl;       // print header
    cout << endl;
    
    // loop through each course and print its number and title
    for (const auto& course : courses) {
        cout << course.courseNumber << ", " << course.courseTitle << endl;
    }
}

// function used to print information about a specific course
void printCourse(const string& courseNumber) {

    // find the course with the given course number
    auto it = find_if(courses.begin(), courses.end(), [&](const Course& c) {
        return c.courseNumber == courseNumber;
        });
    
    // if course is found, print the course number and title
    if (it != courses.end()) {
        cout << it->courseNumber << ", " << it->courseTitle << endl;
        cout << "Prerequisites: ";
        
        // if no prerequisites exist, print "None"
        if (it->prerequisites.empty()) {
            cout << "None" << endl;
        }

        // if a prerequisite does exist, loop through each and print each applicable one
        else {
            for (const auto& prerequisite : it->prerequisites) {
                cout << prerequisite << ", ";
            }
            cout << endl;
        }
    }
    
    // if the course entered by the user does not exist, print error message
    else {
        cout << "Course not found." << endl;
    }
}

int main() {
    string fileName;        // string used to store file name
    int choice;             // integer used to store user's choice
    string courseNumber;    // string used to store course number

    // print welcome message
    cout << "Welcome to the course planner." << endl;
    cout << endl;

    // a do-while loop used to handle the main flow of the program
    do {
        // display menu option
        cout << "1. Load Data Structure" << endl;
        cout << "2. Print Course List" << endl;
        cout << "3. Print Course" << endl;
        cout << "9. Exit" << endl;
        cout << endl;
        
        // prompt the user for a choice
        cout << "What would you like to do? ";
        cout << endl;
        
        // read the user's choice
        cin >> choice;
        cout << endl;

        // switch statement based on user's choice
        switch (choice) {
            
            // if user chooses option 1
            case 1:
                // prompt the user for a file name
                cout << "Enter the file name: ";
                cout << endl;
                
                // read the file name given by the user
                cin >> fileName;
                cout << endl;
                
                // call the loadDataStructure() function to load data from the file
                loadDataStructure(fileName);
                break;

            // if user chooses option 2
            case 2:
                // call the printCourseList() function to print the course list
                printCourseList();
                cout << endl;
                break;

            // if user chooses option 3
            case 3:
                // prompt the user for a course number
                cout << "What course number do you want to know about? ";
                cout << endl;
                
                // read the course number give by the user
                cin >> courseNumber; 
                cout << endl;

                // call the printCourse(courseNumber) function to print the course's information
                printCourse(courseNumber);
                cout << endl;
                break;
                
            // if user chooses option 9
            case 9:
                // print the "goodbye" message
                cout << "Thank you for using the course planner! Goodbye!" << endl;
                break;

            // if the user chooses an invalid option
            default:
                cout << choice << " is not a valid option." << endl;
                cout << endl;
        }
    }

    // continue the do-while loop until user chooses to exit
    while (choice != 9);

    return 0;
}

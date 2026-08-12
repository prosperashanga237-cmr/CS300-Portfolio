/*
 * CS 300 Project Two - ABCU Advising Assistance Program
 * Prosper Ashanga
 *
 * This command-line program reads ABC University course information from a
 * comma-separated file and stores the course objects in a binary search tree.
 * It can print all courses in alphanumeric order and display an individual
 * course with its prerequisite course numbers and titles.
 */

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// A single course record from the input file.
struct Course {
    string courseNumber;
    string title;
    vector<string> prerequisites;
};

// Remove leading and trailing whitespace from user/file input.
string trim(const string& value) {
    const size_t first = value.find_first_not_of(" \t\r\n");
    if (first == string::npos) {
        return "";
    }
    const size_t last = value.find_last_not_of(" \t\r\n");
    return value.substr(first, last - first + 1);
}

// Normalize course numbers so searches are not case sensitive.
string toUpper(string value) {
    transform(value.begin(), value.end(), value.begin(),
              [](unsigned char ch) { return static_cast<char>(toupper(ch)); });
    return value;
}

// Split one comma-separated line. The supplied ABCU file does not use quoted
// commas, so a simple CSV split is sufficient and keeps this project self-contained.
vector<string> splitCsvLine(const string& line) {
    vector<string> tokens;
    string token;
    stringstream stream(line);

    while (getline(stream, token, ',')) {
        tokens.push_back(trim(token));
    }

    // Preserve a trailing empty CSV field (for example: CSCI100,Title,,).
    if (!line.empty() && line.back() == ',') {
        tokens.push_back("");
    }

    return tokens;
}

class BinarySearchTree {
private:
    struct Node {
        Course course;
        Node* left;
        Node* right;

        explicit Node(const Course& courseValue)
            : course(courseValue), left(nullptr), right(nullptr) {}
    };

    Node* root;

    // Recursively insert a course according to its alphanumeric course number.
    Node* insertNode(Node* node, const Course& course) {
        if (node == nullptr) {
            return new Node(course);
        }

        if (course.courseNumber < node->course.courseNumber) {
            node->left = insertNode(node->left, course);
        } else if (course.courseNumber > node->course.courseNumber) {
            node->right = insertNode(node->right, course);
        }

        return node;
    }

    // In-order traversal naturally prints BST keys from lowest to highest.
    void inOrder(Node* node) const {
        if (node == nullptr) {
            return;
        }

        inOrder(node->left);
        cout << node->course.courseNumber << ", " << node->course.title << '\n';
        inOrder(node->right);
    }

    // Delete every node so dynamically allocated memory is released cleanly.
    void destroy(Node* node) {
        if (node == nullptr) {
            return;
        }
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

public:
    BinarySearchTree() : root(nullptr) {}

    ~BinarySearchTree() {
        clear();
    }

    // Prevent accidental shallow copies of raw tree pointers.
    BinarySearchTree(const BinarySearchTree&) = delete;
    BinarySearchTree& operator=(const BinarySearchTree&) = delete;

    void clear() {
        destroy(root);
        root = nullptr;
    }

    bool empty() const {
        return root == nullptr;
    }

    bool insert(const Course& course) {
        if (search(course.courseNumber) != nullptr) {
            return false; // Duplicate course number.
        }
        root = insertNode(root, course);
        return true;
    }

    const Course* search(const string& courseNumber) const {
        const string key = toUpper(trim(courseNumber));
        Node* current = root;

        while (current != nullptr) {
            if (key == current->course.courseNumber) {
                return &current->course;
            }
            current = (key < current->course.courseNumber)
                          ? current->left
                          : current->right;
        }
        return nullptr;
    }

    void printAll() const {
        inOrder(root);
    }
};

// Read, validate, and load a complete file. Data is first collected in a
// temporary vector so an invalid file never leaves a partially loaded tree.
bool loadCoursesFromFile(const string& fileName, BinarySearchTree& tree) {
    ifstream inputFile(fileName);
    if (!inputFile.is_open()) {
        cout << "Error: Unable to open file '" << fileName << "'.\n";
        return false;
    }

    vector<Course> parsedCourses;
    string line;
    size_t lineNumber = 0;

    while (getline(inputFile, line)) {
        ++lineNumber;
        line = trim(line);

        if (line.empty()) {
            // Ignore blank lines without treating them as course records.
            continue;
        }

        vector<string> tokens = splitCsvLine(line);
        if (tokens.size() < 2 || tokens[0].empty() || tokens[1].empty()) {
            cout << "Error: Invalid format on line " << lineNumber
                 << ". Each course requires a course number and title.\n";
            return false;
        }

        Course course;
        course.courseNumber = toUpper(tokens[0]);
        course.title = tokens[1];

        // Every non-empty field after the title is a prerequisite course number.
        for (size_t index = 2; index < tokens.size(); ++index) {
            if (!tokens[index].empty()) {
                course.prerequisites.push_back(toUpper(tokens[index]));
            }
        }

        // Detect duplicate course numbers before building the final tree.
        const bool duplicate = any_of(
            parsedCourses.begin(), parsedCourses.end(),
            [&](const Course& existing) {
                return existing.courseNumber == course.courseNumber;
            });

        if (duplicate) {
            cout << "Error: Duplicate course number '" << course.courseNumber
                 << "' on line " << lineNumber << ".\n";
            return false;
        }

        parsedCourses.push_back(course);
    }

    if (parsedCourses.empty()) {
        cout << "Error: The selected file contains no course records.\n";
        return false;
    }

    // Validate that every prerequisite exists as a course in the same file.
    for (const Course& course : parsedCourses) {
        for (const string& prerequisite : course.prerequisites) {
            const bool exists = any_of(
                parsedCourses.begin(), parsedCourses.end(),
                [&](const Course& candidate) {
                    return candidate.courseNumber == prerequisite;
                });

            if (!exists) {
                cout << "Error: Prerequisite '" << prerequisite << "' for "
                     << course.courseNumber << " does not exist in the file.\n";
                return false;
            }
        }
    }

    tree.clear();
    for (const Course& course : parsedCourses) {
        tree.insert(course);
    }

    cout << parsedCourses.size() << " courses loaded successfully.\n";
    return true;
}

void printMenu() {
    cout << "\nABCU Advising Assistance Program\n"
         << "1. Load Data Structure\n"
         << "2. Print Course List\n"
         << "3. Print Course\n"
         << "9. Exit\n"
         << "What would you like to do? ";
}

void printCourseInformation(const BinarySearchTree& tree,
                            const string& requestedCourse) {
    const Course* course = tree.search(requestedCourse);
    if (course == nullptr) {
        cout << "Course '" << toUpper(trim(requestedCourse))
             << "' was not found.\n";
        return;
    }

    cout << course->courseNumber << ", " << course->title << '\n';
    cout << "Prerequisites: ";

    if (course->prerequisites.empty()) {
        cout << "None\n";
        return;
    }

    for (size_t index = 0; index < course->prerequisites.size(); ++index) {
        const string& prerequisiteNumber = course->prerequisites[index];
        const Course* prerequisite = tree.search(prerequisiteNumber);

        if (index > 0) {
            cout << "; ";
        }

        cout << prerequisiteNumber;
        if (prerequisite != nullptr) {
            cout << " (" << prerequisite->title << ")";
        }
    }
    cout << '\n';
}

int main() {
    BinarySearchTree courses;
    bool dataLoaded = false;
    string input;

    cout << "Welcome to the ABCU course planner.\n";

    while (true) {
        printMenu();
        getline(cin, input);
        input = trim(input);

        if (input == "1") {
            cout << "Enter the course data file name: ";
            string fileName;
            getline(cin, fileName);
            fileName = trim(fileName);

            if (fileName.empty()) {
                cout << "Error: A file name is required.\n";
                dataLoaded = false;
                continue;
            }

            dataLoaded = loadCoursesFromFile(fileName, courses);
        } else if (input == "2") {
            if (!dataLoaded || courses.empty()) {
                cout << "Please load a valid course data file first.\n";
                continue;
            }

            cout << "\nHere is a sample schedule:\n";
            courses.printAll();
        } else if (input == "3") {
            if (!dataLoaded || courses.empty()) {
                cout << "Please load a valid course data file first.\n";
                continue;
            }

            cout << "What course do you want to know about? ";
            string courseNumber;
            getline(cin, courseNumber);
            courseNumber = trim(courseNumber);

            if (courseNumber.empty()) {
                cout << "Error: A course number is required.\n";
                continue;
            }

            printCourseInformation(courses, courseNumber);
        } else if (input == "9") {
            cout << "Thank you for using the ABCU course planner!\n";
            break;
        } else {
            cout << input << " is not a valid option. Please enter 1, 2, 3, or 9.\n";
        }
    }

    return 0;
}

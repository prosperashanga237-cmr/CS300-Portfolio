# CS 300: Data Structures and Algorithms

This repository contains portfolio artifacts from CS 300: Data Structures and Algorithms at Southern New Hampshire University. The projects demonstrate my understanding of algorithm analysis, data structures, and C++ programming.

## Project Overview

The main problem I worked on in this course involved developing an advising assistance program for the Computer Science department at ABCU. The program needed to load course information from a file, organize that information using an appropriate data structure, display all courses in alphanumeric order, and allow an academic advisor to search for an individual course and view its title and prerequisites.

## What was the problem you were solving in the projects for this course?

The problem was determining an effective way to organize and retrieve course information for academic advisors. In Project One, I evaluated vectors, hash tables, and binary search trees by comparing their runtime and memory characteristics. In Project Two, I applied what I learned by developing a working C++ program that loads course information, organizes it in a data structure, displays the complete course list in alphanumeric order, and allows users to search for individual courses and their prerequisites.

## How did you approach the problem? Consider why data structures are important to understand.

I approached the problem by first identifying the operations the advising program needed to perform and then comparing the available data structures. Understanding data structures is important because the way information is organized affects how efficiently a program can search, sort, insert, and retrieve information. I analyzed the Big O runtime characteristics and advantages and disadvantages of vectors, hash tables, and binary search trees. I ultimately selected a binary search tree because it provides efficient searching under typical conditions and an in-order traversal naturally produces the courses in alphanumeric order.

## How did you overcome any roadblocks you encountered while going through the activities or project?

One challenge was making sure that course information was read correctly from the input file while preserving the relationships between courses and their prerequisites. Another challenge was ensuring that the program could display courses in the required alphanumeric order and reliably locate individual courses. I addressed these challenges by breaking the program into smaller functions, validating file and user input, testing different menu options, and using the properties of the binary search tree to organize and retrieve course information. Testing different cases also helped me identify problems and verify that each part of the program behaved as expected.

## How has your work on this project expanded your approach to designing software and developing programs?

This project expanded my approach to software development by showing me that selecting a data structure should be an important part of the design process rather than an afterthought. Before implementing a solution, I now understand the importance of identifying the operations a program will perform and evaluating how different algorithms and data structures affect performance. Big O analysis provided a way to compare possible solutions before implementation. I can apply this approach to future projects by considering efficiency, scalability, and program requirements during the planning and design stages.

## How has your work on this project evolved the way you write programs that are maintainable, readable, and adaptable?

My work in this course reinforced the importance of organizing code into functions with specific responsibilities, using descriptive variable and function names, maintaining consistent formatting, and including useful comments. Separating file processing, searching, traversal, validation, and user interaction makes a program easier to understand, test, and modify. I also learned that selecting an appropriate data structure can improve maintainability because the organization of the program better reflects the operations it needs to perform. Going forward, I will continue designing programs with modularity and readability in mind so that changes can be made without unnecessarily affecting unrelated parts of the program.

## Portfolio Artifacts

### Project One: Pseudocode and Runtime Analysis

Project One contains my analysis of vectors, hash tables, and binary search trees. I evaluated their runtime and memory requirements, advantages and disadvantages, and suitability for the ABCU advising program. Based on the requirements, I recommended a binary search tree as the overall data structure for the application.

### Project Two: Advising Assistance Program

Project Two contains the working C++ code for the ABCU advising assistance program. The program reads course information from a file, stores course objects in a binary search tree, displays the Computer Science curriculum in alphanumeric order, and allows the user to search for an individual course and view its prerequisites.

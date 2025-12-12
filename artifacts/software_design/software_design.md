<div align="center"><a href="https://blackdemily.github.io">BACK TO HOME</a></div>

<img src="../../images/linebreak.png">

<img src="../../images/sde-artifact-head.png">

*Refactored Course Planner Application*

<br>
<img src="../../images/original-artifact-head.png">  

The original artifact was a Course Planner console application created for **CS-300: Data Structures & Algorithms**. The program loaded course data from a CSV file and allowed users to view a sorted list of courses and display course prerequisites. The original implementation was contained in a single source file with tightly coupled logic.  

<br>
<img src="../../images/enhancements-head.png">  

For the capstone, the Course Planner was redesigned using object-oriented and modular software design principles. The application was refactored into multiple classes with clear responsibilities:

- `Course`: Represents the course data model  
- `CourseRepository`: Manages loading and storing course data  
- `CourseService`: Contains business logic and formatting operations  
- `MenuUI`: Handles user interaction and input validation  

Additional enhancements include improved input validation, structured exception handling, and expanded inline documentation to improve maintainability and readability.

<br>
<img src="../../images/skills-tools-demonstrated-head.png">  

This artifact demonstrates the following skills and technologies:

- Object-oriented design and separation of concerns  
- Modular C++ application architecture  
- Use of header and source files  
- Input validation and exception handling  
- Code documentation and maintainability practices

<br>
<img src="../../images/outcome-alignment-head.png">  

This artifact primarily supports the **Software Design & Engineering** program outcome. By refactoring the application into a modular, layered architecture, this enhancement demonstrates the ability to design, implement, and evaluate a computing solution using appropriate software engineering practices and modern development techniques.

<br>
<img src="../../images/supporting-materials-head.png">  

- [View Original Course Planner Source Code (CS-300)](original/EBlack_CS-300_ProjectTwo.cpp)
- [View Enhanced Course Planner Source Code (CS-499)](enhanced/EBlack_Enhanced_CS-300_ProjectTwo.cpp)
- [Read Software Design Enhancement Narrative (Milestone Two)](../../files/milestone_two.pdf)

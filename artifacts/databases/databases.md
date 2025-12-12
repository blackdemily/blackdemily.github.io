<div align="center"><a href="https://blackdemily.github.io">BACK TO HOME</a></div>

<img src="../../images/linebreak.png">

<img src="../../images/db-artifact-head.png">

*Course Planner with Relational Database Integration*

<br>
<img src="../../images/original-artifact-head.png">  

The original artifact was a Course Planner console application developed for **CS-300: Data Structures and Algorithms**. In its initial form, the program loaded course data exclusively from a CSV file and stored the information in memory using basic data structures. While functional, this approach limited scalability, persistence, and flexibility when working with larger or evolving datasets.

<br>
<img src="../../images/enhancements-head.png">  

For the capstone, the Course Planner was enhanced to support persistent data storage using a **MySQL relational database**. A structured `courses` table was introduced with columns for `course_id`, `title`, and `prereqs`. The application was modified to allow users to load course data either from a CSV file or directly from the database at runtime.

The enhancement also includes the use of **MySQL Connector/C++** and **parameterized SQL queries** to safely insert and retrieve records. This design improves data integrity, supports future scalability, and demonstrates best practices for database interaction in C++ applications.

<br>
<img src="../../images/skills-tools-demonstrated-head.png">  

This artifact demonstrates the following skills and technologies:

- Relational database design and schema creation  
- Integration of MySQL with a C++ application  
- Use of parameterized SQL queries  
- Data persistence and retrieval strategies  
- Secure database access considerations  

<br>
<img src="../../images/outcome-alignment-head.png">  

This artifact supports the **Databases** program outcome by demonstrating the ability to design, implement, and integrate a relational database solution within an existing application. The enhancement highlights effective schema design, secure query execution, and thoughtful data access strategies aligned with modern database practices.

<br>
<img src="../../images/supporting-materials-head.png">  

- [View Course Planner with MySQL Integration (Enhanced Code)](../path-to-code-folder/)
- [Read Databases Enhancement Narrative (Milestone Four)](../path-to-narrative/)



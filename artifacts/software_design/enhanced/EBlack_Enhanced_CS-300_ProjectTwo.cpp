/**
 * @file CoursePlanner_Enhanced.cpp
 * @brief Enhanced Course Planner for CS-499 Milestone Two (Software Design & Engineering)
 *
 * Refactors a procedural course planner into a modular design with clear separation of concerns:
 *  - Course: domain model
 *  - CourseRepository: data access and parsing from CSV
 *  - CourseService: business logic (printing lists, details)
 *  - MenuUI: user interaction and input validation
 *
 * Key improvements:
 *  - Object-oriented design and dependency boundaries (SRP, separation of concerns)
 *  - Safer input handling and basic exception management
 *  - Clear inline comments and Doxygen-style documentation for maintainability
 *
 * NOTE (future work): Database integration (parameterized SQL, indexing) will replace CSV in a later milestone.
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <limits>       // for std::numeric_limits
#include <stdexcept>

/**
 * @class Course
 * @brief Domain model for a course with ID, title, and prerequisites.
 *
 * Design decisions:
 *  - Keep members private to enforce encapsulation.
 *  - Provide const-accessors to support read-only use across layers.
 */
class Course {
private:
    std::string id_;                     ///< e.g., "CS101"
    std::string title_;                  ///< e.g., "Introduction to CS"
    std::vector<std::string> prereqs_;   ///< e.g., {"MATH100"}

public:
    Course() = default;

    /**
     * @brief Construct a Course with id and title.
     */
    Course(const std::string& id, const std::string& title)
        : id_(id), title_(title) {}

    /// @return read-only course ID
    const std::string& getId() const { return id_; }

    /// @return read-only title
    const std::string& getTitle() const { return title_; }

    /// @return read-only list of prerequisite IDs
    const std::vector<std::string>& getPrerequisites() const { return prereqs_; }

    /**
     * @brief Add a prerequisite ID if non-empty.
     * Defensive check prevents accidental empty tokens being stored.
     */
    void addPrerequisite(const std::string& prereqId) {
        if (!prereqId.empty()) {
            prereqs_.push_back(prereqId);
        }
    }
};

/**
 * @class CourseRepository
 * @brief Data access layer responsible for loading and providing Course data.
 *
 * Responsibilities:
 *  - Load from CSV (future: from a DB using parameterized queries).
 *  - Provide fast lookups (unordered_map keyed by course ID).
 *  - Provide a stable, sorted view for UI when needed.
 *
 * Why unordered_map?
 *  - O(1) average lookup by ID (good UX for "print one course" queries).
 */
class CourseRepository {
private:
    std::unordered_map<std::string, Course> byId_;  ///< Fast in-memory index
    bool loaded_ = false;                            ///< Guards UI actions that require data

    /**
     * @brief Trim whitespace from both ends of a string.
     * Robust CSV parsing benefits from trimming tokens to avoid surprises.
     */
    static std::string trim(const std::string& s) {
        const auto start = s.find_first_not_of(" \t\r\n");
        const auto end   = s.find_last_not_of(" \t\r\n");
        if (start == std::string::npos || end == std::string::npos) return "";
        return s.substr(start, end - start + 1);
    }

public:
    /// @return whether a dataset has been loaded into memory
    bool isLoaded() const { return loaded_; }

    /**
     * @brief Load courses from a CSV file with the format:
     *        course_id, title, prereq1, prereq2, ...
     *
     * Error handling strategy:
     *  - Throw std::runtime_error on file-open failure (critical).
     *  - Log non-critical issues (e.g., malformed lines) and continue.
     */
    void loadFromCsv(const std::string& fileName) {
        std::ifstream file(fileName);
        if (!file.is_open()) {
            // Critical failure — inform the caller via exception so UI can surface the message.
            throw std::runtime_error("Unable to open file: " + fileName);
        }

        byId_.clear();
        std::string line;
        int lineNumber = 0;

        // Read line by line; this is resilient to very large datasets (streaming).
        while (std::getline(file, line)) {
            ++lineNumber;
            if (line.empty()) continue; // ignore blank lines to be permissive

            std::stringstream ss(line);
            std::string token;
            std::vector<std::string> tokens;

            // Simple CSV split by comma (adequate for this dataset; future work could add quoted fields).
            while (std::getline(ss, token, ',')) {
                tokens.push_back(trim(token));
            }

            // At minimum we expect course_id and title.
            if (tokens.size() < 2) {
                std::cerr << "[WARN] Line " << lineNumber
                          << " missing course_id or title. Skipping.\n";
                continue;
            }

            const std::string courseId    = tokens[0];
            const std::string courseTitle = tokens[1];

            // Construct model and append any prerequisites from remaining tokens.
            Course course(courseId, courseTitle);
            for (std::size_t i = 2; i < tokens.size(); ++i) {
                course.addPrerequisite(tokens[i]);
            }

            // Overwrite behavior: last occurrence wins (keeps parsing simple).
            // Alternative would be to log duplicates or merge intelligently.
            byId_[courseId] = course;
        }

        loaded_ = true;
        std::cout << "[INFO] Loaded " << byId_.size()
                  << " courses from '" << fileName << "'.\n";
    }

    /**
     * @brief Look up a course by ID.
     * @return pointer to Course or nullptr if not found (UI can print a friendly message).
     */
    const Course* getCourseById(const std::string& id) const {
        const auto it = byId_.find(id);
        return (it == byId_.end() ? nullptr : &it->second);
    }

    /**
     * @brief Return a value copy of all courses sorted by course ID.
     * Copy is intentional to avoid exposing internal storage and to simplify sorting.
     */
    std::vector<Course> getAllCoursesSorted() const {
        std::vector<Course> result;
        result.reserve(byId_.size());
        for (const auto& kv : byId_) result.push_back(kv.second);

        std::sort(result.begin(), result.end(),
                  [](const Course& a, const Course& b) {
                      return a.getId() < b.getId();
                  });
        return result;
    }
};

/**
 * @class CourseService
 * @brief Business logic layer: formatting, printing, and future domain logic.
 *
 * Keeps UI dumb and repository focused on data access.
 */
class CourseService {
private:
    const CourseRepository& repo_;

public:
    explicit CourseService(const CourseRepository& repo) : repo_(repo) {}

    /**
     * @brief Print a sorted list of all courses, or guidance if nothing is loaded.
     */
    void printCourseList() const {
        const auto all = repo_.getAllCoursesSorted();
        if (all.empty()) {
            std::cout << "No courses available. Please load data first.\n";
            return;
        }

        std::cout << "\nCourse List\n";
        std::cout << "-----------------------------------------\n";
        for (const auto& c : all) {
            std::cout << c.getId() << ", " << c.getTitle() << "\n";
        }
        std::cout << "-----------------------------------------\n\n";
    }

    /**
     * @brief Print details for a single course by ID (title + prerequisites).
     */
    void printCourseDetails(const std::string& courseId) const {
        const Course* course = repo_.getCourseById(courseId);
        if (!course) {
            std::cout << "Course '" << courseId << "' not found.\n";
            return;
        }

        std::cout << "\nCourse Details\n";
        std::cout << course->getId() << ", " << course->getTitle() << "\n";

        const auto& prereqs = course->getPrerequisites();
        if (prereqs.empty()) {
            std::cout << "Prerequisites: None\n\n";
        } else {
            std::cout << "Prerequisites: ";
            for (std::size_t i = 0; i < prereqs.size(); ++i) {
                std::cout << prereqs[i] << (i + 1 < prereqs.size() ? ", " : "");
            }
            std::cout << "\n\n";
        }
    }
};

/**
 * @class MenuUI
 * @brief Presentation layer for console I/O and safe user input.
 *
 * Design choices:
 *  - All raw I/O lives here (cin, cout) so it can be swapped later for GUI or web.
 *  - Input parsing defends against non-numeric choices.
 */
class MenuUI {
private:
    CourseRepository& repo_;
    CourseService service_;

    static void printMenu() {
        std::cout << "================ Course Planner ================\n";
        std::cout << "  1. Load Data Structure\n";
        std::cout << "  2. Print Course List\n";
        std::cout << "  3. Print Course\n";
        std::cout << "  9. Exit\n";
        std::cout << "================================================\n";
        std::cout << "Enter your choice: ";
    }

    /**
     * @brief Read a safe integer choice from stdin.
     * Consumes bad input and keeps prompting until a number is entered.
     */
    static int readIntChoice() {
        int choice;
        while (true) {
            if (std::cin >> choice) {
                // consume the trailing newline to keep subsequent getline calls clean
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return choice;
            } else {
                // reset the stream and discard the bad token
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input. Please enter a number: ";
            }
        }
    }

public:
    explicit MenuUI(CourseRepository& repo)
        : repo_(repo), service_(repo) {}

    /**
     * @brief Main event loop for the console UI.
     *  - Handles loading CSV
     *  - Lists courses
     *  - Prints one course by ID
     *  - Exits on user request
     */
    void run() {
        bool running = true;
        while (running) {
            printMenu();
            const int choice = readIntChoice();

            switch (choice) {
                case 1: {
                    std::cout << "Enter CSV file name (e.g., courses.csv): ";
                    std::string fileName;
                    std::getline(std::cin, fileName);
                    try {
                        repo_.loadFromCsv(fileName);
                    } catch (const std::exception& ex) {
                        // Centralized error surfacing; repository throws on critical failure
                        std::cerr << "[ERROR] " << ex.what() << "\n";
                    }
                    break;
                }
                case 2:
                    if (!repo_.isLoaded()) {
                        std::cout << "Please load course data first.\n";
                    } else {
                        service_.printCourseList();
                    }
                    break;
                case 3:
                    if (!repo_.isLoaded()) {
                        std::cout << "Please load course data first.\n";
                    } else {
                        std::cout << "Enter course ID: ";
                        std::string id;
                        std::getline(std::cin, id);
                        service_.printCourseDetails(id);
                    }
                    break;
                case 9:
                    std::cout << "Exiting Course Planner. Goodbye.\n";
                    running = false;
                    break;
                default:
                    std::cout << "Invalid choice. Please select 1, 2, 3, or 9.\n";
                    break;
            }
        }
    }
};

int main() {
    try {
        CourseRepository repository;
        MenuUI ui(repository);
        ui.run();
    } catch (const std::exception& ex) {
        // Failsafe to ensure any unexpected exceptions are visible to the user
        std::cerr << "[FATAL] Unexpected error: " << ex.what() << "\n";
        return 1;
    }
    return 0;
}

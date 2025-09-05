#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <cstdlib>
#include <ctime>

using namespace std;

// Basic data structures
struct Student {
    string name;
    int grade;
    vector<string> availableLectures; // e.g., "2023-07-18-1" for July 18, lecture 1
    map<string, int> subjectLectures; // subject -> remaining lectures
    vector<string> preferredInstructors;
    vector<string> siblings; // names of sibling students
};

struct Teacher {
    string name;
    vector<string> subjects; // subjects teacher can teach
    vector<string> availableSubjects;
};

struct Course {
    string name;
    vector<string> teachers; // instructors who can teach
};

struct ScheduleSlot {
    string date;
    int period; // 1-5
    string subject;
    string student1;
    string student2;
    string teacher;
};

// Global containers
vector<Student> students;
vector<Teacher> teachers;
vector<Course> courses;

const int totalDays = 45; // e.g., from July 18 to August 31
const int lecturesPerDay = 5;
const int totalLectures = totalDays * lecturesPerDay;

// Random utility
string getRandomString(const vector<string>& options) {
    if (options.empty()) return "";
    return options[rand() % options.size()];
}

// Load input data from files (simplified)
void loadData(const string& filename) {
    ifstream fin(filename);
    if (!fin) {
        cerr << "Cannot open input file.\n";
        return;
    }
    // Implement parsing logic based on your data format
    // For simplicity, assume a custom format or use dummy data
    // ... (this part needs detailed implementation)
    fin.close();
}

// Generate initial random data for testing
void generateRandomData() {
    srand(time(0));
    // Generate students
    for (int i=0; i<10; ++i) {
        Student s;
        s.name = "Student" + to_string(i+1);
        s.grade = rand() % 3 + 1; // grades 1-3
        // Assign random available lectures
        for (int d=0; d<totalDays; ++d) {
            for (int p=1; p<=lecturesPerDay; ++p) {
                s.availableLectures.push_back("2023-07-" + to_string(18 + d/7) + "-"+ to_string((d%7)*lecturesPerDay + p));
            }
        }
        // Assign subjects
        s.subjectLectures["Math"] = rand() % 3 + 1;
        s.subjectLectures["English"] = rand() % 3 + 1;
        s.preferredInstructors = {"Instructor1", "Instructor2"};
        students.push_back(s);
    }

    // Generate teachers
    Teacher t1{"Instructor1", {"Math", "English"}};
    Teacher t2{"Instructor2", {"Math"}};
    teachers.push_back(t1);
    teachers.push_back(t2);

    // Generate courses
    Course c1{"Math", {"Instructor1", "Instructor2"}};
    Course c2{"English", {"Instructor1"}};
    courses.push_back(c1);
    courses.push_back(c2);
}

// Simplified scheduling algorithm
void generateSchedule() {
    // Create a structure to hold schedule
    // For simplicity, store in a vector
    vector<ScheduleSlot> schedule;

    // Loop over days and periods
    for (int day=0; day<totalDays; ++day) {
        string date = "2023-07-" + to_string(18 + day/7);
        for (int period=1; period<=lecturesPerDay; ++period) {
            // For each teacher, assign students and subjects
            for (auto& teacher : teachers) {
                // Find a subject for this period
                string subject = getRandomString(teacher.subjects);
                // Find students who can attend this lecture
                vector<Student*> availableStudents;
                for (auto& s : students) {
                    // Check if student has remaining lectures in this subject
                    if (s.subjectLectures[subject] > 0) {
                        // Check availability (simplified)
                        // For real implementation, check s.availableLectures for this date and period
                        availableStudents.push_back(&s);
                    }
                }
                // Select up to two students
                Student* s1 = nullptr;
                Student* s2 = nullptr;
                if (!availableStudents.empty()) {
                    s1 = availableStudents[rand() % availableStudents.size()];
                    // Remove s1 from list if needed
                    availableStudents.erase(remove(availableStudents.begin(), availableStudents.end(), s1), availableStudents.end());
                    if (!availableStudents.empty()) {
                        s2 = availableStudents[rand() % availableStudents.size()];
                    }
                }
                // Assign to schedule
                ScheduleSlot slot;
                slot.date = date;
                slot.period = period;
                slot.subject = subject;
                slot.teacher = teacher.name;
                slot.student1 = s1 ? s1->name : "";
                slot.student2 = s2 ? s2->name : "";
                schedule.push_back(slot);

                // Decrease remaining lectures
                if (s1) s1->subjectLectures[subject]--;
                if (s2) s2->subjectLectures[subject]--;
            }
        }
    }

    // Save schedule to output file
    ofstream fout("schedule_output.txt");
    for (auto& slot : schedule) {
        fout << slot.date << " Period " << slot.period << ": "
             << slot.subject << " (Teacher: " << slot.teacher << ")\n"
             << "  Student1: " << slot.student1 << "\n"
             << "  Student2: " << slot.student2 << "\n\n";
    }
    fout.close();
}

int main() {
    // Load real data or generate dummy data
    generateRandomData();

    // Generate schedule
    generateSchedule();

    cout << "Schedule generated and saved to schedule_output.txt\n";
    return 0;
}
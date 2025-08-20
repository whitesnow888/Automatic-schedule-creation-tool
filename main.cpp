// ==========================================================
// Greedy Scheduler for Excel-like Model (arrays only, no STL)
// Jul 18–Aug 31 (45 days), 5 lectures/day, 2 seats/lecture.
// One subject per lecture slot; up to two students per slot.
// ==========================================================
#include <cstdio>
#include <cstring>
#include <cstdlib>

// ---------- Tunables ----------
#define MAX_STUDENTS 300
#define MAX_TEACHERS 100
#define SUBJECTS     31
#define DAYS         43          
#define LECTURES     5
#define SEATS        2
#define MAX_PREF     3

// ---------- Data Model ----------
// Student availability: 1=available,  0=not
static unsigned char studentAvail[MAX_STUDENTS][DAYS][LECTURES];
static unsigned char studentSubjectPerDayCount[MAX_STUDENTS][DAYS][SUBJECTS];

// Teacher availability: 1=available, 0=not
static unsigned char teacherAvail[MAX_TEACHERS][DAYS][LECTURES];

// Teacher can teach subject: 1=yes, 0=no
static unsigned char teacherCanTeach[MAX_TEACHERS][SUBJECTS];

// Student demand (hours) per subject (each lecture = 1 hour)
static int studentDemand[MAX_STUDENTS][SUBJECTS];

// Preferred teachers (up to 3) per student/subject, -1 for empty
static int preferredTeachers[MAX_STUDENTS][SUBJECTS][MAX_PREF];

// Busy guard so a student isn’t double-booked within a slot
static unsigned char studentBusy[MAX_STUDENTS][DAYS][LECTURES];

// Totals to drive priority
static int totalDemandPerStudent[MAX_STUDENTS];
static int preferredCountPerStudent[MAX_STUDENTS]; // across demanded subjects

// Teacher count and student count (set from your Sheet data)
static int N_students = 0;
static int M_teachers = 0; // per spec, also present in Sheet3!B1

// ---------- Schedule Output ----------
// For each teacher/day/lecture: subject taught (-1 = empty)
static int slotSubject[MAX_TEACHERS][DAYS][LECTURES];
// For each teacher/day/lecture/seat: student id (-1 = empty)
static int slotStudent[MAX_TEACHERS][DAYS][LECTURES][SEATS];

// ---------- Helpers ----------
static inline int hasPreferredFor(int stu, int subj) {
    for (int k = 0; k < MAX_PREF; ++k) if (preferredTeachers[stu][subj][k] >= 0) return 1;
    return 0;
}
static inline int prefersTeacher(int stu, int subj, int tch) {
    for (int k = 0; k < MAX_PREF; ++k) if (preferredTeachers[stu][subj][k] == tch) return 1;
    return 0;
}
static inline int remainingTotal(int stu) {
    return totalDemandPerStudent[stu];
}
static inline int remainingAnyDemand() {
    // quick check whether anyone still has demand
    for (int s = 0; s < N_students; ++s) if (totalDemandPerStudent[s] > 0) return 1;
    return 0;
}

// Count how many preferred teachers exist across demanded subjects
static int countPreferredAcrossDemanded(int stu) {
    int cnt = 0;
    for (int subj = 0; subj < SUBJECTS; ++subj) {
        if (studentDemand[stu][subj] > 0) {
            for (int k = 0; k < MAX_PREF; ++k)
                if (preferredTeachers[stu][subj][k] >= 0) { ++cnt; break; }
        }
    }
    return cnt;
}

// Sort students by custom priority (arrays only, simple insertion sort)
static void sortStudentsByPriority(int order[]) {
    for (int i = 1; i < N_students; ++i) {
        int key = order[i];

        int key_prefCount = preferredCountPerStudent[key];            // want more first
        int key_hasPref = (key_prefCount > 0) ? 1 : 0; // want 1 before 0
        int key_total = remainingTotal(key);
        
        int j;
        for(j = i - 1 ;j >= 0 ;j -- ){
            int cur = order[j];
            int cur_hasPref = (preferredCountPerStudent[cur] > 0) ? 1 : 0;
            int cur_total = remainingTotal(cur);
            int cur_prefCount = preferredCountPerStudent[cur];

            // Compare per rules:
            // 1) hasPreferred desc
            if (key_hasPref != cur_hasPref) {
                if (key_hasPref > cur_hasPref) order[j+1] = order[j];
                else break;
            } else {
                // 2) total classes asc
                if (key_total != cur_total) {
                    if (key_total < cur_total) order[j+1] = order[j];
                    else break;
                } else {
                    // 3) preferredCount desc
                    if (key_prefCount != cur_prefCount) {
                        if (key_prefCount > cur_prefCount) order[j+1] = order[j];
                        else break;
                    } else {
                        // 4) stable by id asc
                        if (key < cur) order[j+1] = order[j];
                        else break;
                    }
                }
            }
        }
        order[j+1] = key;
    }
}

// Try to assign up to 2 students for (teacher, day, lec) on subject s
// Pass 1: enforce preference for this teacher
// Pass 2: relax preference (any teacher qualified)
static int fillSlotForSubject(int tch, int day, int lec, int subj, int enforcePreference, int sortedStu[]) {
    int assigned = 0;
    for (int idx = 0; idx < N_students && assigned < SEATS; ++idx) {
        int stu = sortedStu[idx];

        if (studentDemand[stu][subj] <= 0) continue;
        if (!studentAvail[stu][day][lec]) continue;
        if (studentBusy[stu][day][lec]) continue;
		
		if (studentSubjectPerDayCount[stu][day][subj] >= 2) continue;
        if (enforcePreference && !prefersTeacher(stu, subj, tch)) continue;

        // Assign
        slotStudent[tch][day][lec][assigned] = stu;
        studentBusy[stu][day][lec] = 1;
        studentDemand[stu][subj] -= 1;
        totalDemandPerStudent[stu] -= 1;
        studentSubjectPerDayCount[stu][day][subj] += 1;
        ++assigned;
    }
    return assigned;
}

// For a given empty slot, try to choose a subject and fill seats
static void scheduleOneSlot(int tch, int day, int lec, int sortedStu[]) {
    if (!teacherAvail[tch][day][lec]) return;

    // First try subjects where we can serve preferred students
    int bestAssigned = 0, bestSubj = -1, bestPass = 1;

    // Try both passes (1=enforce pref, 0=relaxed)
        for (int pass = 1; pass >= 0; --pass) {
        for (int subj = 0; subj < SUBJECTS; ++subj) {
            if (!teacherCanTeach[tch][subj]) continue;

            // Dry-run: count potential assigns without mutating state
            int assigned = 0;
            int seatsLeft = SEATS;
			
            for (int idx = 0; idx < N_students && seatsLeft > 0; ++idx) {
                int stu = sortedStu[idx];
                if (studentDemand[stu][subj] <= 0) continue;
                if (!studentAvail[stu][day][lec]) continue;
                if (studentBusy[stu][day][lec]) continue;
                if (pass == 1 && !prefersTeacher(stu, subj, tch)) continue;
                ++assigned;
                --seatsLeft;
            }

            if (assigned > bestAssigned) {
                bestAssigned = assigned;
                bestSubj = subj;
                bestPass = pass;
                if (bestAssigned == SEATS) break; // cannot do better
            }
        }
        if (bestAssigned == SEATS) break;
    }

    if (bestAssigned == 0 || bestSubj < 0) return; // keep slot empty

    // Commit chosen subject and real assignments
    slotSubject[tch][day][lec] = bestSubj;
    // First, assign with chosen pass rule
    int got = fillSlotForSubject(tch, day, lec, bestSubj, bestPass, sortedStu);
    // If only 1 was placed with preference-enforced, try to fill 2nd seat relaxing preference
    if (got < SEATS) {
        fillSlotForSubject(tch, day, lec, bestSubj, 0, sortedStu);
    }
}

// ---------- Main scheduling driver ----------
void runSchedule() {
    // Initialize outputs
    for (int t=0;t<MAX_TEACHERS;++t)
        for (int d=0;d<DAYS;++d)
            for (int l=0;l<LECTURES;++l) {
                slotSubject[t][d][l] = -1;
                slotStudent[t][d][l][0] = -1;
                slotStudent[t][d][l][1] = -1;
                for (int s=0;s<MAX_STUDENTS;++s) {
                	studentBusy[s][d][l] = 0;
                	for (int subj=0; subj<SUBJECTS; subj++)
                        studentSubjectPerDayCount[s][d][subj] = 0; // reset counter
				}
					
            }

    // Precompute totals & preferred counts
    for (int s=0; s<N_students; ++s) {
        int tot = 0;
        for (int subj=0; subj<SUBJECTS; ++subj) tot += studentDemand[s][subj];
        totalDemandPerStudent[s] = tot;
        preferredCountPerStudent[s] = countPreferredAcrossDemanded(s);
    }

    // Build priority order
    int order[MAX_STUDENTS];
    for (int i=0;i<N_students;++i) order[i] = i;
    sortStudentsByPriority(order);

    // Sweep calendar
    for (int day=0; day<DAYS && remainingAnyDemand(); ++day) {
        for (int lec=0; lec<LECTURES && remainingAnyDemand(); ++lec) {
            for (int tch=0; tch<M_teachers && remainingAnyDemand(); ++tch) {
                scheduleOneSlot(tch, day, lec, order);
            }
        }
    }
}

// ---------- Example placeholders for loading & dumping ----------
// Replace these with your own Excel/CSV loaders.
static void loadExampleTiny() {
    // Example problem with tiny sizes (overwrite N_students/M_teachers)
    N_students = 5;
    M_teachers = 3;

    // Everyone available by default
    memset(studentAvail, 1, sizeof(studentAvail));
    memset(teacherAvail, 1, sizeof(teacherAvail));

    // Teacher skills
    memset(teacherCanTeach, 0, sizeof(teacherCanTeach));
    teacherCanTeach[0][0] = 1; // T0 can teach subject 0
    teacherCanTeach[0][2] = 1; // and 1
    teacherCanTeach[1][1] = 1; // T1 can teach subject 1
    teacherCanTeach[1][2] = 1; // and 2
    teacherCanTeach[2][0] = 1; // T2 can teach subject 1    

    // Demands
    memset(studentDemand, 0, sizeof(studentDemand));
    studentDemand[0][0] = 3;
	studentDemand[0][1] = 4;
	studentDemand[1][1] = 7;
	studentDemand[2][0] = 9;
	studentDemand[2][2] = 2;
	studentDemand[3][1] = 4; // S0 wants subj1 x3
    studentDemand[4][0] = 3; // S1 wants subj1 x2
    studentDemand[4][2] = 5; // S2 wants subj2 x2

    // Preferences: -1 means none
    for (int s=0;s<MAX_STUDENTS;++s)
        for (int sub=0; sub<SUBJECTS; ++sub)
            for (int k=0;k<MAX_PREF;++k) preferredTeachers[s][sub][k] = -1;

    preferredTeachers[0][0][0] = 0;
	preferredTeachers[0][0][1] = 2;
	preferredTeachers[0][1][0] = 1; // S0 prefers T0 for subj1
    preferredTeachers[1][1][0] = 1; // S1 prefers T0 for subj1
    preferredTeachers[4][0][0] = 2;
	preferredTeachers[4][2][0] = 1; // S2 prefers T1 for subj2
	
	teacherAvail[0][0][0] = 0;
	teacherAvail[0][0][1] = 0;
	teacherAvail[0][0][2] = 0;
	teacherAvail[1][1][0] = 0;
	teacherAvail[1][1][1] = 0;
	
}

// Dump a small part of the schedule (first 2 days) for inspection
static void dumpScheduleSample() {
    for (int tch=0; tch<M_teachers; ++tch) {
        printf("=== Teacher %d ===\n", tch);
        for (int day=0; day<DAYS; ++day) {
            for (int lec=0; lec<LECTURES; ++lec) {
                int subj = slotSubject[tch][day][lec];
                //if (subj < 0) continue;
                int s0 = slotStudent[tch][day][lec][0];
                int s1 = slotStudent[tch][day][lec][1];
                printf("Day %d, Lec %d: Subj %d | S0=%d S1=%d\n", day+1, lec+1, subj+1, (s0==-1?s0:s0+1), (s1==-1?-1:s1+1));
            }
            printf("---------------\n");
        }
        printf("=================\n");
    }
}

int main() {
    // TODO: Replace with your Excel-driven loaders
    loadExampleTiny();

    runSchedule();

    // TODO: Replace with writer into Sheet1’s layout
    dumpScheduleSample();

    return 0;
}
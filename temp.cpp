#include <bits/stdc++.h>
using namespace std;

#define N_STUDENTS 15
#define M_TEACHERS 4
#define NUM_SUBJECTS 8
#define DAYS 10       // July 18–27
#define SLOTS 5       // 5 lectures per day
#define MAX_PREF 3
#define MAX_NAME 32
#define MAX_SIB_GROUP 5

struct Student {
    int id;
    int grade;
    char name[MAX_NAME];
    int desired[NUM_SUBJECTS];         // how many lectures wanted
    int prefs[NUM_SUBJECTS][MAX_PREF]; // teacher preferences (-1 = none)
    bool avail[DAYS][SLOTS];
    int siblingGroup;                  // group ID, -1 if none
};

struct Teacher {
    int id;
    char name[MAX_NAME];
    bool canTeach[NUM_SUBJECTS];
    bool avail[DAYS][SLOTS];
};

struct LectureCell {
    bool assigned;
    int subj;
    int stu1, stu2;
};

Student students[N_STUDENTS];
Teacher teachers[M_TEACHERS];
LectureCell schedule[M_TEACHERS][DAYS][SLOTS];

int rnd(int lo, int hi){ return lo + rand() % (hi-lo+1); }
bool rndP(double p){ return (rand()/(RAND_MAX+1.0)) < p; }

void makeName(char *buf,const char* prefix,int id){
    sprintf(buf,"%s%d",prefix,id+1);
}

// generate random input
void generateInput(){
    srand(42);
    // teachers
    for(int t=0;t<M_TEACHERS;t++){
        teachers[t].id=t;
        makeName(teachers[t].name,"T",t);
        for(int s=0;s<NUM_SUBJECTS;s++) teachers[t].canTeach[s]=rndP(0.5);
        for(int d=0;d<DAYS;d++)for(int sl=0;sl<SLOTS;sl++) teachers[t].avail[d][sl]=rndP(0.9);
    }
    // sibling groups
    int groupCounter=0;
    for(int i=0;i<N_STUDENTS;i++){
        students[i].id=i;
        students[i].grade=rnd(1,3);
        makeName(students[i].name,"S",i);
        for(int s=0;s<NUM_SUBJECTS;s++){
            students[i].desired[s]=0;
            for(int p=0;p<MAX_PREF;p++) students[i].prefs[s][p]=-1;
        }
        int subjCount=rnd(1,3);
        for(int k=0;k<subjCount;k++){
            int subj=rnd(0,NUM_SUBJECTS-1);
            students[i].desired[subj]=rnd(2,5);
            if(rndP(0.5)){
                int cnt=0;
                for(int t=0;t<M_TEACHERS && cnt<MAX_PREF;t++){
                    if(teachers[t].canTeach[subj] && rndP(0.5)){
                        students[i].prefs[subj][cnt++]=t;
                    }
                }
            }
        }
        for(int d=0;d<DAYS;d++)for(int sl=0;sl<SLOTS;sl++) students[i].avail[d][sl]=rndP(0.8);
        students[i].siblingGroup = (rndP(0.2)? groupCounter++ : -1);
    }
}

bool teacherAllowed(const Student &st,int subj,int tid){
    bool hasPref=false;
    for(int p=0;p<MAX_PREF;p++){
        if(st.prefs[subj][p]==-1) continue;
        hasPref=true;
        if(st.prefs[subj][p]==tid) return true;
    }
    return !hasPref;
}

void buildSchedule(){
    memset(schedule,0,sizeof(schedule));
    for(int t=0;t<M_TEACHERS;t++)
        for(int d=0;d<DAYS;d++)
            for(int s=0;s<SLOTS;s++)
                schedule[t][d][s]={false,-1,-1,-1};

    // greedy assign
    for(int d=0;d<DAYS;d++){
        for(int sl=0;sl<SLOTS;sl++){
            for(int t=0;t<M_TEACHERS;t++){
                if(!teachers[t].avail[d][sl]) continue;
                LectureCell &cell=schedule[t][d][sl];

                int bestStu=-1,bestSubj=-1,bestScore=1e9;
                for(int i=0;i<N_STUDENTS;i++){
                    if(!students[i].avail[d][sl]) continue;
                    for(int subj=0;subj<NUM_SUBJECTS;subj++){
                        if(students[i].desired[subj]<=0) continue;
                        if(!teachers[t].canTeach[subj]) continue;
                        if(!teacherAllowed(students[i],subj,t)) continue;
                        int tot=0,cnt=0;
                        for(int s2=0;s2<NUM_SUBJECTS;s2++){ 
                            tot+=students[i].desired[s2]; 
                            if(students[i].desired[s2]>0) cnt++;
                        }
                        int score=tot*10+cnt;
                        if(score<bestScore){
                            bestScore=score; bestStu=i; bestSubj=subj;
                        }
                    }
                }
                if(bestStu==-1) continue;
                cell.assigned=true;
                cell.subj=bestSubj;
                cell.stu1=bestStu;
                students[bestStu].desired[bestSubj]--;

                // try to add sibling first
                int sibId=-1;
                int group=students[bestStu].siblingGroup;
                if(group!=-1){
                    for(int j=0;j<N_STUDENTS;j++){
                        if(j==bestStu) continue;
                        if(students[j].siblingGroup!=group) continue;
                        if(students[j].desired[bestSubj]>0 && students[j].avail[d][sl]){
                            sibId=j; break;
                        }
                    }
                }
                if(sibId!=-1){
                    cell.stu2=sibId;
                    students[sibId].desired[bestSubj]--;
                } else {
                    // otherwise, any student same subject
                    for(int j=0;j<N_STUDENTS;j++){
                        if(j==bestStu) continue;
                        if(students[j].desired[bestSubj]<=0) continue;
                        if(!students[j].avail[d][sl]) continue;
                        if(!teacherAllowed(students[j],bestSubj,t)) continue;
                        cell.stu2=j;
                        students[j].desired[bestSubj]--;
                        break;
                    }
                }
            }
        }
    }
}

void writeIN(){
    FILE *f=fopen("IN.TXT","w");
    fprintf(f,"Teachers=%d Students=%d Subjects=%d\n",M_TEACHERS,N_STUDENTS,NUM_SUBJECTS);
    for(int t=0;t<M_TEACHERS;t++){
        fprintf(f,"Teacher %d %s\n",t,teachers[t].name);
    }
    for(int i=0;i<N_STUDENTS;i++){
        fprintf(f,"Student %d %s Grade=%d SibGroup=%d\n",i,students[i].name,students[i].grade,students[i].siblingGroup);
    }
    fclose(f);
}

void writeOUT(){
    FILE *f=fopen("OUT.TXT","w");
    for(int t=0;t<M_TEACHERS;t++){
        fprintf(f,"Teacher %s (Column %c)\n",teachers[t].name,'D'+t);
        int startRow=2;
        for(int d=0;d<DAYS;d++){
            fprintf(f," Day %d Rows %d-%d\n",d+1,startRow,startRow+10);
            fprintf(f,"  Row %d: Instructor %s\n",startRow,teachers[t].name);
            int r=startRow+1;
            for(int s=0;s<SLOTS;s++){
                LectureCell &c=schedule[t][d][s];
                if(c.assigned){
                    fprintf(f,"  Row %d: Subj%d | %s\n",r++,c.subj+1,students[c.stu1].name);
                    fprintf(f,"  Row %d: Subj%d | %s\n",r++,c.subj+1,(c.stu2!=-1?students[c.stu2].name:"-"));
                } else {
                    fprintf(f,"  Row %d: -\n",r++);
                    fprintf(f,"  Row %d: -\n",r++);
                }
            }
            startRow+=11;
        }
        fprintf(f,"\n");
    }
    fclose(f);
}

int main(){
    generateInput();
    writeIN();
    buildSchedule();
    writeOUT();
    printf("Done. Files IN.TXT and OUT.TXT created.\n");
    return 0;
}

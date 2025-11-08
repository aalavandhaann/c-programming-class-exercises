#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned int MAX_CHARACTERS_IN_LINE = 1024;
char DATABASE_FILE_NAME[] = "studentrecords.db";

struct Student{
    char name[20];
    int sapid;
    int semester;
};
enum MenuChoice {
    CreateStudent = 1, 
    FindBySAPID, 
    FindByName, 
    RefreshDatabase,
    PrintAllStudents,
    Quit
};

typedef struct Student Stu;

unsigned int getTotalRecords(char filename[]){
    int linesCount = 0;
    char c;
    FILE *fp = fopen(filename, "r");
    if(fp == NULL){
        return 0;
    }
    for (c = getc(fp); c != EOF; c = getc(fp)){
        if(c == '\n'){
            linesCount += 1;
        }
    }
    fclose(fp);
    return linesCount;
};

Stu* readAndLoadFromDatabase(char databaseFile[]) {
    unsigned int totalRecordsAvailable = getTotalRecords(databaseFile);
    unsigned int rowIndex = 0;
    unsigned int columnIndex;

    char line[MAX_CHARACTERS_IN_LINE];
    char* token;   

    Stu* students = malloc(totalRecordsAvailable * sizeof(Stu));
    FILE * fp = fopen(databaseFile, "r");

    if(fp == NULL){
        fp = fopen(databaseFile, "w");
        fclose(fp);
        return students;
    }

    while(fgets(line, sizeof(line), fp) != NULL){
        Stu temporaryStudentVariable;
        columnIndex = 0;
        token = strtok(line, ",");

        while(token != NULL){
            printf("Value: %s, %i\n", token, columnIndex);
            if(columnIndex == 0){
                strcpy(temporaryStudentVariable.name, token);
            }
            else if(columnIndex == 1){
                temporaryStudentVariable.sapid = atoi(token);
            }
            else if(columnIndex == 2){
                temporaryStudentVariable.semester = (int) token;
            }
            token = strtok(NULL, ",");
            columnIndex++;
        }
        students[rowIndex] = temporaryStudentVariable;
        rowIndex++;
    }

    fclose(fp);
    return students;
};

void writeNewStudent(char databaseFile[]){
    Stu student;
    FILE *fp = fopen(databaseFile, "a");
    char newLineContent[MAX_CHARACTERS_IN_LINE]; 
    printf("\nEnter name of the student: ");
    scanf("%s", student.name);
    printf("\nEnter sapid of the student: ");
    scanf("%i", &student.sapid);
    printf("\nEnter semester of the student: ");
    scanf("%i", &student.semester);
    
    sprintf(newLineContent, "%s,%i,%i\n", student.name, student.sapid, student.semester);
    fprintf(fp, newLineContent);
    fclose(fp);
}

void printStudent(Stu student){
    printf("\n\n");
    printf("\n\t\tSTUDENT DETAILS FOR %s", student.name);
    printf("\n\t\tSAP ID OF %s is %i", student.name, student.sapid);
    printf("\n\t\tSEMESTER OF %s is %i", student.name, student.semester);
    printf("\n\n");
}

void printAllStudentRecords(char databaseFile[]){
    unsigned int totalStudentRecords = getTotalRecords(databaseFile);
    Stu* students = readAndLoadFromDatabase(databaseFile);

    for(int i = 0; i < totalStudentRecords; i++){
        printStudent(students[i]);
    }
}

int main()
{
    enum MenuChoice menuChoice = CreateStudent;
    do{
        printf("\n##############################");
        printf("\n\t\tSTUDENT RECORDS SOFTWARE");
        printf("\n##############################");
        printf("\nWhat do you want to do? ");
        printf("\n1 - Create a New Student Record");
        printf("\n2 - Find a Student Record by sapid");
        printf("\n3 - Find a Student Record by partial name search");
        printf("\n4 - Reload Database");
        printf("\n5 - Print All Students");
        printf("\n6 - Quit Program");
        printf("\nEnter your choice:  ");
        scanf("%d", &menuChoice);

        switch (menuChoice)
        {
            case CreateStudent:                
                writeNewStudent(DATABASE_FILE_NAME);
                break;
            case RefreshDatabase:
                readAndLoadFromDatabase(DATABASE_FILE_NAME);
                break;
            case PrintAllStudents:
                printAllStudentRecords(DATABASE_FILE_NAME);
                break;
            case FindBySAPID:
            case FindByName:
            case Quit:           
            default:
                break;
        }
    }
    while(menuChoice != Quit);
    
    return 0;
}



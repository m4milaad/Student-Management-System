#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>

#define MAX_NAME_LEN 100
#define MAX_DEPT_LEN 50
#define NUM_SEMESTERS 12
#define MAX_PASS_LEN 25

#define DB_FILE "student_db.dat"
#define PASS_FILE "secure.dat"

typedef struct Student {
    char name[MAX_NAME_LEN];
    char dept[MAX_DEPT_LEN];
    int roll;
    float sgpa[NUM_SEMESTERS];
    float cgpa;
} Student;

void add_student(FILE *fp);
void modify_student(FILE *fp);
void display_all(FILE *fp);
void display_individual(FILE *fp);
FILE *delete_student(FILE *fp);
void change_password();
int handle_login();

void print_char(char ch, int n);
void print_title();
void clear_screen();
void clear_input_buffer();
void simple_xor(char *str, size_t len);

int main() {
    FILE *fp;

    fp = fopen(DB_FILE, "rb+");
    if (fp == NULL) {
        fp = fopen(DB_FILE, "wb+");
        if (fp == NULL) {
            perror("Error: Cannot create or open database file");
            return 1;
        }
    }

    if (!handle_login()) {
        printf("\nLogin failed. Exiting.\n");
        fclose(fp);
        return 1;
    }

    int option;
    while (1) {
        print_title();
        printf("\n\n\t\t\t\t1. Add Student");
        printf("\n\n\t\t\t\t2. Modify Student");
        printf("\n\n\t\t\t\t3. Show All Students");
        printf("\n\n\t\t\t\t4. Individual View");
        printf("\n\n\t\t\t\t5. Remove Student");
        printf("\n\n\t\t\t\t6. Change Password");
        printf("\n\n\t\t\t\t7. Exit\n\t");
        print_char('*', 64);
        printf("\n\n\t\t\t\tEnter Your Option :--> ");

        if (scanf("%d", &option) != 1) {
            printf("\n\t\tInvalid input. Please enter a number.\n");
            clear_input_buffer();
            printf("\n\t\tPress any key to continue...");
            getch();
            continue;
        }
        clear_input_buffer();

        switch (option) {
            case 1: add_student(fp); break;
            case 2: modify_student(fp); break;
            case 3: display_all(fp); break;
            case 4: display_individual(fp); break;
            case 5:
                fp = delete_student(fp);
                break;
            case 6: change_password(); break;
            case 7:
                fclose(fp);
                printf("\n\n\t\t\tThank you for using the system!\n");
                return 0;
            default:
                printf("\n\t\tInvalid option. Please try again.");
                printf("\n\t\tPress any key to continue...");
                getch();
        }
    }

    return 0;
}

int handle_login() {
    clear_screen();
    char password_input[MAX_PASS_LEN] = {0};
    char stored_password[MAX_PASS_LEN] = {0};

    FILE *pass_fp = fopen(PASS_FILE, "r");
    if (pass_fp == NULL) {
        printf("\n\t--- First Time Setup ---\n");
        printf("\tNo password found. Please create one.\n");
        change_password();
        printf("\n\tPassword created successfully. Please login.\n");
        return handle_login();
    } else {
        fgets(stored_password, sizeof(stored_password), pass_fp);
        fclose(pass_fp);
        simple_xor(stored_password, strlen(stored_password));
    }
    
    printf("\n\n\t\t+----------------------------------------+\n");
    printf("\t\t|          STUDENT LOGIN SYSTEM          |\n");
    printf("\t\t+----------------------------------------+\n");
    printf("\n\t\tEnter Password: ");

    int i = 0;
    char ch;
    while (i < MAX_PASS_LEN - 1) {
        ch = getch();
        if (ch == '\r' || ch == '\n') {
            break;
        } else if (ch == '\b') {
            if (i > 0) {
                i--;
                printf("\b \b");
            }
        } else {
            password_input[i++] = ch;
            printf("*");
        }
    }
    password_input[i] = '\0';

    if (strcmp(password_input, stored_password) == 0) {
        printf("\n\n\t\tLogin Successful! Welcome.");
        printf("\n\t\tPress any key to continue...");
        getch();
        return 1;
    } else {
        printf("\n\n\t\tWrong Password. Access Denied.");
        getch();
        return 0;
    }
}

void change_password() {
    print_title();
    char new_pass[MAX_PASS_LEN];
    printf("\n\tEnter new password (max %d chars): ", MAX_PASS_LEN - 1);
    
    fgets(new_pass, sizeof(new_pass), stdin);
    new_pass[strcspn(new_pass, "\n")] = 0; 
    
    simple_xor(new_pass, strlen(new_pass));

    FILE *pass_fp = fopen(PASS_FILE, "w");
    if (pass_fp == NULL) {
        perror("Error: Could not save password file");
        return;
    }

    fputs(new_pass, pass_fp);
    fclose(pass_fp);
    printf("\n\tPassword updated successfully.\n");
    printf("\n\tPress any key to continue...");
    getch();
}

void add_student(FILE *fp) {
    print_title();
    Student s;
    char another = 'y';

    fseek(fp, 0, SEEK_END);

    while (another == 'y' || another == 'Y') {
        printf("\n\n\t\tEnter Full Name of Student: ");
        fgets(s.name, MAX_NAME_LEN, stdin);
        s.name[strcspn(s.name, "\n")] = 0;

        printf("\n\t\tEnter Department Name: ");
        fgets(s.dept, MAX_DEPT_LEN, stdin);
        s.dept[strcspn(s.dept, "\n")] = 0;

        printf("\n\t\tEnter Roll Number: ");
        while (scanf("%d", &s.roll) != 1) {
            printf("\t\tInvalid input. Please enter a number for Roll No: ");
            clear_input_buffer();
        }
        clear_input_buffer();
        
        printf("\n\t\tEnter SGPA for %d semesters:\n", NUM_SEMESTERS);
        float total_sgpa = 0;
        for (int i = 0; i < NUM_SEMESTERS; i++) {
            printf("\t\tSemester %d: ", i + 1);
            while (scanf("%f", &s.sgpa[i]) != 1) {
                 printf("\t\tInvalid input. Please enter a number for SGPA: ");
                 clear_input_buffer();
            }
            total_sgpa += s.sgpa[i];
        }
        clear_input_buffer();
        s.cgpa = total_sgpa / NUM_SEMESTERS;

        fwrite(&s, sizeof(Student), 1, fp);

        printf("\n\n\t\tAdd another student (Y/N)? ");
        scanf(" %c", &another);
        clear_input_buffer();
    }
}

void modify_student(FILE *fp) {
    print_title();
    Student s;
    int temp_roll, found = 0;
    long size = sizeof(Student);

    printf("\n\n\tEnter Roll Number of Student to MODIFY: ");
    scanf("%d", &temp_roll);
    clear_input_buffer();

    rewind(fp);

    while (fread(&s, size, 1, fp) == 1) {
        if (s.roll == temp_roll) {
            found = 1;
            fseek(fp, -size, SEEK_CUR);

            printf("\n\t\t--- Record Found ---\n");
            printf("\t\tStudent Name: %s\n", s.name);
            printf("\t\tStudent Roll: %d\n", s.roll);
            printf("\t\t----------------------\n");

            printf("\n\tEnter New Data for the student:\n");
            printf("\n\t\tEnter Full Name: ");
            fgets(s.name, MAX_NAME_LEN, stdin);
            s.name[strcspn(s.name, "\n")] = 0;

            printf("\n\t\tEnter Department: ");
            fgets(s.dept, MAX_DEPT_LEN, stdin);
            s.dept[strcspn(s.dept, "\n")] = 0;

            printf("\n\t\tEnter Roll Number: ");
            scanf("%d", &s.roll);
            clear_input_buffer();

            printf("\n\t\tEnter new SGPA for %d semesters:\n", NUM_SEMESTERS);
            float total_sgpa = 0;
            for (int i = 0; i < NUM_SEMESTERS; i++) {
                 printf("\t\tSemester %d: ", i + 1);
                 scanf("%f", &s.sgpa[i]);
                 total_sgpa += s.sgpa[i];
            }
            s.cgpa = total_sgpa / NUM_SEMESTERS;
            clear_input_buffer();

            fwrite(&s, size, 1, fp);
            printf("\n\n\tRecord updated successfully!");
            break;
        }
    }

    if (!found) {
        printf("\n\n\tERROR: Record not found!");
    }
    printf("\n\n\tPress any key to continue...");
    getch();
}

FILE *delete_student(FILE *fp) {
    print_title();
    Student s;
    int temp_roll, found = 0;
    long size = sizeof(Student);
    FILE *ft;

    ft = fopen("temp.dat", "wb");
    if (ft == NULL) {
        printf("\n\tERROR: Could not create temporary file.\n");
        printf("\n\tPress any key to continue...");
        getch();
        return fp;
    }

    printf("\n\n\tEnter Roll Number of Student to Delete: ");
    scanf("%d", &temp_roll);
    clear_input_buffer();

    rewind(fp);
    while (fread(&s, size, 1, fp) == 1) {
        if (s.roll == temp_roll) {
            found = 1;
            printf("\n\tRecord for %s (Roll: %d) deleted.\n", s.name, s.roll);
        } else {
            fwrite(&s, size, 1, ft);
        }
    }
    
    fclose(fp);
    fclose(ft);

    if (remove(DB_FILE) != 0) {
        perror("Error removing original database file");
    }
    if (rename("temp.dat", DB_FILE) != 0) {
        perror("Error renaming temporary file");
    }

    fp = fopen(DB_FILE, "rb+");
    if (fp == NULL) {
        perror("Error re-opening database file");
        exit(1);
    }

    if (!found) {
        printf("\n\n\tNO STUDENT FOUND with that Roll Number.\n");
    }

    printf("\n\tPress any key to continue...");
    getch();
    return fp;
}

void display_all(FILE *fp) {
    print_title();
    Student s;
    long size = sizeof(Student);
    int count = 0;

    rewind(fp);

    printf("\n\t--- All Student Records ---\n");
    while (fread(&s, size, 1, fp) == 1) {
        count++;
        printf("\n\t\tNAME       : %s", s.name);
        printf("\n\t\tDEPARTMENT : %s", s.dept);
        printf("\n\t\tROLL       : %d", s.roll);
        printf("\n\t\tCGPA       : %.2f\n", s.cgpa);
        print_char('-', 40);
        printf("\n");
    }
    
    if (count == 0) {
        printf("\n\tNo records found in the database.\n");
    }

    printf("\n\n\tPress any key to return to menu...");
    getch();
}

void display_individual(FILE *fp) {
    print_title();
    Student s;
    int temp_roll, found = 0;
    long size = sizeof(Student);

    printf("\n\n\tEnter Roll Number to view details: ");
    scanf("%d", &temp_roll);
    clear_input_buffer();

    rewind(fp);
    while (fread(&s, size, 1, fp) == 1) {
        if (s.roll == temp_roll) {
            found = 1;
            printf("\n\t\t--- Student Details ---\n");
            printf("\t\tNAME         : %s\n", s.name);
            printf("\t\tDEPARTMENT   : %s\n", s.dept);
            printf("\t\tROLL         : %d\n", s.roll);
            printf("\t\tCGPA         : %.2f\n", s.cgpa);
            printf("\t\t-----------------------\n");
            printf("\t\tSemester GPAs:\n");
            for (int i = 0; i < NUM_SEMESTERS; i++) {
                printf("\t\t  Sem %2d: %.2f\n", i + 1, s.sgpa[i]);
            }
            break;
        }
    }
    
    if (!found) {
        printf("\n\n\tERROR: Record not found for Roll Number %d\n", temp_roll);
    }
    
    printf("\n\n\tPress any key to continue...");
    getch();
}

void print_char(char ch, int n) {
    for (int i = 0; i < n; i++) {
        putchar(ch);
    }
}

void print_title() {
    clear_screen();
    printf("\n\n\t");
    print_char('=', 19);
    printf(" Student Management System ");
    print_char('=', 19);
    printf("\n");
}

void clear_screen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void simple_xor(char *str, size_t len) {
    char key = 'K';
    for (size_t i = 0; i < len; i++) {
        str[i] ^= key;
    }
}


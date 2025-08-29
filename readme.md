# 🎓 Student Management System (C)

A simple **console-based Student Management System** written in C.  
It allows you to securely manage student records with password-protected access.

---

## ✨ Features

- 🔐 **Password-protected login system**  
  - First-time setup requires creating a password  
  - Password is stored in a file with basic XOR encryption  

- 📁 **File-based database**  
  - Stores student records in `student_db.dat`  
  - Password stored in `secure.dat`  

- 👨‍🎓 **Student record management**  
  - Add new students with details (Name, Department, Roll No, SGPA, CGPA)  
  - Modify existing student records  
  - View all student records in a list  
  - View individual student details by Roll Number  
  - Delete a student record  

- 🧮 **Automatic CGPA Calculation**  
  - CGPA is calculated as the average of SGPA values across semesters  

- 🖥️ **Simple CLI Menu**  
  - Easy-to-use, menu-driven interface  
  - Cross-platform screen clearing (`cls` for Windows / `clear` for Linux)  

---

## 📂 File Structure

```plaintext
Student-Management-System/
├── student_db.dat   # Binary database file (auto-created)
├── secure.dat       # Stores encrypted password
├── main.c           # Source code
└── README.md        # Project documentation
```
## 🛠️ Compilation & Usage
### 1. Clone the repository
```bash
git clone https://github.com/m4milaad/Student-Management-System.git
cd Student-Management-System
```


### 2. Compile the program
``` bash
gcc main.c -o main
```
### 3. Run the program
``` bash
./main
```
On Windows (MinGW):
``` bash
gcc main.c -o main.exe
main.exe\
```
## 📖 How It Works

On first run, the system will ask you to set a password

You must enter this password for all future logins

After logging in, you can access the main menu
``` plaintext
1. Add Student
2. Modify Student
3. Show All Students
4. Individual View
5. Remove Student
6. Change Password
7. Exit
```
## ⚠️ Limitations

- Uses XOR encryption for password storage (not secure for real-world use)

- Maximum 12 semesters are hard-coded for SGPA input

- Password length limited to 24 characters

- Uses <conio.h> for getch() (Windows-specific). Replace with getchar() for Linux/macOS

## 🚀 Future Improvements

- Replace XOR encryption with secure hashing (SHA-256/BCrypt)

- Replace binary file database with SQLite or JSON

- Add features like sorting, search by name/department, and report export (CSV)

- Improve UI with tabular formatting
## 👨‍💻 Author

**Milad Ajaz Bhat**

## 📜 License

This project is licensed under the **MIT License** – feel free to use and modify it.
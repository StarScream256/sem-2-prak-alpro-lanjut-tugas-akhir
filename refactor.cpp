#include <iostream>
#include <string>
#include <utility> // pair<>
#include <stdlib.h> // system()
#include <tuple> // tie()
#include <algorithm> // transform()
#include <chrono> // ::chrono::system_clock()
#include <ctime> // time_t, localtime()
#include <sstream> // stringstream
#include <iomanip> // put_time()
#include <time.h> // time()
#include <fstream> // fstream, ifstream, ofstream
#include <cctype>   // isdigit
#include <stdexcept> // out_of_range, invalid_argument
#include <regex> // regex

using namespace std;

struct Department {
    int departmentId;
    string departmentName;
};

enum class Role {
    admin = 1,
    employee = 2
};

struct Employee {
    int employeeId;
    int departmentId;
    string name;
    string dateOfBirth;
    string jobTitle;
    string email;
    string address;
    string password;
    double wage;
    Role role;
};

struct WageRecord {
    int recordId;
    int employeeId;
    double amountPaid;
    string paymentDate;
};

enum class AttendanceStatus {
    present = 1,
    absent = 2,
    late = 3
};

struct Attendance {
    int recordId;
    int employeeId;
    string dateTime;
    AttendanceStatus status;
};

const int maxDepartment = 30;
const int maxEmployee = 100;
const int maxWageRecord = 100;
const int maxAttendance = 100;

Department departments[maxDepartment];
Employee employees[maxEmployee];
WageRecord wageRecords[maxWageRecord];
Attendance attendanceRecords[maxAttendance];

Employee loggedInEmployee;

int departmentsCount = 0;
int employeesCount = 0;
int wageRecordsCount = 0;
int attendancesCount = 0;


void enterToContinue() {
    cout << "Press 'Enter' to continue...";
    cin.ignore();
}

void invalidChoice() {
    cout << "Invalid choice. Please try again.\n";
    enterToContinue();
}

string toLowerCase(const string &str) {
    string lowerStr = str;
    transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    return lowerStr;
}

void errorOpenFile(string fileName, string extra = "use") {
    cout << "WARNING : Failed to open '" << fileName <<  "' file to " << extra << endl;
}

Department* getRealArrayDepartment() {
    Department* realArr = new Department[departmentsCount];
    for (int i = 0; i < departmentsCount; i++) {
        realArr[i] = departments[i];
    }
    return realArr;
}

Employee* getRealArrayEmployee() {
    Employee* realArr = new Employee[employeesCount];
    for (int i = 0; i < employeesCount; i++) {
        realArr[i] = employees[i];
    }
    return realArr;
}

WageRecord* getRealArrayWageRecord() {
    WageRecord* realArr = new WageRecord[wageRecordsCount];
    for (int i = 0; i < wageRecordsCount; i++) {
        realArr[i] = wageRecords[i];
    }
    return realArr;
}

Attendance* getRealArrayAttendance() {
    Attendance* realArr = new Attendance[attendancesCount];
    for (int i = 0; i < attendancesCount; i++) {
        realArr[i] = attendanceRecords[i];
    }
    return realArr;
}

int inputInt(string prompt = "Input integer", int min = -1e9, int max = 1e9) {
    int result = -1;
    string input;
    bool valid, conversionSuccess;
    do {
        cout << prompt << " : ";
        cin >> input;

        valid = true;
        conversionSuccess = false;

        if (input.empty()) {
            valid = false;
            cout << "WARNING : Input cannot be empty" << endl;
        } else {
            for (size_t i = 0; i < input.length(); i++) {
                if (i == 0 && (input[i] == '-' || input[i] == '+')) {
                    if (input.length() == 1) {
                        valid = false;
                        cout << "WARNING : Invalid input, just a sign is not a number" << endl;
                        break;
                    }
                    continue;
                }
                if (!isdigit(input[i])) {
                    valid = false;
                    cout << "WARNING : Invalid input, input must be a whole number" << endl;
                    break;
                }
            }
        }

        if (valid) {
            try {
                result = stoi(input);
                if (result < min || result > max) {
                    valid = false;
                    cout << "WARNING : Value must be between " << min << " and " << max << endl;
                } else {
                    conversionSuccess = true;
                }
            } catch (const out_of_range&) {
                cout << "WARNING : Number out of range" << endl;
            } catch (const invalid_argument&) {
                cout << "WARNING : Invalid argument for conversion" << endl;
            }
        }
    } while (!valid || !conversionSuccess);
    return result;
}

enum class StringValidationRule {
    BASIC,
    EMAIL,
    DATE_YYYY_MM_DD
};

string inputString(string prompt = "Input string", bool canEmpty = false, int minLength = 1, int maxLength = 30, StringValidationRule rule = StringValidationRule::BASIC) {
    string input;
    bool valid;

    do {
        cout << prompt << " : ";
        if (!canEmpty) {
            getline(cin >> ws, input);
        } else {
            cin.ignore();
            getline(cin, input);
        }


        valid = true;

        if (canEmpty && input.empty()) {
            return "";
        }

        if (input.empty()) {
            valid = false;
            cout << "WARNING : Input cannot be empty" << endl;
        } else {
            if (rule == StringValidationRule::BASIC) {
                if (input.length() < minLength) {
                    valid = false;
                    cout << "WARNING : Minimum input is " << minLength << " chars long" << endl;
                } else if (input.length() > maxLength) {
                    valid = false;
                    cout << "WARNING : Maximum input is " << maxLength << " chars long" << endl;
                }
            } else if (rule == StringValidationRule::EMAIL) {
                regex pattern(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
                valid = regex_match(input, pattern);
                if (!valid) {
                    cout << "WARNING : Invalid email address" << endl;
                }
            } else if (rule == StringValidationRule::DATE_YYYY_MM_DD) {
                regex pattern(R"(^(\d{4})-(\d{2})-(\d{2})$)");
                smatch matches;
                if (!regex_match(input, matches, pattern)) {
                    valid = false;
                    cout << "WARNING : Invalid date format, must be YYYY-MM-DD" << endl;
                } else {
                    try {
                        int year = stoi(matches[1].str());
                        int month = stoi(matches[2].str());
                        int day = stoi(matches[3].str());
    
                        bool dateLogicValid = true;
    
                        if (month < 1 || month > 12) {
                            dateLogicValid = false;
                            cout << "WARNING : Invalid month" << endl;
                        }
    
                        if (day < 1) {
                            dateLogicValid = false;
                            cout << "WARNING : Invalid day, must be greater than 1" << endl;
                        }
    
                        int daysInMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
                        bool isLeapYear = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
                        if (isLeapYear) {
                            daysInMonth[1] = 29;
                        }
    
                        if (day > daysInMonth[month - 1]) {
                            dateLogicValid = false;
                            cout << "WARNING : Invalid day (" << day << ") for month " << month << ". Max is " << daysInMonth[month - 1] << std::endl;
                        }
    
                        if (!dateLogicValid) {
                            valid = false;
                        }
                    } catch (const out_of_range&) {
                        valid = false;
                        cout << "WARNING : Input too big, try again" << endl;
                    } catch (const invalid_argument&) {
                        valid = false;
                        cout << "WARNING : Invalid argument, try again" << endl;
                    }
                }
            }
        }
    } while (!valid);
    return input;
}

double inputDouble(string prompt = "Input double", double min = -1e9, double max = 1e9) {
    double result = 0.0;
    string input;
    bool valid, conversionSuccess;
    do {
        cout << prompt << " : ";
        cin >> input;

        valid = true;
        conversionSuccess = false;

        if (input.empty()) {
            valid = false;
            cout << "WARNING : Input cannot be empty" << endl;
        } else {
            int dotCount = 0;
            for (size_t i = 0; i < input.length(); i++) {
                if (i == 0 && (input[i] == '-' || input[i] == '+')) {
                    if (input.length() == 1) {
                        valid = false;
                        cout << "WARNING : Invalid input, just a sign is not a number" << endl;
                        break;
                    }
                    continue;
                }
                if (input[i] == '.') {
                    dotCount++;
                    if (dotCount > 1) {
                        valid = false;
                        cout << "WARNING : Invalid input, too many decimal points" << endl;
                        break;
                    }
                } else if (!isdigit(input[i])) {
                    valid = false;
                    cout << "WARNING : Invalid input, input must be a number" << endl;
                    break;
                }
            }
        }

        if (valid) {
            try {
                result = stod(input);
                if (result < min || result > max) {
                    valid = false;
                    cout << "WARNING : Value must be between " << min << " and " << max << endl;
                } else {
                    conversionSuccess = true;
                }
            } catch (const out_of_range&) {
                cout << "WARNING : Number out of range" << endl;
            } catch (const invalid_argument&) {
                cout << "WARNING : Invalid argument for conversion" << endl;
            }
        }
    } while (!valid || !conversionSuccess);
    return result;
}


int getMainChoice() {
    system("cls");
    cout << "Welcome " << loggedInEmployee.name << "! Please choose an option:" << endl;
    if (loggedInEmployee.role == Role::admin) {
        cout << "1. Employee" << endl;
        cout << "2. Attendance" << endl;
        cout << "3. Wage Manager" << endl;
        cout << "4. Department" << endl;
        cout << "0. Exit" << endl;
        return inputInt("Enter your choice", 0, 4);
    } else {
        cout << "1. Attendance\n";
        cout << "2. Wage Record\n";
        cout << "0. Exit\n";
        return inputInt("Enter your choice", 0, 2);
    }
}

int getEmployeeChoice() {
    system("cls");
    cout << "Employee Menu\n";
    cout << "1. Show All Employee\n";
    cout << "2. Add Employee\n";
    cout << "3. Update Employee\n";
    cout << "4. Delete Employee\n";
    cout << "5. Search Employee\n";
    cout << "0. Back to main menu\n";
    return inputInt("Enter your choice", 0, 5);
}

int getSearchEmployeeChoice() {
    system("cls");
    cout << "Search Employee Menu\n";
    cout << "1. Search by ID\n";
    cout << "2. Search by Name\n";
    cout << "0. Back to employee menu\n";
    return inputInt("Enter your choice", 0, 2);
}

int getAttendanceChoice(bool isAdmin = true) {
    system("cls");
    cout << "Attendance Menu\n";
    cout << "1. Make Attendance\n";
    cout << "2. Self Report\n";
    if (isAdmin) {
        cout << "3. Employee Report\n";
        cout << "0. Back to main menu\n";
        return inputInt("Enter your choice", 0, 3);
    }
    cout << "0. Back to main menu\n";
    return inputInt("Enter your choice", 0, 2);
}

int getWageManagerChoice() {
    system("cls");
    cout << "Wage Manager Menu\n";
    cout << "1. Pay Employee Wage This Month\n";
    cout << "2. Adjust Employee Wage\n";
    cout << "3. Show All Wage Records\n";
    cout << "4. Show My Wage Records\n";
    cout << "0. Back to main menu\n";
    return inputInt("Enter your choice", 0, 4);
}

int getDepartmentChoice() {
    system("cls");
    cout << "Department Menu\n";
    cout << "1. Show All Departments\n";
    cout << "2. Add Department\n";
    cout << "3. Update Department\n";
    cout << "4. Delete Department\n";
    cout << "0. Back to main menu\n";
    return inputInt("Enter your choice", 0, 4);
}


const string departmentFilePath = "departments.txt";

void loadDepartment() {
    ifstream file(departmentFilePath);
    if (file.is_open()) {
        departmentsCount = 0;
        string line;
        Department temp;
        while (getline(file, line) && departmentsCount < maxDepartment) {
            stringstream dataInline(line);
            string attr;
            int attrNum = 1;
            while (getline(dataInline, attr, ',')) {
                switch (attrNum) {
                    case 1: temp.departmentId = stoi(attr); break;
                    case 2: temp.departmentName = attr; break;
                }
                attrNum++;
            }
            departments[departmentsCount++] = temp;
        }
        file.close();
    } else {
        errorOpenFile(departmentFilePath, "read");
    }
}

void storeDepartment() {
    ofstream file(departmentFilePath);
    if (file.is_open()) {
        for (size_t i = 0; i < departmentsCount; i++) {
            file << departments[i].departmentId << "," << departments[i].departmentName << endl;
        }
        file.close();
    } else {
        errorOpenFile(departmentFilePath, "write");
    }
}

string searchDepartmentById(int target) {
    for (int i=0; i < departmentsCount; i++) {
        if (departments[i].departmentId == target) {
            return departments[i].departmentName;
        }
    }
    return "WARNING : Target not found";
}

int getMaxDepartmentId() {
    int maxId = 0;
    for (int i = 0; i < departmentsCount; i++) {
        maxId = max(maxId, departments[i].departmentId);
    }
    return maxId;
}

int getLongestDepartment() {
    int longest = 0;
    for (int i = 0; i < departmentsCount; i++) {
        longest = max(longest, static_cast<int>(departments[i].departmentName.length()));
    }
    return longest;
}

void showDepartments() {
    int idLen = 5;
    int nameLen = max(getLongestDepartment() + 3, 18);
    int separatorLen = idLen + nameLen;

    cout
    << setfill('=') << setw(separatorLen + 1) << "" << setfill(' ') << endl
    << setw(5) << left << "| ID "
    << setw(25) << left << "| Department Name "
    << "|" << endl
    << setfill('=') << setw(separatorLen + 1) << "" << setfill(' ') << endl;
    for (int i = 0; i < departmentsCount; i++) {
        cout
        << setw(5) << left << "| " + to_string(departments[i].departmentId)
        << setw(25) << left << "| " + departments[i].departmentName
        << "|" << endl;
    }
    cout << setfill('=') << setw(separatorLen + 1) << "" << setfill(' ') << endl;
}

void updateDepartment() {
    showDepartments();
    int reqId = inputInt("Enter department ID to be updated", 1, getMaxDepartmentId());
    for (int i = 0; i < departmentsCount; i++) {
        if (departments[i].departmentId == reqId) {
            cout << "Current department name : " << departments[i].departmentName << endl;
            string updatedName = inputString("New Name (enter and leave it blank to cancel)", true, 0);
            if (updatedName.length() > 0) {
                departments[i].departmentName = updatedName;
                cout << "INFO : Successfully updated department name" << endl;
                return;
            }
            cout << "INFO : Update canceled" << endl;
            return;
        }
    }
    cout << "WARNING : Department not found" << endl;
}

void addDepartment() {
    cout << "Menambahkan Departemen Baru\n";
    Department newDepartment;
    ofstream file(departmentFilePath, ios::app);
    if (file.is_open()) {
        Department newDepartment;
        int lastIndex = 0;
        for (int i = 0; i < departmentsCount; i++) {
            lastIndex = max(lastIndex, departments[i].departmentId);
        }
        newDepartment.departmentId = lastIndex + 1;
        newDepartment.departmentName = inputString("Enter Department Name", false, 1, 30, StringValidationRule::BASIC);

        if (lastIndex < maxDepartment) {
            file << newDepartment.departmentId << "," << newDepartment.departmentName << endl;
            departments[departmentsCount++] = newDepartment;
            cout << "INFO : Departemen berhasil ditambahkan\n";
        } else {
            cout << "WARNING : Batas maksimum departemen telah tercapai.\n";
        }
        file.close();
    } else {
        errorOpenFile(departmentFilePath, "write");
    }
}

void deleteDepartment() {
    cout << "Menghapus Departemen\n";
    showDepartments();
    int deletedIndex = -1;
    int target = inputInt("Masukkan ID Departemen yang ingin dihapus", 1, getMaxDepartmentId());
    bool found = false;
    for (int i = 0; i < departmentsCount; i++) {
        if (departments[i].departmentId == target) {
            deletedIndex = i;
            found = true;
            break;
        }
    }
    if (found) {
        bool hasEmployee = false;
        for (int i = 0; i < employeesCount; i++) {
            if (employees[i].departmentId == target) {
                hasEmployee = true;
                break;
            }
        }
        if (hasEmployee) {
            cout << "WARNING : Tidak dapat menghapus departemen, ada karyawan yang terdaftar pada departemen ini\n";
            return;
        }
        for (int i = deletedIndex; i < departmentsCount - 1; i++) {
            departments[i] = departments[i + 1];
        }
        departmentsCount--;
        storeDepartment();
        cout << "INFO : Departemen dengan ID " << target << " berhasil dihapus.\n";
    } else {
        cout << "WARNING : Requested ID can't be found" << endl;
    }
}


const string employeeFilePath = "employees.txt";

void loadEmployee() {
    ifstream file(employeeFilePath);
    if (file.is_open()) {
        employeesCount = 0;
        string line;
        Employee temp;
        while (getline(file, line)) {
            stringstream dataInline(line);
            string attr;
            int attrNum = 1;
            while (getline(dataInline, attr, ',')) {
                switch (attrNum) {
                    case 1: temp.employeeId = stoi(attr); break;
                    case 2: temp.departmentId = stoi(attr); break;
                    case 3: temp.name = attr; break;
                    case 4: temp.dateOfBirth = attr; break;
                    case 5: temp.jobTitle = attr; break;
                    case 6: temp.email = attr; break;
                    case 7: temp.address = attr; break;
                    case 8: temp.password = attr; break;
                    case 9: temp.wage = stod(attr); break;
                    case 10: temp.role = (stoi(attr) == 1) ? Role::admin : Role::employee; break;
                }
                attrNum++;
            }
            employees[employeesCount++] = temp;
        }
        file.close();
    } else {
        errorOpenFile(employeeFilePath, "read");
    }
}

void storeEmployee() {
    ofstream file(employeeFilePath);
    if (file.is_open()) {
        for (size_t i = 0; i < employeesCount; i++) {
            file
                << employees[i].employeeId << ","
                << employees[i].departmentId << ","
                << employees[i].name << ","
                << employees[i].dateOfBirth << ","
                << employees[i].jobTitle << ","
                << employees[i].email << ","
                << employees[i].address << ","
                << employees[i].password << ","
                << employees[i].wage << ","
                << (employees[i].role == Role::admin ? '1' : '2') << endl;
        }
        file.close();
    } else {
        errorOpenFile(employeeFilePath, "write");
    }
}

bool loginEmployee() {
    string email, password;
    int attempts = 0;
    const int maxAttempts = 3;
    while (attempts < maxAttempts) {
        system("cls");
        cout << "Employee Login\n";
        cout << "Email\t\t: ";
        getline(cin, email);
        cout << "Password\t: ";
        getline(cin, password);

        bool emailFound = false;
        for (int i = 0; i < employeesCount; i++) {
            if (employees[i].email == email) {
                emailFound = true;
                if (employees[i].password == password) {
                    loggedInEmployee = employees[i];
                    return true;
                } else {
                    cout << "Login failed, incorrect password" << endl;
                    break;
                }
            }
        }

        if (!emailFound) {
            cout << "Login failed, email not found" << endl;
        }
        attempts++;
        cout << "Attempt " << attempts << " of " << maxAttempts << endl;
        if (attempts < maxAttempts) enterToContinue();
    }
    cout << "Too many failed login attempts. Exiting program.\n";
    return false;
}

enum class LongestEmployee {
    NAME,
    DATE_OF_BIRTH,
    JOB_TITLE,
    EMAIL,
    ADDRESS,
    WAGE
};

int getLongestEmployee(LongestEmployee choice) {
    int longestName = 0;
    int longestDateOfBirth = 0;
    int longestJobTitle = 0;
    int longestEmail = 0;
    int longestAddress = 0;
    int longestWage = 0;

    for (int i = 0; i < employeesCount; i++) {
        longestName = max(longestName, static_cast<int>(employees[i].name.length()));
        longestDateOfBirth = max(longestDateOfBirth, static_cast<int>(employees[i].dateOfBirth.length()));
        longestJobTitle = max(longestJobTitle, static_cast<int>(employees[i].jobTitle.length()));
        longestEmail = max(longestEmail, static_cast<int>(employees[i].email.length()));
        longestAddress = max(longestAddress, static_cast<int>(employees[i].address.length()));
        stringstream ss;
        ss << fixed << setprecision(2) << employees[i].wage;
        longestWage = max(longestWage, static_cast<int>(ss.str().length()));
    }

    switch (choice) {
        case LongestEmployee::NAME: return longestName; break;
        case LongestEmployee::DATE_OF_BIRTH: return longestDateOfBirth; break;
        case LongestEmployee::JOB_TITLE: return longestJobTitle; break;
        case LongestEmployee::EMAIL: return longestEmail; break;
        case LongestEmployee::ADDRESS: return longestAddress; break;
        case LongestEmployee::WAGE: return longestWage; break;
        default: return 0;
    }
}

void showEmployee() {
    int idLen = 5;
    int nameLen = max(getLongestEmployee(LongestEmployee::NAME) + 3, 7);
    int emailLen = max(getLongestEmployee(LongestEmployee::EMAIL) + 3, 8);
    int jobTitleLen = max(getLongestEmployee(LongestEmployee::JOB_TITLE) + 3, 12);
    int adminLen = 11;
    int separatorLen = idLen + nameLen + emailLen + jobTitleLen + adminLen + 1;
    
    cout
        << setfill('=') << setw(separatorLen) << "" << setfill(' ') << endl
        << setw(idLen) << left << "| ID "
        << setw(nameLen) << left << "| Name "
        << setw(emailLen) << left << "| Email "
        << setw(jobTitleLen) << left << "| Job Title "
        << setw(adminLen) << left << "| Role "
        << "|" << endl
        << setfill('=') << setw(separatorLen) << "" << setfill(' ') << endl;
    for (int i = 0; i < employeesCount; i++) {
        string role;
        if (employees[i].role == Role::admin) {
            role = "Admin";
        } else {
            role = "Employee";
        }
        cout
            << setw(idLen) << left << "| " + to_string(employees[i].employeeId)
            << setw(nameLen) << left << "| " + employees[i].name
            << setw(emailLen) << left << "| " + employees[i].email
            << setw(jobTitleLen) << left << "| " + employees[i].jobTitle
            << setw(adminLen) << left << "| " + role
            << "|" << endl;

    }
    cout << setfill('=') << setw(separatorLen) << "" << setfill(' ') << endl;
}

int getMaxEmployeeId() {
    int maxId = 0;
    for (int i = 0; i < employeesCount; i++) {
        maxId = max(maxId, employees[i].employeeId);
    }
    return maxId;
}

void addEmployee() {
    ofstream file(employeeFilePath, ios::app);
    if (file.is_open()) {
        Employee newEmployee;
        int roleNum;

        if (employeesCount < maxEmployee) {
            int lastEmployeeId = 0;
            // Cari id karyawan terakhir
            for (int i = 0; i < employeesCount; i++) {
                lastEmployeeId = max(lastEmployeeId, employees[i].employeeId);
            }
            newEmployee.employeeId = lastEmployeeId + 1;
            showDepartments();
            newEmployee.departmentId = inputInt("Masukkan ID Departemen", 1, getMaxDepartmentId());
            newEmployee.name = inputString("Masukkan Nama Karyawan", false, 1, 30, StringValidationRule::BASIC);
            newEmployee.dateOfBirth = inputString("Masukkan Tanggal Lahir (YYYY-MM-DD)", false, 1, 11, StringValidationRule::DATE_YYYY_MM_DD);
            newEmployee.jobTitle = inputString("Masukkan Jabatan", false, 1, 30, StringValidationRule::BASIC);
            newEmployee.email = inputString("Masukkan Email", false, 1, 35, StringValidationRule::EMAIL);
            newEmployee.address = inputString("Masukkan Alamat", false, 1, 40, StringValidationRule::BASIC);
            newEmployee.password = inputString("Masukkan Password", false, 1, 20, StringValidationRule::BASIC);
            newEmployee.wage = inputDouble("Masukkan Gaji", 1);
            roleNum = inputInt("Masukkan Nomor Role (1-Admin, 2-Karyawan): ", 1, 2);
            newEmployee.role = (roleNum == 1) ? Role::admin : Role::employee;

            file
                << newEmployee.employeeId << ","
                << newEmployee.departmentId << ","
                << newEmployee.name << ","
                << newEmployee.dateOfBirth << ","
                << newEmployee.jobTitle << ","
                << newEmployee.email << ","
                << newEmployee.address << ","
                << newEmployee.password << ","
                << newEmployee.wage << ","
                << (newEmployee.role == Role::admin ? '1' : '2') << endl;

            employees[employeesCount++] = newEmployee;
            cout << "INFO : Karyawan berhasil ditambahkan\n";
        } else {
            cout << "WARNING : Batas maksimum karyawan telah tercapai.\n";
        }
        file.close();
    } else {
        errorOpenFile(employeeFilePath, "write");
    }
}

void updateEmployee() {
    cout << "Mengupdate Karyawan\n";
    int roleNum;
    showEmployee();
    int id = inputInt("Masukan ID Karyawan yang ingin diupdate", 1, getMaxEmployeeId());
    for (int i = 0; i < employeesCount; i++) {
        if (employees[i].employeeId == id) {
            cout << "Karyawan ditemukan: " << employees[i].name << endl;
            employees[i].name = inputString("Masukkan Nama Karyawan baru", false, 1, 30, StringValidationRule::BASIC);
            employees[i].dateOfBirth = inputString("Masukkan Tanggal Lahir baru (YYYY-MM-DD)", false, 1, 11, StringValidationRule::DATE_YYYY_MM_DD);
            employees[i].jobTitle = inputString("Masukkan Jabatan baru", false, 1, 30, StringValidationRule::BASIC);
            employees[i].email = inputString("Masukkan Email baru", false, 1, 35, StringValidationRule::EMAIL);
            employees[i].address = inputString("Masukkan Alamat baru", false, 1, 40, StringValidationRule::BASIC);
            employees[i].password = inputString("Masukkan Password baru", false, 1, 20, StringValidationRule::BASIC);
            employees[i].wage = inputDouble("Masukkan Gaji baru", 1);
            roleNum = inputInt("Masukkan Nomor Role baru (1-Admin, 2-Karyawan)", 1, 2);
            employees[i].role = (roleNum == 1) ? Role::admin : Role::employee;
            cout << "INFO : Data karyawan berhasil diperbarui\n";
            storeEmployee();
            return;
        }
    }
    cout << "WARNING : Karyawan dengan ID " << id << " Tidak ditemukan.\n";
}

void quickSortEmployeeById(Employee* e, int left, int right, bool isAscending = true) {
    int i = left, j = right;
    int pivot = e[(left + right) / 2].employeeId;

    while (i <= j) {
        while (isAscending ? e[i].employeeId < pivot : e[i].employeeId > pivot) i++;
        while (isAscending ? e[j].employeeId > pivot : e[j].employeeId < pivot) j--;
        if (i <= j) {
            swap(e[i], e[j]);
            i++;
            j--;
        }
    }

    if (left < j) quickSortEmployeeById(e, left, j, isAscending);
    if (i < right) quickSortEmployeeById(e, i, right, isAscending);
}

void binarySearchEmployee(Employee* e, int idCari) {
    cout << "\nMelakukan Pencarian Biner untuk ID Karyawan: " << idCari << endl;

    // Urutkan array berdasarkan ID karyawan
    quickSortEmployeeById(e, 0, employeesCount - 1, true);

    int kiri = 0;
    int kanan = employeesCount - 1;
    bool found = false;

    while (kiri <= kanan) {
        int tengah = kiri + (kanan - kiri) / 2;
        if (e[tengah].employeeId == idCari) {
            cout << "Karyawan ditemukan:\n";
            cout
                << "ID: " << e[tengah].employeeId
                << ", Nama: " << e[tengah].name
                << ", Jabatan: " << e[tengah].jobTitle << endl;
            found = true;
            break;
        } else if (e[tengah].employeeId < idCari) {
            kiri = tengah + 1;
        } else {
            kanan = tengah - 1;
        }
    }

    if (!found) {
        cout << "INFO : Karyawan dengan ID " << idCari << " tidak ditemukan.\n";
    }
}

void searchEmployeeByName() {
    string keyword = inputString("Enter name", false, 1, 30, StringValidationRule::BASIC);
    int found = 0;
    keyword = toLowerCase(keyword);

    cout << "Result : \n";
    for (int i=0; i < employeesCount; i++) {
        if (toLowerCase(employees[i].name).find(keyword) != string::npos) {
            found++;
            cout << found << ") "
                << employees[i].name
                << ", from " << searchDepartmentById(employees[i].departmentId) << " department\n";
        }
    }
    if (found == 0) cout << "INFO : No employee found\n";
}

string findEmployeeById(int id) {
    for (int i = 0; i < employeesCount; i++) {
        if (employees[i].employeeId == id) return employees[i].name;
    }
    return "-";
}

void adjustEmployeeWage() {
    showEmployee();
    int targetEmployeeId = inputInt("Enter Employee ID", 1, getMaxEmployeeId());
    bool found = false;
    for (int i = 0; i < employeesCount; i++) {
        if (employees[i].employeeId == targetEmployeeId) {
            found = true;
            cout << "Name : " << employees[i].name << endl;
            stringstream ss;
            ss << fixed << setprecision(2) << employees[i].wage;
            cout << "Current Wage " << ss.str() << endl;
            double newWage = inputDouble("Enter new wage", 1);
            employees[i].wage = newWage;
            cout << "INFO : Successfully adjust wage\n";
            storeEmployee();
            break;
        }
    }
    if (!found) cout << "WARNING : No employee found with ID " << to_string(targetEmployeeId) << endl;
}

void deleteEmployee() {
    showEmployee();
    bool found = false;
    int deletedIndex = -1;
    int target = inputInt("Enter employee ID to delete", 1, getMaxEmployeeId());
    for (int i = 0; i < employeesCount; i++) {
        if (employees[i].employeeId == target) {
            deletedIndex = i;
            found = true;
            break;
        }
    }
    if (found) {
        for (int i = deletedIndex; i < employeesCount - 1; i++) {
            employees[i] = employees[i + 1];
        }
        employeesCount--;
        storeEmployee();
        cout << "INFO : Successfully deleted employee" << endl;
    } else {
        cout << "WARNING : Requested ID can't be found" << endl;
    }
}


const string attendanceFilePath = "attendances.txt";

void loadAttendance() {
    ifstream file(attendanceFilePath);
    if (file.is_open()) {
        attendancesCount = 0;
        string line;
        Attendance temp;
        while (getline(file, line) && attendancesCount < maxAttendance) {
            stringstream dataInline(line);
            string attr;
            int attrNum = 1;
            while (getline(dataInline, attr, ',')) {
                switch (attrNum) {
                    case 1: temp.recordId = stoi(attr); break;
                    case 2: temp.employeeId = stoi(attr); break;
                    case 3: temp.dateTime = attr; break;
                    case 4: {
                        int status = stoi(attr);
                        if (status == 1)
                            temp.status = AttendanceStatus::present;
                        else if (status == 2)
                            temp.status = AttendanceStatus::absent;
                        else if (status == 3)
                            temp.status = AttendanceStatus::late;
                        break;
                    }
                }
                attrNum++;
            }
            attendanceRecords[attendancesCount++] = temp;
        }
        file.close();
    } else {
        errorOpenFile(attendanceFilePath, "read");
    }
}

void storeAttendance() {
    ofstream file(attendanceFilePath);
    if (file.is_open()) {
        for (int i = 0; i < attendancesCount; i++) {
            file
            << attendanceRecords[i].recordId << ","
            << attendanceRecords[i].employeeId << ","
            << attendanceRecords[i].dateTime << ","
            << static_cast<int>(attendanceRecords[i].status) << endl;
        }
        file.close();
    } else {
        errorOpenFile(attendanceFilePath, "write");
    }
}

int getMaxAttendanceId() {
    int maxId = 0;
    for (int i = 0; i < attendancesCount; i++) {
        maxId = max(maxId, attendanceRecords[i].recordId);
    }
    return maxId;
}

bool isAttend() {
    auto now = chrono::system_clock::now();
    time_t now_c = chrono::system_clock::to_time_t(now);
    tm local_time;
    localtime_s(&local_time, &now_c);
    stringstream ss_current_date;
    ss_current_date << put_time(&local_time, "%Y-%m-%d");

    Attendance* lastRecord = nullptr;
    for (int i=0; i < attendancesCount; i++) {
        if (attendanceRecords[i].employeeId == loggedInEmployee.employeeId) {
            if (lastRecord == nullptr || attendanceRecords[i].dateTime > lastRecord->dateTime) {
                lastRecord = &attendanceRecords[i];
            }
        }
    }

    if (lastRecord != nullptr) {
        istringstream iss(lastRecord->dateTime);
        tm t{};
        iss >> get_time(&t, "%Y-%m-%d %H:%M:%S"); // parse to get datetime
        if (!iss.fail()) {
            stringstream ss_last_date;
            ss_last_date << put_time(&t, "%Y-%m-%d");
            return ss_last_date.str() == ss_current_date.str();
        }
        return false;
    }
    return false;
}

void recordAttendance(int status = 1) {
    const int
        max_hour = 8,
        max_minutes = 30;
    auto now = chrono::system_clock::now();
    time_t now_c = chrono::system_clock::to_time_t(now);
    tm local_time;
    localtime_s(&local_time, &now_c);
    stringstream ss;
    ss << put_time(&local_time, "%Y-%m-%d %H:%M:%S");
    string currentDateTime = ss.str();
    int currentHour = local_time.tm_hour;
    int currentMinutes = local_time.tm_min;

    if (attendancesCount < maxAttendance) {
        attendanceRecords[attendancesCount].recordId = getMaxAttendanceId() + 1;
        attendanceRecords[attendancesCount].employeeId = loggedInEmployee.employeeId;
        attendanceRecords[attendancesCount].dateTime = currentDateTime;
        if (status == 1) {
            if (currentHour <= max_hour || (currentHour == max_hour && currentMinutes <= max_minutes)) {
                attendanceRecords[attendancesCount].status = AttendanceStatus::present;
            } else {
                attendanceRecords[attendancesCount].status = AttendanceStatus::late;
            }
        } else {
            attendanceRecords[attendancesCount].status = AttendanceStatus::absent;
        }
        attendancesCount++;
        storeAttendance();
        cout << "INFO : Successfully make attendance" << endl;
    } else {
        cout << "WARNING : Maximum attendance data reached" << endl;
    }
}

void showAttendanceRecord(bool isAll = false, bool isSpecific = false, int reqEmployeeId = 0) {
    string status;
    int recordCount = 0;
    int idLen = 5;
    int employeeLen = max(getLongestEmployee(LongestEmployee::NAME) + 8, 16);
    int dateTimeLen = 23;
    int statusLen = 10;
    int separatorLen = idLen + employeeLen + dateTimeLen + statusLen + 1;

    cout
        << setfill('=') << setw(separatorLen) << "" << setfill(' ') << endl
        << setw(idLen) << left << "| ID "
        << setw(employeeLen) << left << "| [ID] Employee "
        << setw(dateTimeLen) << left << "| Datetime "
        << setw(statusLen) << left << "| Status "
        << "|" << endl
        << setfill('=') << setw(separatorLen) << "" << setfill(' ') << endl;
    for (int i=0; i < attendancesCount; i++) {
        switch (attendanceRecords[i].status) {
            case AttendanceStatus::present:
                status = "Present";
                break;
            case AttendanceStatus::late:
                status = "Late";
                break;
            case AttendanceStatus::absent:
                status = "Absent";
                break;
            default:
                status = "Error";
                break;
        }
        if (!isAll && !isSpecific) {
            if (attendanceRecords[i].employeeId == loggedInEmployee.employeeId) {
                recordCount += 1;
                cout
                    << setw(idLen) << left << "| " + to_string(attendanceRecords[i].recordId)
                    << setw(employeeLen) << left << "| [" + to_string(attendanceRecords[i].employeeId) + "] " + findEmployeeById(attendanceRecords[i].employeeId)
                    << setw(dateTimeLen) << left << "| " + attendanceRecords[i].dateTime
                    << setw(statusLen) << left << "| " + status
                    << "|" << endl;
            }
        } else if (isSpecific) {
            if (attendanceRecords[i].employeeId == reqEmployeeId) {
                recordCount += 1;
                cout
                    << setw(idLen) << left << "| " + to_string(attendanceRecords[i].recordId)
                    << setw(employeeLen) << left << "| [" + to_string(attendanceRecords[i].employeeId) + "] " + findEmployeeById(attendanceRecords[i].employeeId)
                    << setw(dateTimeLen) << left << "| " + attendanceRecords[i].dateTime
                    << setw(statusLen) << left << "| " + status
                    << "|" << endl;
            }
        } else {
            recordCount += 1;
            cout
                << setw(idLen) << left << "| " + to_string(attendanceRecords[i].recordId)
                << setw(employeeLen) << left << "| [" + to_string(attendanceRecords[i].employeeId) + "] " + findEmployeeById(attendanceRecords[i].employeeId)
                << setw(dateTimeLen) << left << "| " + attendanceRecords[i].dateTime
                << setw(statusLen) << left << "| " + status
                << "|" << endl;
        }
    }
    if (recordCount == 0) cout << "INFO : No record available\n";
    cout << setfill('=') << setw(separatorLen) << "" << setfill(' ') << endl;
}


const string wageFilePath = "wages.txt";

void loadWage() {
    ifstream file(wageFilePath);
    if (file.is_open()) {
        wageRecordsCount = 0;
        string line;
        WageRecord temp;
        while (getline(file, line) && wageRecordsCount < maxWageRecord) {
            stringstream dataInline(line);
            string attr;
            int attrNum = 1;
            while (getline(dataInline, attr, ',')) {
                switch (attrNum) {
                    case 1: temp.recordId = stoi(attr); break;
                    case 2: temp.employeeId = stoi(attr); break;
                    case 3: temp.amountPaid = stod(attr); break;
                    case 4: temp.paymentDate = attr; break;
                }
                attrNum++;
            }
            wageRecords[wageRecordsCount++] = temp;
        }
        file.close();
    } else {
        errorOpenFile(wageFilePath, "read");
    }
}

void storeWage() {
    ofstream file(wageFilePath);
    if (file.is_open()) {
        for (int i = 0; i < wageRecordsCount; i++) {
            file
                << wageRecords[i].recordId << ","
                << wageRecords[i].employeeId << ","
                << wageRecords[i].amountPaid << ","
                << wageRecords[i].paymentDate << endl;
        }
        file.close();
    } else {
        errorOpenFile(wageFilePath, "write");
    }
}

int getMaxWageId() {
    int maxId = 0;
    for (int i = 0; i < wageRecordsCount; i++) {
        maxId = max(maxId, wageRecords[i].recordId);
    }
    return maxId;
}

void payWage() {
    time_t now = time(0);
    tm* localTime = localtime(&now);
    int day = localTime->tm_mday;
    int month = localTime->tm_mon + 1;
    int year = localTime->tm_year + 1900;
    string strDay = to_string(day).length() == 1 ? "0" + to_string(day) : to_string(day);
    string strMonth = to_string(month).length() == 1 ? "0" + to_string(month) : to_string(month);
    string strYear = to_string(year);
    string lastPaidYear = wageRecordsCount > 0 ? wageRecords[wageRecordsCount - 1].paymentDate.substr(0, 4) : "";
    string lastPaidMonth = wageRecordsCount > 0 ? wageRecords[wageRecordsCount - 1].paymentDate.substr(5, 2) : "";
    string paidDate = strYear + "-" + strMonth + "-" + strDay;
    
    bool proceed = false;
    if (wageRecordsCount == 0) {
        proceed = true;
    } else {
        string lastPaidYearDb = wageRecords[wageRecordsCount - 1].paymentDate.substr(0, 4);
        string lastPaidMonthDb = wageRecords[wageRecordsCount - 1].paymentDate.substr(5, 2);
        if (strYear > lastPaidYearDb) {
            proceed = true;
        } else if (strYear == lastPaidYearDb && strMonth > lastPaidMonthDb) {
            proceed = true;
        }
    }

    if (proceed) {
        if (wageRecordsCount + employeesCount > maxWageRecord) {
            cout << "WARNING: Not enough space to record all wage payments" << endl;
            return;
        }
        int i = 0;
        int maxId = getMaxWageId();
        while (i < employeesCount) {
            if (wageRecordsCount >= maxWageRecord) {
                cout << "WARNING: Max wage records reached unexpectedly during payment process." << endl;
                break;
            }
            wageRecords[wageRecordsCount++] = { ++maxId, employees[i].employeeId, employees[i].wage, paidDate };
            i++;
        }
        storeWage();
        cout << "INFO : Successfully paid employee wage this period" << endl;
    } else {
        cout << "WARNING : Wage already paid for this\n";
    }
}

int getLongestAmountPaid() {
    int longestAmount = 0;
    for (int i = 0; i < wageRecordsCount; i++) {
        stringstream ss;
        ss << fixed << setprecision(2) << wageRecords[i].amountPaid;
        longestAmount = max(longestAmount, static_cast<int>(ss.str().length()));
    }
    return longestAmount;
}

void showWageRecord(bool isAll = false) {
    int recordCount = 0;
    int idLen = 5;
    int employeeLen = max(getLongestEmployee(LongestEmployee::NAME) + 8, 16);
    int amountLen = max(getLongestAmountPaid() + 3, 9);
    int dateLen = 13;
    int separatorLen = idLen + employeeLen + amountLen + dateLen + 1;

    cout
        << setfill('=') << setw(separatorLen) << "" << setfill(' ') << endl
        << setw(idLen) << left << "| ID "
        << setw(employeeLen) << left << "| [ID] Employee "
        << setw(amountLen) << left << "| Amount "
        << setw(dateLen) << left << "| Date "
        << "|" << endl
        << setfill('=') << setw(separatorLen) << "" << setfill(' ') << endl;
    for (int i=0; i < wageRecordsCount; i++) {
        stringstream ss;
        ss << fixed << setprecision(2) << wageRecords[i].amountPaid;
        if (!isAll) {
            if (wageRecords[i].employeeId == loggedInEmployee.employeeId) {
                recordCount += 1;
                cout
                    << setw(idLen) << left << "| " + to_string(wageRecords[i].recordId)
                    << setw(employeeLen) << left << "| [" + to_string(wageRecords[i].employeeId)  + "] " + findEmployeeById(wageRecords[i].employeeId)
                    << setw(amountLen) << left << "| " + ss.str()
                    << setw(dateLen) << left << "| " + wageRecords[i].paymentDate
                    << "|" << endl;
            }
        } else {
            recordCount += 1;
            cout
                << setw(idLen) << left << "| " + to_string(wageRecords[i].recordId)
                << setw(employeeLen) << left << "| [" + to_string(wageRecords[i].employeeId)  + "] " + findEmployeeById(wageRecords[i].employeeId)
                << setw(amountLen) << left << "| " + ss.str()
                << setw(dateLen) << left << "| " + wageRecords[i].paymentDate
                << "|" << endl;
        }
    }
    if (recordCount == 0) cout << "INFO : No record available\n";
    cout << setfill('=') << setw(separatorLen) << "" << setfill(' ') << endl;
}


int main() {
    loadDepartment();
    loadEmployee();
    loadAttendance();
    loadWage();

    if (!loginEmployee()) return 0;

    while (true) {
        system("cls");
        int mainChoice = getMainChoice();
        if (mainChoice == 0) {
            cout << "Exit\n";
            break;
        }

        if (loggedInEmployee.role == Role::admin) {
            switch (mainChoice) {
                case 1: { // Employee Menu
                    while (true) {
                        int employeeChoice = getEmployeeChoice();
                        if (employeeChoice == 0) break;
                        switch (employeeChoice) {
                            case 1:
                                cout << "Show Employee\n";
                                showEmployee();
                                break;
                            case 2:
                                addEmployee();
                                break;
                            case 3:
                                updateEmployee();
                                break;
                            case 4:
                                deleteEmployee();
                                break;
                            case 5: {
                                while (true) {
                                    int searchEmployeeChoice = getSearchEmployeeChoice();
                                    if (searchEmployeeChoice == 0) break;
                                    Employee* e = getRealArrayEmployee();
                                    switch (searchEmployeeChoice) {
                                        case 1: {
                                            cout << "Search Employee By ID (Binary)\n";
                                            int target = inputInt("Enter employee ID", 1, getMaxEmployeeId());
                                            binarySearchEmployee(e, target);
                                            break;
                                        }
                                        case 2:
                                            searchEmployeeByName();
                                            break;
                                        default:
                                            invalidChoice();
                                            break;
                                    }
                                    delete[] e;
                                    cin.ignore();
                                    enterToContinue();
                                }
                                break;
                            }
                            default:
                                invalidChoice();
                                break;
                        }
                        cin.ignore();
                        enterToContinue();
                    }
                    break;
                }
                case 2: { // Attendance Menu
                    while (true) {
                        int attendanceChoice = getAttendanceChoice(true);
                        if (attendanceChoice == 0) break;
                        switch (attendanceChoice) {
                            case 1: {
                                if (!isAttend()) {
                                    cout << "Make Attendance\n";
                                    cout << "1. Present\n2. Absent\n";
                                    int status = inputInt("Choose option", 1, 2);
                                    recordAttendance(status);
                                } else {
                                    cout << "INFO : You already make attendance today\n";
                                }
                                break;
                            }
                            case 2:
                                cout << "Attendance Self Report\n";
                                showAttendanceRecord(false, false, 0);
                                break;
                            case 3:
                                cout << "Attendance Employee Report\n";
                                showEmployee();
                                {
                                    int targetId = inputInt("Select employee ID", 1, getMaxEmployeeId());
                                    showAttendanceRecord(false, true, targetId);
                                }
                                break;
                            default:
                                invalidChoice();
                                break;
                        }
                        cin.ignore();
                        enterToContinue();
                    }
                    break;
                }
                case 3: { // Wage Manager Menu
                    while (true) {
                        int wageManagerChoice = getWageManagerChoice();
                        if (wageManagerChoice == 0) break;
                        switch (wageManagerChoice) {
                            case 1:
                                payWage();
                                break;
                            case 2:
                                adjustEmployeeWage();
                                break;
                            case 3:
                                showWageRecord(true);
                                break;
                            case 4:
                                showWageRecord(false);
                                break;
                            default:
                                invalidChoice();
                                break;
                        }
                        cin.ignore();
                        enterToContinue();
                    }
                    break;
                }
                case 4: { // Department Menu
                    while (true) {
                        int departmentChoice = getDepartmentChoice();
                        if (departmentChoice == 0) break;
                        switch (departmentChoice) {
                            case 1:
                                showDepartments();
                                break;
                            case 2:
                                addDepartment();
                                break;
                            case 3:
                                updateDepartment();
                                break;
                            case 4:
                                deleteDepartment();
                                break;
                            default:
                                invalidChoice();
                                break;
                        }
                        cin.ignore();
                        enterToContinue();
                    }
                    break;
                }
                default:
                    invalidChoice();
                    break;
            }
        } else if (loggedInEmployee.role == Role::employee) {
            switch (mainChoice) {
                case 1: { // Attendance Menu
                    while (true) {
                        int attendanceChoice = getAttendanceChoice(false);
                        if (attendanceChoice == 0) break;
                        switch (attendanceChoice) {
                            case 1:
                                if (!isAttend()) {
                                    cout << "Make Attendance\n";
                                    cout << "1. Present\n2. Absent\n";
                                    int status = inputInt("Choose option : ", 1, 2);
                                    recordAttendance(status);
                                } else {
                                    cout << "INFO : You already make attendance today\n";
                                }
                                break;
                            case 2:
                                cout << "Attendance Self Report\n";
                                showAttendanceRecord(false, false, 0);
                                break;
                            default:
                                invalidChoice();
                                break;
                        }
                        cin.ignore();
                        enterToContinue();
                    }
                    break;
                }
                case 2: // Wage Record
                    cout << "Wage Records\n";
                    showWageRecord(false);
                    cin.ignore();
                    enterToContinue();
                    break;
                default:
                    invalidChoice();
                    break;
            }
        }
    }
}

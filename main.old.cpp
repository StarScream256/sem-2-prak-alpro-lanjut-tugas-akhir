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

struct GlobalSize {
    static constexpr int department = 11;
    static constexpr int employee = 100;
    static constexpr int attendance = 100;
    static constexpr int wageRecord = 100;
};

Department departments[GlobalSize::department];

Employee employees[GlobalSize::employee];

WageRecord wageRecords[GlobalSize::wageRecord] = {
    {1, 1, 2500.00, "2025-04-04"},
    {-1, -1, 0.0, "-"}
};

Attendance attendanceRecords[GlobalSize::attendance] = {
    {1, 1, "2025-02-14 07:31:21", AttendanceStatus::present},
    {2, 2, "2025-02-14 07:36:38", AttendanceStatus::present},
    {3, 3, "2025-02-14 07:31:21", AttendanceStatus::absent},
    {-1, -1, "Sentinel Value", AttendanceStatus::absent}
};

Employee loggedInUser;
int deparmentCount = 0;
int employeeCount = 0;
int wageRecordCount = 1;
int attendanceRecordCount = 3;

/**
 * Template function generic input
 * 
 * @param string prompt
 * @return input value at any type
 */
template <typename T> T input(const string prompt) {
    T value;
    cout << prompt;
    cin >> value;
    return value;
}

/**
 * Template function specialization for string
 * 
 * @param string prompt
 * @return string input value
 */
template <> string input<string>(const string prompt) {
    string value;
    cout << prompt;
    getline(cin, value);
    return value;
}

void clearScreen() {
    system("cls");
}

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

void setSentinelDepartment(int sentinelIndex) {
    departments[sentinelIndex] = {-1, "Sentinel Value"};
}

void setSentinelEmployee(int sentinelIndex) {
    employees[sentinelIndex] = {-1, -1, "Sentinel Value", "Sentinel Value", "Sentinel Value", "", "", "", -1, Role::employee};
}

void setSentinelAttendance(int sentinelIndex) {
    attendanceRecords[sentinelIndex] = {-1, -1, "Sentinel Value", AttendanceStatus::absent};
}

void setSentinelWageRecord(int sentinelIndex) {
    wageRecords[sentinelIndex] = {-1, -1, 0.0, "-"};
}


Department* getRealArrayDepartment() {
    Department* realArr = new Department[deparmentCount];
    for (int i=0; i < deparmentCount && (deparmentCount < GlobalSize::department ? i < deparmentCount : true); i++) {
        realArr[i] = departments[i];
    }
    return realArr;
}

Employee* getRealArrayEmployee() {
    Employee* realArr = new Employee[employeeCount];
    for (int i=0; i < employeeCount && (employeeCount < GlobalSize::employee ? i < employeeCount : true); i++) {
        realArr[i] = employees[i];
    }
    return realArr;
}

WageRecord* getRealArrayWageRecord() {
    WageRecord* realArr = new WageRecord[wageRecordCount];
    for (int i = 0; i < wageRecordCount && (wageRecordCount < GlobalSize::wageRecord ? i < wageRecordCount : true); i++) {
        realArr[i] = wageRecords[i];
    }
    return realArr;
}

Attendance* getRealArrayAttendance() {
    Attendance* realArr = new Attendance[attendanceRecordCount];
    for (int i = 0; i < attendanceRecordCount && (attendanceRecordCount < GlobalSize::attendance ? i < attendanceRecordCount : true); i++) {
        realArr[i] = attendanceRecords[i];
    }
    return realArr;
}


// START AUTH
pair<bool, string> login(string email, string password) {
    string matchPassword = "";
    int count = 0;
    while(count < GlobalSize::employee && count < employeeCount) {
        if(employees[count].email == email) {
            matchPassword = employees[count].password;
            loggedInUser = employees[count];
            break;
        }
        count++;
    }
    if(!matchPassword.length()) return make_pair(false, "User not found");
    if(matchPassword != password) return make_pair(false, "Invalid password");
    return make_pair(true, "Login success");
}

// START MENU SECTION
int getMainChoice() {
    int choice;
    clearScreen();
    cout << "Welcome " << loggedInUser.name << "! Please choose an option:" << endl;

    if (loggedInUser.role == Role::admin) {
        cout << "1. Employee" << endl;
        cout << "2. Attendance" << endl;
        cout << "3. Wage Manager" << endl;
        cout << "4. Department" << endl;
        cout << "0. Exit" << endl;
    } else {
        cout << "1. Attendance\n";
        cout << "2. Wage Record\n";
        cout << "0. Exit\n";
    }

    choice = input<int>("Enter your choice: ");
    cin.ignore();
    return choice;
}

int getEmployeeChoice() {
    int choice;
    clearScreen();
    cout << "Employee Menu\n";
    cout << "1. Show All Employee\n";
    cout << "2. Add Employee\n";
    cout << "3. Update Employee\n";
    cout << "4. Delete Employee\n";
    cout << "5. Search Employee\n";
    cout << "0. Back to main menu\n";
    choice = input<int>("Enter your choice: ");
    cin.ignore();
    return choice;
}

int getSearchEmployeeChoice() {
    int choice;
    clearScreen();
    cout << "Search Employee Menu\n";
    cout << "1. Search by ID\n";
    cout << "2. Search by Name\n";
    cout << "0. Back to employee menu\n";
    choice = input<int>("Enter your choice: ");
    cin.ignore();
    return choice;
}

int getAttendanceChoice(bool isAdmin = true) {
    int choice;
    clearScreen();
    cout << "Attendance Menu\n";
    cout << "1. Make Attendance\n";
    cout << "2. Self Report\n";
    if (isAdmin) {
        cout << "3. Employee Report\n";
    }
    cout << "0. Back to main menu\n";
    choice = input<int>("Enter your choice: ");
    cin.ignore();
    return choice;
}

int getWageManagerChoice() {
    int choice;
    clearScreen();
    cout << "Wage Manager Menu\n";
    cout << "1. Pay Employee Wage This Month\n";
    cout << "2. Adjust Employee Wage\n";
    cout << "3. Show All Wage Records\n";
    cout << "4. Show My Wage Records\n";
    cout << "0. Back to main menu\n";
    choice = input<int>("Enter your choice : ");
    cin.ignore();
    return choice;
}

int getDepartmentChoice() {
    int choice;
    clearScreen();
    cout << "Department Menu\n";
    cout << "1. Show All Departments\n";
    cout << "2. Add Department\n";
    cout << "3. Update Department\n";
    cout << "4. Delete Department\n";
    cout << "0. Back to main menu\n";
    choice = input<int>("Enter your choice: ");
    cin.ignore();
    return choice;
}


// START DEPARTMENT SECTION
void loadDepartment() {
    ifstream file("./data/departments.txt");
    if (file.is_open()) {
        deparmentCount = 0;
        string line;
        Department temp;
        while (getline(file, line)) {
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
            deparmentCount++;
            departments[deparmentCount - 1] = temp;
        }
        setSentinelDepartment(deparmentCount);
    } else {
        cout << "Failed to load department data\n";
    }
    file.close();
}

void storeDepartment() {
    ofstream file("./data/depatments.txt");
    if (file.is_open()) {
        for (size_t i = 0; i < deparmentCount; i++) {
            file << departments[i].departmentId << "," << departments[i].departmentName << endl;
        }
    } else {
        cout << "Failed to store deparment data\n";
    }
    file.close();
}

string searchDepartmentById(int target) {
    for (int i=0; i < deparmentCount; i++) {
        if (departments[i].departmentId == target) {
            return departments[i].departmentName;
        }
    }
    return "Invalid parameter";
}

void showDepartments() {
    cout << "Department List:\n";
    cout
        << setfill('=') << setw(30) << "" << setfill(' ') << endl
        << setw(5) << left << "ID"
        << setw(25) << left << "Department Name" << endl
        << setfill('=') << setw(30) << "" << setfill(' ') << endl;
    for (int i = 0; i < deparmentCount; i++) {
        cout
            << setw(5) << left << departments[i].departmentId
            << setw(25) << left << departments[i].departmentName << endl;
    }
    cout << setfill('=') << setw(30) << "" << setfill(' ') << endl;
}

// addDepartment()
// updateDepartment()
// showDepartment()
// deleteDepartment()


// START EMPLOYEE SECTION (Done)
void loadEmployee() {
    ifstream file("employees.txt");
    if (file.is_open()) {
        employeeCount = 0;
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
            employees[employeeCount++] = temp;
        }
        setSentinelEmployee(employeeCount);
    } else {
        cout << "Failed to load employee data\n";
    }
    file.close();
}

void storeEmployee() {
    ofstream file("./data/employees.txt");
    if (file.is_open()) {
        for (size_t i = 0; i < employeeCount; i++) {
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
    } else {
        cout << "Failed to store employee data\n";
    }
    file.close();
}

void showEmployee() {
    cout
        << setfill('=') << setw(87) << "" << setfill(' ') << endl
        << setw(6) << left << "| ID"
        << setw(30) << left << "| Name"
        << setw(30) << left << "| Email"
        << setw(20) << left << "| Job Title" << right << "|" << endl
        << setfill('=') << setw(87) << "" << setfill(' ') << endl;
    for (int i = 0; i < employeeCount; i++) {
        cout
            << setw(6) << left << "| " + to_string(employees[i].employeeId)
            << setw(30) << left << "| " + employees[i].name
            << setw(30) << left << "| " + employees[i].email
            << setw(20) << left << "| " + employees[i].jobTitle << right << "|" << endl;

    }
    cout << setfill('=') << setw(87) << "" << setfill(' ') << endl;
}

void addEmployee() {
    ofstream file("./data/employees.txt", ios::app);
    if (file.is_open()) {
        Employee newEmployee;
        int lastIndex = 0, roleNum;
        while (lastIndex < GlobalSize::employee && lastIndex < employeeCount) {
            lastIndex++;
        }

        if (lastIndex < GlobalSize::employee) {
            newEmployee.employeeId = (lastIndex > 0) ? employees[lastIndex - 1].employeeId + 1 : 1;
            showDepartments();
            newEmployee.departmentId = input<int>("Masukkan ID Departemen: ");
            cin.ignore();
            newEmployee.name = input<string>("Masukkan Nama Karyawan : ");
            newEmployee.dateOfBirth = input<string>("Masukkan Tanggal Lahir (YYYY-MM-DD) : ");
            newEmployee.jobTitle = input<string>("Masukkan Jabatan : ");
            newEmployee.email = input<string>("Masukkan Email : ");
            newEmployee.address = input<string>("Masukkan Alamat : ");
            newEmployee.password = input<string>("Masukkan Password : ");
            newEmployee.wage = input<double>("Masukkan Gaji : ");
            roleNum = input<int>("Masukkan Nomor Role (1-Admin, 2-Karyawan): ");
            newEmployee.role = (roleNum == 1) ? Role::admin : Role::employee;
            cin.ignore();

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

            employees[lastIndex] = newEmployee;
            cout << "Karyawan berhasil ditambahkan\n";
            employeeCount += 1;
        } else {
            cout << "Batas maksimum karyawan telah tercapai.\n";
        }

        setSentinelEmployee(lastIndex + 1);
    } else {
        cout << "Failed to store employee data\n";
    }
    file.close();
}

void updateEmployee() {
    cout << "Mengupdate Karyawan\n";
    int roleNum;
    int id = input<int>("Masukan ID Karyawan yang ingin diupdate:");
    cin.ignore();
    for (int i = 0; i < GlobalSize::employee; i++) {
        if (employees[i].employeeId == id) {
            cout << "Karyawan ditemukan:" << employees[i].name << endl;
            employees[i].departmentId = input<int>("Masukkan ID Departemen baru: ");
            cin.ignore();
            employees[i].name = input<string>("Masukkan Nama Karyawan baru: ");
            employees[i].dateOfBirth = input<string>("Masukkan Tanggal Lahir baru (YYYY-MM-DD): ");
            employees[i].jobTitle = input<string>("Masukkan Jabatan baru: ");
            employees[i].email = input<string>("Masukkan Email baru: ");
            employees[i].address = input<string>("Masukkan Alamat baru: ");
            employees[i].password = input<string>("Masukkan Password baru: ");
            employees[i].wage = input<double>("Masukkan Gaji baru: ");
            roleNum = input<int>("Masukkan Nomor Role baru (1-Admin, 2-Karyawan): ");
            employees[i].role = (roleNum == 1) ? Role::admin : Role::employee;
            cin.ignore();
            cout << "Data karyawan berhasil diperbarui\n";
            return;
        }
        if (employees[i].employeeId == -1) break;
    }
    cout << "Karyawan dengan ID " << id << " Tidak ditemukan.\n";
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
    quickSortEmployeeById(e, 0, employeeCount - 1, true);

    int kiri = 0;
    int kanan = employeeCount - 1;
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
        cout << "Karyawan dengan ID " << idCari << " tidak ditemukan.\n";
    }
}

void searchEmployeeByName() {
    string keyword = input<string>("Enter name : ");
    int found = 0;
    keyword = toLowerCase(keyword);

    cout << "Result : \n";
    for (int i=0; i < employeeCount; i++) {
        if (toLowerCase(employees[i].name).find(keyword) != string::npos) {
            found++;
            cout << found << ") "
                << employees[i].name
                << ", from " << searchDepartmentById(employees[i].departmentId) << " department\n";
        }
    }
    if (found == 0) cout << "No employee found\n";
}

void deleteEmployee() {
    showEmployee();
    bool found = false;
    int index = -1;
    int target = input<int>("Enter employee ID to delete : ");
    for (int i = 0; i < employeeCount; i++) {
        if (employees[i].employeeId == target) {
            index = i;
            found = true;
            break;
        }
    }
    if (found) {
        for (int i = index; i < employeeCount - 1; i++) {
            employees[i] = employees[i+1];
        }
        employeeCount -= 1;
    } else {
        cout << "Requested ID can't be found" << endl;
    }
    storeEmployee();
    loadEmployee();
    cin.ignore();
}


// START ATTENDANCE SECTION (Done)
bool isAttend() {
    auto now = chrono::system_clock::now();
    time_t now_c = chrono::system_clock::to_time_t(now);
    tm local_time;
    localtime_s(&local_time, &now_c);
    stringstream ss_current_date;
    ss_current_date << put_time(&local_time, "%Y-%m-%d");

    Attendance* lastRecord = nullptr;
    for (int i=0; i < attendanceRecordCount; i++) {
        if (attendanceRecords[i].employeeId == loggedInUser.employeeId) {
            if (lastRecord == nullptr || attendanceRecords[i].dateTime > lastRecord->dateTime) {
                lastRecord = &attendanceRecords[i];
            }
        }
    }

    if (lastRecord != nullptr) {
        istringstream iss(lastRecord->dateTime);
        tm t{};
        iss >> get_time(&t, "%Y-%m-%d %H:%M:%S"); // parse to get dt
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

    int insertIndex = 0, nextId;
    for (int i=0; i < attendanceRecordCount; i++) {
        insertIndex = i;
        nextId = attendanceRecords[i].recordId;
    }
    insertIndex += 1;
    attendanceRecords[insertIndex].recordId = nextId + 1;
    attendanceRecords[insertIndex].employeeId = loggedInUser.employeeId;
    attendanceRecords[insertIndex].dateTime = currentDateTime;
    if (status == 1) {
        if (currentHour <= max_hour || (currentHour == max_hour && currentMinutes <= max_minutes)) {
            attendanceRecords[insertIndex].status = AttendanceStatus::present;
        } else {
            attendanceRecords[insertIndex].status = AttendanceStatus::late;
        }
    } else {
        attendanceRecords[insertIndex].status = AttendanceStatus::absent;
    }
    attendanceRecordCount++;
    setSentinelAttendance(insertIndex + 1);
    cin.ignore();
}

void showAttendanceRecord(bool isAll = false, bool isSpecific = false, int reqEmployeeId = 0) {
    string status;
    int recordCount = 0;

    cout
        << setfill('=') << setw(45) << "" << setfill(' ') << endl
        << setw(5) << left << "ID"
        << setw(5) << left << "EID"
        << setw(25) << left << "Datetime"
        << setw(10) << left << "Status" << endl
        << setfill('=') << setw(45) << "" << setfill(' ') << endl;
    for (int i=0; i < attendanceRecordCount; i++) {
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
            if (attendanceRecords[i].employeeId == loggedInUser.employeeId) {
                recordCount += 1;
                cout
                    << setw(5) << left << attendanceRecords[i].recordId
                    << setw(5) << left << attendanceRecords[i].employeeId
                    << setw(25) << left << attendanceRecords[i].dateTime
                    << setw(10) << left << status << endl;
            }
        } else if (isSpecific) {
            if (attendanceRecords[i].employeeId == reqEmployeeId) {
                recordCount += 1;
                cout
                    << setw(5) << left << attendanceRecords[i].recordId
                    << setw(5) << left << attendanceRecords[i].employeeId
                    << setw(25) << left << attendanceRecords[i].dateTime
                    << setw(10) << left << status << endl;
            }
        } else {
            recordCount += 1;
            cout
                << setw(5) << left << attendanceRecords[i].recordId
                << setw(5) << left << attendanceRecords[i].employeeId
                << setw(25) << left << attendanceRecords[i].dateTime
                << setw(10) << left << status << endl;
        }
    }
    if (recordCount == 0) cout << "No record available\n";
    cout << setfill('=') << setw(45) << "" << setfill(' ') << endl;
}


// START WAGE SECTION (Done)
void payWage() {
    time_t now = time(0);
    tm* localTime = localtime(&now);
    int day = localTime->tm_mday;
    int month = localTime->tm_mon + 1;
    int year = localTime->tm_year + 1900;
    string strDay = to_string(day);
    string strMonth = to_string(month).length() == 1 ? "0" + to_string(month) : to_string(month);
    string strYear = to_string(year);
    string lastPaid = wageRecords[wageRecordCount-1].paymentDate.substr(5, 2);
    string paidDate = strYear + "-" + strMonth + "-" + strDay;
    if (strMonth != lastPaid && strMonth > lastPaid) {
        int i = 0;
        int newRecordId = wageRecordCount > 0 ? wageRecords[wageRecordCount - 1].recordId + 1 : 1;
        while (i < employeeCount) {
            wageRecords[wageRecordCount] = { newRecordId++, employees[i].employeeId, employees[i].wage, paidDate };
            i++;
            wageRecordCount += 1;
        }
        setSentinelWageRecord(wageRecordCount);
        cout << "Payment success\n";
    } else {
        cout << "Wage already paid\n";
    }
}

void showWageRecord(bool isAll = false) {
    int recordCount = 0;
    cout
        << setfill('=') << setw(38) << "" << setfill(' ') << endl
        << setw(5) << left << "ID"
        << setw(6) << left << "E-ID"
        << setw(12) << left << "Amount"
        << setw(15) << left << "Date" << endl
        << setfill('=') << setw(38) << "" << setfill(' ') << endl;
    for (int i=0; i < wageRecordCount; i++) {
        if (!isAll) {
            if (wageRecords[i].employeeId == loggedInUser.employeeId) {
                recordCount += 1;
                cout
                    << setw(5) << left << wageRecords[i].recordId
                    << setw(6) << left << wageRecords[i].employeeId
                    << setw(12) << left << wageRecords[i].amountPaid
                    << setw(15) << left << wageRecords[i].paymentDate << endl;
            }
        } else {
            recordCount += 1;
            cout
                << setw(5) << left << wageRecords[i].recordId
                << setw(6) << left << wageRecords[i].employeeId
                << setw(12) << left << wageRecords[i].amountPaid
                << setw(15) << left << wageRecords[i].paymentDate << endl;
        }
    }
    if (recordCount == 0) cout << "No record available\n";
    cout << setfill('=') << setw(38) << "" << setfill(' ') << endl;
}

void adjustEmployeeWage() {
    showEmployee();
    int targetEmployeeId = input<int>("Enter Employee ID : ");
    bool found = false;
    for (int i = 0; i < employeeCount; i++) {
        if (employees[i].employeeId == targetEmployeeId) {
            found = true;
            cout << "Name : " << employees[i].name << endl;
            cout << "Current Wage " << employees[i].wage << endl;
            double newWage = input<double>("Enter new wage : ");
            employees[i].wage = newWage;
            cout << "Successfully adjust wage\n";
            break;
        }
    }
    if (!found) cout << "No employee found with ID " << to_string(targetEmployeeId) << endl;
    cin.ignore();
}


int main() {
    loadDepartment();
    loadEmployee();

    int loginCount = 1;
    bool loginSuccess = false;
    string email, password, loginMessage = "";
    while (!loginSuccess && loginCount <= 3) {
        clearScreen();
        cout << "Login to continue\n";
        email = input<string>("Work Email\t: ");
        password = input<string>("Password\t: ");
        tie(loginSuccess, loginMessage) = login(email, password);
        if(!loginSuccess) {
            cout << "Login failed, " << loginMessage << endl;
            cout << 3 - loginCount << " remaining attempt\n";
            cout << "Press 'Enter' to try again ...";
            cin.get();
        }
        loginCount++;
    }
    if(!loginSuccess) {
        cout << "Too many attempt\n";
        return 0;
    }

    int choice;
    do {
        clearScreen();
        choice = getMainChoice();
        if (loggedInUser.role == Role::admin) {
            switch (choice) {
                // Employee
                case 1: {
                    int employeeChoice;
                    do {
                        employeeChoice = getEmployeeChoice();
                        switch (employeeChoice) {
                            case 1:
                                cout << "Show Employee\n";
                                showEmployee();
                                enterToContinue();
                                break;
                            case 2:
                                cout << "Menambahkan Karyawan Baru" << endl;
                                addEmployee();
                                enterToContinue();
                                break;
                            case 3:
                                updateEmployee();
                                enterToContinue();
                                break;
                            case 4:
                                deleteEmployee();
                                enterToContinue();
                                break;
                            case 5: {
                                int searchEmployeeChoice;
                                do {
                                    Employee* e = getRealArrayEmployee();
                                    searchEmployeeChoice = getSearchEmployeeChoice();
                                    switch (searchEmployeeChoice) {
                                        case 1: {
                                            cout << "Search Employee By ID (Binary)\n";
                                            int target = input<int>("Enter employee ID : ");
                                            cin.ignore();
                                            binarySearchEmployee(e, target);
                                            cout << endl;
                                            enterToContinue();
                                            break;
                                        }
                                        case 2:
                                            searchEmployeeByName();
                                            enterToContinue();
                                            break;
                                        case 0:
                                            cout << "Back to employee menu";
                                            break;
                                        default:
                                            invalidChoice();
                                            break;
                                    }
                                } while (searchEmployeeChoice != 0);
                                break;
                            }
                            case 0:
                                cout << "Back to main menu\n";
                                break;
                            default:
                                invalidChoice();
                                break;
                        }
                    } while (employeeChoice != 0);
                    break;
                } 
                // Attendance
                case 2: {
                    int attendanceChoice;
                    do {
                        attendanceChoice = getAttendanceChoice(true);
                        switch (attendanceChoice) {
                            case 1: {
                                if (!isAttend()) {
                                    int status;
                                    cout << "Make Attendance\n";
                                    cout << "1. Present\n";
                                    cout << "2. Absent\n";
                                    status = input<int>("Choose option : ");
                                    recordAttendance(status);
                                } else {
                                    cout << "You already make attendance today\n";
                                }
                                enterToContinue();
                                break;
                            }
                            case 2: {
                                cout << "Attendance Self Report\n";
                                showAttendanceRecord(false, false, 0);
                                enterToContinue();
                                break;
                            }
                            case 3: {
                                cout << "Attendance Employee Report\n";
                                showEmployee();
                                int targetId = input<int>("Select employee ID : ");
                                showAttendanceRecord(false, true, targetId);
                                cin.ignore();
                                enterToContinue();
                                break;
                            }
                            case 0:
                                cout << "Back to main menu\n";
                                break;
                            
                            default:
                                invalidChoice();
                                break;
                        }
                    } while (attendanceChoice != 0);
                    break;
                }
                // Wage Manager
                case 3: {
                    int wageManagerChoice;
                    do {
                        wageManagerChoice = getWageManagerChoice();
                        switch (wageManagerChoice) {
                            case 1:
                                payWage();
                                enterToContinue();
                                break;
                            case 2:
                                adjustEmployeeWage();
                                enterToContinue();
                                break;
                            case 3:
                                showWageRecord(true);
                                enterToContinue();
                                break;
                            case 4:
                                showWageRecord(false);
                                enterToContinue();
                                break;
                            case 0:
                                cout << "Back to main menu\n";
                                break;
                            default:
                                invalidChoice();
                                break;
                        }
                    } while (wageManagerChoice != 0);
                    break;
                }
                // Department
                case 4: {
                    int departmentChoice;
                    do {
                        departmentChoice = getDepartmentChoice();
                        switch (departmentChoice) {
                            case 1:
                                showDepartments();
                                enterToContinue();
                                break;
                            case 2:
                                cout << "Add Department\n";
                                // TODO: Implement addDepartment()
                                enterToContinue();
                                break;
                            case 3:
                                cout << "Update Department\n";
                                // TODO: Implement updateDepartment()
                                enterToContinue();
                                break;
                            case 4:
                                cout << "Delete Department\n";
                                // TODO: Implement deleteDepartment()
                                enterToContinue();
                                break;
                            case 0:
                                cout << "Back to main menu\n";
                                break;
                            default:
                                invalidChoice();
                                break;
                        }
                    } while (departmentChoice != 0);
                    break;
                }
                case 0:
                    cout << "Exit\n";
                    break;
                default:
                    invalidChoice();
                    break;
            }
        } else if (loggedInUser.role == Role::employee) {
            switch (choice) {
                case 1: {
                    int attendanceChoice;
                    do {
                        attendanceChoice = getAttendanceChoice();
                        switch (attendanceChoice) {
                            case 1: {
                                if (!isAttend()) {
                                    int status;
                                    cout << "Make Attendance\n";
                                    cout << "1. Present\n";
                                    cout << "2. Absent\n";
                                    status = input<int>("Choose option : ");
                                    recordAttendance(status);
                                } else {
                                    cout << "You already make attendance today\n";
                                }
                                enterToContinue();
                                break;
                            }
                            case 2: {
                                cout << "Attendance Self Report\n";
                                showAttendanceRecord(false, false, 0);
                                enterToContinue();
                                break;
                            }
                            case 0:
                                cout << "Back to main menu\n";
                                break;
                            
                            default:
                                break;
                        }
                    } while (attendanceChoice != 0);
                }
                case 2: {
                    cout << "Wage Records\n";
                    showAttendanceRecord(false);
                    enterToContinue();
                }
                case 0:
                    cout << "Exit\n";
                    break;
                
                default:
                    invalidChoice();
                    break;
            }
        }
    } while (choice != 0);
}
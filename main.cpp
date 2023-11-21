#include<iostream>
#include<iomanip>
#include<mysql.h>
#include<fstream>
#include<string>
#include <vector>
#include <sstream>
#include <limits>


using namespace std;

const char* host="Enter_hostName";
const char* user="Enter_userName";
const char* password="Enter_password";
const char* database="Enter_databaseName";

const int MAX=100;

MYSQL* admin=mysql_init(NULL);

char name[MAX];
char regNumber[MAX];
char phoneNumber[MAX];
char branch[MAX];
char semester[MAX];
char cgpa[MAX];
char parentNumber[MAX];
char email[MAX];
char address[MAX];

char deletereg[MAX];

void addStudent(MYSQL* admin, char name[MAX], char regNumber[MAX], char phoneNumber[MAX], char branch[MAX], char semester[MAX], char cgpa[MAX], char parentNumber[MAX], char email[MAX], char address[MAX])
{
    cout<<"Enter the registration number of the student: ";
    cin>>regNumber;

    cout<<"Enter the name of the student: ";
    cin>>name;

    cout<<"Enter the phone number of the student: ";
    cin>>phoneNumber;

    cout<<"Enter the branch of the student: ";
    cin>>branch;

    cout<<"Enter the semester of the student: ";
    cin>>semester;

    cout<<"Enter the CGPA of the student: ";
    cin>>cgpa;

    cout<<"Enter the parents' number of the student: ";
    cin>>parentNumber;

    cout<<"Enter the email ID of the student: ";
    cin>>email;

    cout<<"Enter the address of the student: ";
    cin>>address;

    char query[256];
    snprintf(query, sizeof(query), "INSERT INTO students_data VALUES('%s','%s','%s','%s','%s','%s','%s','%s','%s')",name,branch,email,address,semester,phoneNumber,parentNumber,regNumber,cgpa);



    if(mysql_query(admin,query))
    {
        cout<<"Insertion Error"<<endl;
        cout<<mysql_error(admin)<<endl;
    }
    else
    {
        cout<<"Insertion Successful"<<endl;
    }
}

void editStudent(MYSQL*admin, char name[MAX], char regNumber[MAX], char phoneNumber[MAX], char branch[MAX], char semester[MAX], char cgpa[MAX], char parentNumber[MAX], char email[MAX], char address[MAX]) {
    cout << "Enter the registration number of the student: ";
    cin >> regNumber;

    cout << "Enter the name of the student: ";
    cin >> name;

    cout << "Enter the phone number of the student: ";
    cin >> phoneNumber;

    cout << "Enter the branch of the student: ";
    cin >> branch;

    cout << "Enter the semester of the student: ";
    cin >> semester;

    cout << "Enter the CGPA of the student: ";
    cin >> cgpa;

    cout << "Enter the parents' number of the student: ";
    cin >> parentNumber;

    cout << "Enter the email ID of the student: ";
    cin >> email;

    cout << "Enter the address of the student: ";
    cin >> address;

    char query[256];
    snprintf(query, sizeof(query),
             "UPDATE students_data SET name='%s', branch='%s', email='%s', address='%s', semester='%s', phoneNumber='%s', parentsNumber='%s', cgpa='%s' WHERE regNumber='%s'",
             name, branch, email, address, semester, phoneNumber, parentNumber, regNumber, cgpa);

    if (mysql_query(admin, query)) {
        cout << "Updation Error" << endl;
        cout << mysql_error(admin) << endl;
    } else {
        cout << "Updation Successful" << endl;
    }
}

void deleteStudent(MYSQL* admin, char deletereg[MAX])
{
    cout<<"Enter the reg you want to delete: ";
    cin>>deletereg;

    //  This MySQL query used to check the party name is already exist or not
    char selectquery[100];
    snprintf(selectquery, sizeof(selectquery), "SELECT regNumber FROM students_data WHERE regNumber = '%s'", deletereg);

    if (mysql_ping(admin))
    {
        cout<<"ERROR: Impossible to connect."<<endl;
        cout<<mysql_error(admin)<<endl;
        return;
    }

    if (mysql_query(admin, selectquery))
    {
        cout<<"ERROR: "<<mysql_error(admin)<<endl;
        return;
    }

    MYSQL_RES* result=mysql_store_result(admin);
    if (result)
    {
        if (mysql_num_rows(result)>0)
        {
            //  This MySQL query used to delete the party details from the database
            char deletequery[100];
            snprintf(deletequery, sizeof(deletequery), "DELETE FROM students_data WHERE regNumber = '%s'", deletereg);

            if (mysql_query(admin, deletequery))
            {
                cout<<"Error deleting Voter: "<<mysql_error(admin)<<endl;
            }
            else
            {
                cout<<"Student deleted successfully."<<endl;
            }
        }
        else
        {
            cout<<"Student with this ID doesn't exist. No deletion performed."<<endl;
        }
        mysql_free_result(result);
    }
}

void display(MYSQL* admin)
{
    const char* selectquery="SELECT * FROM students_data";

    if (mysql_ping(admin))
    {
        cout<<"ERROR: Impossible to connect."<<endl;
        cout<<mysql_error(admin)<<endl;
        return;
    }

    if (mysql_query(admin, selectquery))
    {
        cout<<"ERROR: "<<mysql_error(admin)<<endl;
        return;
    }

    MYSQL_RES* result=mysql_store_result(admin);
    if (result)
    {
        int numFields=mysql_num_fields(result);
        MYSQL_ROW row;

        // Find the maximum width for each column
        int* columnWidths=new int[numFields];
        for (int i=0; i<numFields; i++)
        {
            columnWidths[i] = 0;
        }

        // Calculate column widths
        while ((row = mysql_fetch_row(result)))
        {
            for (int i = 0; i<numFields; i++)
            {
                int fieldWidth=strlen(row[i]);
                if (fieldWidth>columnWidths[i])
                {
                    columnWidths[i]=fieldWidth;
                }
            }
        }

        // Display the header
        for (int i = 0; i<numFields; i++)
        {
            cout<<left<<setw(columnWidths[i] + 2)<<mysql_fetch_field_direct(result, i)->name<<"\t\t";
        }
        cout<<endl;

        // Display the data
        mysql_data_seek(result, 0);  // Reset the result set to the beginning
        while ((row = mysql_fetch_row(result)))
        {
            for (int i = 0; i<numFields; i++)
            {
                cout<<left<<setw(columnWidths[i] + 2)<<row[i]<<"\t\t";
            }
            cout<<endl;
        }

        delete[] columnWidths;  // Clean up allocated memory

        mysql_free_result(result);
    }
}

void exportCSV(MYSQL* admin)
{
    const char* selectquery = "SELECT * FROM students_data";

    if (mysql_ping(admin))
    {
        cout << "ERROR: Impossible to connect." << endl;
        cout << mysql_error(admin) << endl;
        return;
    }

    if (mysql_query(admin, selectquery))
    {
        cout << "ERROR: " << mysql_error(admin) << endl;
        return;
    }

    MYSQL_RES* result = mysql_store_result(admin);
    if (result)
    {
        ofstream file("students.csv");
        if (!file.is_open()) {
            cout << "ERROR: Unable to create/open file." << endl;
            mysql_free_result(result);
            return;
        }

        int numFields = mysql_num_fields(result);
        MYSQL_ROW row;

        // Add headers
        for (int i = 0; i < numFields; i++)
        {
            file << "\"" << mysql_fetch_field_direct(result, i)->name << "\",";
        }
        file.seekp(-1, ios_base::end); // Remove trailing comma
        file << endl;

        // Add data
        while ((row = mysql_fetch_row(result)))
        {
            for (int i = 0; i < numFields; i++)
            {
                file << "\"" << row[i] << "\",";
            }
            file.seekp(-1, ios_base::end); // Remove trailing comma
            file << endl;
        }

        file.close();
        mysql_free_result(result);
    }
}

// Function to import data from CSV file to MySQL
void importCSVToMySQL(MYSQL* conn, const string& fileName) {
    ifstream file(fileName.c_str()); // Convert string to const char* using c_str()
    if (!file.is_open()) {
        cout << "Unable to open file: " << fileName << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        vector<string> data;
        string cell;

        while (getline(ss, cell, ',')) {
            data.push_back(cell);
        }

        // Assuming CSV columns in the order: name, reg_no, email, phone, address, age, class_id, gpa
        if (data.size() == 8) {
            string insertQuery = "INSERT INTO students_data (name, reg_no, email, phone, address, age, class_id, gpa) VALUES ('" +
                                 data[0] + "', '" + data[1] + "', '" + data[2] + "', '" + data[3] + "', '" + data[4] + "', " +
                                 data[5] + ", " + data[6] + ", " + data[7] + ")";

            if (mysql_query(conn, insertQuery.c_str())) {
                cout << "ERROR: " << mysql_error(conn) << endl;
                return;
            }
        } else {
            cout << "Invalid data format in CSV file!" << endl;
        }
    }

    file.close();
    cout << "Imported data from CSV to MySQL successfully!" << endl;
}
void searchStudent(MYSQL* admin)
{
    const char* selectquery = "SELECT * FROM students_data WHERE ";

    string searchField, searchValue;
    cout << "Enter the field to search (name/reg_no): ";
    cin.ignore(); // Clear the input buffer
    getline(cin, searchField);

    cout << "Enter the search value: ";
    getline(cin, searchValue);

    string query = selectquery + searchField + " LIKE '%" + searchValue + "%'";

    if (mysql_ping(admin))
    {
        cout << "ERROR: Impossible to connect." << endl;
        cout << mysql_error(admin) << endl;
        return;
    }

    if (mysql_query(admin, query.c_str()))
    {
        cout << "ERROR: " << mysql_error(admin) << endl;
        return;
    }

    MYSQL_RES* result = mysql_store_result(admin);
    if (result)
    {
        int numFields = mysql_num_fields(result);
        MYSQL_ROW row;

        cout << "\nSearch Results:\n";

        while ((row = mysql_fetch_row(result)))
        {
            for (int i = 0; i < numFields; i++)
            {
                cout << row[i] << "\t";
            }
            cout << endl;
        }

        mysql_free_result(result);
    }
}


int main()
{
    if(!mysql_real_connect(admin,host,user,password,database,0,NULL,0))
    {
        cout<<"Connection Error"<<endl;
        cout<<mysql_error(admin)<<endl;
        return 1;
    }
    else
    {
        cout<<"Connection Successful"<<endl;
    }

    while (true) {
        cout << endl << "Student Database Management System" << endl;
        cout << "1. Add a new student" << endl;
        cout << "2. Edit student information" << endl;
        cout << "3. Delete a student" << endl;
        cout << "4. Show student data" << endl;
        cout << "5. Export student data to CSV" << endl;
        cout << "6. Search Students (Name/Reg. No.)" << endl;
        cout << "7. Import data from CSV to MySQL" << endl;
        cout << "0. Exit" << endl;
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;

        switch (choice) {
            case 1:
                addStudent(admin, name, regNumber, phoneNumber, branch, semester, cgpa, parentNumber, email, address);
                break;
            case 2:
                editStudent(admin, name, regNumber, phoneNumber, branch, semester, cgpa, parentNumber, email, address);
                break;
            case 3:
                deleteStudent(admin, deletereg);
                break;
            case 4:
                display(admin);
                break;
            case 5:
                exportCSV(admin);
                break;
            case 6:
                searchStudent(admin);
                break;
            case 7:
            {
                string fileName;
                cout << "Enter the CSV file name to import: ";
                cin.ignore(); // Clear input buffer before getline
                getline(cin, fileName);

                importCSVToMySQL(admin, fileName);
            }
                break;
            case 0:
                cout << "Exiting..." << endl;
                exit(0);
            default:
                cout << "Invalid Choice" << endl;
                cin.clear(); // Clear any error flags
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard input buffer
                break;
        }
    }
    return 0;
}

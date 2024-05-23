#include <iostream>
#include "src/sqlite3.h"

using namespace std;

int main() {
    sqlite3* DB;
    int exit = 0;
    exit = sqlite3_open("afriwork.db", &DB);

    char* messageError;

    string sql("CREATE TABLE IF NOT EXISTS customers("
               "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
               "Job_title TEXT NOT NULL,"
               "Job_type TEXT NOT NULL,"
               "sector TEXT,"
               "location TEXT,"
               "Experience_level TEXT,"
               "Vacancies TEXT,"
               "Salary_Compensation TEXT,"
               "Deadline DATE,"
               "description TEXT);");

    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);

    if (exit != SQLITE_OK) {
        cerr << "Error Creating Table: " << messageError << endl;
        sqlite3_free(messageError);
    } else {
        cout << "Table created Successfully!" << endl;
    }
    sqlite3_close(DB);
    
    
    return (0);
}

//g++ your ->"program name" .\src\sqlite3.o -IC:\sqlite3
// & .\"a.exe"

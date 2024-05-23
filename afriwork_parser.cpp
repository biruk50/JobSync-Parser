#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <sstream>
#include <windows.h>
#include "src/sqlite3.h"

using namespace std;

const string dbFileName ="";//enter the name of the database you created

struct Job {
    string job_title,job_type,sector,location,Experience_level,Vacancies,Salary_Compensation,deadline,description;
};

void insert(const string& dbFileName,const Job& job);

void processFile(const string& dbFileName, const string& filePath);

string processDescription(const string& input);

string processDate(const string& input);

int main() {
    string directoryPath = "";//enter file path,once you have downloaded the chat history of the channel

    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile((directoryPath + "/*.html").c_str(), &findFileData);

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            string filename = findFileData.cFileName;

            // Check if the file name starts with "messages" and ends with ".html"
            if (filename.rfind("messages", 0) == 0 && filename.size() >= 5 && filename.substr(filename.size() - 5) == ".html") {
                processFile(dbFileName, directoryPath + "/" + filename);
            }
        } while (FindNextFile(hFind, &findFileData) != 0);

        FindClose(hFind);
    }

    return 0;
}

void processFile(const string& dbFileName, const string& filePath){
    Job job;
    ifstream inStream(filePath);

    if (!inStream.is_open()) {
        std::cerr << "Error opening file: " << filePath << std::endl;
        return; // Skip to the next iteration if the file can't be opened
    }
    string line;
    regex a("<strong>Job Title:</strong>(.*?)<br>");
    regex b("<strong>Job Type:</strong>(.*?)<br>");
    regex c("<strong>Job Sector:</strong>(.*?)</a>");
    regex d("<strong>Work Location:</strong>(.*?)<br>");
    regex e("<strong>Experience Level:</strong>(.*?)<br>");
    regex f("<strong>Vacancies:</strong>(.*?)<br>");
    regex g("<strong>Salary/Compensation:</strong>(.*?)<br>");
    regex h("<strong>Deadline:</strong>(.*?)<br>");
    regex i("<strong>Description:</strong>(.*?)</a>");
    
    smatch match;

    while (getline(inStream, line)) {
        if (regex_search(line, match, a)) {
            job.job_title = match[1];
        }
        if (regex_search(line, match, b)) {
            job.job_type = match[1];
        }
        if (regex_search(line, match, c)) {
            job.sector = match[1];
            job.sector = processDescription(sector);
        }
        if (regex_search(line, match, d)) {
            job.location = match[1];
        }
        if (regex_search(line, match, e)) {
            job.Experience_level = match[1];
        }
        if (regex_search(line, match, f)) {
            job.Vacancies = match[1];
        }
        if (regex_search(line, match, g)) {
            job.Salary_Compensation= match[1];
        }
        if (regex_search(line, match, h)) {
            job.deadline= match[1];
            job.deadline =processDate(deadline);
        }
        if (regex_search(line, match, i)) {
            job.description = match[1];
            job.description = processDescription(description);

        // Call the insert function to insert data into the database
        insert(dbFileName, job);
        }
    }
    inStream.close();
}
    


void insert(const string& dbFileName,const Job& job) {
    sqlite3* DB;
    int exit = 0;
    exit = sqlite3_open(dbFileName.c_str(), &DB);
    char* messageError;

    string insertSql = "INSERT INTO customers (job_title, job_type, sector,location,"
                            "Experience_level,Vacancies,Salary_Compensation,deadline, description)"
                            " VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?);";

    sqlite3_stmt* statement;
    exit = sqlite3_prepare_v2(DB, insertSql.c_str(), -1, &statement, 0);

    if (exit != SQLITE_OK) {
        cerr << "Error preparing statement: " << sqlite3_errmsg(DB) << endl;
        sqlite3_close(DB);
        return;
    }

    exit = sqlite3_bind_text(statement, 1, job.job_title.c_str(), -1, SQLITE_STATIC);
    exit = sqlite3_bind_text(statement, 2, job.job_type.c_str(), -1, SQLITE_STATIC);
    exit = sqlite3_bind_text(statement, 3, job.sector.c_str(), -1, SQLITE_STATIC);
    exit = sqlite3_bind_text(statement, 4, job.location.c_str(), -1, SQLITE_STATIC);
    exit = sqlite3_bind_text(statement, 5, job.Experience_level.c_str(), -1, SQLITE_STATIC);
    exit = sqlite3_bind_text(statement, 6, job.Vacancies.c_str(), -1, SQLITE_STATIC);
    exit = sqlite3_bind_text(statement, 7, job.Salary_Compensation.c_str(), -1, SQLITE_STATIC);
    exit = sqlite3_bind_text(statement, 8, job.deadline.c_str(), -1, SQLITE_STATIC);
    exit = sqlite3_bind_text(statement, 9, job.description.c_str(), -1, SQLITE_STATIC);
  

    if (exit != SQLITE_OK) {
        cerr << "Error binding parameters: " << sqlite3_errmsg(DB) << endl;
        sqlite3_finalize(statement);
        sqlite3_close(DB);
        return;
    }

    exit = sqlite3_step(statement);

    if (exit != SQLITE_DONE) {
        cerr << "Error inserting data: " << sqlite3_errmsg(DB) << endl;
    }

    sqlite3_finalize(statement);
    sqlite3_close(DB);
}

string processDescription(const string& input) {
    // Replace <br> with newline character
    string processed = regex_replace(input, regex("<br>"), "\n");

    // Remove <strong> tags
    processed = regex_replace(processed, regex("<strong>|</strong>"), " ");

    //// Remove <a> tags with specified onclick attribute
    processed = regex_replace(processed, regex("<a[^>]*onclick=\"[^\"]*\"[^>]*>"), "");

    return processed;
}

string processDate(const string& input) {
    // Use stringstream to parse day, month, and year
    stringstream ss(input);
    string day, month, year;

    getline(ss, day, '/');
    getline(ss, month, '/');
    getline(ss, year);
    // Construct the new date string in year-month-day format
    string processed = year + "-" + month + "-" + day;
    return processed;
}

// g++ your ->"program name" .\src\sqlite3.o -IC:\sqlite3
//& .\"a.exe"

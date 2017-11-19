#include <iostream>
#include <regex>
#include <windows.h>
#include <io.h>

using namespace std;
string word_reader(string str, int position){
    string word = "";
    while (isalnum(str[position])) {
        word += str[position];
        position++;
    }
    return word;
}
class database {
public:
    char name[256];
};
class table {
public:
    string name;
    vector <string> name_of_column;
    int rows_amount;
};

class request{
public:
    database base;
    string request_line;
    int type_of_request = 0;
    void RequestSet() {
        getline(cin,request_line);
    }
    void type_identifier() {
        regex create_database_mask("create database [a-zA-Z0-9]+;");
        regex use_mask("use [a-zA-Z0-9]+;");
        regex create_table_mask("create table [a-zA-Z0-9]+;");
        regex select_mask("select [a-zA-Z0-9\\*]+[, [a-zA-Z0-9\\*]+]* from [a-zA-Z0-9]+;");
        regex insert_mask("insert into [a-zA-Z0-9]+\\([a-zA-Z0-9]+[, [a-zA-Z0-9]+]*\\) values [a-zA-Z0-9]+\\.[a-zA-Z0-9]+[, [a-zA-Z0-9]+\\.[a-zA-Z0-9]+]*;");
        regex delete_mask("delete");
        type_of_request =   1 * ((int) regex_match(request_line,create_database_mask)) +
                            2 * ((int) regex_match(request_line,use_mask)) +
                            3 * ((int) regex_match(request_line,create_table_mask)) +
                            4 * ((int) regex_match(request_line,select_mask)) +
                            5 * ((int) regex_match(request_line,insert_mask)) +
                            6 * ((int) regex_match(request_line,delete_mask));
    };
    void create_database() {
        strcpy(base.name,word_reader(request_line,16).c_str());
        mkdir(base.name);
    };
    
};


int main() {
    request str;
    str.RequestSet();
    str.type_identifier();
    cout << str.type_of_request;
    str.create_database();
    system("pause");
    return 0;
}
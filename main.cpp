#include <iostream>
#include <regex>
#include <windows.h>
#include <io.h>
#include <cstring>
#include <cstdio>
#include <fstream>
using namespace std;

void setpos(int x, int y) {
    COORD k;
    k.X = x;
    k.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), k);
}
void horiz_border_printer(int n){
    cout << "+";
    for (int i=1;i<n-1;i++){
        cout << "-";
    }
    cout << "+\n";
}

char *word_reader(string str, int position){
    int i = 0;
    char *word =(char*) malloc(sizeof(char)*(i+1));
    while (isalnum(str[position])) {
        word[i]= str[position];
        position++;
        i++;
        word =(char*) realloc(word, sizeof(char)*(i+1));
    }
    word[i]='\0';
    return word;
}
void string_table_printer(vector <string> &vector1){
    int max_size = -1, tmp_len;
    for (int i = 0; i < vector1.size(); i++) {
        if ((tmp_len = vector1[i].length()) > max_size){
            max_size = tmp_len;
        }
    }
    horiz_border_printer(max_size + 4);
    for (int i = 0; i < vector1.size(); i++) {
        cout << "| " << vector1[i];
        for (int j=0;j<(max_size-vector1[i].length())/8+1;j++){
            cout << "\t";
        }
        cout << "|\n";
    }
    horiz_border_printer(max_size + 4);
    cout << "\n" << vector1.size() << " rows in set.\n\n";
}
void ints_to_strings(vector <int> &vector1){
    vector <string> vector2;
    for (int i=0;i<vector1.size();i++){
        vector2[i]=to_string(vector1[i]);
    }
}
void complex_table_printer(vector <vector <string>> &vector1){
    for (int j=0;j<vector1[1].size();j++) {
        for (int i = 0; i < vector1.size(); i++) {

        }
    }
}

class caret_position{
public:
    struct cur {
        int x;
        int y;
    };
    struct set_cur_pos
};
class table {
public:
    char name[256];
    vector <vector <string>> name_of_column;
    int rows_amount;
};
class database {
public:
    char name[256], current_name[256];
    table current_table;
    vector<string> available_names;
};
class request{
public:
    database base;
    string request_line;
    bool in_list_flag = 0;
    int type_of_request = 0;
    void RequestSet() {
        getline(cin,request_line);
    }
    void type_identifier() {
        regex create_database_mask("create database [a-zA-Z0-9]+;");
        regex show_databases_mask("show databases;");
        regex use_mask("use [a-zA-Z0-9]+;");
        regex create_table_mask("create table [a-zA-Z0-9]+\\([a-z0-9]+ [a-z]\\([0-9]+\\)[, [a-zA-Z0-9]+ [DEFAULT default_value] [AUTO_INCREMENT] [PRIMARY KEY];");
        regex select_mask("select [a-zA-Z0-9\\*]+[, [a-zA-Z0-9\\*]+]* from [a-zA-Z0-9]+;");
        regex insert_mask("insert into [a-zA-Z0-9]+\\([a-zA-Z0-9]+[, [a-zA-Z0-9]+]*\\) values [a-zA-Z0-9]+\\.[a-zA-Z0-9]+[, [a-zA-Z0-9]+\\.[a-zA-Z0-9]+]*;");
        regex delete_mask("delete");
        type_of_request =   1 * ((int) regex_match(request_line,create_database_mask)) +
                            2 * ((int) regex_match(request_line,show_databases_mask)) ;

    };
    void create_database() {
        strcpy(base.name,word_reader(request_line,16));
        strcat(base.name,".db");
        for (int i=0;i<base.available_names.size();i++){
            if ((in_list_flag == 0)&&(base.name==base.available_names[i])){
                in_list_flag = 1;
                break;
            }
        }
        string tmp_str = base.name;
        if (in_list_flag == 0){
                base.available_names.insert(base.available_names.end(),tmp_str);
                cout << base.name << " was added to the db-list." << endl;
        };
        if (mkdir(base.name) != 0 ) {
            cout << "Can't create database." << endl;
        }
        else {
            cout << base.name << " was created." << endl;
        };
    };
    void show_databases(){
        string_table_printer(base.available_names);
        /*int max_size = -1, tmp_len;
        for (int i = 0; i < base.available_names.size(); i++) {
            if ((tmp_len = base.available_names[i].length()) > max_size){
                max_size = tmp_len;
            }
        }
        horiz_border_printer(max_size + 4);
        for (int i = 0; i < base.available_names.size(); i++) {
            cout << "| " << base.available_names[i];
            for (int j=0;j<(max_size-base.available_names[i].length())/8+1;j++){
                cout << "\t";
            }
            cout << "|\n";
        }
        horiz_border_printer(max_size + 4);
        cout << "\n" << base.available_names.size() << " rows in set.\n\n";*/
    }
    void use(){
        strcpy(base.current_name,word_reader(request_line,4));
    }
    void create_table() {
        strcpy(base.current_table.name,word_reader(request_line,14));
    }

};

int one_call() {
    request str;
    fstream list;
    list.open("list.bd", ios::in | ios::app);
    if (list.bad()){
        cout << "Error of creating parameters file.\n";
        exit(-1);
    };
    string buffer_str;
    while (list >> buffer_str) {
        str.base.available_names.push_back(buffer_str);
    }
    list.close();
    str.RequestSet();
    str.type_identifier();
    switch (str.type_of_request) {
        case 0 : cout << "Wrong request.\n"; break;
        case 1 : str.create_database(); break;
        case 2 : str.show_databases(); break;
        case 3 : str.create_table();break;
        case 4 :
        default: cout << "Фича\n";
    };
    ofstream list_out;
    list_out.open("list.bd");
    for (int i=0;i<str.base.available_names.size();i++){
            list_out << str.base.available_names[i] << " ";
        }
    list.close();
    return 0;
}
int main(){
    int status;
    do {
        status = one_call();
    }
    while (status == 0);
    system("pause");
    return 0;
}

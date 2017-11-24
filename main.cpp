#include <iostream>
#include <regex>
#include <windows.h>
#include <io.h>
#include <cstring>
#include <cstdio>
#include <fstream>
using namespace std;

bool check_in_list(vector <string> &available_names, string &current){
    bool in_list_flag = 0;
    for (int i=0;i<available_names.size();i++){
        if ((in_list_flag == 0)&&(available_names[i]==current)){
            in_list_flag = 1;
            break;
        }
    }
    return in_list_flag;
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
        for (int j=0;j<max_size-vector1[i].length();j++){
            cout << " ";
        }
        cout << " |\n";
    }
    horiz_border_printer(max_size + 4);
    cout << "\n" << vector1.size() << " rows in set.";
}
void ints_to_strings(vector <int> &vector1){
    vector <string> vector2;
    for (int i=0;i<vector1.size();i++){
        vector2[i]=to_string(vector1[i]);
    }
}
void complex_table_printer(vector <vector <string>> &vector1){
    vector <int> row_width(vector1.size());
    int max_size = -1, tmp_len;
    for (int j=0;j<vector1.size();j++) {
        for (int i = 0; i < vector1[j].size(); i++) {
            if ((tmp_len = vector1[j][i].length()) > max_size){
                max_size = tmp_len;
            }
            row_width[j] = max_size;
        }
    }
    for (int j=0;j<vector1[0].size();j++) {
        for (int i = 0; i < vector1.size(); i++) {
            cout << "| " << vector1[i][j] << " ";
            for (int k=0;k < (row_width[i]-vector1[i][j].length())/8+1;k++){
                cout << "\t";
            }
        }
        cout << "|\n";
    }
}

class table {
public:
    char name[256];
    vector <string> types_of_columns;
    vector <string> names_of_columns;
    int columns_amount, rows_amount;
};
class database {
public:
    char name[256], current_name[256];
    table current_table;
    int columns_amount;
    vector <string> available_names;
    vector <string> available_tables;
    vector <string> list_of_available_tables;
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
        regex show_databases_mask("show databases;");
        regex use_mask("use [a-zA-Z0-9]+;");
        regex create_table_mask("create table [a-zA-Z0-9]+\\([a-zA-Z]+ [a-zA-Z]+\\([0-9]+\\)\\);");
        regex select_mask("select [a-zA-Z0-9\\*]+[, [a-zA-Z0-9\\*]+]* from [a-zA-Z0-9]+;");
        regex insert_mask("insert into [a-zA-Z0-9]+\\([a-zA-Z0-9]+[, [a-zA-Z0-9]+]*\\) values [a-zA-Z0-9]+\\.[a-zA-Z0-9]+[, [a-zA-Z0-9]+\\.[a-zA-Z0-9]+]*;");
        regex delete_mask("delete");
        type_of_request =   1 * ((int) regex_match(request_line,create_database_mask)) +
                            2 * ((int) regex_match(request_line,show_databases_mask)) +
                            3 * ((int) regex_match(request_line,use_mask)) +
                            4 * ((int) regex_match(request_line,create_table_mask));
    };
    void create_database() {
        strcpy(base.name,word_reader(request_line,16));
        string tmp_str = base.name;
        if (check_in_list(base.available_names,tmp_str) == 0){
                base.available_names.insert(base.available_names.end(),tmp_str);
                cout << base.name << " was added to the db-list." << endl;
        };
        if (mkdir(base.name) != 0 ) {
            cout << "Can't create database.";
            exit(-1);
        }
        else {
            cout << base.name << " was created.";
        };
        ofstream tables_list;
        tables_list.open(strcat(base.name,"/list"));
        if (!tables_list) {
            cout << "Error";
            exit(-1);
        }
        tables_list << "0" << " ";
        tables_list.close();
    };
    void show_databases(){
        string_table_printer(base.available_names);
    }
    void use(){
        string tmp(word_reader(request_line,4));
        if (check_in_list(base.available_names,tmp)){
            strcpy(base.current_name,tmp.c_str());
            cout << "Done.";
        }
        else {
            cout << "Base doesn't exist.";
        }
    }
    void create_table() {
        ifstream tables_list;
        tables_list.open(strcat(base.current_name,"/list"));
        if (!tables_list) {
            cout << "Error 1";
            exit(-1);
        }
        tables_list >> base.columns_amount;
        int i = 0;
        while (tables_list >> base.list_of_available_tables[0]){
            i++;
        }
        tables_list.close();
        strcpy(base.current_table.name,word_reader(request_line,14));
        string tmp = base.current_table.name;
        if (check_in_list(base.available_tables,tmp)){
            cout << "Already exists.";
            exit(-1);
        }
        FILE *table;
        if (!(table = fopen(base.current_table.name,"w"))){
            cout << "Error 2";
            exit(-1);
        }
        fputc(base.current_table.columns_amount,table);

    }
    void select(){
        int colomns, strings;
        vector <vector <string>> mas (colomns, vector <string>(10));
        string k = "gof";
        for (int j=0;j<5;j++) {
            for (int i = 0; i < 10; i++) {
                //mas[j][i] = to_string(k);
            }
        }
        complex_table_printer(mas);
        cout << endl;
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
        case 3 : str.use();break;
        case 4 : str.create_table(); break;
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
        cout << "\n\n";
    }
    while (status == 0);
    system("pause");
    return 0;
}

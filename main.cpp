#include <iostream>
#include <regex>
#include <windows.h>
#include <io.h>
#include <cstring>
#include <cstdio>

using namespace std;
string *char_to_string(char *str){
    for (int i =0; i< str.size();

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
class table {
public:
    char name[256];
    vector <vector <char>> name_of_column;
    int rows_amount;
};
class database {
public:
    char name[256], current_name[256];
    table current_table;
    vector <string> available_names;
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
        regex use_mask("use [a-zA-Z0-9]+;");
        regex create_table_mask("create table [a-zA-Z0-9]+\\([a-z0-9]+ [a-z]\\([0-9]+\\)[, [a-zA-Z0-9]+ [DEFAULT default_value] [AUTO_INCREMENT] [PRIMARY KEY];");
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
        strcpy(base.name,word_reader(request_line,16));
        strcat(base.name,".db");
        for (int i=0;i<base.available_names.size();i++){
            if ((in_list_flag == 0)&&(base.name==base.available_names[i])){
                in_list_flag = 1;
                break;
            }
        }
        string str_tmp;
        if (in_list_flag == 0){
            base.available_names[base.available_names.size()+1] = char_to_string(&base.name);
        }
        if (mkdir(base.name) != 0 ) {
            cout << "Can't create database with name " << base.name;
        }
    };
    void use(){
        strcpy(base.current_name,word_reader(request_line,4));
    }
    void create_table() {
        strcpy(base.current_table.name,word_reader(request_line,14));

    }

};
int main() {
    request str;
    FILE *list;
    if ((list = fopen("list.bd", "r+")) == NULL){
        if ((list = fopen("list.bd", "w")) == NULL){
            cout << "Error of creating parameters file.\n";
            exit(-1);
        };
    };
    char buffer[256];
    int i = 0;
    while ((fgets(buffer,256,list)) != NULL) {
        str.base.available_names[i] = buffer;
        i++;
    }
    fclose(list);
    str.RequestSet();
    str.type_identifier();
    switch (str.type_of_request) {
        case 0 : cout << "Wrong request.\n"; break;
        case 1 : str.create_database(); break;
        case 2 : str.use(); break;
        case 3 : str.create_table();break;
        default: cout << "Фича";


    };
    cout << "\n" << str.type_of_request;
    for (i = 0; i < str.base.available_names.size();i++){
        cout << str.base.available_names[i];
    };
    system("pause");
    return 0;
}
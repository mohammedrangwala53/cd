#include <iostream> //for input output
#include <fstream>  //for file management
#include <sstream>
#include <unordered_map>  //for hash table based map for fast lookup
#include <string>  //for using the stirng class

using namespace std; //we can write string directly instead of std::string

const int MAX = 100;    //constant which allows max no. of statements for now the max is 100

string variables[MAX];      //these arrays store variable names
string expressions[MAX];    //these store the expressions
int statement_count = 0;    //it checks the number of valid statements parsed

bool read_files(const string& filename){   //Reads the input file line by line and fills the vars[] and exprs[] arrays.
    ifstream infile(filename); //opens file to read
    string line;

    if (!infile.is_open()) //checks if file was successfully opened
    {
        cerr << "Could not open file " << filename << endl;
        return false;
    }

    statement_count = 0;
    while (getline(infile, line) && statement_count < MAX) //Reads one line at a time, upto MAX
    {
        if (line.back() == ';')  //Removes a trailing ; at the end of the line.
        {
            line.pop_back();
        }
        while (!line.empty() && (line.back() == ' ' || line.back() == '\t'))
        {
            line.pop_back(); //Trims whitespace at the end.
        }

        int eq_pos = line.find('='); //finds the position of = and splits into variables and expression
        if (eq_pos == string::npos) continue;

        string var = line.substr(0, eq_pos);    //Extracts the left (variable) and right (expression) parts. 
        string expr = line.substr(eq_pos + 1);  //Extracts the left (variable) and right (expression) parts.
        
        while (!var.empty() && (var.back() == ' ' || var.back() == '\t')) //trims whites spaces from the end of var and start of the expressions
        {
            var.pop_back(); //trims whites spaces from the end of var and start of the expressions
        }
        
        while (!expr.empty() && (expr.back() == ' ' || expr.back() == '\t')) //trims whites spaces from the end of var and start of the expressions
        {
            expr.erase(0, 1);   //trims whites spaces from the end of var and start of the expressions
        }
        
        if (!var.empty() && !expr.empty())      //stores valid statements in arrays
        {
            variables[statement_count] = var;
            expressions[statement_count] = expr;
            statement_count++;
        }
        
    }
    infile.close();
    return statement_count > 0;
}

string replace_all(const string& str, const string& old_str, const string& new_str){  //replaces every occurrences of old whith new strings in string
    string result = str;
    size_t pos = 0;
    while ((pos = result.find(old_str, pos)) != string::npos)   //a loop that finds all matches and replaces them
    {
        result.replace(pos, old_str.length(), new_str);
        pos += new_str.length();
    }
    return result;
}

void performCSE(){
    unordered_map<string, string> exprtoVar; //maps expressions to var, unique expressions to the first variable that used them.
    string output[statement_count]; //store final statements
    int output_count = 0;

    // process statements and build mappings
    for (int i = 0; i < statement_count; i++)
    {
        string var = variables[i];
        string expr = expressions[i];

        if (exprtoVar.find(expr) != exprtoVar.end())  //check if expression exists, if yes it's redundant
        {
            string prev_var = exprtoVar[expr];  //expression is redundant, use the previous variable
            for (int j = i+1 ; j < statement_count; j++) // Update all later expressions that use this variable 
            {
                expressions[j] = replace_all(expressions[j], var, prev_var);    //Replaces usage of redundant variable in future expressions.

            }
        }
        else //If expression is new, store it in the map and keep the statement.
        {
            exprtoVar[expr] = var;
            output[output_count] = var + "=" + expr + ";";
            output_count++;
        }
    }

        cout << "After elimination:" << endl << endl;
        for (int i = 0; i < output_count; i++)
        {
            cout << output[i] << endl;      //displays the optimized code.
        }
     
}

int main(){
    string filename = "input.txt"; //input filename is hardcoded
    if (!read_files(filename)){
        cout << "No statements to process or the file is empty" << endl; //if reading fails, exit
        return 1;
    }

    cout << "Before elimination:" << endl << endl;
    for (int i = 0; i < statement_count; i++)
    {
        cout << variables[i] << "=" << expressions[i] << ";" << endl;
    }
    cout << endl;

    performCSE();

    return 0;
    
}


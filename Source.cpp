#include <iostream>
#include <conio.h>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <stack>
using namespace std;



void read_parse_table(vector<vector<string>>& table, ifstream& reader, vector<string>&t, vector<string>&nt)
{
	table.resize(26);
	for (int i = 0; i < 26; i++) {
		table[i].resize(38);
	}
	for (int i = 0; i < 26; i++)
	{
		string temp;
		string store;
		std::getline(reader, temp);
		int col = 0;
		for (int x = 0; x < temp.size(); x++) // picks all columns 
		{
			if (i == 0 && x == 0) {// [0][0]
				++col;
				continue;
			}
			else if (temp[x] == ',')
				continue;
			else if (temp[x + 1] == ',' && x < temp.size()) //  , is in the next 
			{
				if (temp[x] == 34 && temp[x + 1] == 44 && temp[x + 2] == 34) {
					store.push_back(temp[x + 1]);
					x += 2;
				}
				else {
					store.push_back(temp[x]);
				}
			}
			else
			{
				do
				{
					store.push_back(temp[x]);
					++x;
					if (x >= temp.size())
					{
						break;
					}
				} while (temp[x + 1] != '\n' && temp[x + 1] != ',');
				store.push_back(temp[x]);
			}
			if (i == 0) 
			{
				t.push_back(store);
			}
			if (col == 0)
			{
				nt.push_back(store);
			}
			table[i][col++] = store;
			store.clear();
		}
		temp.clear();
	}
}

void read_input(vector<vector<string>>& input, ifstream& reader_in) {
	while (!reader_in.eof())
	{
		string temp;
		vector<string> temp2;
		reader_in >> temp;

		temp.erase(temp.size() - 1);
		temp.erase(temp.begin());

		temp2.push_back(temp);

		temp.clear();
		reader_in >> temp;
		temp.erase(temp.size() - 1);

		temp2.push_back(temp);

		input.push_back(temp2);

	}

}

void print_parse_table(vector<vector<string>> table)
{
	for (int i = 0; i < table.size(); i++) {
		for (int j = 0; j < table[i].size(); j++) {
			cout << setw(3) << table[i][j];
		}
		cout << endl;
	}
}

void load_grammar(vector<vector<string>>& A, ifstream& reader)
{
	A.resize(57); // temp size
	int idx = 0;
	string temp;

	vector<string> str;
	bool first = true;
	while (getline(reader, temp))
	{
		for (int i = 0; i < temp.size(); i++)
		{
			if (i != 0)
				first = false;
			string b;
			if (temp[i] == '*')
				b.append("*");
			else if (temp[i] == '-')
				b.append("-");
			else if (temp[i] == '+')
				b.append("+");
			else if (temp[i] == ')')
				b.append(")");
			else if (temp[i] == '(')
				b.append("(");
			else if (temp[i] == '}')
				b.append("}");
			else if (temp[i] == '{')
				b.append("{");
			else if (temp[i] == ':') {
				b.append(":");
			}
			else if (temp[i] == '~') {
				b.append("~");
			}
			else if (temp[i] == '\t')
				continue;
			else if (temp[i + 1] == '\t')
			{
				b.append(1, temp[i]);
			}
			else if (temp[i + 1] != '\t' && temp[i + 1] != '\0')
			{
				while (temp[i] != '\t' && temp[i] != '\0')
				{
					b.append(1, temp[i++]);
				}
			}
			A[idx].push_back(b);

		}
		idx++;
		first = true;
	}
	A.resize(idx);
}

bool is_in_vec(vector<string> vec, string str) {

	if (std::find(vec.begin(), vec.end(), str) != vec.end()) {
		return true;
	}
	return false;
}


void print_vec_str(vector<string>a) {
	for (int i = 0; i < a.size(); i++) {
		cout << a[i] << "  ";
	}
	cout << endl << endl << endl;
}

void print_2d_string(vector<vector<string>> A) {
	for (int i = 0; i < A.size(); i++) {
		for (int j = 0; j < A[i].size(); j++) {
			cout << A[i][j] << "  ";
		}
		cout << endl;
	}
}

void find_in_table(vector<vector<string>> table, int& ni, int& ti, string top, string  i, string token)
{
	for (int a = 0; a < table.size(); a++) {
		if (table[a][0] == top) {
			ni = a;
			break;
		}
	}
	for (int a = 0; a < table[0].size(); a++) {
		if (table[0][a] == i || table[0][a] == token)
		{
			ti = a;
			break;
		}
	}
}

void insert_grammar_to_stack(string table_entry, vector<vector<string>> grammar, stack<string>& main)
{
	int index = stoi(table_entry);
	--index;
	int i = grammar[index].size();
	--i;
	main.pop();
	while (i > 0) {
		main.push(grammar[index][i--]);
	}

}

void print_stack(stack<string>s, string input, ofstream&writer)
{
	cout << "\n------------" << input << "-----------\n";
	stack<string>temp;
	while (!s.empty())
	{
		temp.push(s.top());
		s.pop();
	}
	while (!temp.empty())
	{
		writer << temp.top() << "      " ;
		cout << temp.top() << "      ";
		temp.pop();
	}
	writer << endl;
}

void parsing(vector<vector<string>>  table, vector<vector<string>> grammar, vector<vector<string>> tokens, vector<string>terminals, ofstream & writer)
{
	stack<string> main;
	string dollar = "$";
	vector<string> dollar1;
	dollar1.push_back(dollar);
	dollar1.push_back(dollar);
	tokens.push_back(dollar1);
	main.push("$");
	main.push(grammar[0][0]); //  start symbol
	int ind = 0;
	string i = tokens[ind][0];
	string token = tokens[ind][1];
	while (main.top() != dollar) {
		string t = main.top();  // t : top of the stack
		if (ind == tokens.size() && t != dollar) 
		{
			cout << "\nTHERE WERE ERRORS - PARSING SUCESSFUL\n";
			cout << "\nTHERE WERE ERRORS - PARSING SUCESSFUL\n";
			break;
		}
		if (t == "~")
		{
			main.pop();
			t = main.top();
		}
		if (tokens[ind][1] == t || tokens[ind][0] == t) // if stack and input match then move the marker of the input
		{
			if (main.top() != dollar) {
				main.pop();
				t = main.top();
				++ind;
			}
			if (t == dollar and tokens[ind][0] == dollar) {
				writer << "\nPARSING SUCCESSFUL!\n";

				cout << "\nPARSING SUCCESSFUL!\n";
				break;
			}
			if (ind < tokens.size()) 
			{
			//	print_stack(main, tokens[ind][0], writer); //print the state of the stack
				i = tokens[ind][0];
				token = tokens[ind][1];
			}
		}
		if (is_in_vec(terminals, t) and (is_in_vec(terminals, tokens[ind][0])|| is_in_vec(terminals, tokens[ind][1])) and  (t!=tokens[ind][0] and t!=tokens[ind][1])) // Panic mode recovery, if we get terminals on stack and input that do not match
		{
			writer << "\n TERMINALS NOT MATCHING : " << t << " and  " << tokens[ind][0] << endl;
			cout << "\n TERMINALS NOT MATCHING : " << t << " and  " << tokens[ind][0] << endl;
			if (ind < tokens.size()-1)
			{
				ind++;
				i = tokens[ind][0];
				token = tokens[ind][1];
			}
			continue;
		}
		if (tokens[ind][1] == t || tokens[ind][0] == t) // symbols same again
			continue;
		int ni = -1, ti = -1;
		find_in_table(table, ni, ti, t, i, token); // find index of the terminal and nonterminal
		string table_entry = table[ni][ti]; // get entry from the table
		if (table_entry == "-") // panic mode recovery
		{
			writer << "\n TABLE ENTRY NULL at " << table[ni][0] << "   " << table[0][ti] << endl;
			cout << "\n TABLE ENTRY NULL at " << table[ni][0] << "   " << table[0][ti] << endl;
			if (ind < tokens.size()-1)
			{
				ind++;
				i = tokens[ind][0];
				token = tokens[ind][1];
			}
			else {
				cout << "\nPARSING UNSUCCESSFUL!\n";
				break;
			}
			continue;
		}
		insert_grammar_to_stack(table_entry, grammar, main); // pop and add new grammar from parse table
		print_stack(main, tokens[ind][0], writer); //print the state of the stack

	}
}

int main() {
	vector<vector<string>> table;
	vector <vector<string>> tokens;
	vector <vector<string>> tokens2;
	vector<vector<string>> grammar;
	vector<string> terminal;
	vector<string> non_terminal;


	ifstream reader("final3.csv");
	ifstream reader_in("input5.txt");
	ifstream reader_grammar("grammar.txt");

	ofstream writer1("output1.txt");
	ofstream writer2("output2.txt");

	read_parse_table(table, reader, terminal, non_terminal);
	read_input(tokens, reader_in);
	load_grammar(grammar, reader_grammar);

	cout << "----------------------parsing-------------------\n";
	parsing(table, grammar, tokens, terminal, writer2);
	return _getch();
}
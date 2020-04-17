#pragma once
#include <map>
#include <set>
#include <vector>
#include <string>
#include <fstream>

class Symbol {
	std::string sym;
	bool terminal;
public:
	Symbol(std::string sym, bool terminal);
	bool is_terminal();
	std::string get_sym();
	bool get_type();
};

typedef std::map<std::pair<std::string, std::string>, std::vector<Symbol>> Table;

class PredTable {
	const std::set<std::string> ralation_operator_syms = std::set<std::string>({ "<", "<=", "=", ">=", "<>", ">" });
	const std::set<std::string> addition_syms = std::set<std::string>({ "+", "-" });
	const std::set<std::string> multiplication_syms = std::set<std::string>({ "*", "/" });
	Table tbl;
public:
	PredTable();
	Table get_table();
};

class Node {
	int num;
	std::string name;
	bool terminal;
	bool error;
	std::vector<Node*> children;
public:
	Node(int num, std::string name, bool terminal);
	void set_error();
	void add_child(Node *child);
	int get_num();
	std::string get_name();
	bool get_terminal();
	bool get_error();
	std::vector<Node*> get_children();
	~Node();
};

class Storage {
	std::ifstream text;
	std::vector<Node*> tree;
	int current_node;
	Table tbl;
	std::vector<std::string> symbols;
	void parse(std::string nonterminal, std::string first, Node *root);
	void make_graph();
public:
	Storage(const char* filename);
	bool parse();
	bool handle();
	~Storage();
};

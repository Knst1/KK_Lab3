#include "pch.h"
#include "../parser/parser.h"
#include <algorithm>
#include <fstream>
#include <regex>

#define ADD(K, V) insert(pair<pair<string, string>, vector<Symbol>>(pair<string, string>K, vector<Symbol>V))

using namespace std;

Symbol::Symbol(string sym, bool terminal) : sym(sym), terminal(terminal) {}

bool Symbol::is_terminal() {
	return terminal;
}

string Symbol::get_sym() {
	return sym;
}

bool Symbol::get_type() {
	return terminal;
}

PredTable::PredTable() {
	tbl.ADD(("blok", "begin"), ({ Symbol("begin", true), Symbol("operators", false), Symbol("end", true) }));
	tbl.ADD(("operators", "identifier"), ({ Symbol("identifier", true), Symbol(":=", true), Symbol("expression", false), Symbol("operators_1", false) }));
	tbl.ADD(("operators_1", ";"), ({ Symbol(";", true), Symbol("operator", false), Symbol("operators_1", false) }));
	tbl.ADD(("operators_1", ""), ({ Symbol("", false) }));
	tbl.ADD(("operator", "identifier"), ({ Symbol("identifier", true), Symbol(":=", true), Symbol("expression", false) }));
	tbl.ADD(("expression", "const"), ({ Symbol("const", true), Symbol("expression_4", false) }));
	tbl.ADD(("expression", "identifier"), ({ Symbol("identifier", true), Symbol("expression_4", false) }));
	tbl.ADD(("expression", "("), ({ Symbol("(", true), Symbol("arithmetic_expression", false), Symbol(")", true), Symbol("expression_4", false) }));
	tbl.ADD(("expression_1", ""), ({ Symbol("", false) }));
	tbl.ADD(("expression_3", ""), ({ Symbol("", false) }));
	tbl.ADD(("expression_4", ""), ({ Symbol("", false) }));
	tbl.ADD(("arithmetic_expression", "const"), ({ Symbol("term", false), Symbol("arithmetic_expression_2", false) }));
	tbl.ADD(("arithmetic_expression", "identifier"), ({ Symbol("term", false), Symbol("arithmetic_expression_2", false) }));
	tbl.ADD(("arithmetic_expression", "("), ({ Symbol("term", false), Symbol("arithmetic_expression_2", false) }));
	tbl.ADD(("arithmetic_expression_2", ""), ({ Symbol("", false) }));
	tbl.ADD(("term", "const"), ({ Symbol("const", true), Symbol("term_2", false) }));
	tbl.ADD(("term", "identifier"), ({ Symbol("identifier", true), Symbol("term_2", false) }));
	tbl.ADD(("term", "("), ({ Symbol("(", true), Symbol("arithmetic_expression", false), Symbol(")", true), Symbol("term_2", false) }));
	tbl.ADD(("term_2", ""), ({ Symbol("", false) }));
	tbl.ADD(("faktor", "identifier"), ({ Symbol("identifier", true) }));
	tbl.ADD(("faktor", "const"), ({ Symbol("const", true) }));
	tbl.ADD(("faktor", "("), ({ Symbol("(", true), Symbol("arithmetic_expression", false), Symbol(")", true) }));
	for (string sym : ralation_operator_syms)
	{
		tbl.ADD(("expression_1", sym), ({ Symbol(sym, true), Symbol("arithmetic_expression", false) }));
		tbl.ADD(("expression_3", sym), ({ Symbol(sym, true), Symbol("arithmetic_expression", false) }));
		tbl.ADD(("expression_4", sym), ({ Symbol(sym, true), Symbol("arithmetic_expression", false) }));
	}
	for (string sym : addition_syms)
	{
		tbl.ADD(("expression_3", sym), ({ Symbol("arithmetic_expression_1", false), Symbol("expression_1", false) }));
		tbl.ADD(("expression_4", sym), ({ Symbol("arithmetic_expression_1", false), Symbol("expression_1", false) }));
		tbl.ADD(("arithmetic_expression_1", sym), ({ Symbol(sym, true), Symbol("term", false), Symbol("arithmetic_expression_2", false) }));
		tbl.ADD(("arithmetic_expression_2", sym), ({ Symbol("arithmetic_expression_1", false) }));
	}
	for (string sym : multiplication_syms)
	{
		tbl.ADD(("expression_4", sym), ({ Symbol("term_1", false), Symbol("expression_3", false) }));
		tbl.ADD(("term_1", sym), ({ Symbol(sym, true), Symbol("faktor", false), Symbol("term_2", false) }));
		tbl.ADD(("term_2", sym), ({ Symbol("term_1", false) }));
	}
}

Table PredTable::get_table() { return tbl; }

Node::Node(int num, string name, bool terminal) : num(num), name(name), terminal(terminal), error(false)
{ }

void Node::set_error()
{
	error = true;
}

void Node::add_child(Node *child)
{
	children.push_back(child);
}

int Node::get_num()
{
	return num;
}

string Node::get_name()
{
	return name;
}

bool Node::get_terminal()
{
	return terminal;
}

bool Node::get_error()
{
	return error;
}

vector<Node*> Node::get_children()
{
	return children;
}

Node::~Node()
{
	children.clear();
}


void Storage::parse(string nonterminal, string first, Node *root)
{
	if (first != *symbols.rbegin())
		if (tbl.find(pair<string, string>(nonterminal, "")) != tbl.end())
		{
			tree.push_back(new Node(current_node, "", true));
			root->add_child(tree[current_node]);
			current_node++;
			return;
		}
	string err;
	int num;
	for (Symbol sym : tbl[pair<string, string>(nonterminal, first)])
	{
		num = 0;
		err.clear();
		if (sym.get_type())
		{
			do {
				if (symbols.size() > 0)
					if (sym.get_sym() == symbols[symbols.size() - 1 - num])
					{
						if (num != 0)
						{
							if (err.size() > 0)
								err.pop_back();
							tree.push_back(new Node(current_node, err, sym.get_type()));
							root->add_child(tree[current_node]);
							tree[current_node]->set_error();
							root->set_error();
							current_node++;
							symbols.erase(symbols.end() - num, symbols.end());
							err.clear();
							num = 0;
						}
						tree.push_back(new Node(current_node, sym.get_sym(), sym.get_type()));
						root->add_child(tree[current_node]);
						current_node++;
						symbols.pop_back();
					}
					else
					{
						err = err + symbols[symbols.size() - 1 - num] + " ";
						num++;
					}
				else
					root->set_error();
			} while (0 < num && num < symbols.size());
		}
		else
		{
			do {
				if (symbols.size() > 0 && tbl.find(pair<string, string>(sym.get_sym(), symbols[symbols.size() - 1 - num])) == tbl.end() && tbl.find(pair<string, string>(sym.get_sym(), "")) == tbl.end())
				{
					err = err + symbols[symbols.size() - 1 - num] + " ";
					num++;
				}
				else
				{
					if (num != 0)
					{
						if (err.size() > 0)
							err.pop_back();
						tree.push_back(new Node(current_node, err, true));
						root->add_child(tree[current_node]);
						tree[current_node]->set_error();
						root->set_error();
						current_node++;
						symbols.erase(symbols.end() - num, symbols.end());
						err.clear();
						num = 0;
					}
					tree.push_back(new Node(current_node, sym.get_sym(), sym.get_type()));
					root->add_child(tree[current_node]);
					current_node++;
					if (symbols.size() == 0)
					{
						if (tbl.find(pair<string, string>(sym.get_sym(), "")) == tbl.end())
							tree[current_node - 1]->set_error();

						return;
					}
					else
					{
						parse(sym.get_sym(), *symbols.rbegin(), tree[current_node - 1]);
					}
				}
			} while (0 < num && num < symbols.size());
		}
		if (num > 0)
			root->set_error();
	}
}

void Storage::make_graph()
{
	ofstream tree_dot("tree.dot");
	if (!tree_dot.is_open())
		throw runtime_error("Не удалось открыть файл tree.dot для записи\n");
	tree_dot << "digraph G{" << endl << "node[shape=rectangle style=filled fillcolor=white fontsize=12];" << endl;
	for (Node* node : tree)
	{
		tree_dot << node->get_num() << "[label=\"" << node->get_name() << "\"";
		if (node->get_terminal())
			tree_dot << " fillcolor=lightgrey shape=ellipse";
		if (node->get_error())
			tree_dot << " fillcolor=coral";
		tree_dot << "];" << endl;
		for (Node* child : node->get_children())
			tree_dot << node->get_num() << "->" << child->get_num() << ";" << endl;
	}
	tree_dot << "}" << endl;
	tree_dot.close();
	system("dot -Tsvg tree.dot -o tree.svg");
}

Storage::Storage(const char* filename)
{
	tbl = PredTable().get_table();
	text.open(filename);
	if (!text.is_open())
		throw runtime_error("Не удалось открыть файл для чтения\n");
	text.seekg(0, ios::end);
	size_t size = text.tellg();
	string fstr(size + 1, ' ');
	text.seekg(0);
	text.read(&fstr[0], size);
	regex exp("[ \t\r\n]+");
	string str = regex_replace(fstr, exp, " ");
	if (str[0] == ' ')
		str.erase(0, 1);
	string delimiter = " ";
	size_t pos = 0;
	while ((pos = str.find(delimiter)) != string::npos) {
		symbols.push_back(str.substr(0, pos));
		str.erase(0, pos + delimiter.length());
	}
	reverse(symbols.begin(), symbols.end());
	current_node = 0;
}

bool Storage::parse()
{
	string err;
	size_t i, num, size = symbols.size();
	for (i = size - 1; i > -1; i--)
		if (symbols[i] != "begin")
			err = err + symbols[i] + " ";
	tree.push_back(new Node(current_node, "blok", false));
	current_node++;
	if (err.size() > 0)
	{
		tree.push_back(new Node(current_node, err, true));
		tree[0]->add_child(tree[current_node]);
		tree[0]->set_error();
		tree[current_node]->set_error();
		current_node++;
		err.clear();
		num = size - i - 1;
		symbols.erase(symbols.end() - num, symbols.end());
	}
	parse("blok", "begin", tree[0]);
	while (symbols.size() > 0)
	{
		err = err + *symbols.rbegin() + " ";
		symbols.pop_back();
	}
	if (err.size() > 0)
	{
		err.pop_back();
		tree.push_back(new Node(current_node, err, true));
		tree[0]->add_child(tree[current_node]);
		tree[0]->set_error();
		tree[current_node]->set_error();
		current_node++;
	}
	err.clear();
	make_graph();
	for (Node* node : tree)
		if (node->get_error())
			return false;
	return true;
}

bool Storage::handle()
{
	bool ans = parse();
	make_graph();
	return ans;
}

Storage::~Storage()
{
	text.close();
	for (auto i = 0; i < tree.size(); i++)
		delete tree[i];
	tree.clear();
}

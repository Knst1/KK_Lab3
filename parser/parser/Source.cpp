#include <iostream>
#include "parser.h"

using namespace std;

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "Russian");
	try {
		Storage s(argv[1]);
		bool ans = s.handle();
		if (ans)
			cout << "Ошибок нет" << endl;
		else
			cout << "Текст содержит ошибки" << endl;
	}
	catch (const exception& err) {
		cerr << err.what() << endl;
	}
	return 0;
}

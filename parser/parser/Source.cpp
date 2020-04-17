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
			cout << "������ ���" << endl;
		else
			cout << "����� �������� ������" << endl;
	}
	catch (const exception& err) {
		cerr << err.what() << endl;
	}
	return 0;
}

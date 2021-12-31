#include "LayoutMatrix.h"
using namespace std;
int main()
{	

	//LayoutMatrix test(10, 10);
	//test.print();
	//std::cout << test.encodeHash();
	//test.decodeHash();
	//test.print();
	//
	//test.randm();
	//test.print();
	//std::cout <<endl<<test.encodeHash()<<endl;
	//test.decodeHash();
	//test.print();
	try {
		for (size_t i = 1000; i < 10000; i+=500)
		{
			for (size_t j = 1; j < 10000; j++)
			{
				
				LayoutMatrix matr(i, j);
				matr.randm();
				matr.encodeHash();
				LayoutMatrix matr2;
				matr2.setHash(matr.getHash());
				matr2.decodeHash();
				cout << "\ni=" << i << "\tj=" << j;
				if (matr == matr2)
					cout << "\tcorrect";
				else
				{
					cout << "\tBAD";
					system("pause");
				}
			}
		}
	}
	catch (std::exception& ex)
	{
		cout << endl<<ex.what();
	}
	
	//LayoutMatrix lMatr(30, 30);
	//LayoutMatrix KMatr(30, 30);
	
	//lMatr.randm();
	//KMatr = lMatr;
	//lMatr.print();
	//if (lMatr == KMatr)
	//{
		//cout << "\nCORRECT!\n";
	//}
	//std::cout << std::endl<<lMatr.encodeHash();
	//lMatr.setHash(lMatr.getHash());
	//std::cout << std::endl;
	//lMatr.decodeHash().print();
	
	//matr.ones();
	//matr.print();
	//std::cout << std::endl;
	//matr.zeros();
	//matr.print();
	//std::cout << std::endl;
	//std::cout << std::endl;
	//std::cout << std::endl;
	//std::cout << std::endl;
	//bool flag = true;
	//for (size_t i = 0; i < matr.get_i_size()-i-1; i++)
	//{
	//	matr.setRange(i, i, matr.get_i_size() - i - 1, matr.get_i_size() - i - 1, flag);
	//	flag = !flag;
	//	matr.print();
	//	
	//}
	//
	//	matr.setRange(1, 1, matr.get_i_size() - 1, matr.get_j_size() - 1 , 1);
	//	matr.print();
	//	matr.resize(20, 20);
	//	matr.print();
	//	matr.setRange(1, 1, matr.get_i_size() - 1, matr.get_j_size() - 1, 1);
	//	matr.print();
	//
	//
	//
	//
	//
	//
	return 0;
}
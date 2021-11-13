#include "Matrix.h"
#include "Base64.h"
using namespace std;
int main()
{	  
	
	unsigned char a = 0b01111111;
	string code;
	code+= (char)0b10100111;
	code+= (char)0b00100010;
	code+= (char)0b10010010;
	std::cout << code<<endl;
	string encoded;
	encoded = base64_encode(code, false);
	cout << encoded << "   " << encoded.length();
	Bit_matrix  matr(10,25),matr1(10,25),matr2;
	string from_matr,check;
	for (size_t i = 0; i < 10 * 25; i++)
		check += '0';

	cout << check;
	//if (!matr2)
		//cout << "NO";
	for (size_t i = 0; i < 10; i++)
	{
		for (size_t j = 0; j < 25;j++)
		{
			matr.set(i, j, 1);
		}
	}
	matr.print();
	for (size_t i = 0; i < 10; i+=2)
		for (size_t j = 0; j < 25; j += 2)
		{
			matr1.set(i, j, 1);
			check[i + j * (i + 1)] = '1';
		}
	from_matr = get_str(matr1);
	check = base64_encode(check, false);
	from_matr = base64_encode(from_matr, false);
	cout << check.length() << "    " << from_matr.length();
	cout << endl;
	//matr1.print();
	cout << endl;
	(matr1 ^ matr).print();
	cout << endl << (matr1 ^ matr).unit_ratio();
	

	
	return 0;
}
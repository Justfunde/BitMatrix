#include "LayoutMatrix.h"

//Constrictors
LayoutMatrix::LayoutMatrix(const std::string& in_hash)
{
	hash = in_hash;
	RLE_decode(Base64_decode(in_hash));
}

LayoutMatrix::LayoutMatrix(const LayoutMatrix& matrix) :BitMatrix(matrix)
{
	hash = matrix.hash;
}

LayoutMatrix::LayoutMatrix(LayoutMatrix&& matrix) noexcept
{
	cols_count = matrix.cols_count;
	rows_count = matrix.rows_count;
	i_size = matrix.i_size;
	j_size = matrix.j_size;
	is_alloc = matrix.is_alloc;
	bitmap = matrix.bitmap;
	bit_count = matrix.bit_count;
	matrix.bitmap = nullptr;
	hash = std::move(matrix.hash);
}

LayoutMatrix::LayoutMatrix(BitMatrix&& matrix) noexcept
{
	cols_count = matrix.cols_count;
	rows_count = matrix.rows_count;
	i_size = matrix.i_size;
	j_size = matrix.j_size;
	is_alloc = matrix.is_alloc;
	bitmap = matrix.bitmap;
	bit_count = matrix.bit_count;
	matrix.bitmap = nullptr;
}


//Operators

LayoutMatrix& LayoutMatrix::operator=(const BitMatrix& matrix)
{
	if (!matrix.is_alloc)
		return *this;
	if (&matrix == this)
		return *this;
	reset();
	rows_count = matrix.rows_count;
	cols_count = matrix.cols_count;
	i_size = matrix.i_size;
	j_size = matrix.j_size;
	bit_count = matrix.bit_count;
	try {
		allocMem();
		cpyBitmap(bitmap, matrix.bitmap, cols_count, rows_count);
	}
	catch (std::exception& exception)
	{
		std::cerr << "\nStandard error:" << exception.what();
		this->reset();
	}
	return *this;
}

LayoutMatrix& LayoutMatrix::operator=(const LayoutMatrix& matrix)
{
	if (!matrix.is_alloc)
		return *this;
	if (&matrix == this)
		return *this;
	reset();
	hash = matrix.hash;
	rows_count = matrix.rows_count;
	cols_count = matrix.cols_count;
	i_size = matrix.i_size;
	j_size = matrix.j_size;
	bit_count = matrix.bit_count;
	try {
		allocMem();
		cpyBitmap(bitmap, matrix.bitmap, cols_count, rows_count);
	}
	catch (std::exception& exception)
	{
		std::cerr << "\nStandard error:" << exception.what();
		this->reset();
	}
	return *this;
}
	
LayoutMatrix& LayoutMatrix::operator=(BitMatrix&& matrix) noexcept
{
	reset();

	bitmap = matrix.bitmap;
	matrix.bitmap = nullptr;
	cols_count = matrix.cols_count;
	rows_count = matrix.rows_count;
	i_size = matrix.i_size;
	j_size = matrix.j_size;
	bit_count = matrix.bit_count;
	is_alloc = matrix.is_alloc;
	return *this;
}

LayoutMatrix& LayoutMatrix::operator=(LayoutMatrix&& matrix) noexcept
{
	reset();
	hash = std::move(matrix.hash);
	bitmap = matrix.bitmap;
	matrix.bitmap = nullptr;
	cols_count = matrix.cols_count;
	rows_count = matrix.rows_count;
	i_size = matrix.i_size;
	j_size = matrix.j_size;
	bit_count = matrix.bit_count;
	is_alloc = matrix.is_alloc;
	return *this;
}



BitMatrix LayoutMatrix::getMatrix() const
{
	return static_cast<BitMatrix>(*this);
}

std::string LayoutMatrix::getHash() const
{
	return hash;
}

void LayoutMatrix::setMatrix(const BitMatrix& matrix)
{
	*this = matrix;
	hash.clear();
}

void LayoutMatrix::setHash(const std::string& hash)
{
	this->hash = hash;
	reset();
}



std::string LayoutMatrix::encodeHash()
{
	if (!(*this)) return std::string();
	std::pair <std::string, double> RLE = RLE_encode();
	if (RLE.second < 1)
		hash = Base64_encode(makeString(), 0);
	else hash = Base64_encode(RLE.first, 1);
	return hash;
}

BitMatrix LayoutMatrix::decodeHash()
{
	RLE_decode(Base64_decode(hash));
	return static_cast<BitMatrix>(*this);
}

std::pair<std::string,double> LayoutMatrix::RLE_encode()
{
	assert(i_size != 0 && j_size != 0);
	std::string RLE_encoded;
	bool curr_value = get(0, 0);
	char key = 0;
	for (size_t i = 0; i < i_size; i++)
	{
		for (size_t j = 0; j < j_size; j++)
		{
			if (curr_value == get(i, j))
			{
				key++;
			}
			else {
				RLE_encoded += writeLastBit(key, curr_value);
				key = 1;
				curr_value = !curr_value;
			}
			if (key == 0b01111111)
			{
				RLE_encoded += writeLastBit(key, curr_value);
				key = 0;
			}
		}
	}

	RLE_encoded += writeLastBit(key, curr_value);
	double compression_coeff = static_cast<double>(cols_count * rows_count) / static_cast<double>(RLE_encoded.length()) ;
	return std::make_pair(RLE_encoded, compression_coeff);
}


std::string LayoutMatrix::Base64_encode(const std::string& str,bool RLE)
{
	std::string is_RLE;
	if (RLE)
		is_RLE = "RLE";
	return base64_encode(str + std::string(i_size_param) + std::to_string(i_size) + std::string(j_size_param) + std::to_string(j_size) + std::string(encode_param) + is_RLE);
}


char LayoutMatrix::writeLastBit(char word, bool value)
{
	switch (value)
	{
	case true:
		return word | BIT(7);
	case false:
		return word & ~BIT(7);
	}
}
std::string LayoutMatrix::Base64_decode(const std::string& str)
{
	std::string encoded_str = base64_decode(str);
	
		std::string_view parameters = std::string_view(encoded_str).substr(encoded_str.find(i_size_param), encoded_str.length() - encoded_str.find(i_size_param));
		std::string_view i_size = parameters.substr(parameters.find(i_size_param) + i_size_param.length(), parameters.find(j_size_param));
		std::string_view j_size = parameters.substr(parameters.find(j_size_param) + j_size_param.length(), parameters.find(encode_param));
		size_t i=std::stoull(std::string(i_size));
		size_t j = std::stoull(std::string(j_size));

		resize(i, j);


		if (parameters.find("RLE") == std::string::npos)
		{
			const std::string_view str_matr = std::string_view(encoded_str).substr(0, encoded_str.find(i_size_param));
			std::string_view::const_iterator it = str_matr.begin();
			for (size_t i = 0; i < rows_count; i++)
				for (size_t j = 0; j < cols_count; j++)
				{
					bitmap[i][j] = *it;
					it++;
				}
			return std::string();
		}
	
	
	return encoded_str.substr(0, encoded_str.find_first_of(i_size_param));
}

void LayoutMatrix::RLE_decode(const std::string& str)
{
	if (str.empty())
		return;
	size_t j = 0, k = 0;
	for (size_t i = 0; i < str.length(); i++)
	{
		char tmp = str[i];
		bool value = (tmp & BIT(7)) >> 7;
		tmp = writeLastBit(tmp, 0);

		while (tmp != 0)
		{
			set(k, j, value);
			j++;
			if (j == j_size)
			{
				k++;
				j = 0;
			}
			tmp--;
		}
	}
}




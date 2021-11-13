#include "Matrix.h"
//конструкторы
Bit_matrix::Bit_matrix(const size_t rows, const size_t cols):i_size(rows),j_size(cols)
{
	rows_count = rows;
	cols_count = (j_size % (sizeof(int8_t) * BITS)) ? (j_size / (sizeof(int8_t) * BITS) + 1) : (j_size / (sizeof(int8_t) * BITS));
	alloc_mem();
	
}

Bit_matrix::Bit_matrix(const Bit_matrix& matrix) :is_alloc(false)
{
	rows_count = matrix.rows_count;
	cols_count = matrix.cols_count;
	if (matrix.is_alloc)
		alloc_mem();
	cpy_bitmap(bitmap, matrix.bitmap, cols_count, rows_count);
	i_size = matrix.i_size;
	j_size = matrix.j_size;
}

Bit_matrix::Bit_matrix(Bit_matrix&& matrix) noexcept
{
	cols_count = matrix.cols_count;
	rows_count = matrix.rows_count;
	i_size = matrix.i_size;
	j_size = matrix.j_size;
	is_alloc = matrix.is_alloc;
	bitmap = matrix.bitmap;
	matrix.bitmap = nullptr;
	
}


//Перегруженные операторы
Bit_matrix& Bit_matrix::operator=(const Bit_matrix& matrix)
{
	if (!matrix.is_alloc)
		return *this;
	if (&matrix == this)
		return *this;
	dealloc_mem();
	rows_count = matrix.rows_count;
	cols_count = matrix.cols_count;
	i_size = matrix.i_size;
	j_size = matrix.j_size;
	alloc_mem();
	cpy_bitmap(bitmap, matrix.bitmap, cols_count, rows_count);
	return *this;
}

Bit_matrix& Bit_matrix::operator=(Bit_matrix&& matrix) noexcept
{
	bitmap = matrix.bitmap;
	matrix.bitmap = nullptr;
	cols_count = matrix.cols_count;
	rows_count = matrix.rows_count;
	i_size = matrix.i_size;
	j_size = matrix.j_size;
	is_alloc = matrix.is_alloc;
	return *this;
}

Bit_matrix operator^(const Bit_matrix& matr1, const Bit_matrix& matr2)
{
	if (!matr1.is_alloc || !matr2.is_alloc)
		return Bit_matrix();
	if (matr1.j_size != matr2.j_size || matr1.i_size != matr2.i_size)
		return Bit_matrix();
	Bit_matrix result(matr1.i_size, matr1.j_size);
	for (size_t i = 0; i < matr1.rows_count; i++)
		for (size_t j = 0; j < matr1.cols_count; j++)
			result.bitmap[i][j] = matr1.bitmap[i][j] ^ matr2.bitmap[i][j];
	return result;
}

bool Bit_matrix::operator!()
{
	if (!is_alloc)
		return true;
	if (i_size == 0 || j_size == 0)
		return true;
	return false;
}

//Геттеры сеттеры
Matrix_state Bit_matrix::set(size_t i, size_t j, bool value)
{
	if (!is_alloc)
		return Matrix_state::ALLOC_ERROR;

	if (i >= i_size || j >= j_size)
		return Matrix_state::INDEX_ERROR;
	const uint8_t bit_count = sizeof(bitmap[0][0]) * BITS;
	//finding byte pos in arr
	const size_t i_byte_pos = i;
	const size_t j_byte_pos = j / bit_count;

	const size_t j_addit_bit_pos = bit_count - 1 - j % bit_count;
	switch (value)
	{
	case true:
		bitmap[i_byte_pos][j_byte_pos] |= BIT(j_addit_bit_pos);
		break;
	case false:
		bitmap[i_byte_pos][j_byte_pos] &= ~BIT(j_addit_bit_pos);
		break;
	}

	switch ((bitmap[i_byte_pos][j_byte_pos] & BIT(j_addit_bit_pos)) >> j_addit_bit_pos)
	{
	case 0x0: return Matrix_state::bit_0;
	case 0x1: return Matrix_state::bit_1;
	default:  return Matrix_state::RECEIVE_ERROR;
	}
}

int32_t Bit_matrix::get(size_t i, size_t j) const 
{
	if (!is_alloc)
		return static_cast<int8_t>(Matrix_state::ALLOC_ERROR);
	const uint8_t bit_count = sizeof(bitmap[0][0]) * BITS;

	if (i >= i_size || j >= j_size)
		return static_cast<int32_t>(Matrix_state::INDEX_ERROR);
	//finding byte pos in arr
	const size_t i_byte_pos = i;
	const size_t j_byte_pos = j / bit_count;
	const size_t j_addit_bit_pos = bit_count - 1 - j% bit_count;

	return (bitmap[i_byte_pos][j_byte_pos] & BIT(j_addit_bit_pos)) >> j_addit_bit_pos;
}

Matrix_state Bit_matrix::print() const
{
	if (!is_alloc)
		return Matrix_state::ALLOC_ERROR;

	const int8_t bit_count = sizeof(bitmap[0][0]) * BITS;

	for (size_t i = 0; i < i_size; i++)
	{
		for (size_t j = 0; j < j_size; j++)
		{

			const int8_t shift = bit_count - 1 - j % bit_count;
			std::cout<<((bitmap[i][j /bit_count] & BIT(shift)) >> shift);
		}
		std::cout << "\n";
	}
	return Matrix_state::INDEX_ERROR;
}
				
double Bit_matrix::unit_ratio()
{
	if (!is_alloc)
		return -1;
	const uint8_t bit_count = sizeof(bitmap[0][0]) * BITS;
	double unit = 0;
	
	for (size_t i = 0; i < rows_count; i++)
	{
		for (size_t j = 0; j < cols_count; j++)
		{
			for (size_t k = bit_count - 1; k > 0; k--)
			{
				if (j + k >= j_size)
					continue;
				if ((BIT(k) & bitmap[i][j]) == BIT(k))
					unit++;
			}
		}
	}
	return unit / static_cast<double>(i_size * j_size);
}

double Bit_matrix::compare(const Bit_matrix& matr1, const Bit_matrix& matr2)
{
	Bit_matrix res = matr1 ^ matr2;
	return res.unit_ratio();
}


std::string get_str(const Bit_matrix& matr)
{
	std::string tmp;
	for (size_t i = 0; i < matr.rows_count; i++)
	{
		for (size_t j = 0; j < matr.cols_count; j++)
		{
			tmp += matr.bitmap[i][j];
		}
	}
	return tmp;
}
//служебные методы
void Bit_matrix::cpy_bitmap(int8_t** dest, int8_t** source, const size_t cols, const size_t rows)
{
	for (size_t i = 0; i < rows; i++)
		for (size_t j = 0; j < cols; j++)
			dest[i][j] = source[i][j];
}

void Bit_matrix::alloc_mem()
{
	bitmap = new int8_t * [rows_count];
	for (size_t i = 0; i < rows_count; i++)
	{
		bitmap[i] = new int8_t[cols_count];
	}
	for (size_t i = 0; i < rows_count; i++)
		for (size_t j = 0; j < cols_count; j++)
		{
			bitmap[i][j] = 0;
		}
	is_alloc = true;
}

void Bit_matrix::dealloc_mem()
{
	if (!is_alloc || !bitmap)
		return;

	for (size_t i = 0; i < rows_count; i++)
	{
		delete[] bitmap[i];
		bitmap[i] = nullptr;
	}
	delete[] bitmap;
	bitmap = nullptr;
}

Bit_matrix::~Bit_matrix()
{
	dealloc_mem();
}
#include "Matrix.h"
Bit_matrix::Bit_matrix(const size_t rows, const size_t cols):i_size(rows),j_size(cols)
{
	rows_count = rows;
	cols_count = (j_size % (sizeof(int8_t) * BITS)) ? (j_size / (sizeof(int8_t) * BITS) + 1) : (j_size / (sizeof(int8_t) * BITS));
	bit_count = sizeof(bitmap[0][0]) * BITS;
	alloc_mem();
	bit_count = sizeof(bitmap[0][0]) * BITS;
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
	bit_count = sizeof(bitmap[0][0]) * BITS;
}

Bit_matrix::Bit_matrix(Bit_matrix&& matrix) noexcept
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

Bit_matrix::~Bit_matrix()
{
	reset();
}

//overloaded operators
Bit_matrix& Bit_matrix::operator=(const Bit_matrix& matrix)
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
	alloc_mem();
	cpy_bitmap(bitmap, matrix.bitmap, cols_count, rows_count);
	return *this;
}

Bit_matrix& Bit_matrix::operator=(Bit_matrix&& matrix) noexcept
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
	if (!is_alloc||!bitmap)
		return true;
	if (i_size == 0 || j_size == 0)
		return true;
	return false;
}

//Getters

int32_t Bit_matrix::get(size_t i, size_t j) const 
{
	if (!is_alloc)
		return static_cast<int8_t>(Matrix_state::ALLOC_ERROR);
	if (i >= i_size || j >= j_size)
		return static_cast<int32_t>(Matrix_state::INDEX_ERROR);
	//finding byte pos in arr
	const size_t i_byte_pos = i;
	const size_t j_byte_pos = j / bit_count;
	const size_t j_addit_bit_pos = bit_count - 1 - j% bit_count;
	switch ((bitmap[i_byte_pos][j_byte_pos] & BIT(j_addit_bit_pos)) >> j_addit_bit_pos)
	{
	case 0x0:return 0x0;
	case 0x1:return 0x1;
	default:return static_cast<int32_t>(Matrix_state::OP_FAILURE);
	}
	
}
int32_t Bit_matrix::unsafe_get(size_t i, size_t j) const
{
	return (bitmap[i][j / bit_count] & BIT(bit_count - 1 - j % bit_count)) >> (bit_count - 1 - j % bit_count);
}
bool Bit_matrix::is_allocated() const 
{ 
	return is_alloc; 
}
size_t Bit_matrix::get_i_size() const
{
	return i_size;
}
size_t Bit_matrix::get_j_size() const
{
	return j_size;
}

//Setters

Matrix_state Bit_matrix::set(size_t i, size_t j, bool value)
{
	if (!is_alloc)
		return Matrix_state::ALLOC_ERROR;

	if (i >= i_size || j >= j_size)
		return Matrix_state::INDEX_ERROR;
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
	case 0x0: return Matrix_state::OP_SUCCESS;
	case 0x1: return Matrix_state::OP_SUCCESS;
	default:  return Matrix_state::OP_FAILURE;
	}
}

void Bit_matrix::unsafe_set(size_t i, size_t j, bool value)
{
	switch (value)
	{
	case true:
		bitmap[i][j / bit_count] |= BIT(bit_count - 1 - j % bit_count);
		break;
	case false:
		bitmap[i][j / bit_count] &= ~BIT(bit_count - 1 - j % bit_count);
		break;
	}
}

Matrix_state Bit_matrix::set_in_range(size_t i_start, size_t j_start, size_t i_end, size_t j_end, bool value)
{
	const size_t j_start_byte_pos = j_start / bit_count;
	const size_t j_end_byte_pos = j_end / bit_count;
	const bool key = (j_end_byte_pos - j_start_byte_pos > 1) ? true : false;

	switch (value)
	{
	case 1:
	{
		if (key)
		{
			for (size_t i = i_start; i <= i_end; i++)
			{
				for (size_t j = j_start_byte_pos + 1; j <= j_end_byte_pos - 1; j++)
				{
					bitmap[i][j] = 0xFF;
					
				}
			}
			for (size_t i = i_start; i <= i_end; i++)
			{
				for (size_t j = j_start; j < (j_start_byte_pos + 1) * bit_count; j++)
					unsafe_set(i, j, value);

				for (size_t j = (j_end_byte_pos - 1) * bit_count; j <= j_end; j++)
					unsafe_set(i, j, value);
			}
		}
		else
			for (size_t i = i_start; i <= i_end; i++)
				for (size_t j = j_start; j <= j_end; j++)
					unsafe_set(i, j, value);


		break;
	}
	case 0:
	{
		if (key)
		{
			for (size_t i = i_start; i <= i_end; i++)
				for (size_t j = j_start_byte_pos + 1; j <= j_end_byte_pos - 1; j++)
					bitmap[i][j] = 0;
			for (size_t i = i_start; i <= i_end; i++)
			{
				for (size_t j = j_start; j < (j_start_byte_pos+1) * bit_count; j++)
					unsafe_set(i, j, value);

				for (size_t j = (j_end_byte_pos-1) * bit_count; j <= j_end; j++)
					unsafe_set(i, j, value);
			}
		}
		else
		{
			for (size_t i = i_start; i <= i_end; i++)
				for (size_t j = j_start; j <= j_end; j++)
					unsafe_set(i, j, value);
		}
		break;
	}
	}
	

	return Matrix_state::OP_SUCCESS;
}




Matrix_state Bit_matrix::print() const
{
	if (!is_alloc)
		return Matrix_state::ALLOC_ERROR;

	for (size_t i = 0; i < i_size; i++)
	{
		for (size_t j = 0; j < j_size; j++)
		{

			const int8_t shift = bit_count - 1 - j % bit_count;
			std::cout << ((bitmap[i][j / bit_count] & BIT(shift)) >> shift);
		}
		std::cout << "\n";
	}
	return Matrix_state::INDEX_ERROR;
}

double Bit_matrix::unit_ratio()
{
	if (!is_alloc)
		return -1;
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

Matrix_state Bit_matrix::ones()
{
	if (!is_alloc)
		return Matrix_state::ALLOC_ERROR;
	
	int8_t unused_area = 0xFF;
	for (size_t i = 0; i < rows_count * bit_count - j_size; i++)
		unused_area &= ~BIT(i);

	for (size_t i = 0; i < rows_count; i++)
		for (size_t j = 0; j < cols_count; j++)
		{
			bitmap[i][j] = static_cast<int8_t>(0xFFFFFFFF);
			if (j == rows_count - 1)
				bitmap[i][j] &= unused_area;
		}

	return Matrix_state::MATR_CORRECT;
}
Matrix_state Bit_matrix::zeros()
{
	if (!is_alloc)
		return Matrix_state::ALLOC_ERROR;

	for (size_t i = 0; i < rows_count; i++)
		for (size_t j = 0; j < cols_count; j++)
			bitmap[i][j] = 0;
	return Matrix_state::MATR_CORRECT;
}
Matrix_state Bit_matrix::randm()
{
	if (!is_alloc)
		return Matrix_state::ALLOC_ERROR;
	srand(time(nullptr));
	//rand();
	int8_t unused_area = 0xFF;
	for (size_t i = 0; i < rows_count * bit_count - j_size; i++)
		unused_area &= ~BIT(i);

	for (size_t i = 0; i < rows_count; i++)
		for (size_t j = 0; j < cols_count; j++)
		{
			bitmap[i][j] = static_cast<int8_t>(rand());
			if (j == rows_count - 1)
				bitmap[i][j] &= unused_area;
		}
	return Matrix_state::OP_SUCCESS;

}
Matrix_state Bit_matrix::resize(size_t rows, size_t cols)
{
	if (rows <= 0 || cols <= 0)
		return Matrix_state::INDEX_ERROR;
	reset();
	rows_count = i_size = rows;
	j_size = cols;
	cols_count = (j_size % (sizeof(int8_t) * BITS)) ? (j_size / (sizeof(int8_t) * BITS) + 1) : (j_size / (sizeof(int8_t) * BITS));
	bit_count = sizeof(bitmap[0][0]) * BITS;
	alloc_mem();
	return Matrix_state::OP_SUCCESS;
	
}
//utility methods


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

void Bit_matrix::reset()
{
	if (bitmap)
	{
		for (size_t i = 0; i < rows_count; i++)
		{
			delete[] bitmap[i];
			bitmap[i] = nullptr;
		}
		delete[] bitmap;
		bitmap = nullptr;
	}
	is_alloc = false;
	i_size = 0;
	j_size = 0;
	cols_count = 0;
	rows_count = 0;
	bit_count = 0;
}
#pragma once
#include <vector>
#include <iostream>
#include <bitset>
#include <ctime>
#include <cstdlib>

#define BIT(n)         (1U << (n))
#define BITS           (8U)

enum class Matrix_state {
	OP_SUCCESS = -4,
	OP_FAILURE = -5,
	MATR_CORRECT = -4,
	ALLOC_ERROR = -3,
	INDEX_ERROR = -2,
	RECEIVE_ERROR = -1,
	bit_0 = 0x0,
	bit_1 = 0x1
};
//ASVsasvA==
class Bit_matrix{
private:
	int8_t**      	           bitmap;
	uint8_t                    bit_count;
	size_t                     j_size,      i_size;
	size_t                     cols_count, rows_count;
	bool                       is_alloc;
public:
	Bit_matrix():bitmap(nullptr),cols_count(0),rows_count(0),j_size(0),i_size(0),is_alloc(false),bit_count(0) {}
	Bit_matrix(const size_t rows, const size_t cols);
	Bit_matrix(const Bit_matrix& matrix);
	Bit_matrix(Bit_matrix&& matrix) noexcept;
	virtual ~Bit_matrix();
	
	//getters
	int32_t          get(size_t i, size_t j) const;
	inline int32_t   unsafe_get(size_t i, size_t j) const;
	bool             is_allocated() const;
	size_t           get_i_size() const;
	size_t           get_j_size() const;

	//setters
	Matrix_state     set(size_t i, size_t j, bool value);
	inline void      unsafe_set(size_t i, size_t j, bool value);
	Matrix_state     set_in_range(size_t i_start, size_t j_start, size_t i_end, size_t j_end, bool value);

	Matrix_state     ones();
	Matrix_state     zeros();
	Matrix_state     randm();
	Matrix_state     resize(size_t rows,size_t cols);
	Matrix_state     print() const;
	double           unit_ratio();


	Bit_matrix&      operator=(const Bit_matrix& matrix);
	Bit_matrix&      operator=(Bit_matrix&& matrix) noexcept;
	bool             operator!();

	static double    compare(const Bit_matrix& matr1, const Bit_matrix& matr2);

private:
	//utility methods
	void             cpy_bitmap(int8_t** dest, int8_t** source,const size_t cols,const size_t rows);
	void             alloc_mem();
	void             reset();

public:
	friend Bit_matrix  operator^(const Bit_matrix& matr1, const Bit_matrix& matr2);
	
};
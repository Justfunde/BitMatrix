#pragma once
#include <vector>
#include <iostream>
#include <bitset>

#define BIT(n)         (1U << (n))
#define BITS           (8U)

enum class Matrix_state {
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
	size_t                     j_size,      i_size;
	size_t                     cols_count, rows_count;
	bool                       is_alloc;
public:
	Bit_matrix():bitmap(nullptr),cols_count(0),rows_count(0),j_size(0),i_size(0),is_alloc(false) {}
	Bit_matrix(const size_t rows, const size_t cols);
	Bit_matrix(const Bit_matrix& matrix);
	Bit_matrix(Bit_matrix&& matrix) noexcept;
	~Bit_matrix();
	
	Matrix_state set(size_t i, size_t j, bool value);
    int32_t get(size_t i, size_t j) const;
	bool is_allocated() const { return is_alloc; }
	Matrix_state print() const;
	void bits_print();
	double unit_ratio();


	Bit_matrix& operator=(const Bit_matrix& matrix);
	Bit_matrix& operator=(Bit_matrix&& matrix) noexcept;
	bool operator!();

	static double compare(const Bit_matrix& matr1, const Bit_matrix& matr2);

private:
	//utility methods
	void cpy_bitmap(int8_t** dest, int8_t** source,const size_t cols,const size_t rows);

	//inline Matrix_state correct_matr() const ;
	//inline Matrix_state correct_matr(size_t i,size_t j) const;
	void alloc_mem();
	void dealloc_mem();

public:
	friend Bit_matrix operator^(const Bit_matrix& matr1, const Bit_matrix& matr2);
	friend std::string get_str(const Bit_matrix &matr);
};
std::string get_str(const Bit_matrix& matr);
#pragma once
#include "Matrix.h"
#include <string>
#include "Base64.h"
#include <memory>

constexpr std::string_view i_size_param = "I__SIZE__";
constexpr std::string_view j_size_param = "__J__SIZE__";
constexpr std::string_view encode_param = "__ENCODE__PARAM__";


class LayoutMatrix:public BitMatrix
{
private:
	std::string hash; 
	
public:
	//Constructors
	LayoutMatrix() {}
	LayoutMatrix(const std::string& in_hash);
	LayoutMatrix(size_t i, size_t j) :BitMatrix(i, j) {}
	LayoutMatrix(const BitMatrix& matrix):BitMatrix(matrix) {}
	LayoutMatrix(const LayoutMatrix& matrix);
	LayoutMatrix(LayoutMatrix&& matrix) noexcept;
	LayoutMatrix(BitMatrix&& matrix) noexcept;


	//Matrix based methods
	BitMatrix getMatrix() const;
	void setMatrix(const BitMatrix& matrix);
	std::string encodeHash();

	//Hash based methods
	std::string getHash() const;
	void setHash(const std::string& hash);
	BitMatrix decodeHash();

	
	LayoutMatrix& operator=(const BitMatrix& matrix);
	LayoutMatrix& operator=(BitMatrix&& matrix) noexcept;
	LayoutMatrix& operator=(const LayoutMatrix& matrix);
	LayoutMatrix& operator=(LayoutMatrix&& matrix) noexcept;
	
private:
	
	std::string Base64_encode(const std::string& str,bool RLE);
	std::string Base64_decode(const std::string& str);
	std::pair<std::string, double> RLE_encode();
	void RLE_decode(const std::string& str);
	inline char writeLastBit(char word,bool value);
};




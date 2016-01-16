#pragma once
#include <vector>
#include <iterator>

class row_iterator{};

template <typename T>
class Board
{
public:
	using Matrix = std::vector<T>;
	using IT = typename std::vector<T>::iterator;

	class row_iterator 
	{
	public:
		row_iterator(IT begin,
					IT end,
					IT it,
					int pos)
					: begin(begin), end(end), it(it), pos(pos)
		{};

		T operator *() { return *it._Ptr; }
		T operator =(const T rhs) const 
		{
			*it._Ptr = rhs;
			return *it._Ptr;
		}
		T value() 
		{ 
			if (!isEnd())
				return *it;
			return 0;
		}

		bool isEnd() { return it == end; }
		bool isBegin() { return it == begin; }
		void next()
		{
			if (!isEnd())
			{
				it++;
				pos++;
			}
		}
		void previous() { it--; }
		void goToEnd() { it = end; }
		void goToBeginning() { it = begin;}
		row_iterator operator++(int) {
			row_iterator tmp(*this);
			operator++();
			return tmp;
		}

		row_iterator &operator++()
		{

				it++;
				pos++;
			return *this;
		}

		row_iterator operator--(int) {
			if (!isBegin()) {
				it--;
				pos--;
			}
			return *this; }

		friend bool operator!=(const row_iterator &lhs, const row_iterator &rhs) {
			return &lhs.it != &rhs.it;
		}

		friend bool operator==(const row_iterator &lhs, const row_iterator &rhs) {
			return &lhs.it == &rhs.it;
		}

		const bool operator!=(const row_iterator rhs) {
			return it != rhs.it;
		}

		const bool operator==(const row_iterator rhs) {
			return it == rhs.it;
		}

		IT getIt() { return it; }

		IT it;
		int pos;
	private:
		IT begin;
		IT end;
		
	};

	Board();
	Board(const Board&) = default;
	Board(unsigned, unsigned);
	Board(unsigned, unsigned, T);

	void combineRows(unsigned, unsigned);
	void combineColumns(unsigned, unsigned);

	row_iterator insertRow(row_iterator it, T);
	row_iterator insertColumn(row_iterator it, T);

	void appendHorizontally(Board<T>);
	void appendVertically(Board<T>);

	row_iterator rowIteratorBegin()
	{
		return row_iterator(matrix.begin(), matrix.end(), matrix.begin(), 0);
	}

	row_iterator rowIteratorEnd()
	{
		return row_iterator(matrix.begin(), matrix.end(), matrix.end(), columns());
	}

	unsigned rows();
	unsigned columns();
	unsigned totalSize();

	void setRows(unsigned);
	void setColumns(unsigned);
	T& at(unsigned, unsigned);

private:
	void insertOnPos(unsigned, unsigned, T);
	unsigned rowCount;
	unsigned columnCount;

	Matrix matrix;
};

template<typename T>
Board<T>::Board() :
	rowCount(1), columnCount(1)
{
	matrix.push_back(0);
}

template<typename T>
Board<T>::Board(unsigned row, unsigned col) 
	: rowCount(row), columnCount(col)
{
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			matrix.push_back(0);
		}
	}
}

template<typename T>
Board<T>::Board(unsigned row, unsigned col, T var)
	: rowCount(row), columnCount(col)
{
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			matrix.push_back(var);
		}
	}
}

template<typename T>
void Board<T>::combineRows(unsigned rRow, unsigned lRow)
{
	for (int i = 0; i < columns(); i++)
	{
		at(lRow, i) += at(rRow, i);
	}
}

template<typename T>
inline void Board<T>::combineColumns(unsigned rCol, unsigned lCol)
{
	for (int i = 0; i < rows(); i++)
	{
		at(i, lCol) += at(i, rCol);
	}
}

template<typename T>
typename Board<T>::row_iterator Board<T>::insertRow(typename Board<T>::row_iterator it, T val)
{
	matrix.insert(it.it, columnCount, val);
	rowCount++;
	return it;
}

template<typename T>
typename Board<T>::row_iterator Board<T>::insertColumn(typename Board<T>::row_iterator it, T val)
{
	columnCount++;
	for (int i = 0; i < rowCount; i++)
	{
		insertOnPos(i, it.pos, val);
	}
	return it;
}

template<typename T>
void Board<T>::appendHorizontally(Board<T> secBoard)
{
	int diffRow = secBoard.rows() - this->rows();
	if (diffRow > 0) 
	{
		for (int i = 0; i < diffRow; i++)
		{
			for (int j = 0; j < columns(); j++)
			{
				insertOnPos(rowCount+i, j, 0);
			}
		}
		rowCount = secBoard.rows();
	}
	else
	{
		diffRow = abs(diffRow);
		for (int i = 0; i < diffRow; i++)
		{
			for (int j = 0; j < columns(); j++)
			{
				secBoard.insertOnPos(rowCount + i, j, 0);
			}
		}
		secBoard.setRows(rows());
	}

	for (int i = 0; i < secBoard.rows(); i++)
	{
		for (int j = 0; j < secBoard.columns(); j++)
		{
			T val = secBoard.at(i, j);
			insertOnPos(i, columnCount+j, val);
		}	
	}
	columnCount += secBoard.columns();
}

template<typename T>
inline void Board<T>::appendVertically(Board<T> secBoard)
{
	int diffCol = secBoard.columns() - this->columns();
	if (diffCol > 0)
	{
		for (int i = 0; i < rows(); i++)
		{
			for (int j = 0; j < diffCol; j++)
			{
				insertOnPos(i, columnCount + j, 0);
			}
		}
		setColumns(secBoard.columns());
	}
	else
	{
		diffCol = abs(diffCol);
		for (int i = 0; i < diffCol; i++)
		{
			for (int j = 0; j < columns(); j++)
			{
				secBoard.insertOnPos(i, columnCount + j, 0);
			}
		}
		secBoard.setColumns(columns());
	}


	for (int i = 0; i < secBoard.rows(); i++)
	{
		for (int j = 0; j < secBoard.columns(); j++)
		{
			T val = secBoard.at(i, j);
			insertOnPos(rowCount+i, j, val);
		}
	}
	rowCount += secBoard.rows();
}

template<typename T>
unsigned Board<T>::rows()
{
	return rowCount;
}

template<typename T>
unsigned Board<T>::columns()
{
	return columnCount;
}

template<typename T>
unsigned Board<T>::totalSize()
{
	return rows() * columns();
}

template<typename T>
void Board<T>::setRows(unsigned val)
{
	rowCount = val;
}

template<typename T>
void Board<T>::setColumns(unsigned val)
{
	columnCount = val;
}

template<typename T>
T& Board<T>::at(unsigned row, unsigned col)
{
	int pos = row*columns() + col;
	if (pos < matrix.size())
		return matrix.at(pos);
	else
		return matrix.back();
}

template<typename T>
void Board<T>::insertOnPos(unsigned row, unsigned col, T val)
{
	int pos = row*columns() + col;
	if (pos < totalSize())
		matrix.insert(matrix.begin() + pos, val);
	else
		matrix.push_back(val);
}


// iterator


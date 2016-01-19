#pragma once
#include <string>
#include <fstream>
#include <stdio.h>
#include <vector>
#include <exception>

template <typename P>
class Row
{
public:
	P & operator[](unsigned index)
	{
		if (index > row.size())
			throw out_of_range("Out of Row");
		else
			return row.at(index);
	}
	std::vector<P> row;
};


template <typename P>
class Column
{
public:
	Row<P> & operator[](unsigned index)
	{
		if (index > column.size())
			throw std::out_of_range("Out of columns");
		else
			return column.at(index);
	}
	std::vector<Row<P> > column;
};

template <typename T>
class FileMatrix
{
public:

	FileMatrix();
	FileMatrix(const FileMatrix&) = default;
	FileMatrix(const std::vector<std::vector<T> >&);
	~FileMatrix() = default;
	std::string GetFileExtention() const;

	Row<T> & operator[](unsigned row);
	friend bool operator!=(const FileMatrix & lhs, const FileMatrix & rhs)
	{
		return &lhs.matrix != &rhs.matrix;
	}

	friend bool operator==(const FileMatrix & lhs, const FileMatrix & rhs)
	{
		return &lhs.matrix != &rhs.matrix;
	}

	void operator*=(T& value);

	FileMatrix<T> operator *(const FileMatrix<T> &rhs);


	void fileName();
	void flush();
	void setDiagonal(T val);
	void initialise(unsigned ROW, unsigned COLS);
	void initialise(unsigned ROW, unsigned COLS, std::string name);
	void initialiseFromFile(std::string name);
	unsigned getNumberOfRows() const;
	unsigned getNumberOfColumns() const;
	std::string getFileName() const; 
	unsigned rowCount;
	unsigned colCount;
	Column<T> matrix;
private:

	std::string ex;
	std::string fName;
	
};

template<typename T>
FileMatrix<T>::FileMatrix()
{
	ex = ".fm";
}

template<typename T>
inline FileMatrix<T>::FileMatrix(const std::vector<std::vector<T> >& m)
{
	rowCount = m.size();
	colCount = m.at(0).size();
	for (int i = 0; i < rowCount; i++)
	{
		matrix.column.push_back(Row<T>());
		for (int j = 0; j < colCount; j++)
		{
			matrix.column.at(i).row.push_back(m.at(i).at(j));
		}
	}
}

template<typename T>
std::string FileMatrix<T>::GetFileExtention() const
{
	return ex;
}

template<typename T>
Row<T> & FileMatrix<T>::operator[](unsigned row)
{
	if (row > rowCount)
		throw std::out_of_range("Out of rows");
	else
		return matrix[row];
}

template<typename T>
inline void FileMatrix<T>::operator*=(T & value)
{
	for (int i = 0; i < rowCount; i++)
	{
		for (int j = 0; j < colCount; j++)
		{
			matrix[i][j] *= value;
		}
	}
}

template<typename T>
inline FileMatrix<T> FileMatrix<T>::operator*(const FileMatrix<T>& rhs)
{
	FileMatrix<T> newMatrix;
	newMatrix.initialise(getNumberOfRows(), getNumberOfRows());
	for (int i = 0; i < getNumberOfRows(); i++)
	{
		for (int j = 0; j < getNumberOfColumns(); j++)
		{
			for (int k = 0; k< getNumberOfColumns(); k++)
			{
				T val = matrix.column.at(i).row.at(k) * rhs.matrix.column.at(k).row.at(j);
				newMatrix.matrix[i][j] += val;
			}
		}
	}
	return newMatrix;
}

template<typename T>
void FileMatrix<T>::fileName()
{
	fName = std::to_string(reinterpret_cast<unsigned>(this))+ex;
	initialise(rowCount, colCount, fName);
}

template<typename T>
void FileMatrix<T>::flush()
{
	ofstream myFile(fName, std::ofstream::binary);

	myFile.write(reinterpret_cast<char*>(&rowCount), sizeof(unsigned));
	myFile.write(reinterpret_cast<char*>(&colCount), sizeof(unsigned));

	for (int i = 0; i < rowCount; i++)
	{
		for (int j = 0; j < colCount; j++)
		{
			myFile.write(reinterpret_cast<char*>(&matrix[i][j]), sizeof(T));
		}
	}
}

template<typename T>
inline void FileMatrix<T>::setDiagonal(T val)
{
	for (int i = 0; i < rowCount; i++)
	{
		matrix[i][i] = val;
	}
}

template<typename T>
void FileMatrix<T>::initialise(unsigned ROW, unsigned COLS)
{
	T val = 0;
	rowCount = ROW;
	colCount = COLS;

	unsigned memory = reinterpret_cast<unsigned>(this);
	fName = std::to_string(memory) + ex;

	ofstream myFile(fName, std::ofstream::binary);

	myFile.write(reinterpret_cast<char*>(&rowCount), sizeof(unsigned));
	myFile.write(reinterpret_cast<char*>(&colCount), sizeof(unsigned));

	for (int i = 0; i < rowCount; i++)
	{
		matrix.column.push_back(Row<T>());
		for (int j = 0; j < colCount; j++)
		{
			matrix.column.at(i).row.push_back(val);
			myFile.write(reinterpret_cast<char*>(&val), sizeof(T));
		}
	}
}

template<typename T>
void FileMatrix<T>::initialise(unsigned ROW, unsigned COLS, std::string name)
{
	T val = 0;
	rowCount = ROW;
	colCount = COLS;
	fName = name;
	ofstream myFile(fName, std::ofstream::binary);
	
	myFile.write(reinterpret_cast<char*>(&rowCount), sizeof(unsigned));
	myFile.write(reinterpret_cast<char*>(&colCount), sizeof(unsigned));

	for (int i = 0; i < rowCount; i++)
	{
		matrix.column.push_back(Row<T>());
		for (int j = 0; j < colCount; j++)
		{
			matrix.column.at(i).row.push_back(val);
			myFile.write(reinterpret_cast<char*>(&val), sizeof(T));
		}
	}
}

template<typename T>
void FileMatrix<T>::initialiseFromFile(std::string name)
{
	T val = 0;
	ifstream myFile(name, std::ifstream::binary);
	myFile.read(reinterpret_cast<char*>(&rowCount), sizeof(unsigned));
	myFile.read(reinterpret_cast<char*>(&colCount), sizeof(unsigned));
	for (int i = 0; i < rowCount; i++)
	{
		matrix.column.push_back(Row<T>());
		for (int j = 0; j < colCount; j++)
		{
			matrix.column.at(i).row.push_back(val);
			myFile.read(reinterpret_cast<char*>(&matrix[i][j]), sizeof(T));
		}
	}
}

template<typename T>
inline unsigned FileMatrix<T>::getNumberOfRows() const
{
	return rowCount;
}

template<typename T>
inline unsigned FileMatrix<T>::getNumberOfColumns() const 
{
	return colCount;
}

template<typename T>
inline std::string FileMatrix<T>::getFileName() const
{
	return fName;
}

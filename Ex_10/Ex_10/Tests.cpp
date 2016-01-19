#include <vector>
#include <string>
#include <fstream>
#include <iterator>  // ostream_iterator<>()
#include <algorithm> // copy()
#include <cstdio>    // remove()

#include "dirent.h"
#include "gtest\gtest.h"
#include "FileMatrix.h"

using namespace std;
using namespace testing;

struct FileMatrixTester : public ::testing::Test
{
	vector<string> filesToRemove;

	vector<string> filesInWorkingDirectory_beforeTest;
	vector<string> filesInWorkingDirectory_afterTest;

	void SetUp();
	void TearDown();

	template<typename T>
	const string CalculateFileName(const FileMatrix<T>& object);

	template<typename T>
	const string CalculateFileNameAndMakeSureTheFileWouldBeRemoved(const FileMatrix<T>& object);

	bool FileExists(const string& fileName);

	long GetFileSize(const string& fileName);

	template<typename T>
	long CalculateSizeForFile(unsigned rows, unsigned cols, const FileMatrix<T>&);

	template<typename T>
	void CheckDataInFile(const string& fileName, unsigned expectedCols, unsigned expectedRows, const vector<T>& dataToCheck);

	template<typename Container>
	void ReadNearFilesToContainer(Container& c);
};

TEST_F(FileMatrixTester, TestInitialisationOfSmallMatrix)
{
	const unsigned ROWS = 1, COLS = 1;

	typedef int Type;

	FileMatrix<Type> matrix;
	matrix.initialise(ROWS, COLS);

	const string& matrixFileName = CalculateFileNameAndMakeSureTheFileWouldBeRemoved(matrix);
	ASSERT_TRUE(FileExists(matrixFileName)) << matrixFileName;
	ASSERT_EQ(CalculateSizeForFile(ROWS, COLS, matrix), GetFileSize(matrixFileName)) << matrixFileName;
}

TEST_F(FileMatrixTester, TestInitialisationOfVector)
{
	const unsigned ROWS = 1000, COLS = 1;

	typedef double Type;

	FileMatrix<Type> matrix;
	matrix.initialise(ROWS, COLS);

	const string& matrixFileName = CalculateFileNameAndMakeSureTheFileWouldBeRemoved(matrix);
	ASSERT_TRUE(FileExists(matrixFileName)) << matrixFileName;
	ASSERT_EQ(CalculateSizeForFile(ROWS, COLS, matrix), GetFileSize(matrixFileName)) << matrixFileName;
}

TEST_F(FileMatrixTester, TestInitialisationOfSingleRowAndMultipleColumns)
{
	const unsigned ROWS = 1, COLS = 1000;

	typedef double Type;

	FileMatrix<Type> matrix;
	matrix.initialise(ROWS, COLS);

	const string& matrixFileName = CalculateFileNameAndMakeSureTheFileWouldBeRemoved(matrix);
	ASSERT_TRUE(FileExists(matrixFileName)) << matrixFileName;
	ASSERT_EQ(CalculateSizeForFile(ROWS, COLS, matrix), GetFileSize(matrixFileName)) << matrixFileName;
}

TEST_F(FileMatrixTester, TestInitialisationOfSquareMatrix)
{
	const unsigned ROWS = 1000, COLS = 1000;

	typedef double Type;

	FileMatrix<Type> matrix;
	matrix.initialise(ROWS, COLS);

	const string& matrixFileName = CalculateFileNameAndMakeSureTheFileWouldBeRemoved(matrix);
	ASSERT_TRUE(FileExists(matrixFileName)) << matrixFileName;
	ASSERT_EQ(CalculateSizeForFile(ROWS, COLS, matrix), GetFileSize(matrixFileName)) << matrixFileName;
}

TEST_F(FileMatrixTester, TestInitialisationOfSquareMatrixForDifferentType)
{
	const unsigned ROWS = 1000, COLS = 1000;

	typedef double Type;

	FileMatrix<Type> matrix;
	matrix.initialise(ROWS, COLS);

	const string& matrixFileName = CalculateFileNameAndMakeSureTheFileWouldBeRemoved(matrix);
	ASSERT_TRUE(FileExists(matrixFileName)) << matrixFileName;
	ASSERT_EQ(CalculateSizeForFile(ROWS, COLS, matrix), GetFileSize(matrixFileName)) << matrixFileName;
}

TEST_F(FileMatrixTester, TestSettingMatrixName)
{
	const unsigned ROWS = 100, COLS = 100;

	typedef double Type;

	const string matrixFileName("myMatrix.matrix");
	FileMatrix<Type> matrix;
	matrix.initialise(ROWS, COLS, matrixFileName);

	ASSERT_TRUE(FileExists(matrixFileName)) << matrixFileName;
	ASSERT_EQ(CalculateSizeForFile(ROWS, COLS, matrix), GetFileSize(matrixFileName)) << matrixFileName;

	filesToRemove.push_back(matrixFileName);
}

TEST_F(FileMatrixTester, TestElementAccessingOutOfRange)
{
	const unsigned ROWS = 5, COLS = 10;
	const unsigned rowIndexOutOfRange = ROWS + 1;

	typedef double Type;

	FileMatrix<Type> matrix;
	matrix.initialise(ROWS, COLS);

	ASSERT_THROW(matrix[rowIndexOutOfRange], out_of_range);

	const string& matrixFileName = CalculateFileNameAndMakeSureTheFileWouldBeRemoved(matrix);
	ASSERT_TRUE(FileExists(matrixFileName)) << matrixFileName;
	ASSERT_EQ(CalculateSizeForFile(ROWS, COLS, matrix), GetFileSize(matrixFileName)) << matrixFileName;
}

TEST_F(FileMatrixTester, TestElementAccessing)
{
	const unsigned ROWS = 2, COLS = 3;

	typedef int Type;

	FileMatrix<Type> matrix;
	matrix.initialise(ROWS, COLS);

	const string& matrixFileName = CalculateFileNameAndMakeSureTheFileWouldBeRemoved(matrix);

	vector<Type> valuesWhichIExpect;
	valuesWhichIExpect.reserve(ROWS*COLS);
	for (int r = 0; r < ROWS; ++r)
		for (int c = 0; c < COLS; ++c)
		{
			auto valueToAdd = r*COLS + c;
			matrix[r][c] = valueToAdd;

			valuesWhichIExpect.push_back(valueToAdd);
		}
	matrix.flush(); // to save current row to file

	ASSERT_TRUE(FileExists(matrixFileName)) << matrixFileName;
	ASSERT_EQ(CalculateSizeForFile(ROWS, COLS, matrix), GetFileSize(matrixFileName)) << matrixFileName;

	CheckDataInFile(matrixFileName, ROWS, COLS, valuesWhichIExpect);
}

TEST_F(FileMatrixTester, TestComparingOfMatrixes)
{
	const unsigned ROWS = 100, COLS = 200;

	typedef long long Type;

	FileMatrix<Type> matrix1;
	matrix1.initialise(ROWS, COLS);
	const string& matrixFileName1 = CalculateFileNameAndMakeSureTheFileWouldBeRemoved(matrix1);

	FileMatrix<Type> matrix2;
	matrix2.initialise(ROWS, COLS);
	const string& matrixFileName2 = CalculateFileNameAndMakeSureTheFileWouldBeRemoved(matrix2);

	for (int r = 0; r < ROWS; ++r)
		for (int c = 0; c < COLS; ++c)
		{
			auto valueToAdd = r*COLS + c;
			matrix1[r][c] = valueToAdd;
			matrix2[r][c] = valueToAdd;
		}
	matrix1.flush();
	matrix2.flush();

	ASSERT_TRUE(FileExists(matrixFileName1)) << matrixFileName1;
	ASSERT_TRUE(FileExists(matrixFileName2)) << matrixFileName2;
	ASSERT_EQ(GetFileSize(matrixFileName1), GetFileSize(matrixFileName2)) << "size should be: " << CalculateSizeForFile(ROWS, COLS, matrix1);

	for (int r = 0; r < ROWS; ++r)
		for (int c = 0; c < COLS; ++c)
			ASSERT_EQ(matrix1[r][c], matrix2[r][c]) << "matrix[" << r << "][" << c << "]";
	ASSERT_EQ(matrix1, matrix2);

	++matrix1[ROWS / 2][COLS / 2];
	ASSERT_NE(matrix1, matrix2);
}

TEST_F(FileMatrixTester, TestCreatingDiagonalMatrix)
{
	const unsigned ROWS = 100;

	typedef int Type;

	FileMatrix<Type> matrix1;
	matrix1.initialise(ROWS, ROWS);
	const string& matrixFileName1 = CalculateFileNameAndMakeSureTheFileWouldBeRemoved(matrix1);

	FileMatrix<Type> matrix2;
	matrix2.initialise(ROWS, ROWS);
	const string& matrixFileName2 = CalculateFileNameAndMakeSureTheFileWouldBeRemoved(matrix2);

	Type valueToDiagonal = Type();
	++valueToDiagonal;

	for (int i = 0; i < ROWS; ++i)
		matrix1[i][i] = valueToDiagonal;
	matrix1.flush();

	matrix2.setDiagonal(valueToDiagonal);
	ASSERT_EQ(matrix1, matrix2);

	++valueToDiagonal;
	matrix1.setDiagonal(valueToDiagonal);
	ASSERT_NE(matrix1, matrix2);
}

TEST_F(FileMatrixTester, TestInitialisationFromFile)
{
	const unsigned ROWS = 100, COLS = 200;

	typedef int Type;

	const string matrixFileName("sourceMatrix.matrix");

	FileMatrix<Type> matrix1;
	matrix1.initialise(ROWS, COLS, matrixFileName);

	for (int r = 0; r < ROWS; ++r)
		for (int c = 0; c < COLS; ++c)
		{
			auto valueToAdd = r*COLS + c;
			matrix1[r][c] = valueToAdd;
		}
	matrix1.flush();

	FileMatrix<Type> matrix2;
	matrix2.initialiseFromFile(matrixFileName);

	ASSERT_TRUE(FileExists(matrixFileName)) << matrixFileName;
	ASSERT_EQ(CalculateSizeForFile(ROWS, COLS, matrix1), GetFileSize(matrixFileName)) << "size should be: " << CalculateSizeForFile(ROWS, COLS, matrix1);

	ASSERT_EQ(matrix1, matrix2);
	filesToRemove.push_back(matrixFileName);
}

TEST_F(FileMatrixTester, TestCopyingOfMatrix)
{
	const unsigned ROWS = 100, COLS = 200;

	typedef int Type;

	FileMatrix<Type> matrix1;
	matrix1.initialise(ROWS, COLS);

	for (int r = 0; r < ROWS; ++r)
		for (int c = 0; c < COLS; ++c)
		{
			auto valueToAdd = r*COLS + c;
			matrix1[r][c] = valueToAdd;
		}
	matrix1.flush();

	const string& matrixFileName1 = CalculateFileNameAndMakeSureTheFileWouldBeRemoved(matrix1);
	ASSERT_TRUE(FileExists(matrixFileName1)) << matrixFileName1;

	FileMatrix<Type> matrix2 = matrix1;
	const string& matrixFileName2 = CalculateFileNameAndMakeSureTheFileWouldBeRemoved(matrix2);
	matrix2.fileName();
	ASSERT_TRUE(FileExists(matrixFileName2)) << matrixFileName2;
	ASSERT_EQ(GetFileSize(matrixFileName1), GetFileSize(matrixFileName2)) << "size should be: " << CalculateSizeForFile(ROWS, COLS, matrix1);

	ASSERT_EQ(matrix1, matrix2);

	filesToRemove.push_back(matrix2.getFileName());
}

TEST_F(FileMatrixTester, TestInicializationFromStaticArray)
{
	const unsigned ROWS = 2, COLS = 3;

	typedef int Type;

	const vector<vector<Type> > sourceArray = {
		{ 1, 2, 3 },
		{ 4, 5, 6 }
	};

	FileMatrix<Type> matrix(sourceArray);

	ASSERT_EQ(ROWS, matrix.getNumberOfRows());
	ASSERT_EQ(COLS, matrix.getNumberOfColumns());

	for (unsigned r = 0; r < ROWS; ++r)
		for (unsigned c = 0; c < COLS; ++c)
			ASSERT_EQ(sourceArray[r][c], matrix[r][c]) << "r = " << r << ", c = " << c;

	filesToRemove.push_back(matrix.getFileName());
}

TEST_F(FileMatrixTester, TestMatrixNumberMultiplication)
{
	const unsigned ROWS = 10, COLS = 20;

	typedef int Type;

	FileMatrix<Type> matrix1, matrix2;
	matrix1.initialise(ROWS, COLS);
	matrix2.initialise(ROWS, COLS);

	Type initValue = Type();
	++initValue;
	for (int r = 0; r < ROWS; ++r)
		for (int c = 0; c < COLS; ++c)
			matrix1[r][c] = matrix2[r][c] = initValue;
	matrix1.flush();
	matrix2.flush();

	const string& matrixFileName1 = CalculateFileNameAndMakeSureTheFileWouldBeRemoved(matrix1);
	ASSERT_TRUE(FileExists(matrixFileName1)) << matrixFileName1;

	const string& matrixFileName2 = CalculateFileNameAndMakeSureTheFileWouldBeRemoved(matrix2);
	ASSERT_TRUE(FileExists(matrixFileName2)) << matrixFileName2;

	ASSERT_EQ(GetFileSize(matrixFileName1), GetFileSize(matrixFileName2)) << "size should be: " << CalculateSizeForFile(ROWS, COLS, matrix1);

	ASSERT_EQ(matrix1, matrix2);

	/////////////////////////////////////////////////////////////////////////////////////////
	++initValue;
	for (int r = 0; r < ROWS; ++r)
		for (int c = 0; c < COLS; ++c)
			matrix2[r][c] = initValue;
	ASSERT_NE(matrix1, matrix2);

	matrix1 *= initValue;
	ASSERT_EQ(matrix1, matrix2);
}

TEST_F(FileMatrixTester, TestMatrixMultipledWithMatrix)
{
	typedef int Type;

	const unsigned ROWS = 2, COLS = 3;
	const vector<vector<Type> > sourceMatrix1 =
	{
		{ 1, 2, 3 },
		{ 4, 5, 6 }
	};

	const vector<vector<Type> >  sourceMatrix2 =
	{
		{ 7,  8 },
		{ 9,  10 },
		{ 11, 12 }
	};

	FileMatrix<Type> matrix1(sourceMatrix1);
	FileMatrix<Type> matrix2(sourceMatrix2);

	ASSERT_EQ(matrix1.getNumberOfColumns(), matrix2.getNumberOfRows());
	ASSERT_EQ(matrix2.getNumberOfColumns(), matrix1.getNumberOfRows());

	/////////////////////////////////////////////////////////////////// M1m2 = matrix1 * matrix2:
	FileMatrix<Type> matrix1m2;
	matrix1m2.initialise(ROWS, ROWS);
	
	const vector<vector<Type> > sourceMatrix_firstMultipliedWithSecond =
	{
		{ 58,  64 },
		{ 139, 154 }
	};

	ASSERT_EQ(matrix1m2.getNumberOfColumns(), ROWS);
	ASSERT_EQ(matrix1m2.getNumberOfRows(), ROWS);

	ASSERT_TRUE(sourceMatrix_firstMultipliedWithSecond == matrix1m2);

	/////////////////////////////////////////////////////////////////////// M2m1 = matrix2 * matrix1:
	const vector<vector<Type> > sourceMatrix_secondMultipliedWithFirst =
	{
		{ 39, 54, 69 },
		{ 49, 68, 87 },
		{ 59, 82, 105 }
	};

	const FileMatrix<Type>& matrix2m1 = matrix2 * matrix1;
	ASSERT_EQ(COLS, matrix2m1.getNumberOfRows());
	ASSERT_EQ(COLS, matrix2m1.getNumberOfColumns());
	for (unsigned r = 0; r < COLS; ++r)
		for (unsigned c = 0; c < COLS; ++c)
			ASSERT_EQ(sourceMatrix_secondMultipliedWithFirst, matrix2m1) << "r = " << r << ", c = " << c;

	///////////////////////////////////////////////////////////////////// cleanup:
	filesToRemove.push_back(matrix1.getFileName());
	filesToRemove.push_back(matrix2.getFileName());
	CalculateFileNameAndMakeSureTheFileWouldBeRemoved(matrix1m2);
	CalculateFileNameAndMakeSureTheFileWouldBeRemoved(matrix2m1);
}

int main(int argc, char* argv[])
{
	InitGoogleTest(&argc, argv);
	int retVal = RUN_ALL_TESTS();
#ifdef _WIN32
	cin.get();
#endif
	return retVal;
}

// Functions

void FileMatrixTester::SetUp()
{
	ReadNearFilesToContainer(filesInWorkingDirectory_beforeTest);
}

template<typename Container>
void FileMatrixTester::ReadNearFilesToContainer(Container & c)
{
	c.clear();
	dirent* file;
	DIR* directory;

	if ((directory = opendir(".")))
	{
		while ((file = readdir(directory)))
			c.push_back(file->d_name);

		closedir(directory);
	}
	else
		throw runtime_error("The current directory can't be opened!");
}

void FileMatrixTester::TearDown()
{
	for (auto s : filesToRemove)
		remove(s.c_str());
	filesToRemove.clear();

	ReadNearFilesToContainer(filesInWorkingDirectory_afterTest);

	//EXPECT_EQ(filesInWorkingDirectory_beforeTest.size(), filesInWorkingDirectory_afterTest.size());
	/// The test can't be done in VS, because after removing a file sometimes it is still able to read the removed file's name during some time.
	if (filesInWorkingDirectory_beforeTest.size() != filesInWorkingDirectory_afterTest.size())
	{
		cerr << "Does the test cleaned up files? Before test were " << filesInWorkingDirectory_beforeTest.size() << " files, after are: " << filesInWorkingDirectory_afterTest.size() << endl;
		cerr << "Files before: ";
		copy(filesInWorkingDirectory_beforeTest.begin(), filesInWorkingDirectory_beforeTest.end(), ostream_iterator<string>(cerr, ", "));
		cerr << "\nFiles after:  ";
		copy(filesInWorkingDirectory_afterTest.begin(), filesInWorkingDirectory_afterTest.end(), ostream_iterator<string>(cerr, ", "));
		cerr << endl;
	}
}

bool FileMatrixTester::FileExists(const string & fileName)
{
	ifstream myfile(fileName);
	if (myfile.is_open())
	{
		myfile.close();
		return true;
	}
	else
		return false;
}

inline long FileMatrixTester::GetFileSize(const string & fileName)
{
	ifstream myFile(fileName, ios_base::in | ios_base::binary);
	if (!myFile)
		throw runtime_error("File not opened! Does it exist?");

	myFile.seekg(0, ios_base::end);
	return static_cast<long>(myFile.tellg());
}

template<typename T>
const string FileMatrixTester::CalculateFileNameAndMakeSureTheFileWouldBeRemoved(const FileMatrix<T>& object)
{
	const string& returnName = CalculateFileName(object);
	filesToRemove.push_back(returnName);
	return returnName;
}

template<typename T>
const string FileMatrixTester::CalculateFileName(const FileMatrix<T>& object)
{
	unsigned addressInMemory = reinterpret_cast<unsigned>(&object);
	const string& generatedFileName = to_string(addressInMemory) + object.GetFileExtention();
	return generatedFileName;
}

template<typename T>
long FileMatrixTester::CalculateSizeForFile(unsigned rows, unsigned cols, const FileMatrix<T>&)
{
	unsigned sizeOfInformationAboutSizeOfMatrix = 2 * sizeof(unsigned);
	return sizeOfInformationAboutSizeOfMatrix + rows * cols * sizeof(T);
}

template<typename T>
void FileMatrixTester::CheckDataInFile(const string & fileName, unsigned expectedCols, unsigned expectedRows, const vector<T>& dataToCheck)
{
	ifstream myFile(fileName, ios_base::in | ios_base::binary);
	if (!myFile)
		throw runtime_error("File not opened! Does it exist?");

	char buffer[sizeof(T) + sizeof(unsigned) + 1];
	memset(buffer, 0, sizeof(buffer));

	myFile.read(buffer, sizeof(unsigned));
	ASSERT_EQ(expectedCols, *reinterpret_cast<unsigned*>(buffer));
	myFile.read(buffer, sizeof(unsigned));
	ASSERT_EQ(expectedRows, *reinterpret_cast<unsigned*>(buffer));

	unsigned counter = 0;
	while (true)
	{
		myFile.read(buffer, sizeof(T));
		if (myFile.eof())
			break;
		ASSERT_EQ(dataToCheck[counter], *reinterpret_cast<T*>(buffer)) << counter;
		++counter;
	}
}
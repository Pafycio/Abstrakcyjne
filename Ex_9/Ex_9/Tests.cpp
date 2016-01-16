#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>

#include <gtest/gtest.h>

#include "board.h"

using namespace std;
using namespace ::testing;

bool isPositive(int elem)
{
	return elem > 0;
}

bool isNegative(int elem)
{
	return elem < 0;
}

class BoardTests : public ::testing::Test
{
};

TEST_F(BoardTests, testDefaultConstructor)
{
	Board<int> board;

	ASSERT_EQ(1, board.rows());
	ASSERT_EQ(1, board.columns());
	ASSERT_EQ(1, board.totalSize());
	ASSERT_EQ(0, board.at(0, 0));

	const unsigned int row = 0;
	const unsigned int column = 0;

	board.at(row, column) = 5;
	ASSERT_EQ(5, board.at(0, 0));
}

TEST_F(BoardTests, testCustomSizeConstructor)
{
	const unsigned int number_of_rows = 10;
	const unsigned int number_of_columns = 25;

	Board<unsigned int> board(number_of_rows, number_of_columns);

	ASSERT_EQ(10, board.rows());
	ASSERT_EQ(25, board.columns());
	ASSERT_EQ(250, board.totalSize());
}

TEST_F(BoardTests, testDefaultValueConstructorFloat)
{
	const unsigned int number_of_rows = 4;
	const unsigned int number_of_columns = 9;
	const float default_value = 1.5f;

	Board<float> board(number_of_rows, number_of_columns, default_value);

	ASSERT_EQ(4, board.rows());
	ASSERT_EQ(9, board.columns());
	ASSERT_EQ(number_of_rows*number_of_columns, board.totalSize());

	for (unsigned int i = 0; i < number_of_rows; i++)
		for (unsigned int j = 0; j < number_of_columns; j++)
			ASSERT_EQ(default_value, board.at(i, j));
}

TEST_F(BoardTests, testCopyConstructor)
{
	unsigned int rows = 2, cols = 2;
	Board<int> board(rows, cols);

	board.at(0, 0) = 1;
	board.at(0, 1) = 2;
	board.at(1, 0) = -2;
	board.at(1, 1) = 5;

	Board<int> copied_board(board);

	ASSERT_EQ(copied_board.at(0, 0), 1);
	ASSERT_EQ(copied_board.at(0, 1), 2);
	ASSERT_EQ(copied_board.at(1, 0), -2);
	ASSERT_EQ(copied_board.at(1, 1), 5);
}

TEST_F(BoardTests, testAssignmentOperator)
{
	unsigned int rows = 2, cols = 2;
	Board<int> board(rows, cols);

	board.at(0, 0) = 1;
	board.at(0, 1) = 2;
	board.at(1, 0) = -2;
	board.at(1, 1) = 5;

	Board<int> copied_board;
	ASSERT_EQ(1, copied_board.totalSize());

	copied_board = board;

	ASSERT_EQ(copied_board.at(0, 0), 1);
	ASSERT_EQ(copied_board.at(0, 1), 2);
	ASSERT_EQ(copied_board.at(1, 0), -2);
	ASSERT_EQ(copied_board.at(1, 1), 5);
}

TEST_F(BoardTests, testDefaultValueConstructorChar)
{
	const unsigned int number_of_rows = 4;
	const unsigned int number_of_columns = 9;
	const char default_value = 'a';

	Board<char> board(number_of_rows, number_of_columns, default_value);

	ASSERT_EQ(number_of_rows, board.rows());
	ASSERT_EQ(number_of_columns, board.columns());
	ASSERT_EQ(number_of_rows*number_of_columns, board.totalSize());

	for (unsigned int i = 0; i < number_of_rows; i++)
		for (unsigned int j = 0; j < number_of_columns; j++)
			ASSERT_EQ(default_value, board.at(i, j));
}

TEST_F(BoardTests, testRowCombining)
{
	unsigned int rows = 2, cols = 2;
	Board<int> board(rows, cols);

	board.at(0, 0) = 1;
	board.at(0, 1) = 2;
	board.at(1, 0) = -2;
	board.at(1, 1) = 5;

	unsigned int rowA = 0, rowB = 1;
	// Dodaj wiersz A do wiersza B. Wiersz A powinien pozostać nienaruszony.
	board.combineRows(rowA, rowB);

	ASSERT_EQ(board.at(0, 0), 1);
	ASSERT_EQ(board.at(0, 1), 2);
	ASSERT_EQ(board.at(1, 0), -1);
	ASSERT_EQ(board.at(1, 1), 7);

	board.combineRows(rowA, rowA);

	ASSERT_EQ(board.at(0, 0), 2);
	ASSERT_EQ(board.at(0, 1), 4);
	ASSERT_EQ(board.at(1, 0), -1);
	ASSERT_EQ(board.at(1, 1), 7);
}

TEST_F(BoardTests, testColumnsCombining)
{
	unsigned int rows = 2, cols = 2;
	Board<int> board(rows, cols);

	board.at(0, 0) = 1;
	board.at(0, 1) = 2;
	board.at(1, 0) = -2;
	board.at(1, 1) = 5;

	unsigned int columnA = 0, columnB = 1;
	// Dodaj kolumnę A do kolumny B. Kolumna A powinna pozostać nienaruszona.
	board.combineColumns(columnA, columnB);

	ASSERT_EQ(board.at(0, 0), 1);
	ASSERT_EQ(board.at(0, 1), 3);
	ASSERT_EQ(board.at(1, 0), -2);
	ASSERT_EQ(board.at(1, 1), 3);

	board.combineColumns(columnA, columnA);

	ASSERT_EQ(board.at(0, 0), 2);
	ASSERT_EQ(board.at(0, 1), 3);
	ASSERT_EQ(board.at(1, 0), -4);
	ASSERT_EQ(board.at(1, 1), 3);
}

TEST_F(BoardTests, testAppendingBoardsHorizontally)
{
	const unsigned int number_of_rows = 1;
	const unsigned int number_of_columns = 1;
	const int default_value = 1;

	Board<int> board(number_of_rows, number_of_columns, default_value);
	/* Początkowa tablica - jednoelementowa:
	[1]
	*/

	const unsigned int number_of_rows_1 = 2;
	const unsigned int number_of_columns_1 = 1;
	const int default_value_1 = 2;
	/* Dołączana tablica 1 - dwuelementowa:
	[2]
	[2]
	*/
	Board<int> appended_board_1(number_of_rows_1, number_of_columns_1, default_value_1);

	/* Łączenie w poziomie powinno uzupełnić dodatkowe pola o wartości domyślne:
	[1]
	+
	[2]
	[2]
	=
	[1 2]
	[0 2]
	*/
	board.appendHorizontally(appended_board_1);
	ASSERT_EQ(2, board.rows());
	ASSERT_EQ(2, board.columns());
	ASSERT_EQ(4, board.totalSize());
	ASSERT_EQ(board.at(0, 0), 1);
	ASSERT_EQ(board.at(0, 1), 2);
	ASSERT_EQ(board.at(1, 0), 0);
	ASSERT_EQ(board.at(1, 1), 2);

	/* Dołączana tablica 2 - dwuelementowa:
	[3][3]
	*/
	const unsigned int number_of_rows_2 = 1;
	const unsigned int number_of_columns_2 = 2;
	const int default_value_2 = 3;
	Board<int> appended_board_2(number_of_rows_2, number_of_columns_2, default_value_2);

	/*
	[1 2]
	[0 2]
	+
	[3 3]
	=
	[1 2 3 3]
	[0 2 0 0]
	*/
	board.appendHorizontally(appended_board_2);
	ASSERT_EQ(8, board.totalSize());
	ASSERT_EQ(board.at(0, 2), 3);
	ASSERT_EQ(board.at(0, 3), 3);
	ASSERT_EQ(board.at(1, 2), 0);
	ASSERT_EQ(board.at(1, 3), 0);
}

TEST_F(BoardTests, testAppendingBoardsVertically)
{
	const unsigned int number_of_rows = 1;
	const unsigned int number_of_columns = 1;
	const char default_value = 'a';

	Board<char> board(number_of_rows, number_of_columns, default_value);
	/* Początkowa tablica - jednoelementowa:
	[a]
	*/

	const unsigned int number_of_rows_1 = 1;
	const unsigned int number_of_columns_1 = 2;
	const char default_value_1 = 'b';
	/* Dołączana tablica 1 - dwuelementowa:
	[b][b]
	*/
	Board<char> appended_board_1(number_of_rows_1, number_of_columns_1, default_value_1);

	/* Łączenie w pionie powinno uzupełnić dodatkowe pola o wartości domyślne:
	[a]
	+
	[b][b]
	=
	[a '\0']
	[b b]
	*/
	board.appendVertically(appended_board_1);
	ASSERT_EQ(2, board.rows());
	ASSERT_EQ(2, board.columns());
	ASSERT_EQ(4, board.totalSize());
	ASSERT_EQ(board.at(0, 0), 'a');
	ASSERT_EQ(board.at(0, 1), '\0');
	ASSERT_EQ(board.at(1, 0), 'b');
	ASSERT_EQ(board.at(1, 1), 'b');

	/* Dołączana tablica 2 - dwuelementowa:
	[c]
	[c]
	*/
	const unsigned int number_of_rows_2 = 1;
	const unsigned int number_of_columns_2 = 2;
	const int default_value_2 = 'c';
	Board<char> appended_board_2(number_of_rows_2, number_of_columns_2, default_value_2);

	/*
	[a '\0']
	[b b]
	+
	[c]
	[c]
	=
	[a '\0']
	[b b]
	[c '\0']
	[c '\0']
	*/
	board.appendHorizontally(appended_board_2);
	ASSERT_EQ(8, board.totalSize());
	ASSERT_EQ(board.at(0, 2), 'c');
	ASSERT_EQ(board.at(0, 3), 'c');
	ASSERT_EQ(board.at(1, 2), '\0');
	ASSERT_EQ(board.at(1, 3), '\0');
}

TEST_F(BoardTests, testIteratorsShifts)
{
	unsigned int rows = 2, cols = 2;
	Board<int> board(rows, cols);

	board.at(0, 0) = 1;
	board.at(0, 1) = 2;
	board.at(1, 0) = -2;
	board.at(1, 1) = 5;

	Board<int>::row_iterator iterator = board.rowIteratorBegin();

	// Dereferencja iteratora
	ASSERT_EQ(*iterator, 1);
	// Sprawdzenie wartości wskazywanej przez iterator
	ASSERT_EQ(iterator.value(), 1);

	// STL'owe przesunięcie.
	iterator++;
	ASSERT_EQ(*iterator, 2);

	// I alternatywna metoda.
	iterator.next();
	ASSERT_EQ(*iterator, -2);

	// Przesunięcie na koniec.
	iterator.goToEnd();
	ASSERT_TRUE(iterator.isEnd());

	// STL'owe przesunięcie.
	iterator--;
	ASSERT_EQ(*iterator, 5);

	// I alternatywna metoda.
	iterator.previous();
	ASSERT_EQ(*iterator, -2);

	// Powrót na początek.
	iterator.goToBeginning();
	ASSERT_EQ(*iterator, 1);
}

TEST_F(BoardTests, testIteratorsComparision)
{
	int value = 5;
	Board<int> board1(1, 1, value);
	Board<int> board2(3, 4, value);

	Board<int>::row_iterator iterator_begin_1 = board1.rowIteratorBegin();
	Board<int>::row_iterator iterator_end_1 = board1.rowIteratorEnd();
	Board<int>::row_iterator iterator_begin_2 = board2.rowIteratorBegin();
	Board<int>::row_iterator iterator_end_2 = board2.rowIteratorEnd();

	/* Iterator na tej samej pozycji tablicy, np. (0,0), ale dla różnych obiektów, nie powinien być
	uznawany za ten sam. */
	ASSERT_NE(iterator_begin_1, iterator_begin_2);
	///* Znaczniki końca iteratora powinny być różne dla różnych obiektów. */
	ASSERT_NE(iterator_end_1, iterator_end_2);
	///* Iterator na ostatniej pozycji tablicy (tutaj jednoelementowa tablica) powinien być różny od wskaźnika końca. */
	ASSERT_NE(iterator_begin_1, iterator_end_1);

	/* Sprawdź wartość wskazywaną przez iterator... */
	ASSERT_EQ(iterator_begin_1.value(), value);
	/* ... i bardziej w stylu STLowych iteratorów.*/
	ASSERT_EQ(*iterator_begin_1, value);

	/* Przesunięcie iteratora w jednoelementowej tablicy. */
	iterator_begin_1++;
	/* Po przesunięciu powinien wskazywać koniec... */
	ASSERT_TRUE(iterator_begin_1.isEnd());
	/* ... i być równy znacznikowi końca. */
	ASSERT_EQ(iterator_begin_1.it, iterator_end_1.it);
	/* Wyciągnięcie wartości ze znacznika końca nie powinno się wykrzaczyć. Chociaż zwracana
	wartość nie jest rzeczywistym elementem tablicy. W C++ dla tablicy intów iterator zwraca 0. */
	ASSERT_NO_THROW({ value = iterator_end_1.value(); });
	ASSERT_NO_THROW({ iterator_end_1.next(); });
}

TEST_F(BoardTests, testSimpleIterations)
{
	const unsigned int rows = 2, cols = 3;
	Board<int> board(rows, cols);

	int value = 0;
	Board<int>::row_iterator iterator = board.rowIteratorBegin();
	// Wstaw wartości za pomocą iteratora.
	for (; iterator != board.rowIteratorEnd(); iterator++)
	{
		iterator = value++;
	}
	ASSERT_EQ(iterator.it, board.rowIteratorEnd().it);

	int test_value = 5;

	// Sprawdź wartości.
	do
	{
		iterator--;
		ASSERT_EQ(test_value, *iterator);
		test_value--;
	} while (iterator != board.rowIteratorBegin());
}

TEST_F(BoardTests, testRowInserting)
{
	Board<char> board(1, 2);
	board.at(0, 0) = 'o';
	board.at(0, 1) = 'x';

	Board<char>::row_iterator iterator = board.rowIteratorBegin();
	int a = 2;
	/* insertRow
	iterator jest na pozycji (0,1) - oznaczonej 'x'
	ooo
	oxo

	wywołanie board.insertRow(it, 'a') - wstawia dodatkowy wiersz przed wiersz w którym
	znajduje się iterator, tzn dostaniemy:

	ooo
	aaa
	oxo

	iterator nadal wskazuje 'x'
	*/

	//  Przed:  ox
	iterator = board.insertRow(iterator, 'a');
	/* Po:
	aa
	ox
	*/
	ASSERT_EQ(board.at(0, 0), 'a');
	ASSERT_EQ(board.at(0, 1), 'a');
	ASSERT_EQ(board.at(1, 0), 'o');
	ASSERT_EQ(board.at(1, 1), 'x');
	ASSERT_EQ(board.totalSize(), 4);

	// Wstawienie wiersza przed znacznikiem końca.
	iterator = board.insertRow(board.rowIteratorEnd(), 'b');
	/* Wynik:
	aa
	ox
	bb
	*/
	ASSERT_EQ(board.totalSize(), 6);
	ASSERT_EQ(board.at(2, 0), 'b');
	ASSERT_EQ(board.at(2, 1), 'b');
}

TEST_F(BoardTests, testColumnInserting)
{
	Board<char> board(2, 1);
	board.at(0, 0) = 'o';
	board.at(1, 0) = 'x';

	Board<char>::row_iterator iterator = board.rowIteratorBegin();

	/* insertColumn
	iterator jest na pozycji (0,1) - oznaczonej 'x'
	ooo
	oxo

	wywołanie board.insertColumn(it, 'a') - wstawia dodatkową kolumnę przed kolumnę w której
	znajduje się iterator, tzn dostaniemy:

	oaoo
	oaxo

	iterator nadal wskazuje 'x'
	*/
	/*  W zadaniu, tablica początkowa:
	o
	x
	*/
	iterator = board.insertColumn(iterator, 'a');
	/* Po:
	ao
	ax
	*/
	ASSERT_EQ(board.at(0, 0), 'a');
	ASSERT_EQ(board.at(0, 1), 'o');
	ASSERT_EQ(board.at(1, 0), 'a');
	ASSERT_EQ(board.at(1, 1), 'x');
	ASSERT_EQ(board.totalSize(), 4);

	///* Po wstawieniu kolumny, iterator będzie miał więcej elementów do przejścia, tzn. może
	//trafić na nowo dodaną kolumnę (z dodawaniem wierszy nie było takich problemów. */

	//// Wstawienie kolumny przed znacznikiem końca.
	iterator = board.insertColumn(board.rowIteratorEnd(), 'b');
	///* Wynik:
	//aob
	//axb
	//*/

	ASSERT_EQ(board.totalSize(), 6);
	ASSERT_EQ(board.at(0, 2), 'b');
	ASSERT_EQ(board.at(1, 2), 'b');
}

TEST_F(BoardTests, testMultipleInserting)
{
	const unsigned int rows = 4, cols = 3;
	Board<char> board(rows, cols);

	board.at(0, 0) = 'x';
	board.at(1, 0) = 'y';

	/* Początkowa tablica:
	x
	y
	*/

	// Wstawiany znak

	char inserted_char = 'a';
	// Liczba wykonanych operacji

	int counter = 0;
	Board<char>::row_iterator iterator = board.rowIteratorBegin();
	for (int i = 0; i < 1; i++)
	{
		board.insertRow(iterator, inserted_char++);
		counter += 3;
		board.insertColumn(iterator, inserted_char++);
		counter += 3;
	}
	/* Wynikowa tablica (można sobie rozpisać te kroki i zobaczyć, że tak wyjdzie:) :
	dbfa
	dbfx
	dcfc
	eefe
	dbfy
	*/

	ASSERT_EQ(counter, 6);
	ASSERT_EQ(board.totalSize(), 20);

	ASSERT_EQ(board.rows(), 5);
	ASSERT_EQ(board.columns(), 4);

	ASSERT_EQ(board.at(0, 0), 'b');
	ASSERT_EQ(board.at(0, 1), 'a');
	ASSERT_EQ(board.at(0, 2), 'a');
	ASSERT_EQ(board.at(0, 3), 'a');

	ASSERT_EQ(board.at(1, 1), 'x');
	ASSERT_EQ(board.at(1, 3), '\0');

}


int main(int argc, char* argv[])
{
	InitGoogleTest(&argc, argv);
	int retVal = RUN_ALL_TESTS();
	cin.get();
	return retVal;
}
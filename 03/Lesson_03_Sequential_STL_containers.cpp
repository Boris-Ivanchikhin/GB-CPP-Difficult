// *** Урок 03.  Последовательные контейнеры STL / Sequential STL containers

#include <locale.h>  // Для setlocale (LC_ALL, "")
#include <iostream>
#include <iterator>
#include <random>
#include <vector>
#include <algorithm>
#include <list>
#include <optional>
//#include "timer.h"

using namespace std;

/*
 * 1. Написать функцию, добавляющую в конец списка вещественных чисел элемент, 
 *    значение которого равно среднему арифметическому всех его элементов.
 */

template <typename T>
void addAvg (list<T>& _list) {
    // check
    if (!_list.size())
        return;
    // calc
	T average = 0;
	for (const auto& elem : _list)
		average += elem;
	_list.push_back (average / static_cast<T>(_list.size()));
}

/*
 * 2. Создать класс, представляющий матрицу. 
 *    Реализовать в нем метод, вычисляющий определитель матрицы.
 *    Для реализации используйте контейнеры из STL.
 */

template <typename T>
class Matrix
{
private:
    // data
    vector<vector<T>> data;
    // helper function to make an array rectangular
    void MakeRectangle() {
        // Calculating the maximum length of the string
        size_t maxSize = 0;
        for (const auto& row : data)
            if (row.size() > maxSize)
                maxSize = row.size();
        for (auto& row : data)   // iteration without const allows you to change row
            row.resize(maxSize); // increase the length of the lines if necessary
    };

public:
    // constructors
    Matrix(const vector<vector<T>>& _data): data(_data) {
        MakeRectangle();  // observe the invariant
    };
    Matrix(size_t rows, size_t columns) {
        data.resize(rows);
        for (auto& row : data)
            row.resize(columns);
    };
    Matrix(const Matrix&&) = delete;
    // destructor
    ~Matrix() = default;
    // methods
    size_t GetColumns() const {
        // for an empty vector, the null element cannot be accessed
        if (data.empty()) {
            return 0;
        }
        return data[0].size();
    };
    size_t GetRows() const {
        return data.size();
    };
    optional<T> determinant () {
        return calculate (*this);
    }
    // operators
    // []
    const vector<T>& operator [] (size_t i) const {
        return data[i];
    };
    vector<T>& operator [] (size_t i) {  // constant overload
        return data[i];
    };
    // ()
    T& operator () (size_t i, size_t j) {
        return data[i][j];
    };
    const T& operator () (size_t i, size_t j) const {
        return data[i][j];
    };
    // = &&
    void operator= (Matrix&&) = delete;
    // +=
    Matrix<T>& operator += (const Matrix<T>& other) {
        const size_t rows = GetRows();
        const size_t columns = GetColumns();
        if (rows != other.GetRows() || columns != other.GetColumns()) {
            throw invalid_argument("matrices have different size!");
        };
        for (size_t i = 0; i != rows; ++i) {
            for (size_t j = 0; j != columns; ++j) {
                data[i][j] += other.data[i][j];
            };
        };
        // return
        return *this;
    };
    // ==
    template <typename T2>
    bool operator == (const Matrix<T2>& other) const {
        const size_t rows = GetRows();
        const size_t columns = GetColumns();
        if (rows != other.GetRows() || columns != other.GetColumns()) {
            return false;
        }
        for (size_t i = 0; i != rows; ++i) {
            for (size_t j = 0; j != columns; ++j) {
                if (!((*this)(i, j) == other(i, j))) {
                    return false;
                }
            }
        }
        // return
        return true;
    };
    // !=
    template <typename T2>
    bool operator != (const Matrix<T2>& other)  const {
        // return
        return !(*this == other);
    };

    // iterators

    using const_iterator = decltype(data.cbegin());
    // cbegin
    const_iterator cbegin() const {
        return data.cbegin();
    };
    // cend
    const_iterator cend() const {
        return data.cend();
    };

    using iterator = decltype(data.begin());
    // begin
    iterator begin() {
        return data.begin();
    };
    // end
    iterator end() {
        return data.end();
    };

    // friends
    // ...

protected:
    optional<T> calculate (Matrix<T>&);
    void getMatrixWithoutRowAndCol (Matrix<T>&, int, int, Matrix<T>&);

}; // class Matrix

template <typename T>
ostream& operator << (ostream& out, const Matrix<T>& matrix) {
    const size_t rows = matrix.GetRows();
    const size_t columns = matrix.GetColumns();

    for (size_t i = 0; i != rows; ++i) {
        for (size_t j = 0; j != columns; ++j) {
            if (j > 0) {
                out << "\t";
            };
            out << matrix[i][j];
        };
        out << "\n";
    };
    // return
    return out;
}

template <typename T>
Matrix<T> operator + (const Matrix<T>& m1, const Matrix<T>& m2) {
    auto tmp = m1;
    tmp += m2;
    return tmp;
}

template <typename T>
void Matrix<T>::getMatrixWithoutRowAndCol (Matrix<T>& matrix, int row, int col, Matrix<T>& newMatrix) {
    int size      = matrix.GetRows();
    int offsetRow = 0; // Смещение индекса строки в матрице
    int offsetCol = 0; // Смещение индекса столбца в матрице
    for(int i = 0; i < size-1; i++) {
        // Пропустить row-ую строку
        if(i == row) {
            offsetRow = 1; // Как только встретили строку, которую надо пропустить, делаем смещение для исходной матрицы
        }

        offsetCol = 0; // Обнулить смещение столбца
        for(int j = 0; j < size - 1; j++) {
            // Пропустить col-ый столбец
            if(j == col) {
                offsetCol = 1; // Встретили нужный столбец, проускаем его смещением
            }
            newMatrix[i][j] = matrix[i + offsetRow][j + offsetCol];
        }
    }
}

template <typename T>
optional<T> Matrix<T>::calculate (Matrix<T>& matrix) {
    
    if (matrix.GetRows() != matrix.GetColumns())
        return nullopt;
    
    T det = 0;
    int degree = 1; // (-1)^(1+j) из формулы определителя
    int size = matrix.GetRows();

    // Условие выхода из рекурсии
    if(size == 1) {
        return matrix[0][0];
    }
    // Условие выхода из рекурсии
    else if(size == 2) {
        return matrix[0][0]*matrix[1][1] - matrix[0][1]*matrix[1][0];
    }
    else {
        // Матрица без строки и столбца
        Matrix<T> newMatrix (size - 1, size - 1);
        // Раскладываем по 0-ой строке, цикл бежит по столбцам
        for(int j = 0; j < size; j++) {
            // Удалить из матрицы i-ю строку и j-ый столбец
            // Результат в newMatrix
            getMatrixWithoutRowAndCol(matrix, 0, j, newMatrix);

            // Рекурсивный вызов
            // По формуле: сумма по j, (-1)^(1+j) * matrix[0][j] * minor_j (это и есть сумма из формулы)
            // где minor_j - дополнительный минор элемента matrix[0][j]
            // (минор - это определитель матрицы без 0-ой строки и j-го столбца)
            auto value = calculate(newMatrix);
            det = det + (static_cast<T>(degree) * matrix[0][j] * value.value_or(0));
            // "Накручиваем" степень множителя
            degree = -degree;
        }
    }
    // return
    return det;
}

/*
 * 3. Реализовать собственный класс итератора, с помощью которого
 *    можно будет проитерироваться по диапазону целых чисел в цикле for-range-based.
 */

template<typename T>
class RangeIterator
{
public:
	typedef T RangeType;
	typedef RangeIterator<RangeType> SelfType;
	typedef typename RangeType::ValueType ValueType;

private:
	const RangeType* const m_range;
	ValueType m_value;

public:
	RangeIterator(const RangeType* const range, ValueType startValue)
		: m_range(range), m_value(startValue) {};

	operator ValueType() const {
		return m_value;
	};

	ValueType operator*() const	{
		return m_value;
	};

	SelfType& operator++() {
		m_value += m_range->step();
		return *this;
	};

	SelfType operator++(int) {
		SelfType tempIt(*this);
		++(*this);
		return tempIt;
	};

	bool operator!= (const SelfType& right)	{
		return !(*this == right);
	};
};

template <typename T>
class Range
{
private:
	T m_min, m_max, m_step;

public:
	typedef size_t SizeType;
	typedef T ValueType;
	typedef T RangeType;
	
	Range(T min, T max, T step = T(1)) 
		: m_min(min), m_max(max), m_step(step) {};

	ValueType operator[](RangeType& i) const {
		return (m_min + (i * m_step));
	};

	SizeType size()	{
		return static_cast<SizeType>((m_max - m_min) / m_step);
	};

	ValueType step() const {
		return m_step;
	};

	RangeIterator<Range<T>> begin()	{
		return RangeIterator<Range<T>>(this, m_min);
	};

	RangeIterator<Range<T>> end() {
		return RangeIterator<Range<T>>(this, m_min + size() * m_step);
	};
};

// Function called at shutdown
void endFunction (void)
{
    // Output of the shutdown message
    puts ("\nthe work is completed!");
} // endFunction

// main
int main (void) {

    // Russian language in the console
    setlocale (LC_ALL, "");

     // Registration of the function to be called during normal shutdown
    if (atexit (endFunction)) {
        // Checking the registration of the endFunction
        puts ("post- function registration error!");
    }

    cout << "Lesson 03. Sequential STL containers.\n\n";

    // * Exercises #1
    cout << "Exercise #1.\n";

    // create & initialize RNG
    mt19937 gen {random_device()()};
    // distribution real of RNG
    uniform_real_distribution <> RNG_REAL(0, 100.0f);
    // list
    const size_t LIST_SIZE = 10;
    list <float> List (LIST_SIZE);
    // fill the List
    generate(List.begin(), List.end(), [&RNG_REAL, &gen]() {return RNG_REAL(gen);});
    // display the List
    cout << "source list   : ";
    copy (List.begin(), List.end(), ostream_iterator<float> (cout, " "));
    cout << endl;
    // add average
    addAvg (List);
    // list with average value
    cout << "with avg.value: ";
    copy (List.begin(), List.end(), ostream_iterator<float> (cout, " "));
    cout << endl;

    // * Exercise #2
    cout << "\nExercise #2.\n";

    const int MAX_SIZE = 3;
    Matrix<int> matrix (MAX_SIZE, MAX_SIZE);
    // distribution int of RNG
    uniform_int_distribution <int> RNG_INT(0, 100);
    
    for (auto& row : matrix) {
        // processing the string (row)
        generate (row.begin(), row.end(), [&RNG_INT, &gen]() {return RNG_INT(gen);});
    }

    cout << "source matrix:\n" << matrix;
    auto value = matrix.determinant();
    if (value.has_value())
        cout << "matrix determinant: " << value.value() << endl;
    else
        cout << ("error in calculating the determinant") << endl;

    // * Exercises #3
    cout << "\nExercise #3.\n";

    cout << "range<int> (0, 10): [";
    for (auto elem : Range<int> (0, 10))
        cout << elem << ' ';
    cout << ']' << endl;

    std::cout << "range<float> (0, 1.0f, 0.1f): [";
    for (auto elem : Range<float>(1.25f, 3.0f, 0.25f))
        cout << elem << ' ';
    cout << ']' << endl;

    // return o.k.
    return EXIT_SUCCESS;
} // main

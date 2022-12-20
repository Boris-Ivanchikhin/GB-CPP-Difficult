// *** Урок 02.  Модель памяти и время выполнения / Memory model and execution time

#include <locale.h>  // Для setlocale (LC_ALL, "")
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <stdint.h>
#include "timer.h"

using namespace std;

/*
 * 1. Реализуйте шаблонную функцию Swap, которая принимает два указателя и обменивает местами значения,
 *    на которые указывают эти указатели (нужно обменивать именно сами указатели,
 *    переменные должны оставаться в тех же адресах памяти).
 */

template<typename T>
void Swap(T** lPtr, T** rPtr) {
    // check
    if (*lPtr == *rPtr)
        return;
    // do
	T *tValue = *lPtr;
	*lPtr = *rPtr;
	*rPtr = tValue;
}

/*
 * 2. Реализуйте шаблонную функцию SortPointers, которая принимает вектор указателей
 *    и сортирует указатели по значениям, на которые они указывают.
 */

template<typename T>
void SortPointers(vector<T*>& vec) {
	sort(vec.begin(), vec.end(),
		[](T* lValue, T* rValue) {
			return (*lValue < *rValue);
		});
}

/*
 * 3. Подсчитайте количество гласных букв в книге “Война и мир”. Для подсчета используйте 4 способа:
 *    • count_if и find
 *    • count_if и цикл for
 *    • цикл for и find
 *    • 2 цикла for
 *    Замерьте время каждого способа подсчета и сделайте выводы.
 * 
 *    Справка:
 *    • count_if - это алгоритмическая функция из STL, которая принимает 3 параметра: 
 *          итератор на начало, 
 *          итератор на конец,
 *          унарный предикат (функцию, принимающую один параметр и возвращающую тип bool).
 *    • find - это метод класса string, который возвращает позицию символа (строки), 
 *      переданного в качестве параметра, в исходной строке. 
 *      Если символ не найден, то метод возвращает string::npos.
 */

uint16_t Count_if_find(const string& source, const string& search) {
    string_view wSource = source, wSearch = search;
	return count_if(wSource.begin(), wSource.end(),
		[&wSearch](const char& c) {
			return (wSearch.find(c) != string::npos);
		});
}

uint16_t Count_if_for(const string& source, const string& search) {
    string_view wSource = source, wSearch = search;
	return count_if(wSource.begin(), wSource.end(),
		[&wSearch](const char& c) {
			for (const auto& element : wSearch)
				if (c == element)
                    return true;
			return false;
		});
}

uint16_t For_find(const string& source, const string& search) {
    string_view wSource = source, wSearch = search;
	uint16_t count{};
	for (const auto& c : wSource)
		if (wSearch.find(c) != string::npos)
            count++;
    // return
	return count;
}

uint16_t x2_For(const string& source, const string& search) {
    string_view wSource = source, wSearch = search;
	uint16_t count{};
	for (const auto& c : wSource)
		for (const auto& element : wSearch)
			if (c == element) {
				++count;
				break;
			}
    // return
	return count;
}


// function generator:
int* RandomNumber () {
    int *Number = new int (rand() % 100);
    return Number;
}

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

    cout << "Lesson 02. Memory model and execution time.\n\n";

    // * Exercises #1
    cout << "Exercise #1.\n";

    int *A = new int (10);
    int *B = new int (200);
    
    cout << "initial (before swapping):" << endl;
    cout << "A = " << *A << ", &A = " << A << "; B = " << *B << ", &B = " << B << "." << endl;
	
    Swap(&A, &B);

    cout << "result (after swapping):" << endl;
    cout << "A = " << *A << ", &A = " << A << "; B = " << *B << ", &B = " << B << "." << endl;

    delete A;
    delete B;

    // * Exercise #2
    cout << "\nExercise #2.\n";

    const size_t MAX_COUNT = 10;
	vector<int *> ptrVector(MAX_COUNT);

    srand (unsigned (time(0)));
    generate (ptrVector.begin(), ptrVector.end(), RandomNumber);

    // lambda vector display function
    auto display = [&]() {
        cout << "{address, value}: ";
		for (const auto &elem : ptrVector)
			cout << "{" << elem << "->" << *elem << "} ";
		cout << endl;
	};
    cout << "vector before sorting..." << endl;
    display();

    SortPointers<int>(ptrVector);

    cout << "vector after sorting..." << endl;
    display();

    // garbage collector
	for (auto &elem : ptrVector)
		delete elem;

    // * Exercises #3
    cout << "\nExercise #3.\n";

    ifstream file;
    file.open("voina-i-mir.txt", ios::in);
    if (!file.is_open()) {
        cerr << "can't open source file!" << endl; 
        return EXIT_FAILURE;
    };
    
    file.seekg(0, ios::end);
    size_t fSize = file.tellg();
    file.seekg(0);
    string source(fSize, ' ');
    file.read(&source[0], fSize);

    const string vowels = "АОИЕЁЭЫУЮЯаоиеёэыуюя";

	// Count_if_find
	Timer timer("Count_if + find");
    uint16_t res = Count_if_find(source, vowels);
    timer.print();
    cout <<	res << " vowel letters." << endl;

    // Count_if_for
    timer.start("Count_if + for");
    res = Count_if_for(source, vowels);
    timer.print();
    cout <<	res << " vowel letters." << endl;

    // For_find
    timer.start("For + find");
    res = For_find(source, vowels);
    timer.print();
    cout <<	res << " vowel letters." << endl;

    // x2_For
    timer.start("2 x For");
    res = x2_For(source, vowels);
    timer.print();
    cout <<	res << " vowel letters." << endl;
	
    if (file.is_open())
        file.close();

    // return o.k.
    return EXIT_SUCCESS;
} // main

// *** Урок 05.  Алгоритмы STL / Associative STL containers

#include <locale.h>  // Для setlocale (LC_ALL, "")
#include <iostream>

#include <unordered_set>
#include <array>
#include <iterator>
#include <random>
#include <algorithm>
#include <sstream>
#include <map>

using namespace std;

/*
 * 1. Создать шаблонную функцию, которая принимает итераторы на начало и конец последовательности слов,
 *    и выводящую в консоль список уникальных слов (если слово повторяется больше 1 раза, то вывести его надо один раз).
 *    Продемонстрировать работу функции, передав итераторы различных типов.
 */

template <typename T, typename It>
void displayUnique (const It& begin, const It& end) {
	unordered_set <T> atoms;
	copy (begin, end, inserter (atoms, atoms.end ()));
    // display the result
    cout << "result <" << atoms.size() << ">: ";
	copy (atoms.begin (), atoms.end (), ostream_iterator <T> (cout, " "));
	cout << endl;
}

/*
 * 2. Используя ассоциативный контейнер, напишите программу, которая будет считывать данные введенные пользователем
 *    из стандартного потока ввода и разбивать их на предложения.
 *    Далее программа должна вывести пользователю все предложения, а также количество слов в них,
 *    отсортировав предложения по количеству слов.
 */


void clearPhrase (string& phrase) {
    
    const string chars (" \n\t\r");
    replace_if (phrase.begin(), phrase.end(),
                            [&chars](const char &c)
                            {
                            return chars.find(c) != string::npos;
                            },
                            ' ');

	const auto begin = phrase.find_first_not_of (' ');
	const auto end   = phrase.find_last_not_of  (' ');

    phrase = (begin != string::npos) ? phrase.substr(begin, end - begin + 1) : string{};
}

uint32_t countWords (string const& phrase) {
    stringstream stream;

    // sneaky way to use the string as the buffer to avoid copy.
    stream.rdbuf()->pubsetbuf ((char*)phrase.c_str(), phrase.length() );
    return distance (istream_iterator <string> (stream), istream_iterator<string>());
}

void calcValues (const string& source, multimap <uint32_t, string>& res) {

    auto itBegin = begin (source);
	auto itEnd   = end   (source);

    auto if_phrase = [] (const char& c)
                        {
                        const string chars = ".!?"+'\0';
                        return (chars.find(c) != string::npos);
                        };
    // first phrase
    auto itSrc = find_if(itBegin, itEnd, if_phrase);
    // get next phrase
	while (itBegin < itEnd && distance (itBegin, itSrc)) {
        // phrase
        string phrase = {itBegin, itSrc};
        // clean phrase
        clearPhrase(phrase);
        // count of words
        uint32_t count = countWords (phrase);
        if (count)
            res.insert ({countWords (phrase), move (phrase)});
        // next phrase
		itBegin = next (itSrc, 1);
		itSrc   = find_if (itBegin, itEnd, if_phrase);
	}
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

    cout << "Lesson 05. Associative STL containers.\n\n";

    // * Exercises #1
    cout << "Exercise #1.\n";

    const size_t MAX_SIZE = 10;
    // * example with array
    array <string, MAX_SIZE> example_1;
    // distribution int of RNG
    uniform_int_distribution <int> RNG_INT(0, 5);
    // create & initialize RNG
    mt19937 gen {random_device()()};
    // fill the array
    // generate (begin(example_1), end(example_1), [&RNG_INT, &gen]() {return "0" + to_string (RNG_INT(gen));});
    for_each (begin(example_1), end(example_1),
        [&RNG_INT, &gen] (string &value)
        {
        char buf[5];
        sprintf (buf, "%02d", RNG_INT(gen));
        value = buf;}
        );
    // display the array
    cout << "source array <" << example_1.size() << ">: ";
    copy (begin(example_1), end(example_1), ostream_iterator<string> (cout, " "));
    cout << endl;
    // display the result
    displayUnique <string> (example_1.begin(), example_1.end());

    // * example with string stream
    stringstream example_2;
    copy (begin(example_1), end(example_1), ostream_iterator<string> (example_2, " "));
    cout << "source strstream <the same data> ... " << endl;
    displayUnique <string> (istream_iterator<string>{example_2}, {});

    // * Exercise #2
    cout << "\nExercise #2.\n";

    cout << "input text: ";
    string source;
    cin.unsetf(ios::skipws);
    // get text
    getline (cin, source, '\n');
    // result
    multimap <uint32_t, string> result;
    // calculation
    calcValues (source, result);
    // display result
    cout << "top 5 longest sentences:" << endl;
    int count = 5;
    for (auto item = result.rbegin(); item != result.rend() && count > 0; item++, count--) {
        cout << item->first << " word[s] =>> " << item->second << endl;
    }
    
    // return o.k.
    return EXIT_SUCCESS;
} // main

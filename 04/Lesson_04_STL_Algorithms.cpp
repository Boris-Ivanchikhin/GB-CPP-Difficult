// *** Урок 04.  Алгоритмы STL / STL Algorithms

#include <locale.h>  // Для setlocale (LC_ALL, "")
#include <iostream>
#include <iterator>
#include <random>
#include <vector>
#include <algorithm>
#define _USE_MATH_DEFINES
#include <math.h>
#include <numeric>

using namespace std;

/*
 * 1. Имеется отсортированный массив целых чисел. Необходимо разработать функцию insert_sorted,
 *    которая принимает вектор и новое число и вставляет новое число в определенную позицию в векторе,
 *    чтобы упорядоченность контейнера сохранялась.
 *    Реализуйте шаблонную функцию insert_sorted, которая сможет аналогично работать с любым контейнером,
 *    содержащим любой тип значения.
 */

template <typename Container, typename Value>
void insert_sorted(Container& cont, const Value& val) {

	auto it = upper_bound(cont.begin(), cont.end(), val);
	cont.insert(it, val);
}

/*
 * 2. Сгенерируйте вектор a, состоящий из 100 вещественный чисел, представляющий собой значения аналогового сигнала.
 *    На основе этого массива чисел создайте другой вектор целых чисел b, представляющий цифровой сигнал,
 *    в котором будут откинуты дробные части чисел. Выведите получившиеся массивы чисел.
 *    Посчитайте ошибку, которой обладает цифровой сигнал по сравнению с аналоговым.
 */

class wave
{
private:
    float m_amplitude; // Amplitude (A) — we can use it as a volume
    float m_frequency; // Frequency (f) — how many cycles per second it does
    float m_phase;     // Phase (phi)   — starting point of sine wave
//  float m_time;      // Time (t)      — just a time counter value (in our case)

public:
    // constructor
    wave(const float _amplitude = 10.0f, const float _frequency = 1.0f, const float _phase = 0.0f)
        : m_amplitude(_amplitude), m_frequency(_frequency), m_phase (_phase) {};
    // destructor
    ~wave() = default;
    // methods
    void getWave (vector<float_t>& _signal) {
        const size_t count = _signal.size();
        generate(_signal.begin(), _signal.end(),
                [=, m_time = 0]() mutable
                {return m_amplitude * sin(m_time++ * 2 * M_PI / count + m_phase);}
                );
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

    cout << "Lesson 03. STL Algorithms.\n\n";

    // * Exercises #1
    cout << "Exercise #1.\n";

    const size_t MAX_SIZE1 = 10;
    vector <int> vec(MAX_SIZE1);
    // distribution int of RNG
    uniform_int_distribution <int> RNG_INT(0, 20);
    // create & initialize RNG
    mt19937 gen {random_device()()};
    // fill the vector
    generate (vec.begin(), vec.end(), [&RNG_INT, &gen]() {return RNG_INT(gen);});
    // sorting the vector
    sort(vec.begin(), vec.end());
    // display the vector
    cout << "source vector   : ";
    copy (vec.begin(), vec.end(), ostream_iterator<int> (cout, " "));
    cout << endl;
    int rValue = RNG_INT(gen);
    cout << "inserting a random value: " << rValue << endl;
    // inserting the rValue
    insert_sorted(vec, rValue);
    cout << "resulting vector: ";
    copy (vec.begin(), vec.end(), ostream_iterator<int> (cout, " "));
    cout << endl;
    
 
    // * Exercise #2
    cout << "\nExercise #2.\n";

    const size_t MAX_SIZE2 = 100;
    // source wave
    wave Wave{};
    // analog signal
    vector <float_t> analog(MAX_SIZE2);
    // fill the vector
    Wave.getWave (analog);
    // display the signal
    cout << "analog signal  >> ";
    copy (analog.begin(), analog.end(), ostream_iterator<float_t> (cout, " "));
    cout << endl;
    // digital signal
    vector<int64_t> digital(MAX_SIZE2);
    // (!) рука не поднимается просто отбросить дробную часть без мат.округления
    // copy(analog.begin(), analog.end(), digital.begin());
    for_each(digital.begin(), digital.end(), 
            [analog, n = 0](int64_t &value) mutable
            {value = static_cast<int64_t>(round(analog[n++]));}
            );
    // display the signal
    cout << "digital signal >> ";
    copy (digital.begin(), digital.end(), ostream_iterator<int64_t> (cout, " "));
    cout << endl;
    // value of error
    float_t mathErr{};
    for_each(digital.begin(), digital.end(), 
            [&mathErr, analog, n = 0](int64_t value) mutable
            {mathErr += pow (analog[n++] - static_cast<float_t>(value), 2);}
            );
    cout << "value of digital signal error: " << mathErr << endl;

    // auto mathErr2 = inner_product(analog.begin(), analog.end(), digital.begin(), 0.0f,
	// 	plus<float_t>(), 
	// 	[](float_t a, int64_t b) 
	// 	{return pow(a - static_cast<float_t>(b), 2);}
    //     );
    // cout << "value of digital signal error: " << mathErr2 << endl;
    
    // return o.k.
    return EXIT_SUCCESS;
} // main

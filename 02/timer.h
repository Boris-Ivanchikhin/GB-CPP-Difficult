#ifndef _TIMER_H_
#define _TIMER_H_ 

#include <chrono>
#include <iostream>

using namespace std;

class Timer
{
private:
    using clock_t = chrono::high_resolution_clock;
    using second_t = chrono::duration<double, ratio<1>>;
    string m_name;
    chrono::time_point<clock_t> m_beg;
    double elapsed() const {
        return chrono::duration_cast<second_t>(clock_t::now() - m_beg).count();
    };

public:
    // constructors
    Timer() : m_beg(clock_t::now()) {};
    Timer(string name) : m_name(name), m_beg(clock_t::now()) {};
    // copy constructor
    Timer(const Timer& obj) : m_name(obj.m_name), m_beg(obj.m_beg) {};
    // move constructor
    Timer(Timer&& obj) = delete;
    // destructor
    ~Timer() = default;
    // copying assignment operator
    void operator = (const Timer& obj) {
        m_name = obj.m_name;
        m_beg  = obj.m_beg;
    };
    // moving assignment operator
    void operator = (Timer&& obj) = delete;
    // start
    void start(std::string name) {
        m_name = name;
        m_beg  = clock_t::now();
    };
    // print
    void print() const {
        cout << m_name << " -> " << elapsed() * 1000 << " ms" << '\n';
    };
}; // class Timer

#endif // _TIMER_H_
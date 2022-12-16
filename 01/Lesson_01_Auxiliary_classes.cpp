// *** Урок 01.  Вспомогательные классы для удобной работы / Auxiliary classes for convenient work

#include <locale.h>  // Для setlocale (LC_ALL, "")
#include <iostream>
#include <optional>
#include <iomanip>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <tuple>

using namespace std;

/*
 * 1. Создайте структуру Person с 3 полями: фамилия, имя, отчество. 
 *    Поле отчество должно быть опционального типа, т.к. не у всех людей есть отчество. 
 *    Перегрузите оператор вывода данных для этой структуры. 
 *    Также перегрузите операторы < и == (используйте tie).
 */

class Person
{
private:	
	string firstName, lastName;
	optional<string> middleName;

public:
	Person(const string& _lName, const string& _fName, const optional<string>& _mName) : 
        lastName(_lName), firstName(_fName), middleName(_mName) {};
    ~Person() = default;

    string First () const {return firstName;};
    string Last () const {return lastName;};
    string Middle () const {return middleName.value_or("");};

    // operators
	friend ostream& operator<< (ostream&, const Person&);
	friend bool operator< (const Person&, const Person&);
	friend bool operator== (const Person&, const Person&);
}; // class Person

ostream& operator << (ostream& out, const Person& obj) {
    ios oldState(nullptr);
    oldState.copyfmt(out);
	out << setfill (' ') << right << setw(14) << obj.lastName << setw(12) << obj.firstName << setw(16) << obj.middleName.value_or("");
    out.copyfmt(oldState);
    // return
	return out;
}

bool operator < (const Person& obj1, const Person& obj2) {
    // return
	return tie(obj1.lastName, obj1.firstName, obj1.middleName) < tie(obj2.lastName, obj2.firstName, obj2.middleName);
}

bool operator == (const Person& obj1, const Person& obj2) {
    // return
	return tie(obj1.lastName, obj1.firstName, obj1.middleName) == tie(obj2.lastName, obj2.firstName, obj2.middleName);
}

/*
 * 2. Создайте структуру PhoneNumber с 4 полями:
 *    · код страны (целое число)
 *    · код города (целое число)
 *    · номер (строка)
 *    · добавочный номер (целое число, опциональный тип)
 *    Для этой структуры перегрузите оператор вывода. 
 *    Необходимо, чтобы номер телефона выводился в формате: +7(911)1234567 12, где:
 *    · 7       –> это номер страны, 
 *    · 911     –> номер города, 
 *    · 1234567 –> номер, 
 *    · 12      –> добавочный номер. 
 *    Если добавочного номера нет, то его выводить не надо. 
 *    Также перегрузите операторы < и == (используйте tie)
 */

using uint = unsigned int; // С++11

struct PhoneNumber
{
	uint countryCode, cityCode;
	string number;
	optional<uint> extNumber;

	PhoneNumber(const uint& _countryCode, const uint& _cityCode, const string& _number, const optional<uint>& _extNumber) : 
        countryCode(_countryCode), cityCode(_cityCode), number(_number), extNumber(_extNumber) {};
    PhoneNumber () = default;
    ~PhoneNumber() = default;
    void Clear ();

    // operators
    friend ostream& operator << (ostream&, const PhoneNumber&);
	friend bool operator < (const PhoneNumber&, const PhoneNumber&);
}; // struct PhoneNumber

void PhoneNumber::Clear () {
    countryCode= NULL;
    cityCode = NULL;
    number.clear();
    extNumber = nullopt;
}

ostream& operator << (ostream& out, const PhoneNumber& obj) {
    ios oldState(nullptr);
    oldState.copyfmt(out);
	// out << left << showpos << setfill('0') << setw(2) << obj.countryCode << "(" << setw(3) << noshowpos << obj.cityCode << ") "
    //     << setw(7) << obj.number << (obj.extNumber.has_value() ? " " : "") << setw(2) << (obj.extNumber.has_value() ? obj.extNumber.value() : '\0');
    out << left << showpos << (int)obj.countryCode << noshowpos << " (" << obj.cityCode << ") " << obj.number;
    if (obj.extNumber.has_value())
        out << " " << obj.extNumber.value();
    out.copyfmt(oldState);
    // return
	return out;
}

bool operator < (const PhoneNumber& obj1, const PhoneNumber& obj2) {
	return (tie(obj1.countryCode, obj1.cityCode, obj1.number, obj1.extNumber) < tie(obj2.countryCode, obj2.cityCode, obj2.number, obj2.extNumber));
}

/*
 * 3. Создайте класс PhoneBook, который будет в контейнере хранить пары: Человек – Номер телефона. 
 *    Конструктор этого класса должен принимать параметр типа ifstream – поток данных, полученных из файла. 
 *    В теле конструктора происходит считывание данных из файла и заполнение контейнера. 
 *    Класс PhoneBook должен содержать перегруженный оператор вывода, для вывода всех данных из контейнера в консоль.
 * 
 *    В классе PhoneBook реализуйте:
 *    · метод SortByName, который должен сортировать элементы контейнера по фамилии людей в алфавитном порядке. 
 *      Если фамилии будут одинаковыми, то сортировка должна выполняться по именам, если имена будут одинаковы, 
 *      то сортировка производится по отчествам. Используйте алгоритмическую функцию sort;
 *    · метод SortByPhone, который должен сортировать элементы контейнера по номерам телефонов.
 *      Используйте алгоритмическую функцию sort;
 *    · метод GetPhoneNumber, который принимает фамилию человека, а возвращает кортеж из строки и PhoneNumber. 
 *      Строка должна быть пустой, если найден ровно один человек с заданном фамилией в списке. 
 *      Если не найден ни один человек с заданной фамилией, то в строке должна быть запись «not found», 
 *      если было найдено больше одного человека, то в строке должно быть «found more than 1».
 *    · метод ChangePhoneNumber, который принимает человека и новый номер телефона и, 
 *      если находит заданного человека в контейнере, то меняет его номер телефона на новый, иначе ничего не делает.
 */

class ByName
{
public:
    bool operator() (const pair<Person, PhoneNumber>& _left, const pair<Person, PhoneNumber>& _right) {
        // return
        return (_left.first < _right.first);
    };
}; // the functor : class ByName

class ByPhone
{
public:
    bool operator() (const pair<Person, PhoneNumber>& _left, const pair<Person, PhoneNumber>& _right) {
        // return
        return (_left.second < _right.second);
    };
}; // the functor : class ByPhone

class PhoneBook
{
private:
	vector <pair <Person, PhoneNumber>> db;

public:
	PhoneBook (ifstream&);
    ~PhoneBook() = default;

	void SortByName();
	void SortByPhone();
	tuple <string, PhoneNumber> GetPhoneNumber(const string&);
	void ChangePhoneNumber(const Person&, const PhoneNumber&);

	friend ostream& operator << (ostream&, PhoneBook);
}; // class PhoneBook

PhoneBook::PhoneBook(ifstream& fstr) {
	string firstName, lastName, middleName, extNumber;

	if (fstr.is_open())	{
        PhoneNumber _phone;
		while (fstr >> lastName >> firstName >> middleName
                    >> _phone.countryCode >> _phone.cityCode >> _phone.number >> extNumber)
		{
            auto _mName = (middleName.empty () || middleName == "-") ? nullopt : optional<string> (middleName);
			Person _person(lastName, firstName, _mName);

            _phone.extNumber = (extNumber.empty () || extNumber == "-") ? nullopt : optional<int> (stoi(extNumber));
			db.push_back(make_pair(_person, _phone));
            _phone.Clear();
		}
	}
	fstr.close();
}

tuple <string, PhoneNumber> PhoneBook::GetPhoneNumber(const string& _lastName) {
	PhoneNumber phone{};
	uint found {};

	for_each(db.begin(), db.end(),
        [&](const auto& record) {
			if (record.first.Last() == _lastName) {
				phone = record.second;
				++found;
		    }
        });
    
    string answer {};
	if (!found) 
        answer = "not found";
	else if (found > 1) 
        answer = "found more than 1";
    // return
	return make_tuple(answer, phone);
}

void PhoneBook::ChangePhoneNumber(const Person& _person, const PhoneNumber& _newPhone) {
	auto found = find_if(db.begin(), db.end(), 
		[_person](const auto& record) {
			return (record.first == _person);
		});
	if (found != db.end()) 
        found->second = _newPhone;
}

ostream& operator << (ostream& out, PhoneBook obj) {	
	for (const auto& [person, phone] : obj.db)
		out << person << string (5, ' ') << phone << endl;
    // return
	return out;
}

void PhoneBook::SortByName() {
	sort(db.begin(), db.end(), ByName{});
    //     [](const auto& _left, const auto& _right) {
    //         return (_left.first < _right.first);
    // });
}

void PhoneBook::SortByPhone() {
	sort(db.begin(), db.end(), ByPhone {});
    //     [](const auto& _left, const auto& _right) {
    //         return (_left.second < _right.second);
    // });
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

    cout << "Lesson 01. Auxiliary classes for convenient work.\n\n";

    // * Exercises ##1,2,3
    cout << "Exercises ##1,2,3:\n";

    ifstream file("PhoneBook.txt"); // file path «PhoneBook.txt»
    cout << "-----PhoneBook.txt------" << endl;
    PhoneBook book(file);
    cout << book;
    cout << "------SortByPhone-------" << endl;
    book.SortByPhone();
    cout << book;
    cout << "------SortByName--------" << endl;
    book.SortByName();
    cout << book;
    cout << "-----GetPhoneNumber-----" << endl;
    // a lambda function that takes a surname and outputs a person's phone number, 
    //  or a string with an error
    auto print_phone_number = [&book](const string& surname) {
        cout << surname << "\t";
        auto answer = book.GetPhoneNumber(surname);
        if (get<0>(answer).empty())
            cout << get<1>(answer);
        else
            cout << get<0>(answer);
        cout << endl;
    };
    // lambda calls
    print_phone_number("Ivanov");
    print_phone_number("Petrov");
    cout << "----ChangePhoneNumber----" << endl;
    book.ChangePhoneNumber(Person{ "Kotov", "Vasilii", "Eliseevich" },
    PhoneNumber{7, 123, "15344458", nullopt});
    book.ChangePhoneNumber(Person{ "Mironova", "Margarita", "Vladimirovna" },
    PhoneNumber{ 16, 465, "9155448", 13 });
    cout << book;

    // * Exercise #2
    //cout << "\nExercise #2.\n";

    // * Exercises #3
    //cout << "\nExercise #3.\n";

    // return o.k.
    return EXIT_SUCCESS;
} // main

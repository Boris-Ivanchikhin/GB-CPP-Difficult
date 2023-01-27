// *** Урок 07.  Пространство имен и бинарная серилизация / Namespace and binary serialization


#include <locale.h>  // Для setlocale (LC_ALL, "")
#include "Lesson_07_Namespace_and_binary_serialization.h"



/*
 * 3. Создать класс StudentsGroup, который реализует интерфейсы:
 *
 *      class IRepository {
 *          virtual void Open() = 0; // бинарная десериализация из файла
 *          virtual void Save() = 0; // бинарная сериализация в файл
 *      };
 * 
 *      class IMethods {
 *          virtual double GetAverageScore (const FullName& name) = 0;
 *          virtual string GetAllInfo (const FullName& name) = 0;
 *          virtual string GetAllInfo () = 0;
 *      };
 * 
 */

namespace Exercise_3 {

    
    auto FullName::Open (ifstream &in)  -> size_t
    {
        const auto pos = in.tellg ();
        size_t sz{};
        char *buf;

        // firstName
        in.read (reinterpret_cast <char*> (&sz), sizeof (sz));
        firstName.clear();
        if (sz) {
            buf = new char [sz];
            in.read (buf, sz);
            firstName.append (buf, 0, sz);
            //firstName.resize (sz);
            delete []buf;
        }

        // lastName
        lastName.clear();
        in.read (reinterpret_cast <char*> (&sz), sizeof (sz));
        if (sz) { 
            buf = new char [sz];
            in.read (buf, sz);
            lastName.append (buf, 0, sz);
            //lastName.resize (sz);
            delete []buf;
        }

        // patronymic
        patronymic.clear();
        in.read (reinterpret_cast <char*> (&sz), sizeof (sz));
        if (sz) {
            buf = new char [sz];
            in.read (buf, sz);
            patronymic.append (buf, 0, sz);
            //patronymic.resize (sz);
            delete []buf;
        }

        // return
        return static_cast <ssize_t> (in.tellg () - pos);

    } // FullName::Open

    auto FullName::Save (ofstream &out) -> size_t
    {
        const auto pos = out.tellp ();

        // firstName
        size_t sz = firstName.length ();
        out.write (reinterpret_cast <const char*> (&sz), sizeof (sz));
        out.write (firstName.c_str(), firstName.length());
        
        // lastName
        sz = lastName.length ();
        out.write (reinterpret_cast <const char*> (&sz), sizeof (sz));
        out.write (lastName.c_str (), lastName.length());
        
        // patronymic
        sz = patronymic.length ();
        out.write (reinterpret_cast <const char*> (&sz), sizeof (sz));
        out.write (patronymic.c_str (), patronymic.length());

        // return
        return static_cast <ssize_t> (out.tellp () - pos);

    } // FullName::Save

    

    auto Student::Open (ifstream &in) -> size_t
    {
        const auto pos = in.tellg ();

        // FIO
        FIO.Open (in);

        // avgGrade
        in.read (reinterpret_cast <char*> (&avgGrade), sizeof (avgGrade));

        // grades (vector <uint16_t>)
        grades.clear();
        grades.shrink_to_fit();
        size_t sz{};
        in.read (reinterpret_cast <char*> (&sz), sizeof (sz));
        grades.resize (sz);
        in.read (reinterpret_cast <char*> (&grades[0]), sizeof (uint16_t) * sz);

        // return
        return static_cast <ssize_t> (in.tellg () - pos);

    } // Student::Open

    auto Student::Save (ofstream &out) -> size_t
    {
        const auto pos = out.tellp ();

        // FIO
        FIO.Save (out);

        // avgGrade
        out.write (reinterpret_cast <const char*> (&avgGrade), sizeof (avgGrade));

        // grades (vector <uint16_t>)
        size_t sz = grades.size ();
        out.write (reinterpret_cast <const char*> (&sz), sizeof (sz));
        out.write (reinterpret_cast <const char*> (&grades[0]), sizeof (uint16_t) * sz);

        // return
        return static_cast <ssize_t> (out.tellp () - pos);

    } // Student::Save

    

    auto StudentsGroup::Open (ifstream &in) -> size_t
    {
        const auto pos = in.tellg ();

        // students (vector <Student>)
        students.clear();
        students.shrink_to_fit();
        size_t sz{};
        in.read (reinterpret_cast <char*> (&sz), sizeof (sz));
        for (size_t i = 0; i < sz; ++i)
        {
            Student iobj;
            iobj.Open (in);
            students.emplace_back (iobj);
        }

        // return
        return static_cast <ssize_t> (in.tellg () - pos);

    } // StudentsGroup::Open

    auto StudentsGroup::Save (ofstream &out) -> size_t
    {
        const auto pos = out.tellp ();

        // students (vector <Student>)
        size_t sz = students.size ();
        out.write (reinterpret_cast <const char*> (&sz), sizeof (sz));
        for (size_t i = 0; i < sz; ++i) {
            auto& iobj = students [i];
            iobj.Save (out);
        }

        // return
        return static_cast <ssize_t> (out.tellp () - pos);

    } // StudentsGroup::Save

    optional <double> StudentsGroup::GetAverageScore (const FullName& name) {
        auto it = find_if (students.begin(), students.end(),
                    [&] (const auto& value) {
                        return (tie (value.FIO.firstName, value.FIO.lastName, value.FIO.patronymic) ==
                                tie (name.firstName, name.lastName, name.patronymic));
                    });

        if (it == students.end())
            return nullopt; // {}

        // return
        return it->avgGrade;

    } // StudentsGroup::GetAverageScore

    optional <string> StudentsGroup::GetAllInfo (const FullName& name) {
        
        auto it = find_if (students.begin(), students.end(),
                    [&] (const auto& value) {
                        return (tie (value.FIO.firstName, value.FIO.lastName, value.FIO.patronymic) ==
                                tie (name.firstName, name.lastName, name.patronymic));
                    });

        if (it == students.end())
            return nullopt; // {}

        stringstream _ss {};

        //_ss << "\n#" << i + 1 << endl;
        _ss << "first name: " << it->FIO.firstName << endl;
        _ss << "last name: "  << it->FIO.lastName << endl;

        if (!it->FIO.patronymic.empty())
            _ss << "second name: " << it->FIO.patronymic << endl;

        _ss << "avg.grade: " << it->avgGrade << endl;

        _ss << "grades: ";
        for (size_t j = 0; j < it->grades.size (); ++j)
            _ss << it->grades [j] << " ";
        _ss << endl;

        // return
        return _ss.str();

    } // StudentsGroup::GetAllInfo

    string StudentsGroup::GetAllInfo () {

        stringstream _ss;
        int count {};

        for (size_t i = 0; i < students.size(); ++i) {
            auto info = GetAllInfo (students [i].FIO);
            if (!info.has_value ())
                continue;
            _ss << "\n#" << count + 1 << endl;
            _ss << info.value ();
        }
            
        // return
        return _ss.str();

    } // StudentsGroup::GetAllInfo

    // This function fills in a Student object based on user input.
    void PromptForStudent (Student* student) {

        cin.ignore(256, '\n');

        // class FullName
        auto &full_name =  student->FIO;

        cout << "\nenter first name: ";
        getline (cin, full_name.firstName);

        cout << "enter last name: ";
        getline (cin, full_name.lastName);

        cout << "enter second name: ";
        getline (cin, full_name.patronymic);

        // grades (vector <uint16_t>)
        float _total{};
        while (true) {
            cout << "enter a grade (or leave blank to finish): ";
            string _str_value;
            getline (cin, _str_value);
            if (_str_value.empty ()) {
                break;
            }
            stringstream _ss {_str_value};
            int _number;
            _ss >> _number;

            student->grades.emplace_back (static_cast <uint16_t> (_number));
            _total += static_cast <float> (_number);
        }

        student->avgGrade = _total / static_cast <float> (student->grades.size ());

    } // PromptForStudent

} // namespace Exercise_3


// // Function called at shutdown
// void endFunction (void)
// {
//     // Output of the shutdown message
//     puts ("\nthe work is completed!");
// } // endFunction

// main
int main_07 (void) {

    // // Russian language in the console
    // setlocale (LC_ALL, "");

    //  // Registration of the function to be called during normal shutdown
    // if (atexit (endFunction)) {
    //     // Checking the registration of the endFunction
    //     puts ("post- function registration error!");
    // }

    cout << "Lesson 07. Namespace and binary serialization.\n\n";

    
    // * Exercise #3
    cout << "\nExercise #3.\n";

    Exercise_3::StudentsGroup obj_students_group;

    // serialize
    cin.ignore(256, '\n');
    cout << "\nproceed to serialize?(Y/N): ";
    char answer = getchar ();
    if (answer == 'Y' || answer == 'y') {
        ofstream out;
        out.open (Exercise_3::f_name, ios::out | ios::trunc | ios::binary);
        if (!out.is_open ()) {
            cout << "failed to open «" << Exercise_3::f_name << "»!" << endl;
            return EXIT_FAILURE;
        }
        // adding students to a group
        do {
            Exercise_3::Student _student{};
            Exercise_3::PromptForStudent (&_student);
            obj_students_group.addStudent (&_student);
            cout << "proceed the next?(Y/N): ";
            answer = getchar ();
            if (answer != 'Y' && answer != 'y')
                break;
        } while (true);
        // write to the file
        if (!obj_students_group.Save (out)) {
            cout << "failed to write «"<< Exercise_3::f_name << "»!" << endl;
            //return EXIT_FAILURE;
        } else {
            cout << "file «" << Exercise_3::f_name << "» was successfully written." << endl;
        }
        // close
        if (out.is_open ())
            out.close ();
    }

    // deserialize
    cin.ignore(256, '\n');
    cout << "\nproceed to deserialize?(Y/N): ";
    answer = getchar ();
    if (answer == 'Y' || answer == 'y') {
        ifstream in;
        in.open (Exercise_3::f_name, ios::in | ios::binary);
        if (!in.is_open ()) {
            cout << "failed to open «" << Exercise_3::f_name << "»!" << endl;
            //return EXIT_FAILURE;
        } else {
            if (!obj_students_group.Open (in)) {
                cout << "failed to parse from «"<< Exercise_3::f_name << "»!" << endl;
                //return EXIT_FAILURE;
            } else
                cout << obj_students_group.GetAllInfo ();
        }
        // close
        if (in.is_open ())
            in.close ();
    }

    // return o.k.
    return EXIT_SUCCESS;
} // main


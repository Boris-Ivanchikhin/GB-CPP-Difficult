#ifndef LESSON_07
#define LESSON_07

#include <iostream>


#include <fstream>
#include <sstream>
#include <vector>
#include <optional>
#include <algorithm>
#include <tuple>

using namespace std;

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

    const string f_name {"Exercise-03.dat"};

    struct FullName;

    // * class IRepository

    class IRepository
    {
        public:
            // binary deserialization from a file
            virtual auto Open (ifstream &) -> size_t = 0;
            // binary serialization to a file
            virtual auto Save (ofstream &) -> size_t = 0; 

            IRepository() = default;
            ~IRepository() = default;
    }; // class IRepository

    // * class IMethods

    class IMethods
    {
        public:
            virtual optional <double> GetAverageScore (const FullName &) = 0;
            virtual optional <string> GetAllInfo (const FullName &) = 0;
            virtual string GetAllInfo () = 0;

            IMethods() = default;
            ~IMethods() = default;
    }; // class IMethods

    // * class FullName 

    class FullName : public IRepository
    {
        public :
            string firstName;
            string lastName;
            string patronymic;

            // interface IRepository
            auto Open (ifstream &) -> size_t override; // binary deserialization from a file
            auto Save (ofstream &) -> size_t override; // binary serialization to a file

            FullName (const string& _firstName, const string& _lastName,const string& _patronymic)
            : firstName (_firstName), lastName (_lastName) , patronymic (_patronymic)
            {};
            FullName ()
            : firstName (""), lastName ("") , patronymic ("")
            {};
            ~FullName () = default;

    }; // class FullName

    // * class Student
    
    class Student : public IRepository
    {
        public:
            FullName FIO;
            vector <uint16_t> grades;
            float avgGrade;

            // interface IRepository
            auto Open (ifstream &) -> size_t override; // binary deserialization from a file
            auto Save (ofstream &) -> size_t override; // binary serialization to a file

            Student() : avgGrade (0.0f) {};
            ~Student() = default;

    }; // class Student

    // * class StudentsGroup

    class StudentsGroup : public IRepository, public IMethods
    {
        public:
            // interface IRepository
            auto Open (ifstream &) -> size_t override; // binary deserialization from a file
            auto Save (ofstream &) -> size_t override; // binary serialization to a file
            // interface IRepository
            optional <double> GetAverageScore (const FullName& name) override;
            optional <string> GetAllInfo (const FullName& name) override;
            string GetAllInfo () override;

            void addStudent (Student* _student) {
                if (_student != nullptr)
                    students.emplace_back (*_student);
            };

            StudentsGroup () = default;
            ~StudentsGroup() = default;

        private:
            vector <Student> students;

    }; // class StudentsGroup

}












#endif // LESSON_07
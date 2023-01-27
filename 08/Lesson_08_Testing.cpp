// *** Урок 08.  Тестирование / Testing

#include <locale.h>  // Для setlocale (LC_ALL, "")
#include <iostream>

#include <gtest/gtest.h>
#include "Lesson_07_Namespace_and_binary_serialization.h"

using namespace std;

/*
 * 1. Установить фреймворк Google Test.
 */

// См.файл «SetUp_Google_test_in_MSYS2.pdf»

/*
 * 2. Добавить unit-тесты в проект домашнего задания из седьмого или первого урока.
 */

class TestStudentsGroup : public testing::Test
{
public:

    const string _file_name = "Lesson-08.dat";
    // test data
    Exercise_3::StudentsGroup *sg;
    Exercise_3::Student *student1, *student2;

    // analog of the constructor
    void SetUp () override {
        
        // object StudentsGroup from Lesson 7 (Exercise #3)
        sg = new Exercise_3::StudentsGroup ();
        student1 = new Exercise_3::Student();
        student2 = new Exercise_3::Student();

        student1->FIO.firstName = "Ivan";
        student1->FIO.lastName = "Ivanov";
        student1->grades = {5, 4, 3};
        student1->avgGrade = 4.0f;
        sg->addStudent (student1);

        student2->FIO.firstName = "Petr";
        student2->FIO.lastName = "Petrov";
        student2->FIO.patronymic = "Petrovich";
        student2->grades = {3, 4, 3};
        student2->avgGrade = 3.3f;
        sg->addStudent (student2);
    };

    // analog of the destructor
    void TearDown() {
        delete sg;
        delete student1;
        delete student2;
    };
};

TEST_F (TestStudentsGroup, GetAverageScore) {
    
    auto _score = sg->GetAverageScore (student1->FIO);
    ASSERT_TRUE (_score.has_value());
    ASSERT_NEAR (_score.value (), student1->avgGrade, 1E-5);

    _score = sg->GetAverageScore (student2->FIO);
    ASSERT_TRUE (_score.has_value());
    ASSERT_NEAR (_score.value (), student2->avgGrade, 1E-5);
}

TEST_F (TestStudentsGroup, GetAllInfo) {
    
    auto _info = sg->GetAllInfo (student1->FIO);
    ASSERT_TRUE (_info.has_value());
    ASSERT_FALSE (_info.value ().empty());

    _info = sg->GetAllInfo (student2->FIO);
    ASSERT_TRUE (_info.has_value());
    ASSERT_FALSE (_info.value ().empty());
}

TEST_F (TestStudentsGroup, Serialization) {

    ofstream out;
    out.open (_file_name, ios::trunc | ios::binary);
    ASSERT_TRUE (out.is_open ());

    auto written = sg->Save (out);
    ASSERT_GT (written, sizeof (*sg));

    if (out.is_open ())
        out.close ();;
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

    cout << "Lesson 08. Testing.\n\n";

    // * Exercises #1
    cout << "Exercise #1.\n";
    cout << "see the explanations in file «SetUp_Google_test_in_MSYS2.pdf»" << endl;

    // * Exercise #2
    cout << "\nExercise #2.\n";

    testing::InitGoogleTest ();
    return RUN_ALL_TESTS ();


    // return o.k.
    return EXIT_SUCCESS;
} // main

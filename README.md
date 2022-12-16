#### C++: сложные моменты / C++: difficult moments  
#### Урок 01. Вспомогательные классы для удобной работы / Auxiliary classes for convenient work  

***

Имеется база сотрудников и номеров их телефонов. Требуется написать соответствующие структуры для хранения данных,
и заполнить контейнер записями из базы. Затем необходимо реализовать методы обработки данных,
а также вывести на экран всю необходимую информацию.  
> Важно!  
> Имена переменным, классам и функциям давайте осознанные, состоящие из слов на английском языке.

  1. Создайте структуру <b>Person</b> с 3 полями: фамилия, имя, отчество.
  Поле отчество должно быть опционального типа,
  т.к. не у всех людей есть отчество. Перегрузите оператор вывода данных для этой структуры.
  Также перегрузите операторы < и == (используйте <b>tie</b>).

  2. Создайте структуру <b>PhoneNumber</b> с 4 полями:
      - код страны (целое число);
      - код города (целое число);
      - номер (строка);
      - добавочный номер (целое число, опциональный тип).

      Для этой структуры перегрузите оператор вывода. Необходимо, чтобы номер телефона выводился в формате:
      +7(911)1234567 12, где 7 – это номер страны, 911 – номер города, 1234567 – номер, 12 – добавочный номер.
      Если добавочного номера нет, то его выводить не надо. Также перегрузите операторы < и == (используйте <b>tie</b>).

  3. Создайте класс <b>PhoneBook</b>, который будет в контейнере хранить пары: Человек – Номер телефона. 
  Конструктор этого класса должен принимать параметр типа <b>ifstream</b> – поток данных, полученных из файла. 
  В теле конструктора происходит считывание данных из файла и заполнение контейнера. 
  Класс <b>PhoneBook</b> должен содержать перегруженный оператор вывода, для вывода всех данных из контейнера в консоль.
  В классе <b>PhoneBook</b> реализуйте:
      - метод <b>SortByName</b>, который должен сортировать элементы контейнера по фамилии людей в алфавитном порядке.
      Если фамилии будут одинаковыми, то сортировка должна выполняться по именам, если имена будут одинаковы,
      то сортировка производится по отчествам. Используйте алгоритмическую функцию <b>sort</b>;
      - метод <b>SortByPhone</b>, который должен сортировать элементы контейнера по номерам телефонов. 
      Используйте алгоритмическую функцию <b>sort</b>;
      - метод <b>GetPhoneNumber</b>, который принимает фамилию человека, а возвращает кортеж из строки и PhoneNumber.
      Строка должна быть пустой, если найден ровно один человек с заданном фамилией в списке. 
      Если не найден ни один человек с заданной фамилией, то в строке должна быть запись «not found»,
      если было найдено больше одного человека, то в строке должно быть «found more than 1».
      - метод <b>ChangePhoneNumber</b>, который принимает человека и новый номер телефона и,
      если находит заданного человека в контейнере, то меняет его номер телефона на новый, иначе ничего не делает.

***

Функция main будет выглядеть так:

```C++
int main() {
    ifstream file("ХХХ"); // путь к файлу PhoneBook.txt
    PhoneBook book(file);
    cout << book;
    
    cout << "------SortByPhone-------" << endl;
    book.SortByPhone();
    cout << book;
    
    cout << "------SortByName--------" << endl;
    book.SortByName();
    cout << book;
    
    cout << "-----GetPhoneNumber-----" << endl;
        
    // лямбда функция, которая принимает фамилию и выводит номер телефона этого человека, 
    // либо строку с ошибкой 
    auto print_phone_number = [&book](const string& surname) {
        cout << surname << "\t";
        auto answer = book.GetPhoneNumber(surname);
        if (get<0>(answer).empty())
            cout << get<1>(answer);
        else
            cout << get<0>(answer);
        cout << endl;
        };
        
        // вызовы лямбды
        print_phone_number("Ivanov");
        print_phone_number("Petrov");
        
        cout << "----ChangePhoneNumber----" << endl;
        book.ChangePhoneNumber(Person{ "Kotov", "Vasilii", "Eliseevich" },
        PhoneNumber{7, 123, "15344458", nullopt});
        book.ChangePhoneNumber(Person{ "Mironova", "Margarita", "Vladimirovna" },
        PhoneNumber{ 16, 465, "9155448", 13 });
        cout << book;
}
```

У вас должен получиться следующий вывод:

  ![Alt text](/Screenshot_1.png?raw=true "Result 1")

  ![Alt text](/Screenshot_2.png?raw=true "Result 2")
  
<b>Входные данные:</b>

    Ilin Petr Artemovich 7 17 4559767 -
    Zaitsev Zakhar Artemovich 125 44 4164751 -
    Dubinin Aleksei Mikhailovich 7 473 7449054 -
    Solovev Artur Mikhailovich 4 940 2556793 -
    Gerasimov Miroslav Stanislavovich 7 367 7508887 -
    Makeev Marat - 77 4521 8880876 999
    Solovev Ivan Vladimirovich 7 273 5699819 5543
    Egorov Savelii Stanislavovich 77 4521 8880876 99
    Sokolov Arsenii - 93 163 1992257 16
    Davydov Filipp Grigorevich 7 247 1377660 -
    Morozov Vladimir Mikhailovich 37 2290 5613649 -
    Orekhov Matvei Petrovich 81 8281 7420182 2
    Titova Natalia - 93 163 1992257 9
    Markelov Dmitrii Vadimovich 19 7576 5734416 2
    Kozlovskii Artem Daniilovich 81 8281 7420182 1
    Kuznetsov Kirill Kirillovich 7 017 8346563 -
    Mironova Margarita Aleksandrovna 7 273 5699819 5542
    Kotov Vasilii Eliseevich 7 367 7508888 -
    Ivanov Daniil Maksimovich 7 366 7508887 -
    Aleksandrov Georgii - 493 7637 6114861 -

#### C++: сложные моменты / C++: difficult moments  
#### Урок 07. Пространство имен и бинарная сериализация / Namespace and binary serialization  

***

1. Установить библиотеку [protobuf](https://developers.google.com/protocol-buffers?hl=ru).  

2. С помощью компилятора <b>protobuf</b> в отдельном пространстве имен сгенерировать классы:  
    a. <b>FullName</b> с полями имя, фамилия, отчество (отчество опционально);  
    b. <b>Student</b> с полями полное имя, массив оценок, средний балл;  
    c. <b>StudentsGroup</b> с полем массив студентов.  

3. Создать класс <b>StudentsGroup</b>, который реализует интерфейсы:  

```C++
class IRepository {
    virtual void Open () = 0; // бинарная десериализация в файл
    virtual void Save () = 0; // бинарная сериализация в файл
};

class IMethods {
    virtual double GetAverageScore (const FullName& name) = 0;
    virtual string GetAllInfo (const FullName& name) = 0;
    virtual string GetAllInfo () = 0;
};
```
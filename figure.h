
#ifndef PROCEDURAL_FIGURE_H
#define PROCEDURAL_FIGURE_H
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>

namespace figure_space {

    // Параметры, общие для всех альтернатив:
    // Цвет фигуры (перечислимый тип)
    // {красный, оранжевый, желтый, зеленый, голубой, синий, фиолетовый}
    enum Color {
        RED = 0,
        ORANGE = 1,
        YELLOW = 2,
        GREEN = 3,
        BLUE = 4,
        SAPPHIRE = 5,
        PURPLE = 6
    };


    // Перечисляемый тип, определяющий тип фигуры
    enum eFigure {
        CIRCLE = 1,
        RECTANGLE = 2
    };

    // Структура, объединяющая все виды фигур
    class figure {
    public:
        // Статический метод (static) - можно вызвать без создания объекта (как функцию)
        // Считывает один объект figure и возвращает указатель на него
        static figure *read_one(std::ifstream &ifstr);

        virtual void multiMethod(figure *other, std::ofstream &ofst) = 0;
        virtual void multiCycle(std::ofstream &ofst) = 0;
        virtual void multiRect(std::ofstream &ofst) = 0;
        // Виртуальный метод "virtual" - требует от потомков переопределения этого метода
        // Чисто виртуальный "pure virtual" (= 0) - помимо необходимости переопределения не может быть определен в предке
        // Считывает уникальные свойства в объект
        virtual void read(std::ifstream &ifstr) = 0;
        // Выводит уникальные свойства объекта
        virtual void write(std::ofstream &ofstr) = 0;

        // gets
        figure *get_next();
        Color get_color();

        // sets
        void set_next(figure *_next);
        void set_color(Color _color);
    private:
        Color figure_color; // Цвет
        figure *next = nullptr; // Следующий элемент в списке
    };

    // Структура, описывающая круг
    class figure_circle : public figure {
    public:
        // Переопределяем методы предка
        void read(std::ifstream &ifstr) override;
        void multiMethod(figure *other, std::ofstream &ofst);
        void multiCycle(std::ofstream &ofst);
        void multiRect(std::ofstream &ofst);
        void write(std::ofstream &ofstr) override;
    private:
        // Координаты центра (целочисленные)
        int center_x;
        int center_y;
        // Радиус
        double radius;
    };

    // Структура, описывающая прямоугольник
    class figure_rectangle : public figure {
    public:
        // Переопределяем методы предка
        void read(std::ifstream &ifstr) override;
        void write(std::ofstream &ofstr) override;
        void multiMethod(figure *other, std::ofstream &ofst);
        void multiCycle(std::ofstream &ofst);
        void multiRect(std::ofstream &ofst);
    private:
        // Координаты левого верхнего угла (целочисленные)
        int upper_x;
        int upper_y;
        // Координаты нижнего правого угла (целочисленные)
        int bottom_x;
        int bottom_y;
    };

    // Контейнер на основе однонаправленного линейного списка.
    // В списке находятся элементы, каждый из них (кроме последнего) имеет ссылку на следующего.
    class figure_container {
    public:
        // Конструктор контейнера
        figure_container();
        void multiM(std::ofstream &ofst);
        // Очистка контейнер
        void clear();
        // Добавление элемента в контейнер
        void append(figure *new_element);
        // Чтение из файла
        void read(std::ifstream &ifstr);
        // Вывод в файл
        void write(std::ofstream &ofstr);
    private:
        // Начальный элемент
        figure *begin;
        // Последний элемент
        figure *end;
    };

    /// Объявление функций:
}

#endif //PROCEDURAL_FIGURE_H

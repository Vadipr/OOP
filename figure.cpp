#include "figure.h"

namespace figure_space {

    // Цвета для сопоставления при считывании с файла
    std::string color_strings[8] = {"red", "orange", "yellow", "green", "blue", "sapphire", "purple"};
    const int color_count = 7;

    // ЧТЕНИЕ ИЗ ФАЙЛА
    // Требования ко входному файлу:
    // *тип фигуры*
    // *цвет фигуры*
    // *характеристики фигуры*
    void figure_container::read(std::ifstream &ifstr) {
        // Чтение до end of file
        while (!ifstr.eof()) {
            // Считываем новый элемент, вызывая статический метод класса figure
            figure *new_element = figure::read_one(ifstr);
            if (new_element == nullptr) { // Если произошла ошибка при считывании
                std::cout << "Can't read file, because it has wrong structure." << std::endl;
                // Очищаем уже заполненные элементы списка
                clear();
                return;
            }
            // Добавляем в список считанный элемент
            append(new_element);
        }
    }

    void figure_container::append(figure *new_element) {
        if (begin == nullptr) { // Если список пустой
            begin = end = new_element;
        } else {
            end->set_next(new_element); // Последний элемент хранит ссылку на новый элемент
            end = new_element; // Последний элемент теперь - новый элемент
        }
        new_element->set_next(nullptr);
    }

    void figure_container::clear() {
        figure *it_next = nullptr; // Храним следующий элемент, т.к. будем удалять элементы в цикле
        for (figure *it = begin; it != nullptr; it = it_next) {
            it_next = it->get_next(); // Сохраняем следующий элемент
            delete it; // Удаляем элемент
        }
        begin = end = nullptr;
    }

    // Выводит уникальные свойства объекта
    void figure_circle::write(std::ofstream &ofstr) {
        // Вывод номера и цвета
        ofstr << color_strings[get_color()] << " ";
        ofstr << "circle: (" << calculate() << ")" << ": ";
        ofstr << "x0 = " << center_x << "; ";
        ofstr << "y0 = " << center_y << "; ";
        ofstr << "radius = " << radius << ";\n";
    }

    // Выводит уникальные свойства объекта
    void figure_rectangle::write(std::ofstream &ofstr) {
        // Вывод номера и цвета
        ofstr << color_strings[get_color()] << " ";
        ofstr << "rectangle (" << calculate() << ")" << ": ";
        ofstr << "x1 = " << bottom_x << "; ";
        ofstr << "y1 = " << bottom_y << "; ";
        ofstr << "x2 = " << upper_x << "; ";
        ofstr << "y2 = " << upper_y << ";\n";
    }

    // Выводит данные всего контейнера
    void figure_container::write(std::ofstream &ofstr) {
        int index = 0;
        if(begin == nullptr) { // Если пустой контейнер
            ofstr << "Empty container. " << std::endl;
            return;
        }
        for(figure *it = begin; it != nullptr; it = it->get_next()) {
            index++;
            // Вывод номера и цвета
            ofstr << index << ". ";
            it->write(ofstr);
        }
        std::cout << "Successfully printed to file." << std::endl;
    }

    figure *figure::read_one(std::ifstream &ifstr) {
        figure *res = nullptr; // Указатель на объект, который будет сформирован
        std::string readLine; // Считываемая строка
        char chars[255];
        ifstr.getline(chars, 255);
        // ВВОД ТИПА ФИГУРЫ
        readLine = std::string(chars); // Переводим в строку
        if (readLine == "circle") { // Если вводится круг
            res = new figure_circle;
        } else if (readLine == "rectangle") { // Если вводится прямоугольник
            res = new figure_rectangle;
        } else {
            return nullptr; // Произошла ошибка при вводе
        }
        // ВВОД ЦВЕТА ФИГУРЫ
        ifstr.getline(chars, 255);
        readLine = std::string(chars); // Переводим в строку
        for (int i = 0; i < color_count; i++) {
            if (readLine == color_strings[i]) { // Если узнали, что за цвет
                res->figure_color = (Color) i; // Сохраняем цвет
                break;
            } else if (i + 1 == color_count) { // Если цвет не был распознан
                delete res;
                return nullptr; // Произошла ошибка при вводе
            }
        }
        // ВВОД ОСТАЛЬНЫХ ХАРАКТЕРИСТИК
        res->read(ifstr);
        return res;
    }

    figure_container::figure_container() {
        // Задаем начальный и последний элементы в нулевой указатель
        end = begin = nullptr;
    }

    void figure_circle::read(std::ifstream &ifstr) {
        int _center_x, _center_y;
        double _radius;
        ifstr >> _center_x >> _center_y >> _radius;
        center_x = _center_x;
        center_y = _center_y;
        radius = _radius;
        if(!ifstr.eof()) { // Считаем переход на новую строку
            ifstr.get();
        }
    }

    double figure_circle::calculate() {
        return 2*PI*radius;
    }

    void figure_rectangle::read(std::ifstream &ifstr) {
        int _bottom_x, _bottom_y;
        int _upper_x, _upper_y;
        ifstr >> _bottom_x >> _bottom_y >> _upper_x >> _upper_y;
        bottom_x = _bottom_x;
        bottom_y = _bottom_y;
        upper_x = _upper_x;
        upper_y = _upper_y;
        if(!ifstr.eof()) { // Считаем переход на новую строку
            ifstr.get();
        }
    }

    double figure_rectangle::calculate() {
        int width = bottom_x - upper_x;
        int height = upper_y - bottom_y;
        return std::abs(2*width) + std::abs(2*height);
    }

    figure *figure::get_next() {
        return next;
    }

    void figure::set_next(figure *_next) {
        next = _next;
    }

    Color figure::get_color() {
        return figure_color;
    }

    void figure::set_color(Color _color) {
        figure_color = _color;
    }
}
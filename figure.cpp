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
            container_node * node = new container_node;
            // Считываем новый элемент, вызывая статический метод класса figure
            figure *new_element = figure::read_one(ifstr);
            if (new_element == nullptr) { // Если произошла ошибка при считывании
                std::cout << "Can't read file, because it has wrong structure." << std::endl;
                // Очищаем уже заполненные элементы списка
                clear();
                return;
            }
            // Добавляем в список считанный элемент
            node->_f = new_element;
            append(node);
        }
    }

    void figure_container::append(container_node *new_element) {
        if (begin == nullptr) { // Если список пустой
            begin = end = new_element;
        } else {
            end->next = new_element; // Последний элемент хранит ссылку на новый элемент
            end = new_element; // Последний элемент теперь - новый элемент
        }
        new_element->next = nullptr;
    }

    void figure_container::clear() {
        container_node *it_next = nullptr; // Храним следующий элемент, т.к. будем удалять элементы в цикле
        for (container_node *it = begin; it != nullptr; it = it_next) {
            delete it->_f;
            it_next = it->next; // Сохраняем следующий элемент
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
        ofstr << "radius = " << radius << "; ";
        ofstr << "density = " << get_density() << ";\n";
    }

    // Выводит уникальные свойства объекта
    void figure_rectangle::write(std::ofstream &ofstr) {
        // Вывод номера и цвета
        ofstr << color_strings[get_color()] << " ";
        ofstr << "rectangle (" << calculate() << ")" << ": ";
        ofstr << "x1 = " << bottom_x << "; ";
        ofstr << "y1 = " << bottom_y << "; ";
        ofstr << "x2 = " << upper_x << "; ";
        ofstr << "y2 = " << upper_y << "; ";
        ofstr << "density = " << get_density() << ";\n";
    }

    // Выводит данные всего контейнера
    void figure_container::write(std::ofstream &ofstr) {
        int index = 0;
        if(begin == nullptr) { // Если пустой контейнер
            ofstr << "Empty container. " << std::endl;
            return;
        }
        for(container_node *it = begin; it != nullptr; it = it->next) {
            index++;
            // Вывод номера и цвета
            ofstr << index << ". " << color_strings[it->_f->get_color()] << " ";
            it->_f->write(ofstr);
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
        //std::cout << "!!!" << readLine << std::endl;
        if (readLine == "circle") { // Если вводится круг
            res = new figure_circle;
            res->type = eFigure::CIRCLE;
        } else if (readLine == "rectangle") { // Если вводится прямоугольник
            res = new figure_rectangle;
            res->type = eFigure::RECTANGLE;
        } else if (readLine == "triangle") { // Если вводится прямоугольник
            res = new figure_triangle;
            res->type = eFigure::TRIANGLE;
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
        // ВВОД ПЛОТНОСТИ
        double dens;
        ifstr >> dens;
        res->set_density(dens);
        // ВВОД ОСТАЛЬНЫХ ХАРАКТЕРИСТИК
        res->read(ifstr);
        return res;
    }

    figure_container::figure_container() {
        // Задаем начальный и последний элементы в нулевой указатель
        end = begin = nullptr;
    }

    void figure_container::sort() {
        for (container_node *it = begin; (it) && (it->next); it = it->next) {
            for (container_node *jt = it->next; jt; jt = jt->next) {
                if (figure::comparator(it->_f, jt->_f)) {
                    figure *temp = it->_f;
                    it->_f = jt->_f;
                    jt->_f = temp;
                }
                std::cout << it->_f->calculate() << " vs " << jt->_f->calculate() << std::endl;
            }
        }
    }

    void figure_container::writeIgnore(std::ofstream &ofstr, eFigure type) {
        ofstr << "Ignoring type: " << type << std::endl;
        int index = 0;
        if(begin == nullptr) { // Если пустой контейнер
            ofstr << "Empty container. " << std::endl;
            return;
        }
        for(container_node *it = begin; it != nullptr; it = it->next) {
            if(type == it->_f->type) {
                continue;
            }
            index++;
            // Вывод номера и цвета
            ofstr << index << ". ";
            it->_f->write(ofstr);
        }
        std::cout << "Successfully printed to file." << std::endl;
    }

    container_node *figure_container::get_begin() {
        return begin;
    }

    int figure_container::get_size() {
        int i = 0;
        for (container_node *jt = begin; jt; jt = jt->next) {
            i++;
        }
        return i;
    }

    void figure_container::multiM(std::ofstream &ofst) {
        ofst << "Writing multimethod" << std::endl;
        for (container_node *it = begin; (it) && (it->next); it = it->next) {
            for (container_node *jt = begin->next; jt; jt = jt->next) {
                it->_f->multiMethod(jt->_f, ofst);
                it->_f->write(ofst);
                jt->_f->write(ofst);
            }
        }
    }

    void figure_circle::read(std::ifstream &ifstr) {
        int _center_x, _center_y;
        double _radius;
        if(ifstr.eof()) {
            return;
        }

        if(!readInt(_center_x, ifstr)) return;
        if(ifstr.eof()) {
            return;
        }
        if(!readInt(_center_y, ifstr)) return;
        if(ifstr.eof()) {
            return;
        }
        ifstr >> _radius;

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

    void figure_circle::multiMethod(figure *other, std::ofstream &ofst) {
        other->multiCycle(ofst);
    }

    void figure_circle::multiCycle(std::ofstream &ofst) {
        ofst << ": CYCLE and CYCLE" << std::endl;
    }

    void figure_circle::multiRect(std::ofstream &ofst) {
        ofst << ": CYCLE and RECTANGLE" << std::endl;
    }

    void figure_circle::multiTriangle(std::ofstream &ofst) {
        ofst << ": CYCLE and TRIANGLE" << std::endl;
    }

    void figure_rectangle::read(std::ifstream &ifstr) {
        int _bottom_x, _bottom_y;
        int _upper_x, _upper_y;

        if(ifstr.eof()) {
            return;
        }
        if(!readInt(_bottom_x, ifstr)) return;
        if(ifstr.eof()) {
            return;
        }
        if(!readInt(_bottom_y, ifstr)) return;
        if(ifstr.eof()) {
            return;
        }
        if(!readInt(_upper_x, ifstr)) return;
        if(ifstr.eof()) {
            return;
        }
        if(!readInt(_upper_y, ifstr)) return;
        if(!ifstr.eof()) { // Считаем переход на новую строку
            ifstr.get();
        }
        bottom_x = _bottom_x;
        bottom_y = _bottom_y;
        upper_x = _upper_x;
        upper_y = _upper_y;
    }

    double figure_rectangle::calculate() {
        int width = bottom_x - upper_x;
        int height = upper_y - bottom_y;
        return std::abs(2*width) + std::abs(2*height);
    }

    void figure_rectangle::multiMethod(figure *other, std::ofstream &ofst) {
        other->multiRect(ofst);
    }

    void figure_rectangle::multiCycle(std::ofstream &ofst) {
        ofst << ": RECTANGLE and CYCLE" << std::endl;
    }

    void figure_rectangle::multiRect(std::ofstream &ofst) {
        ofst << ": RECTANGLE and RECTANGLE" << std::endl;
    }

    void figure_rectangle::multiTriangle(std::ofstream &ofst) {
        ofst << ": RECTANGLE and TRIANGLE" << std::endl;
    }

    Color figure::get_color() {
        return figure_color;
    }

    void figure::set_color(Color _color) {
        figure_color = _color;
    }

    bool figure::comparator(figure *first, figure *second) {
        return first->calculate() < second->calculate();
    }

    double figure::get_density() {
        return density;
    }

    void figure::set_density(double _density) {
        density = _density;
    }

    void figure_triangle::read(std::ifstream &ifstr) {
        if(ifstr.eof()) {
            return;
        }
        if(!readInt(x1, ifstr)) return;
        if(ifstr.eof()) {
            return;
        }
        if(!readInt(y1, ifstr)) return;
        if(ifstr.eof()) {
            return;
        }
        if(!readInt(x2, ifstr)) return;
        if(ifstr.eof()) {
            return;
        }
        if(!readInt(y2, ifstr)) return;
        if(ifstr.eof()) {
            return;
        }
        if(!readInt(x3, ifstr)) return;
        if(ifstr.eof()) {
            return;
        }
        if(!readInt(y3, ifstr)) return;

        if(!ifstr.eof()) { // Считаем переход на новую строку
            ifstr.get();
        }
    }

    void figure_triangle::write(std::ofstream &ofstr) {
        ofstr << "triangle: ";
        ofstr << "x1 = " << x1 << "; ";
        ofstr << "y1 = " << y1 << "; ";
        ofstr << "x2 = " << x2 << "; ";
        ofstr << "y2 = " << y2 << "; ";
        ofstr << "x3 = " << x3 << "; ";
        ofstr << "y3 = " << y3 << "; ";
        ofstr << "density = " << get_density() << ";\n";
    }

    double figure_triangle::calculate() {
        double a = (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2);
        double b = (x1-x3)*(x1-x3) + (y1-y3)*(y1-y3);
        double c = (x3-x2)*(x3-x2) + (y3-y2)*(y3-y2);
        return a+b+c;
    }

    void figure_triangle::multiTriangle(std::ofstream &ofst) {
        ofst << ": TRIANGLE and TRIANGLE" << std::endl;
    }

    void figure_triangle::multiRect(std::ofstream &ofst) {
        ofst << ": TRIANGLE and RECTANGLE" << std::endl;
    }

    void figure_triangle::multiCycle(std::ofstream &ofst) {
        ofst << ": TRIANGLE and CYCLE" << std::endl;
    }

    void figure_triangle::multiMethod(figure *other, std::ofstream &ofst) {
        other->multiTriangle(ofst);
    }

    bool readInt(int &buffer, std::ifstream &ifstr) {
        char str[255];
        ifstr >> str;
        for(int i = 0; i < 255; i++) {
            if(str[i] == 0) break;
            bool fail = str[i] < '0' || str[i] > '9';
            if(fail) return false;
        }
        buffer = atoi(str);
        return true;
    }
}
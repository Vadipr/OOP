#include "gtest/gtest.h"
#include "figure.h"

using namespace figure_space;

TEST(Functions, Comparator) {
    std::ifstream ifstr("input.txt");
    figure_container c{};
    c.read(ifstr);
    // Проверка первых двух элементов
    figure *F1 = c.get_begin()->_f;
    figure *F2 = c.get_begin()->next->_f;
    bool calculate_res = (F1->calculate()<F2->calculate());
    bool comparator_res = figure::comparator(F1, F2);
    EXPECT_EQ(calculate_res, comparator_res);
}

TEST(Functions, Calculate) {
    std::ifstream ifstr("input.txt");
    figure_container c{};
    c.read(ifstr);
    // Проверка первых двух элементов
    figure *F1 = c.get_begin()->_f;
    double calculated1 = F1->calculate();

    double a,b,d;
    int width, height;
    double actual1 = 0;
    figure_triangle*FT = (figure_triangle*)F1;
    figure_rectangle*FR = (figure_rectangle*)F1;
    figure_circle*FC = (figure_circle*)F1;
    switch(F1->type) {
        case eFigure::TRIANGLE:
            a = (FT->x1-FT->x2)*(FT->x1-FT->x2) + (FT->y1-FT->y2)*(FT->y1-FT->y2);
            b = (FT->x1-FT->x3)*(FT->x1-FT->x3) + (FT->y1-FT->y3)*(FT->y1-FT->y3);
            d = (FT->x3-FT->x2)*(FT->x3-FT->x2) + (FT->y3-FT->y2)*(FT->y3-FT->y2);
            actual1 = a+b+d;
            break;
        case eFigure::RECTANGLE:
            width = FR->bottom_x - FR->upper_x;
            height = FR->upper_y - FR->bottom_y;
            actual1 = std::abs(2*width) + std::abs(2*height);
            break;
        case eFigure::CIRCLE:
            actual1 = 2*PI*FC->radius;
            break;
    }

    EXPECT_EQ(actual1, calculated1);
}

TEST(Functions, Clear) {
    std::ifstream ifstr("input.txt");
    figure_container c{};
    c.read(ifstr);
    c.clear();
    auto q = c.get_begin();
    // Если контейнер очищен, то тест пройден
    EXPECT_EQ(q, nullptr);
}

TEST(Functions, Append) {
    std::ifstream ifstr("input.txt");
    figure_container c{};
    c.read(ifstr);
    int size1 = c.get_size();
    container_node*CN = new container_node;
    CN->_f = new figure_circle;
    CN->_f->type = eFigure::CIRCLE;
    c.append(CN);
    int size2 = c.get_size();
    auto q = c.get_begin();
    // Если контейнер очищен, то тест пройден
    EXPECT_EQ(size1+1, size2);
}


TEST(Functions, Get_size) {
    std::ifstream ifstr("input.txt");
    figure_container c{};
    int size1 = c.get_size();
    container_node*CN = new container_node;
    CN->_f = new figure_circle;
    CN->_f->type = eFigure::CIRCLE;
    c.append(CN);
    int size2 = c.get_size();
    auto q = c.get_begin();
    // Если контейнер очищен, то тест пройден
    EXPECT_EQ(size1, 0);
    EXPECT_EQ(size2, 1);
}


TEST(Functions, Read) {
    std::ifstream ifstr("input.txt");
    figure_container c{};
    c.read(ifstr);
    // Если контейнер заполнен
    EXPECT_TRUE(c.get_size() > 0);
}


TEST(Functions, Write) {
    std::ifstream ifstr("input.txt");
    std::ofstream ofstr("output.txt");
    figure_container c{};
    c.read(ifstr);
    c.write(ofstr);
    ofstr.close();
    ifstr.close();
    std::ifstream ifstr2("output.txt");
    // Если файл создан и он не пустой
    EXPECT_TRUE(!ifstr2.eof());
}


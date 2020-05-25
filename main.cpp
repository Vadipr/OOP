#include <gtest/gtest.h>
#include "figure.h"
using namespace std;
using namespace figure_space;

int main(int argc, char* argv[]) {
    // Если некорректно введены аргументы
    if(argc == 2) {
        testing::InitGoogleTest();
        RUN_ALL_TESTS();
        return 0;
    } else
    if(argc !=3) {
        cout << "incorrect command line! " << endl
             << "Waited: command in_file out_file"
             << endl;
        exit(1);
    }
    ifstream ifst(argv[1]);
    if(!ifst.is_open() || ifst.bad()) {
        cout << "Can\'t open input file." << endl;
        return 0;
    }
    ofstream ofst(argv[2]);
    if(!ofst.is_open() || ofst.bad()) {
        cout << "Can\'t open output file." << endl;
        return 0;
    }
    cout << "Program started"<< endl;
    figure_container c{};
    c.read(ifst);
    c.write(ofst);
    c.multiM(ofst);
    c.writeIgnore(ofst, eFigure::CIRCLE);
    c.writeIgnore(ofst, eFigure::RECTANGLE);
    cout << "Program ended"<< endl;
    return 0;
}

#include "figure.h"
using namespace std;
using namespace figure_space;

int main(int argc, char* argv[]) {
    // Если некорректно введены аргументы
    if(argc !=3) {
        cout << "incorrect command line! " << endl
             << "Waited: command in_file out_file"
             << endl;
        exit(1);
    }
    ifstream ifst(argv[1]);
    ofstream ofst(argv[2]);
    cout << "Program started"<< endl;
    figure_container c{};
    c.read(ifst);
    c.write(ofst);
    c.multiM(ofst);
    cout << "Program ended"<< endl;
    return 0;
}

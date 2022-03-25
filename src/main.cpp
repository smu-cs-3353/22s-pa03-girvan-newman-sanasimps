//
// Created by Zachary on 3/25/2022.
//

#include <iostream>

using namespace std;

int main(int argc, char** argv) {
    cout << "Hello CS3353, your arguments were: ";
    for (int i = 1; i < argc; i++) {
        cout << argv[i];
        if (i < argc - 1)
            cout << ", ";
    }
    cout << endl << endl;

    return 0;
}
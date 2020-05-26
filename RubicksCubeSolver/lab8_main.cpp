#include <iostream>
#include "Cube.h"
#include "CubeSolver.h"
using std::vector;
using std::cout;

// F == �����     == 2
// R == ��������� == 3
// L == �������   == 1
// U == �������   == 0
// D == �����     == 5
// B == ������    == 4

int main() {
	
    Cube cube("rubik.txt");
    cout << cube;
	
	vector <const char*> putaem = { "L", "D", "B", "F" };
	for (auto i : putaem) {
		cout << i << "\n";
		cube.rotate(i);
		cout << cube;
	}

	cout << "Making white cross:\n";
	CubeSolver algo(cube);
	algo.make_white_cross();
	algo.synch(cube);
	cout << cube;

	cout << "Making white cross correct\n";
	algo.correct_white_cross();
	algo.synch(cube);
	cout << cube;


	// ������� ��� � �������� �� ������ �����
	/*cout << "Preparing cube a little bit\n";
	cube.rotate(5, 1);
	cube.rotate(4, 1);
	cube.rotate(5, 0);
	algo.r_synch(cube);
	cout << cube;*/
	
	cout << "Making first layer\n";
	algo.first_layer();
	algo.synch(cube);
	cout << cube;
    return 0;
}
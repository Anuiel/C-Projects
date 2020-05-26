#pragma once
#ifndef LAB8_CUBESOLVER_H
#define LAB8_CUBESOLVER_H
#include "Cube.h"

class CubeSolver {
public:
	// ������������
	CubeSolver(Cube cube);
	
	// ������
	void generate_random_state(Cube& obj);
	void synch(Cube& cube);
	void r_synch(Cube& cube);

	// �������� ������
	void make_white_cross();
	void correct_white_cross();
	void first_layer();
private:
	Cube _cube;

	// ��������������� �������
	// make_white_cross():
	bool check_white_cross();
	void free_the_place(int margin, int ind);
	void fill_the_place(int margin, int ind);
	vector <int> check_edge(int margin, int ind);
	void check_mar_edge(std::pair<int, int> mep, vector<std::pair<int, int>> fo, vector<std::pair<int, int>> so, vector<std::pair<int, int>> to);
	// correct_white_cross():
	vector <int> check_cross_state();
	// first_layer():
	vector <int> find_white_corner();
	void set_corner(int margin, int ind);
	void get_corner_up(int margin, int ind, int type);
	// �����
	void reverse_edge(int margin, int edge_ind);
};


#endif //LAB8_CUBESOLVER_H
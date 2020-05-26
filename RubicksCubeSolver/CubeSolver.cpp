#include "CubeSolver.h"
#include <iostream>
#include <ctime>
using std::cout;

// ��������� ����� ��������� �������
template <class T>
T sum(const vector <T>& v) {
	T s = 0;
	for (auto i : v)
		s = s + i;
	return s;
}



CubeSolver::CubeSolver(Cube cube) {
	std::srand(unsigned(std::time(0)));
	_cube = cube;
}

// �������� ������
void CubeSolver::make_white_cross() {
	vector <int> margins = { 0, 1, 3, 4, 5 };
	const vector <std::pair<int, int>> first_option = { {1, 1}, {1, 3}, {0, 0}, {0, 2}, {3, 1}, {3, 3}, {5, 0}, {5, 2} };
	const vector <std::pair<int, int>> second_option = { {1, 0}, {0, 1}, {3, 2}, {5, 3} };
	const vector <std::pair<int, int>> third_option = { {1, 2}, {0, 3}, {3, 0}, {5, 1} };
	// �������� ������� ����� ����� �� ����� �����
	while (!check_white_cross()) { // ���� ����� �� ��������
		for (auto mar_ind : margins) { // ��������� ��� ����� �� ������� ����� �����
			auto m = _cube.get_margin(mar_ind); // ����� i-�� �����
			for (int edge_ind = 0; edge_ind < 4; edge_ind++) { // ��������� ��� 4 ����� �� ������ �����, edge_ind - ����� ����� �� i-�� �����
				vector <int> edge = _cube._edges[edge_ind]; // edge_ind-�� ����� ������ �����
				if (m[edge[0]][edge[1]] == 2) { // ���� ��� ����� ������ �����
					if (mar_ind == 4) { // ���� ����� ������
						free_the_place(2, edge_ind); // ����������� ������ ����� �����
						const int target = _cube._connected[2][edge_ind]; // �����, ������� ���� ���������, ����� ��������� ���� ����� ����� �� �����
						_cube.rotate(target, 0); // ������ ���� ����� ����� �� ����� �����
						_cube.rotate(target, 0);
					}
					else {
						const std::pair <int, int> mar_and_edge = {mar_ind, edge_ind};
						check_mar_edge(mar_and_edge, first_option, second_option, third_option);
					}
				}
			}
		}
	}
}

void CubeSolver::correct_white_cross() {
	// ���� 2 ��� 4 ����� �� �������� � �������, ������������ ������� �����
	// ���� ������� 4 - ��� �����
	// ���� ������� 2:
	//		���� ��� ������ ((current_cross_state[1] - current_cross_state[0]) % 2 == 1), ��������� ������� ��� ������� ������������ �������
	//		���� ��� �� ������, ��������� ������ ������� ������������ �������
	vector <int> current_cross_state = check_cross_state();
	int rotate_counter = 0; // ����� � ������, � �� ������ 2 ��� 4 ����� ����� ���������
	while (current_cross_state.size() < 2 && rotate_counter < 5) {
		_cube.rotate(2, 0); // ������������ ������� �����
		current_cross_state = check_cross_state(); // ��������� ���������� � ������
		rotate_counter++;
	}
	if (rotate_counter >= 5) {
		cout << "This state broke my algorithm and my hopes...\n";
		return;
	}
	if (current_cross_state.size() == 2) { // � ������ � 4-�� �������� ������ ������ �� �����
		if ((current_cross_state[1] - current_cross_state[0]) % 2 == 1) { // ��� ������
			cout << _cube;
			// �������� ��� ����� �� ������ �����
			_cube.rotate(current_cross_state[0], 0);
			_cube.rotate(current_cross_state[0], 0);
			_cube.rotate(current_cross_state[1], 0);
			_cube.rotate(current_cross_state[1], 0);
			// ������ ���� �� ��� �� ��� �����
			_cube.rotate(4, 0);
			// ���������� ��� �� ����� �����
			_cube.rotate(current_cross_state[0], 0);
			_cube.rotate(current_cross_state[0], 0);
			// ������ ������ �� ��� �����
			_cube.rotate(4, 1);
			_cube.rotate(4, 1);
			// ������ ��� �� ��� �����
			_cube.rotate(current_cross_state[1], 0);
			_cube.rotate(current_cross_state[1], 0);
		}
		else {
			// �������� ��� ����� �� ������ �����
			_cube.rotate(current_cross_state[0], 0);
			_cube.rotate(current_cross_state[0], 0);
			_cube.rotate(current_cross_state[1], 0);
			_cube.rotate(current_cross_state[1], 0);
			// ������ �� ������� (������������ ������ ����� 2 ����)
			_cube.rotate(4, 0);
			_cube.rotate(4, 0);
			// ���������� �� �� ����� �����
			_cube.rotate(current_cross_state[0], 0);
			_cube.rotate(current_cross_state[0], 0);
			_cube.rotate(current_cross_state[1], 0);
			_cube.rotate(current_cross_state[1], 0);
		}
	}
}
void CubeSolver::first_layer() {
	vector <int> corner_state = find_white_corner();
	int m = corner_state[0];
	int ind = corner_state[1];
	while (m != -1) { // ���� ��� ���� ����� ���� �� �� ����� �����
		vector <int> conn = _cube._connected[m]; // �����, ������� � ������
		if (m == 4) { // ���� ��� ������ �����
			// vector <int> neigh_colors = _cube._three_con[4][ind]; // ���������� �������� ������ ������ ����
			// ���� (������ �� ���� �� ����� ��� ������ ���� � ����� �� ���� �� ����� ��� ������ ����)
			// ������������ ������ �����
			// ������ - conn[ind], ������ - _cube.get_neigh_color(4, 3, ind, 1)
			// ����� - conn[(ind + 1) % 4], ������ - _cube.get_neigh_color(4, 3, ind, 0)
			set_corner(m, ind); // �������� ��� �����, ������� �� ������ ����� ��� ����� ������
		}
		else {
			vector <vector <int>> down_corners = {
				{3, 2}, {2, 1}, {0, 3}, {1, 0}
			};
			vector <vector <int>> up_corners = {
				{0, 1}, {3, 0}, {1, 2}, {2, 3}
			};
			int type;
			if (ind == down_corners[m / 2 + m % 2][0]) { // ����� ������
				type = 0;
			}
			else if (ind == down_corners[m / 2 + m % 2][1]) { // ������ ������
				type = 1;
			}
			else if (ind == up_corners[m / 2 + m % 2][0]) {  // ����� �������
				type = 2;
			}
			else {  // ������ �������
				type = 3;
			}
			get_corner_up(m, ind, type);
			// set_corner(m, ind);
		}
		corner_state = find_white_corner();
		m = corner_state[0];
		ind = corner_state[1];
	}
}

// ������
void CubeSolver::generate_random_state(Cube& obj) {
	const int count = rand() % 11 + 10; // ���������� ��������� ������
	// ��� �������� ���������
	vector <const char*> margins = { "U", "L", "F", "R", "B", "D", "U\'", "L\'", "F\'", "R\'", "B\'", "D\'" };
	for (size_t i = 0; i < count; i++)
		obj.rotate(margins[rand() % 12]);
}
void CubeSolver::synch(Cube& cube) {
	cube = _cube;
}

void CubeSolver::r_synch(Cube& cube) {
	_cube = cube;
}

// ��������������� �������
void CubeSolver::reverse_edge(int margin, int edge_ind) {
	const int target_ind = (edge_ind + 2) % 4;
	const int not_connected = 15 - sum(_cube._connected[margin]) - margin; // ������� ����� �����, ������� �� �������� � ������
	const int target1 = _cube._connected[margin][target_ind];
	const int target2 = _cube._connected[margin][(target_ind + 1) % 4];
	_cube.rotate(target1, 1); // ������ ���� ����� ����
	_cube.rotate(target2, 0); // ������� ���� ����� �� ������ ����
	_cube.rotate(not_connected, 0); // ������� ���� ����� ��� ��� "��������" �����
	_cube.rotate(target2, 1); // ���������� ����� ����� �� �������� �����
	_cube.rotate(target1, 0);
	_cube.rotate(target1, 0); // ������� ���� ����� �� ��� "�������� �����"
}

bool CubeSolver::check_white_cross() {
	const int color = 2; // ���� ����� �����
	auto white = _cube.get_margin(color); // �������� ��������� ����� �����
	for (auto edge: _cube._edges) // ������ ��� ����� �� ����� �����
		if (white[edge[0]][edge[1]] != color) // ���� ���� ���� �� ����� �� �����, ���������� false
			return false;
	return true;
}
void CubeSolver::free_the_place(int margin, int ind) {
	for (int j = 0; j < 3; j++) { // ��������� �������� �� ������ ���� ���
		vector <int> e = check_edge(margin, ind);
		if (e[0] != 2 && e[1] != 2) // ���� ��������� ����� �������, �����������
			break;
		_cube.rotate(margin, 0); // ������������ ����� �����, ������� ����� ��������� ����� (�� ��� ����� ������)
	}
}
void CubeSolver::fill_the_place(int margin, int ind) {
	const int target = _cube._connected[margin][ind]; // �������� �����, ������� ����� ���������, ����� ��������� ind-�� �����
	for (int j = 0; j < 3; j++) { // ��������� �������� �� ������ ���� ���
		_cube.rotate(target, 0); // ������������ target �����, ������� ��������� ��������� ����� (�� ��� ����� ��������)
		vector <int> e = check_edge(margin, ind);
		if (e[0] == margin || e[1] == margin) // ���� ���� ����� ������ �� �����, �����������
			break;
	}
}
vector <int> CubeSolver::check_edge(int margin, int ind) {
	vector <int> e(2); // ������ ����� ������� ����� �� ������ ����� � �� �������
	e[0] = _cube._state[margin][_cube._edges[ind][0]][_cube._edges[ind][1]]; // ���� ������� ����� �� ������ �����
	const int neigh_margin = _cube._connected[margin][ind]; // ������� �����, �� ������� ����� ����� ������ �����
	vector <int> neigh_ind = _cube._two_con[margin][ind]; // ������ ������� ����� �� ������� �����
	e[1] = _cube._state[neigh_margin][neigh_ind[0]][neigh_ind[1]]; // ���� ������� ����� �� ������� �����
	return e;
}
void CubeSolver::check_mar_edge(std::pair<int, int> mep, vector<std::pair<int, int>> fo, vector<std::pair<int, int>> so, vector<std::pair<int, int>> to) {
	for (auto p : fo) {
		if (p == mep) {
			free_the_place(2, mep.second); // ����������� ������ ����� �����
			fill_the_place(2, mep.second); // ������ ��� �� ������������� �����
			return;
		}
	}
	for (auto p : so) {
		if (p == mep) {
			free_the_place(2, mep.second); // ����������� ������ ����� �����
			_cube.rotate(mep.first, 0);
			_cube.rotate(mep.first, 0); // ������ ��� �� ������������� �����
			return;
		}
	}
	for (auto p : to) {
		if (p == mep) {
			reverse_edge(2, mep.second);
			return;
		}
	}
}
vector<int> CubeSolver::check_cross_state() {
	// ��������� ��� ������� � ����� ����� (����, ����, �����, ���) == _cube._connecetd[2][i]
	// �������, ����� �� i-�� ������� ����� (i+2)%4-�� ����� ���� ����� i
	// ���� �� - ��������� i � ������ correct, ���� ���, ���� �������� ������
	vector <int> correct; // ����� ��������� ������ ������, �� ������� ����� ����� ��������� �� ����� � ������� �����
	vector <vector <int>> e = _cube._edges; // ������ ��������� ����� �� ����� (�����)
	for (int conn_ind = 0; conn_ind < 4; conn_ind++) { // ��������� ��� �����, ������� � ����� ������ (����, ����, �����, ���)
		int margin_ind = _cube._connected[2][conn_ind];
		if (_cube._state[margin_ind][e[(conn_ind + 2) % 4][0]][e[(conn_ind + 2) % 4][1]] == margin_ind) { // ���� ����� ����� �� ���� ����� ������ �� ����� ��� � ����� �����
			correct.push_back(conn_ind);
		}
	}
	return correct;
}
vector<int> CubeSolver::find_white_corner() {
	vector <int> corner_info(3); // { margin, x, y }
	vector <int> margins = { 4, 0, 1, 3, 5 };
	for (auto m : margins) // ������� �� ������ ����� �� ������ �� �������
		for (int i = 0; i < 4; i++) { // ������� �� ������ ���� �� ���� �����
			auto e = _cube._corners[i];
			if (_cube._state[m][e[0]][e[1]] == 2) // ���� ���� ���� �����
				return { m, i }; // ���������� �������, �� ������� �� ��� ����� � ��� �����
		}
	return { -1, -1 };
}

void CubeSolver::set_corner(int margin, int ind) {
	while (_cube._connected[margin][ind] != _cube.get_neigh_color(margin, 3, ind, 1) && _cube._connected[margin][(ind + 1) % 4] != _cube.get_neigh_color(margin, 3, ind, 0)) {
		_cube.rotate(4, 0);
		ind = (ind + 1) % 4; // ��-�� �������� ���� ����� ������ ���� ��������
	}
	_cube.rotate(_cube._connected[margin][(ind + 1) % 4], 1); // �������������� ����
	_cube.rotate(margin, 0); // ������ ���, ����� ������� �� ����� ��, ��� ������������� ������ � �����
	_cube.rotate(_cube._connected[margin][(ind + 1) % 4], 0); // ���������� ����� �� �������� �����
	_cube.rotate(margin, 0); // ������� ���� � ���-����
	_cube.rotate(margin, 0);
	_cube.pif_paf(margin, ind, 1); // ���-����� ������ ��� �� �����
}

void CubeSolver::get_corner_up(int margin, int ind, int type) {
	if (type == 0) { // ����� ������
		_cube.rotate(_cube._connected[margin][(ind + 1) % 4], 1);
		_cube.rotate(4, 1);
		_cube.rotate(_cube._connected[margin][(ind + 1) % 4], 0);
	}
	else if (type == 1) { // ������ ������
		_cube.rotate(_cube._connected[margin][ind], 0);
		_cube.rotate(4, 0);
		_cube.rotate(_cube._connected[margin][ind], 1);
	}
	else if (type == 2) { // ����� �������
		vector <int> circle_margins = { 0, 1, 5, 3 };
		vector <int> circle_inds = { 0, 3, 2, 1 };
		int m_ind = 0;
		for (int i = 0; i < circle_margins.size(); i++) {
			if (circle_margins[i] == margin) {
				m_ind = i;
				break;
			}
		}
		int i_ind = 0;
		for (int i = 0; i < circle_inds.size(); i++) {
			if (circle_inds[i] == ind) {
				i_ind = i;
				break;
			}
		}
		while (_cube.get_neigh_color(margin, 3, ind, 0) != _cube._connected[margin][ind]) {
			_cube.rotate(4, 0);
			margin = circle_margins[(m_ind + 1) % 4];
			ind = circle_inds[(i_ind + 1) % 4];
		}
		_cube.rotate(_cube._connected[margin][ind], 1);
		_cube.rotate(4, 0);
		_cube.rotate(_cube._connected[margin][ind], 0);
	}
	else if (type == 3) { // ������ �������
		cout << _cube;
		vector <int> circle_margins = { 0, 1, 5, 3 };
		vector <int> circle_inds = { 0, 3, 2, 1 };
		int m_ind = 0;
		for (int i = 0; i < circle_margins.size(); i++) {
			if (circle_margins[i] == margin) {
				m_ind = i;
				break;
			}
		}
		int i_ind = 0;
		for (int i = 0; i < circle_inds.size(); i++) {
			if (circle_inds[i] == ind) {
				i_ind = i;
				break;
			}
		}
		while (_cube.get_neigh_color(margin, 3, ind, 1) != _cube._connected[margin][(ind + 1) % 4]) {
			_cube.rotate(4, 0);
			margin = circle_margins[(m_ind + 1) % 4];
			ind = circle_inds[(i_ind + 1) % 4];
		}
		_cube.rotate(_cube._connected[margin][(ind + 1) % 4], 0);
		_cube.rotate(4, 1);
		_cube.rotate(_cube._connected[margin][(ind + 1) % 4], 1);
	}
}


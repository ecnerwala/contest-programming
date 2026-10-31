#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class ToddlerToy {

public:

	string solve(vector <string> start, vector <string> goal) {
		string resp; resp.reserve(3000);
		// we have 6 holes: one on each column, and one on each border
		auto do_move = [&](int c, int p, int hole, bool back) {
			assert(hole != c);
			char cur_col = char('C' + 2 * c);
			char cur_row = char('3' + p);
			char dest_col, dest_row;
			if (hole == 4) dest_col = 'B', dest_row = '1';
			else if (hole == 5) dest_col = 'J', dest_row = '1';
			else dest_col = char('C' + 2 * hole), dest_row = '2';

			if (back) {
				swap(cur_col, dest_col);
				swap(cur_row, dest_row);
			}

			while (cur_row > '1') {
				resp += cur_col;
				resp += cur_row;
				resp += 'U';
				cur_row--;
			}
			while (cur_col < dest_col) {
				resp += cur_col;
				resp += cur_row;
				resp += 'R';
				cur_col++;
			}
			while (cur_col > dest_col) {
				resp += cur_col;
				resp += cur_row;
				resp += 'L';
				cur_col--;
			}
			while (cur_row < dest_row) {
				resp += cur_col;
				resp += cur_row;
				resp += 'D';
				cur_row++;
			}
		};
		auto move_to_hole = [&](int c, int p, int hole) {
			do_move(c, p, hole, false);
		};
		auto move_from_hole = [&](int c, int p, int hole) {
			do_move(c, p, hole, true);
		};
		auto do_swap = [&](int c1, int p1, int c2, int p2) {
			vector<int> holes; holes.reserve(6);
			for (int i = 0; i < 6; i++) if (i != c1 && i != c2) holes.push_back(i);
			if (c1 == c2) {
				for (int i = 0; i <= max(p1, p2); i++) {
					move_to_hole(c1, i, holes[i]);
				}
				for (int i = max(p1, p2); i >= 0; i--) {
					move_from_hole(c1, i, holes[(i == p1 || i == p2) ? (i ^ p1 ^ p2) : i]);
				}
				return;
			}
			assert(p1 + 1 + p2 + 1 <= 4);
			for (int i = 0; i <= p1; i++) {
				move_to_hole(c1, i, holes[i]);
			}
			for (int i = 0; i <= p2; i++) {
				move_to_hole(c2, i, holes[p1+1+i]);
			}
			move_from_hole(c2, p2, holes[p1]);
			move_from_hole(c1, p1, holes[p1+1+p2]);
			for (int i = p2-1; i >= 0; i--) {
				move_from_hole(c2, i, holes[p1+1+i]);
			}
			for (int i = p1-1; i >= 0; i--) {
				move_from_hole(c1, i, holes[i]);
			}
		};
		auto do_swap_safe = [&](int c1, int p1, int c2, int p2) {
			if (c1 == c2 || p1 + p2 + 2 <= 4) {
				do_swap(c1, p1, c2, p2);
				return;
			}
			assert(p1 == 2 || p2 == 2);
			if (p2 == 2) {
				swap(c1, c2);
				swap(p1, p2);
			}
			assert(p1 == 2);
			do_swap(c2, p2, c2, 0);
			do_swap(c1, p1, c2, 0);
			do_swap(c2, p2, c2, 0);
		};
		for (int cg = 0; cg < 4; cg++) {
			for (int pg = 0; pg < 3; pg++) {

				for (int cs = cg; cs < 4; cs++) {
					for (int ps = (cs == cg ? pg : 0); ps < 3; ps++) {
						if (start[cs][ps] == goal[cg][pg]) {
							do_swap_safe(cs, ps, cg, pg);
							swap(start[cs][ps], start[cg][pg]);
							goto found;
						}
					}
				}
				assert(false);
found:
				assert(start[cg][pg] == goal[cg][pg]);
			}
		}
		return resp;
	}
};

void my_tests() {
	mt19937 mt(48);
	auto gen_val = [&]() -> vector<string> {
		string a;
		for (char c : string("0123")) {
			a += c;
			a += c;
			a += c;
		}
		std::shuffle(a.begin(), a.end(), mt);
		return {a.substr(0, 3), a.substr(3, 3), a.substr(6, 3), a.substr(9, 3)};
	};
	for (int z = 0; z < 100000; z++) {
		vector<string> st = {"333", "000", "111", "222"};
		vector<string> ds = {"000", "111", "222", "333"};

		string resp = ToddlerToy().solve(st, ds);
		if (z == 0) cerr << resp.size()/3 << '\n';
		if (resp.size() >= 3000) {
			for (string s : st) { cerr << s << ' '; } cerr << '\n';
			for (string s : ds) { cerr << s << ' '; } cerr << '\n';
		}
		assert(resp.size()/3<=1000);
	}
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::ToddlerToy;

// vim:ft=cpp

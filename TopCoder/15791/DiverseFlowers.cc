#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

using state = vector<vector<int>>;
class DiverseFlowers {

public:
	int getBest(state S) {
		for (int big = 0; big < 2; big++) {
			for (int v = 0; v < 3; v++) {
				if (S[big][v] < 0) return -1;
			}
		}
		int minColor = min(S[0][0] + S[1][0], min(S[0][1] + S[1][1], S[0][2] + S[1][2]));
		for (int big = 0; big < 2; big++) {
			for (int v = 0; v < 3; v++) {
				S[big][v] = min(S[big][v], minColor);
			}
		}
		int minSize = min(S[0][0] + S[0][1] + S[0][2], S[1][0] + S[1][1] + S[1][2]);
		return min(minSize, minColor);
	}

	vector<string> solve3(state S) {
		vector<string> res;
		int val = getBest(S);
		while (val > 0) {
			for (int big = 0; big < 2; big++) {
				for (int v = 0; v < 3; v++) {
					state ns = S;
					ns[big][v] --;
					ns[!big][(v+1)%3] --;
					ns[!big][(v+2)%3] --;
					if (getBest(ns) == val-1) {
						string s;
						s += char("rgb"[v] + (big ? 'A' - 'a' : 0));
						s += char("rgb"[(v+1)%3] + (!big ? 'A' - 'a' : 0));
						s += char("rgb"[(v+2)%3] + (!big ? 'A' - 'a' : 0));
						res.push_back(s);
						val--;
						S = ns;
						goto found;
					}
				}
			}
			assert(false);
found:;
		}
		return res;
	}

	vector <string> solve(string S, int K) {
		int N = int(S.size());
		int mx = N / K;

		state st = vector<vector<int>>(2, vector<int>(3, 0));
		for (char c : S) {
			bool big = false;
			if ('A' <= c && c <= 'Z') {
				big = true;
				c = char(c - 'A' + 'a');
			}
			int v;
			switch(c) {
				case 'r': v = 0; break;
				case 'g': v = 1; break;
				case 'b': v = 2; break;
				default: assert(false);
			}
			st[big][v] ++;
		}
		vector<string> res = solve3(st);
		if (int(res.size()) > mx) res.resize(mx);
		for (string s : res) {
			for (char c : s) {
				bool big = false;
				if ('A' <= c && c <= 'Z') {
					big = true;
					c = char(c - 'A' + 'a');
				}
				int v;
				switch(c) {
					case 'r': v = 0; break;
					case 'g': v = 1; break;
					case 'b': v = 2; break;
					default: assert(false);
				}
				st[big][v] --;
			}
		}
		vector<char> extras;
		for (int big = 0; big < 2; big++) {
			for (int v = 0; v < 3; v++) {
				char c = "rgb"[v] + (big ? 'A' - 'a' : 0);
				for (int i = 0; i < st[big][v]; i++) extras.push_back(c);
			}
		}
		for (string& s : res) {
			while (int(s.size()) < K) {
				s.push_back(extras.back());
				extras.pop_back();
			}
		}
		return res;
	}
};

void my_tests() {
	mt19937 mt(48);
	for (int z = 0; z < 10000; z++) {
		state st = vector<vector<int>>(2, vector<int>(3, 0));
		for (int a = 0; a < 2; a++) for (int b = 0; b < 3; b++) {
			st[a][b] = int(mt() % 30);
		}
		DiverseFlowers().solve3(st);
	}
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::DiverseFlowers;

// vim:ft=cpp

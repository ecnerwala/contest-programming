#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class MatchingPatterns {

public:

	string solve(int N, vector <int> L, vector <string> patterns) {
		int V = 26;
		std::vector<int> st(L);
		for (int i = 0; i < N; i++) {
			st[i] = V;
			V += L[i];
		}
		std::vector<int> par(V, -1);
		auto get_par = [&](int a) {
			while (par[a] >= 0) {
				if (par[par[a]] >= 0) par[a] = par[par[a]];
				a = par[a];
			}
			return a;
		};
		auto merge = [&](int a, int b) -> bool {
			a = get_par(a);
			b = get_par(b);
			if (a == b) return true;
			if (a > b) std::swap(a, b);
			assert(a < b);
			if (b < 26) {
				return false;
			}
			par[a] += par[b];
			par[b] = a;
			return true;
		};

		std::vector<int> cur_string;
		for (string s : patterns) {
			std::vector<int> nxt_string;
			for (char c : s) {
				if ('a' <= c && c <= 'z') nxt_string.push_back(c - 'a');
				else if ('A' <= c && c <= 'Z') {
					int i = c - 'A';
					assert(0 <= i && i < N);
					for (int j = 0; j < L[i]; j++) {
						nxt_string.push_back(st[i] + j);
					}
				} else assert(false);
			}
			if (cur_string.empty()) cur_string = nxt_string;
			else {
				if (cur_string.size() != nxt_string.size()) return "";
				for (int i = 0; i < int(cur_string.size()); i++) {
					if (!merge(cur_string[i], nxt_string[i])) return "";
				}
			}
		}
		string res;
		for (int i : cur_string) {
			i = get_par(i);
			if (i >= 26) {
				// shouldn't ever happen
				if (!merge(0, i)) assert(false);
				i = get_par(i);
			}
			assert(0 <= i && i < 26);
			res += char('a' + i);
		}
		return res;
	}
};

void my_tests() {
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::MatchingPatterns;

// vim:ft=cpp

#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class SelfDescFind {

public:

	bool isGood(const vector<int>& digits, const vector<int>& cnts) {
		assert(cnts.size() == digits.size());
		int c[10]; memset(c, 0, sizeof(c));
		for (int i = 0; i < int(digits.size()); i++) {
			c[digits[i]] = cnts[i];
		}
		for (int i = 0; i < int(digits.size()); i++) {
			c[digits[i]] --;
			c[cnts[i]] --;
		}
		for (int i = 0; i < 10; i++) if (c[i]) return false;
		return true;
	}

	string dfs(const vector<int>& digits, int ind, vector<int>& cnts, int sumA, int sumProd) {
		if (sumA < 0 || sumProd < 0) {
			return "";
		}
		if (ind == 0) {
			if (sumA || sumProd) return "";
			if (!isGood(digits, cnts)) return "";
			vector<string> v;
			for (int i = 0; i < int(digits.size()); i++) {
				string s;
				s += char('0' + cnts[i]);
				s += char('0' + digits[i]);
				v.push_back(s);
			}
			sort(v.begin(), v.end());
			string res;
			for (string s : v) res += s;
			return res;
		}
		string res = "";
		ind--;
		for (int c = 1; c <= 9; c++) {
			cnts[ind] = c;
			string cnd = dfs(digits, ind, cnts, sumA - c, sumProd - c * digits[ind]);
			if (cnd == "") continue;
			if (res == "" || cnd < res) res = cnd;
		}
		return res;
	}

	string construct(vector <int> digits) {
		int sumA = int(digits.size()) * 2;
		int sumProd = sumA;
		for (int d : digits) {
			sumProd += d;
		}
		vector<int> cnts(digits.size());
		return dfs(digits, int(digits.size()), cnts, sumA, sumProd);
	}
};

void my_tests() {
	// FILL IN TESTS
	for (int m = 1; m < 1024; m++) {
		vector<int> digits;
		for (int i = 0; i < 10; i++) {
			if (m & (1 << i)) {
				digits.push_back(i);
			}
		}
		cerr << m << '\n';
		SelfDescFind a;
		cerr << a.construct(digits) << '\n';
	}
}

}  // namespace ecnerwala

using ecnerwala::SelfDescFind;

// vim:ft=cpp

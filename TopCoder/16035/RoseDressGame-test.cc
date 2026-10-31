#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala_test {

map<vector<int>, bool> mem;
vector<int> canon(vector<int> v) {
	sort(v.begin(), v.end());
	int N = 0;
	for (int i = 0; i < int(v.size()); i++) {
		if (v[i] == 0) continue;
		if (N && v[i] == v[N-1]) {
			N--;
		} else {
			v[N++] = v[i];
		}
	}
	v.resize(N);
	return v;
}

bool winsFast(vector<int> v) {
	v = canon(v);
	if (v.size() % 2) v.insert(v.begin(), 0);
	bool isWeird = true;
	for (int i = 0; i < int(v.size()); i+=2) {
		isWeird = isWeird && ((v[i] ^ v[i+1]) == 1);
	}
	int x = 0;
	for (int a : v) {
		x ^= a;
	}
	return (x != 0) ^ isWeird;
}

bool wins(vector<int> v) {
	if (v.empty()) {
		return true; // winning position
	}
	if (mem.count(v)) return mem[v];
	for (int i = 0; i < int(v.size()); i++) {
		for (int x = 1; x <= v[i]; x++) {
			vector<int> nv = v;
			nv[i] -= x;
			if (!wins(canon(nv))) {
				//assert(winsFast(v));
				return mem[v] = true;
			}
		}
	}
	//assert(!winsFast(v));
	return mem[v] = false;
}

class RoseDressGame {

public:

	string getWinner(vector <int> roses) {
		roses = canon(roses);
		return wins(roses) ? "Coco" : "Yves";
	}
};

void my_tests() {
	// FILL IN TESTS
	for (int m = 1; m <= 1024; m++) {
		vector<int> v;
		for (int i = 0; i < 10; i++) {
			if (m & (1<<i)) v.push_back(i+1);
		}
		assert(wins(v) == winsFast(v));
	}
	cerr << "Good" << '\n';
	for (int i = 1; i <= 10; i++) {
		if (wins(vector<int>({i})) != winsFast({i})) {
			cerr << i << ' ' << wins(vector<int>({i})) << '\n';
		}
	}
	for (int i = 1; i <= 10; i++) {
		for (int j = i+1; j <= 10; j++) {
			if (wins(vector<int>({i,j})) != winsFast({i,j})) {
				cerr << i << ' ' << j << ' ' << wins(vector<int>({i, j})) << '\n';
			}
		}
	}
	for (int i = 1; i <= 10; i++) {
		for (int j = i+1; j <= 10; j++) {
			for (int k = j+1; k <= 10; k++) {
				if (wins({i,j,k}) != winsFast({i,j,k})) {
					cerr << i << ' ' << j << ' ' << k << ' ' << (i^j^k) << ' ' << wins(vector<int>({i, j, k})) << '\n';
				}
			}
		}
	}
	for (int i = 1; i <= 20; i++) {
		for (int j = i+1; j <= 20; j++) {
			for (int k = j+1; k <= 20; k++) {
				for (int l = k+1; l <= 20; l++) {
					if (wins({i,j,k,l}) != winsFast({i,j,k,l})) {
						cerr << i << ' ' << j << ' ' << k << ' ' << l << ' ' << (i^j^k^l) << ' ' << wins(vector<int>({i, j, k, l})) << '\n';
					}
				}
			}
		}
	}
	for (int i = 1; i <= 20; i++) {
		for (int j = i+1; j <= 20; j++) {
			for (int k = j+1; k <= 20; k++) {
				for (int l = k+1; l <= 20; l++) {
					for (int m = l+1; m <= 20; m++) {
						if (wins({i,j,k,l,m}) != winsFast({i,j,k,l,m})) {
							cerr << i << ' ' << j << ' ' << k << ' ' << l << ' ' << m << ' ' << (i^j^k^l^m) << ' ' << wins(vector<int>({i, j, k, l, m})) << '\n';
						}
					}
				}
			}
		}
	}
	for (int i = 1; i <= 20; i++) {
		for (int j = i+1; j <= 20; j++) {
			for (int k = j+1; k <= 20; k++) {
				for (int l = k+1; l <= 20; l++) {
					for (int m = l+1; m <= 20; m++) {
						for (int n = m+1; n <= 20; n++) {
							if (wins({i,j,k,l,m,n}) != winsFast({i,j,k,l,m,n})) {
								cerr << i << ' ' << j << ' ' << k << ' ' << l << ' ' << m << ' ' << n << ' ' << (i^j^k^l^m^n) << ' ' << wins(vector<int>({i, j, k, l, m, n})) << '\n';
							}
						}
					}
				}
			}
		}
	}
}

}  // namespace ecnerwala

//using ecnerwala::RoseDressGame;

// vim:ft=cpp

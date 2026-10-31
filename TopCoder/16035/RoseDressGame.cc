#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

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

class RoseDressGame {

public:

	string getWinner(vector <int> roses) {
		roses = canon(roses);
		return winsFast(roses) ? "Coco" : "Yves";
	}
};

}  // namespace ecnerwala

using ecnerwala::RoseDressGame;

// vim:ft=cpp

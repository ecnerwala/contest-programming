#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

typedef pair<int, int> pii;
#define A first
#define B second

class ExactTreeEasy {

public:

	vector <int> getTree(int n, int m) {
		vector<int> res;
		for(int i = 0; i < m - 1; i++) {
			res.push_back(i);
			res.push_back(m - 1);
		}
		for(int i = m - 1; i + 1 < n; i++) {
			res.push_back(i);
			res.push_back(i + 1);
		}
		return res;
	}
};

// vim:ft=cpp

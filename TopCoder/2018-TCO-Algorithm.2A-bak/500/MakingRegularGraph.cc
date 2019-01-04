#include <bits/stdc++.h>
using namespace std;

class MakingRegularGraph {

	const vector<int> BAD{-1};

	static const int MAXN = 2000;
	int N;
	int oth[MAXN];
	int len[MAXN];
	int n[4];

	void merge(int x, int y) {
		if (oth[x] == y) {
			// completed the cycle
			n[len[x]] --;
			oth[x] = oth[y] = -1;
			len[x] = len[y] = 0;
		} else {
			int a = oth[x], b = oth[y];
			int tot = min(len[x] + len[y], 3);
			assert(a != -1);
			assert(b != -1);
			assert(a != b);
			oth[x] = oth[y] = -1;
			n[len[x]] --;
			n[len[y]] --;
			len[x] = len[y] = 0;
			oth[a] = b;
			oth[b] = a;
			len[a] = len[b] = tot;
			n[tot] ++;
		}
	}

public:

	vector <int> addEdges(int _N, vector <int> X, vector <int> Y) {
		N = _N;
		n[1] = n[2] = n[3] = 0;
		for (int i = 0; i < N; i++) {
			oth[i] = i;
			len[i] = 1; // vertex lengt
			n[len[i]] ++;
		}
		for (int k = 0; k < int(X.size()); k++) {
			merge(X[k], Y[k]);
		}
		if (n[1] + n[2] + n[3] == 0) {
			return vector<int>{};
		}
		if (n[1] + 2 * n[2] + 3 * n[3] <= 2) {
			return BAD;
		}
		vector<int> res;
		while (n[1] + n[2] + n[3] > 0) {
			int i = 0;
			while (oth[i] == -1) i++;
			int j = i+1;
			while (oth[j] == -1) j++;
			if (oth[i] != j) {
				res.push_back(i);
				res.push_back(j);
				merge(i, j);
			} else {
				assert(len[i] != 1);
				int rem = n[1] + 2 * n[2] + 3 * n[3] - len[i];
				if (len[i] == 2 || rem == 1 || rem == 2) {
					// we need to do something else
					j++;
					while (oth[j] == -1) j++;
					res.push_back(i);
					res.push_back(j);
					merge(i, j);
				} else {
					res.push_back(i);
					res.push_back(j);
					merge(i, j);
				}
			}
		}
		return res;
	}
};

// vim:ft=cpp

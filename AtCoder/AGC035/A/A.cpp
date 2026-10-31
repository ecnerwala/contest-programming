#include<bits/stdc++.h>
using namespace std;

const int MAXN = 1.1e5;
int N;
int A[MAXN];

bool go() {
	map<int, int> cnts;
	for (int i = 0; i < N; i++) {
		cnts[A[i]] ++;
	}
	if (cnts.size() == 1) {
		return cnts.begin()->first == 0;
	} else {
		if (N % 3 != 0) return false;
		vector<int> vals;
		for (auto& it : cnts) {
			if (it.second % (N/3) != 0) return false;
			it.second /= (N/3);
			for (int v = 0; v < it.second; v ++) {
				vals.push_back(it.first);
			}
		}
		assert(vals.size() == 3);
		return (vals[0] ^ vals[1] ^ vals[2]) == 0;
	}
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N;
	for (int i = 0; i < N; i++) cin >> A[i];

	cout << (go() ? "Yes" : "No") << '\n';

	return 0;
}

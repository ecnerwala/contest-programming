#include<bits/stdc++.h>
using namespace std;

const int V = 20;
int nimber[1 << (V+1)];

inline int& getNimber(int l, int m) {
	assert(0 <= m && m < (1 << l));
	return nimber[(1<<l) | m];
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	nimber[0] = 0;
	for (int l = 1; l <= 20; l++) {
		for (int m = 0; m < (1 << l); m++) {
			unordered_set<int> mex;
			for (int a = 0; a < l; a++) {
				if (m & (1 << a)) {
					mex.insert(getNimber(l, m ^ (1<<a)));
					mex.insert(getNimber(a, m & ((1<<a)-1)) ^ getNimber(l-1-a, m >> (a+1)));
				} else {
					int lhs = (a == 0) ? 0 : (getNimber(a-1, m & ((1<<(a-1))-1)) ^ ((m >> (a-1)) & 1));
					int rhs = (a == l-1) ? 0 : (getNimber(l-2-a, m >> (a+2)) ^ ((m >> (a+1)) & 1));
					mex.insert(lhs ^ rhs);
				}
			}
			int v = 0;
			while(mex.count(v)) ++v;
			getNimber(l, m) = v;
			//if (l <= 5) cerr << l << ' ' << m << ' ' << getNimber(l, m) << '\n';
		}
	}

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		vector<int> pts;
		for (int i = 0; i < N; i++) {
			string s; cin >> s;
			if (s == "III") {
				pts.push_back(3);
			} else if (s == "I.I") {
				pts.push_back(0);
			} else if (s == ".II" || s == "II.") {
				pts.push_back(2);
			} else if (s == ".I.") {
				pts.push_back(1);
			} else assert(false);
		}
		int ans = 0;
		for (int i = 0; i < N; i++) {
			if (pts[i] == 1) {
				for (int j : {i-1, i+1}) {
					if (!(0 <= j && j < N)) continue;
					assert(pts[j] != 1);
					if (pts[j] == 3) {
						ans ^= 1;
					}
					pts[j] = 0;
				}
			}
		}
		for (int i = 0; i < N; ) {
			int v = 0;
			int j = i;
			while (j < N && pts[j] >= 2) {
				v = (v << 1) | (pts[j] - 2);
				j++;
			}
			//cerr << j-i << ' ' << v << ' ' << getNimber(j-i, v) << '\n';
			ans ^= getNimber(j - i, v);
			i = max(i+1, j);
		}
		cout << (ans == 0 ? "Second" : "First") << '\n';
	}

	return 0;
}

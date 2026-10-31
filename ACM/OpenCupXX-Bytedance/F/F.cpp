#include<bits/stdc++.h>

template <typename T> T& setmin(T& a, const T& b) { if (b < a) a = b; return a; }

int go(int N, int C, std::vector<int> A) {
	using namespace std;
	int cur = 0;
	vector<int> pos; pos.reserve(N);
	vector<int> neg; neg.reserve(N); // absolute value
	for (auto& a : A) {
		assert(a > 0);
		if (a + cur <= C) {
			cur += a;
			neg.push_back(a);
		} else {
			pos.push_back(a);
		}
	}

	assert(cur <= C);

	if (pos.empty()) return cur;
	assert(C - cur < *min_element(pos.begin(), pos.end()));

	const int M = *max_element(A.begin(), A.end());

	const int BASE = cur - M;
	const int INF = int(pos.size())+1;
	vector<int> prvDp(C + M - BASE + 1, INF);
	vector<int> curDp(C + M - BASE + 1, INF);
	curDp[cur - BASE] = 0;

	for (int i = 0; i <= int(neg.size()); i++) {
		for (int v = cur - M; v <= C; v++) {
			int st = curDp[v - BASE];
			int en = min(int(pos.size()), prvDp[v - BASE]);
			for (int j = st; j < en; j++) {
				setmin(curDp[v + pos[j] - BASE], j+1);
			}
		}
		if (i < int(neg.size())) {
			std::swap(curDp, prvDp);
			std::copy(prvDp.begin(), prvDp.end(), curDp.begin());
			for (int v = cur; v <= C + M; v++) {
				setmin(curDp[v - neg[i] - BASE], prvDp[v - BASE]);
			}
		}
	}
	for (int v = C; v >= cur; v--) {
		if (curDp[v - BASE] != INF) return v;
	}
	assert(false);
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int T; cin >> T;
	while (T--) {
		int N, C; cin >> N >> C;
		std::vector<int> A(N); for (auto& it : A) cin >> it;
		int ans = go(N, C, A);
		cout << ans << '\n';
	}

	return 0;
}

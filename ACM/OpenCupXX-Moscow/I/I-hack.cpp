#include<bits/stdc++.h>
using namespace std;

namespace std {

template<class Fun>
	class y_combinator_result {
		Fun fun_;
	public:
		template<class T>
			explicit y_combinator_result(T &&fun): fun_(std::forward<T>(fun)) {}

		template<class ...Args>
			decltype(auto) operator()(Args &&...args) {
				return fun_(std::ref(*this), std::forward<Args>(args)...);
			}
	};

template<class Fun>
	decltype(auto) y_combinator(Fun &&fun) {
		return y_combinator_result<std::decay_t<Fun>>(std::forward<Fun>(fun));
	}

} // namespace std

const int BLOCK = 300;

int main() {
	ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
	int N; cin >> N;
	vector<pair<int, int>> A(N);
	for (auto& it : A) cin >> it.first >> it.second;
	sort(A.begin(), A.end());

	int M; cin >> M;
	vector<pair<pair<int, int>, int>> B(M);
	for (int q = 0; q < M; q++) {
		int x, y; cin >> x >> y;
		B[q] = {{x,y}, q};
	}

	const int S = 1 << 17;
	vector<set<pair<int, int>>> seg(2*S);
	vector<set<pair<int, int>>> segDone(2*S);
	for (int i = 0; i < N; i++) {
		int x, y; tie(x,y) = A[i];
		for (int a = S+x; a; a /= 2) {
			seg[a].insert(pair<int, int>(y, i));
		}
	}

	auto moveA = [&](int i) {
		int x, y; tie(x,y) = A[i];
		for (int a = S+x; a; a /= 2) {
			seg[a].erase(pair<int, int>(y, i));
			segDone[a].insert(pair<int, int>(y, i));
		}
	};
	auto removeA = [&](int i) {
		int x, y; tie(x,y) = A[i];
		for (int a = S+x; a; a /= 2) {
			segDone[a].erase(pair<int, int>(y, i));
		}
	};

	vector<int> rev(N, -1);
	vector<int> vis(M);

	mt19937 mt(48);

	auto augment = y_combinator([&](auto self, int cur) -> bool {
		if (cur == -1) return true;
		if (vis[cur]) return false;
		vis[cur] = true;
		vector<int> segInds;
		for (int a = S, b = S+B[cur].first.first+1; a < b; a /= 2, b /= 2) {
			if (b & 1) {
				b--;
				segInds.push_back(b);
			}
		}
		shuffle(segInds.begin(), segInds.end(), mt);
		auto check = [&](pair<int, int> it) -> bool {
			int cnd = it.second;
			if (self(rev[cnd])) {
				if (rev[cnd] == -1) {
					moveA(cnd);
				}
				rev[cnd] = cur;
				vis[cur] = false;
				return true;
			} else {
				removeA(cnd);
				return false;
			}
		};
		for (int b : segInds) {
			auto& st = seg[b];
			if (mt() & 1) {
				auto hiIt = st.upper_bound(pair<int, int>(B[cur].first.second, N));
				for (auto iter = hiIt; iter != st.begin();) {
					--iter;
					auto it = *iter;
					if (check(it)) return true;
				}
			} else {
				for (auto it : st) {
					if (it.first > B[cur].first.second) {
						break;
					}
					if (check(it)) return true;
				}
			}
		}
		for (int b : segInds) {
			auto& st = segDone[b];
			if (mt() & 1) {
				auto hiIt = st.upper_bound(pair<int, int>(B[cur].first.second, N));
				for (auto iter = hiIt; iter != st.begin();) {
					--iter;
					auto it = *iter;
					if (check(it)) return true;
				}
			} else {
				for (auto it : st) {
					if (it.first > B[cur].first.second) {
						break;
					}
					if (check(it)) return true;
				}
			}
		}
		return false;
	});

	int ans = 0;
	for (int q = 0; q < M; q++) {
		if (augment(q)) {
			ans++;
		}
		cout << ans << '\n';
	}


	return 0;
}

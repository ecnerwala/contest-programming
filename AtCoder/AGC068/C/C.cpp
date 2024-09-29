#include <bits/stdc++.h>

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

bool solve(std::vector<int> A) {
	int N = int(A.size());
	std::vector<int> vis(N, -1);
	std::vector<bool> is_cyc(N);
	for (int i = 0; i < N; i++) {
		// first, find and break cycles
		int cur = i;
		while (vis[cur] == -1) {
			vis[cur] = i;
			cur = A[cur];
		}
		if (vis[cur] == i) {
			// we've hit a cycle starting with cur
			for (int a = A[cur]; true; a = A[a]) {
				is_cyc[a] = true;
				if (a == cur) break;
			}
		}
	}
	std::vector<std::vector<int>> ch(N);
	for (int i = 0; i < N; i++) {
		if (!is_cyc[i]) ch[A[i]].push_back(i);
	}
	bool good = true;
	for (int i = 0; i < N; i++) {
		if (is_cyc[i]) {
			std::y_combinator([&](auto self, int cur) -> int {
				int res = cur;
				for (int nxt : ch[cur]) {
					int v = self(nxt);
					if (v < cur) {
						good = false;
					}
					res = std::max(res, v);
				}
				return res;
			})(i);
		}
	}
	return good;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	// The max of every subtree must be greater than you
	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N); for (auto& a : A) { cin >> a; a--; }
		cout << (solve(A) ? "Yes" : "No") << '\n';
	}

	return 0;
}

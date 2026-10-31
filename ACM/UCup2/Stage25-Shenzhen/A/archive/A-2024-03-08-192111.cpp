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

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<std::pair<int, int>> A(N);
	for (int i = 0; i < N; i++) {
		cin >> A[i].first;
		A[i].second = i+1;
	}
	std::sort(A.begin(), A.end());
	std::vector<std::pair<int, int>> ops;
	ops.reserve(20000);
	for (int z = 0; z < A[0].first; z++) ops.push_back({1, z});

	std::y_combinator([&](auto self, int l, int r, int val) -> void {
		assert(l < r);
		assert(val == A[l].first);
		if (r - l == 1) return;
		int m = (l + r) / 2;
		if (A[m].first > val) {
			for (int i = m; i < r; i++) ops.push_back({2, A[i].second});
			for (int z = val+1; z < A[m].first; z++) ops.push_back({1, z});
		}
		self(m, r, A[m].first);
		self(l, m, val);
	})(0, N, A[0].first);
	cout << ops.size() << '\n';
	for (auto [x, y] : ops) {
		cout << x << ' ' << y << '\n';
	}

	return 0;
}

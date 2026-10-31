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
	std::vector<int64_t> A(N);
	for (auto& a : A) cin >> a;

	std::vector<std::vector<int>> adj(N);
	for (int e = 0; e < N-1; e++) {
		int u, v; cin >> u >> v;
		u--, v--;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}
	int64_t finals = 0;
	auto resp = std::y_combinator([&](auto self, int cur, int prv) -> std::priority_queue<int64_t> {
		std::priority_queue<int64_t> pq;
		for (int nxt : adj[cur]) {
			if (nxt == prv) continue;
			auto ch = self(nxt, cur);
			if (ch.size() > pq.size()) {
				std::swap(pq, ch);
			}
			while (!ch.empty()) {
				pq.push(ch.top());
				ch.pop();
			}
		}
		int64_t v = A[cur];
		while (!pq.empty() && v <= pq.top()) {
			int64_t m = pq.top();
			pq.pop();
			if (pq.empty()) {
				finals += v - m;
				return {};
			} else {
				int64_t u = pq.top(); pq.pop();
				v -= m - u;
			}
		}
		pq.push(v);
		return pq;
	})(0, -1);
	int sign = 1;
	int64_t ans = 0;
	while (!resp.empty()) {
		ans += resp.top() * sign; resp.pop();
		sign = -sign;
	}
	ans += sign * finals;

	ans = (ans + std::accumulate(A.begin(), A.end(), int64_t(0))) / 2;
	cout << ans << '\n';

	return 0;
}

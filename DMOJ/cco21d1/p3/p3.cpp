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

	int N, Q; cin >> N >> Q;
	vector<vector<int>> C(N);
	for (int i = 0; i < N; i++) {
		int K; cin >> K;
		C[i].resize(K);
		for (auto& c : C[i]) { cin >> c; c--; }
	}

	vector<int> euler_tour; euler_tour.reserve(2*(N-1));
	vector<int> start_idx(N);
	map<int, int> unvisited;
	std::y_combinator([&](auto self, int cur, int prv) -> void {
		int first_loc = C[cur][1 % int(C[cur].size())];
		if (prv != -1) {
			auto it = std::find(C[cur].begin(), C[cur].end(), prv);
			std::rotate(C[cur].begin(), it + 1, C[cur].end());
			assert(C[cur].back() == prv);
			C[cur].pop_back();
		}
		for (int nxt : C[cur]) {
			if (first_loc == nxt) {
				start_idx[cur] = int(euler_tour.size());
			}
			unvisited.insert({int(euler_tour.size()), nxt});
			euler_tour.push_back(cur);
			self(nxt, cur);
			euler_tour.push_back(nxt);
		}
		if (first_loc == prv) {
			start_idx[cur] = int(euler_tour.size());
		}
	})(0, -1);
	assert(int(unvisited.size()) == N-1);
	assert(int(euler_tour.size()) == 2 * (N-1));

	vector<pair<int64_t, int>> segments; segments.reserve(N);
	int64_t cur_t = 0;
	int cur = 0;
	while (true) {
		int cur_idx = start_idx[cur];
		segments.push_back({cur_t, cur_idx});

		if (unvisited.empty()) break;

		int delta_t = 0;
		auto it = unvisited.lower_bound(cur_idx);
		if (it == unvisited.end()) {
			delta_t += int(euler_tour.size());
			it = unvisited.begin();
		}
		delta_t += it->first - cur_idx + 1;
		cur_t += delta_t;
		cur = it->second;
		unvisited.erase(it);
	}

	while (Q--) {
		int64_t K; cin >> K;
		auto it = upper_bound(segments.begin(), segments.end(), pair<int64_t, int>{K+1, -1});
		--it;
		int idx = int((it->second + (K - it->first)) % int(euler_tour.size()));
		int resp = euler_tour[idx];
		cout << resp+1 << '\n';
	}

	return 0;
}

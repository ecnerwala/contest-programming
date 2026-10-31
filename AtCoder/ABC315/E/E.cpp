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
	std::vector<std::vector<int>> in_edges(N);
	std::vector<int> outdeg(N);
	for (int i = 0; i < N; i++) {
		int d; cin >> d;
		in_edges[i].resize(d);
		for (auto& j : in_edges[i]) {
			cin >> j; j--;
			outdeg[j]++;
		}
	}
	std::vector<int> q; q.reserve(N);
	for (int i = 0; i < N; i++) {
		if (outdeg[i] == 0) q.push_back(i);
	}
	std::vector<int> needed(N);
	needed[0] = true;
	std::vector<int> ans; ans.reserve(N);

	for (int z = 0; z < int(q.size()); z++) {
		int cur = q[z];
		if (needed[cur]) ans.push_back(cur);
		for (int nxt : in_edges[cur]) {
			if (needed[cur]) needed[nxt] = true;
			assert(outdeg[nxt] > 0);
			outdeg[nxt]--;
			if (outdeg[nxt] == 0) {
				q.push_back(nxt);
			}
		}
	}
	assert(int(q.size()) == N);
	std::reverse(ans.begin(), ans.end());
	ans.pop_back();
	for (int i = 0; i < int(ans.size()); i++) {
		cout << ans[i]+1 << " \n"[i+1==int(ans.size())];
	}

	return 0;
}

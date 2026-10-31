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

	int N_SOLS, M; cin >> N_SOLS >> M;
	assert(N_SOLS > 0);

	using bs = bitset<2000>;
	std::vector<bs> poss(M);
	for (int i = 0; i < N_SOLS; i++) {
		string s; cin >> s;
		for (int j = 0; j < M; j++) {
			assert(s[j] == '0' || s[j] == '1');
			poss[j][i] = s[j] == '1';
		}
	}
	for (int j = 0; j < M; j++) {
		bool v0 = poss[j][0];
		for (int i = N_SOLS; i < 2000; i++) {
			poss[j][i] = v0;
		}
	}

	std::vector<std::tuple<int, int, int>> constraints; constraints.reserve(2*M*M);
	bs always_set;
	bs always_unset;
	for (int i = 0; i < M; i++) {
		if (!(poss[i].any())) {
			constraints.push_back({i, i, 1});
			always_unset[i] = true;
		} else if (poss[i].all()) {
			constraints.push_back({i, i, 4});
			always_set[i] = true;
		}
	}

	std::vector<bs> implies_set(2*M);
	std::vector<bs> implies_unset(2*M);
	for (int i = 0; i < M; i++) {
		implies_set[2*i+1][i] = true;
		implies_unset[2*i][i] = true;
	}

	for (int i = 0; i < M; i++) {
		if (always_set[i] || always_unset[i]) continue;
		for (int j = i+1; j < M; j++) {
			if (always_set[j] || always_unset[j]) continue;
			if (((~poss[i]) | (~poss[j])).all()) {
				constraints.push_back({i, j, 1});
				implies_unset[2*i+1][j] = true;
				implies_unset[2*j+1][i] = true;
			}
			if (((poss[i]) | (~poss[j])).all()) {
				constraints.push_back({i, j, 2});

				implies_unset[2*i][j] = true;
				implies_set[2*j+1][i] = true;
			}
			if (((~poss[i]) | (poss[j])).all()) {
				constraints.push_back({i, j, 3});
				implies_set[2*i+1][j] = true;
				implies_unset[2*j][i] = true;
			}
			if (((poss[i]) | (poss[j])).all()) {
				constraints.push_back({i, j, 4});
				implies_set[2*i][j] = true;
				implies_set[2*j][i] = true;
			}
		}
	}
	/*
		cerr << constraints.size() << '\n';
		for (auto [i, j, t] : constraints) {
			cerr << i << ' ' << j << ' ' << t << '\n';
		}
		*/

	bs ones;
	for (int i = 0; i < M; i++) ones[i] = true;

	// The implication graph implied here is necessarily transitively closed already
	// Thus, generating assignments can be fast, since there are no transitive dependencies
	int num_solutions = 0;
	std::y_combinator([&](auto self, bs cur_set, bs cur_unset) -> void {
		//for (int i = 0; i < M; i++) { cerr << cur_set[i]; } cerr << '\n';
		//for (int i = 0; i < M; i++) { cerr << cur_unset[i]; } cerr << '\n';
		bs cur_unknown = ones & ~cur_set & ~cur_unset;
		int idx = int(cur_unknown._Find_first());
		if (idx >= M) {
			num_solutions ++;
			return;
		}
		if (num_solutions > N_SOLS) return;
		{
			// now, just set i
			bs nxt_set = cur_set | implies_set[2*idx+1];
			bs nxt_unset = cur_unset | implies_unset[2*idx+1];
			self(nxt_set, nxt_unset);
		}
		if (num_solutions > N_SOLS) return;
		{
			bs nxt_set = cur_set | implies_set[2*idx];
			bs nxt_unset = cur_unset | implies_unset[2*idx];
			self(nxt_set, nxt_unset);
		}
	})(always_set, always_unset);


	assert(num_solutions >= N_SOLS);
	if (num_solutions > N_SOLS) {
		cout << -1 << '\n';
	} else {
		cout << constraints.size() << '\n';
		for (auto [i, j, t] : constraints) {
			cout << i << ' ' << j << ' ' << t << '\n';
		}
	}

	return 0;
}

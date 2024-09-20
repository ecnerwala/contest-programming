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

bool is_valid(std::vector<int>& P, std::vector<int>& L, std::vector<int>& ordering) {
	int N = int(P.size());
	assert(int(L.size()) == N);
	assert(int(ordering.size()) == N);
	std::vector<int> loc(N, -1);
	for (int i = 0; i < N; i++) {
		assert(0 <= ordering[i] && ordering[i] < N);
		assert(loc[ordering[i]] == -1);
		loc[ordering[i]] = i;
	}

	for (int i = 0; i < N; i++) {
		if (loc[P[i]] < loc[i]) {
			// we're fine
		} else if (loc[i] < loc[L[i]] && loc[L[i]] < loc[P[i]]) {
			// we're also okay
		} else {
			return false;
		}
	}
	return true;
}

std::optional<std::vector<int>> solve_brute_force(std::vector<int> P, std::vector<int> L) {
	int N = int(P.size());
	assert(N == int(L.size()));
	std::vector<int> ordering(N);
	std::iota(ordering.begin(), ordering.end(), 0);
	do {
		if (is_valid(P, L, ordering)) {
			return ordering;
		}
	} while (std::next_permutation(ordering.begin(), ordering.end()));
	return std::nullopt;
}

std::optional<std::vector<int>> solve(std::vector<int> P, std::vector<int> L) {
	int N = int(P.size());
	assert(N == int(L.size()));

	std::vector<int> prv(N+1);
	std::vector<int> nxt(N+1);
	std::iota(prv.begin(), prv.end(), 0);
	std::iota(nxt.begin(), nxt.end(), 0);

	std::vector<bool> alive(N, true);

	std::vector<bool> broken(N, false);
	std::vector<int> early(N, -1);
	if (std::y_combinator([&](auto self, int root, int top, int early_epoch) -> bool {
		// root is the bottom of the cycle
		// root must come after everything with early[i] == early_epoch
		for (int cur = root; true; cur = P[cur]) {
			// we actually looped back somewhere else, so this guy is good
			if (!alive[cur]) return true;
			alive[cur] = false;
			if (P[cur] == top) break;
		}

		for (int cur = root; true; cur = P[cur]) {
			// We're going to try to break the cur .. P[cur] edge
			int nroot = L[cur];
			int ntop = P[cur];
			if (early[nroot] == early_epoch) {
				// excluded
			} else {
				int n_early_epoch = early_epoch + (cur != root);
				// don't worry if early is already set, cause we'll always return true
				early[nroot] = n_early_epoch;
				if (self(nroot, ntop, n_early_epoch)) {
					broken[cur] = true;
					return true;
				}
				early[nroot] = -1;
			}

			if (P[cur] == top) break;
		}

		for (int cur = root; true; cur = P[cur]) {
			alive[cur] = true;
			if (P[cur] == top) break;
		}
		early[root] = -1;
		return false;
	})(0, 0, 0)) {
		return std::vector<int>({0});
	} else {
		return std::nullopt;
	}
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<int> P(N);
	std::vector<int> L(N);
	for (int i = 0; i < N; i++) {
		cin >> P[i] >> L[i]; P[i]--, L[i]--;
	}

	auto res = solve(P, L);
	if (res) {
		for (int i = 0; i < N; i++) {
			cout << (*res)[i] << " \n"[i+1==N];
		}
	} else {
		cout << "impossible" << '\n';
	}

	return 0;
}

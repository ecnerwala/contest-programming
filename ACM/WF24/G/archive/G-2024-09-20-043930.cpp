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
				early[cur] = n_early_epoch;
				if (self(nroot, ntop, n_early_epoch)) {
					std::cerr << "break " << cur << ' ' << P[cur] << '\n';
					broken[cur] = true;
					return true;
				}
				early[cur] = -1;
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
		std::vector<std::vector<int>> out_edges(N);
		std::vector<int> in_deg(N);
		for (int i = 0; i < N; i++) {
			if (broken[i]) {
				out_edges[P[i]].push_back(L[i]);
				in_deg[L[i]]++;
				out_edges[L[i]].push_back(i);
				in_deg[i]++;
			} else {
				out_edges[i].push_back(P[i]);
				in_deg[P[i]] ++;
			}
		}
		std::vector<int> ordering; ordering.reserve(N);
		for (int i = 0; i < N; i++) {
			if (!in_deg[i]) ordering.push_back(i);
		}
		for (int z = 0; z < int(ordering.size()); z++) {
			for (int u : out_edges[ordering[z]]) {
				if (!--in_deg[u]) ordering.push_back(u);
			}
		}
		assert(int(ordering.size()) == N);
		std::reverse(ordering.begin(), ordering.end());
		assert(is_valid(P, L, ordering));
		return ordering;
	} else {
		return std::nullopt;
	}
}

void test() {
	std::mt19937 mt(48);
	for (int z = 0; z < 1000000; z++) {
		std::cerr << "test " << z << '\n';
		int N = std::uniform_int_distribution(3, 10)(mt);
		std::vector<int> P(N);
		std::vector<int> L(N);
		for (int i = 0; i < N; i++) {
			if (i) {
				P[i] = std::uniform_int_distribution(0, i-1)(mt);
			} else {
				P[i] = std::uniform_int_distribution(1, N-1)(mt);
			}

			do {
				L[i] = std::uniform_int_distribution(0, N-1)(mt);
			} while (L[i] == i || L[i] == P[i]);
		}
		std::cerr << N << '\n';
		for (int i = 0; i < N; i++) {
			std::cerr << P[i] << ' ' << L[i] << '\n';
		}
		auto r = solve(P, L);
		if (r) {
			assert(is_valid(P, L, *r));
		} else {
			auto slo = !solve_brute_force(P, L);
			if (slo) {
				for (auto v : *slo) { std::cerr << v << ' '; } std::cerr << '\n';
				assert(!slo);
			}
		}
	}
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	test();

	int N; cin >> N;
	std::vector<int> P(N);
	std::vector<int> L(N);
	for (int i = 0; i < N; i++) {
		cin >> P[i] >> L[i]; P[i]--, L[i]--;
	}

	auto res = solve(P, L);
	if (res) {
		for (int i = 0; i < N; i++) {
			cout << (*res)[i]+1 << " \n"[i+1==N];
		}
	} else {
		cout << "impossible" << '\n';
	}

	return 0;
}

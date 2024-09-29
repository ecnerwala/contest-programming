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
	std::vector<int> val(N);
	std::iota(val.begin(), val.end(), 0);
	std::vector<int> deg(N);
	for (int i = 0; i < N; i++) deg[A[i]] += i;
	std::vector<int> q; q.reserve(N);
	for (int i = 0; i < N; i++) {
		if (!deg[i]) q.push_back(i);
	}
	for (int z = 0; z < int(q.size()); z++) {
		int cur = q[z];
		int nxt = A[cur];
		if (val[cur] < nxt) return false;
		val[nxt] = std::min(val[nxt], val[cur]);
		if (!--deg[nxt]) q.push_back(nxt);
	}
	return true;
}

void test(int N) {
	std::vector<std::vector<int>> all_paths;
	all_paths.emplace_back();
	for (int z = 0; z < N; z++) {
		std::vector<std::vector<int>> n_all_paths; n_all_paths.reserve(all_paths.size() * N);
		for (int i = 0; i < N; i++) {
			for (auto p : all_paths) {
				p.push_back(i);
				n_all_paths.emplace_back(std::move(p));
			}
		}
		all_paths = std::move(n_all_paths);
	}

	std::set<std::vector<int>> legal_paths(all_paths.begin(), all_paths.end());
	for (int cur = 0; cur < N; cur++) {
		std::set<std::vector<int>> nxt_legal_paths;
		for (const auto& A : legal_paths) {
			std::vector<int> locs; locs.reserve(N);
			for (int i = 0; i < N; i++) {
				if (A[i] == cur) locs.push_back(i);
			}
			do {
				std::vector<int> nA = A;
				for (int i = 0; i < int(locs.size()); i++) {
					nA[locs[i]] = locs[(i + 1) % int(locs.size())];
				}
				nxt_legal_paths.insert(nA);
			} while (std::next_permutation(locs.begin(), locs.end()));
		}
		legal_paths = std::move(nxt_legal_paths);
	}
	for (auto p : all_paths) {
		bool mine = solve(p);
		bool real = legal_paths.count(p);
		assert(mine == real);
	}
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	//for (int t = 1; t <= 7; t++) { cerr << "testing " << t << '\n'; test(t); }

	// The max of every subtree must be greater than you
	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N); for (auto& a : A) { cin >> a; a--; }
		cout << (solve(A) ? "Yes" : "No") << '\n';
	}

	return 0;
}

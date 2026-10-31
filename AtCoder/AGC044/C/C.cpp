#include<bits/stdc++.h>

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

	vector<int> perm;
	auto solve = std::y_combinator([&](auto self, int N, string T, int base = 1, int loS = 0, int loT = 0) -> void {
		if (N == 0) {
			perm[loS] = loT;
			return;
		}
		for (int v = 0; v < 3; v++) {
			string nT;
			int nV = v;
			for (char c : T) {
				if (c == 'S') {
					if (!nT.empty() && nT.back() == 'S') nT.pop_back();
					else nT += 'S';
					if (nV) nV = 3 - nV;
				} else {
					if (nV == 2) {
						nT += 'R';
						nV = 0;
					} else {
						nV++;
					}
				}
			}
			self(N-1, std::move(nT), base * 3, loS + base *v, loT + base * nV);
		}
	});

	int N; string T; cin >> N >> T;
	int p3 = 1; for (int i = 0; i < N; i++) p3 *= 3;
	perm.resize(p3);
	solve(N, T);
	for (int z = 0; z < p3; z++) {
		cout << perm[z] << " \n"[false && z+1==p3];
	}
	cout << '\n';

	return 0;
}

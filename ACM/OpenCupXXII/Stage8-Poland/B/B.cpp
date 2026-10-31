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

struct map_t {
	std::set<int> lefts;
	std::set<int> rights;
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; int64_t C; cin >> N >> C;
		std::vector<int> A(N);
		std::vector<int> B(N);
		int64_t cost = 0;
		for (int i = 0; i < N; i++) {
			cin >> A[i] >> B[i];
			cost += std::abs(A[i] - B[i]);
		}
		cost /= 2;

		std::vector<int> vals = A;
		std::sort(vals.begin(), vals.end());
		for (int i = 0; i < N; i++) {
			A[i] = int(std::lower_bound(vals.begin(), vals.end(), A[i]) - vals.begin());
			B[i] = int(std::lower_bound(vals.begin(), vals.end(), B[i]) - vals.begin());
		}

		std::vector<int> orig_idx(N);
		for (int i = 0; i < N; i++) {
			orig_idx[B[i]] = i;
		}

		std::vector<int> targets(N);
		for (int i = 0; i < N; i++) {
			targets[A[i]] = B[i];
		}

		std::vector<std::array<int, 2>> ops; ops.reserve(N);
		auto do_op = [&](int x, int y) {
			ops.push_back({targets[x], targets[y]});
		};

		//for (int i = 0; i < N; i++) { cerr << targets[i] << ' '; } cerr << '\n';

		auto solve = std::y_combinator([&](auto self, map_t& mp) -> void {
			while (true) {
				if (mp.lefts.empty()) return;

				int x, y;
				{
					auto xit = mp.lefts.begin();
					x = *xit;
					mp.lefts.erase(xit);
				}
				{
					auto yit = mp.rights.lower_bound(x);
					assert(yit != mp.rights.begin());
					--yit;
					y = *yit;
					mp.rights.erase(yit);
				}
				assert(y < x);
				assert(targets[y] >= x && targets[x] <= y);
				do_op(x, y);
				std::swap(targets[x], targets[y]);

				if (targets[x] >= x) {
					mp.rights.insert(x);
				} else {
					mp.lefts.insert(x);
				}
				if (targets[y] >= y) {
					mp.rights.insert(y);
				} else {
					mp.lefts.insert(y);
				}

				// now, we trace the two cycles and set x to the shorter one
				for (int xcur = x, ycur = y; true; ) {
					xcur = targets[xcur];
					ycur = targets[ycur];
					if (xcur == x) {
						break;
					} else if (ycur == y) {
						std::swap(x, y);
						break;
					}
				}

				map_t nmp;
				for (int cur = x; true; ) {
					if (targets[cur] >= cur) {
						int found = int(mp.rights.erase(cur));
						assert(found);
						nmp.rights.insert(cur);
					} else {
						int found = int(mp.lefts.erase(cur));
						assert(found);
						nmp.lefts.insert(cur);
					}
					cur = targets[cur];
					if (cur == x) break;
				}

				self(nmp);
			}
		});

		for (int i = 0; i < N; i++) {
			if (targets[i] == i) {
				continue;
			}
			map_t mp;
			for (int cur = i; true; cur = targets[cur]) {
				if (targets[cur] >= cur) {
					mp.rights.insert(cur);
				} else {
					mp.lefts.insert(cur);
				}
				if (targets[cur] == i) break;
			}
			solve(mp);
		}

		cost += C * int(ops.size());
		cout << cost << ' ' << ops.size() << '\n';
		for (auto [x, y] : ops) {
			cout << orig_idx[x]+1 << ' ' << orig_idx[y]+1 << '\n';
		}
	}

	return 0;
}

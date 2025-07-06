#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> X(N);
		for (auto& x : X) cin >> x;
		assert(X[0] == 1);
		auto ans = [&]() -> std::optional<std::vector<int>> {
			std::vector<int> stk(N+1, -1);
			stk[X[0]] = 0;
			std::vector<int> par(N, -1);
			std::vector<bool> is_extend(N, false);
			for (int i = 1; i < N; i++) {
				if (X[i] > X[i-1] + 1) {
					return std::nullopt;
				}
				if (X[i] == 1) return std::nullopt;
				assert(X[i] >= 2);
				par[i] = stk[X[i]-1];
				if (X[i] <= X[i-1]) is_extend[par[i]] = true;
				stk[X[i]] = i;
			}
			for (int i = 1; i < N; i++) {
				if (is_extend[i] && is_extend[par[i]]) {
					return std::nullopt;
				}
			}
			std::vector<int> nxt(N, -1);
			struct lst_t {
				int h, t;
			};
			auto merge = [&](lst_t a, lst_t b) -> lst_t {
				if (a.t == -1) return b;
				if (b.t == -1) return a;
				nxt[a.t] = b.h;
				return {a.h, b.t};
			};
			std::vector<lst_t> lsts(N, {-1, -1});
			for (int i = N-1; i >= 0; i--) {
				if (is_extend[i]) {
					lsts[i] = merge({i, i}, lsts[i]);
				} else {
					lsts[i] = merge(lsts[i], {i, i});
				}
				if (i == 0) break;
				int p = par[i];
				lsts[p] = merge(lsts[i], lsts[p]);
			}
			std::vector<int> res(N, -1);
			for (int v = 0, cur = lsts[0].h; v < N; v++, cur = nxt[cur]) {
				res[cur] = v;
			}
			return res;
		}();
		if (ans) {
			cout << "YES" << '\n';
			for (int i = 0; i < N; i++) {
				cout << (*ans)[i] + 1 << " \n"[i+1==N];
			}
		} else {
			cout << "NO" << '\n';
		}
	}

	return 0;
}

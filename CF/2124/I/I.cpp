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
			std::vector<int> extend_par(N, -1);
			std::vector<bool> is_extend(N, false);
			for (int i = 1; i < N; i++) {
				if (X[i] > X[i-1] + 1) {
					return std::nullopt;
				}
				if (X[i] == 1) return std::nullopt;
				assert(X[i] >= 2);
				if (X[i] <= X[i-1]) {
					// jump to the last time this occurred
					// revert to this guy, so our parent is the same as theirs
					int j = stk[X[i]];
					assert(j >= 1);
					is_extend[j] = true;
					is_extend[i] = true;
					extend_par[i] = extend_par[j];
				} else {
					assert(X[i] == X[i-1] + 1);
					// could be an increment or an extend
					extend_par[i] = i-1;
				}
				stk[X[i]] = i;
			}
			assert(!is_extend[0]);
			for (int i = 0; i < N; i++) {
				if (is_extend[i] && is_extend[extend_par[i]]) {
					return std::nullopt;
				}
			}
			std::vector<int> prv(N, -1);
			std::vector<int> nxt(N, -1);
			struct lst_t {
				int h, t;
			};
			auto merge = [&](lst_t a, lst_t b) -> lst_t {
				if (a.t == -1) return b;
				if (b.t == -1) return a;
				nxt[a.t] = b.h;
				prv[b.h] = a.t;
				return {a.h, b.t};
			};
			std::vector<lst_t> lsts(N);
			for (int i = 0; i < N; i++) {
				lsts[i] = {-1, -1};
			}
			for (int i = N-1; i >= 0; i--) {
				if (lsts[i].t == i) {
					// our child is not extend
				} else {
					assert(prv[i] == -1 && nxt[i] == -1);
					lsts[i] = merge({i, i}, lsts[i]);
				}
				if (i == 0) break;
				int p = extend_par[i];
				if (is_extend[i]) {
					lsts[p] = merge(lsts[i], lsts[p]);
				} else {
					lsts[p] = merge(lsts[i], {p, p});
				}
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

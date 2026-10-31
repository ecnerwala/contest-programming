#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M;
	while (cin >> N >> M) {
		vector<int> val(2*M, -1);
		vector<int> pos(2*N, -1);
		for (int e = 0; e < M; e++) {
			int k; cin >> k;
			for (int z = 0; z < k; z++) {
				int v; cin >> v; v--;
				v *= 2;
				if (pos[v] != -1) v++;
				assert(pos[v] == -1);
				int p = 2*e+z;
				pos[v] = p;
				val[p] = v;
			}
		}
		for (int i = 0; i < 2*N; i++) {
			assert(pos[i] != -1);
		}

		// from - to
		vector<pair<int, int>> plan; plan.reserve(3*N/2);
		auto do_op = [&](int f, int t) -> void {
			assert(0 <= f && f < 2*M);
			assert(0 <= t && t < 2*M);
			assert(val[f] != -1);
			assert(val[t] == -1);
			plan.push_back({f/2, t/2});
			if (t & 1) {
				assert(val[t-1] != -1 && val[f]/2 == val[t-1]/2);
			}
			int v = val[f];
			val[t] = val[f];
			val[f] = -1;

			assert(pos[v] == f);
			pos[v] = t;
		};

		auto solve = [&]() -> bool {
			vector<int> empty; empty.reserve(M);
			for (int m = 0; m < 2*M; m += 2) {
				if (val[m] == -1) empty.push_back(m);
			}

			// check if this position is an open loner
			auto check_pos = [&](int p) -> int {
				while (true) {
					assert(!(p & 1));
					assert(val[p+1] == -1);
					int v = val[p];
					if (!(pos[v^1] % 2 == 1 || val[pos[v^1]+1] == -1)) return p;
					int f = pos[v^1];
					do_op(f, p+1);
					if (f % 2 == 0) {
						empty.push_back(f);
						return -1;
					}
					p = f-1;
				}
			};

			// First, clean up all free paths
			for (int i = 0; i < 2*N; i++) {
				if (!(pos[i] & 1) && val[pos[i]+1] == -1) {
					check_pos(pos[i]);
				}
			}

			auto clean_path = [&](int p) -> bool {
				for (p = check_pos(p); p != -1; ) {
					assert(pos[val[p]^1] % 2 == 0 && val[pos[val[p]^1]+1] != -1);
					assert(val[p] != -1);
					int np = p;
					assert(val[np] != -1);
					while (pos[val[np]^1] % 2 == 0 && val[pos[val[np]^1]+1] != -1) {
						np = pos[val[np]^1]+1;
						assert(val[np] != -1);
					}
					assert(np & 1);
					if (pos[val[np]^1] % 2 == 0 && val[pos[val[np]^1]+1] == -1) {
						int vleft = check_pos(pos[val[np]^1]);
						assert(vleft == -1);
						break;
					}
					assert(pos[val[np]^1] % 2 == 1);
					if (empty.empty()) return false;
					int ep = empty.back(); empty.pop_back();
					do_op(np, ep);
					int vleft = check_pos(np-1);
					assert(vleft == -1);
					p = check_pos(ep);
				}
				return true;
			};

			// Now, clean up all paths that cost 1
			for (int st = 0; st < 2*N; st++) {
				if (!(pos[st] & 1) && val[pos[st]+1] == -1) {
					if (!clean_path(pos[st])) return false;
				}
			}

			// We've now cleaned up all orphans
			for (int e = 0; e < M; e++) {
				assert((val[2*e] == -1) == (val[2*e+1] == -1));
			}

			// Now, we can clean up cycles
			for (int st = 0; st < 2*N; st += 2) {
				if ((pos[st] & 1) && (pos[st^1] & 1)) {
					// break this
					if (empty.empty()) return false;
					int ep = empty.back(); empty.pop_back();
					do_op(pos[st], ep);
					if (!clean_path(ep)) return false;
				}
			}

			for (int st = 0; st < 2*N; st ++) {
				if ((pos[st] & 1) && pos[st] != (pos[st^1]^1)) {
					if (empty.empty()) return false;
					int ep = empty.back(); empty.pop_back();
					do_op(pos[st], ep);
					if (!clean_path(ep)) return false;
				}
			}

			return true;
		};

		if (solve() && int(plan.size()) <= 3 * N / 2) {
			cout << plan.size() << '\n';
			for (auto it : plan) {
				cout << it.first+1 << ' ' << it.second+1 << '\n';
			}
		} else {
			cout << -1 << '\n';
		}
	}


	return 0;
}

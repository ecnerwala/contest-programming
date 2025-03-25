#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T, M; cin >> T >> M;
	while (T--) {
		int N; cin >> N;
		std::vector<std::array<std::array<int, 2>, 2>> corners(N);
		for (auto& c : corners) for (auto& v : c) for (auto& x : v) { cin >> x; x--; }
		if (M == 1) {
			// solve the actual problem I guess?
			// just implicit dfs?
			int S = 1; while (S < 2*N) S *= 2;
			std::vector<int> seg(2*S, 2*N);
			std::vector<int> x_to_idx(2*N);
			for (int i = 0; i < N; i++) {
				auto [x, y] = corners[i][0];
				x_to_idx[x] = i;
				seg[S+y] = x;
			}
			for (int a = S-1; a > 0; a--) seg[a] = std::min(seg[2*a], seg[2*a+1]);
			std::vector<int> perm; perm.reserve(N);
			std::vector<int> stk; stk.reserve(N);
			while (int(perm.size()) < N) {
				int x, y;
				int a;
				if (stk.empty()) {
					x = 2*N, y = 2*N;
					a = 1;
					assert(seg[a] < x);
				} else {
					x = corners[stk.back()][1][0];
					y = corners[stk.back()][1][1];
					// look for the first thing < y which is < x
					a = 2*S;
					for (int pa = S, pb = S+y; pa < pb; pa >>= 1, pb >>= 1) {
						assert(pa & 1);
						if (pb & 1) {
							--pb;
							if (seg[pb] < x) a = pb;
						}
					}
					if (a == 2*S) {
						perm.push_back(stk.back());
						stk.pop_back();
					}
				}
				assert(seg[a] < x);
				while (a < S) {
					if (seg[2*a+0] < x) a = 2*a+0;
					else if (seg[2*a+1] < x) a = 2*a+1;
				}
				int nx = seg[a];
				int ni = x_to_idx[nx];
				seg[a] = 2*N;
				for (int v = a/2; v > 0; v /= 2) seg[v] = std::min(seg[2*v+0], seg[2*v+1]);
				stk.push_back(ni);
			}
			for (int i = 0; i < N; i++) cout << perm[i]+1 << " \n"[i+1==N];
		} else if (M == 2) {
			std::string ans(N, '?');
			std::vector<int> bit(2*N, 2*N);
			// M = 2: just go backwards
			for (int i = N-1; i >= 0; i--) {
				// check if box i can be removed
				{
					auto [qx, qy] = corners[i][1];
					// check if something < qx is < qy
					bool good = true;
					for (int b = qx; b > 0; b &= (b-1)) {
						if (bit[b-1] < qy) good = false;
					}
					ans[i] = good ? '1' : '0';
				}
				{
					auto [qx, qy] = corners[i][0];
					for (int b = qx; b < 2*N; b |= b+1) {
						bit[b] = std::min(bit[b], qy);
					}
				}
			}
			cout << ans << '\n';
		} else if (M == 3) {
			cout << "I'm a human!" << '\n';
			assert(false);
		} else assert(false);
	}

	return 0;
}

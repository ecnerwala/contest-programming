#include <bits/stdc++.h>

struct seg_t {
	int v;
	int l;
	int t;
	int t_en;
	// reversed so that pq works
	friend bool operator < (const seg_t& a, const seg_t& b) { return a.v > b.v; }
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;
		// loc should range from 0..3*(N-1) inclusive
		std::vector<int> A(N*N);
		std::vector<int> loc(N*N);
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				int v; cin >> v; v--;
				A[N*i+j] = v;
				loc[v] = (i == 0 ? i - j : i + j) + (N-1);
			}
		}
		int num_queries = 0;
		auto query = [&](int l, int t) -> int {
			assert(1 <= l && l <= N);
			assert(0 <= t && t <= 2*(N-1));
			num_queries++;
			assert(num_queries <= 120 * N + M);
			cout << '?' << ' ' << l << ' ' << t+1 << '\n' << flush;
			int res; cin >> res; res--;
			return res;
		};
		std::priority_queue<seg_t> segs;
		for (int l = 1; l <= N; l++) {
			// snake i
			int o_hi = N;
			int o_lo = N - l;
			// t goes from 0..2*N-2
			int v_0 = *std::max_element(A.begin(), A.begin() + l);
			int t_0 = 0;
			while (t_0 < 2*N-2) {
				segs.push({v_0, l, t_0, t_0 + 1});

				int t_1 = std::min(t_0 + l, 2*N-2);
				int v_1 = query(l, t_1);
				assert(t_0 + o_hi >= t_1 + o_lo);
				{
					int mi = t_0, ma = t_1;
					while (ma - mi > 1) {
						int md = (mi + ma) / 2;
						int v_md = query(l, md);
						if (loc[v_md] < t_0 + o_hi) {
							// we're in the first half, so the beginning is monotonic
							segs.push({v_md, l, md, mi});
							mi = md;
						} else {
							// we're in the second half
							segs.push({v_md, l, md, ma});
							ma = md;
						}
					}
				}
				t_0 = t_1;
				v_0 = v_1;
			}
			// segment of length 1
			segs.push({v_0, l, t_0, t_0 + 1});
		}
		std::vector<int> ans; ans.reserve(M);
		for (int z = 0; z < M; z++) {
			auto seg = segs.top(); segs.pop();
			assert(seg.t != seg.t_en);
			ans.push_back(seg.v);
			cerr << seg.l << ' ' << seg.t << ' ' << seg.v << '\n';
			seg.t += (seg.t_en > seg.t ? +1 : -1);
			if (seg.t != seg.t_en) {
				int nv = query(seg.l, seg.t);
				assert(nv >= seg.v);
				seg.v = nv;
				segs.push(seg);
			}
		}
		assert(int(ans.size()) == M);
		cout << '!';
		for (auto v : ans) cout << ' ' << v+1;
		cout << '\n' << flush;
	}

	return 0;
}

#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<int> W(N);
	for (auto& w : W) cin >> w;
	std::vector<int> C(N-1);
	for (auto& c : C) cin >> c;

	struct seg_t {
		int64_t slope;
		int64_t len;
	};
	std::vector<seg_t> segs(2*N+1);
	int lo = N, hi = N;
	int64_t v_lo = int64_t(W[0]) * int64_t(C[0]);
	int64_t v_hi = v_lo;
	int64_t tot_len = W[0];
	int64_t extra_slope = 0;
	segs[N] = {0, W[0]};

	for (int i = 0; i < N-1; i++) {
		// transition to costs on C[i]
		int64_t c = C[i];
		{
			// transition from c-1 -> c across W[i]
			assert(tot_len == W[i]);
			if (i == 0) {
				// initialized
			} else if (i & 1) {
				extra_slope -= c;
				v_lo += tot_len * c;
				// flatten suffix
				int64_t new_0_len = 0;
				while (hi >= lo && segs[hi].slope + extra_slope >= 0) {
					new_0_len += segs[hi].len;
					v_hi -= segs[hi].len * (segs[hi].slope + extra_slope);
					--hi;
				}
				if (new_0_len > 0) {
					segs[++hi] = {-extra_slope, new_0_len};
				}
			} else {
				extra_slope += c;
				v_hi += tot_len * c;
				// flatten prefix
				int64_t new_0_len = 0;
				while (lo <= hi && segs[lo].slope + extra_slope <= 0) {
					new_0_len += segs[lo].len;
					v_lo += segs[lo].len * (segs[lo].slope + extra_slope);
					++lo;
				}
				if (new_0_len > 0) {
					segs[--lo] = {-extra_slope, new_0_len};
				}
			}
		}
		{
			// extend to W[i+1]
			int64_t w = W[i+1];
			if (w > tot_len) {
				// extend
				if (i & 1) {
					int64_t l = w - tot_len;
					segs[--lo] = {-c - extra_slope, l};
					v_lo += c * l;
					tot_len += l;
				} else {
					int64_t l = w - tot_len;
					segs[++hi] = {c - extra_slope, l};
					v_hi += c * l;
					tot_len += l;
				}
			} else {
				while (tot_len > w) {
					assert(lo <= hi);
					// shrink
					if (i & 1) {
						int64_t delta = std::min(segs[lo].len, tot_len - w);
						v_lo += delta * (segs[lo].slope + extra_slope);
						tot_len -= delta;
						segs[lo].len -= delta;
						if (segs[lo].len == 0) ++lo;
					} else {
						int64_t delta = std::min(segs[hi].len, tot_len - w);
						v_hi -= delta * (segs[hi].slope + extra_slope);
						tot_len -= delta;
						segs[hi].len -= delta;
						if (segs[hi].len == 0) --hi;
					}
				}
			}
		}
		cout << (i & 1 ? v_lo : v_hi) << '\n';
	}

	return 0;
}

// we want to pick values k_i so that sum c_i k_i minimized, and
// k_i + k_{i-1} >= w_i
// and k_i >= 0
//
// The dual: pick a_i >= 0 so that sum a_i w_i maximized, where a_i + a_{i+1} <= c_i
//
// It's pretty much self dual?

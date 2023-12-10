#include<bits/stdc++.h>

// Floor of log_2(a); index of highest 1-bit
inline int log_2(int a) {
	return a ? (8 * sizeof(a)) - 1 - __builtin_clz(a) : -1;
}

inline int next_pow_2(int a) {
	assert(a > 0);
	return 1 << log_2(2*a-1);
}

struct prng_t {
	int x, y, z;
	friend std::istream& operator >> (std::istream& in, prng_t& a) {
		return in >> a.x >> a.y >> a.z;
	}
	int p = 0;
	int step() {
		return p = int((int64_t(p) * int64_t(x) + y) % z);
	}
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		int N; cin >> N;
		prng_t rng_a, rng_b, rng_c;
		cin >> rng_a >> rng_b >> rng_c;
		std::vector<int> A(N+1);
		std::vector<int> B(N+1);
		std::vector<int> C(N+1);
		for (int i = 1; i <= N; i++) {
			A[i] = std::min(i, 1 + rng_a.step());
			B[i] = std::max(A[i], i - rng_b.step());
			C[i] = std::min(i-1, rng_c.step());
			assert(1 <= A[i] && A[i] <= B[i] && B[i] <= i);
			assert(0 <= C[i] && C[i] < i);
		}

		cerr << "Case #" << case_num << ": " << "N = " << N << '\n';
		/*
		cerr << '\n';
		for (int i = 1; i <= N; i++) {
			cerr << i << ' ' << A[i] << ' ' << B[i] << ' ' << C[i] << '\n';
		}
		cerr << '\n';
		*/

		int S = next_pow_2(N+1);
		int L = log_2(S);
		struct seg_t {
			std::array<int, 2> c;
			int v;
		};

		std::vector<seg_t> seg; seg.reserve(1 + (N+1) * (L+1));
		seg.push_back({{0, 0}, -1});

		std::vector<int> roots_seg(N+2, -1);
		roots_seg[0] = 0;

		int64_t ans = 0;
		std::vector<int> nimber(N+1);
		std::vector<int> first_val(N+1, -1);
		std::vector<int> stk(L+1);
		for (int i = 0; i <= N; i++) {
			if (i == 0) {
				nimber[i] = 0;
			} else {
				int t_lo = i - B[i], t_hi = i - A[i];
				int c = nimber[C[i]];

				int a = roots_seg[t_hi+1];
				for (int b = L-1; b >= 0; b--) {
					assert(seg[a].v < t_lo);
					int d = ((c >> b) & 1);
					if (seg[seg[a].c[d]].v < t_lo) {
						a = seg[a].c[d];
					} else {
						nimber[i] += (1 << b);
						a = seg[a].c[!d];
					}
					assert(seg[a].v < t_lo);
				}
			}
			// insert the guy
			assert(nimber[i] < S);
			if (i > 0) {
				if (first_val[nimber[i]] == -1) first_val[nimber[i]] = i;
				ans += first_val[nimber[i]];
			}
			roots_seg[i+1] = roots_seg[i];
			stk[L] = roots_seg[i];
			for (int b = L-1; b >= 0; b--) {
				stk[b] = seg[stk[b+1]].c[(nimber[i] >> b) & 1];
			}
			seg.push_back({{0, 0}, i});
			stk[0] = int(seg.size()) - 1;
			for (int b = 0; b < L; b++) {
				seg_t s = seg[stk[b+1]];
				s.c[(nimber[i] >> b) & 1] = stk[b];
				s.v = std::min(seg[s.c[0]].v, seg[s.c[1]].v);
				seg.push_back(s);
				stk[b+1] = int(seg.size()) - 1;
			}
			roots_seg[i+1] = stk[L];
			//cerr << "run " << i << ' ' << i-B[i] << '-' << i-A[i] << ' ' << C[i] << '(' << nimber[C[i]] << ')' << '\n';
			//cerr << "nimber " << i << ' ' << nimber[i] << ' ' << first_val[nimber[i]] << '\n';
		}

		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}

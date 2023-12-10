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
			int v;
			int l_sz;
		};

		// empty means v = -1
		std::vector<std::vector<seg_t>> seg(2*S);

		int64_t ans = 0;
		std::vector<int> nimber(N+1);
		std::vector<int> first_val(N+1, -1);
		for (int i = 0; i <= N; i++) {
			if (i == 0) {
				nimber[i] = 0;
			} else {
				int t_lo = i - B[i], t_hi = i - A[i];
				int c = nimber[C[i]];

				int a = 1;
				int sz = t_hi + 1;
				for (int b = L-1; b >= 0; b--) {
					assert(sz == 0 || seg[a][sz-1].v < t_lo);
					std::array<int, 2> n_sz;
					n_sz[0] = sz ? seg[a][sz-1].l_sz : 0;
					n_sz[1] = sz - n_sz[0];
					int d = ((c >> b) & 1);
					if (n_sz[d] == 0 || seg[2*a+d][n_sz[d]-1].v < t_lo) {
						// go left
						sz = n_sz[d];
						a = 2*a+d;
					} else {
						// go right
						nimber[i] += (1 << b);
						sz = n_sz[!d];
						a = 2*a+!d;
					}
					assert(sz == 0 || seg[a][sz-1].v < t_lo);
				}
			}
			// insert the guy
			assert(nimber[i] < S);
			if (i > 0) {
				if (first_val[nimber[i]] == -1) first_val[nimber[i]] = i;
				ans += first_val[nimber[i]];
			}
			//cerr << "run " << i << ' ' << i-B[i] << '-' << i-A[i] << ' ' << C[i] << '(' << nimber[C[i]] << ')' << '\n';
			//cerr << "nimber " << i << ' ' << nimber[i] << ' ' << first_val[nimber[i]] << '\n';
			seg[S + nimber[i]].push_back(seg_t{i, 0});
			for (int a = (S + nimber[i])/2; a >= 1; a /= 2) {
				seg[a].push_back(seg_t{
					std::min(
						seg[2*a].empty() ? -1 : seg[2*a].back().v,
						seg[2*a+1].empty() ? -1 : seg[2*a+1].back().v
					),
					int(seg[2*a].size())
				});
			}
		}

		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

using ll = long long;

const int S = 1 << 18;
struct {
	int sum;
	int suff;
} seg[2 * S];
void update(int i) {
	if (i >= S) {
		seg[i].suff = max(seg[i].sum, 0);
	} else {
		seg[i].sum = seg[2*i].sum + seg[2*i+1].sum;
		seg[i].suff = max(seg[2*i].suff + seg[2*i+1].sum, seg[2*i+1].suff);
	}
}
void reset() {
	memset(seg, 0, sizeof(seg));
}
void set_point(int i, int val) {
	seg[i+S].sum = val;
	for (int a = i + S; a; a /= 2) update(a);
}

class DejaVu {

public:


	int solve(int N, vector<ll> M) {
		int ans = 0;
		reset();
		unordered_map<ll, vector<int>> locs;
		for (int i = 0; i < N; i++) {
			ll v = M[i];
			locs[v].push_back(i);
			const int vals[] = {0, +1, -1, 0};
			for (int z = 0; z < 4 && z < locs[v].size(); z++) {
				set_point(locs[v][int(locs[v].size()) - 1 - z], vals[z]);
			}
			ans = max(ans, seg[1].suff);
		}
		return ans;
	}

	int mostDejaVus(int N, int seed, int R) {
		vector<ll> A(N);
		A[0] = seed;
		for (int i = 1; i < N; i++) {
			A[i] = (A[i-1] * ll(1664525) + ll(1013904223)) % ll(4294967296);
		}
		vector<ll> M(N);
		for (int i = 0; i < N; i++) {
			M[i] = A[i] % R;
		}
		return solve(N, M);
	}
};

}  // namespace ecnerwala

using ecnerwala::DejaVu;

// vim:ft=cpp

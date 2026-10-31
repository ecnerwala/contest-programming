#include<bits/stdc++.h>

template <typename T> void setmin(T& a, const T& b) { if (b < a) a = b; }

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int K, N, M, Q; cin >> K >> N >> M >> Q;
	assert(K <= 5);

	const int INF = 1e9;
	using mat = array<int, 25>;
	auto mul = [&](const mat& a, const mat& b) -> mat {
		mat r; r.fill(INF);
		for (int i = 0; i < 5; i++) {
			for (int k = 0; k < 5; k++) {
				for (int j = 0; j < 5; j++) {
					setmin(r[i*5+j], a[i*5+k]+b[k*5+j]);
				}
			}
		}
		return r;
	};

	int S = (N-1)/K;
	vector<mat> seg(2*S);
	for (auto& a : seg) a.fill(INF);

	for (int e = 0; e < M; e++) {
		int a, b, t; cin >> a >> b >> t;
		assert(b/K - a/K == 1);
		setmin(seg[S+(a/K)][(a%K)*5 + (b%K)], t);
	}
	for (int i = S-1; i; --i) {
		seg[i] = mul(seg[2*i], seg[2*i+1]);
	}

	for (int q = 0; q < Q; q++) {
		int x, y; cin >> x >> y;
		assert(x != y);

		array<int, 5> larr; larr.fill(INF);
		larr[x%K] = 0;
		array<int, 5> rarr; rarr.fill(INF);
		rarr[y%K] = 0;

		for (int a = S+(x/K), b = S+(y/K); a < b; a >>= 1, b >>= 1) {
			if (a & 1) {
				array<int, 5> narr; narr.fill(INF);
				for (int i = 0; i < 5; i++) {
					for (int j = 0; j < 5; j++) {
						setmin(narr[j], larr[i] + seg[a][i*5+j]);
					}
				}
				larr = narr;
				a++;
			}
			if (b & 1) {
				--b;
				array<int, 5> narr; narr.fill(INF);
				for (int i = 0; i < 5; i++) {
					for (int j = 0; j < 5; j++) {
						setmin(narr[i], seg[b][i*5+j] + rarr[j]);
					}
				}
				rarr = narr;
			}
		}

		int ans = INF;
		for (int i = 0; i < 5; i++) setmin(ans, larr[i] + rarr[i]);
		if (ans == INF) cout << -1 << '\n';
		else cout << ans << '\n';
	}

	return 0;
}

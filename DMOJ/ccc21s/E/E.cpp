#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, Q; cin >> N >> Q;
	struct req_t {
		int l, r;
		int v;
	};
	vector<req_t> reqs(Q);
	vector<int> seg(2*N, 1);
	for (auto& r : reqs) {
		cin >> r.l >> r.r >> r.v;
		r.l--;
		for (int a = N+r.l, b = N+r.r; a < b; a >>= 1, b >>= 1) {
			if (a & 1) {
				seg[a] = std::lcm(seg[a], r.v);
				a++;
			}
			if (b & 1) {
				--b;
				seg[b] = std::lcm(seg[b], r.v);
			}
		}
	}
	for (int a = 2; a < 2*N; a++) {
		seg[a] = std::lcm(seg[a], seg[a>>1]);
	}
	for (int a = N-1; a; a--) {
		seg[a] = std::gcd(seg[2*a], seg[2*a+1]);
	}
	for (auto r : reqs) {
		int g = 0;
		for (int a = N+r.l, b = N+r.r; a < b; a >>= 1, b >>= 1) {
			if (a & 1) g = std::gcd(g, seg[a++]);
			if (b & 1) g = std::gcd(g, seg[--b]);
		}
		if (g != r.v) {
			cout << "Impossible" << '\n';
			exit(0);
		}
	}
	for (int i = 0; i < N; i++) {
		cout << seg[N+i] << " \n"[i+1==N];
	}


	return 0;
}

#include <bits/stdc++.h>

const int64_t INF = 1e18;
struct func_t {
	int64_t add = 0, val = -INF;
	func_t() {}
	func_t(int64_t a, int64_t v) : add(a), val(v) {}

	friend func_t operator + (const func_t& a, const func_t& b) {
		return func_t(a.add + b.add, std::max(a.val + b.add, b.val));
	}
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	for (int N, Q; cin >> N >> Q; ) {
		vector<func_t> seg(2*N);
		for (int i = 0; i < N; i++) {
			cin >> seg[N+i].val;
		}
		for (int i = 0; i < N; i++) {
			cin >> seg[N+i].add;
		}
		for (int a = N-1; a; a--) {
			seg[a] = seg[2*a] + seg[2*a+1];
		}

		for (int q = 0; q < Q; q++) {
			int op; cin >> op;
			if (op == 1) {
				int x, y; cin >> x >> y; x--;
				seg[N+x].val = y;
				for (int a = (N+x) >> 1; a; a >>= 1) {
					seg[a] = seg[2*a] + seg[2*a+1];
				}
			} else if (op == 2) {
				int x, y; cin >> x >> y; x--;
				seg[N+x].add = y;
				for (int a = (N+x) >> 1; a; a >>= 1) {
					seg[a] = seg[2*a] + seg[2*a+1];
				}
			} else if (op == 3) {
				int x; cin >> x;
				func_t fl, fr;
				for (int a = N, b = N+x; a < b; a /= 2, b /= 2) {
					if (a & 1) fl = fl + seg[a++];
					if (b & 1) fr = seg[--b] + fr;
				}
				func_t f = fl + fr;
				cout << max(f.add + 0, f.val) << '\n';
			} else assert(false);
		}
	}

	return 0;
}

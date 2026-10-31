#include <bits/stdc++.h>

struct op_t {
	bool flip = false;
	std::array<int, 2> val{0,0};
	op_t() {}
	explicit op_t(int a) {
		flip = true;
		val[0] = a;
		val[1] = 0;
	}
	op_t(int a, int b) {
		int l = (a + b - 1) / b;
		flip = (l & 1);
		val[0] = (l-1) / 2 * b;
		val[1] = (l-1) / 2 * b;
		a -= (l-1) / 2 * b * 2;
		assert(0 <= a && a <= 2 * b);
		if (a <= b) {
			val[0] += a;
		} else {
			val[0] += b;
			val[1] += a - b;
		}
	}

	op_t(op_t l, op_t r) {
		flip = l.flip ^ r.flip;
		val[0] = l.val[0] + r.val[l.flip];
		val[1] = l.val[1] + r.val[!l.flip];
	}
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N);
		for (auto& a : A) cin >> a;
		int V = *std::max_element(A.begin(), A.end());
		std::vector<op_t> vals(V+1);
		std::vector<op_t> lazy(V+1);
		for (int a : A) {
			op_t cur_lazy;
			for (int b = 1; b <= a; b++) {
				vals[b] = op_t(vals[b], cur_lazy);
				vals[b] = op_t(vals[b], op_t(a, b));
				cur_lazy = op_t(lazy[b], cur_lazy);
				lazy[b] = op_t();
			}
			lazy[a] = op_t(cur_lazy, op_t(a));
		}

		op_t cur_lazy;
		for (int b = 1; b <= V; b++) {
			vals[b] = op_t(vals[b], cur_lazy);
			cout << vals[b].val[0] << '\n';

			cur_lazy = op_t(lazy[b], cur_lazy);
			lazy[b] = op_t();
		}
	}

	return 0;
}

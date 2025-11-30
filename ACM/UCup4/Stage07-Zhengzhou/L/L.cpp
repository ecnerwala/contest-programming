#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, Q; cin >> N >> Q;
	struct op_t {
		int op;
		std::array<int, 3> xs;
		uint32_t w;
	};
	std::vector<op_t> ops(Q);
	int num_out = 0;
	for (auto& q : ops) {
		cin >> q.op;
		assert(q.op == 1 || q.op == 2);
		int type, x, y, d; cin >> type >> x >> y >> d;
		x--, y--;
		assert(0 <= y && y <= x && x <= N);
		uint32_t w;
		if (q.op == 1) { cin >> w; }
		else if (q.op == 2) { w = num_out++; }
		else assert(false);

		if (type == 0) {
		} else if (type == 1) {
			d = -d;
		} else assert(false);

		std::array<int, 3> coords;
		coords[0] = (N-x) - d;
		coords[1] = y;
		coords[2] = x - y;
		q.xs = coords;
		q.w = w;
		// area of a positive region is (N - coords[0] - coords[1] - coords[2])^2
		if (type == 0) assert(coords[0] + coords[1] + coords[2] < N);
		else if (type == 1) assert(coords[0] + coords[1] + coords[2] > N);
	}

	std::vector<uint32_t> ans(num_out);
	for (int m1 = 0; m1 < 7; m1++) {
		for (int m2 = 0; m2 < 7; m2++) {
			bool neg = (__builtin_popcount(m1) + __builtin_popcount(m2)) & 1;
			std::vector<std::pair<std::array<int, 3>, uint32_t>> coords;
			for (const auto& op : ops) {
				if (op.op == 1) {
					// update
					coords.push_back({op.xs, op.w});
					for (int z = 0; z < 3; z++) if (!((m1 >> z) & 1)) coords.back().first[z] = 0;
				} else if (op.op == 2) {
					int q = op.w;
					// query
					for (auto [a, w] : coords) {
						for (int z = 0; z < 3; z++) {
							if (!((m2 >> z) & 1)) {
								a[z] = std::max(a[z], op.xs[z]);
							}
						}
						if (a[0] + a[1] + a[2] <= N) {
							uint32_t v = N - a[0] - a[1] - a[2];
							v *= v;
							v *= w;
							ans[q] += neg ? -v : v;
						}
					}
				} else assert(false);
			}
		}
	}

	for (auto v : ans) cout << v << '\n';

	return 0;
}

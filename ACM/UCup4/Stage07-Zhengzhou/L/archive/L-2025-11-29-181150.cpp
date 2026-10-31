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
	for (auto& q : ops) {
		cin >> q.op;
		assert(q.op == 1 || q.op == 2);
		int type, x, y, d; cin >> type >> x >> y >> d;
		x--, y--;
		assert(0 <= y && y <= x && x <= N);
		uint32_t w;
		if (q.op == 1) { cin >> w; }
		else if (q.op == 2) {}
		else assert(false);

		if (type == 0) {
		} else if (type == 1) {
			w = -w;
			d = -d;
		} else assert(false);

		std::array<int, 3> coords;
		coords[0] = (N-x) - d;
		coords[1] = y;
		coords[2] = x - y;
		q.xs = coords;
		q.w = w;
	}

	return 0;
}

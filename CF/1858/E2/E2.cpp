#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int Q; cin >> Q;

	int L = 0;
	std::vector<int> vals(Q, 0);
	std::vector<int> sizes(Q+1);
	sizes[0] = 0;

	const int MAXV = int(1.01e6);
	std::vector<int> occs(MAXV, -1);

	std::vector<std::pair<int*, int>> updates;
	updates.reserve(5*Q);
	auto set = [&](int& a, int b) {
		updates.push_back({&a, a});
		a = b;
	};
	auto checkpoint = [&]() -> int {
		return int(updates.size());
	};

	std::vector<int> ops; ops.reserve(Q);
	for (int q = 0; q < Q; q++) {
		char op; cin >> op;
		if (op == '+') {
			ops.push_back(checkpoint());

			int a; cin >> a;
			if (occs[vals[L]] == L) {
				set(occs[vals[L]], -1);
			}
			bool is_new = (occs[a] == -1 || occs[a] >= L);
			if (is_new) {
				set(vals[L], a);
				set(occs[a], L);
				set(L, L+1);
				set(sizes[L], sizes[L-1] + 1);
			} else {
				set(vals[L], 0);
				set(L, L+1);
				set(sizes[L], sizes[L-1]);
			}
		} else if (op == '-') {
			ops.push_back(checkpoint());
			int k; cin >> k;
			set(L, L-k);
		} else if (op == '!') {
			int cp = ops.back(); ops.pop_back();
			while (int(updates.size()) > cp) {
				auto [a, v] = updates.back();
				*a = v;
				updates.pop_back();
			}
		} else if (op == '?') {
			cout << sizes[L] << '\n' << flush;
		} else assert(false);
	}

	return 0;
}

#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	struct op_t {
		int delta;
		int weight;
		int extra_val;
		int id;
	};
	auto solve = [&](std::vector<op_t> ops, int64_t base_val) -> std::vector<int> {
		for (auto& op : ops) {
			base_val += op.delta;
		}
		std::vector<std::pair<int64_t, int>> op_ids; op_ids.reserve(ops.size());
		auto check = [&](int64_t v) -> bool {
			op_ids.clear();
			for (int i = 0; i < int(ops.size()); i++) {
				auto op = ops[i];
				int64_t cap = v / int64_t(op.weight) - op.extra_val;
				op_ids.push_back({cap, i});
			}
			std::sort(op_ids.begin(), op_ids.end(), std::greater<>());
			int64_t cur_val = base_val;
			for (auto [cap, i] : op_ids) {
				if (cur_val > cap) return false;
				auto op = ops[i];
				cur_val -= op.delta;
			}
			return true;
		};
		int64_t mi = -1;
		int64_t ma = 0;
		for (auto op : ops) {
			ma = std::max(ma, (base_val + op.extra_val) * int64_t(op.weight));
		}
		while (ma - mi > 1) {
			int64_t md = (mi + ma) / 2;
			if (check(md)) ma = md;
			else mi = md;
		}
		check(ma);
		std::vector<int> res; res.reserve(ops.size());
		for (auto [_, i] : op_ids) {
			res.push_back(ops[i].id);
		}
		return res;
	};

	int N; cin >> N;
	int64_t base_val = 0;
	std::vector<op_t> pref; pref.reserve(N);
	std::vector<op_t> suff; suff.reserve(N);
	for (int i = 0; i < N; i++) {
		int a, b, c; cin >> a >> b >> c;
		if (a >= b) {
			suff.push_back(op_t{a - b, c, b, i});
			base_val += b;
		} else {
			pref.push_back(op_t{b - a, c, a, i});
			base_val += a;
		}
	}

	auto ans0 = solve(pref, base_val);
	auto ans1 = solve(suff, base_val);

	std::vector<int> ans; ans.reserve(N);
	ans.insert(ans.end(), ans0.begin(), ans0.end());
	ans.insert(ans.end(), ans1.rbegin(), ans1.rend());
	for (int i = 0; i < N; i++) {
		cout << ans[i]+1 << " \n"[i+1==N];
	}

	return 0;
}

#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	int X; cin >> X;
	struct op_t {
		int A, B;
	};
	std::vector<op_t> ops; ops.reserve(N);
	int num_extra = 0;
	for (int i = 0; i < N; i++) {
		int a, b; cin >> a >> b;
		if (a == 1) {
			X -= b;
			num_extra += b;
			continue;
		}
		ops.push_back({a, b});
	}
	N = int(ops.size());

	if (N == 0) {
		cout << 1 << ' ' << num_extra << '\n';
		exit(0);
	}

	std::vector<op_t> cur_ops; cur_ops.reserve(N);

	std::vector<std::vector<std::pair<int, int>>> transitions(N);

	for (int i = 0; i < N; i++) {
		cur_ops.clear();
		for (int j = i; j < N; j++) {
			cur_ops.push_back(ops[j]);
			std::sort(cur_ops.begin(), cur_ops.end(), [&](auto x, auto y) {
				return int64_t(y.A - 1) * x.B < int64_t(x.A - 1) * y.B;
			});
			int64_t x = 0;
			for (auto op : cur_ops) {
				x *= op.A;
				x += op.B;
				if (x > X) break;
			}
			if (x > X) {
				break;
			}
			transitions[i].push_back({j+1, int(x)});
			//cerr << i << ' ' << j+1 << ' ' << x << '\n';
		}
	}

	auto get_val = [&](int64_t lambda) -> std::pair<int64_t, int> {
		// each day costs lambda
		const int64_t INF = 1e18;
		std::vector<std::pair<int64_t, int>> dp(N+1, {INF, N+1});
		dp[0] = {0, 0};
		for (int i = 0; i < N; i++) {
			for (auto [j, v] : transitions[i]) {
				dp[j] = std::min(dp[j], {dp[i].first + v + lambda, dp[i].second + 1});
			}
		}
		//cerr << "solve " << lambda << '\n';
		//for (int i = 0; i <= N; i++) { cerr << i << ' ' << dp[i].first - dp[i].second * lambda << ' ' << dp[i].first << ' ' << dp[i].second << '\n'; }
		return {dp[N].first - dp[N].second * lambda, dp[N].second};
	};

	int64_t mi = 0;
	auto v_mi = get_val(mi);
	assert(v_mi.first <= X);
	int64_t ma = X+1;
	auto v_ma = get_val(ma);
	if (v_ma.first <= X) {
		cout << v_ma.second << ' ' << v_ma.first + num_extra << '\n';
		exit(0);
	}
	assert(v_ma.first > X);

	while (ma - mi > 1) {
		int64_t md = (mi + ma) / 2;
		auto v_md = get_val(md);
		if (v_md.first <= X) {
			mi = md;
			v_mi = v_md;
		} else {
			ma = md;
			v_ma = v_md;
		}
	}

	assert((v_ma.first - v_mi.first) == int64_t(v_mi.second - v_ma.second) * ma);
	int64_t num_steps = (v_ma.first - X + ma - 1) / ma;
	//cerr << v_ma.first << ' ' << v_ma.second << ' ' << ma << '\n';
	//cerr << v_mi.first << ' ' << v_mi.second << ' ' << mi << '\n';

	cout << v_ma.second + num_steps << ' ' << v_ma.first - ma * num_steps + num_extra << '\n';

	return 0;
}

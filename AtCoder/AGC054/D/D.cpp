#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	string S; cin >> S;
	int N = int(S.size());
	vector<pair<bool, int>> paren_cost; paren_cost.reserve(N);
	vector<bool> is_x; is_x.reserve(N);
	int base_cost = 0;
	{
		vector<int> open_cost; open_cost.reserve(N);
		vector<int> closed_cost; closed_cost.reserve(N);
		int idx = 0;
		for (int i = 0; i < N; i++) {
			if (S[i] == '(') {
				open_cost.push_back(int(is_x.size()));
				base_cost += max(0, int(closed_cost.size()) - idx);
				paren_cost.push_back({false, open_cost.back()});
			} else if (S[i] == ')') {
				closed_cost.push_back(int(is_x.size()));
			} else if (S[i] == 'o' || S[i] == 'x'){
				is_x.push_back(S[i] == 'x');
			} else assert(false);

			for (; idx < int(closed_cost.size()) && idx < int(open_cost.size()); idx++) {
				paren_cost.push_back({true, closed_cost[idx]});
			}
		}
		assert(int(closed_cost.size()) == int(open_cost.size()));
		assert(idx == int(closed_cost.size()));
	}

	//cerr << base_cost << '\n';

	int P = int(paren_cost.size());
	int X = int(is_x.size());
	const int INF = 1e9;

	vector<int> dp(X+1, INF);
	dp[0] = base_cost;
	int cur_sum = 0;

	auto relax_ox = [&]() -> void {
		for (int x = 0; x < X; x++) {
			if (cur_sum || !is_x[x]) {
				dp[x+1] = min(dp[x+1], dp[x]);
			}
		}
	};

	relax_ox();
	for (int i = 0; i < P; i++) {
		auto [is_close, cost] = paren_cost[i];
		for (int x = 0; x <= X; x++) {
			dp[x] += abs(cost - x);
		}
		if (is_close) cur_sum--;
		else cur_sum++;
		assert(cur_sum >= 0);
		relax_ox();
	}

	cout << dp[X] << '\n';

	return 0;
}

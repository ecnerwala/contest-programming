#include <bits/stdc++.h>

std::vector<int> btoa;
std::vector<char> vis;
std::vector<char> used;

int64_t N;
int K;

bool dfs(int cur) {
	if (vis[cur]) return false;
	vis[cur] = true;

	// First, look ahead by 1
	for (int64_t tgt = N % (cur+1); tgt < K; tgt += cur+1) {
		if (btoa[tgt] == -1) {
			btoa[tgt] = cur;
			return true;
		}
	}

	for (int64_t tgt = N % (cur+1); tgt < K; tgt += cur+1) {
		int nxt = std::exchange(btoa[tgt], cur);
		if (dfs(nxt)) {
			return true;
		}
		btoa[tgt] = nxt;
	}

	return false;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	cin >> N;
	int M = int(min<int64_t>(N/2, 4000));

	btoa.resize(M, -1);
	vis.resize(M);
	used.resize(M);

	vector<int> ans; ans.reserve(M+1);
	ans.push_back(1);

	int cur_matching = 0;
	for (K = 1; K <= M; K++) {
		for (int i = K-1; i >= 0; i--) {
			if (used[i]) continue;
			if (dfs(i)) {
				used[i] = true;
				for (int j = 0; j < K; j++) vis[j] = false;
				cur_matching++;
			}
		}
		ans.push_back(cur_matching == K);
	}

	vector<int> rev_ans = ans;
	if (M*2 == N) rev_ans.pop_back();
	reverse(rev_ans.begin(), rev_ans.end());
	int64_t num_mid = N+1 - (int(ans.size()) + int(rev_ans.size()));
	vector<vector<int>> ops;
	for (int v = 2; num_mid > 0; num_mid >>= 1, v++) {
		if (v == 2) {
			ops.push_back(vector<int>({0}));
		} else {
			ops.push_back(vector<int>({v-1, v-1}));
		}
		if (num_mid & 1) {
			ans.push_back(v);
		}
	}

	ans.insert(ans.end(), rev_ans.begin(), rev_ans.end());
	ops.push_back(ans);

	cout << ops.size()+1 << '\n';
	for (const auto& op : ops) {
		cout << op.size();
		for (int v : op) cout << ' ' << v;
		cout << '\n';
	}

	return 0;
}

#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, S; cin >> N >> S; S--;
	int64_t tot = 0;
	vector<int> dir(N-1);

	int cur_pos = S;
	int flip_cost = 0;
	int flip_idx = -1;
	for (int i = 0; i < N-1; i++) {
		array<int, 2> C; cin >> C[0] >> C[1];
		dir[i] = C[1] < C[0];
		tot += C[dir[i]];

		if (i > 0 && dir[i] != dir[i-1]) {
			cur_pos = -1;
		}

		if (cur_pos != -1) {
			int cnd = C[!dir[i]] - C[dir[i]];
			if (flip_idx == -1 || cnd < flip_cost) {
				flip_cost = cnd;
				flip_idx = i;
			}
			cur_pos += (dir[i] ? +1 : -1);
			if (cur_pos == -1 || cur_pos == N) {
				dir[flip_idx] = !dir[flip_idx];
				tot += flip_cost;
				cur_pos = -1;
			}
		}
	}

	int run_len = 1;
	while (run_len < N-1 && dir[run_len] == dir[0]) {
		dir[run_len] ^= 1;
		run_len++;
	}

	vector<int> dq; dq.reserve(N - 1);
	for (int i = 0; i < N; i++) {
		if (i != S) dq.push_back(i);
	}
	int lo = 0, hi = N-1;
	vector<int> ans(N);
	ans[0] = S;
	for (int i = 1; i < N-1; i++) {
		if (dir[i]) ans[i] = dq[lo++];
		else ans[i] = dq[--hi];
	}
	ans[N-1] = dq[lo];
	std::reverse(ans.begin()+1, ans.begin()+run_len+1);

	cout << tot << '\n';
	for (int i = 0; i < N; i++) {
		cout << ans[i]+1 << " \n"[i+1==N];
	}

	return 0;
}

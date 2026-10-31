#include<bits/stdc++.h>
using namespace std;

const int MAXN = 1.1e4;
int N;
const int MAXV = MAXN * 60;
int V;
int ch[MAXV][26];
int cnt[MAXV];

int get_new_node() {
	int ind = V++;
	cnt[ind] = 0;
	memset(ch[ind], -1, sizeof(ch[ind]));
	return ind;
}

int ans;
int dfs(int cur, bool isRoot) {
	if (cur == -1) return 0;
	for (int i = 0; i < 26; i++) {
		cnt[cur] += dfs(ch[cur][i], false);
	}
	if (!isRoot) {
		if (cnt[cur] >= 2) {
			cnt[cur] -= 2;
			ans += 2;
		}
	}
	return cnt[cur];
}

int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T;

	for (int case_num = 1; case_num <= T; case_num ++) {
		cin >> N;
		V = 0;
		int root = get_new_node();

		for (int i = 0; i < N; i++) {
			string s; cin >> s;
			reverse(s.begin(), s.end());
			int cur = root;
			for (char c : s) {
				assert('A' <= c && c <= 'Z');
				if (ch[cur][c - 'A'] == -1) {
					ch[cur][c - 'A'] = get_new_node();
				}
				cur = ch[cur][c - 'A'];
			}
			cnt[cur] ++;
		}

		ans = 0;
		dfs(root, true);

		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}

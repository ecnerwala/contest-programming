#include<bits/stdc++.h>
using namespace std;

struct trie {
	trie* ch[26] = {0,};
	int cnt = 0;
	~trie() {
		for (int i = 0; i < 26; i++) delete ch[i];
	}
};

int ans;
int dfs(trie* cur) {
	if (cur == nullptr) return 0;
	for (int i = 0; i < 26; i++) cur->cnt += dfs(cur->ch[i]);
	if (cur->cnt >= 2) {
		ans += 2;
		cur->cnt -= 2;
	}
	return cur->cnt;
}

int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T;

	for (int case_num = 1; case_num <= T; case_num ++) {
		int N; cin >> N;
		trie* ROOT = new trie();
		for (int i = 0; i < N; i++) {
			string s; cin >> s;
			reverse(s.begin(), s.end());
			trie* cur = ROOT;
			for (char c : s) {
				if (cur->ch[c-'A'] == nullptr) {
					cur->ch[c-'A'] = new trie();
				}
				cur = cur->ch[c-'A'];
			}
			cur->cnt ++;
		}

		ans = 0;
		for (int i = 0; i < 26; i++) dfs(ROOT->ch[i]);

		cout << "Case #" << case_num << ": " << ans << '\n';
		delete ROOT;
	}

	return 0;
}

#include<bits/stdc++.h>
using namespace std;

int hsh(string s) {
	int res = 0;
	for(char c : s) res *= 31, res += c;
	return res;
}

int main() {
	if(fopen("hash.in", "r")) {
		freopen("hash.in", "r", stdin);
		freopen("hash.out", "w", stdout);
	}
	ios_base::sync_with_stdio(0);

	int k; cin >> k;
	string start = "ZZZZZZZZZZZZZZ";
	int val = hsh(start);
	for(int m = 0; m < k; m++) {
		string cur = start;
		for(int i = 0; (1 << i) <= m; i++) {
			if(m & (1 << i)) {
				cur[i] += 31;
				cur[i + 1] --;
			}
		}
		reverse(cur.begin(), cur.end());
		cout << cur << '\n';
		assert(hsh(cur) == val);
	}
}

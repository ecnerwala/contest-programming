#include<bits/stdc++.h>
using namespace std;

int main() {
	int T;
	cin >> T;

	for(int t = 1; t <= T; t++) {
		int N; string s;
		cin >> N >> s;
		int res = 0;
		assert(s.size() == N + 1);
		assert(s.back() > '0');
		while(!s.empty() && s.back() == '0') s.pop_back();
		int cnt = 0;
		for(int i = 0; i < N; i++) {
			cnt++;
			cnt -= s[i] - '0';
			if(cnt > 0) {
				res += cnt;
				cnt = 0;
			}
		}
		cout << "Case #" << t << ": " << res << '\n';
	}
	return 0;
}

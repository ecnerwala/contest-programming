#include<bits/stdc++.h>
using namespace std;

bool isPalindrome(string s) {
	return s == string(s.rbegin(), s.rend());
}

int cntPalindrome(string s) {
	int ans = 0;
	for (int i = 0; i < int(s.size()); i++) {
		string t;
		for (int j = i; j < int(s.size()); j++) {
			t += s[j];
			ans += isPalindrome(t);
		}
	}
	return ans;
}

string go(int K) {
	string s = "";
	int n = 0;
	while ((n+1) * (n+2) / 2 <= K) s.push_back('A'), n++;
	K -= (n) * (n+1) / 2;
	while (K--) {
		s.push_back("BCDEFG"[K % 6]);
	}
	return s;
}

void test() {
	for (int K = 1; K <= 200; K++) {
		string v = go(K);
		assert(cntPalindrome(v) == K);
	}
	for (int K = int(1e5) - 1000; K <= int(1e5); K++) {
		string v = go(K);
		assert(v.size() <= 1000);
	}
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	test();
	int T; cin >> T;
	for (int t = 1; t <= T; t++) {
		int K; cin >> K;

		string ans = go(K);
		cout << "Case #" << t << ": " << ans << '\n';
	}

	return 0;
}

#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int N; cin >> N;
	vector<int> mp(1.1e5);
	int c4 = 0;
	int c2 = 0;
	while (N--) {
		int a; cin >> a;
		int& v = mp[a];
		c4 -= v / 4;
		c2 -= v / 2;
		v++;
		c4 += v / 4;
		c2 += v / 2;
	}
	int Q; cin >> Q;
	while (Q--) {
		char c; cin >> c; int a; cin >> a;
		int& v = mp[a];
		c4 -= v / 4;
		c2 -= v / 2;
		if (c == '+') v++;
		else if (c == '-') v--;
		else assert(false);
		c4 += v / 4;
		c2 += v / 2;

		bool res = c4 >= 1 && c2 >= 4;
		cout << (res ? "YES" : "NO") << '\n';
	}

	return 0;
}

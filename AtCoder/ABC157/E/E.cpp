#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	const int S = 1 << 19;
	vector<int> seg(2*S);
	int N; cin >> N;
	string str; cin >> str;
	for (int i = 0; i < N; i++) {
		seg[S+i] = 1<<(str[i] - 'a');
	}
	for (int i = S-1; i; i--) seg[i] = seg[2*i] | seg[2*i+1];

	int Q; cin >> Q;
	while (Q--) {
		int t; cin >> t;
		if (t == 1) {
			int i; char c; cin >> i >> c; i--;
			seg[S+i] = 1<<(c-'a');
			for (int a = (S+i)/2; a; a /= 2) {
				seg[a] = seg[2*a] | seg[2*a+1];
			}
		} else if (t == 2) {
			int l, r; cin >> l >> r; l--;
			int ans = 0;
			for (int a = S+l, b = S+r; a < b; a /= 2, b /= 2) {
				if (a & 1) ans |= seg[a++];
				if (b & 1) ans |= seg[--b];
			}
			cout << __builtin_popcount(ans) << '\n';
		} else assert(false);
	}

	return 0;
}

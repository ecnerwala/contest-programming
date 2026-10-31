#include<bits/stdc++.h>
using namespace std;

const int MAXN = 2.1e5;
int N, K;
int P[MAXN];

const int S = 1 << 18;
pair<int, int> seg[2*S];
pair<int, int> merge(pair<int, int> a, pair<int, int> b) {
	return {min(a.first, b.first), max(a.second, b.second)};
}
// inclusive
pair<int, int> query(int a, int b) {
	pair<int, int> ans(int(1e9), -int(1e9));
	for (int l = a+S, r = b+1+S; l < r; l /= 2, r /= 2) {
		if (l & 1) {
			ans = merge(ans, seg[l]);
			l++;
		}
		if (r & 1) {
			r--;
			ans = merge(ans, seg[r]);
		}
	}
	return ans;
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N >> K;
	for (int i = 0; i < N; i++) {
		cin >> P[i];
		seg[S+i] = {P[i], P[i]};
	}
	for (int i = S-1; i > 0; i--) {
		seg[i] = merge(seg[2*i], seg[2*i+1]);
	}
	int ans = 1;
	for (int i = 0; i + K < N; i++) {
		if (query(i, i + K) == pair<int,int>(P[i], P[i+K])) {
		} else {
			ans++;
		}
	}

	int totSorted = 0;
	int numSorted = 1;
	for (int i = 1; i < N; i++) {
		if (P[i] > P[i-1]) {
			numSorted++;
		} else {
			numSorted = 1;
		}
		if (numSorted == K) {
			totSorted++;
		}
	}

	if (totSorted) ans -= totSorted - 1;
	cout << ans << '\n';

	return 0;
}

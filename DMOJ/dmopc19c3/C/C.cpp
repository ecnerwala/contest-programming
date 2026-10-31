#include<bits/stdc++.h>
using namespace std;

const int MAXN = 2.1e5;
int bit[MAXN * 2];
void update(int v) {
	v += MAXN;
	for (int a = v; a < MAXN * 2; a += a & (-a)) {
		bit[a] ++;
	}
}
int query(int v) {
	int ans = 0;
	v += MAXN;
	for (int a = v-1; a; a -= a & (-a)) {
		ans += bit[a];
	}
	return ans;
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	// number of ways subarrays which are strictly greater than 0
	int N; cin >> N;
	int pref = 0;
	update(pref);
	long long ans = 0;
	for (int i = 0; i < N; i++) {
		int v; cin >> v;
		if (v == 1) pref++;
		else if (v == 2) pref--;
		else assert(false);
		ans += query(pref);
		update(pref);
	}
	cout << ans << '\n';

	return 0;
}

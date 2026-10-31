#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N; cin >> N;
	int lo = int(1e9);
	int s = 0;
	for (int i = 0; i < N; i++) {
		int v; cin >> v;
		s += v;
		if (v % 10) lo = min(lo, v);
	}
	cout << max(0, s % 10 ? s : (s - lo)) << '\n';
	return 0;
}

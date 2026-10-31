#include<bits/stdc++.h>
using namespace std;

const int MAXN = 1.1e5;
int N;
int R[MAXN];
int C[MAXN];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N;
	int Q; cin >> Q;
	for (int i = 0; i < N; i++) {
		cin >> R[i];
		R[i] %= 2;
	}
	for (int i = 0; i < N; i++) {
		cin >> C[i];
		C[i] %= 2;
	}

	vector<int> rBads, cBads;
	for (int i = 0; i+1 < N; i++) {
		if (R[i] != R[i+1]) rBads.push_back(i);
		if (C[i] != C[i+1]) cBads.push_back(i);
	}
	rBads.push_back(N-1);
	cBads.push_back(N-1);
	while (Q--) {
		int ra, ca, rb, cb; cin >> ra >> ca >> rb >> cb;
		ra--, ca --, rb --, cb--;
		if (ra > rb) swap(ra, rb);
		if (ca > cb) swap(ca, cb);
		if (
				rb <= *lower_bound(rBads.begin(), rBads.end(), ra) &&
				cb <= *lower_bound(cBads.begin(), cBads.end(), ca)
				) {
			cout << "YES" << '\n';
		} else {
			cout << "NO" << '\n';
		}
	}

	return 0;
}

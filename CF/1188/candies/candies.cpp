#include<bits/stdc++.h>
using namespace std;

const int MAXN = 1.1e5;
int N;
int S[MAXN];
int pref[MAXN];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N;
	for (int i = 0; i < N; i++) {
		cin >> S[i];
	}
	pref[0] = 0;
	for (int i = 0; i < N; i++) pref[i+1] = pref[i] + S[i];

	int Q; cin >> Q;
	for (int q = 0 ; q < Q; q++) {
		int l, r; cin >> l >> r; l--;
		cout << (pref[r] - pref[l]) / 10 << '\n';
	}

	return 0;
}

#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N; cin >> N;
	vector<int> pfac(N+1);
	for (int p = 2; p <= N; p++) {
		if (pfac[p]) continue;
		for (int j = p; j <= N; j += p) {
			if (!pfac[j]) pfac[j] = p;
		}
	}

	vector<int> vals(N-1);
	for (int i = 2; i <= N; i++) {
		vals[i-2] = i / pfac[i];
	}
	sort(vals.begin(), vals.end());
	for (int i = 0; i < N-1; i++) {
		cout << vals[i] << " \n"[i+1==N-1];
	}

	return 0;
}

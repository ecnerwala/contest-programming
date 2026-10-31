#include<bits/stdc++.h>
using namespace std;

using ld = long double;

const int MAXN = 200;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N, K; cin >> N >> K;

	ld best = 0;

	for (int k = 0; k < K; k++) {
		ld nbest = 0;
		for (int i = 1; i <= N; i++) {
			nbest += max(best, ld(i));
		}
		best = nbest / N;
	}
	cout << fixed << setprecision(30) << best << '\n';

	return 0;
}

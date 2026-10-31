#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<int> P(N);
	for (auto& p : P) { cin >> p; }
	if (N == 1 || (N == 3 && P[1] == 1)) {
		int v = int(std::find(P.begin(), P.end(), N) - P.begin());
		cout << v+1 << ' ' << v+1 << '\n';
	} else {
		cout << 1 + (P[0] == 1) << ' ' << N - (P[N-1] == 1) << '\n';
	}

	return 0;
}

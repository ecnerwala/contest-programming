#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int N, K; cin >> N >> K;
	string S; cin >> S;
	string NS(N, '\0');
	auto merge = [&](char a, char b) -> char {
		if (a == b) return a;
		if ((a == 'R' && b == 'S') || (a == 'P' && b == 'R') || (a == 'S' && b == 'P')) return a;
		return b;
	};
	for (int k = 0, l = 1; k < K; k++, l *= 2, l %= N) {
		for (int i = 0; i < N; i++) {
			NS[i] = merge(S[i], S[(i+l)%N]);
		}
		swap(NS, S);
	}
	cout << S[0] << '\n';

	return 0;
}

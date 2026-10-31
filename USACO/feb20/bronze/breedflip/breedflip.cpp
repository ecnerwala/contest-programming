#include<bits/stdc++.h>
using namespace std;

int main() {
	if (fopen("breedflip.in", "r")) {
		freopen("breedflip.in", "r", stdin);
		freopen("breedflip.out", "w", stdout);
	}
	int N; cin >> N;
	string S1, S2;
	cin >> S1 >> S2;
	S1 = "H" + S1 + "H";
	S2 = "H" + S2 + "H";
	int ans = 0;
	for (int i = 0; i <= N; i++) {
		ans += (S1[i] != S1[i+1]) != (S2[i] != S2[i+1]);
	}
	cout << ans / 2 << '\n';

	return 0;
}

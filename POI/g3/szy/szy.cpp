#include<bits/stdc++.h>
using namespace std;

char shift(char c, int a) {
	return char(((c - 'A') + a) % 26 + 'A');
}

int cnt[200];

int main() {
	ios_base::sync_with_stdio(0);
	int N; char C; string S;
	cin >> N >> C >> S;
	for (int i = 0; i < int(S.size()); i++) cnt[int(S[i])] ++;
	int m = 0;
	for (char a = 'A'; a <= 'Z'; a++) {
		m = max(m, cnt[int(a)]);
	}
	for (char a = 'A'; a <= 'Z'; a++) {
		if (cnt[int(a)] == m) {
			int diff = (C - a + 26) % 26;
			assert(shift(a, diff) == C);
			for (int i = 0; i < int(S.size()); i++) S[i] = shift(S[i], diff);
			cout << S << '\n';
			return 0;
		}
	}

	assert(false);
	return 0;
}

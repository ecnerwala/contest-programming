#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int dx = 0, dy = 0;
	string S; cin >> S;
	for (char c : S) {
		if (c == 'N') dy++;
		else if (c == 'S') dy--;
		else if (c == 'E') dx++;
		else if (c == 'W') dx--;
		else assert(false);
	}
	cout << dx << ' ' << dy << '\n';
	int Q; cin >> Q;
	while (Q--) {
		int i; cin >> i; i--;
		{
			char c = S[i];
			if (c == 'N') dy--;
			else if (c == 'S') dy++;
			else if (c == 'E') dx--;
			else if (c == 'W') dx++;
			else assert(false);
		}
		cin >> S[i];
		{
			char c = S[i];
			if (c == 'N') dy++;
			else if (c == 'S') dy--;
			else if (c == 'E') dx++;
			else if (c == 'W') dx--;
			else assert(false);
		}
		cout << dx << ' ' << dy << '\n';
	}

	return 0;
}

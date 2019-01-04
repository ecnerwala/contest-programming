#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int T; cin >> T;
	while (T--) {
		string s;
		for (int a = 0; a < 6; a++) {
			cin >> s;
		}
		int numJokers = 0;
		for (int a = 0; a < 6; a++) {
			cin >> s;
			if (s == "BJ" || s == "RJ") {
				numJokers++;
			}
		}
		for (int a = 0; a < 42; a++) {
			cin >> s;
		}
		cin >> s; // trump suit
		if (numJokers == 2) {
			cout << "Johann" << '\n';
		} else {
			cout << "Sebastian" << '\n';
		}
	}
	return 0;
}

// if the covering player has <= 1 joker, always pass, starting get stuck with the joker and lose
// if the covering player has both jokers... they can't cover anything, so they get stuck with all the cards and lose

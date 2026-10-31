#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	string S; cin >> S;

	string V;
	int64_t p = 0;
	while (!S.empty()) {
		for (int z = 0; true; z++) {
			if (z == 2) {
				cout << "No solution" << '\n';
				exit(0);
			}

			char c = S[z];
			bool is_good = true;
			for (int i = 0; i < int(S.size()); i++) {
				if ((S[i] == c) != (i % 2 == z)) {
					is_good = false;
				}
			}
			if (!is_good) {
				continue;
			}

			if (z == 1) {
				p += (1 << int(V.size()));
			}
			V += c;
			string T;
			for (int i = 0; i < int(S.size()); i++) {
				if (i % 2 != z) T += S[i];
			}
			S = T;

			break;
		}
	}

	for (char c = 'A'; c <= 'Z'; c++) {
		if (V.find(c) == string::npos) V += c;
	}

	p++;
	cout << V << '\n';
	cout << p << '\n';

	return 0;
}

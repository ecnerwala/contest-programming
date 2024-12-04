#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	std::string S; cin >> S;
	int N = int(S.size());
	{
	char best = 'z' + 1;
	for (int i = 0; i < N; i++) {
		if (i+1 < N && S[i] == S[i+1]) {
			best = std::min<char>(best, S[i]);
		}
	}
	if (best <= 'z') {
		cout << best << best << '\n';
		exit(0);
	}
}
	std::string best;
	for (int i = 0; i < N; i++) {
		if (i+2 < N && S[i] == S[i+2]) {
			std::string cnd(S.begin() + i, S.begin() + i + 3);
			if (best.empty() || cnd < best) best = cnd;
		}
	}
if (best.empty()) best = "-1";
cout << best << '\n';

	return 0;
}

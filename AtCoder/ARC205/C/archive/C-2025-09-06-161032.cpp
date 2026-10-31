#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<std::pair<int, int>> S(N);
	std::vector<std::pair<int, int>> T(N);
	for (int i = 0; i < N; i++) {
		std::cin >> S[i].first >> T[i].first;
		S[i].second = T[i].second = i;
	}
	std::sort(S.begin(), S.end());
	std::sort(T.begin(), T.end());
	bool good = true;
	for (int i = 0; i < N; i++) {
		if (S[i].second != T[i].second) {
			good = false;
			break;
		}
	}
	if (good) {
		cout << "Yes" << '\n';
		std::vector<int> P; P.reserve(N);
		for (int i = 0; i < N; i++) {
			if (S[i].first > T[i].first) {
				P.push_back(S[i].second);
			}
		}
		for (int i = N-1; i >= 0; i--) {
			if (S[i].first < T[i].first) {
				P.push_back(S[i].second);
			}
		}
		for (int i = 0; i < N; i++) {
			cout << P[i]+1 << " \n"[i+1==N];
		}
	} else {
		cout << "No" << '\n';
	}

	return 0;
}

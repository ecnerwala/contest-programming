#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	const int N = 100;
	const int M = 10000;

	int T = 100;
	int P; cin >> P;
	const double CHEAT_FRAC = 0.01 * P;

	cout << T << '\n';
	cout << P << '\n';

	mt19937 mt(48);

	for (int case_num = 1; case_num <= T; case_num ++) {
		std::uniform_real_distribution<double> dist(-3, 3);
		vector<double> people(N);
		for (auto& v : people) v = exp(dist(mt));
		vector<double> probs(M);
		for (auto& v : probs) v = exp(dist(mt));

		vector<vector<bool>> scores(N, vector<bool>(M));
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				scores[i][j] = std::bernoulli_distribution(people[i] / (people[i] + probs[j]))(mt);
			}
		}

		int cheater = std::uniform_int_distribution(0, N-1)(mt);
		for (int j = 0; j < M; j++) {
			if (std::bernoulli_distribution(CHEAT_FRAC)(mt)) scores[cheater][j] = true;
		}

		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				cout << scores[i][j];
			}
			cout << '\n';
		}

		cerr << "Case #" << case_num << ": " << cheater+1 << '\n';
	}

	return 0;
}

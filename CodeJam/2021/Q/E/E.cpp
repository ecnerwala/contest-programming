#include<bits/stdc++.h>
#include <random>

const int N = 100;
const int M = 10000;
const double CHEAT_FRAC = 0.5;

int solve(std::vector<std::vector<bool>> scores) {
	using namespace std;
	assert(int(scores.size()) == N);
	assert(int(scores[0].size()) == M);

	vector<double> people(N);
	vector<double> cheat_people(N);
	for (int i = 0; i < N; i++) {
		int num_solves = std::accumulate(scores[i].begin(), scores[i].end(), 0);
		double frac_solves = double(num_solves) / double(M);
		// if our score is v, then our solve fraction should be 1/6 integral_{-3+v}^{3+v} 1/1+e^{-x} dx = log(e^(v+3) + 1) - log(e^(v-3) + 1) = log((e^3 * e^v + 1) / (e^-3 * e^v + 1))
		people[i] = std::clamp(exp(3) * (exp(6 * frac_solves - 3) - exp(-3)) / (exp(3) - exp(6 * frac_solves - 3)), exp(-3), exp(3));

		double cheat_frac_solves = std::clamp((frac_solves - CHEAT_FRAC) / (1 - CHEAT_FRAC), 0., 1.);
		cheat_people[i] = std::clamp(exp(3) * (exp(6 * cheat_frac_solves - 3) - exp(-3)) / (exp(3) - exp(6 * cheat_frac_solves - 3)), exp(-3), exp(3));
	}
	vector<double> probs(M);
	{
		vector<char> cur_scores(N);
		for (int j = 0; j < M; j++) {
			for (int i = 0; i < N; i++) {
				cur_scores[i] = scores[i][j];
			}
			auto eval_prob = [&](double v) {
				double res = 1;
				for (int i = 0; i < N; i++) {
					res *= (cur_scores[i] ? people[i] : v) / (people[i] + v);
				}
				return res;
			};

			// estimate the difficulty
			double mi = exp(-3), ma = exp(3);
			for (int z = 0; z < 30; z++) {
				double md1 = (2 * mi + ma) / 3;
				double md2 = (mi + 2 * ma) / 3;
				if (eval_prob(md1) < eval_prob(md2)) {
					mi = md1;
				} else {
					ma = md2;
				}
			}
			probs[j] = (mi+ma)/2;
		}
	}

	/*
	cerr << "Inferred values" << '\n';
	for (int i = 0; i < N; i++) {
		cerr << showpos << fixed << setprecision(2) << log(people[i]) << " \n"[i+1==N];
	}
	for (int j = 0; j < M; j++) {
		cerr << showpos << fixed << setprecision(2) << log(probs[j]) << " \n"[j+1==M];
	}
	*/

	vector<double> cheater_vals(N);
	for (int i = 0; i < N; i++) {
		double cheater_odds = 1;
		for (int j = 0; j < M; j++) {
			cheater_odds *= (scores[i][j] ? (cheat_people[i] + CHEAT_FRAC * probs[j]) : ((1-CHEAT_FRAC) * probs[j])) / (cheat_people[i] + probs[j]);
			cheater_odds /= (scores[i][j] ? people[i] : probs[j]) / (people[i] + probs[j]);
		}
		cheater_vals[i] = cheater_odds;
	}

	return int(max_element(cheater_vals.begin(), cheater_vals.end()) - cheater_vals.begin());
}

std::mt19937 mt(48);
bool run_test() {
	using namespace std;
	std::uniform_real_distribution<double> dist(-3, 3);
	vector<double> people(N);
	for (auto& v : people) v = exp(dist(mt));
	vector<double> probs(M);
	for (auto& v : probs) v = exp(dist(mt));

	/*
	cerr << "Original values" << '\n';
	for (int i = 0; i < N; i++) {
		cerr << showpos << fixed << setprecision(2) << log(people[i]) << " \n"[i+1==N];
	}
	for (int j = 0; j < M; j++) {
		cerr << showpos << fixed << setprecision(2) << log(probs[j]) << " \n"[j+1==M];
	}
	*/

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
	int found_cheater = solve(scores);
	cerr << "Actual cheater" << ' ' << cheater << '\n';
	cerr << "Guessed cheater" << ' ' << found_cheater << '\n';
	return cheater == found_cheater;
}

void test() {
	using namespace std;
	int tot_tests = 0;
	int tot_pass = 0;
	for (int z = 0; z < 100; z++) {
		tot_tests++;
		tot_pass += run_test();
		cerr << "Ratio: " << tot_pass << "/" << tot_tests << " = " << fixed << setprecision(4) << double(tot_pass)/double(tot_tests) << '\n';
	}
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	//test();

	int T; cin >> T;
	int P; cin >> P;
	for (int case_num = 1; case_num <= T; case_num ++) {
		vector<vector<bool>> v(N, vector<bool>(M));
		for (auto& r : v) {
			string s; cin >> s;
			assert(int(s.size()) == M);
			for (int j = 0; j < M; j++) r[j] = s[j] - '0';
		}

		int ans = solve(v);
		cout << "Case #" << case_num << ": " << ans+1 << '\n';
	}

	return 0;
}

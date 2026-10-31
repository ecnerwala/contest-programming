#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	std::string S_BASE;
	{
		std::ifstream base("ProblemText.txt");
		std::getline(base, S_BASE);
	}
	const int L_BASE = int(S_BASE.size());
	cerr << "L_BASE = " << L_BASE << '\n';
	std::set<char> has;
	for (char c : S_BASE) has.insert(c);
	cerr << "letters: \"";
	for (auto c : has) { cerr << c; }
	cerr << "\"" << '\n';

	const int C = 56;
	assert(C == int(has.size()));
	auto id = [&](char c) {
		if (c == ' ') return 0;
		else if (c == '#') return 1;
		else if (c == ',') return 2;
		else if (c == '.') return 3;
		else if ('A' <= c && c <= 'Z') return 4 + (c - 'A');
		else if ('a' <= c && c <= 'z') return 30 + (c - 'a');
		else assert(false);
	};

	std::vector<std::array<int, 56>> pref(L_BASE+1);
	pref[0].fill(0);
	for (int i = 0; i < L_BASE; i++) {
		pref[i+1] = pref[i];
		pref[i+1][id(S_BASE[i])]++;
	}
	std::vector<int> starts;
	const int L_SAMP = 2025;
	for (int i = 0; i+L_SAMP <= L_BASE; i++) {
		if (i == 0 || S_BASE[i-1] == ' ') starts.push_back(i);
	}
	const int N_CND = int(starts.size());
	cerr << "N_CND = " << N_CND << '\n';
	std::vector<std::array<int, 56>> cnd_freqs(N_CND);
	for (int i = 0; i < N_CND; i++) {
		auto p = pref[starts[i]+L_SAMP];
		auto q = pref[starts[i]];
		for (int z = 0; z < C; z++) p[z] -= q[z];
		cnd_freqs[i] = p;
	}

	constexpr bool TESTING = true;
	int T;
	if (TESTING) {
		T = 150;
	} else {
		cin >> T;
	}
	std::mt19937 mt(48);
	std::vector<std::string> Ss(T);
	std::vector<int> TESTING_ANS(T);
	for (int case_num = 1; case_num <= T; case_num ++) {
		if (TESTING) {
			int cnd = std::uniform_int_distribution(0, N_CND-1)(mt);
			std::string S = S_BASE.substr(starts[cnd], L_SAMP);
			TESTING_ANS[case_num-1] = cnd;
			// TODO: Shuffle
			std::string S_filt; S_filt.reserve(L_SAMP);
			for (char c : S) {
				if (std::uniform_int_distribution(0, 1)(mt)) {
					S_filt.push_back(c);
				}
			}
			Ss[case_num-1] = S_filt;
		} else {
			int N; cin >> N;
			cin.ignore(1, '\n');
			std::string S; std::getline(cin, S);
			//cerr << "Case #" << case_num << " ";
			//cerr << "S = \"" << S << '"' << '\n';
			assert(int(S.size()) == N);

			Ss[case_num-1] = S;
		}
	}
	std::vector<std::string> answers(T);

	using dbl = double;
	std::vector<dbl> log_fact(L_SAMP+1);
	for (int i = 1; i <= L_SAMP; i++) log_fact[i] = log_fact[i-1] + std::log(dbl(i));

	std::vector<std::vector<int>> posses(T);

	for (int case_num = 0; case_num < T; case_num++) {
		auto S = Ss[case_num];
		//cerr << '\n'; cerr << "Case " << int(S.size()) << '\n';
		std::array<int, 56> freq; freq.fill(0);
		for (char c : S) freq[id(c)]++;
		int num_poss = 0;
		std::vector<std::pair<double, int>> probs;
		for (int cnd = 0; cnd < N_CND; cnd++) {
			bool poss = true;
			for (int z = 0; z < C; z++) if (freq[z] > cnd_freqs[cnd][z]) poss = false;
			if (!poss) continue;
			num_poss ++;
			posses[case_num].push_back(cnd); 
		}
	}
	const int NEED = T * 6 / 10;
	std::vector<std::pair<int, int>> easy;
	for (int case_num = 0; case_num < T; case_num++) {
		easy.push_back({posses[case_num].size(), case_num});
	}
	std::sort(easy.begin(), easy.end());

	int cur_has = 0;
	for (int z = 0; cur_has < NEED; z++) {
		int case_num = easy[z].second;
		auto S = Ss[case_num];
		cerr << '\n';
		cerr << "Case " << z << ' ' << int(S.size()) << '\n';
		std::array<int, 56> freq; freq.fill(0);
		for (char c : S) freq[id(c)]++;
		int num_poss = int(posses[case_num].size());
		std::vector<std::pair<int, int>> probs;
		for (int cnd : posses[case_num]) {
			// let's try computing the LCS, it should be at least 1/6
			std::vector<int> dp(L_SAMP+1, 0);
			for (char c : S) {
				for (int i = L_SAMP-1; i >= 0; i--) {
					if (S_BASE[starts[cnd]+i] == c) {
						dp[i+1] = std::max(dp[i+1], dp[i]+1);
					}
				}
				for (int i = 0; i < L_SAMP; i++) {
					dp[i+1] = std::max(dp[i+1], dp[i]);
				}
			}
			int len = dp[L_SAMP];
			probs.push_back({len, cnd});
		}
		/*
		double tot_prob = 0;
		for (auto [p, i] : probs) tot_prob += p;
		for (auto& [p, i] : probs) p /= tot_prob;
		*/
		std::sort(probs.begin(), probs.end(), std::greater());
		cerr << "num_poss " << num_poss << '\n';
		if (int(probs.size()) >= 2 && probs[1].first >= probs[0].first - 0) {
			cerr << "skipping" << '\n';
			continue;
		}
		cur_has++;
		if (TESTING) {
			int idx = 0;
			while (probs[idx].second != TESTING_ANS[case_num]) idx++;
			cerr << "0 -> " << probs[0].first << "; " << idx << " -> " << probs[idx].first << '\n';
			assert(probs[0].first == probs[idx].first);
			assert(idx <= 5);
		}
	}

	int num_skip = 0;
	for (int case_num = 1; case_num <= T; case_num ++) {
		auto ans = answers[case_num-1];
		if (ans.empty()) {
			ans = "SKIP";
			num_skip++;
		} else {
			assert(int(ans.size()) == 2025);
			if (TESTING) {
			}
		}
		cout << "Case #" << case_num << ": " << ans << '\n';
	}
	cerr << "tot skip " << num_skip << '\n';

	return 0;
}

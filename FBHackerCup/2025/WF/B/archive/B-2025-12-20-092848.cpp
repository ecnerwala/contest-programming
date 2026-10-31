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
		T = 300;
	} else {
		cin >> T;
	}
	std::mt19937 mt(48);
	std::vector<std::string> Ss(T);
	std::vector<std::string> TESTING_ANS(T);
	for (int case_num = 1; case_num <= T; case_num ++) {
		if (TESTING) {
			int cnd = std::uniform_int_distribution(0, N_CND-1)(mt);
			std::string S = S_BASE.substr(starts[cnd], L_SAMP);
			TESTING_ANS[case_num-1] = S;
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

	for (auto S : Ss) {
		cerr << "Case " << int(S.size()) << '\n';
		std::array<int, 56> freq; freq.fill(0);
		for (char c : S) freq[id(c)]++;
		int num_poss = 0;
		for (int i = 0; i < N_CND; i++) {
			bool poss = true;
			for (int z = 0; z < C; z++) if (freq[z] > cnd_freqs[i][z]) poss = false;
			if (!poss) continue;
			num_poss ++;
		}
		cerr << "num_poss " << num_poss << '\n';
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
				if (TESTING_ANS[case_num - 1] != ans) {
					cerr << "FAIL!" << '\n';
					cerr << TESTING_ANS[case_num - 1] << ' ' << ans << '\n';
				}
			}
		}
		cout << "Case #" << case_num << ": " << ans << '\n';
	}
	cerr << "tot skip " << num_skip << '\n';

	return 0;
}

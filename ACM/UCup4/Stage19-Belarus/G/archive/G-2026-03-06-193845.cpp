#include <bits/stdc++.h>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int P, S; std::cin >> P >> S;
		struct submission_t {
			std::string team_name;
			int prob;
			int time;
			bool ac;
		};
		std::vector<submission_t> submissions(S);
		for (auto& s : submissions) {
			std::cin >> s.team_name;

			char prob_letter; std::cin >> prob_letter;
			assert('A' <= prob_letter && prob_letter <= 'Z');
			s.prob = prob_letter - 'A';

			std::string time, verdict; std::cin >> time >> verdict;
			assert(int(time.size()) == 4);
			assert(time[1] == ':');
			s.time = (time[0] - '0') * 60 + (time[2] - '0') * 10 + (time[3] - '0');

			s.ac = verdict == "OK";
		}
		struct score_t {
			int cnt = 0;
			int penalty = 0;
			std::array<int, 26> wrongs{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
			int last_ac = 0;
		};
		std::map<std::string, score_t> scores;
		for (int z = 0; z < int(submissions.size()); z++) {
			const auto& s = submissions[z];
			auto& sc = scores[s.team_name];
			if (sc.wrongs[s.prob] == -1) continue;
			if (s.ac) {
				sc.cnt++;
				sc.penalty += s.time + 20 * sc.wrongs[s.prob];
				sc.wrongs[s.prob] = -1;
				sc.last_ac = z;
			} else {
				sc.wrongs[s.prob]++;
			}
		}
		std::cerr << int(scores.size()) << '\n';
		std::vector<std::pair<std::string, score_t>> teams(scores.begin(), scores.end());
		assert(int(teams.size()) >= 13);
		auto get_val = [](const score_t& s) {
			return std::tuple(s.cnt, -s.penalty, -s.last_ac);
		};
		auto cmp = [&](const std::pair<std::string, score_t>& a, const std::pair<std::string, score_t>& b) -> bool {
			return get_val(a.second) > get_val(b.second);
		};
		std::nth_element(teams.begin(), teams.begin() + 12, teams.end(), cmp);
		std::sort(teams.begin(), teams.begin() + 12, cmp);
		for (auto [t, s] : teams) {
			std::cerr << t << ' ' << s.cnt << ' ' << -s.penalty << ' ' << -s.last_ac << '\n';
		}

		scores = {};
		std::map<std::string, int> pass_time;
		for (int z = 0; z < int(submissions.size()); z++) {
			const auto& s = submissions[z];
			auto& sc = scores[s.team_name];
			if (sc.wrongs[s.prob] == -1) continue;
			if (s.ac) {
				sc.cnt++;
				sc.penalty += s.time + 20 * sc.wrongs[s.prob];
				sc.wrongs[s.prob] = -1;
				sc.last_ac = z;
				if (get_val(sc) > get_val(teams[12].second)) {
					std::cerr << s.team_name << ' ' << s.time << '\n';
					pass_time.insert({s.team_name, s.time});
				}
			} else {
				sc.wrongs[s.prob]++;
			}
		}
		assert(int(pass_time.size()) == 12);
		for (int z = 0; z < 12; z++) {
			auto team_name = teams[z].first;
			int t = pass_time[team_name];
			std::cout << team_name << ' ' << t/60 << ':' << t%60/10 << t%10 << '\n';
		}
	}

	return 0;
}

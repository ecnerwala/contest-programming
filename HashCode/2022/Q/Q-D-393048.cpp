#include <bits/stdc++.h>
using namespace std;

struct project_t {
	int idx;
	int duration;
	int points;
	int deadline;
};

int main() {
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int S = 0;
	std::map<string, int> s_ids;
	std::vector<string> s_names;
	auto get_skill_id = [&](string s) -> int {
		if (s_ids.count(s)) return s_ids[s];
		s_names.push_back(s);
		return s_ids[s] = S++;
	};

	int C, P; cin >> C >> P;
	std::vector<string> c_names(C);

	std::vector<std::map<int, int>> orig_c_skills(C);
	for (int i = 0; i < C; i++) {
		int cnt;
		cin >> c_names[i] >> cnt;
		while (cnt--) {
			string s; int level; cin >> s >> level;
			orig_c_skills[i][get_skill_id(s)] = level;
		}
	}

	std::vector<string> p_names(P);
	std::vector<project_t> p_projects(P);
	std::vector<std::vector<std::pair<int, int>>> p_skills(P);
	for (int i = 0; i < P; i++) {
		int n_roles;
		cin >> p_names[i] >> p_projects[i].duration >> p_projects[i].points >> p_projects[i].deadline >> n_roles;
		p_projects[i].idx = i;
		for (int z = 0; z < n_roles; z++) {
			string s; int level; cin >> s >> level;
			p_skills[i].push_back({get_skill_id(s), level});
		}
	}

	auto get_points = [&](int p, int start_time) {
		int end_time = start_time + p_projects[p].duration;
		int penalty = std::max(0, end_time - p_projects[p].deadline);
		return std::max(0, p_projects[p].points - penalty);
	};

	auto eval_ans = [&](const std::vector<std::pair<int, std::vector<int>>>& seq) -> int64_t {
		std::vector<int> free_times(C);
		int64_t tot = 0;
		auto cur_c_skills = orig_c_skills;
		for (const auto& [p, roles] : seq) {
			assert(roles.size() == p_skills[p].size());
			int R = int(roles.size());

			int start_time = 0;
			for (int z = 0; z < R; z++) {
				int c = roles[z];
				auto [s, l] = p_skills[p][z];
				// TODO: assert that there exist mentors
				if (cur_c_skills[c][s] < l-1) {
					return -1;
				}

				start_time = max(start_time, free_times[c]);
			}

			int end_time = start_time + p_projects[p].duration;

			for (int z = 0; z < R; z++) {
				int c = roles[z];
				auto [s, l] = p_skills[p][z];

				free_times[c] = end_time;
				if (cur_c_skills[c][s] <= l) cur_c_skills[c][s] ++;
			}
			tot += get_points(p, start_time);
		}
		return tot;
	};

	if (true) {
		for (int p = 0; p < P; p++) {
			auto v = p_skills[p];
			std::sort(v.begin(), v.end());
			bool has_dupe = false;
			for (int i = 0; i+1 < int(v.size()); i++) has_dupe = has_dupe || v[i].first == v[i+1].first;
			if (has_dupe) {
				cerr << p << ' ' << p_names[p] << '\n';
				for (auto [s, l] : v) {
					cerr << "  " << s << ' ' << l << '\n';
				}
			}
		}
	}

	auto greedy_ans = [&]() {
		std::vector<std::pair<int, std::vector<int>>> seq;
		std::vector<std::map<int, int>> cur_c_skills = orig_c_skills;
		std::vector<std::set<std::pair<int, int>>> s_people(S);
		for (int c = 0; c < C; c++) {
			for (auto [s, l] : cur_c_skills[c]) {
				s_people[s].insert({l, c});
			}
		}
		std::vector<int> free_times(C);
		std::vector<bool> done(P);

		int tot_points = 0;

		auto do_project = [&](int p, std::vector<int> r) {
			assert(!done[p]);
			done[p] = true;
			seq.emplace_back(p, r);
			int start_time = 0;
			for (int c : r) {
				start_time = max(start_time, free_times[c]);
			}
			tot_points += get_points(p, start_time);
			int end_time = start_time + p_projects[p].duration;
			for (int c : r) {
				free_times[c] = end_time;
			}

			/*
			cerr << "Doing project " << p << ' ' << p_names[p] << '\n';
			for (int z = 0; z < int(r.size()); z++) {
				int c = r[z];
				auto [s, l] = p_skills[p][z];
				cerr << "  Role " << c << ' ' << cur_c_skills[c][s] << (cur_c_skills[c][s] <= l ? "++" : "  ") << ' ' << s << ' ' << l << '\n';
			}
			*/

			for (int z = 0; z < int(r.size()); z++) {
				int c = r[z];
				auto [s, l] = p_skills[p][z];
				if (cur_c_skills[c][s] <= l) {
					s_people[s].erase({cur_c_skills[c][s], c});
					++cur_c_skills[c][s];
					s_people[s].insert({cur_c_skills[c][s], c});
				}
			}
		};

		while (true) {
			std::mt19937 mt(101);

			int best_project = -1;
			std::vector<int> best_r;
			int best_score = -1;

			for (int p = 0; p < P; p++) {
				if (done[p]) continue;
				int R = int(p_skills[p].size());
				std::vector<int> r(R, -1);
				bool is_good = true;
				std::set<int> used;
				std::vector<int> idxs(R);
				std::shuffle(idxs.begin(), idxs.end(), mt);
				std::iota(idxs.begin(), idxs.end(), 0);
				std::sort(idxs.begin(), idxs.end(), [&](auto a, auto b) { return p_skills[p][a].second > p_skills[p][b].second; });
				for (int z : idxs) {
					auto [s, l] = p_skills[p][z];

					bool has_mentor = false;
					for (int y = 0; y < R; y++) {
						if (r[y] == -1) continue;
						if (cur_c_skills[r[y]][s] >= l) {
							has_mentor = true;
							break;
						}
					}

					if (has_mentor) {
						if (l-1 == 0) {
							int c = mt() % C;
							while (used.count(c) || cur_c_skills[c][s] > 0) {
								c = mt() % C;
							}
							r[z] = c;
							used.insert(c);
						} else {
							for (auto [cl, c] : s_people[s]) {
								if (used.count(c)) continue;
								if (cl >= l-1) {
									r[z] = c;
									used.insert(c);
									break;
								}
							}
						}
					} else {
						for (auto [cl, c] : s_people[s]) {
							if (used.count(c)) continue;
							if (cl >= l) {
								r[z] = c;
								used.insert(c);
								break;
							}
						}
					}
					if (r[z] == -1) {
						is_good = false;
						break;
					}
				}

				if (!is_good) {
					continue;
				}

				int start_time = 0;
				for (int c : r) {
					start_time = max(start_time, free_times[c]);
				}
				int points = get_points(p, start_time);

				int n_levels = 0;
				int n_new = 0;
				int n_bad = 0;
				for (int z = 0; z < R; z++) {
					int c = r[z];
					auto [s, l] = p_skills[p][z];
					n_bad += cur_c_skills[c][s] > l;
					n_levels += cur_c_skills[c][s] <= l;
					n_new += cur_c_skills[c][s] == 0;
				}

				int score = mt();
				//int score = int(1e8) - start_time - n_bad * 100000;
				if (best_project == -1 || score > best_score) {
					best_score = score;
					best_project = p;
					best_r = r;
				}
			}

			if (best_project == -1) break;
			do_project(best_project, best_r);
			cerr << seq.size() << ' ' << best_score << ' ' << tot_points << '\n';
		}
		for (int p = 0; p < P; p++) {
			if (done[p]) continue;
			auto v = p_skills[p];
			std::sort(v.begin(), v.end());
			bool is_point_loss = false;
			for (int z = 0; z < int(v.size()); z++) {
				auto [s, l] = v[z];
				if (s_people[s].rbegin()->first < l) is_point_loss = true;
			}
			if (is_point_loss) continue;
			cerr << p << '\n';
			for (int z = 0; z < int(v.size()); z++) {
				auto [s, l] = v[z];
				cerr << ' ' << s << ' ' << s_names[s] << ' ' << l << ": ";
				for (auto [cl, c] : s_people[s]) cerr << cl << ' ';
				cerr << '\n';
			}
		}
		return seq;
	};


	std::vector<std::pair<int, std::vector<int>>> best_ans = greedy_ans();

	cerr << "Final score: " << eval_ans(best_ans) << '\n';

	cout << best_ans.size() << '\n';
	for (const auto& [p, roles] : best_ans) {
		cout << p_names[p] << '\n';
		for (int z = 0; z < int(roles.size()); z++) {
			cout << c_names[roles[z]] << " \n"[z+1==int(roles.size())];
		}
	}

	return 0;
}

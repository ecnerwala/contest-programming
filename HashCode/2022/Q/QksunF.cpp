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

	int max_score = 0;
	for(auto p : p_projects) max_score += p.points;
	int64_t total_person_hours = 0;
	for(int i = 0; i < P; i++){
		total_person_hours += p_projects[i].duration * p_skills[i].size();
	}
	int max_deadline = 0;
	int min_deadline = 1e8;
	for(int i = 0; i < P; i++) max_deadline = max(max_deadline, p_projects[i].deadline);
	for(int i = 0; i < P; i++) min_deadline = min(min_deadline, p_projects[i].deadline);
	vector<int> deadlines;
	for(int i = 0; i < P; i++){
		deadlines.push_back(p_projects[i].deadline);
	}
	sort(deadlines.begin(), deadlines.end());

	cerr << "people: " << C << '\n';
	cerr << "projects: " << P << '\n';
	cerr << "max_score: " << max_score << '\n';
	cerr << "nskills: " << S << '\n';
	cerr << "total_person_hours need: " << total_person_hours << '\n';
	cerr << "available hours: " << (max_deadline * int64_t(C)) << '\n';
	cerr << "deadlines: [" << min_deadline << "," << max_deadline << "]" << '\n';
	cerr << '\n';
	cerr << '\n';

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

		auto do_project = [&](int p, std::vector<int> r) {
			assert(!done[p]);
			done[p] = true;
			seq.emplace_back(p, r);
			int start_time = 0;
			for (int c : r) {
				start_time = max(start_time, free_times[c]);
			}
			int end_time = start_time + p_projects[p].duration;
			for (int c : r) {
				free_times[c] = end_time;
			}
			for (int z = 0; z < int(r.size()); z++) {
				int c = r[z];
				auto [s, l] = p_skills[p][z];
				if (cur_c_skills[c][s] <= l) {
					s_people[s].erase({cur_c_skills[c][s], c});
					++cur_c_skills[c][s];
					s_people[s].insert({cur_c_skills[c][s], c});
				}
				// cerr << s << ":" << cur_c_skills[c][s] << '/' << l << ' ';
			}
			cerr << "start: " << start_time << '-' << end_time << "/" << p_projects[p].deadline << " points " << p_projects[p].points << '\n';
		};

		auto get_people = [&](int id) -> vector<int> {
			vector<int> found;
			set<int> used;
			int tlevel = 0;
			vector<pair<int,int> > score(C);
			for(auto [s, level] : p_skills[id]){
				vector<int> ord;
				for(auto [cl, c] : s_people[s]){
					if(cur_c_skills[c][s] >= level && !used.count(c)){
						if(cur_c_skills[c][s] == level){
							score[c] = {-1, free_times[c]};
						} else {
							score[c] = {0, free_times[c]};
						}
						ord.push_back(c);
					}
				}
				if(ord.empty()) return {};
				sort(ord.begin(), ord.end(), [&](int x, int y) { return score[x] < score[y]; });
				int c = ord.front();
				found.push_back(c);
				used.insert(c);
				if(score[c].first == -1) tlevel++;
			}
			return found;
		};
		while (true) {
			int best_project = -1;
			std::vector<int> best_r;
			int best_score = -1;

			int doable = 0;

			for (int p = 0; p < P; p++) {
				if (done[p]) continue;
				int R = int(p_skills[p].size());

				// std::vector<int> r(R, -1);
				// bool is_good = true;
				// set<int> used;
				// for (int z = 0; z < R; z++) {
				// 	auto [s, l] = p_skills[p][z];

				// 	// TODO: Pick between the people better
				// 	for (auto [cl, c] : s_people[s]) {
				// 		if(used.count(c)) continue;
				// 		if (cl == l) {
				// 			r[z] = c;
				// 			break;
				// 		}
				// 	}
				// 	if(r[z] == -1){
				// 		for (auto [cl, c] : s_people[s]) {
				// 			if(used.count(c)) continue;
				// 			if (cl >= l) {
				// 				r[z] = c;
				// 				break;
				// 			}
				// 		}
				// 	}
				// 	if (r[z] == -1) {
				// 		is_good = false;
				// 		break;
				// 	}
				// }

				// if (!is_good) {
				// 	continue;
				// }
				// if (int(std::set<int>(r.begin(), r.end()).size()) < R) {
				// 	continue;
				// }

				vector<int> r = get_people(p);
				if(r.empty()) continue;
				doable++;

				int start_time = 0;
				for (int c : r) {
					start_time = max(start_time, free_times[c]);
				}
				int points = get_points(p, start_time);

				int n_levels = 0;
				for (int z = 0; z < R; z++) {
					int c = r[z];
					auto [s, l] = p_skills[p][z];
					n_levels += cur_c_skills[c][s] <= l;
				}

				if(points == 0) continue;

				int end_time = start_time + p_projects[p].duration;

				int score = 1e8 - start_time;
				if (score > best_score) {
					best_score = score;
					best_project = p;
					best_r = r;
				}
			}
			cerr << doable << '\n';

			if (best_project == -1) break;
			do_project(best_project, best_r);
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

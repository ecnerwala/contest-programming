#include <bits/stdc++.h>
using namespace std;

int T_MAX;

int N_INTER;
int N_STREETS;
int N_CARS;
int SCORE_FLAT;

const int INF = 1e9;

struct edge_t {
	int st, en;
	int len;
	string name;
	int id;
};
vector<edge_t> edges;
map<string, int> edgeNames;

vector<vector<int>> carPath;
vector<int> carPathLen;

vector<int> edgeCarCount;
vector<int> edgeStartCount;

using schedule_t = vector<vector<pair<int, int>>>; // edgeId, interval

struct simulate_response_t {
	int64_t score;
	vector<int> finish_times;
	vector<pair<int, int>> ejj_schedule;
};

simulate_response_t simulate(schedule_t schedule) {
	vector<int> mods(N_INTER);
	vector<tuple<int, int, int>> open_time(N_STREETS, {0, 0, 0});
	if (!schedule.empty()) {
		assert(int(schedule.size()) == N_INTER);
		for (int i = 0; i < N_INTER; i++) {
			mods[i] = 0;
			for (auto [e, t] : schedule[i]) {
				mods[i] += t;
			}
			int cur_t = 0;
			for (auto [e, t] : schedule[i]) {
				open_time[e] = {cur_t, cur_t + t, mods[i]};
				cur_t += t;
			}
		}
	}

	vector<int> carAns(N_CARS, 0);
	vector<int> carFinish(N_CARS, INF);

	vector<queue<pair<int, int>>> queues(N_STREETS); // time, carId
	vector<int> curIdx(N_CARS, 0);
	priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq; // time, edge number

	auto fill_q_head = [&](int e, int t_min = -1) -> void {
		if (queues[e].empty()) return;
		auto [st, en, mod] = open_time[e];
		int t = queues[e].front().first;
		t = max(t, t_min);
		if (!schedule.empty()) {
			if (mod == 0) return;
			if (t % mod < st) {
				t += (st - t % mod);
			} else if (t % mod < en) {
				// that's it
			} else {
				t += (st + mod - t % mod);
			}
		}
		pq.push({t, e});
	};

	auto advance_car = [&](int c, int t) -> void {
		int z = curIdx[c]++;
		int e = carPath[c][z];
		if (t == -1) {
			t = 0;
		} else {
			t += edges[e].len;
		}

		if (z+1 == int(carPath[c].size())) {
			carFinish[c] = t;
			// we're done
			if (t <= T_MAX) {
				carAns[c] = SCORE_FLAT + (T_MAX - t);
			} else {
				carAns[c] = 0;
			}
			return;
		}
		queues[e].push({t, c});
		if (queues[e].size() == 1) {
			fill_q_head(e);
		}
	};

	for (int c = 0; c < N_CARS; c++) {
		advance_car(c, -1);
	}

	while (!pq.empty()) {
		auto [t, e] = pq.top(); pq.pop();
		int c = queues[e].front().second;
		queues[e].pop();
		advance_car(c, t);
		fill_q_head(e, t+1);
	}

	simulate_response_t resp;
	resp.score = std::accumulate(carAns.begin(), carAns.end(), int64_t(0));
	resp.finish_times = carFinish;
	return resp;
}

int main() {
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	cin >> T_MAX;
	cin >> N_INTER;
	cin >> N_STREETS;
	cin >> N_CARS;
	cin >> SCORE_FLAT;

	edges.resize(N_STREETS);
	for (int e = 0; e < N_STREETS; e++) {
		edges[e].id = e;
		cin >> edges[e].st >> edges[e].en >> edges[e].name >> edges[e].len;
		edgeNames[edges[e].name] = e;
	}

	carPath.resize(N_CARS);
	carPathLen.resize(N_CARS);
	for (int c = 0; c < N_CARS; c++) {
		int P; cin >> P;
		carPath[c].resize(P);
		carPathLen[c] = 0;
		for (auto& e : carPath[c]) {
			string name; cin >> name;
			e = edgeNames[name];
			carPathLen[c] += edges[e].len;
		}
		carPathLen[c] -= edges[carPath[c][0]].len;
	}

	edgeCarCount.resize(N_STREETS);
	edgeStartCount.resize(N_STREETS);
	for (int c = 0; c < N_CARS; c++) {
		for (int z = 0; z < int(carPath[c].size())-1; z++) {
			edgeCarCount[carPath[c][z]]++;
		}
		edgeStartCount[carPath[c][0]]++;
	}

	vector<int> sortedWeights = edgeCarCount;
	sort(sortedWeights.begin(), sortedWeights.end());
	for (int z = 0; z < 20; z++) {
		cerr << sortedWeights[(N_STREETS / 20) * z] << ' ';
	}
	cerr << sortedWeights.back() << '\n';

	cerr << "T_MAX = " << T_MAX << '\n';
	cerr << "N_INTER = " << N_INTER << '\n';
	cerr << "N_STREETS = " << N_STREETS << '\n';
	cerr << "N_CARS = " << N_CARS << '\n';
	cerr << "SCORE_FLAT = " << SCORE_FLAT << '\n';


	schedule_t best_answer(N_INTER);
	for (const auto& e : edges) {
		if (edgeCarCount[e.id]) {
			best_answer[e.en].push_back({e.id, 1});
		}
	}
	auto best_sim = simulate(best_answer);
	cerr << "INITIAL SCORE = " << best_sim.score << '\n';

	mt19937 mt(48);

	schedule_t answers;
	simulate_response_t sim;

	for (int big_iter = 0; big_iter < 3; big_iter++) {
		vector<bool> carAlive(N_CARS, true);
		for (int iter = 0; iter < 100; iter++) {
			answers = schedule_t(N_INTER);
			edgeCarCount = vector<int>(N_STREETS);
			edgeStartCount = vector<int>(N_STREETS);
			for (int c = 0; c < N_CARS; c++) {
				if (carAlive[c]) {
					for (int z = 0; z < int(carPath[c].size())-1; z++) {
						edgeCarCount[carPath[c][z]]++;
					}
					edgeStartCount[carPath[c][0]]++;
				}
			}
			for (const auto& e : edges) {
				if (edgeCarCount[e.id]) {
					answers[e.en].push_back({e.id, 1});
				}
			}

			/*
			vector<double> wgt(N_INTER, 1);
			for (const auto& e : edges) {
				wgt[e.en] += edgeCarCount[e.id];
			}
			for(auto& x : answers){
				if(x.size() <= 1) continue;
				int n = (int)x.size() * 2;
				for(auto& y : x){
					auto e = edges[y.first];
					y.second += int(n / wgt[e.en] * edgeCarCount[e.id]);
				}
			}
			*/

			for (int i = 0; i < N_INTER; i++) {
				sort(answers[i].begin(), answers[i].end(), [&](pair<int,int> x, pair<int,int> y){
					return edgeStartCount[x.first] > edgeStartCount[y.first];
				});
				// shuffle(answers[i].begin(), answers[i].end(), mt);
			}
			sim = simulate(answers);
			if (sim.score > best_sim.score) {
				best_sim = sim;
				best_answer = answers;
			}

			cerr << "CURRENT SCORE = " << sim.score << '\n';
			vector<pair<int, int>> bad_cars; bad_cars.reserve(N_CARS);
			for (int c = 0; c < N_CARS; c++) {
				bool used = (sim.finish_times[c] <= T_MAX);
				if (!used) {
					bad_cars.push_back({sim.finish_times[c], c});
				}
				if (used) carAlive[c] = true;
				else if (carAlive[c]) {
					if (mt() % 10 == 0) {
						carAlive[c] = false;
					}
				}
			}
		}
		schedule_t cur_answer = answers;
		int64_t last_score = sim.score;
		for (int iter = 0; iter < 50; iter++) {
			schedule_t nxt_answer = cur_answer;
			int e = mt() % N_STREETS;
			for (auto& s : nxt_answer) {
				int z = 0;
				while (z < int(s.size()) && s[z].first != e) z++;
				if (z < int(s.size())) s.erase(s.begin()+z);
			}
			auto nxt_sim = simulate(nxt_answer);
			cerr << "TURN OFF EDGE = " << nxt_sim.score << '\n';
			if (nxt_sim.score > best_sim.score) {
				best_sim = nxt_sim;
				best_answer = nxt_answer;
			}
			if (nxt_sim.score > last_score) {
				last_score = nxt_sim.score;
				cur_answer = nxt_answer;
			}
		}
	}

	vector<int> waitTimes;
	for (int c = 0; c < N_CARS; c++) {
		waitTimes.push_back(best_sim.finish_times[c] - carPathLen[c]);
	}
	sort(waitTimes.begin(), waitTimes.end());
	for (int z = 0; z < 20; z++) {
		cerr << waitTimes[(N_CARS / 20) * z] << ' ';
	}
	cerr << waitTimes.back() << '\n';

	cerr << "BEST SCORE = " << best_sim.score << '\n';
	int val = 0;
	for(int i = 0; i < N_INTER; i++){
		if(best_answer[i].size()) val++;
	}
	cout << val << '\n';
	for(int i = 0; i < N_INTER; i++){
		if(best_answer[i].size()){
			cout << i << '\n';
			cout << best_answer[i].size() << '\n';
			for(pair<int,int> a : best_answer[i]){
				string name = edges[a.first].name;
				cout << name << ' ' << a.second << '\n';
			}
		}
	}

	return 0;
}

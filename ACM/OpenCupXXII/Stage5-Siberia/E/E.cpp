#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;

	int last_t = 0;
	std::map<std::pair<int, int>, int> ans;

	int a = 5;
	int b = 5;
	std::vector<std::tuple<int, int, int>> evts; evts.reserve(10);
	auto process_to = [&](int t) -> void {
		while (!evts.empty() && std::get<0>(evts[0]) <= t) {
			auto [nt, team, type] = evts[0];
			evts.erase(evts.begin());
			if (nt > last_t) {
				ans[{a, b}] += (nt - last_t);
			}
			last_t = nt;
			if (team == 0) a++;
			else b++;
		}
		int nt = t;
		if (nt > last_t) {
			ans[{a, b}] += (nt - last_t);
		}
		last_t = nt;
	};

	for (int i = 0; i < N; i++) {
		string ts; cin >> ts;
		int team; cin >> team; team--;
		string type; cin >> type;

		int t = (((ts[0] - '0') * 10 + (ts[1] - '0')) * 60 + ((ts[3] - '0') * 10 + (ts[4] - '0'))) * 10 + (ts[6] - '0');
		process_to(t);

		//cerr << ts << ' ' << t << ' ' << team <<' ' << type << '\n';

		if (type == "goal") {
			team = !team;
			if ((team == 0 ? a < b : b < a)) {
				auto it = std::find_if(evts.begin(), evts.end(), [&](auto v) -> bool {
					return std::get<1>(v) == team && std::get<2>(v) == 0;
				});
				if (it != evts.end()) {
					if (team == 0) a++;
					else b++;
					evts.erase(it);
				}
			}
		} else if (type == "minor") {
			if (team == 0) a--;
			else b--;
			evts.push_back({t + 2 * 60 * 10, team, 0});
		} else if (type == "major") {
			if (team == 0) a--;
			else b--;
			evts.push_back({t + 5 * 60 * 10, team, 1});
		} else assert(false);
		std::sort(evts.begin(), evts.end());
	}

	process_to(60 * 60 * 10);

	for (auto [x, t] : ans) {
		cout << x.first << 'x' << x.second << ' ';
		int m = t / 10 / 60;
		int s = t / 10 % 60;
		int d = t % 10;
		cout << m / 10 << m % 10 << ':' << s / 10 << s % 10 << '.' << d << '\n';
	}

	return 0;
}

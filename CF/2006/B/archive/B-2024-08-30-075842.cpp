#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; int64_t W; cin >> N >> W;
		std::vector<int> P(N);
		P[0] = -1;
		for (int i = 1; i < N; i++) {
			cin >> P[i]; P[i]--;
		}
		std::vector<std::pair<int, int64_t>> evts(N-1);
		for (auto& [x, y] : evts) { cin >> x >> y; x--; }

		std::vector<int> evt_time(N);
		evt_time[0] = -1;
		for (int t = 0; t < N-1; t++) {
			int x = evts[t].first;
			evt_time[x] = t;
		}

		std::vector<int64_t> num_question(N);
		for (int i = 0; i < N; i++) {
			int a = i, b = (i+1)%N;
			if (i == N-1) std::swap(a, b);
			assert(a < b);
			int max_time = -1;
			while (b > a) {
				max_time = std::max(max_time, evt_time[b]);
				b = P[b];
			}
			while (a > b) {
				max_time = std::max(max_time, evt_time[a]);
				a = P[a];
			}
			assert(a == b);
			assert(0 <= max_time && max_time < N-1);
			num_question[max_time]++;
		}
		for (int i = N-1; i >= 1; i--) {
			num_question[i-1] += num_question[i];
		}

		int64_t tot_seen = 0;
		for (int i = 0; i < N-1; i++) {
			tot_seen += evts[i].second;
			cout << tot_seen * 2 + (W - tot_seen) * num_question[i+1] << " \n"[i+1==N-1];
		}
	}

	return 0;
}

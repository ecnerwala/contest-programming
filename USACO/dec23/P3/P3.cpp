#include <bits/stdc++.h>

template<typename T> void setmin(T& a, T b) { if (b < a) a = b; }

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	// There are 2 types of states:
	// 1. We just sent a train at the first possible time, and we're holding some number of the opposite color.
	// 2. We just sent all trains of some alternating parity, and we're holding everything of the opposite color which started within T seconds.
	// From each, we can either keep alternating (as long as there's at least one hold), or we can jump to a train of the same color.

	int N; int64_t T; cin >> N >> T;
	std::vector<std::pair<int64_t, bool>> trains(N);
	for (auto& [t, s] : trains) {
		char c; cin >> c >> t;
		assert(c == 'A' || c == 'B');
		s = (c - 'A');
	}
	std::sort(trains.begin(), trains.end());

	// Your state at any point in time is:
	// Waiting for the next train A, holding some number of B's; we store this as the number of B trains already sent.
	// Waiting for your train A to arrive so that you can send your accumulated B's (evts); all trains since your send are
	// You need to store: # of trains of each color we've already accounted for, which actually includes the other info
	const int64_t INF = 2e18;

	int64_t ans = INF;

	std::array<std::vector<int64_t>, 2> waiting;
	waiting[0].resize(N+1, INF);
	waiting[0][0] = 0;
	waiting[1].resize(N+1, INF);
	waiting[1][0] = 0;

	struct evt_state {
		int64_t t;
		int64_t evt_val;
		std::array<int, 2> pref_cnt;
		int nxt_side;
	};

	std::vector<evt_state> evts; evts.reserve(N*N);
	std::array<int, 2> pref_cnt{0, 0};
	for (int t_idx = 0, e_idx = 0; t_idx < int(trains.size()) || e_idx < int(evts.size()); ) {
		if (t_idx < int(trains.size()) && (e_idx == int(evts.size()) || trains[t_idx].first <= evts[e_idx].t)) {
			auto [t, s] = trains[t_idx++];
			cerr << "t = " << t << ' ' << "s = " << s << ' ' << "train" << '\n';
			// For each state in waiting[!s], we just stay in the same state
			pref_cnt[s]++;
			// For each state in waiting[s], we send ourself.
			// Otherwise, we could switch to evt.
			int64_t evt_val = INF;
			for (int b_cnt = 0; b_cnt <= N; b_cnt++) {
				if (waiting[s][b_cnt] == INF) continue;
				int64_t v = waiting[s][b_cnt] += t;
				evt_val = std::min(evt_val, v + (pref_cnt[!s] - b_cnt) * (t + T));
			}
			if (evt_val != INF) {
				cerr << "  " << "adding event " << t+T << ' ' << evt_val << ' ' << !s << '\n';
				evts.push_back({t + T, evt_val, pref_cnt, !s});
			}
		} else {
			auto [t, evt_val, old_pref_cnt, s] = evts[e_idx++];
			// Send everything in between
			int num_new = pref_cnt[s] - old_pref_cnt[s];
			evt_val += num_new * t;
			old_pref_cnt[s] = pref_cnt[s];
			cerr << "t = " << t << ' ' << "s = " << s << ' ' << "event" << ' ' << evt_val << ' ' << pref_cnt[!s] - old_pref_cnt[!s] << '\n';

			if (old_pref_cnt[s] + old_pref_cnt[!s] == N) {
				// sent everything, we're done
				ans = std::min(ans, evt_val);
			} else {
				// Now, we can either add a new event or we can just wait for the same color as s
				cerr << "  " << "adding event " << t+T << ' ' << evt_val + (pref_cnt[!s] - old_pref_cnt[!s]) * (t + T) << ' ' << !s << '\n';
				evts.push_back({t + T, evt_val + (pref_cnt[!s] - old_pref_cnt[!s]) * (t + T), pref_cnt, !s});
				cerr << "  " << "adding waiting " << s << ' ' << old_pref_cnt[!s] << ' ' << evt_val << '\n';
				setmin(waiting[s][old_pref_cnt[!s]], evt_val);
			}
		}
	}

	for (auto [t, s] : trains) ans -= t;
	cout << ans << '\n';

	return 0;
}

#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; int64_t V, K, D; cin >> N >> M >> V >> K >> D;
	struct zombie_t {
		int64_t T, H;
		int idx;
	};
	vector<zombie_t> zombies(N); for (auto& z : zombies) cin >> z.T >> z.H;
	for (int i = 0; i < N; i++) zombies[i].idx = i;
	sort(zombies.begin(), zombies.end(), [&](const auto& a, const auto& b) { return tie(a.T, a.idx) < tie(b.T, b.idx); });

	map<int64_t, int64_t> mp_spikeweeds;
	for (int j = 0; j < M; j++) {
		int64_t p, a; cin >> p >> a;
		p = (p + V - 1) / V;
		mp_spikeweeds[p] += a;
	}
	mp_spikeweeds[0] = 0;
	{
		int64_t cur = 0;
		for (auto& v : mp_spikeweeds) {
			cur += v.second;
			v.second = cur;
			//cerr << "Spikeweeds " << v.first << ' ' << v.second << '\n';
		}
	}

	vector<int64_t> ans(N);

	int64_t cur_time = 0;
	for (int i = 0; i < N; i++) {
		//cerr << "processing zombie " << zombies[i].T << ' ' << zombies[i].H << ' ' << zombies[i].idx << '\n';

		cur_time = max(cur_time, zombies[i].T * K);
		int64_t H = zombies[i].H;
		auto get_spikeweeds = [&](int64_t pos) -> int64_t {
			assert(pos >= 1);
			auto it = mp_spikeweeds.upper_bound(pos);
			assert(it != mp_spikeweeds.begin());
			--it;
			return it->second;
		};
		// is he alive at the end of second a
		auto is_alive = [&](int64_t a) -> bool {
			assert(a >= zombies[i].T);
			int64_t pos = a - zombies[i].T + 1;
			return get_spikeweeds(pos) + std::max<int64_t>(0, (a+1)*K - cur_time) * D < H;
		};
		int64_t mi = zombies[i].T - 1; // at the end of time mi, he's alive
		int64_t ma = (cur_time + H + K) / K + 2; // at the end of time ma, he's dead
		while (ma - mi > 1) {
			int64_t md = mi + (ma - mi) / 2;
			if (is_alive(md)) mi = md;
			else ma = md;
		}
		ans[zombies[i].idx] = ma;
		//cerr << "dies at " << ma << '\n';
		{
			assert(ma >= zombies[i].T);
			int64_t pos = ma - zombies[i].T + 1;
			H -= get_spikeweeds(pos);
			H -= std::max<int64_t>(0, ma*K - cur_time) * D;
			cur_time = max(cur_time, ma*K);
			if (H > 0) {
				int64_t num_shots = (H + D - 1) / D;
				cur_time += num_shots;
				assert(cur_time <= (ma+1)*K);
			}
		}
		//cerr << cur_time << '\n';
	}

	for (int i = 0; i < N; i++) {
		cout << ans[i] << " \n"[i+1==N];
	}

	return 0;
}

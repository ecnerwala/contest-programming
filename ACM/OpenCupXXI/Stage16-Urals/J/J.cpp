#include <bits/stdc++.h>

template <typename T> using min_pq = std::priority_queue<T, std::vector<T>, std::greater<T>>;

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, R_ex, J, ST, EN; cin >> N >> R_ex >> J >> ST >> EN;
	J *= 2;

	vector<int> X(N); for (auto& x : X) cin >> x;
	sort(X.begin(), X.end());

	auto is_good = [&](int a) -> bool {
		auto it = lower_bound(X.begin(), X.end(), a);
		if (it != X.end() && *it - a < R_ex) return false;
		if (it != X.begin() && a - *prev(it) < R_ex) return false;
		return true;
	};

	vector<int> pts; pts.reserve((N+1)*N*4);
	pts.push_back(ST);

	auto add_dest = [&](int a) -> void {
		if (!is_good(a)) return;
		pts.push_back(a);
		{
			auto it = lower_bound(X.begin(), X.end(), a);
			for (int cur = a + J; it != X.end(); cur += J) {
				if (*it >= cur) break;
				while (it != X.end() && *it < cur) ++it;
				assert(*prev(it) < cur);
				if (*prev(it) > cur - R_ex) break;
				if (it != X.end() && *it < cur + R_ex) break;

				pts.push_back(cur);
			}
		}

		{
			auto it = lower_bound(X.begin(), X.end(), a);
			for (int cur = a - J; it != X.begin(); cur -= J) {
				--it;
				if (*it < cur) break;
				while (it != X.begin() && *prev(it) >= cur) --it;
				assert(*it >= cur);
				if (*it < cur + R_ex) break;
				if (it != X.begin() && *prev(it) > cur - R_ex) break;

				pts.push_back(cur);
			}
		}
	};

	for (int x : X) {
		add_dest(x - R_ex);
		add_dest(x + R_ex);
	}
	add_dest(EN);

	sort(pts.begin(), pts.end());
	pts.resize(unique(pts.begin(), pts.end()) - pts.begin());

	const int V = int(pts.size());

	vector<int> num_smaller(V);
	for (int i = 0, j = 0; i < V; i++) {
		while (j < N && X[j] < pts[i]) j++;
		num_smaller[i] = j;
		assert(j == 0 || X[j-1] + R_ex <= pts[i]);
		assert(j == N || pts[i] <= X[j] - R_ex);
	}

	vector<int> jump_l(V, -1);
	vector<int> jump_r(V, -1);
	for (int i = 0, j = 0; i < V; i++) {
		while (j < i && pts[j] < pts[i]-J) j++;
		assert(j <= i);
		if (pts[j] == pts[i] - J) {
			jump_r[j] = i;
			jump_l[i] = j;
		}
	}

	//for (int i = 0; i < V; i++) { cerr << pts[i] << ' '; } cerr << '\n';
	//for (int i = 0; i < V; i++) { cerr << num_smaller[i] << ' '; } cerr << '\n';

	ST = int(lower_bound(pts.begin(), pts.end(), ST) - pts.begin());
	EN = int(lower_bound(pts.begin(), pts.end(), EN) - pts.begin());

	const double J_COST = J * M_PI / 2;
	vector<double> dist(V, -1);
	vector<bool> vis(V, false);

	min_pq<pair<double, int>> pq;
	pq.push({dist[ST] = 0, ST});
	while (!pq.empty()) {
		auto [d, cur] = pq.top(); pq.pop();
		if (vis[cur]) continue;
		vis[cur] = true;
		if (cur == EN) {
			cout << fixed << setprecision(10) << d << '\n';
			exit(0);
		}

		auto relax = [&](double nd, int nxt) -> void {
			if (dist[nxt] < -0.5 || nd < dist[nxt]) {
				pq.push({dist[nxt] = nd, nxt});
			}
		};
		if (cur > 0 && num_smaller[cur-1] == num_smaller[cur]) {
			relax(d + (pts[cur] - pts[cur-1]), cur-1);
		}
		if (cur+1 < V && num_smaller[cur+1] == num_smaller[cur]) {
			relax(d + (pts[cur+1] - pts[cur]), cur+1);
		}

		if (jump_l[cur] != -1) {
			relax(d + J_COST, jump_l[cur]);
		}
		if (jump_r[cur] != -1) {
			relax(d + J_COST, jump_r[cur]);
		}
	}

	cout << -1 << '\n';

	return 0;
}

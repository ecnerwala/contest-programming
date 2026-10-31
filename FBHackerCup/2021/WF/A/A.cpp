#include<bits/stdc++.h>

struct union_find {
	std::vector<std::pair<int, bool>> par;
	std::vector<std::array<int, 2>> cnt;
	int tot_cost = 0;
	union_find() {}
	explicit union_find(int N) : par(N, {-1, false}), cnt(N, {1, 0}), tot_cost(0) {}

	std::pair<int, bool> get_par(std::pair<int, bool> x) {
		auto [a, b] = x;
		while (par[a].first >= 0) {
			int p = par[a].first;
			if (par[p].first >= 0) {
				par[a].second ^= par[p].second;
				par[a].first = par[p].first;
			}
			b ^= par[a].second;
			a = par[a].first;
		}
		return {a, b};
	}
	bool merge(std::pair<int, bool> a, std::pair<int, bool> b) {
		auto [x, fa] = get_par(a);
		auto [y, fb] = get_par(b);
		bool f = fa ^ fb;
		if (x == y) {
			return f == false;
		}
		if (par[x].first > par[y].first) { std::swap(x, y); }
		if (f) {
			std::swap(cnt[y][0], cnt[y][1]);
		}
		tot_cost -= std::min(cnt[x][0], cnt[x][1]);
		tot_cost -= std::min(cnt[y][0], cnt[y][1]);
		cnt[x][0] += cnt[y][0];
		cnt[x][1] += cnt[y][1];
		tot_cost += std::min(cnt[x][0], cnt[x][1]);

		par[x].first += par[y].first;
		par[y] = {x, f};

		return true;
	}
};


int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		cerr << "Case #" << case_num << "\n";

		int N, K; cin >> N >> K;
		std::vector<std::array<std::string, 2>> A(N);
		for (int i = 0; i < N; i++) {
			cin >> A[i][0] >> A[i][1];
		}

		int L;
		{
			std::vector<int> lens; lens.reserve(2*N);
			for (int i = 0; i < N; i++) {
				lens.push_back(int(A[i][0].size()));
				lens.push_back(int(A[i][1].size()));
			}
			std::nth_element(lens.begin(), lens.begin() + N, lens.end());
			L = lens[std::min(2*N-1, N+10)] + 10;
			for (int i = 0; i < N; i++) {
				for (int z = 0; z < 2; z++) {
					std::reverse(A[i][z].begin(), A[i][z].end());
					A[i][z].resize(L, '0');
				}
			}
		}

		union_find cur(N);

		std::vector<int> res(L+2);
		for (int l = L-1; l >= 0; l--) {
			union_find cnd = cur;
			// all the 0's are on the same side
			std::pair<int, bool> last_0 = {-1, false};
			for (int i = 0; i < N; i++) {
				for (int z = 0; z < 2; z++) {
					if (A[i][z][l] == '0') {
						std::pair<int, bool> cur_0{i, z};
						if (last_0.first != -1) {
							if (!cnd.merge(last_0, cur_0)) {
								goto bad;
							}
						}
						last_0 = cur_0;
					}
				}
			}

			if (cnd.tot_cost <= K) {
				if (last_0.first == -1) {
					res[l] = 2;
				} else {
					res[l] = 1;
				}
				cur = cnd;
			}
bad:
			;
		}

		for (int i = 0; i < int(res.size()); i++) {
			if (res[i] >= 2) {
				assert(i+1 < int(res.size()));
				res[i+1] += res[i] / 2;
				res[i] &= 1;
			}
		}
		while (int(res.size()) >= 2 && res.back() == 0) res.pop_back();
		std::reverse(res.begin(), res.end());

		cout << "Case #" << case_num << ": ";
		for (int v : res) { cout << v; }
		cout << '\n';
	}

	return 0;
}

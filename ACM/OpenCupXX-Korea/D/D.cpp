#include<bits/stdc++.h>
using namespace std;

namespace std {

template<class Fun>
class y_combinator_result {
	Fun fun_;
public:
	template<class T>
	explicit y_combinator_result(T &&fun): fun_(std::forward<T>(fun)) {}

	template<class ...Args>
	decltype(auto) operator()(Args &&...args) {
		return fun_(std::ref(*this), std::forward<Args>(args)...);
	}
};

template<class Fun>
decltype(auto) y_combinator(Fun &&fun) {
	return y_combinator_result<std::decay_t<Fun>>(std::forward<Fun>(fun));
}

} // namespace std

int C;

// moves t to s
vector<pair<int, int>> solve(vector<int> sloc, vector<int> tloc) {
	assert(sloc.size() == tloc.size());
	int M = int(sloc.size());

	for (int i = 0; i < M; i++) {
		assert(sloc[i] <= tloc[i]);
		//cerr << sloc[i] << '-' << tloc[i] << '\n';
	}

	const int INF = 1e9;
	vector<int> dp(M+1, INF);
	vector<int> prv(M+1);
	dp[0] = 0;
	prv[0] = -1;
	for (int i = 0; i < M; i++) {
		int cnd = dp[i] + (tloc[i] - sloc[i]) / 2 * (4+C) + (tloc[i] - sloc[i]) % 2 * (3+C);
		if (cnd < dp[i+1]) {
			dp[i+1] = cnd;
			prv[i+1] = i;
		}

		if ((tloc[i] - sloc[i]) % 2) {
			deque<int> locs;
			int cost = dp[i];
			for (int j = i; j < M; j++) {
				if (!locs.empty() && sloc[j] > locs.front()) {
					break;
				}

				int l = tloc[j] - sloc[j] - int(locs.size());

				if (tloc[j] % 2 == tloc[i] % 2) {
					locs.push_back(tloc[j]);
				}
				if (sloc[j] % 2 == tloc[i] % 2) {
					locs.pop_front();
				}

				l -= int(locs.size());
				assert(l % 2 == 0);

				cost += l / 2 * (4+C);
				cost += int(locs.size()) * (5+C);

				if (locs.empty()) {
					if (cost < dp[j+1]) {
						dp[j+1] = cost;
						prv[j+1] = i;
					}
					break;
				}
			}
		}
	}

	//cerr << dp[M] << '\n';

	vector<int> locs;
	for (int cur = M; cur >= 0; cur = prv[cur]) {
		locs.push_back(cur);
	}
	reverse(locs.begin(), locs.end());

	vector<pair<int, int>> res;
	for (int z = 0; z+1 < int(locs.size()); z++) {
		int i = locs[z], j = locs[z+1];
		//cerr << i << ' ' << j << '\n';
		if (j-i == 1) {
			while (tloc[i] - sloc[i] >= 2) {
				res.push_back({tloc[i]-2, tloc[i]});
				tloc[i] -= 2;
			}
			if (tloc[i] - sloc[i] == 1) {
				res.push_back({tloc[i]-1, tloc[i]});
				tloc[i] -= 1;
			}
		} else {
			auto moveTo = std::y_combinator([&tloc, &res](auto self, int r, int d) -> void {
				assert(tloc[r] >= d);
				while (tloc[r] > d) {
					if ((tloc[r] - d) % 2) {
						self(r+1, tloc[r]+1);
						res.push_back({tloc[r]-1, tloc[r]+1});
						tloc[r+1] --;
						tloc[r] --;
					} else {
						res.push_back({tloc[r]-2, tloc[r]});
						tloc[r] -= 2;
					}
				}
			});
			for (int k = i; k < j; k++) {
				moveTo(k, sloc[k]);
			}
		}
	}
	return res;
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N; cin >> N;
	cin >> C;
	string S, T; cin >> S >> T;
	assert(int(S.size()) == N);
	assert(int(T.size()) == N);
	vector<int> sloc;
	vector<int> tloc;
	for (int i = 0; i < N; i++) {
		if (S[i] == '2') sloc.push_back(i+1);
		if (T[i] == '2') tloc.push_back(i+1);
	}
	assert(sloc.size() == tloc.size());

	int M = int(sloc.size());
	vector<pair<int, int>> ans;
	for (int i = 0; i < M; ) {
		bool dir = sloc[i] <= tloc[i];
		int j = i;
		while (j < M && (sloc[j] <= tloc[j]) == dir) {
			j++;
		}
		vector<int> subs(sloc.begin()+i, sloc.begin()+j);
		vector<int> subt(tloc.begin()+i, tloc.begin()+j);
		if (dir) {
			vector<pair<int, int>> cur = solve(subs, subt);
			ans.insert(ans.end(), cur.rbegin(), cur.rend());
		} else {
			vector<pair<int, int>> cur = solve(subt, subs);
			ans.insert(ans.end(), cur.begin(), cur.end());
		}

		i = j;
	}

	cout << ans.size() << '\n';
	for (auto it : ans) {
		cout << it.first << ' ' << it.second << '\n';
	}
	return 0;
}

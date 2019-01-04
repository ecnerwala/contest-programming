#include<bits/stdc++.h>
using namespace std;

using ll = long long;

const int MAXN = 3e5;
const int MAXM = 3e5;
const int MAXQ = 3e5;
int N, M, Q;
int A[MAXN];

vector<int> pos[MAXM];

vector<pair<pair<int, int>, int>> queries[MAXM];
ll answer[MAXQ];

ll c2(ll v) {
	return v * (v-1) / 2;
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N >> M >> Q;
	for (int i = 1; i <= N; i++) {
		cin >> A[i];
		pos[A[i]].push_back(i);
	}

	for (int q = 1; q <= Q; q++) {
		int l, r, m; cin >> l >> r >> m;
		queries[m].emplace_back(pair<int, int>{l, r}, q);
	}

	vector<pair<int, int>> ranges;
	for (int m = 1; m <= M; m++) {
		{
			// merge the two smaller
			vector<pair<int, int>> nranges;
			for (int i = 0, j = 0; j < int(ranges.size()); ) {
				if (ranges[i].second >= ranges[j].first) {
					j++;
					continue;
				}
				assert(ranges[i].second < ranges[j].first);
				while (!nranges.empty() && nranges.back().second == ranges[j].second) {
					nranges.pop_back();
				}
				nranges.emplace_back(ranges[i].first, ranges[j].second);
				i++;
			}
			ranges.clear();
			// now move things from nranges into ranges, killing the pos
			for (int i = 0, k = 0; i < int(nranges.size()) || k < int(pos[m].size()); ) {
				if (k < int(pos[m].size())) {
					while (i < int(nranges.size()) && nranges[i].second < pos[m][k]) {
						ranges.push_back(nranges[i]);
						i++;
					}
					while (i < int(nranges.size()) && nranges[i].first <= pos[m][k]) {
						i++;
					}
					ranges.emplace_back(pos[m][k], pos[m][k]);
					k++;
				} else {
					while (i < int(nranges.size())) {
						ranges.push_back(nranges[i]);
						i++;
					}
				}
			}
		}

		for (int i = 0; i + 1 < int(ranges.size()); i++) {
			assert(ranges[i].first < ranges[i+1].first);
			assert(ranges[i].second < ranges[i+1].second);
		}

		vector<ll> prefCnts;
		prefCnts.push_back(0);
		for (int i = 0; i + 1 < int(ranges.size()); i++) {
			// v is the # of things that start in (ranges[i].first, ranges[i+1].first] and are bad
			// they have to end < ranges[i+1].second
			ll v = c2(ranges[i+1].first - ranges[i].first) + ll(ranges[i+1].second - ranges[i+1].first) * ll(ranges[i+1].first - ranges[i].first);
			prefCnts.push_back(prefCnts.back() + v);
		}

		sort(queries[m].begin(), queries[m].end());
		for (auto query : queries[m]) {
			int l = query.first.first, r = query.first.second, q = query.second;
			auto it = lower_bound(ranges.begin(), ranges.end(), pair<int, int>{l, l});
			if (it == ranges.end() || it->second > r) {
				answer[q] = 0;
				continue;
			}
			int lind = int(it - ranges.begin());
			int rind;
			{
				int mi = lind, ma = int(ranges.size());
				while (ma - mi > 1) {
					int md = (mi + ma) / 2;
					if (ranges[md].second > r) {
						ma = md;
					} else {
						mi = md;
					}
				}
				assert(ranges[mi].second <= r);
				rind = mi;
			}
			l--;
			answer[q] = c2(r - l + 1);
			answer[q] -= prefCnts[rind] - prefCnts[lind];
			answer[q] -= c2(ranges[lind].first - l) + ll(ranges[lind].second - ranges[lind].first) * ll(ranges[lind].first - l);
			answer[q] -= c2(r - ranges[rind].first + 1);
		}
	}
	for (int q = 1; q <= Q; q++) {
		cout << answer[q] << '\n';
	}

	return 0;
}

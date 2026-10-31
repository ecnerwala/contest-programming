#include<bits/stdc++.h>
using namespace std;

using ll = long long;

const int MAXN = 1.1e5;
int N;
int X[MAXN];
int Y[MAXN];
pair<double, int> score[MAXN];
map<int, int> mp[MAXN];

vector<int> go() {
	int S; double thresh_d;
	cin >> N >> S >> thresh_d;

	int thresh_i = int(thresh_d * 1000 + 0.1);
	for (int i = 0; i < N; i++) {
		cin >> X[i] >> Y[i];
		double s; cin >> s;
		score[i] = {-s, i};
	}
	sort(score, score + N);
	//reverse(score, score + N);

	vector<int> buckets(N);
	for (int i = 0; i < N; i++) {
		buckets[i] = X[i] / S;
	}
	sort(buckets.begin(), buckets.end());
	buckets.resize(unique(buckets.begin(), buckets.end()) - buckets.begin());

	vector<int> ans;
	for (int z = 0; z < N; z++) {
		int i = score[z].second;
		//cerr << "trying " << i << '\n';
		int midB = int(lower_bound(buckets.begin(), buckets.end(), X[i] / S) - buckets.begin());
		for (int b = max(0, midB - 1); b <= midB + 1 && b < int(buckets.size()); b++) {
			auto it = mp[b].lower_bound(Y[i]);
			for (auto jt = it; jt != mp[b].end(); jt++) {
				if (jt->first >= Y[i] + S) break;

				ll ix = max(0, S - abs(jt->second - X[i]));
				ll iy = max(0, S - abs(jt->first - Y[i]));
				ll ia = ix * iy;
				if (ia * 1000 > thresh_i * (2ll * S * S - ia)) {
					goto isBad;
				}
			}

			for (auto jt = it; jt != mp[b].begin(); ) {
				--jt;
				if (jt->first <= Y[i] - S) break;

				ll ix = max(0, S - abs(jt->second - X[i]));
				ll iy = max(0, S - abs(jt->first - Y[i]));
				ll ia = ix * iy;
				if (ia * 1000 > thresh_i * (2ll * S * S - ia)) {
					goto isBad;
				}
			}
		}
		ans.push_back(i);
		mp[midB].emplace(Y[i], X[i]);
isBad:;
	}

	for (int i = 0; i < int(buckets.size()); i++) {
		mp[i].clear();
	}
	return ans;
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int T; cin >> T;
	for (int t = 1; t <= T; t++) {
		vector<int> ans = go();
		sort(ans.begin(), ans.end());
		cout << "Case #" << t << ": " << ans.size() << '\n';
		for (int i = 0; i < int(ans.size()); i++) {
			cout << ans[i]+1 << " \n"[i+1==int(ans.size())];
		}
	}

	return 0;
}

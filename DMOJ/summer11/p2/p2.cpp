#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int N; cin >> N;
	int64_t X; cin >> X;
	vector<int64_t> T(N); for (auto& t : T) cin >> t;

	vector<int64_t> dp1; dp1.reserve(N+1); dp1.push_back(0);
	map<int64_t, int> dp2; dp2.emplace(0, 0);
	for (int64_t t : T) {
		{
			int l = int(lower_bound(dp1.begin(), dp1.end(), t+X) - dp1.begin());

			auto it = dp2.lower_bound(t);
			assert(it != dp2.begin());
			auto pt = it; --pt;

			l = max(l, pt->second + 1);

			if (it != dp2.end() && it->first == t) l = max(l, it->second);

			while (it != dp2.end() && it->second <= l) {
				it = dp2.erase(it);
			}
			dp2.emplace(t, l);
		}

		{
			auto it = lower_bound(dp1.begin(), dp1.end(), t);
			if (it == dp1.end()) dp1.push_back(t);
			else *it = t;
		}
	}
	cout << dp2.rbegin()->second << '\n';

	return 0;
}

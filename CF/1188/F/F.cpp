#include<bits/stdc++.h>
using namespace std;

using ll = long long;

const ll INF = 2e18;

const int MAXN = 1.1e5;
const int MAXL = 62;
const int L = 60;
int N;
ll A[MAXN];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N;
	for (int i = 0; i < N; i++) { cin >> A[i]; }
	ll maxA = *max_element(A, A + N);
	for (int i = 0; i < N; i++) { A[i] = maxA - A[i]; }

	vector<ll> cutoffs({0ll, 1ll});
	vector<ll> dp({0ll}); // min cost

	vector<pair<ll, int>> Aval(N);
	for (int i = 0; i < N; i++) Aval[i] = {0, i};

	for (int l = 0; l < L; l++) {
		{
			vector<pair<ll, int>> nAval[2];
			for (int i = 0; i < N; i++) {
				ll bit = (1ll << l) & A[Aval[i].second];
				Aval[i].first |= bit;
				nAval[bool(bit)].push_back(Aval[i]);
			}
			Aval = std::move(nAval[0]);
			Aval.insert(Aval.end(), nAval[1].begin(), nAval[1].end());
		}

		vector<ll> ncutoffs({(1ll << (l+1))});
		for (int i = 0; i < N; i++) {
			ncutoffs.push_back((1ll << (l+1)) - Aval[i].first);
		}
		ncutoffs.push_back(0ll);
		reverse(ncutoffs.begin(), ncutoffs.end());
		ncutoffs.resize(unique(ncutoffs.begin(), ncutoffs.end()) - ncutoffs.begin());

		vector<ll> ndp(ncutoffs.size() - 1, INF);

		for (ll b = 0; b < 2; b++) {
			auto c1 = Aval.rbegin();
			auto c2 = Aval.rbegin();
			auto c3 = Aval.rbegin();
			auto c4 = Aval.rbegin();

			int nz = 0;
			for (int z = 0; z < int(dp.size()); z++) {
			ll lo = cutoffs[z];
			//ll hi = cutoffs[z+1];
			ll val = dp[z];
				ll nlo = lo + (b << l);
				//ll nhi = hi + (b << l);

				ll nval = val;

				// nlo is the concrete value
				// either between (1 << l) and (2 << l) or between (3 << l) and (4 << l)
				while (c1 != Aval.rend() && (1ll << l) - nlo <= c1->first) { ++c1; }
				while (c2 != Aval.rend() && (2ll << l) - nlo <= c2->first) { ++c2; }
				while (c3 != Aval.rend() && (3ll << l) - nlo <= c3->first) { ++c3; }
				while (c4 != Aval.rend() && (4ll << l) - nlo <= c4->first) { ++c4; }
				nval += (c1 - c2) + (c3 - c4);

				while (nlo >= ncutoffs[nz+1]) nz++;
				ndp[nz] = min(ndp[nz], nval);
			}
		}

		cutoffs = ncutoffs;
		dp = ndp;
	}

	cout << dp[0] << '\n';

	return 0;
}

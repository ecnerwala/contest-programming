#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

using ll = long long;

const int MAXA = 22;
const int MAXS = 5.1e5;
int A;
ll arrivalS[MAXA];

const int MAXC = 1.1e6;
int C;
pair<ll, int> containers[MAXC];
bool dp[MAXS];

const int MAXQ = 1.1e6;
int Q;
pair<ll, ll> queries[MAXQ];

const int MAXR = MAXS + 20;
int R;
ll reached[MAXR];
int pos[MAXA];

class Depot {

public:

	int countPositive(vector <string> arrival_strings, vector <string> query_strings) {
		A = 0; // number of arrival types
		C = 0; // number of containers
		Q = 0; // number of queries
		for (const string& line : arrival_strings) {
			istringstream str(line);
			ll D, I, M, S; str >> D >> I >> M >> S;
			assert(S);
			arrivalS[A] = S;

			M = min(M, MAXS / S + 1); // we don't care after we hit MAXS / S, cause after that it's too big
			for (ll i = 0; i < M; i++) {
				containers[C++] = {D, A};
				D += I;
			}
			A++;
		}
		for (const string& line : query_strings) {
			istringstream str(line);
			ll D, S; str >> D >> S;
			ll A1, B1, A2, B2, N;
			if (str >> A1) {
				str >> B1 >> A2 >> B2 >> N;
				for (ll i = 0; i < N; i++) {
					D--, D %= B1, D++;
					S--, S %= B2, S++;
					queries[Q++] = {D, S};
					D += A1;
					S += A2;
				}
			} else {
				queries[Q++] = {D, S};
			}
		}
		sort(queries, queries + Q);
		sort(containers, containers + C);
		memset(dp, 0, sizeof(dp));
		dp[0] = true;
		R = 0;
		reached[R++] = 0;
		for (int i = 0; i < A; i++) {
			pos[i] = 0;
		}

		int res = 0;
		for (int c = 0, q = 0; q < Q; ) {
			if (c < C && containers[c].first <= queries[q].first) {
				// insert the container
				int a = containers[c].second;
				ll S = arrivalS[a];
				//cerr << S << '\n';
				int curR = R;
				while (pos[a] < curR) {
					ll cur = reached[pos[a]++];
					assert(dp[cur]);
					cur += S;
					if (cur < MAXS && !dp[cur]) {
						dp[cur] = true;
						reached[R++] = cur;
					}
				}
				c++;
			} else {
				ll S = queries[q].second;
				assert(S < MAXS);
				res += dp[queries[q].second];
				q++;
			}
		}
		return res;
	}
};

}  // namespace ecnerwala

using ecnerwala::Depot;

// vim:ft=cpp

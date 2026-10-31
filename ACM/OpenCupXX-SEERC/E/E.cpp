#include<bits/stdc++.h>
using namespace std;

using ll = long long;

const ll INF = 4e18;

const int MAXN = 1.1e5;
const int MAXK = 1.1e5;
int N, K;
const int LR = 1; // min age of a rider
ll LC, PC, LM, PM;
ll T, D;
ll A[MAXN];

// first is # needed, second is # can give
pair<ll, ll> prefR[MAXN];
pair<ll, ll> prefM[MAXN];
pair<ll, ll> prefC[MAXN];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N >> K;
	cin >> LC >> PC >> LM >> PM;
	cin >> T >> D;
	for (int i = 0; i < N; i++) {
		cin >> A[i];
		assert(A[i] >= LR);
	}
	sort(A, A + N);

	prefR[0] = prefM[0] = prefC[0] = {0,0};

	for (int i = 0; i < N; i++) {
		prefR[i+1] = {prefR[i].first + max(0ll, LR - A[i]), prefR[i].second + max(0ll, min(D, A[i] - LR))};
		prefM[i+1] = {prefM[i].first + max(0ll, LM - A[i]), prefM[i].second + max(0ll, min(D, A[i] - LM))};
		prefC[i+1] = {prefC[i].first + max(0ll, LC - A[i]), prefC[i].second + max(0ll, min(D, A[i] - LC))};
	}


	ll bestCost = INF;
	for (int c = 0; c <= (N + K-1) / K; c++) {
		// last c people need to be able to drive
		int ic = N - c;
		// these people need motorcycles
		int im = min(ic, (K-1) * c);

		if (ic < N && A[ic] + D < LC) {
			continue;
		}

		if (im < ic && A[im] + D < LM) {
			continue;
		}

		ll totNeed = prefR[im].first + (prefM[ic].first - prefM[im].first) + (prefC[N].first - prefC[ic].first);
		ll totGive = prefR[im].second + (prefM[ic].second - prefM[im].second) + (prefC[N].second - prefC[ic].second);
		if (totGive < totNeed) {
			continue;
		}

		ll cost = PM * (ic - im) + PC * (N - ic) + T * totNeed;

		bestCost = min(bestCost, cost);
	}

	if (bestCost == INF) {
		cout << -1 << '\n';
	} else {
		cout << bestCost << '\n';
	}

	return 0;
}

#include<bits/stdc++.h>
using namespace std;

using ll = long long;

const int MAXN = 1.1e5;
int N;
ll L[MAXN], R[MAXN];


int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N;
	for (int i = 0; i < N; i++) {
		cin >> L[i] >> R[i];
	}

	ll cost = 0;

	ll lo_shift = 0;
	priority_queue<ll> lo;
	ll hi_shift = 0;
	priority_queue<ll, vector<ll>, greater<ll>> hi;
	lo.push(L[0] - lo_shift);
	hi.push(L[0] - hi_shift);

	for (int i = 1; i < N; i++) {
		lo_shift -= (R[i] - L[i]);
		hi_shift += (R[i-1] - L[i-1]);

		ll vlo = lo.top() + lo_shift;
		ll vhi = hi.top() + hi_shift;
		if(vlo <= L[i] && L[i] <= vhi) {
			lo.push(L[i] - lo_shift);
			hi.push(L[i] - hi_shift);
		} else if (vhi < L[i]) {
			cost += L[i] - vhi;
			hi.pop();
			lo.push(vhi - lo_shift);
			hi.push(L[i] - hi_shift);
			hi.push(L[i] - hi_shift);
		} else if (L[i] < vlo) {
			cost += vlo - L[i];
			lo.pop();
			hi.push(vlo - hi_shift);
			lo.push(L[i] - lo_shift);
			lo.push(L[i] - lo_shift);
		} else assert(false);
	}

	cout << cost << '\n';
	return 0;
}

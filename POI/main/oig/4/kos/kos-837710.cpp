#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const ll INF = 1e9;

const int MAXN = 1e6 + 200;
int N;
ll A[MAXN];
ll V[MAXN];

// max time to shoot
ll get_time(int i) {
	ll a = A[0] - A[i];
	ll v = V[i] - V[0];
	if (v < 0) {
		v *= -1, a *= -1;
	}
	if (a < 0) {
		return INF; // never hit
	}
	if (a == 0) {
		return 0; // must shoot now
	}
	if (v == 0) {
		return INF;
	}
	return a / v; // floor
}

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N;
	for (int i = 0; i <= N; i++) {
		cin >> A[i] >> V[i];
	}
	vector<pair<ll, int> > res;
	for (int i = 1; i <= N; i++) {
		res.push_back(pair<ll,int>(get_time(i), i));
	}
	sort(res.begin(), res.end());
	bool good = true;
	for (int i = 0; i < N; i++) {
		if (res[i].first < i) {
			good = false;
		}
	}
	if (good) {
		for (int i = 0; i < N; i++) {
			if(i) cout << ' ';
			cout << res[i].second;
		}
		cout << '\n';
	} else {
		cout << "GAME OVER" << '\n';
	}
	return 0;
}

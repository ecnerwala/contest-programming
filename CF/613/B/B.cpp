#include<bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef pair<ll, ll> pll;
#define A first
#define B second

const int MAXN = 2e5;
ll N;
ll M; // max skill
ll CF, CM;
ll V; // total value to spend
pll A[MAXN];

int main() {
	ios_base::sync_with_stdio(0):
	cin >> N;
	cin >> M >> CF >> CM >> V;
	for(int i = 0; i < N; i++) {
		cin >> A[i].A;
		A[i].B = i;
	}
	sort(A, A + N);

	ll res = 0;
	ll best = 0; // best number of perfect

	int cur_cost = 0;
	return 0;
}

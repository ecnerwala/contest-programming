#include<bits/stdc++.h>
using namespace std;

using perm = vector<int>;
int N;
perm inv(const perm& a) {
	perm r(N);
	for (int i = 0; i < N; i++) r[a[i]] = i;
	return r;
}
perm operator * (const perm& a, const perm& b) {
	perm r(N);
	for (int i = 0; i < N; i++) r[i] = a[b[i]];
	return r;
}

perm pow(perm a, int b) {
	perm r(N);
	iota(r.begin(),r.end(),0);
	while (b) {
		if (b&1) r = r * a;
		a = a * a;
		b >>= 1;
	}
	return r;
}

int K;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N >> K; K--;
	perm P(N), Q(N);
	for (int i = 0; i < N; i++) cin >> P[i], P[i]--;
	for (int i = 0; i < N; i++) cin >> Q[i], Q[i]--;
	perm pref = Q * inv(P) * inv(Q) * P;
	pref = pow(pref, K / 6);
	perm mid;
	switch(K % 6) {
		case 0:
			mid = P;
			break;
		case 1:
			mid = Q;
			break;
		case 2:
			mid = Q * inv(P);
			break;
		case 3:
			mid = Q * inv(P) * inv(Q);
			break;
		case 4:
			mid = Q * inv(P) * inv(Q) * P * inv(Q);
			break;
		case 5:
			mid = Q * inv(P) * inv(Q) * P * P * inv(Q);
			break;
		default:
			assert(false);
	}
	perm res = pref * mid * inv(pref);

	for (int i = 0; i < N; i++) cout << res[i]+1 << " \n"[i+1==N];

	return 0;
}

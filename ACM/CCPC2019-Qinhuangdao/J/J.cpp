#include<bits/stdc++.h>
using namespace std;

using ll = long long;

const int MAXN = 1.1e7;
int N;
int kmp[MAXN];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	ll A, B; cin >> A >> B;
	string S; cin >> S;
	S = string(S.begin() + S.find('.') + 1, S.end());
	reverse(S.begin(), S.end());

	cerr << S << '\n';
	N = int(S.size());
	kmp[0] = -1;
	kmp[1] = 0;
	for (int i = 1; i < N; i++) {
		int cnd = kmp[i];
		while (cnd >= 0 && S[cnd] != S[i]) {
			cnd = kmp[cnd];
		}
		kmp[i+1] = cnd+1;
	}

	ll ans = A - B;
	for (int i = 1; i <= N; i++) {
		ans = max(ans, A * i - B * (i - kmp[i]));
	}
	cout << ans << '\n';

	return 0;
}

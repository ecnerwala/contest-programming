#include<bits/stdc++.h>
using namespace std;

using ll = long long;

const int MAXN = 3.1e5;
int N;
ll S[MAXN];
ll Spref[MAXN];
ll T[MAXN];
ll Tpref[MAXN];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N;
	for (int i = 0; i < N; i++) { cin >> S[i]; }
	for (int i = 0; i < N; i++) { cin >> T[i]; }
	sort(S, S + N);
	sort(T, T + N);

	Spref[0] = Tpref[0] = 0;
	for (int i = 0; i < N; i++) {
		Spref[i+1] = S[i] + Spref[i];
		Tpref[i+1] = T[i] + Tpref[i];
		if (Spref[i+1] > Tpref[i+1]) {
			cout << "NO\n";
			exit(0);
		}
	}
	if (Spref[N] != Tpref[N]) {
		cout << "NO\n";
		exit(0);
	}

	cout << "YES\n";

	return 0;
}

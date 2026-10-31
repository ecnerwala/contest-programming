#include<bits/stdc++.h>
#include<bits/extc++.h>
using namespace std;
using namespace __gnu_pbds;

// Custom hash function approach is better
const int RANDOM = int(chrono::high_resolution_clock::now().time_since_epoch().count());
struct chash {
	int operator()(int x) const { return x ^ RANDOM; }
};

const int MAXN = 2.1e3;
int N;
int A[MAXN];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int T; cin >> T;
	while (T--) {
		cin >> N;
		for (int i = 0; i < N; i++) {
			cin >> A[i];
		}
		unordered_map<int, int, chash> mp;
		long long ans = 0;
		for (int j = 0; j < N; j++) {
			for (int k = j+1; k < N; k++) {
				ans += int(mp[A[j] + A[j] - A[k]]);
			}
			mp[A[j]]++;
		}
		cout << ans << '\n';
	}

	return 0;
}

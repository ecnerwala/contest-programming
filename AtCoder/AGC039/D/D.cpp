#include<bits/stdc++.h>
using namespace std;

using ll = long long;
using pt = complex<double>;

const int MAXN = 4000;
int N;
ll L;
ll T[MAXN];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N >> L;
	for (int i = 0; i < N; i++) cin >> T[i];
	sort(T, T + N);
	pt ans = 0;
	for (int i = 0; i < N; i++) {
		for (int j = i+1; j < N; j++) {
			double probInner = double(j-i-1) / (N-2) / N / (N-1) * 6;
			double probOuter = double(N - (j-i+1)) / (N-2) / N / (N-1) * 6;

			pt ptOuter = polar(1., M_PI / L * (T[i] + T[j]));
			pt ptInner = -ptOuter;
			//cerr << i << ' ' << j << '\n';
			//cerr << probInner << ' ' << probOuter << '\n';
			//cerr << ptInner << ' ' << ptOuter << '\n';
			ans += probInner * ptInner + probOuter * ptOuter;
		}
	}
	cout << fixed << setprecision(15);
	cout << ans.real() << ' ' << ans.imag() << '\n';

	return 0;
}

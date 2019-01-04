#include<bits/stdc++.h>
using namespace std;

const double INF = HUGE_VAL;

const int MAXN = 60;
const int MAXH = 60;

int N;

int H[MAXN];

double dist[MAXN * 2][MAXN * 2];
bool vis[MAXN*2][MAXN*2];

double go() {
	cin >> N;
	for (int i = 0; i <= N; i++) {
		cin >> H[i];
	}

	for (int i = 0; i <= 2 * N; i++) {
		for (int j = 0; j <= 2 * N; j++) {
			dist[i][j] = HUGE_VAL;
			vis[i][j] = false;
		}
	}

	priority_queue<pair<double, pair<int, int>>, vector<pair<double, pair<int, int>>>, greater<pair<double, pair<int, int>>>> pq;

	pair<int, int> st(0, 2*N);
	dist[st.first][st.second] = 0.;
	pq.emplace(dist[st.first][st.second], st);
	pair<int, int> en(2*N, 0);

	while (!pq.empty()) {
		double d = pq.top().first;
		pair<int, int> state = pq.top().second;
		pq.pop();

		int a = state.first, b = state.second;


		if (vis[a][b]) continue;
		vis[a][b] = true;

		if (state == en) {
			return d;
		}

		int h;
		if (a % 2 == 0) {
			h = H[a / 2];
		} else if (b % 2 == 0) {
			h = H[b / 2];
		} else {
			assert(false);
		}

		if (a % 2 == 0) {
			assert(h == H[a / 2]);
		} else {
			int l = H[a / 2];
			int r = H[a / 2 + 1];
			assert(min(l, r) < h && h < max(l, r));
		}

		if (b % 2 == 0) {
			assert(h == H[b / 2]);
		} else {
			int l = H[b / 2];
			int r = H[b / 2 + 1];
			assert(min(l, r) < h && h < max(l, r));
		}

		//cerr << a << ' ' << b << ' ' << h << ' ' << d << '\n';

		// we can go to the next vertex over
		for (int na = a - 1 - (a % 2 == 0); na <= a + 1 + (a % 2 == 0); na++) {
			for (int nb = b - 1 - (b % 2 == 0); nb <= b + 1 + (b % 2 == 0); nb++) {
				if (!(0 <= na && na <= 2 * N && 0 <= nb && nb <= 2 * N)) continue;
				if (na == a && nb == b) continue;
				if (vis[na][nb]) continue;

				if (na % 2 == 1 && nb % 2 == 1) continue;

				int nh = (na % 2 == 0) ? H[na / 2] : H[nb / 2];
				if (na % 2 == 0) {
					if (nh != H[na / 2]) continue;
				} else {
					int l = H[na / 2];
					int r = H[na / 2 + 1];
					if (!(min(l, r) < nh && nh < max(l, r))) {
						continue;
					}
				}
				if (nb % 2 == 0) {
					if (nh != H[nb / 2]) continue;
				} else {
					int l = H[nb / 2];
					int r = H[nb / 2 + 1];
					if (!(min(l, r) < nh && nh < max(l, r))) {
						continue;
					}
				}

				double nd = d;

				{
					// add stuff to d
					int l = min(a, na);
					if (l % 2 == 1) l--;
					int r = max(a, na);
					if (r % 2 == 1) r++;
					assert(r - l == 0 || r - l == 2);
					if (r - l == 2) {
						if (H[l/2] == H[r/2]) {
							assert(min(a, na) == l && max(a, na) == r);
							nd += 1.;
						} else {
							int dh = abs(H[l/2] - H[r/2]);

							nd += sqrt(dh * dh + 1) / double(dh) * double(abs(h - nh));
						}
					}
				}

				{
					// add stuff to d
					int l = min(b, nb);
					if (l % 2 == 1) l--;
					int r = max(b, nb);
					if (r % 2 == 1) r++;
					assert(r - l == 0 || r - l == 2);

					if (r - l == 2) {
						if (H[l/2] == H[r/2]) {
							assert(min(b, nb) == l && max(b, nb) == r);
							nd += 1.;
						} else {
							int dh = abs(H[l/2] - H[r/2]);

							nd += sqrt(dh * dh + 1) / double(dh) * double(abs(h - nh));
						}
					}
				}

				//cerr << a << ' ' << b << ' ' << h << ' ' << d << ' ' << na << ' ' << nb << ' ' << nh << ' ' << nd << '\n';

				if (nd < dist[na][nb]) {
					dist[na][nb] = nd;
					pq.emplace(nd, pair<int, int>(na, nb));
				}
			}
		}
	}
	assert(false);
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);

	int T; cin >> T;
	while (T--) {
		double ans = go();
		cout << fixed << setprecision(15) << ans << '\n';
	}


	return 0;
}

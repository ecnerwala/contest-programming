#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		string S; cin >> S;
		int N = int(S.size());
		vector<array<int, 10>> nxt(N+1);
		nxt[N].fill(N);
		for (int i = N-1; i >= 0; i--) {
			nxt[i] = nxt[i+1];
			nxt[i][S[i]-'0'] = i;
		}

		int K; cin >> K;
		array<int, 10> extra_cnt; extra_cnt.fill(0);
		string resp; resp.reserve(N);
		for (int i = 0; i < N; i++) {
			for (int d = 1; true; d++) {
				assert(d <= 9);
				if (nxt[i][d] <= i + K) {
					while (i < N && S[i]-'0' != d) {
						extra_cnt[S[i]-'0']++;
						K--;
						i++;
					}
					assert(K >= 0);
					if (i < N) resp += S[i];
					break;
				}
			}
		}
		for (int d = 1; d < 10; d++) {
			for (int z = 0; z < extra_cnt[d]; z++) {
				resp += char('0' + d);
			}
		}
		cout << resp << '\n';
	}

	return 0;
}

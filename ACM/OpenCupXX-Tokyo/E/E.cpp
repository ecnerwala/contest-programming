#include<bits/stdc++.h>
using namespace std;


int main() {
	ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
	int T; cin >> T;

	while (T--) {
		int64_t N, S; int K; cin >> N >> S >> K;
		vector<int> A(K); for (auto& a : A) cin >> a;

		using bs = bitset<int(2.1e5)>;

		bs cur; cur[0] = 1;
		for (; S || N; N >>= 1, S >>= 1) {
			bs nxt;
			if (N & 1) for (int a : A) nxt ^= cur << a;
			else nxt = cur;

			nxt >>= (S & 1);

			cur.reset();
			for (int i = 0; 2*i < int(nxt.size()); i++) {
				cur[i] = nxt[2*i];
			}
		}

		cout << (cur[0] ? 1 : 0) << '\n';
	}

	return 0;
}

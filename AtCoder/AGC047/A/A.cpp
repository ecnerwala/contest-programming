#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int N; cin >> N;
	int cnt[20][20];
	memset(cnt, 0, sizeof(cnt));
	int64_t ans = 0;
	for (int z = 0; z < N; z++) {
		string a; cin >> a;
		if (a.find('.') == string::npos) a += '.';
		int dot_ind = int(a.find('.'));
		while (int(a.size()) - dot_ind - 1 < 9) {
			a += '0';
		}
		assert(dot_ind == int(a.size()) - 9 - 1);
		a.erase(a.begin()+dot_ind);
		int64_t v = stoll(a);
		assert(v);
		int num_2 = -9;
		int num_5 = -9;
		for (; v % 2 == 0; v /= 2) num_2++;
		for (; v % 5 == 0; v /= 5) num_5++;
		num_2 = min(num_2, 9);
		num_5 = min(num_5, 9);

		for (int i = -num_2; i <= 9; i++) {
			for (int j = -num_5; j <= 9; j++) {
				ans += cnt[i+9][j+9];
			}
		}
		cnt[num_2+9][num_5+9]++;
	}
	cout << ans << '\n';

	return 0;
}

#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, B; cin >> N >> B;
	string cur_lo(B+N, '1');

	deque<int> cur_alive;
	for (int i = 1; i <= N; i++) cur_alive.push_back(i);
	int b = 0;
	cur_lo[0] = '0';
	while (b - int(cur_alive.size()) < B-1) {
		cout << cur_alive.front() << ' ' << cur_lo.substr(0, B) << '\n' << flush;
		string resp; cin >> resp;
		if (resp == "yes") {
			while (cur_lo[b] == '1') {
				b--;
				cur_alive.pop_back();
			}
			assert(cur_lo[b] == '0');
			cur_lo[b] = '1';
		} else if (resp == "no") {
			b++;
			assert(cur_lo[b] == '1');
			cur_lo[b] = '0';
			cur_alive.push_back(cur_alive.front());
			cur_alive.pop_front();
		} else assert(false);
	}

	cout << 0 << ' ' << cur_lo.substr(0, B) << '\n' << flush;

	return 0;
}

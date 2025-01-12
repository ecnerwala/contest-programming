#include <bits/stdc++.h>

template <typename T> void setmax(T& a, T b) { if (b > a) a = b; }

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		struct person_t { int a, b, i; };
		std::vector<person_t> people(3*N);
		for (int i = 0; i < 3*N; i++) {
			auto& p = people[i];
			cin >> p.a >> p.b;
			p.i = i;
		}

		std::vector<std::vector<int>> goods(3);
		for (int z = 0; z < 3; z++) {
			goods[z].reserve(N);
			int max_A = 0;
			for (auto [a, b, i] : people) {
				setmax(max_A, a);
			}
			std::sort(people.begin(), people.end(), [&](auto a, auto b) -> bool {
				return std::make_pair(a.b >= max_A, a.a) <= std::make_pair(b.b >= max_A, b.a);
			});
			if (people[int(people.size()) - N].b < max_A) {
				goto bad;
			}
			for (int a = 0; a < N; a++) {
				goods[z].push_back(people.back().i);
				people.pop_back();
			}
		}

		cout << "Yes" << '\n';
		for (int z = 0; z < 3; z++) {
			for (int i = 0; i < N; i++) {
				cout << goods[z][i]+1 << " \n"[i+1==N];
			}
		}

		continue;
bad:
		cout << "No" << '\n';
	}

	return 0;
}

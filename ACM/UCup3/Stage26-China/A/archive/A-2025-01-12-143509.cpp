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

		std::set<std::pair<int, int>> alive_As;
		for (auto p : people) {
			alive_As.insert({p.a, p.i});
		}
		std::sort(people.begin(), people.end(), [&](person_t a, person_t b) { return a.b < b.b; });
		std::set<std::pair<int, int>> big_valid;

		std::vector<std::array<int, 3>> goods; goods.reserve(N);
		for (int z = 0; z < N; z++) {
			int max_A = alive_As.rbegin()->first;
			while (!people.empty() && people.back().b >= max_A) {
				big_valid.insert({people.back().a, people.back().i});
				people.pop_back();
			}
			if (big_valid.size() < 3) goto bad;
			std::array<int, 3> cur;
			for (int a = 0; a < 3; a++) {
				auto v = *big_valid.rbegin();
				cur[a] = v.second;
				alive_As.erase(v);
				big_valid.erase(v);
			}
			goods.push_back(cur);
		}

		cout << "Yes" << '\n';
		for (int z = 0; z < N; z++) {
			for (int i = 0; i < 3; i++) {
				cout << goods[z][i]+1 << " \n"[i+1==3];
			}
		}

		continue;
bad:
		cout << "No" << '\n';
	}

	return 0;
}

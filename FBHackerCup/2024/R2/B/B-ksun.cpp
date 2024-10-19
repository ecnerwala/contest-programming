#include <bits/stdc++.h>
using namespace std;

namespace std {

template<class Fun>
class y_combinator_result {
	Fun fun_;
public:
	template<class T>
	explicit y_combinator_result(T &&fun): fun_(std::forward<T>(fun)) {}

	template<class ...Args>
	decltype(auto) operator()(Args &&...args) {
		return fun_(std::ref(*this), std::forward<Args>(args)...);
	}
};

template<class Fun>
decltype(auto) y_combinator(Fun &&fun) {
	return y_combinator_result<std::decay_t<Fun>>(std::forward<Fun>(fun));
}

} // namespace std
void solve(int t){
	cout << "Case #" << t << ": ";
	vector<string> rows(6);
	for(int i = 0; i < 6; i++){
		cin >> rows[i];
	}
	reverse(rows.begin(), rows.end());

	int R = 6;
	int C = 7;
	vector<vector<pair<int,int> > > win_patterns;
	vector<pair<int,int> > del = {{1, 0}, {0, 1}, {1, 1}, {1, -1}};
	for(int r = 0; r < R; r++){
		for(int c = 0; c < C; c++){
			for(auto [dr, dc] : del){
				int er = r + 3 * dr;
				int ec = c + 3 * dc;
				if(er < 0 || er >= R || ec < 0 || ec >= C) continue;
				vector<pair<int,int> > pattern;
				for(int i = 0; i <= 3; i++){
					pattern.push_back({r + i * dr, c + i * dc});
				}
				win_patterns.push_back(pattern);
			}
		}
	}

	auto has_win = [&](vector<int> v) -> int {
		int winners = 0;
		for(auto pattern : win_patterns){
			bool ok_C = true;
			bool ok_F = true;
			for(auto [r, c] : pattern){
				if(v[c] <= r){
					ok_C = ok_F = false;
				}
				if(rows[r][c] != 'C') ok_C = false;
				if(rows[r][c] != 'F') ok_F = false;
			}
			if(ok_C) winners |= 1;
			if(ok_F) winners |= 2;
		}
		return winners;
	};

	auto process = [&](vector<int> v){
		int st = 0;
		for(int x : v){
			st = st * 7 + x;
		}
		return st;
	};

	vector<pair<int,int> > saved(823543, {-1, -1});
	// whether the current grid is valid, who can win first
	auto [g_valid, ans] = y_combinator(
		[&](auto self, vector<int> v) -> pair<int, int> {
			int msk = process(v);
			if(saved[msk].first == -1){
				saved[msk] = [&]() -> pair<int, int> {
					int turn = 0;
					for(int x : v) turn += x;
					int cur_has_win = has_win(v);
					if(turn == R * C){
						return {1, has_win(v)};
					}
					int valid = 0;
					int dp_has_win = 0;
					for(int c = 0; c < C; c++){
						if(v[c] < R && rows[v[c]][c] == "CF"[turn % 2]){
							vector<int> nv = v;
							nv[c]++;
							auto [x, y] = self(nv);
							if(!x) continue;
							valid = 1;
							dp_has_win |= y;
						}
					}
					if(!valid) return {0, 0};
					if(cur_has_win) return {1, cur_has_win};
					return {1, dp_has_win};
				}();
			}
			return saved[msk];
		}
	)(vector<int>(C, 0));
	cout << "0CF?"[ans] << '\n';
}

int main(){
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int T;
	cin >> T;
	for(int t = 1; t <= T; t++){
		solve(t);
		cout << flush;
	}
}

#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	using i128 = __int128_t;
	auto to_string = [&](i128 a) -> string {
		string res;
		for (; a; a /= 10) {
			res += char('0' + int(a % 10));
		}
		if (res.empty()) res += "0";
		reverse(res.begin(), res.end());
		return res;
	};

	const int V = 122;
	vector<vector<i128>> choose(V+1);
	for (int v = 0; v <= V; v++) {
		choose[v].resize(v+1);
		choose[v][0] = choose[v][v] = 1;
		for (int i = 1; i < v; i++) {
			choose[v][i] = choose[v-1][i-1] + choose[v-1][i];
		}
	}

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		cout << "Case #" << case_num << ": ";

		int P, Q; cin >> P >> Q;
		vector<array<bool, 3>> ans(Q);
		array<int, 3> scores;
		for (int z = 0; z < 3; z++) {
			if (z < P) {
				string s; cin >> s;
				for (int q = 0; q < Q; q++) {
					ans[q][z] = (s[q] == 'T');
				}
				cin >> scores[z];
			} else {
				for (int q = 0; q < Q; q++) {
					ans[q][z] = ans[q][0];
				}
				scores[z] = scores[0];
			}
		}

		// We have 3 people with 
		array<int, 4> qcnt{0,0,0,0};
		vector<int> type(Q);
		for (int q = 0; q < Q; q++) {
			if (ans[q][0] == ans[q][1] && ans[q][1] == ans[q][2]) {
				type[q] = 3;
			} else {
				type[q] = int(find(ans[q].begin(), ans[q].end(), ans[q][0] ^ ans[q][1] ^ ans[q][2]) - ans[q].begin());
			}
			qcnt[type[q]]++;
		}

		string best_string(Q, '-');
		i128 tot_val = 0;
		i128 ways = 0;

		array<i128, 4> vals{0,0,0,0};

		// a - b - c = scores[0] - d - qcnt[1] - qcnt[2]
		// a + b + c = 2 * (qcnt[0] + qcnt[1] + qcnt[2]) + 3 * d - (scores[0] + scores[1] + scores[2])
		for (int d = 0; d <= qcnt[3]; d++) {
			int ab_min_c = d + qcnt[0] + qcnt[1] - scores[2];
			int bc_min_a = d + qcnt[1] + qcnt[2] - scores[0];
			int ca_min_b = d + qcnt[2] + qcnt[0] - scores[1];
			int a = (ab_min_c + ca_min_b);
			int b = (bc_min_a + ab_min_c);
			int c = (ca_min_b + bc_min_a);
			if ((a & 1) || (b & 1) || (c & 1)) continue;
			a /= 2, b /= 2, c /= 2;
			if (a < 0 || a > qcnt[0]) continue;
			if (b < 0 || b > qcnt[1]) continue;
			if (c < 0 || c > qcnt[2]) continue;

			i128 cur_ways = choose[qcnt[0]][a] * choose[qcnt[1]][b] * choose[qcnt[2]][c] * choose[qcnt[3]][d];

			ways += cur_ways;
			vals[0] += cur_ways * a;
			vals[1] += cur_ways * b;
			vals[2] += cur_ways * c;
			vals[3] += cur_ways * d;
		}

		array<bool, 4> should_flip;
		for (int z = 0; z <= 3; z++) {
			should_flip[z] = ways * qcnt[z] - vals[z] > vals[z];
			if (should_flip[z]) {
				tot_val += ways * qcnt[z] - vals[z];
			} else {
				tot_val += vals[z];
			}
		}

		for (int q = 0; q < Q; q++) {
			best_string[q] = "FT"[ans[q][min(type[q], 2)] ^ should_flip[type[q]]];
		}

		{
			i128 a = tot_val, b = ways;
			while (a) {
				b %= a;
				std::swap(a, b);
			}
			i128 g = b;
			tot_val /= g;
			ways /= g;
		}
		cout << best_string << ' ' << to_string(tot_val) << '/' << to_string(ways) << '\n';
	}

	return 0;
}

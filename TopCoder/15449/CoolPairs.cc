#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class CoolPairs {

public:

	vector <string> smallest(vector <string> firstname, vector <string> lastname, vector <int> weight) {
		int N = int(firstname.size());
		for (int i = 0; i < N; i++) weight[i*N + i] = 0; // convenience

		vector<string> cnds; cnds.push_back("a");
		vector<int> stars;
		for (int i = 0; i < N; i++) {
			{
				const string& s = firstname[i];
				if (s == "*") {
					stars.push_back(i);
				} else {
					cnds.push_back(s);
				}
			}
			{
				const string& s = lastname[i];
				if (s == "*") {
					stars.push_back(~i);
				} else {
					cnds.push_back(s);
				}
			}
		}

		reverse(stars.begin(), stars.end());

		sort(cnds.begin(), cnds.end());
		cnds.resize(unique(cnds.begin(), cnds.end()) - cnds.begin());
		reverse(cnds.begin(), cnds.end());

		int base_score = 0;
		for (int i = 0; i < N; i++) {
			if (lastname[i] == "*") continue;
			for (int j = 0; j < N; j++) {
				if (firstname[j] == "*") continue;
				if (lastname[i] == firstname[j]) {
					base_score += weight[i*N+j];
				}
			}
		}
		int L = int(stars.size());
		using state = __int128_t;
		vector<pair<int, __int128_t>> score(1 << L, {-1, -1});

		for (int z = 0; z < int(cnds.size()); z++) {
			const string& cnd = cnds[z];

			vector<int> first_eq; first_eq.reserve(N);
			for (int i = 0; i < N; i++) if (firstname[i] == cnd) first_eq.push_back(i);
			vector<int> last_eq; last_eq.reserve(N);
			for (int i = 0; i < N; i++) if (lastname[i] == cnd) last_eq.push_back(i);

			vector<int> star_weight(L);
			for (int i = 0; i < L; i++) {
				if (stars[i] < 0) {
					int a = ~stars[i];
					for (int b : first_eq) star_weight[i] += weight[a * N + b];
				} else {
					int b = stars[i];
					for (int a : last_eq) star_weight[i] += weight[a * N + b];
				}
			}

			for (int msk = 0; msk < (1 << L); msk++) {
				int cur_score = 0;
				state v = 0;
				for (int i = 0; i < L; i++) {
					if (msk & (1 << i)) {
						cur_score += star_weight[i];
						v |= state(z) << (i*7);
					}
				}
				score[msk] = max(score[msk], pair<int, state>(cur_score, v));
			}
		}

		// extra value for star w/ star
		for (int msk = 0; msk < (1 << L); msk++) {
			for (int i = 0; i < L; i++) {
				if (stars[i] < 0 && (msk & (1 << i))) {
					for (int j = 0; j < L; j++) {
						if (stars[j] >= 0 && (msk & (1 << j))) {
							score[msk].first += weight[(~stars[i]) * N + stars[j]];
						}
					}
				}
			}
		}

		vector<pair<int, state>> dp(1 << L);
		dp[0].first = 0;
		dp[0].second = 0;
		for (int msk = 1; msk < (1 << L); msk++) {
			dp[msk].first = -1;
			for (int v = msk; v > 0; v = (v-1) & msk) {
				auto cnd = dp[msk ^ v];
				cnd.first += score[v].first;
				cnd.second ^= score[v].second;
				dp[msk] = max(dp[msk], cnd);
			}
		}
		vector<string> res(2*N);
		for (int i = 0; i < N; i++) res[2*i] = firstname[i], res[2*i+1] = lastname[i];
		for (int i = 0; i < L; i++) {
			int c = (dp[(1<<L)-1].second >> (i*7)) & 127;
			if (stars[i] < 0) {
				int b = ~stars[i];
				res[2*b+1] = cnds[c];
			} else {
				int a = stars[i];
				res[2*a] = cnds[c];
			}
		}
		return res;
	}
};

void my_tests() {
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::CoolPairs;

// vim:ft=cpp

#include<bits/stdc++.h>
using namespace std;

struct state {
	int lastRock;
	int num1, num2, num3;
	friend bool operator < (const state& a, const state& b) {
		return tie(a.lastRock, a.num1, a.num2, a.num3) < tie(b.lastRock, b.num1, b.num2, b.num3);
	}
};

const int MAXN = 1.1e5;
int N, K;
int G1, G2, G3;
string S;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);

	cin >> N >> K >> G1 >> G2 >> G3 >> S;
	S = "##" + S + "##";

	set<state> states;
	states.insert(state{0, 0, 0, 0});
	for (int i = 1; i+1 < int(S.size()); i++) {
		if (S[i] == '#') {
			set<state> nstates;
			if (S[i-1] == '.' && S[i+1] == '.') {
				// take the 3
				for (auto s : states) {
					assert(i > s.lastRock);
					if (s.lastRock == i-1) continue;
					int l = i - 1 - s.lastRock - 1;
					s.num1 += l % 2;
					s.num2 += l / 2;
					s.num3 ++;
					s.lastRock = i+1;
					nstates.insert(s);
				}
			}
			{
				for (auto s : states) {
					assert(i > s.lastRock);
					int l = i - s.lastRock - 1;
					s.num1 += l % 2;
					s.num2 += l / 2;
					s.lastRock = i;
					nstates.insert(s);
				}
			}
			states = std::move(nstates);
		} else if (S[i] == '.') {
			// do nothing
		} else assert(false);
	}

	int ans = 0;
	for (auto s : states) {
		assert(s.lastRock == int(S.size()) - 2);
		if (s.num1 > K) {
			s.num1 = K;
		}
		if (G1 + G1 >= G2) {
			int v = min(s.num2, (K - s.num1) / 2);
			s.num1 += 2 * v;
			s.num2 -= v;
			if (G1 >= G2 && s.num1 < K && s.num2 > 0) {
				assert(s.num1 == K-1);
				s.num1 ++;
				s.num2 --;
			}
		}
		int cnd = s.num1 * G1 + s.num2 * G2 + s.num3 * G3;
		ans = max(ans, cnd);
	}
	cout << ans << '\n';

	return 0;
}

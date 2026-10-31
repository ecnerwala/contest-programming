#include <bits/stdc++.h>

std::map<char, std::string> morse = {
	{'A', ".-"},
	{'B', "-..."},
	{'C', "-.-."},
	{'D', "-.."},
	{'E', "."},
	{'F', "..-."},
	{'G', "--."},
	{'H', "...."},
	{'I', ".."},
	{'J', ".---"},
	{'K', "-.-"},
	{'L', ".-.."},
	{'M', "--"},
	{'N', "-."},
	{'O', "---"},
	{'P', ".--."},
	{'Q', "--.-"},
	{'R', ".-."},
	{'S', "..."},
	{'T', "-"},
	{'U', "..-"},
	{'V', "...-"},
	{'W', ".--"},
	{'X', "-..-"},
	{'Y', "-.--"},
	{'Z', "--.."},
	{'0', "-----"},
	{'1', ".----"},
	{'2', "..---"},
	{'3', "...--"},
	{'4', "....-"},
	{'5', "....."},
	{'6', "-...."},
	{'7', "--..."},
	{'8', "---.."},
	{'9', "----."}
};


int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	std::string S; cin >> S;
	std::string morse_S;
	for (char c : S) {
		morse_S += morse[c];
	}

	using state_t = std::array<std::string, 2>;
	std::map<state_t, std::pair<char, state_t>> prv;
	std::vector<state_t> q;
	prv[{morse_S, ""}] = {'?', {}};
	q.push_back({morse_S, ""});

	int idx = 0;

	for (int d = 0; true; d++) {
		for (int q_sz = int(q.size()); idx < q_sz; idx++) {
			auto cur_state = q[idx];
			//cerr << d << ' ' << idx << ' ' << cur_state[0] << ' ' << cur_state[1] << '\n';
			{
				// first, check if this is a palindrome
				bool is_done = true;
				std::string a = cur_state[0] + cur_state[1];
				for (int i = 0; i < int(a.size()); i++) {
					is_done = is_done && (a[i] == a[int(a.size()) - 1 - i]);
				}

				if (is_done) {
					std::string ans;
					while (true) {
						auto [c, prv_state] = prv[cur_state];
						if (c == '?') {
							break;
						}
						assert(prv_state[0].empty() != prv_state[1].empty());
						if (prv_state[0].empty()) {
							ans = c + ans;
						} else {
							ans = ans + c;
						}
						cur_state = prv_state;
					}
					assert(int(ans.size()) == d);
					// we could recheck S + ans, but whatever

					cout << d << ' ' << ans << '\n';
					exit(0);
				}
			}

			assert(cur_state[0].empty() != cur_state[1].empty());
			// now, try to append/prepend every character
			for (auto [c, s] : morse) {
				auto nxt_state = cur_state;
				if (cur_state[0].empty()) nxt_state[0] = s;
				else nxt_state[1] = s;
				// now trim from the outside
				bool is_good = true;
				while (!nxt_state[0].empty() && !nxt_state[1].empty()) {
					if (nxt_state[0].front() != nxt_state[1].back()) {
						is_good = false;
						break;
					}
					nxt_state[0].erase(nxt_state[0].begin());
					nxt_state[1].pop_back();
				}
				if (!is_good) continue;
				if (prv.insert({nxt_state, {c, cur_state}}).second) {
					q.push_back(nxt_state);
				}
			}
		}
	}

	return 0;
}

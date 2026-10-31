#include <bits/stdc++.h>

int signum(int a) {
	return (a > 0) - (a < 0);
}

int get_winding_number(int X0, int Y0, const std::string& S) {
	int x = X0, y = Y0;
	int res = 0;
	for (char c : S) {
		int ox = x, oy = y;
		if (c == 'N') y++;
		else if (c == 'S') y--;
		else if (c == 'E') x++;
		else if (c == 'W') x--;
		if (ox == x && x > 0) {
			if (oy == 1 && y == 0) res++;
			else if (oy == 0 && y == 1) res--;
		}
	}
	return res;
}

char oppo(char a) {
	if (a == 'N') return 'S';
	else if (a == 'S') return 'N';
	else if (a == 'E') return 'W';
	else if (a == 'W') return 'E';
	else assert(false);
}

void go() {
	using namespace std;
	int X0, Y0; cin >> X0 >> Y0;
	array<int, 2> L; cin >> L[0] >> L[1];
	array<string, 2> T; cin >> T[0] >> T[1];
	if (get_winding_number(X0, Y0, T[0]) != get_winding_number(X0, Y0, T[1])) {
		cout << "NO" << '\n';
		return;
	}

	array<string, 2> res;
	//array<string, 2> final_S;
	for (int z = 0; z < 2; z++) {
		string S = T[z];
		{
			int x = X0, y = Y0;
			while (!(x == 1 && y == 1)) {
				char to_prepend, to_append;
				int dx = signum(1-x);
				int dy = signum(1-y);
				if (dx != 0 && !(x + dx == 0 && y == 0)) {
					// change x
					if (dx == +1) to_append = 'E', to_prepend = 'W';
					else if (dx == -1) to_append = 'W', to_prepend = 'E';
					else assert(false);
					x += dx;
				} else if (dy != 0 && !(y + dy == 0 && x == 0)) {
					if (dy == +1) to_append = 'N', to_prepend = 'S';
					else if (dy == -1) to_append = 'S', to_prepend = 'N';
					else assert(false);
					y += dy;
				} else assert(false);

				if (z == 0) {
					res[z] += to_append;
					res[z] += '-';
				} else if (z == 1) {
					// To get from to_prepend + S + to_append, we should walk then R; so we'll append the reverse
					res[z].append(1, 'R');
					res[z].append(int(S.size())+1, '-');
				} else assert(false);
				S = to_prepend + S + to_append;
			}
		}

		// Now, we're staring at 1,1
		while (true) {
			int x = 1, y = 1;
			for (int i = 0; i+1 < int(S.size()); i++) {
				int j = i+1;

				int cx = x, cy = y;
				while (true) {
					if (j == int(S.size())) goto bad_cycle;
					if (S[j] == S[i]) goto bad_cycle;
					if (S[j] == oppo(S[i])) {
						goto good_cycle;
					}
					if (j > i+1 && S[j] != S[j-1]) {
						assert(S[j] == oppo(S[j-1]));
						i = j-1;
						goto good_cycle;
					}
					if (S[j] == 'W') cx--;
					else if (S[j] == 'E') cx++;
					else if (S[j] == 'N') cy++;
					else if (S[j] == 'S') cy--;
					else assert(false);
					if ((cx == 0 && cy == 0)) goto bad_cycle;
					j++;
				}
good_cycle:
				assert(j-i >= 1);
				assert(S[j] == oppo(S[i]));
				// now we just delete i and j
				if (z == 0) {
					int dist = j-i-1;
					res[z].append(i, '-');
					for (int a = 0; a < dist; a++) res[z] += "C-";
					res[z] += 'R';
					res[z].append(int(S.size())-j-1, '-');
				} else if (z == 1) {
					int dist = j-i-1;
					res[z].append(int(S.size()) - j, '-');
					for (int a = 0; a < dist; a++) res[z] += "-C";
					res[z] += '-';
					res[z] += S[i];
					res[z].append(i, '-');
				} else assert(false);
				S = S.substr(0, i) + S.substr(i+1, j-i-1) + S.substr(j+1);
				goto perimeter_shrunk;
bad_cycle:
				if (S[i] == 'W') x--;
				else if (S[i] == 'E') x++;
				else if (S[i] == 'N') y++;
				else if (S[i] == 'S') y--;
				else assert(false);
				assert(!(x == 0 && y == 0));
			}
			break;
perimeter_shrunk:;
		}
		//final_S[z] = S;
	}

	//if (final_S[0] != final_S[1]) { cout << "NO" << '\n'; return; }

	cout << "YES" << '\n';
	reverse(res[1].begin(), res[1].end());
	cout << res[0] << res[1] << '\n';

	if constexpr (false) {
		int x = X0, y = Y0;
		string S = T[0];
		for (char c : res[0] + res[1]) {
			if (c == 'C') {
				assert(int(S.size()) >= 2);
				swap(S[0], S[1]);
			} else if (c == '-') {
				if (S[0] == 'N') y++;
				else if (S[0] == 'S') y--;
				else if (S[0] == 'E') x++;
				else if (S[0] == 'W') x--;
				else assert(false);
				assert(!(x == 0 && y == 0));
				S = S.substr(1) + S[0];
			} else if (c == 'R') {
				assert(int(S.size()) >= 2);
				assert(S[0] == oppo(S[1]));
				S = S.substr(2);
			} else if (c == 'N' || c == 'S' || c == 'E' || c == 'W') {
				S = c + (oppo(c) + S);
			} else assert(false);
		}
		assert(x == X0 && y == Y0 && S == T[1]);
	}
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		go();
	}

	return 0;
}

#include <bits/stdc++.h>

void solve(int X, int Y, int Z, int P) {
	const int Xorig = X, Yorig = Y, Zorig = Z;
	using namespace std;
	assert(X >= 2 && Y >= 2 && Z >= 2);
	string A, B, C;
	if (P == 0) {
		// Maximize sum
		A = "0" + string(Z-1, '0') + string(Y-1, '0') + string(X-1, '1') + "1";
		B = "0" + string(Z-1, '0') + string(Y-1, '1') + string(X-1, '0') + "1";
		C = "0" + string(Z-1, '1') + string(Y-1, '0') + string(X-1, '0') + "1";
	} else if (P == 1) {
		// Minimize sum
		// We roughly just want 10-9-9-9-9-9-9-9-extras
		// But sometimes this is hard to attain...
		// If extras is even, then we can split the previous 9 for no cost
		// If extras = 1, the
		[&]() -> void {
			int tot = X+Y+Z;


			// can never carry, just spread them out
			if (tot < 10) {
				A = "0" + to_string(X-2) + "011";
				B = "0" + to_string(Y-2) + "101";
				C = "0" + to_string(Z-2) + "110";

				return;
			}

			int num9 = (tot - 10) / 9;
			int extras = (tot - 10) % 9;

			// 2 + 9 + 9 = 20
			// 2 + 11 + 16 = 10 + 9 + 9 + 1
			// 10 + 19
			//
			// 02
			// 88
			// 29
			//
			// 0002 0001

			vector<int> sums(num9 + 2, 9);
			sums[0] = 10;
			sums[num9 + 1] = extras;

			if (extras == 0) {
				sums[num9] -= 6;
				sums[num9 + 1] += 6;
			} else if (extras == 1) {
				// 10 + 9 + 9 + ... + 1 is impossible to satisfy triangle
				// 20 + 18
				if (tot - max({X,Y,Z}) <= 10) {
					sums[num9] -= 2;
					sums[num9 + 1] += 2;
				} else {
					if (tot == 20) {
						A = to_string(X);
						B = to_string(Y);
						C = to_string(Z);
						return;
					}
					sums.pop_back();
					sums.pop_back();
					sums.back() = 19;
				}
			} else if (extras == 2) {
				sums[num9] -= 4;
				sums[num9 + 1] += 4;
			} else if (extras == 3) {
				// this is fine,1,1,1
			} else if (extras == 4) {
				sums[num9] -= 2;
				sums[num9 + 1] += 2;
			} else if (extras == 5) {
				// this is fine,2,2,1
			} else if (extras == 6) {
				// this is fine,2,2,2
			} else if (extras == 7) {
				// this is fine, 2,2,3
			} else if (extras == 8) {
				sums[num9] -= 2;
				sums[num9 + 1] += 2;

				sums[num9 + 1] -= 6;
				sums.push_back(6);
			} else assert(false);

			//for (int s : sums) { cerr << s << ' '; } cerr << '\n';

			A = string(sums.size(), '0');
			B = string(sums.size(), '0');
			C = string(sums.size(), '0');

			if (sums.back() == 19) {
				int a = min(9, X-1);
				int b = min(9, Y-1);
				int c = min(9, Z-1);
				if (a+b+c < 19) {
					assert(a+b+c >= 18);
					if (a < 9) a++;
					else if (b < 9) b++;
					else if (c < 9) c++;
					else assert(false);
				} else {
					while (a+b+c > 19 && a > 1) a--;
					while (a+b+c > 19 && b > 1) b--;
					while (a+b+c > 19 && c > 1) c--;
				}
				assert(a+b+c == 19);
				A.back() = char('0' + a);
				B.back() = char('0' + b);
				C.back() = char('0' + c);
				X -= a, Y -= b, Z -= c;
			} else if (sums.back() == 3) {
				A.back() = '1';
				X--;
				B.back() = '1';
				Y--;
				C.back() = '1';
				Z--;
			} else if (sums.back() == 6) {
				assert(X + Y + Z > 6);
				// 10, 11, 3
				if (X > 3) {
					A.back() = '3';
					X -= 3;
					B.back() = '1';
					B.push_back('1');
					Y -= 2;
					C.back() = '0';
					C.push_back('1');
					Z -= 1;
				} else if (Y > 3) {
					A.back() = '1';
					A.push_back('1');
					X -= 2;
					B.back() = '3';
					Y -= 3;
					C.back() = '0';
					C.push_back('1');
					Z -= 1;
				} else if (Z > 3) {
					A.back() = '1';
					A.push_back('1');
					X -= 2;
					B.back() = '0';
					B.push_back('1');
					Y -= 1;
					C.back() = '3';
					Z -= 3;
				} else assert(false);
				assert(bool(X) + bool(Y) + bool(Z) >= 2);
			} else if (sums.back() == 5) {
				int a, b, c;
				a = 1, b = 2, c = 2;
				if (Y == 2 && Z == 2) {
					a = 2, b = 1;
				}
				A.back() = char(a + '0');
				X -= a;
				B.back() = char(b + '0');
				Y -= b;
				C.back() = char(c + '0');
				Z -= c;
				assert(bool(X) + bool(Y) + bool(Z) >= 2);
			} else if (sums.back() == 7) {
				assert(X + Y + Z >= 17);
				// 10, 11, 4
				if (X > 4) {
					A.back() = '4';
					X -= 4;
					B.back() = '1';
					B.push_back('1');
					Y -= 2;
					C.back() = '0';
					C.push_back('1');
					Z -= 1;
				} else if (Y > 4) {
					A.back() = '1';
					A.push_back('1');
					X -= 2;
					B.back() = '4';
					Y -= 4;
					C.back() = '0';
					C.push_back('1');
					Z -= 1;
				} else if (Z > 4) {
					A.back() = '1';
					A.push_back('1');
					X -= 2;
					B.back() = '0';
					B.push_back('1');
					Y -= 1;
					C.back() = '4';
					Z -= 4;
				} else assert(false);
				assert(bool(X) + bool(Y) + bool(Z) >= 2);
			} else assert(false);
			sums.back() = 0;

			int k = 0;
			// now, just round robin it
			for (int a = 0; a < int(sums.size()); a++) {
				while (sums[a] > 0) {
					if (k == 0) {
						if (X > 0) {
							assert(A[a] < '9');
							A[a]++;
							sums[a]--;
							X--;
						}
					} else if (k == 1) {
						if (Y > 0) {
							assert(B[a] < '9');
							B[a]++;
							sums[a]--;
							Y--;
						}
					} else if (k == 2) {
						if (Z > 0) {
							assert(C[a] < '9');
							C[a]++;
							sums[a]--;
							Z--;
						}
					} else assert(false);
					k++; k %= 3;
				}
			}
			assert(X == 0 && Y == 0 && Z == 0);
			//cerr << A << ' ' << B << ' ' << C << '\n';
		}();
	} else assert(false);

	while (!A.empty() && A.back() == '0') A.pop_back();
	while (!B.empty() && B.back() == '0') B.pop_back();
	while (!C.empty() && C.back() == '0') C.pop_back();

	auto add = [&](const string& a, const string& b) -> string {
		string res(max(a.size(), b.size()) + 1, '0');
		for (int i = 0; i < int(res.size()); i++) {
			if (i < int(a.size())) res[i] += char(a[i] - '0');
			if (i < int(b.size())) res[i] += char(b[i] - '0');
			if (res[i] > '9') {
				assert(i < int(res.size()));
				res[i] -= 10;
				res[i+1] += 1;
			}
		}
		while (!res.empty() && res.back() == '0') res.pop_back();
		return res;
	};

	auto sub = [&](const string& a, const string& b) -> string {
		string res(a.size(), '0');
		for (int i = 0; i < int(res.size()); i++) {
			if (i < int(a.size())) res[i] += char(a[i] - '0');
			if (i < int(b.size())) res[i] -= char(b[i] - '0');
			if (res[i] < '0') {
				assert(i < int(res.size()));
				res[i] += 10;
				res[i+1] -= 1;
			}
		}
		while (!res.empty() && res.back() == '0') res.pop_back();
		return res;
	};

	string tgt = add(A, add(B, C));
	for (int i = 0; i < int(tgt.size()); i++) {
		int v = tgt[i] - '0';
		if (v & 1) {
			assert(i > 0);
			tgt[i-1] += 5;
		}
		tgt[i] = char('0' + (v/2));
	}
	while (!tgt.empty() && tgt.back() == '0') tgt.pop_back();

	auto canonize = [&](string&& s) -> string {
		while (!s.empty() && s.back() == '0') s.pop_back();
		if (s.empty()) s.push_back('0');
		reverse(s.begin(), s.end());
		return std::move(s);
	};

	auto digit_sum = [&](const string& a) {
		int res = 0;
		for (char c : a) res += c - '0';
		return res;
	};

	assert(digit_sum(A) == Xorig);
	assert(digit_sum(B) == Yorig);
	assert(digit_sum(C) == Zorig);

	int ans = 0;
	for (char c : tgt) ans += c - '0';

	cout << ans << ' ' << canonize(sub(tgt, B)) << ' ' << canonize(sub(tgt, C)) << ' ' << canonize(sub(tgt, A)) << '\n';
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int T; cin >> T;
	while (T--) {
		int X, Y, Z, P; cin >> X >> Y >> Z >> P;
		solve(X,Y,Z,P);
	}

	return 0;
}

// Relabel A,B,C = A+B,B+C,C+A, so that S(A) = X, etc.

// We need A+B > C and cyclic, and target is (A + B + C) / 2 (must be even)
// Equivalently, A,B,C < tgt
// To divide by 2, you can "precarry"; in other words, S((A+B+C)/2) = (S(A+B+C) + 9 * # odd) / 2
// S((A+B+C)/2) = 9 * ((X + Y + Z)/9 - # carries + # odd) / 2
// If we're trying to maximize original digit sum, we want many carries

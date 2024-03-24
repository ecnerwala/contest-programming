#include <bits/stdc++.h>

static struct FastInput {
  static constexpr int BUF_SIZE = 1 << 20;
  char buf[BUF_SIZE];
  size_t chars_read = 0;
  size_t buf_pos = 0;
  FILE *in = stdin;
  char cur = 0;

  inline char get_char() {
    if (buf_pos >= chars_read) {
      chars_read = fread(buf, 1, BUF_SIZE, in);
      buf_pos = 0;
      buf[0] = (chars_read == 0 ? -1 : buf[0]);
    }
    return cur = buf[buf_pos++];
  }
 
  inline void tie(std::nullptr_t) {}

  inline explicit operator bool() {
    return cur != -1;
  }

  inline static bool is_blank(char c) {
    return c <= ' ';
  }

  inline bool skip_blanks() {
    while (is_blank(cur) && cur != -1) {
      get_char();
    }
    return cur != -1;
  }
 
  inline FastInput& operator>>(char& c) {
    skip_blanks();
    c = cur;
    return *this;
  }
  
  inline FastInput& operator>>(string& s) {
    if (skip_blanks()) {
      s.clear();
      do {
        s += cur;
      } while (!is_blank(get_char()));
    }
    return *this;
  }
 
  template <typename T>
  inline FastInput& read_integer(T& n) {
    // unsafe, doesn't check that characters are actually digits
    n = 0;
    if (skip_blanks()) {
      int sign = +1;
      if (cur == '-') {
        sign = -1;
        get_char();
      }
      do {
        n += n + (n << 3) + cur - '0';
      } while (!is_blank(get_char()));
      n *= sign;
    }
    return *this;
  }

  template <typename T>
  inline typename enable_if<is_integral<T>::value, FastInput&>::type operator>>(T& n) {
    return read_integer(n);
  }
  
  #if !defined(_WIN32) || defined(_WIN64)
  inline FastInput& operator>>(__int128& n) {
    return read_integer(n);
  }
  #endif

  template <typename T>
  inline typename enable_if<is_floating_point<T>::value, FastInput&>::type operator>>(T& n) {
    // not sure if really fast, for compatibility only
    n = 0;
    if (skip_blanks()) {
      string s;
      (*this) >> s;
      sscanf(s.c_str(), "%lf", &n);
    }
    return *this;
  }
} fast_input;

#define cin fast_input

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<std::pair<int, int>> cells(N);
		for (auto& [x, y] : cells) cin >> x >> y;
		/*
		int N = 1000000;
		std::vector<std::array<int, 2>> cells(N);
		for (int i = 0; i < N/2; i++) cells[i] = {0, i};
		for (int i = N/2; i < N; i++) cells[i] = {1+i-(N/2), 0};
		*/
		//cerr << "Start case " << N << '\n';
		auto solve = [&]() -> bool {
			std::sort(cells.begin(), cells.end());
			auto cell_it = cells.begin();

			// half open ranges
			std::vector<std::pair<std::pair<int, int>, int>> col;
			col.push_back({{0, 1}, 1});
			std::vector<std::pair<std::pair<int, int>, int>> ncol;
			for (int x = 0; true; x++) {
				if (col.empty()) {
					// cell is 0, so we can break
					break;
					if (cell_it == cells.end()) break;
					x = cell_it->first;
				}
				auto col_it = col.begin();
				ncol.clear();
				int vy = 0;
				for (int y = 0; true; y++) {
					if (vy == 0) {
						if (cell_it != cells.end() && cell_it->first == x) {
							int cell_y = cell_it->second;
							if (col_it != col.end() && col_it->first.first <= cell_y) {
								y = col_it->first.first;
							} else {
								y = cell_y;
							}
						} else if (col_it != col.end()) {
							y = col_it->first.first;
						} else {
							break;
						}
					}
					if (cell_it != cells.end() && cell_it->first == x && cell_it->second == y) {
						vy ++;
						++cell_it;
					}
					if (col_it != col.end() && col_it->first.first == y) {
						if (col_it->second == 1) {
							// a whole run of 1s, pretty much does nothing
							int yhi = col_it->first.second;
							if (cell_it != cells.end() && cell_it->first == x && cell_it->second < yhi) {
								yhi = cell_it->second;
								col_it->first.first = yhi;
							} else {
								++col_it;
							}
							assert(yhi > y);
							//cerr << x << ' ' << y << '-' << yhi << ' ' << vy+1 << '\n';
							if (vy > 0) {
								if (vy == 1) {
									if (vy == 1 && !ncol.empty() && ncol.back().second == 1 && ncol.back().first.second == y) {
										ncol.back().first.second = yhi;
									} else {
										ncol.push_back({{y, yhi}, vy});
									}
								} else {
									assert(vy >= 2);
									if (vy >= 3) return false;
									if (yhi - y >= 3) {
										return false;
									}
									for (int a = y; a < yhi; a++) {
										ncol.push_back({{a, a+1}, vy});
									}
								}
							}
							y = yhi - 1;
							continue;
						} else {
							assert(col_it->first.second == y+1);
							vy += col_it->second;
							++col_it;
						}
					}
					//cerr << x << ' ' << y << ' ' << vy << '\n';

					assert(vy > 0);
					if (vy >= 4) return false;
					if (vy > 0) {
						vy--;
					}
					if (vy > 0) {
						if (vy == 1 && !ncol.empty() && ncol.back().second == 1 && ncol.back().first.second == y) {
							ncol.back().first.second = y+1;
						} else {
							ncol.push_back({{y, y+1}, vy});
						}
					}
				}
				std::swap(col, ncol);
			}
			return true;
		};
		bool ans = solve();
		cout << (ans ? "YES" : "NO") << '\n';
	}

	return 0;
}

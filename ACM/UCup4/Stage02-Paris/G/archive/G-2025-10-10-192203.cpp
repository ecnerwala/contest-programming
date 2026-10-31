#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	std::vector<std::string> lines;
	{
		std::string line;
		while (std::getline(std::cin, line)) {
			lines.push_back(line);
		}
	}

	int ans = 0;
	for (int i = 0; i < int(lines.size()); i++) {
		// check if i is a valid starting line
		if (int(lines[i].size()) <= 7) continue;
		if ((int(lines[i].size()) - 7) % 4) continue;
		std::string S = lines[i].substr(int(lines[i].size()) - (int(lines[i].size()) - 7) / 4);
		bool good = true;
		for (char c : S) {
			if ('a' <= c && c <= 'z') continue;
			else good = false;
		}
		if (!good) continue;

		auto cnd = S + ", " + S + " " + S + "ity " + S;
		auto cnd2 = "i said " + cnd;
		int len = 0;
		for (int j = i; j+1 < int(lines.size()) && lines[j] == cnd && lines[j+1] == cnd2; j += 2) {
			len += int(lines[j].size()) + 1;
			len += int(lines[j+1].size()) + 1;
		}
		ans = std::max(ans, len);
	}
	if (ans == 0) ans = -1;
	cout << ans << '\n';

	return 0;
}

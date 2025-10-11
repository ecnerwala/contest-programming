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
	cerr << lines.size() << '\n';
	for (auto& line : lines) std::cerr << line << '\n';

	for (int i = 0; i < int(lines.size()); i++) {
		// check if i is a valid starting line
		int(lines[i].size()) - 7;
	}

	return 0;
}

#include <bits/stdc++.h>

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

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int F; cin >> F;
	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::map<std::string, std::vector<bool>> funcs;
	for (int i = 0; i < F; i++) {
		string s; getline(cin, s);

		assert(s.substr(0, 3) == "fn ");
		int st = int(s.find("("));
		string name = s.substr(3, st - 3);
		bool is_ref = name[0] == '&';
		if (is_ref) name = name.substr(1);
		assert(s.back() == ')');
		string args = s.substr(st+1, s.size() - st - 2);
		//cerr << name << ' ' << is_ref << ' ' << args << '\n';
		std::vector<bool> arg_list;
		if (!args.empty()) {
			arg_list.push_back(false);
			for (char c : args) {
				if (c == '&') arg_list.back() = true;
				else if (c == ',') arg_list.push_back(false);
			}
		}
		arg_list.push_back(is_ref);
		funcs[name] = arg_list;
	}

	int L; cin >> L;
	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	struct var_t {
		bool is_ref = false;
		int var_id = -1;
	};
	std::map<std::string, var_t> vars;

	std::set<int> moved;

	for (int line = 1; line <= L; line++) {
		auto error = [&]() {
			cout << "Error in line " << line << '\n';
			exit(0);
		};

		string var_name; cin >> var_name;

		std::set<int> referenced;

		string mid; cin >> mid;
		assert(mid == ":=");
		cin >> ws;
		string expr; getline(cin, expr);

		//cerr << var_name << ' ' << expr << '\n';

		var_t new_var;
		{
			int idx = 0;
			new_var = std::y_combinator([&](auto self) -> var_t {
				assert(idx < int(expr.size()));
				bool is_ref = false;
				if (expr[idx] == '&') {
					// must be a variable
					is_ref = true;
					idx++;
				}
				string ident;
				while (idx < int(expr.size()) && 'a' <= expr[idx] && expr[idx] <= 'z') {
					ident += expr[idx];
					idx++;
				}
				assert(!ident.empty());
				//cerr << "got ident " << ident << '\n';
				if (idx < int(expr.size()) && expr[idx] == '(') {
					idx++;
					// function call
					assert(!is_ref);
					if (!funcs.count(ident)) error();
					const auto& arg_list = funcs[ident];
					for (int z = 0; z+1 < int(arg_list.size()); z++) {
						assert(idx < int(expr.size()));
						if (expr[idx] == ')') error();
						if (z) {
							assert(expr[idx] == ',');
							idx++;
							assert(expr[idx] == ' ');
							idx++;
						}
						auto resp = self();
						if (resp.is_ref != arg_list[z]) error();
					}
					assert(idx < int(expr.size()));
					if (expr[idx] == ',') error();
					assert(expr[idx] == ')');
					idx++;
					return var_t{arg_list.back(), -1};
				} else {
					if (!vars.count(ident)) {
						error();
					}
					auto v = var_t(vars[ident]);
					if (is_ref) {
						if (v.is_ref) error();
						v.is_ref = true;
					} else if (v.is_ref) {
						referenced.insert(v.var_id);
					} else {
						assert(v.var_id != -1);
						if (moved.count(v.var_id)) error();
						moved.insert(v.var_id);
					}
					return v;
				}
			})();

			if (idx != int(expr.size())) error();
		}

		for (auto v : referenced) {
			if (moved.count(v)) error();
		}

		if (!new_var.is_ref && new_var.var_id == -1) new_var.var_id = line;

		if (vars.count(var_name)) {
			auto v = vars[var_name];
			if (!v.is_ref) {
				moved.insert(v.var_id);
			}
		}

		vars[var_name] = new_var;
	}

	cout << "Valid" << '\n';

	return 0;
}

#include <bits/stdc++.h>

template <class To, class From>
std::enable_if_t<
    sizeof(To) == sizeof(From) &&
    std::is_trivially_copyable_v<From> &&
    std::is_trivially_copyable_v<To>,
    To>
// constexpr support needs compiler magic
bit_cast(const From& src) noexcept
{
    static_assert(std::is_trivially_constructible_v<To>,
        "This implementation additionally requires destination type to be trivially constructible");
 
    To dst;
    std::memcpy(&dst, &src, sizeof(To));
    return dst;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M, K; cin >> N >> M >> K;
	double mi = 1e-5;
	double ma = std::max(N, M) + 1;
	for (int z = 0; z < 60; z++) {
		auto a = ::bit_cast<int64_t>(mi);
		auto b = ::bit_cast<int64_t>(ma);
		auto c = a + ((b-a)>>1);
		double md = ::bit_cast<double>(c);
		if (__int128(N / md) * __int128(M / md) >= K) mi = md;
		else ma = md;
	}
	cout << fixed << setprecision(20) << mi << '\n';

	return 0;
}

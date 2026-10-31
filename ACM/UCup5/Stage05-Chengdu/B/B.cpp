#include <bits/stdc++.h>
#include <cassert>

#if 1 // region https://github.com/ecnerwala/cp-book (`scripts/bundle.py src/containers/vec.hpp src/num/modnum.hpp src/geometry/point.hpp`)
#include <algorithm>
#include <concepts>
#include <cstdlib>
#include <format>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <memory>
#include <ranges>
#include <source_location>
#include <span>
#include <variant>
#include <cassert>
#include <istream>
#include <ostream>
#include <cstdint>
#include <limits>
#include <type_traits>
#include <complex>
#include <tuple>
#include <numeric>
#line 2 "src/containers/vec.hpp"

#line 15 "src/containers/vec.hpp"

namespace wala {

#ifdef _GLIBCXX_DEBUG
constexpr bool WALA_DEBUG = true;
#else
constexpr bool WALA_DEBUG = false;
#endif

template <typename... A> struct fmt_loc {
	std::format_string<A...> fmt;
	std::source_location loc;
	template <typename S> consteval fmt_loc(const S& s, std::source_location l = std::source_location::current()) : fmt(s), loc(l) {}
};
template <typename... A> void debug_assert(bool b, fmt_loc<std::type_identity_t<A>...> f, const A&... a) {
	if (!b) [[unlikely]] {
		if constexpr (WALA_DEBUG) {
			std::cerr << f.loc.file_name() << ':' << f.loc.line() << ": wala: " << std::vformat(f.fmt.get(), std::make_format_args(a...)) << '\n';
			std::abort();
		} else {
			__builtin_unreachable();
		}
	}
}

struct uninit_t{}; inline constexpr uninit_t uninit{};
struct with_capacity{ int n; };

template <typename T> struct vec;
template <typename T> struct bounded_vec;
template <typename T> struct bounded_stack;
template <typename T> struct growable_vec;

namespace detail {
template <typename C, bool Const>
struct contiguous_iterator_impl {
	using Self = contiguous_iterator_impl;

	using iterator_category = std::random_access_iterator_tag;
	using iterator_concept = std::contiguous_iterator_tag;
	// value_type should be non-cv qualified
	using value_type = C::value_type;
	using T = std::conditional_t<Const, const value_type, value_type>;
	using pointer = T*;
	using reference = T&;
	using difference_type = int;

	pointer p = nullptr;
	[[no_unique_address]] std::conditional_t<WALA_DEBUG, const C*, std::monostate> c{};

	contiguous_iterator_impl() = default;
	contiguous_iterator_impl(T* p_, [[maybe_unused]] const C* c_) : p(p_) { if constexpr (WALA_DEBUG) c = c_; }
	// Cast from non-const to const
	template <bool OConst> requires (Const && !OConst) contiguous_iterator_impl(const contiguous_iterator_impl<C, OConst>& o)  : p(o.p), c(o.c) {}

	void assert_valid_deref() const { if constexpr (WALA_DEBUG) { c->assert_valid_deref(p); } }
	static void assert_same_container([[maybe_unused]] Self a, [[maybe_unused]] Self b) {
		if constexpr (WALA_DEBUG) {
			debug_assert(a.c == b.c, "comparing two iterators from different containers");
		}
	}

	T& operator * () const { assert_valid_deref(); return *p; }
	T* operator -> () const { assert_valid_deref(); return p; }
	T& operator [] (int n) const { return *(*this + n); }

	Self& operator ++ () { ++p; return *this; }
	Self operator ++ (int) { auto tmp = *this; operator ++ (); return tmp; }
	Self& operator -- () { --p; return *this; }
	Self operator -- (int) { auto tmp = *this; operator -- (); return tmp; }
	Self& operator += (int n) { p += n; return *this; }
	friend Self operator + (Self a, int n) { return a += n; }
	friend Self operator + (int n, Self a) { return a + n; }
	Self& operator -= (int n) { p -= n; return *this; }
	friend Self operator - (Self a, int n) { return a -= n; }
	friend int operator - (Self a, Self b) { assert_same_container(a, b); return int(a.p - b.p); }

	friend bool operator == (Self a, Self b) { assert_same_container(a, b); return a.p == b.p; }
	friend auto operator <=> (Self a, Self b) { assert_same_container(a, b); return a.p <=> b.p; }
};
} // namespace detail
} // namespace wala

template <typename C, bool Const> struct std::pointer_traits<wala::detail::contiguous_iterator_impl<C, Const>> {
	using pointer = wala::detail::contiguous_iterator_impl<C, Const>;
	using element_type = pointer::T;
	using difference_type = pointer::difference_type;
	static element_type* to_address(pointer it) noexcept { return it.p; }
};

namespace wala {

namespace detail {
template <typename T> T* allocate(int n) {
	debug_assert(n >= 0, "allocating negative {} items", n);
	return n ? std::allocator<T>().allocate(std::size_t(n)) : nullptr;
}
template <typename T> void deallocate(T* p, int n) {
	if (p) std::allocator<T>().deallocate(p, std::size_t(n));
}

// Ownership of an existing allocation of cap items, the first sz of which are live.
// Passed between the containers by release() and the allocation constructors.
template <typename T> struct allocation { T* p; int sz; int cap; };

template <typename T> void uninitialized_construct(T* first, T* last) { std::uninitialized_value_construct(first, last); }
template <typename T> void uninitialized_construct(T*, T*, uninit_t) requires std::is_trivially_default_constructible_v<T> {}
template <typename T> void uninitialized_construct(T* first, T* last, const std::type_identity_t<T>& v) { std::uninitialized_fill(first, last, v); }
template <typename T, typename... A> concept uninitialized_constructor = requires (T* p, const A&... a) { detail::uninitialized_construct(p, p, a...); };

} // namespace detail

template <typename T> struct vec {
	static_assert(!std::is_const_v<T>);
	using value_type = T;
	using size_type = int;
	using difference_type = int;
	using iterator = detail::contiguous_iterator_impl<vec, false>;
	using const_iterator = detail::contiguous_iterator_impl<vec, true>;

	size_type sz = 0;
	T* base = nullptr;

	vec() = default;

	friend void swap(vec& a, vec& b) noexcept { std::swap(a.sz, b.sz); std::swap(a.base, b.base); }
	vec(vec&& o) noexcept : vec() { swap(*this, o); }
	vec& operator=(vec&& o) noexcept { swap(*this, o); return *this; }
	vec(const vec&) = delete;
	vec& operator=(const vec&) = delete;

	~vec() { std::destroy_n(base, sz); detail::deallocate(base, sz); }
private:
	struct alloc_only { size_type n; };
	explicit vec(alloc_only a) : sz(a.n), base(detail::allocate<T>(a.n)) {}
public:
	template <typename... A> requires detail::uninitialized_constructor<T, A...>
	explicit vec(size_type n, const A&... a) : vec(alloc_only{n}) { detail::uninitialized_construct(base, base + n, a...); }
	explicit vec(size_type n, const T& v) requires std::copy_constructible<T> : vec(alloc_only{n}) { detail::uninitialized_construct(base, base + n, v); }

	template <std::ranges::sized_range R> requires std::constructible_from<T, std::ranges::range_reference_t<R>>
	vec(std::from_range_t, R&& r) : vec(alloc_only{size_type(std::ranges::size(r))}) {
		std::ranges::uninitialized_copy(r, std::span(base, sz));
	}
	vec(std::initializer_list<T> list) requires std::copy_constructible<T> : vec(std::from_range, list) {}

	vec(const vec& o) requires std::copy_constructible<T> : vec(std::from_range, o) {}
	vec& operator=(const vec& o) requires std::copy_constructible<T> { *this = vec(o); return *this; }

	// Conversions, transferring the allocation
	explicit vec(detail::allocation<T> a) : sz(a.sz), base(a.p) {
		debug_assert(a.sz == a.cap, "adopting allocation with size {} not equal to capacity {}", a.sz, a.cap);
	}
	detail::allocation<T> release() {
		detail::allocation<T> a{base, sz, sz};
		base = nullptr;
		sz = 0;
		return a;
	}
	bounded_vec<T> into_bounded() && { return bounded_vec<T>(release()); }
	bounded_stack<T> into_stack() && { return bounded_stack<T>(release()); }
	growable_vec<T> into_growable() && { return growable_vec<T>(release()); }

	[[nodiscard]] bool empty() const { return !sz; }
	[[nodiscard]] size_type size() const { return sz; }

	void assert_valid_deref(const T* p) const {
		debug_assert(base <= p && p < base + sz, "iterator position {} out of range [0, {})", p - base, sz);
	}
	void assert_valid_index(size_type i) const {
		debug_assert(0 <= i && i < sz, "index {} out of range [0, {})", i, sz);
	}
	void assert_nonempty() const {
		debug_assert(sz > 0, "container is empty");
	}

	T* data() { return base; }
	const T* data() const { return base; }
	iterator begin() { return iterator(base, this); }
	const_iterator begin() const { return const_iterator(base, this); }
	iterator end() { return iterator(base + sz, this); }
	const_iterator end() const { return const_iterator(base + sz, this); }
	T& operator [] (size_type i) { assert_valid_index(i); return base[i]; }
	const T& operator [] (size_type i) const { assert_valid_index(i); return base[i]; }
	T& front() { assert_nonempty(); return base[0]; }
	const T& front() const { assert_nonempty(); return base[0]; }
	T& back() { assert_nonempty(); return base[sz-1]; }
	const T& back() const { assert_nonempty(); return base[sz-1]; }

	friend bool operator == (const vec& a, const vec& b) { return std::ranges::equal(a, b); }
	friend auto operator <=> (const vec& a, const vec& b) {
		return std::lexicographical_compare_three_way(a.begin(), a.end(), b.begin(), b.end());
	}
};

namespace detail {

struct insert_ops {
	template <typename Self>
	void push_back(this Self& self, const Self::value_type& v) requires std::copy_constructible<typename Self::value_type> {
		self.emplace_back(v);
	}
	template <typename Self>
	void push_back(this Self& self, Self::value_type&& v) {
		self.emplace_back(std::move(v));
	}

	// Add/remove to the middle
	template <typename Self, typename... Args>
	requires std::constructible_from<typename Self::value_type, Args&&...>
	Self::value_type& emplace(this Self& self, Self::size_type i, Args&&... args) {
		self.assert_valid_insert(i);
		self.emplace_back(std::forward<Args>(args)...);
		std::rotate(self.begin() + i, self.end() - 1, self.end());
		return self[i];
	}
	template <typename Self>
	void insert(this Self& self, Self::size_type i, const Self::value_type& v) requires std::copy_constructible<typename Self::value_type> {
		self.emplace(i, v);
	}
	template <typename Self>
	void insert(this Self& self, Self::size_type i, Self::value_type&& v) {
		self.emplace(i, std::move(v));
	}
	template <typename Self, typename... A> requires detail::uninitialized_constructor<typename Self::value_type, A...>
	void insert_n(this Self& self, Self::size_type i, Self::size_type n, const A&... a) {
		self.assert_valid_insert(i);
		auto [first, last] = self.grow_by(n, a...);
		std::rotate(self.begin() + i, first, last);
	}
	template <typename Self>
	void insert_n(this Self& self, Self::size_type i, Self::size_type n, const Self::value_type& v) requires std::copy_constructible<typename Self::value_type> {
		self.template insert_n<Self, typename Self::value_type>(i, n, v);
	}
	template <typename Self, std::ranges::input_range R>
	requires std::constructible_from<typename Self::value_type, std::ranges::range_reference_t<R>>
	void insert_range(this Self& self, Self::size_type i, R&& r) {
		self.assert_valid_insert(i);
		auto [first, last] = self.append_range(std::forward<R>(r));
		std::rotate(self.begin() + i, first, last);
	}

	template <typename Self>
	void erase(this Self& self, Self::size_type i) {
		self.assert_valid_index(i);
		std::shift_left(self.begin() + i, self.end(), 1);
		self.pop_back();
	}
	template <typename Self>
	void erase_n(this Self& self, Self::size_type i, Self::size_type n) {
		self.assert_valid_insert(i);
		self.assert_valid_insert(i + n);
		std::shift_left(self.begin() + i, self.end(), n);
		self.shrink_by(n);
	}

	template <typename Self, typename... Args>
	requires std::constructible_from<typename Self::value_type, Args&&...>
	Self::value_type& emplace_front(this Self& self, Args&&... args) {
		return self.emplace(0, std::forward<Args>(args)...);
	}
	template <typename Self> void push_front(this Self& self, const Self::value_type& v) requires std::copy_constructible<typename Self::value_type> { self.emplace_front(v); }
	template <typename Self> void push_front(this Self& self, Self::value_type&& v) { self.emplace_front(std::move(v)); }
	template <typename Self> void pop_front(this Self& self) { self.erase(0); }
};

}

template <typename T> struct bounded_vec : detail::insert_ops {
	static_assert(!std::is_const_v<T>);
	using value_type = T;
	using size_type = int;
	using difference_type = int;
	using iterator = detail::contiguous_iterator_impl<bounded_vec, false>;
	using const_iterator = detail::contiguous_iterator_impl<bounded_vec, true>;

	size_type sz = 0;
	size_type cap = 0;
	T* base = nullptr;

	bounded_vec() = default;

	friend void swap(bounded_vec& a, bounded_vec& b) noexcept { std::swap(a.sz, b.sz); std::swap(a.cap, b.cap); std::swap(a.base, b.base); }
	bounded_vec(bounded_vec&& o) noexcept : bounded_vec() { swap(*this, o); }
	bounded_vec& operator=(bounded_vec&& o) noexcept { swap(*this, o); return *this; }

	// Destruction
	~bounded_vec() { std::destroy_n(base, sz); detail::deallocate(base, cap); }

	// Construction
	explicit bounded_vec(with_capacity c) : cap(c.n), base(detail::allocate<T>(c.n)) {}
	template <typename... A> requires detail::uninitialized_constructor<T, A...>
	explicit bounded_vec(with_capacity c, size_type n, const A&... a) : bounded_vec(c) {
		debug_assert(n <= c.n, "size {} exceeds capacity {}", n, c.n);
		detail::uninitialized_construct(base, base + n, a...); sz = n;
	}
	template <typename... A> requires detail::uninitialized_constructor<T, A...>
	explicit bounded_vec(size_type n, const A&... a) : bounded_vec(with_capacity{n}, n, a...) {}
	explicit bounded_vec(with_capacity c, size_type n, const T& v) requires std::copy_constructible<T> : bounded_vec(c) { grow_by<T>(n, v); }
	explicit bounded_vec(size_type n, const T& v) requires std::copy_constructible<T> : bounded_vec(with_capacity{n}, n, v) {}
	template <std::ranges::input_range R>
	requires std::constructible_from<T, std::ranges::range_reference_t<R>>
	bounded_vec(with_capacity c, std::from_range_t, R&& r) : bounded_vec(c) {
		auto [in, out] = std::ranges::uninitialized_copy(r, std::ranges::subrange(base, base + cap));
		// Must have drained all of r
		debug_assert(in == std::ranges::end(r), "range size exceeds capacity {}", c.n);
		sz = size_type(out - base);
	}
	template <std::ranges::sized_range R>
	requires std::constructible_from<T, std::ranges::range_reference_t<R>>
	bounded_vec(std::from_range_t f, R&& r) : bounded_vec(with_capacity{size_type(std::ranges::size(r))}, f, r) {}
	bounded_vec(std::initializer_list<T> list) requires std::copy_constructible<T> : bounded_vec(std::from_range, list) {}
	bounded_vec(with_capacity c, std::initializer_list<T> list) requires std::copy_constructible<T> : bounded_vec(c, std::from_range, list) {}
	bounded_vec(const bounded_vec& o) requires std::copy_constructible<T> : bounded_vec(with_capacity{o.cap}, std::from_range, o) {}
	bounded_vec& operator=(const bounded_vec& o) requires std::copy_constructible<T> { *this = bounded_vec(o); return *this; }

	// Conversions, transferring the allocation
	explicit bounded_vec(detail::allocation<T> a) : sz(a.sz), cap(a.cap), base(a.p) {
		debug_assert(0 <= a.sz && a.sz <= a.cap, "adopting allocation with size {} out of range [0, {}]", a.sz, a.cap);
	}
	detail::allocation<T> release() {
		detail::allocation<T> a{base, sz, cap};
		base = nullptr;
		sz = cap = 0;
		return a;
	}
	vec<T> into_vec() && {
		debug_assert(sz == cap, "converting container with size {} and capacity {} to vec", sz, cap);
		return vec<T>(release());
	}
	bounded_stack<T> into_stack() && { return bounded_stack<T>(release()); }
	growable_vec<T> into_growable() && { return growable_vec<T>(release()); }

	[[nodiscard]] bool empty() const { return !sz; }
	[[nodiscard]] size_type size() const { return sz; }
	[[nodiscard]] size_type capacity() const { return cap; }

	void assert_valid_deref(const T* p) const {
		debug_assert(base <= p && p < base + sz, "iterator position {} out of range [0, {})", p - base, sz);
	}
	void assert_valid_index(size_type i) const {
		debug_assert(0 <= i && i < sz, "index {} out of range [0, {})", i, sz);
	}
	void assert_valid_insert(size_type i) const {
		debug_assert(0 <= i && i <= sz, "index {} out of range [0, {}]", i, sz);
	}
	void assert_nonempty() const {
		debug_assert(sz > 0, "container is empty");
	}

	T* data() { return base; }
	const T* data() const { return base; }
	iterator begin() { return iterator(base, this); }
	const_iterator begin() const { return const_iterator(base, this); }
	iterator end() { return iterator(base + sz, this); }
	const_iterator end() const { return const_iterator(base + sz, this); }
	T& operator [] (size_type i) { assert_valid_index(i); return base[i]; }
	const T& operator [] (size_type i) const { assert_valid_index(i); return base[i]; }
	T& front() { assert_nonempty(); return base[0]; }
	const T& front() const { assert_nonempty(); return base[0]; }
	T& back() { assert_nonempty(); return base[sz-1]; }
	const T& back() const { assert_nonempty(); return base[sz-1]; }

	// Resizing ops
	template <typename... Args> requires std::constructible_from<T, Args&&...>
	T& emplace_back(Args&&... args) {
		debug_assert(sz < cap, "pushing into full container (capacity {})", cap);
		return *std::construct_at(base + sz++, std::forward<Args>(args)...);
	}
	template <std::ranges::input_range R>
	requires std::constructible_from<T, std::ranges::range_reference_t<R>>
	std::ranges::subrange<iterator> append_range(R&& r) {
		auto old_end = end();
		// Use raw pointers since the memory is uninitialized
		auto [in, out] = std::ranges::uninitialized_copy(r, std::ranges::subrange(base + sz, base + cap));
		// Must have drained all of r
		debug_assert(in == std::ranges::end(r), "range size exceeds capacity {}", cap);
		sz = size_type(out - base);
		return {old_end, end()};
	}
	template <typename... A> requires detail::uninitialized_constructor<T, A...>
	std::ranges::subrange<iterator> grow_by(size_type n, const A&... a) {
		debug_assert(0 <= n, "growing container by negative {}", n);
		debug_assert(sz + n <= cap, "growing container with size {} by {} exceeds capacity {}", sz, n, cap);
		auto old_end = end();
		detail::uninitialized_construct(base + sz, base + sz + n, a...);
		sz += n;
		return {old_end, end()};
	}
	std::ranges::subrange<iterator> grow_by(size_type n, const T& v) requires std::copy_constructible<T> { return grow_by<T>(n, v); }
	template <typename... A> requires detail::uninitialized_constructor<T, A...>
	std::ranges::subrange<iterator> grow_to(size_type n, const A&... a) {
		debug_assert(sz <= n, "growing container with size {} to smaller target {}", sz, n);
		debug_assert(n <= cap, "growing container to {} exceeds capacity {}", n, cap);
		auto old_end = end();
		detail::uninitialized_construct(base + sz, base + n, a...);
		sz = n;
		return {old_end, end()};
	}
	std::ranges::subrange<iterator> grow_to(size_type n, const T& v) requires std::copy_constructible<T> { return grow_to<T>(n, v); }
	template <typename... A> requires detail::uninitialized_constructor<T, A...>
	std::ranges::subrange<iterator> grow_to(iterator it, const A&... a) {
		debug_assert(base + sz <= it.p, "growing container with size {} to smaller target {}", sz, it.p - base);
		debug_assert(it.p <= base + cap, "growing container to {} exceeds capacity {}", it.p - base, cap);
		auto old_end = end();
		detail::uninitialized_construct(base + sz, it.p, a...);
		sz = size_type(it.p - base);
		return {old_end, end()};
	}
	std::ranges::subrange<iterator> grow_to(iterator it, const T& v) requires std::copy_constructible<T> { return grow_to<T>(it, v); }

	// Remove from the back
	void pop_back() { assert_nonempty(); std::destroy_at(base + --sz); }
	void shrink_by(size_type n) {
		debug_assert(0 <= n && n <= sz, "shrinking container with size {} by {}", sz, n);
		std::destroy(base + sz - n, base + sz);
		sz -= n;
	}
	void shrink_to(size_type n) {
		debug_assert(0 <= n && n <= sz, "shrinking container with size {} to {}", sz, n);
		std::destroy(base + n, base + sz);
		sz = n;
	}
	void shrink_to(iterator it) {
		debug_assert(base <= it.p && it.p <= base + sz, "shrinking container with size {} to {}", sz, it.p - base);
		std::destroy(it.p, base + sz);
		sz = size_type(it.p - base);
	}

	void clear() { shrink_to(0); }

	friend bool operator == (const bounded_vec& a, const bounded_vec& b) { return std::ranges::equal(a, b); }
	friend auto operator <=> (const bounded_vec& a, const bounded_vec& b) {
		return std::lexicographical_compare_three_way(a.begin(), a.end(), b.begin(), b.end());
	}
};

template <typename T> struct bounded_stack : detail::insert_ops {
	static_assert(!std::is_const_v<T>);
	using value_type = T;
	using size_type = int;
	using difference_type = int;
	using iterator = detail::contiguous_iterator_impl<bounded_stack, false>;
	using const_iterator = detail::contiguous_iterator_impl<bounded_stack, true>;

	T* base = nullptr;
	T* top = nullptr;
	T* lim = nullptr;

	bounded_stack() = default;

	friend void swap(bounded_stack& a, bounded_stack& b) noexcept { std::swap(a.base, b.base); std::swap(a.top, b.top); std::swap(a.lim, b.lim); }
	bounded_stack(bounded_stack&& o) noexcept : bounded_stack() { swap(*this, o); }
	bounded_stack& operator=(bounded_stack&& o) noexcept { swap(*this, o); return *this; }

	// Destruction
	~bounded_stack() { std::destroy(base, top); detail::deallocate(base, int(lim - base)); }

	// Construction
	explicit bounded_stack(with_capacity c) : base(detail::allocate<T>(c.n)), top(base), lim(base + c.n) {}
	template <typename... A> requires detail::uninitialized_constructor<T, A...>
	explicit bounded_stack(with_capacity c, size_type n, const A&... a) : bounded_stack(c) {
		debug_assert(n <= c.n, "size {} exceeds capacity {}", n, c.n);
		detail::uninitialized_construct(base, base + n, a...); top = base + n;
	}
	template <typename... A> requires detail::uninitialized_constructor<T, A...>
	explicit bounded_stack(size_type n, const A&... a) : bounded_stack(with_capacity{n}, n, a...) {}
	explicit bounded_stack(with_capacity c, size_type n, const T& v) requires std::copy_constructible<T> : bounded_stack(c) { grow_by<T>(n, v); }
	explicit bounded_stack(size_type n, const T& v) requires std::copy_constructible<T> : bounded_stack(with_capacity{n}, n, v) {}
	template <std::ranges::input_range R>
	requires std::constructible_from<T, std::ranges::range_reference_t<R>>
	bounded_stack(with_capacity c, std::from_range_t, R&& r) : bounded_stack(c) {
		auto [in, out] = std::ranges::uninitialized_copy(r, std::ranges::subrange(base, lim));
		// Must have drained all of r
		debug_assert(in == std::ranges::end(r), "range size exceeds capacity {}", c.n);
		top = out;
	}
	template <std::ranges::sized_range R>
	requires std::constructible_from<T, std::ranges::range_reference_t<R>>
	bounded_stack(std::from_range_t f, R&& r) : bounded_stack(with_capacity{size_type(std::ranges::size(r))}, f, r) {}
	bounded_stack(std::initializer_list<T> list) requires std::copy_constructible<T> : bounded_stack(std::from_range, list) {}
	bounded_stack(with_capacity c, std::initializer_list<T> list) requires std::copy_constructible<T> : bounded_stack(c, std::from_range, list) {}
	bounded_stack(const bounded_stack& o) requires std::copy_constructible<T> : bounded_stack(with_capacity{o.capacity()}, std::from_range, o) {}
	bounded_stack& operator=(const bounded_stack& o) requires std::copy_constructible<T> { *this = bounded_stack(o); return *this; }

	// Conversions, transferring the allocation
	explicit bounded_stack(detail::allocation<T> a) : base(a.p), top(a.p + a.sz), lim(a.p + a.cap) {
		debug_assert(0 <= a.sz && a.sz <= a.cap, "adopting allocation with size {} out of range [0, {}]", a.sz, a.cap);
	}
	detail::allocation<T> release() {
		detail::allocation<T> a{base, size_type(top - base), size_type(lim - base)};
		base = top = lim = nullptr;
		return a;
	}
	vec<T> into_vec() && {
		debug_assert(top == lim, "converting container with size {} and capacity {} to vec", top - base, lim - base);
		return vec<T>(release());
	}
	bounded_vec<T> into_bounded() && { return bounded_vec<T>(release()); }
	growable_vec<T> into_growable() && { return growable_vec<T>(release()); }

	[[nodiscard]] bool empty() const { return top == base; }
	[[nodiscard]] size_type size() const { return size_type(top - base); }
	[[nodiscard]] size_type capacity() const { return size_type(lim - base); }

	void assert_valid_deref(const T* p) const {
		debug_assert(base <= p && p < top, "iterator position {} out of range [0, {})", p - base, top - base);
	}
	void assert_valid_index(size_type i) const {
		debug_assert(0 <= i && base + i < top, "index {} out of range [0, {})", i, top - base);
	}
	void assert_valid_insert(size_type i) const {
		debug_assert(0 <= i && base + i <= top, "index {} out of range [0, {}]", i, top - base);
	}
	void assert_nonempty() const {
		debug_assert(top > base, "container is empty");
	}

	T* data() { return base; }
	const T* data() const { return base; }
	iterator begin() { return iterator(base, this); }
	const_iterator begin() const { return const_iterator(base, this); }
	iterator end() { return iterator(top, this); }
	const_iterator end() const { return const_iterator(top, this); }
	T& operator [] (size_type i) { assert_valid_index(i); return base[i]; }
	const T& operator [] (size_type i) const { assert_valid_index(i); return base[i]; }
	T& front() { assert_nonempty(); return base[0]; }
	const T& front() const { assert_nonempty(); return base[0]; }
	T& back() { assert_nonempty(); return top[-1]; }
	const T& back() const { assert_nonempty(); return top[-1]; }

	// Resizing ops
	template <typename... Args> requires std::constructible_from<T, Args&&...>
	T& emplace_back(Args&&... args) {
		debug_assert(top < lim, "pushing into full container (capacity {})", lim - base);
		return *std::construct_at(top++, std::forward<Args>(args)...);
	}
	template <std::ranges::input_range R>
	requires std::constructible_from<T, std::ranges::range_reference_t<R>>
	std::ranges::subrange<iterator> append_range(R&& r) {
		auto old_end = end();
		// Use raw pointers since the memory is uninitialized
		auto [in, out] = std::ranges::uninitialized_copy(r, std::ranges::subrange(top, lim));
		// Must have drained all of r
		debug_assert(in == std::ranges::end(r), "range size exceeds capacity {}", lim - base);
		top = out;
		return {old_end, end()};
	}
	template <typename... A> requires detail::uninitialized_constructor<T, A...>
	std::ranges::subrange<iterator> grow_by(size_type n, const A&... a) {
		debug_assert(0 <= n, "growing container by negative {}", n);
		debug_assert(top + n <= lim, "growing container with size {} by {} exceeds capacity {}", top - base, n, lim - base);
		auto old_end = end();
		detail::uninitialized_construct(top, top + n, a...);
		top += n;
		return {old_end, end()};
	}
	std::ranges::subrange<iterator> grow_by(size_type n, const T& v) requires std::copy_constructible<T> { return grow_by<T>(n, v); }
	template <typename... A> requires detail::uninitialized_constructor<T, A...>
	std::ranges::subrange<iterator> grow_to(size_type n, const A&... a) {
		debug_assert(top <= base + n, "growing container with size {} to smaller target {}", top - base, n);
		debug_assert(base + n <= lim, "growing container to {} exceeds capacity {}", n, lim - base);
		auto old_end = end();
		detail::uninitialized_construct(top, base + n, a...);
		top = base + n;
		return {old_end, end()};
	}
	std::ranges::subrange<iterator> grow_to(size_type n, const T& v) requires std::copy_constructible<T> { return grow_to<T>(n, v); }
	template <typename... A> requires detail::uninitialized_constructor<T, A...>
	std::ranges::subrange<iterator> grow_to(iterator it, const A&... a) {
		debug_assert(top <= it.p, "growing container with size {} to smaller target {}", top - base, it.p - base);
		debug_assert(it.p <= lim, "growing container to {} exceeds capacity {}", it.p - base, lim - base);
		auto old_end = end();
		detail::uninitialized_construct(top, it.p, a...);
		top = it.p;
		return {old_end, end()};
	}
	std::ranges::subrange<iterator> grow_to(iterator it, const T& v) requires std::copy_constructible<T> { return grow_to<T>(it, v); }

	// Remove from the back
	void pop_back() { assert_nonempty(); std::destroy_at(--top); }
	void shrink_by(size_type n) {
		debug_assert(0 <= n && base <= top - n, "shrinking container with size {} by {}", top - base, n);
		std::destroy(top - n, top);
		top -= n;
	}
	void shrink_to(size_type n) {
		debug_assert(0 <= n && base + n <= top, "shrinking container with size {} to {}", top - base, n);
		std::destroy(base + n, top);
		top = base + n;
	}
	void shrink_to(iterator it) {
		debug_assert(base <= it.p && it.p <= top, "shrinking container with size {} to {}", top - base, it.p - base);
		std::destroy(it.p, top);
		top = it.p;
	}

	void clear() { shrink_to(0); }

	friend bool operator == (const bounded_stack& a, const bounded_stack& b) { return std::ranges::equal(a, b); }
	friend auto operator <=> (const bounded_stack& a, const bounded_stack& b) {
		return std::lexicographical_compare_three_way(a.begin(), a.end(), b.begin(), b.end());
	}
};

template <typename T> struct growable_vec : private bounded_vec<T> {
	using base = bounded_vec<T>;
	using typename base::value_type, typename base::size_type, typename base::difference_type, typename base::iterator, typename base::const_iterator;
	// Inherit constructors
	using base::bounded_vec;
	using base::empty, base::size, base::capacity, base::data, base::begin, base::end, base::operator[], base::front, base::back;
	// Needed for insert_ops and other consumers
	using base::assert_valid_deref, base::assert_valid_index, base::assert_valid_insert, base::assert_nonempty;
	using base::pop_back, base::shrink_by, base::shrink_to, base::clear;
	// From insert_ops
	using base::push_back, base::emplace, base::insert, base::insert_n, base::insert_range, base::erase, base::erase_n, base::emplace_front, base::push_front, base::pop_front;

	friend void swap(growable_vec& a, growable_vec& b) noexcept { swap(static_cast<base&>(a), static_cast<base&>(b)); }

	// Conversions, transferring the allocation
	using base::release;
	vec<T> into_vec() && {
		shrink_to_fit();
		return std::move(static_cast<base&>(*this)).into_vec();
	}
	bounded_vec<T> into_bounded() && { return std::move(static_cast<base&>(*this)); }
	bounded_stack<T> into_stack() && { return std::move(static_cast<base&>(*this)).into_stack(); }

	// Update capacity when running growing ops
	void set_capacity(size_type n) {
		debug_assert(n >= size(), "reserving less capacity {} than size {}", n, size());
		base::operator=(base(with_capacity{n}, std::from_range, *this | std::views::as_rvalue));
	}
	void reserve_for(int n) {
		int c = capacity();
		if (n > c) {
			set_capacity(std::max(2 * c, n));
		}
	}
	void shrink_to_fit() { set_capacity(size()); }

	template <typename... Args> requires std::constructible_from<T, Args&&...>
	T& emplace_back(Args&&... args) {
		if (size() < capacity()) return base::emplace_back(std::forward<Args>(args)...);
		// args may refer to our own elements, so construct before reallocating
		T x(std::forward<Args>(args)...);
		reserve_for(size() + 1);
		return base::emplace_back(std::move(x));
	}
	template <std::ranges::input_range R>
	requires std::constructible_from<T, std::ranges::range_reference_t<R>>
	std::ranges::subrange<iterator> append_range(R&& r) {
		if constexpr (std::ranges::sized_range<R>) {
			reserve_for(size() + size_type(std::ranges::size(r)));
			return base::append_range(std::forward<R>(r));
		} else {
			size_type old_sz = size();
			for (auto&& x : r) emplace_back(std::forward<decltype(x)>(x));
			return {begin() + old_sz, end()};
		}
	}
	template <typename... A> requires detail::uninitialized_constructor<T, A...>
	std::ranges::subrange<iterator> grow_by(size_type n, const A&... a) {
		if (size() + n <= capacity()) return base::grow_by(n, a...);
		// a may refer to our own elements, so copy before reallocating
		std::tuple<A...> c(a...);
		reserve_for(size() + n);
		return std::apply([&](const A&... x) { return base::grow_by(n, x...); }, c);
	}
	std::ranges::subrange<iterator> grow_by(size_type n, const T& v) requires std::copy_constructible<T> { return grow_by<T>(n, v); }
	template <typename... A> requires detail::uninitialized_constructor<T, A...>
	std::ranges::subrange<iterator> grow_to(size_type n, const A&... a) {
		debug_assert(size() <= n, "growing container with size {} to smaller target {}", size(), n);
		return grow_by(n - size(), a...);
	}
	std::ranges::subrange<iterator> grow_to(size_type n, const T& v) requires std::copy_constructible<T> { return grow_to<T>(n, v); }

	friend bool operator == (const growable_vec& a, const growable_vec& b) { return std::ranges::equal(a, b); }
	friend auto operator <=> (const growable_vec& a, const growable_vec& b) {
		return std::lexicographical_compare_three_way(a.begin(), a.end(), b.begin(), b.end());
	}
};

} // namespace wala
#line 2 "src/num/modnum.hpp"

#line 10 "src/num/modnum.hpp"

namespace wala {

template <typename T> T mod_inv_in_range(T a, T m) {
	// assert(0 <= a && a < m);
	T x = a, y = m;
	// abs coeff of a in x and y (they're always opposite sign)
	T vx = 1, vy = 0;
	bool swap = false;
	while (x) {
		T k = y / x;
		y %= x;
		vy += k * vx;
		std::swap(x, y);
		std::swap(vx, vy);
		swap ^= 1;
	}
	assert(y == 1);
	return swap ? vy : m - vy;
}

template <typename T> struct extended_gcd_result {
	T gcd;
	T coeff_a, coeff_b;
};
template <typename T> extended_gcd_result<T> extended_gcd(T a, T b) {
	T x = a, y = b;
	// coeff of a and b in x and y
	T ax = 1, ay = 0;
	T bx = 0, by = 1;
	while (x) {
		T k = y / x;
		y %= x;
		ay -= k * ax;
		by -= k * bx;
		std::swap(x, y);
		std::swap(ax, ay);
		std::swap(bx, by);
	}
	return {y, ay, by};
}

template <typename T> T mod_inv(T a, T m) {
	a %= m;
	a = a < 0 ? a + m : a;
	return mod_inv_in_range(a, m);
}

// Derives the boilerplate operator surface of a number type from its compound
// ops, ==, neg(), and inv().
// Bodies are only instantiated on use, so a type may omit some of the
// underlying pieces if the corresponding derived ops are never called.
template <typename Self>
struct num_ops {
	Self operator+ () const { return static_cast<const Self&>(*this); }
	Self operator- () const { return static_cast<const Self&>(*this).neg(); }

	friend Self operator ++ (Self& a, int) { Self r = a; ++a; return r; }
	friend Self operator -- (Self& a, int) { Self r = a; --a; return r; }
	friend Self operator + (const Self& a, const Self& b) { return Self(a) += b; }
	friend Self operator - (const Self& a, const Self& b) { return Self(a) -= b; }
	friend Self operator * (const Self& a, const Self& b) { return Self(a) *= b; }
	friend Self operator / (const Self& a, const Self& b) { return Self(a) /= b; }

	friend bool operator != (const Self& a, const Self& b) { return !(a == b); }

	friend Self neg(const Self& a) { return a.neg(); }
	friend Self inv(const Self& a) { return a.inv(); }
};

// Storage and arithmetic for numbers mod Self::MOD, as a reduced
// representative v in [0, MOD) of unsigned type V.
// The type provides static MOD (of type V), reduce (value -> representative),
// and *=;
// everything else is derived here, valid for any MOD up to V's full range
// (sums and differences are tracked mod 2^bits, so no headroom is needed).
// Hooks may be overridden in the type's own body (e.g. a faster += / -=).
template <typename Self, typename V>
struct mod_ops : num_ops<Self> {
	static_assert(std::unsigned_integral<V>);
	V v;

	struct is_reduced_tag {};

	mod_ops() : v(0) {}
	mod_ops(V v_, is_reduced_tag) : v(v_) { assert(v < Self::MOD); }
	template <std::integral I> mod_ops(I x) : v(Self::reduce(x)) {}

	static Self from_reduced(V v) { return Self(v, is_reduced_tag{}); }

	// A negative value reduces via its nonnegative complement: x = -1 - ~x.
	static V reduce(std::signed_integral auto x) {
		using U = std::make_unsigned_t<decltype(x)>;
		return x < 0 ? V(Self::MOD - 1 - Self::reduce(U(~x))) : Self::reduce(U(x));
	}

	explicit operator V() const { return v; }
	std::make_signed_t<V> balanced() const {
		return std::make_signed_t<V>(Self::MOD-v > v ? v : v - Self::MOD);
	}

	friend bool operator == (const Self& a, const Self& b) { return a.v == b.v; }
	friend std::ostream& operator << (std::ostream& out, const Self& n) { return out << n.v; }
	friend std::istream& operator >> (std::istream& in, Self& n) { int64_t v_; in >> v_; n = Self(v_); return in; }

	Self& operator ++ () {
		++v;
		if (v == Self::MOD) v = 0;
		return self();
	}
	Self& operator -- () {
		if (v == 0) v = Self::MOD;
		--v;
		return self();
	}
	Self& operator += (const Self& o) { v = Self::sub_mod_raw(v, Self::MOD - o.v); return self(); }
	Self& operator -= (const Self& o) { v = Self::sub_mod_raw(v, o.v); return self(); }
	Self& operator /= (const Self& o) { return self() *= o.inv(); }

	// Returns a - b mod MOD, for b in [0, MOD]; wraparound detects the underflow.
	static V sub_mod_raw(V a, V b) { return a < b ? a - b + Self::MOD : a - b; }

	Self neg() const { return from_reduced(v ? Self::MOD - v : 0); }
	Self inv() const { return from_reduced(mod_inv_in_range(v, Self::MOD)); }

private:
	Self& self() { return static_cast<Self&>(*this); }
};

template <auto MOD_> struct modnum : mod_ops<modnum<MOD_>, std::make_unsigned_t<decltype(MOD_)>> {
	using Self = modnum;
	static_assert(MOD_ > 0, "MOD must be positive");
	using V = std::make_unsigned_t<decltype(MOD_)>;
	static constexpr V MOD = V(MOD_);

	using base = mod_ops<modnum, V>;
	using base::base;
	using base::v;
	using base::reduce;

	static V reduce(std::unsigned_integral auto x) { return V(x % MOD); }

	explicit operator std::make_signed_t<V>() const
		requires (MOD <= V(std::numeric_limits<std::make_signed_t<V>>::max()))
	{
		return std::make_signed_t<V>(v);
	}

	Self& operator *= (const Self& o) {
		if constexpr (sizeof(V) <= 4) v = V(uint64_t(v) * o.v % MOD);
		else v = V(__uint128_t(v) * o.v % MOD);
		return *this;
	}
};

struct mod_goldilocks : mod_ops<mod_goldilocks, uint64_t> {
	using Self = mod_goldilocks;
	static constexpr uint64_t MOD = 0xffffffff00000001ull;
	static constexpr uint64_t EPS = -MOD;
	// We have 2^32 is a primitive 6th root of unity.
	// Note that omega_8 + omega_8^7 == 2^24 - 2^72 == sqrt(2)
	// We'll pick the root so that 2^24 - 2^72 is our primitive 384th root of unity.
	static constexpr uint64_t PRIMITIVE_ROOT = 2717;

	using base = mod_ops<mod_goldilocks, uint64_t>;
	using base::base;
	using base::reduce;
	mod_goldilocks() = default;
	mod_goldilocks(__int128_t a) : base(a < 0 ? uint64_t(MOD - 1 - __uint128_t(~a) % MOD) : uint64_t(__uint128_t(a) % MOD), is_reduced_tag{}) {}
	mod_goldilocks(__uint128_t a) : base(uint64_t(a % MOD), is_reduced_tag{}) {}

	// Avoids the division: any uint64_t is within MOD of reduced.
	static uint64_t reduce(std::unsigned_integral auto x) {
		static_assert(sizeof(x) <= 8);
		uint64_t a = x;
		return a >= MOD ? a - MOD : a;
	}

	// returns a-b, assuming -MOD <= a-b, e.g. b <= MOD
	static uint64_t sub_mod_raw(uint64_t a, uint64_t b) {
#if defined(__x86_64__)
		// TODO: We could try to write this using intrinsics, but GCC sometimes produces the wrong code.
		uint64_t res_wrapped = a;
		uint64_t adjustment = b;
		asm (
			// AT&T syntax: SRC DST
			"sub %[y], %[x]\n\t"
			// Trick from plonky2 implementation:
			// After sub, flag CF is set iff we underflowed. We want to correct by EPS == 2^32 - 1 iff C is set.
			// sbb (subtract with borrow) computes DST <- DST - SRC - CF
			// Thus, we can use the 32-bit form of sbb on a dummy register to load CF ? EPS : 0.
			// Here, we'll just reuse the original register holding b.
			"sbb %k[y], %k[y]\n\t"
			: [x] "+r"(res_wrapped),
			[y] "+r"(adjustment)
			:
			: "cc"
		);
#else
		uint64_t res_wrapped = a - b;
		uint64_t adjustment = (res_wrapped > a) ? EPS : 0;
#endif
		return res_wrapped - adjustment;
	}

	// Reduce lo + 2^64 * mi + 2^96 * hi, where hi <= MOD
	static uint64_t reduce_u160_raw(uint64_t lo, uint32_t mi, uint64_t hi) {
		// result = lo - hi + EPS * mi
		// 0 <= lo <= 2^64 - 1 = MOD + EPS - 1
		// 0 <= EPS * mi <= (2^32 - 1) * EPS = MOD - 1 - EPS
		// 0 <= hi <= MOD
		// -MOD <= lo - hi + EPS * mi <= 2*MOD-2
		// so we do have some leeway
		return sub_mod_raw(sub_mod_raw(lo, hi), MOD-(uint64_t(mi)<<32)+mi);
	}

	static uint64_t reduce_u128_raw(__uint128_t v) {
		uint64_t hi = uint64_t(v >> 64);
		uint64_t lo = uint64_t(v);
		uint32_t hi_hi = uint32_t(hi >> 32);
		uint32_t hi_lo = uint32_t(hi);
		return reduce_u160_raw(lo, hi_lo, hi_hi);
	}

	Self& operator *= (Self o) {
		v = reduce_u128_raw(__uint128_t(v) * __uint128_t(o.v));
		return *this;
	}
};

template <typename T> T power(T a, long long b) {
	assert(b >= 0);
	T r = 1; while (b) { if (b & 1) r *= a; b >>= 1; a *= a; } return r;
}

template <typename tag> struct dynamic_modnum : mod_ops<dynamic_modnum<tag>, uint32_t> {
	using Self = dynamic_modnum;

private:
	inline static uint32_t MOD_ = 0;
	inline static uint64_t BARRETT_M = 0;

public:
	// Make only the const-reference public, to force the use of set_mod
	static constexpr uint32_t const& MOD = MOD_;

	using base = mod_ops<dynamic_modnum, uint32_t>;
	using base::base;
	using base::v;
	using base::reduce;

	// Barret reduction taken from KACTL:
	/**
	 * Author: Simon Lindholm
	 * Date: 2020-05-30
	 * License: CC0
	 * Source: https://en.wikipedia.org/wiki/Barrett_reduction
	 * Description: Compute $a \% b$ about 5 times faster than usual, where $b$ is constant but not known at compile time.
	 * Returns a value congruent to $a \pmod b$ in the range $[0, 2b)$.
	 * Status: proven correct, stress-tested
	 * Measured as having 4 times lower latency, and 8 times higher throughput, see stress-test.
	 * Details:
	 * More precisely, it can be proven that the result equals 0 only if $a = 0$,
	 * and otherwise lies in $[1, (1 + a/2^64) * b)$.
	 */
	static void set_mod(int mod) {
		assert(mod > 0);
		MOD_ = uint32_t(mod);
		BARRETT_M = (uint64_t(-1) / MOD);
	}
	static uint32_t barrett_reduce_partial(uint64_t a) {
		return uint32_t(a - uint64_t((__uint128_t(BARRETT_M) * a) >> 64) * MOD);
	}
	static uint32_t barrett_reduce(uint64_t a) {
		int32_t res = int32_t(barrett_reduce_partial(a) - MOD);
		return uint32_t((res < 0) ? res + int32_t(MOD) : res);
	}

	struct mod_reader {
		friend std::istream& operator >> (std::istream& i, mod_reader) {
			int mod; i >> mod;
			Self::set_mod(mod);
			return i;
		}
	};
	static mod_reader MOD_READER() {
		return mod_reader();
	}

	static uint32_t reduce(std::unsigned_integral auto x) {
		static_assert(sizeof(x) <= 8);
		return barrett_reduce(x);
	}

	explicit operator int() const { return int(v); }

	Self& operator *= (const Self& o) {
		v = barrett_reduce(uint64_t(v) * o.v);
		return *this;
	}
};

template <typename T> struct mod_constraint {
	T v, mod;

	friend mod_constraint operator & (mod_constraint a, mod_constraint b) {
		if (a.mod < b.mod) std::swap(a, b);
		if (b.mod == 1) return a;

		extended_gcd_result<T> egcd = extended_gcd<T>(a.mod, b.mod);
		assert(a.v % egcd.gcd == b.v % egcd.gcd);

		T extra = b.v - a.v % b.mod;
		extra /= egcd.gcd;

		extra *= egcd.coeff_a;
		extra %= b.mod / egcd.gcd;
		extra += (extra < 0) ? b.mod / egcd.gcd : 0;

		return mod_constraint{
			a.v + extra * a.mod,
			a.mod * (b.mod / egcd.gcd)
		};
	}
};

} // namespace wala
#line 2 "src/geometry/point.hpp"

#line 8 "src/geometry/point.hpp"

namespace wala {

template <typename T, typename AreaT=T> struct Point {
public:
	T x, y;
	Point() : x(0), y(0) {}
	Point(T x_, T y_) : x(x_), y(y_) {}
	template <typename U, typename V> explicit Point(const Point<U, V>& p) : x(p.x), y(p.y) {}
	Point(const std::pair<T, T>& p) : x(p.first), y(p.second) {}
	Point(const std::complex<T>& p) : x(real(p)), y(imag(p)) {}
	explicit operator std::pair<T, T> () const { return std::pair<T, T>(x, y); }
	explicit operator std::complex<T> () const { return std::complex<T>(x, y); }
	auto as_pair() const { return std::pair<T, T>(*this); }
	auto as_complex() const { return std::complex<T>(*this); }

	friend std::ostream& operator << (std::ostream& o, const Point& p) { return o << '(' << p.x << ',' << p.y << ')'; }
	friend std::istream& operator >> (std::istream& i, Point& p) { return i >> p.x >> p.y; }
	friend bool operator == (const Point& a, const Point& b) { return a.x == b.x && a.y == b.y; }
	friend bool operator != (const Point& a, const Point& b) { return !(a==b); }

	Point operator + () const { return Point(+x, +y); }
	Point operator - () const { return Point(-x, -y); }

	Point& operator += (const Point& p) { x += p.x, y += p.y; return *this; }
	Point& operator -= (const Point& p) { x -= p.x, y -= p.y; return *this; }
	Point& operator *= (const T& t) { x *= t, y *= t; return *this; }
	Point& operator /= (const T& t) { x /= t, y /= t; return *this; }

	friend Point operator + (const Point& a, const Point& b) { return Point(a.x+b.x, a.y+b.y); }
	friend Point operator - (const Point& a, const Point& b) { return Point(a.x-b.x, a.y-b.y); }
	friend Point operator * (const Point& a, const T& t) { return Point(a.x*t, a.y*t); }
	friend Point operator * (const T& t ,const Point& a) { return Point(t*a.x, t*a.y); }
	friend Point operator / (const Point& a, const T& t) { return Point(a.x/t, a.y/t); }

	AreaT dist2() const { return AreaT(x) * AreaT(x) + AreaT(y) * AreaT(y); }
	auto dist() const { return std::sqrt(dist2()); }
	Point unit() const { return *this / this->dist(); }
	auto angle() const { return std::atan2(y, x); }

	T int_norm() const { return std::gcd(x,y); }
	Point int_unit() const { if (!x && !y) return *this; return *this / this->int_norm(); }

	// Convenient free-functions, mostly for generic interop
	friend auto norm(const Point& a) { return a.dist2(); }
	friend auto abs(const Point& a) { return a.dist(); }
	friend auto unit(const Point& a) { return a.unit(); }
	friend auto arg(const Point& a) { return a.angle(); }
	friend auto int_norm(const Point& a) { return a.int_norm(); }
	friend auto int_unit(const Point& a) { return a.int_unit(); }

	Point perp_cw() const { return Point(y, -x); }
	Point perp_ccw() const { return Point(-y, x); }

	friend AreaT dot(const Point& a, const Point& b) { return AreaT(a.x) * AreaT(b.x) + AreaT(a.y) * AreaT(b.y); }
	friend AreaT cross(const Point& a, const Point& b) { return AreaT(a.x) * AreaT(b.y) - AreaT(a.y) * AreaT(b.x); }
	friend AreaT cross3(const Point& a, const Point& b, const Point& c) { return cross(b-a, c-a); }

	// Complex numbers and rotation
	friend Point conj(const Point& a) { return Point(a.x, -a.y); }

	// Returns conj(a) * b
	friend Point dot_cross(const Point& a, const Point& b) { return Point(dot(a, b), cross(a, b)); }
	friend Point cmul(const Point& a, const Point& b) { return dot_cross(conj(a), b); }
	friend Point cdiv(const Point& a, const Point& b) { return dot_cross(b, a) / b.dist2(); }

	// Must be a unit vector; otherwise multiplies the result by abs(u)
	Point rotate(const Point& u) const { return dot_cross(conj(u), *this); }
	Point unrotate(const Point& u) const { return dot_cross(u, *this); }

	friend bool lex_less(const Point& a, const Point& b) {
		return std::tie(a.x, a.y) < std::tie(b.x, b.y);
	}

	friend bool same_dir(const Point& a, const Point& b) { return cross(a,b) == 0 && dot(a,b) > 0; }

	// check if 180 <= s..t < 360
	friend bool is_reflex(const Point& a, const Point& b) { auto c = cross(a,b); return c ? (c < 0) : (dot(a, b) < 0); }

	// operator < (s,t) for angles in [base,base+2pi)
	friend bool angle_less_from(const Point& base, const Point& s, const Point& t) {
		int r = is_reflex(base, s) - is_reflex(base, t);
		return r ? (r < 0) : (0 < cross(s, t));
	}
	// operator < (s,t) for angles in (base,base+2pi]
	friend bool angle_less_upto(const Point& base, const Point& s, const Point& t) {
		int r = is_reflex(t, base) - is_reflex(s, base);
		return r ? (r < 0) : (0 < cross(s, t));
	}

	friend auto angle_cmp_from(const Point& base) {
		return [base](const Point& s, const Point& t) { return angle_less_from(base, s, t); };
	}
	friend auto angle_cmp_upto(const Point& base) {
		return [base](const Point& s, const Point& t) { return angle_less_upto(base, s, t); };
	}
	friend auto angle_cmp_center_from(const Point& center, const Point& dir) {
		return [center, dir](const Point& s, const Point& t) -> bool { return angle_less_from(dir, s-center, t-center); };
	}
	friend auto angle_cmp_center_upto(const Point& center, const Point& dir) {
		return [center, dir](const Point& s, const Point& t) -> bool { return angle_less_upto(dir, s-center, t-center); };
	}

	// is p in [s,t] taken ccw? 1/0/-1 for in/border/out
	friend int angle_between(const Point& s, const Point& t, const Point& p) {
		if (same_dir(p, s) || same_dir(p, t)) return 0;
		return angle_less_from(s, p, t) ? 1 : -1;
	}
};

} // namespace wala
#endif // endregion

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	auto solve = [&](int64_t N, int64_t X0, int64_t Y0, int64_t A, int64_t B, int64_t C, int64_t T) -> int64_t {
		assert(1 <= X0 && X0 <= N);
		assert(1 <= Y0 && Y0 <= N);
		X0--, Y0--, C += A + B;
		N--;
		assert(0 <= X0 && X0 <= N);
		assert(0 <= Y0 && Y0 <= N);
		if (B == 0) {
			std::swap(A, B);
			std::swap(X0, Y0);
		}
		assert(B != 0);
		// Since B != 0, there's a unique Y for each X on the line, so we can just use X coordinates of intersections
	};

	int CASES; std::cin >> CASES;
	while (CASES--) {
		int N, X0, Y0, Q; std::cin >> N >> X0 >> Y0 >> Q;
		while (Q--) {
			int A, B, C, T; std::cin >> A >> B >> C >> T;
			std::cout << solve(N, X0, Y0, A, B, C, T) << '\n';
		}
	}

	return 0;
}

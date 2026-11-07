#include <bits/stdc++.h>
#include <cassert>

#if 1 // region https://github.com/ecnerwala/cp-book (`scripts/bundle.py fft/engines/ntt.hpp fft/series.hpp`)
#include <algorithm>
#include <cassert>
#include <span>
#include <utility>
#include <cmath>
#include <iterator>
#include <concepts>
#include <cstdlib>
#include <format>
#include <initializer_list>
#include <iostream>
#include <memory>
#include <ranges>
#include <source_location>
#include <variant>
#include <istream>
#include <ostream>
#include <cstdint>
#include <limits>
#include <type_traits>
#include <cstddef>
#include <functional>
#include <optional>
#line 2 "/home/andrew/dev/cp-book/src/fft/engines/ntt.hpp"

#line 7 "/home/andrew/dev/cp-book/src/fft/engines/ntt.hpp"

#line 2 "/home/andrew/dev/cp-book/src/fft/core.hpp"

#line 7 "/home/andrew/dev/cp-book/src/fft/core.hpp"

#line 2 "/home/andrew/dev/cp-book/src/fft/common.hpp"

#line 7 "/home/andrew/dev/cp-book/src/fft/common.hpp"

#line 2 "/home/andrew/dev/cp-book/src/containers/vec.hpp"

#line 15 "/home/andrew/dev/cp-book/src/containers/vec.hpp"

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
#line 9 "/home/andrew/dev/cp-book/src/fft/common.hpp"

/**
 * Author: Andrew He
 * Source: http://neerc.ifmo.ru/trains/toulouse/2017/fft2.pdf
 * Papers about accuracy: http://www.daemonology.net/papers/fft.pdf, http://www.cs.berkeley.edu/~fateman/papers/fftvsothers.pdf
 * For integers rounding works if $(|a| + |b|)\max(a, b) < \mathtt{\sim} 10^9$, or in theory maybe $10^6$.
 *
 * Abstraction layers:
 *   fft_core<num>   FFT itself and other ops on rings with 2^k-th roots of unity. We use bit-reversed indexing in the frequency domain.
 *
 *   engines         Engines for packing/unpacking arbitrary rings for convolution (the `engine` concept).
 *                   Still expose (opaque) transform-domain objects for caching/fusion.
 *
 *   multiply layer  Wrappers for convolving bounded sequences: track length/truncation.
 *
 *   value types     series::vec<E, exact> - R[[x]]
 *                   series::exact<E> - exact (finite-support) power series
 *                   series::trunc<E> - truncated prefix of an (infinite) power series
 *
 *                   polynomials - R[x]. Under x -> 1/x a polynomial becomes a Laurent polynomial in 1/x;
 *                                 shifting by x^{deg P} (reversal) lands it in R[[x]], and we store that exact series.
 *                   poly::vec<E> - polynomial type, supporting natural indexing
 *                   poly::form<E> - finite-support linear forms, via the pairing <P, S> = [x^0] P(1/x) S(x)
 *                                    a linear form is one side of this pairing, applied to the other
 *
 *                   online_multiplier<E> - online (relaxed) multiplication of 2 sequences in n log^2 n time
 *                   ap_sampled_poly<E> - a polynomial stored as its evaluations on an arithmetic progression
 */

namespace wala {

template <class T> int sz(T&& arg) { using std::size; return int(size(std::forward<T>(arg))); }
inline int nextPow2(int s) { return 1 << (s > 1 ? 32 - __builtin_clz(s-1) : 0); }

} // namespace wala

namespace wala::fft {

using std::swap;
using std::min;
using std::max;

// Reusable scratch buffers. Not thread-safe by default: this is deliberately plain
// static storage so single-threaded programs pay no TLS indirection; define
// ECNERWALA_FFT_POOL_STORAGE to `thread_local` for multithreaded use.
#ifndef ECNERWALA_FFT_POOL_STORAGE
#define ECNERWALA_FFT_POOL_STORAGE
#endif
template <typename T> struct buffer_pool {
	static inline ECNERWALA_FFT_POOL_STORAGE growable_vec<bounded_vec<T>> free_list;
	struct handle {
		bounded_vec<T> v;
		explicit handle(int n) {
			if (!free_list.empty()) {
				v = std::move(free_list.back());
				free_list.pop_back();
			}
			if (v.capacity() < n) v = bounded_vec<T>(with_capacity{n});
			v.clear();
			v.grow_to(n, T());
		}
		handle(const handle&) = delete;
		handle& operator=(const handle&) = delete;
		handle(handle&& o) noexcept : v(std::move(o.v)) {}
		~handle() { if (v.capacity()) free_list.push_back(std::move(v)); }
		T& operator[](int i) { return v[i]; }
		operator std::span<T>() { return std::span<T>(v); }
		std::span<T> span() { return std::span<T>(v); }
	};
	static handle get(int n) { return handle(n); }
};

} // namespace wala::fft
#line 2 "/home/andrew/dev/cp-book/src/num/modnum.hpp"

#line 10 "/home/andrew/dev/cp-book/src/num/modnum.hpp"

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
#line 10 "/home/andrew/dev/cp-book/src/fft/core.hpp"

namespace wala::fft {

// ==== core: roots, buffers, raw transforms ====

// Complex
template <typename dbl> struct cplx { /// start-hash
	dbl x, y;
	cplx(dbl x_ = 0, dbl y_ = 0) : x(x_), y(y_) { }
	friend cplx operator+(cplx a, cplx b) { return cplx(a.x + b.x, a.y + b.y); }
	friend cplx operator-(cplx a, cplx b) { return cplx(a.x - b.x, a.y - b.y); }
	friend cplx operator-(cplx a) { return cplx(-a.x, -a.y); }
	friend cplx operator*(cplx a, cplx b) { return cplx(a.x * b.x - a.y * b.y, a.x * b.y + a.y * b.x); }
	friend cplx conj(cplx a) { return cplx(a.x, -a.y); }
	friend cplx inv(cplx a) { dbl n = (a.x*a.x+a.y*a.y); return cplx(a.x/n,-a.y/n); }
};

// getRoot implementations
template <typename num> struct getRoot {
	static num f(int k) = delete;
};
template <typename dbl> struct getRoot<cplx<dbl>> {
	static cplx<dbl> f(int k) {
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
		dbl a=2*M_PI/k;
		return cplx<dbl>(cos(a),sin(a));
	}
};
template <int MOD> struct primitive_root {
	static const int value;
};
// 998244353 = (119 << 23) + 1 = 2^30 - 2^26 - 2^23 + 1
template <> struct primitive_root<998244353> {
	static const int value = 3;
};
// babybear prime
template <> struct primitive_root<(15 << 27) + 1> {
	static const int value = 31;
};
// koalabear prime
template <> struct primitive_root<(127 << 24) + 1> {
	static const int value = 3;
};
template <> struct primitive_root<(7 << 26) + 1> {
	static const int value = 3;
};
template <> struct primitive_root<(5 << 25) + 1> {
	static const int value = 3;
};
template <int MOD> struct getRoot<modnum<MOD>> {
	static modnum<MOD> f(int k) {
		assert((MOD-1)%k == 0);
		return power(modnum<MOD>(primitive_root<MOD>::value), (MOD-1)/k);
	}
};
template <> struct getRoot<mod_goldilocks> {
	static mod_goldilocks f(int k) {
		assert((mod_goldilocks::MOD-1)%k == 0);
		return power(mod_goldilocks(mod_goldilocks::PRIMITIVE_ROOT), (mod_goldilocks::MOD-1)/k);
	}
};

// We take the bit-reverse convention: the coefficient of a[i] -> b[j] is omega^{i * bit_reverse(j)}.
// This means that the size 2^{k-1} transform is the prefix of the size 2^k transform (wrapping the input).
//
// We mostly work with spans here:
//   spans of transforms are expected to have length exactly 2^k
//   spans of inputs/outputs are expected to have length [0, 2^{k+1})
//
// Inputs/outputs are treated mod x^{2^k} - 1.
// Their length is allowed to be bigger than 2^k mostly to perform ops on sequences of size n+1 with only transforms of size n.
// The upper bound of 2^{k+1} is arbitrary: we could tighten it to 2^k + 1 or loosen it to infinity, this is just a "defensive" choice.
template <typename num> struct fft_core {
	static inline vec<int> rev;
	// rt[2^k + i] = 1^{i / 2^(k+1)}
	// TODO: can we get rid of inv_rt; alternatively, should we store inv_rt in bit-reverse order?
	static inline growable_vec<num> rt, inv_rt;

	static void init(int n) {
		if (n <= sz(rt)) return;
		rev = vec<int>(n);
		for (int i = 0; i < n; i++) {
			rev[i] = (rev[i>>1] | ((i&1)*n)) >> 1;
		}
		rt.reserve_for(n); inv_rt.reserve_for(n);
		while (sz(rt) < 2 && sz(rt) < n) { rt.push_back(num(1)); inv_rt.push_back(num(1)); }
		for (int k = sz(rt); k < n; k *= 2) {
			rt.grow_to(2*k); inv_rt.grow_to(2*k);
			num z = getRoot<num>::f(2*k);
			num iz = inv(z);
			for (int i = k/2; i < k; i++) {
				rt[2*i] = rt[i], rt[2*i+1] = rt[i]*z;
				inv_rt[2*i] = inv_rt[i], inv_rt[2*i+1] = inv_rt[i]*iz;
			}
		}
	}

	// bit-reversal of i as a log2(n)-bit number
	static int brev(int i, int n) {
		int s = __builtin_ctz(unsigned(sz(rev)/n));
		return rev[i] >> s;
	}
	// index of the conjugate evaluation point, in the returned bit-reversed order
	static int conj_index(int j) {
		return j == 0 ? 0 : j ^ ((1 << (31 - __builtin_clz(unsigned(j)))) - 1);
	}

	static void forward(std::span<num> a) {
		int n = sz(a);
		if (n <= 1) return;
		init(n);
		for (int k = n/2; k >= 1; k /= 2) {
			for (int i = 0; i < n; i += 2*k) {
				for (int j = 0; j < k; j++) {
					num u = a[i+j], v = a[i+j+k];
					a[i+j] = u + v;
					a[i+j+k] = (u - v) * rt[j+k];
				}
			}
		}
	}

	static void inverse(std::span<num> a) {
		int n = sz(a);
		if (n <= 1) return;
		init(n);
		for (int k = 1; k < n; k *= 2) {
			for (int i = 0; i < n; i += 2*k) {
				for (int j = 0; j < k; j++) {
					num t = inv_rt[j+k] * a[i+j+k];
					a[i+j+k] = a[i+j] - t;
					a[i+j] = a[i+j] + t;
				}
			}
		}
	}

	// Extend a size 2^{k-1} transform to size 2^k; we need the coefficients.
	// t must have size 2^k, and coeffs must have size at most 2^{k+1}.
	static void extend(std::span<num> t, std::span<const num> coeffs) {
		int n = sz(t) / 2;
		assert(sz(coeffs) <= 2 * n);
		init(sz(t));
		auto b = t.subspan(n, n);
		int lo = min(sz(coeffs), n);
		for (int i = 0; i < lo; i++) {
			// rt[n + i] = w_{2n}^i
			b[i] = coeffs[i] * rt[n + i];
		}
		std::fill(b.begin() + lo, b.end(), num(0));
		for (int i = n; i < sz(coeffs); i++) {
			b[i - n] = b[i - n] - coeffs[i] * rt[i];
		}
		forward(b);
	}

	// Consider t = transform(P) and P(x) = E(x^2) + O(x^2) * x
	// `even_half` and `odd_half` extract a size 2^{k-1} transform of E/O, respectively.
	static void even_half(std::span<const num> t, std::span<num> out) {
		int n = sz(out);
		assert(sz(t) >= 2*n);
		num half = inv(num(2));
		for (int j = 0; j < n; j++) out[j] = (t[2*j] + t[2*j+1]) * half;
	}
	static void odd_half(std::span<const num> t, std::span<num> out) {
		int n = sz(out);
		assert(sz(t) >= 2*n);
		init(2*n);
		num half = inv(num(2));
		for (int j = 0; j < n; j++) {
			// entry j of the size-2n transform pairs (w, -w) with w = w_{2n}^{brev(j, n)}
			out[j] = (t[2*j] - t[2*j+1]) * half * inv_rt[n + brev(j, n)];
		}
	}
	static void downsample(std::span<const num> t, std::span<num> out, bool odd) {
		if (odd) odd_half(t, out);
		else even_half(t, out);
	}

	static void upsample_as_evens(std::span<const num> t, std::span<num> out) {
		int n = sz(out);
		assert(2 * sz(t) >= n);
		for (int j = 0; j < n; j++) out[j] = t[j>>1];
	}
	// TODO: We could just implement a generic "cyclic-shift" primitive; at least we should make it not involve brev
	static void upsample_as_odds(std::span<const num> t, std::span<num> out) {
		int n = sz(out);
		assert(n >= 2);
		assert(sz(t) >= n / 2);
		init(n);
		for (int j = 0; j < n/2; j++) {
			num v = t[j] * rt[n/2 + brev(j, n/2)];
			out[2*j+0] = v;
			out[2*j+1] = -v;
		}
	}
	static void upsample(std::span<const num> t, std::span<num> out, bool odd) {
		if (odd) upsample_as_odds(t, out);
		else upsample_as_evens(t, out);
	}
};

} // namespace wala::fft
#line 2 "/home/andrew/dev/cp-book/src/fft/engine.hpp"

#line 7 "/home/andrew/dev/cp-book/src/fft/engine.hpp"

#line 9 "/home/andrew/dev/cp-book/src/fft/engine.hpp"

namespace wala::fft {

// ==== engine concept ====

// Output operations for the finish step to express arbitrary fusion into the output buffer.
struct assign_op { template <typename T> void operator()(T& d, T v) const { d = v; } };
struct add_op { template <typename T> void operator()(T& d, T v) const { d += v; } };
struct sub_op { template <typename T> void operator()(T& d, T v) const { d -= v; } };
struct add_twice_op { template <typename T> void operator()(T& d, T v) const { d += v + v; } };

// `engine` contract
//   engine represents a way of packing/unpacking sequences over an arbitrary ring into FFT-style transforms.
//   We expect transforms/products of transforms to be linear but potentially lossy/imprecise, so we'll track precision
//   at compile-time as a template parameter.
//
//   E::value_type   The ring we operate over
//   E::unit_scale   0 or 1 depending on whether there's error that can accumulate
//   E::commutative  A marker for whether the ring is commutative
//
//   transformed_t<A>  The transform of a sequence. This object owns its data buffer.
//   product_t<A>      The product of 2 transforms. May equal transformed_t, particularly when unit_scale = 0.
//
//   transformed       alias for transformed_t<unit_scale>
//   product           alias for product_t<unit_scale>
//
//   The basic multiplication API is
//      transform(span<const value_type> in, int n) -> transformed_t<unit_scale>
//      mul(transformed_t<A>, transformed_t<B>, int n) -> product_t<A*B>
//      mul2(a1, b1, a2, b2, int n) -> product_t<A1*B1 + A2*B2>, computing a1*b1 + a2*b2 in one pass
//      finish(product_t<A>, span<value_type>& out, Op) -> void
//
//   Input span can be length up to 2n.
//   Output spans can be length up to n; only the prefix that exists is filled.
//   finish applies Op exactly once per out element, in index order, to
//   value_type targets (so ops may be stateful).
//   Transforms can be longer than necessary, and only the relevant prefix is used.
//
//   For non-exact engines, there's some subtlety in whether we wrap before or after packing.
//   We will choose to wrap *after* packing, which hurts error bounds but makes the prefix condition more uniform.
//
//   Additionally, we have APIs to take advantage of linearity in both transformed and product space:
//      add(transformed_t<A>, transformed_t<B>) -> transformed_t<A+B>
//      add(product_t<K1>, product_t<K2>) -> product_t<K1+K2>
//
//   Finally, we expose some additional fast-transform optimization paths.
//   extend_to only operates on transformed_t<unit_scale>; the others are scale-generic.
//   downsample is also defined on product_t (halving before finish saves inverse-transform work).
//      extend_to       build (if empty) or grow a transform to size m by repeated doubling; feed the same coefficients (sz <= 2m) every time
//                      (each doubling step reads only the coefficients that fit, so zero-padded buffers are fine:
//                       coefficients past twice the existing transform's size must be zero, or it couldn't be a prefix)
//      downsample      compute the half-sized transform/product of just the even (odd = false) or odd terms of the input
//      upsample        size n (n >= 2) transform/product of the input spread as evens (odd = false) or odds (odd = true); inverse of downsample
//      negate_arg      size n transform of A(-x)
//      of_reverse      size n transform of A(1/x) mod x^n - 1, i.e. of the cyclically reversed coefficients a[-k mod n]
//
//   Optional (exact-ish engines only, currently ntt and real): read a product out without an inverse transform.
//      dot(product_t<K>, transformed_t<A>, int n) -> value_type
//                      sum_k finish(p)[k] * b[k] over k < n, where b is the coefficient sequence (mod x^n - 1) of the
//                      transform; Parseval, so it costs O(n) instead of an inverse transform.
//                      <P, B> with B = of_reverse(T) is the constant term of P * T.
//                      Not offered by split/crt: their pointwise data isn't exact enough to sum before reduction.
template <typename E>
concept engine = requires(
	std::span<const typename E::value_type> in,
	std::span<typename E::value_type> out,
	E::transformed& t,
	const E::transformed& ct,
	E::product& p,
	const E::product& cp,
	int n
) {
	typename E::value_type;
	{ E::transform(in, n) } -> std::same_as<typename E::transformed>;
	{ ct.size() } -> std::same_as<int>;
	E::extend_to(t, n, in);
	{ E::downsample(ct, n, false) } -> std::same_as<typename E::transformed>;
	{ E::downsample(cp, n, false) } -> std::same_as<typename E::product>;
	{ E::upsample(ct, n, false) } -> std::same_as<typename E::transformed>;
	{ E::upsample(cp, n, false) } -> std::same_as<typename E::product>;
	{ E::negate_arg(ct, n) } -> std::same_as<typename E::transformed>;
	{ E::of_reverse(ct, n) } -> std::same_as<typename E::transformed>;
	{ E::mul(ct, ct, n) } -> std::same_as<typename E::product>;
	{ E::sq(ct, n) } -> std::same_as<typename E::product>;
	{ E::mul2(ct, ct, ct, ct, n) } -> std::same_as<typename E::template product_t<2 * E::unit_scale>>;
	E::finish(std::move(p), out);
	E::finish(std::move(p), out, add_op{});
	E::finish(E::add(std::move(p), std::move(p)), out);
	{ E::add(E::transform(in, n), ct) } -> std::same_as<typename E::template transformed_t<2 * E::unit_scale>>;
	{ E::add(std::move(p), std::move(p)) } -> std::same_as<typename E::template product_t<2 * E::unit_scale>>;
	requires std::same_as<std::remove_cvref_t<decltype(E::commutative)>, bool>;
	requires std::same_as<std::remove_cvref_t<decltype(E::unit_scale)>, int>;
};

// Constrains two engine-parameterized value types to share the same engine.
template <typename A, typename B>
concept same_engine = std::same_as<typename A::engine_t, typename B::engine_t>;

// short spelling for E::transformed at use sites
template <engine E> using transformed = E::transformed;

} // namespace wala::fft
#line 10 "/home/andrew/dev/cp-book/src/fft/engines/ntt.hpp"

namespace wala::fft::engines {

template <typename num> struct ntt {
	using value_type = num;
	static constexpr bool commutative = true;
	using core = fft_core<num>;
	struct transformed {
		vec<num> v;
		int size() const { return sz(v); }
	};
	using product = transformed;
	static constexpr int unit_scale = 0;
	template <int A = 0> using transformed_t = transformed;
	template <int K = 0> using product_t = product;

	static transformed transform(std::span<const num> a, int n) {
		assert(sz(a) <= 2 * n);
		transformed r{vec<num>(n, num(0))};
		int lo = min(sz(a), n);
		std::copy(a.begin(), a.begin() + lo, r.v.begin());
		for (int i = n; i < sz(a); i++) r.v[i - n] += a[i];
		core::forward(std::span<num>(r.v));
		return r;
	}
	static void extend_to(transformed& t, int m, std::span<const num> coeffs) {
		assert(!(m & (m-1)) && sz(coeffs) <= 2 * m);
		if (t.size() >= m) return;
		if (t.size() == 0) { t = transform(coeffs, m); return; }
		while (t.size() < m) {
			int s = t.size();
			vec<num> nv(2 * s);
			std::copy(t.v.begin(), t.v.end(), nv.begin());
			t.v = std::move(nv);
			// coeffs past 2s are zero: they didn't fit in the transform we're a prefix of
			core::extend(std::span<num>(t.v), coeffs.first(size_t(min(sz(coeffs), 2 * s))));
		}
	}
	static transformed downsample(const transformed& t, int n, bool odd) {
		transformed r{vec<num>(n)};
		core::downsample(std::span<const num>(t.v), std::span<num>(r.v), odd);
		return r;
	}
	static transformed upsample(const transformed& t, int n, bool odd) {
		transformed r{vec<num>(n)};
		core::upsample(std::span<const num>(t.v), std::span<num>(r.v), odd);
		return r;
	}
	static transformed negate_arg(const transformed& t, int n) {
		assert(n >= 2 && t.size() >= n);
		transformed r{vec<num>(n)};
		for (int j = 0; j < n; j++) r.v[j] = t.v[j ^ 1];
		return r;
	}
	// a[-k]: evaluation at w^{-brev(j)}, which is the conjugate index
	static transformed of_reverse(const transformed& t, int n) {
		assert(t.size() >= n);
		transformed r{vec<num>(n)};
		for (int j = 0; j < n; j++) r.v[j] = t.v[core::conj_index(j)];
		return r;
	}
	static product mul(const transformed& a, const transformed& b, int n) {
		assert(a.size() >= n && b.size() >= n);
		product p{vec<num>(n)};
		for (int i = 0; i < n; i++) p.v[i] = a.v[i] * b.v[i];
		return p;
	}
	static product sq(const transformed& a, int n) { return mul(a, a, n); }
	static product mul2(
		const transformed& a1, const transformed& b1,
		const transformed& a2, const transformed& b2,
		int n
	) {
		assert(a1.size() >= n && b1.size() >= n && a2.size() >= n && b2.size() >= n);
		product p{vec<num>(n)};
		for (int i = 0; i < n; i++) p.v[i] = a1.v[i] * b1.v[i] + a2.v[i] * b2.v[i];
		return p;
	}
	static product add(product&& a, const product& b) {
		assert(a.size() == b.size());
		for (int i = 0; i < a.size(); i++) a.v[i] += b.v[i];
		return std::move(a);
	}
	// sum_k finish(p)[k] * b[k] by Parseval: (1/n) sum_j P[j] B[-j]
	static num dot(const product& p, const transformed& t, int n) {
		assert(p.size() >= n && t.size() >= n);
		num r = 0;
		for (int j = 0; j < n; j++) r += p.v[j] * t.v[core::conj_index(j)];
		return r * inv(num(n));
	}
	template <typename Op = assign_op> static void finish(product&& p, std::span<num> out, Op op = {}) {
		int n = p.size();
		assert(sz(out) <= n);
		core::inverse(std::span<num>(p.v));
		num d = inv(num(n));
		for (int i = 0; i < sz(out); i++) op(out[i], p.v[i] * d);
	}
};

} // namespace wala::fft::engines
#line 2 "/home/andrew/dev/cp-book/src/fft/series.hpp"

#line 8 "/home/andrew/dev/cp-book/src/fft/series.hpp"

#line 2 "/home/andrew/dev/cp-book/src/fft/series_core.hpp"

#line 11 "/home/andrew/dev/cp-book/src/fft/series_core.hpp"

#line 2 "/home/andrew/dev/cp-book/src/fft/multiply.hpp"

#line 9 "/home/andrew/dev/cp-book/src/fft/multiply.hpp"

#line 11 "/home/andrew/dev/cp-book/src/fft/multiply.hpp"

namespace wala::fft {

// ==== multiply layer ====
// These are free functions to convolve spans.
//
// The interfaces will typically take input spans, an output span, and an Op representing how to fold the result into the output.
// Output spans may alias one of the input spans.
// Output spans may be shorter than expected; the output will just be truncated.
//
// Some functions may also take E::transformed& objects associated with the input
// spans. These will be lazily filled (see E::extend_to) and used if available.

// Circular convolution mod n (power of 2)
template <engine E, typename Op = assign_op>
void multiply_circular(std::span<const typename E::value_type> a, std::span<const typename E::value_type> b,
		std::span<typename E::value_type> out, int n, Op op = {}) {
	assert(!(n & (n-1)));
	auto ta = E::transform(a, n);
	auto tb = E::transform(b, n);
	E::finish(E::mul(ta, tb, n), out, op);
}

template <engine E, typename Op = assign_op>
void square_circular(std::span<const typename E::value_type> a, std::span<typename E::value_type> out, int n, Op op = {}) {
	assert(!(n & (n-1)));
	auto ta = E::transform(a, n);
	E::finish(E::sq(ta, n), out, op);
}

namespace detail {
// Arrays of length 2^k + 1 are somewhat common, so we will optimize them by
// multiplying mod 2^k, and fixing up the leading coefficient.

// Helpers to detect and perform this optimization.
struct conv_size { int n; bool cut; };
inline conv_size conv_size_for(int s) {
	int n = nextPow2(s);
	bool cut = (n == 2 * (s - 1));
	return {cut ? n / 2 : n, cut};
}

// Call op while lazily applying the correction if necessary
template <typename T, typename Op>
void emit_linear(std::span<T> buf, int n, int s, bool cut, T c0, std::span<T> out, Op op) {
	T cn{};
	if (cut) {
		cn = buf[0] - c0;
		buf[0] = c0;
	}
	int lim = min(sz(out), min(s, n));
	for (int i = 0; i < lim; i++) op(out[i], buf[i]);
	if (cut && sz(out) >= s) op(out[s-1], cn);
}

// Applies op, diverting the wrapped leading coefficient of a cut product:
// out[0] receives c0 and the wraparound term is captured into cn for the
// caller to emit at out[s-1].
template <typename T, typename Op>
struct cut_op {
	Op op;
	T* out0;
	T c0;
	T& cn;
	void operator()(T& x, T v) const {
		if (&x == out0) { cn = v - c0; v = c0; }
		op(x, v);
	}
};

// finish + emit_linear fused: write the finished product directly into out,
// applying the cut correction in place.
template <engine E, typename P, typename Op = assign_op>
void finish_linear(
	P&& p, int n, int s, bool cut,
	typename E::value_type c0, std::span<typename E::value_type> out, Op op = {}
) {
	using T = E::value_type;
	if (sz(out) == 0) return;
	int lim = min(sz(out), min(s, n));
	if (!cut) {
		E::finish(std::move(p), out.subspan(0, lim), op);
	} else {
		T cn{};
		E::finish(std::move(p), out.subspan(0, lim), cut_op<T, Op>{op, &out[0], c0, cn});
		if (sz(out) >= s) op(out[s-1], cn);
	}
}
} // namespace detail

template <engine E, typename Op = assign_op>
void multiply(std::span<const typename E::value_type> a, std::span<const typename E::value_type> b,
		std::span<typename E::value_type> out, Op op = {}) {
	using T = E::value_type;
	if (sz(a) == 0 || sz(b) == 0) return;
	int s = sz(a) + sz(b) - 1;
	auto [n, cut] = detail::conv_size_for(s);
	T c0 = a[0] * b[0];
	auto buf = buffer_pool<T>::get(n);
	multiply_circular<E>(a, b, buf.span(), n);
	detail::emit_linear<T>(buf.span(), n, s, cut, c0, out, op);
}

template <engine E, typename Op = assign_op>
void multiply(std::span<const typename E::value_type> a, transformed<E>& ta,
		std::span<const typename E::value_type> b, transformed<E>& tb,
		std::span<typename E::value_type> out, Op op = {}) {
	using T = E::value_type;
	if (sz(a) == 0 || sz(b) == 0) return;
	int s = sz(a) + sz(b) - 1;
	auto [n, cut] = detail::conv_size_for(s);
	T c0 = a[0] * b[0];
	E::extend_to(ta, n, a);
	E::extend_to(tb, n, b);
	detail::finish_linear<E>(E::mul(ta, tb, n), n, s, cut, c0, out, op);
}

template <engine E, typename Op = assign_op>
void multiply_add2(std::span<const typename E::value_type> a1, transformed<E>& ta1,
		std::span<const typename E::value_type> b1, transformed<E>& tb1,
		std::span<const typename E::value_type> a2, transformed<E>& ta2,
		std::span<const typename E::value_type> b2, transformed<E>& tb2,
		std::span<typename E::value_type> out, Op op = {}) {
	using T = E::value_type;
	assert(sz(a1) > 0 && sz(b1) > 0 && sz(a2) > 0 && sz(b2) > 0);
	int s = sz(a1) + sz(b1) - 1;
	assert(sz(a2) + sz(b2) - 1 == s);
	auto [n, cut] = detail::conv_size_for(s);
	T c0 = a1[0] * b1[0] + a2[0] * b2[0];
	E::extend_to(ta1, n, a1); E::extend_to(tb1, n, b1);
	E::extend_to(ta2, n, a2); E::extend_to(tb2, n, b2);
	detail::finish_linear<E>(E::mul2(ta1, tb1, ta2, tb2, n), n, s, cut, c0, out, op);
}

// As multiply_add2, but also outputs the summed pointwise product as a reusable
// transform of the (full-length) result, like multiply_cached.
template <engine E>
void multiply_add2_cached(
		std::span<const typename E::value_type> a1, transformed<E>& ta1,
		std::span<const typename E::value_type> b1, transformed<E>& tb1,
		std::span<const typename E::value_type> a2, transformed<E>& ta2,
		std::span<const typename E::value_type> b2, transformed<E>& tb2,
		vec<typename E::value_type>& coeffs, transformed<E>& t) {
	using T = E::value_type;
	assert(sz(a1) > 0 && sz(b1) > 0 && sz(a2) > 0 && sz(b2) > 0);
	int s = sz(a1) + sz(b1) - 1;
	assert(sz(a2) + sz(b2) - 1 == s);
	coeffs = vec<T>(s, T{});
	t = transformed<E>{};
	if constexpr (std::same_as<typename E::product, transformed<E>>) {
		auto [n, cut] = detail::conv_size_for(s);
		T c0 = a1[0] * b1[0] + a2[0] * b2[0];
		E::extend_to(ta1, n, a1); E::extend_to(tb1, n, b1);
		E::extend_to(ta2, n, a2); E::extend_to(tb2, n, b2);
		auto p = E::mul2(ta1, tb1, ta2, tb2, n);
		auto tp = p;
		detail::finish_linear<E>(std::move(p), n, s, cut, c0, std::span<T>(coeffs));
		t = std::move(tp);
	} else {
		multiply_add2<E>(a1, ta1, b1, tb1, a2, ta2, b2, tb2, std::span<T>(coeffs));
	}
}

// This helper also accepts an output transform which will be populated if it is cheap to do so
template <engine E>
void multiply_cached(std::span<const typename E::value_type> a, transformed<E>& ta,
		std::span<const typename E::value_type> b, transformed<E>& tb,
		vec<typename E::value_type>& coeffs, transformed<E>& t) {
	using T = E::value_type;
	coeffs = vec<T>(sz(a) && sz(b) ? sz(a) + sz(b) - 1 : 0, T{});
	t = transformed<E>{};
	if (coeffs.empty()) return;
	int s = sz(coeffs);
	if constexpr (std::same_as<typename E::product, transformed<E>>) {
		auto [n, cut] = detail::conv_size_for(s);
		T c0 = a[0] * b[0];
		E::extend_to(ta, n, a);
		E::extend_to(tb, n, b);
		auto p = E::mul(ta, tb, n);
		auto tp = p;
		detail::finish_linear<E>(std::move(p), n, s, cut, c0, std::span<T>(coeffs));
		t = std::move(tp);
	} else {
		multiply<E>(a, ta, b, tb, std::span<T>(coeffs));
	}
}

template <engine E, typename Op = assign_op>
void square(std::span<const typename E::value_type> a, std::span<typename E::value_type> out, Op op = {}) {
	using T = E::value_type;
	if (sz(a) == 0) return;
	int s = 2 * sz(a) - 1;
	auto [n, cut] = detail::conv_size_for(s);
	T c0 = a[0] * a[0];
	auto buf = buffer_pool<T>::get(n);
	square_circular<E>(a, buf.span(), n);
	detail::emit_linear<T>(buf.span(), n, s, cut, c0, out, op);
}

template <engine E, typename Op = assign_op>
void square(std::span<const typename E::value_type> a, transformed<E>& ta,
		std::span<typename E::value_type> out, Op op = {}) {
	using T = E::value_type;
	if (sz(a) == 0) return;
	int s = 2 * sz(a) - 1;
	auto [n, cut] = detail::conv_size_for(s);
	T c0 = a[0] * a[0];
	E::extend_to(ta, n, a);
	detail::finish_linear<E>(E::sq(ta, n), n, s, cut, c0, out, op);
}

// As square, but also outputs the pointwise product as a reusable transform of
// the result (empty when the engine's product isn't a transform).
template <engine E>
void square_cached(std::span<const typename E::value_type> a, transformed<E>& ta,
		vec<typename E::value_type>& coeffs, transformed<E>& t) {
	using T = E::value_type;
	coeffs = vec<T>(sz(a) ? 2 * sz(a) - 1 : 0, T{});
	t = transformed<E>{};
	if (coeffs.empty()) return;
	int s = sz(coeffs);
	if constexpr (std::same_as<typename E::product, transformed<E>>) {
		auto [n, cut] = detail::conv_size_for(s);
		T c0 = a[0] * a[0];
		E::extend_to(ta, n, a);
		auto p = E::sq(ta, n);
		auto tp = p;
		detail::finish_linear<E>(std::move(p), n, s, cut, c0, std::span<T>(coeffs));
		t = std::move(tp);
	} else {
		square<E>(a, ta, std::span<T>(coeffs));
	}
}

template <engine E> vec<typename E::value_type> multiply(
		std::span<const typename E::value_type> a, std::span<const typename E::value_type> b) {
	using T = E::value_type;
	if (sz(a) == 0 || sz(b) == 0) return {};
	vec<T> r(sz(a) + sz(b) - 1);
	multiply<E>(a, b, std::span<T>(r));
	return r;
}

template <engine E> vec<typename E::value_type> square(std::span<const typename E::value_type> a) {
	using T = E::value_type;
	if (sz(a) == 0) return {};
	vec<T> r(2 * sz(a) - 1);
	square<E>(a, std::span<T>(r));
	return r;
}

namespace detail {
// emit_linear but for middle_product
template <typename T, typename Op>
void emit_middle(std::span<T> buf, bool cut, int la, int lb, T c0, T ctop, std::span<T> out, Op op) {
	int m = la - lb + 1;
	T cn{};
	if (cut) {
		cn = buf[0] - c0; // for lb == 1 these coincide: slot 0 = c_0 + c_n and ctop = c_n
		buf[lb - 1] -= ctop;
	}
	int lim = min(sz(out), cut ? m - 1 : m);
	for (int t = 0; t < lim; t++) op(out[t], buf[lb - 1 + t]);
	if (cut && sz(out) >= m) op(out[m-1], cn);
}
} // namespace detail

// Middle product (the transposed multiplication): takes only coefficients of a * b which include terms from all of b.
// Must have len(a) >= len(b)
template <engine E, typename Op = assign_op>
void middle_product(std::span<const typename E::value_type> a, std::span<const typename E::value_type> b,
		std::span<typename E::value_type> out, Op op = {}) {
	using T = E::value_type;
	if (sz(a) == 0 || sz(b) == 0) return;
	assert(sz(a) >= sz(b));
	if (sz(a) == sz(b)) {
		T r{};
		for (int i = 0; i < sz(a); i++) {
			r += a[i] * b[sz(b) - 1 - i];
		}
		if (sz(out) > 0) op(out[0], r);
		return;
	}
	auto [n, cut] = detail::conv_size_for(sz(a));
	auto buf = buffer_pool<T>::get(n);
	multiply_circular<E>(a, b, buf.span(), n);
	detail::emit_middle<T>(buf.span(), cut, sz(a), sz(b),
			a[0] * b[0], a[sz(a) - 1] * b[sz(b) - 1], out, op);
}

// TODO: Let's decide whether to keep vec<> returning forms or not; this
// largely depends on whether we think these functions are a public interface or
// merely convenience for value type implementors.
template <engine E> vec<typename E::value_type> middle_product(
		std::span<const typename E::value_type> a, std::span<const typename E::value_type> b) {
	using T = E::value_type;
	if (sz(a) == 0 || sz(b) == 0) return {};
	assert(sz(a) >= sz(b));
	vec<T> r(sz(a) - sz(b) + 1);
	middle_product<E>(a, b, std::span<T>(r));
	return r;
}

template <engine E, typename Op = assign_op>
void middle_product(std::span<const typename E::value_type> a, transformed<E>& ta,
		std::span<const typename E::value_type> b, transformed<E>& tb,
		std::span<typename E::value_type> out, Op op = {}) {
	using T = E::value_type;
	if (sz(a) == 0 || sz(b) == 0) return;
	assert(sz(a) >= sz(b));
	if (sz(a) == sz(b)) {
		T r{};
		for (int i = 0; i < sz(a); i++) {
			r += a[i] * b[sz(b) - 1 - i];
		}
		if (sz(out) > 0) op(out[0], r);
		return;
	}
	auto [n, cut] = detail::conv_size_for(sz(a));
	E::extend_to(ta, n, a);
	E::extend_to(tb, n, b);
	auto buf = buffer_pool<T>::get(n);
	E::finish(E::mul(ta, tb, n), buf.span());
	detail::emit_middle<T>(buf.span(), cut, sz(a), sz(b),
			a[0] * b[0], a[sz(a) - 1] * b[sz(b) - 1], out, op);
}

template <engine E>
vec<typename E::value_type> middle_product(std::span<const typename E::value_type> a, transformed<E>& ta,
		std::span<const typename E::value_type> b, transformed<E>& tb) {
	using T = E::value_type;
	if (sz(a) == 0 || sz(b) == 0) return {};
	assert(sz(a) >= sz(b));
	vec<T> r(sz(a) - sz(b) + 1);
	middle_product<E>(a, ta, b, tb, std::span<T>(r));
	return r;
}

} // namespace wala::fft
#line 14 "/home/andrew/dev/cp-book/src/fft/series_core.hpp"

// ==== value types ====

namespace wala::series {

// A series is either exact (a finite series, R[x] sitting inside R[[x]]: the
// length is just the support bound) or trunc (a known prefix of an infinite
// series: the length is the precision, and products truncate to it).

// Non-owning view of power series coefficients: the span pattern (contiguous
// window + series semantics), borrowed from an owning series-like type.
template <fft::engine E, bool exact_>
struct span {
	using T = E::value_type;
	using engine_t = E;
	static constexpr bool exact_v = exact_;

	span() = default;
	explicit span(std::span<const T> s_) : s(s_) {}
	// exact -> trunc is implicit, trunc -> exact is explicit
	template <bool oe> requires (oe != exact_)
	explicit(oe < exact_) span(span<E, oe> o) : s(o.coeffs()) {}

	int len() const { return sz(s); }
	const T& operator[](int i) const { return s[size_t(i)]; }
	auto begin() const { return s.begin(); }
	auto end() const { return s.end(); }
	// engine primitives borrow through std::span's range constructor
	std::span<const T> coeffs() const { return s; }
	// the first n coefficients; requires n <= len().
	// Widening past len() is explicit: see with_len.
	span first(int n) const {
		assert(n <= len());
		return span(s.first(size_t(n)));
	}

private:
	std::span<const T> s;
};

// `vec` represents both exact (finite) power series (R[x]) and prefixes of infinite power series (R[[x]]), depending on the flag.
// `exact` and `trunc` are aliases.
//
// Operators here are typically permissive: they will accept combinations of unequal types and lengths.
template <fft::engine E, bool exact_>
struct vec : public growable_vec<typename E::value_type> {
	using T = E::value_type;
	using engine_t = E;
	static constexpr bool exact_v = exact_;
	using growable_vec<T>::growable_vec;

	// a free const borrow of the coefficients: implicit
	operator span<E, exact_>() const {
		return span<E, exact_>(std::span<const T>(*this));
	}

	// exact -> trunc is implicit, trunc -> exact is explicit
	template <bool oe> requires (oe != exact_)
	explicit(oe < exact_) vec(const vec<E, oe>& p) : growable_vec<T>(p) {}
	template <bool oe> requires (oe != exact_)
	explicit(oe < exact_) vec(vec<E, oe>&& p) : growable_vec<T>(std::move(p)) {}

	// adopt a plain coefficient vector
	explicit vec(wala::vec<T> v) : growable_vec<T>(std::move(v).into_growable()) {}
	// materialize an owned copy of any borrowed series, of either exactness
	explicit vec(span<E, exact_> s) : growable_vec<T>(std::from_range, s.coeffs()) {}
	explicit vec(span<E, !exact_> s) : growable_vec<T>(std::from_range, s.coeffs()) {}

	span<E, exact_> first(int n) const { return span<E, exact_>(*this).first(n); }

	int len() const {
		return int(this->size());
	}
	int degree() const requires (exact_) {
		return len() - 1;
	}
	void extend(int sz) {
		assert(sz >= len());
		this->grow_to(sz);
	}
	void shrink(int sz) {
		assert(sz <= len());
		this->shrink_to(sz);
	}
	// extend or shrink to sz
	void resize(int sz) {
		if (sz > len()) extend(sz);
		else shrink(sz);
	}
	// multiply by x^n within the fixed precision window
	void shift_trunc(int n = 1) requires (!exact_) {
		assert(n >= 0 && n <= len());
		std::rotate(this->begin(), this->end()-n, this->end());
		std::fill(this->begin(), this->begin()+n, T(0));
	}
	// divide by x^n and 0-pad within the fixed precision window
	void unshift_trunc(int n = 1) requires (!exact_) {
		assert(n >= 0 && n <= len());
		std::fill(this->begin(), this->begin()+n, T(0));
		std::rotate(this->begin(), this->begin()+n, this->end());
	}

	// in-place forms require that the result's exactness/length must equal this operand's
	template <bool oe> requires (exact_ <= oe)
	vec& operator += (const vec<E, oe>& o) {
		if constexpr (exact_) { if (o.len() > len()) this->grow_to(o.len()); }
		else if constexpr (!oe) { if (o.len() < len()) this->shrink_to(o.len()); }
		for (int i = 0; i < std::min(len(), o.len()); i++) {
			(*this)[i] += o[i];
		}
		return *this;
	}
	template <bool oe> requires (exact_ <= oe)
	vec& operator -= (const vec<E, oe>& o) {
		if constexpr (exact_) { if (o.len() > len()) this->grow_to(o.len()); }
		else if constexpr (!oe) { if (o.len() < len()) this->shrink_to(o.len()); }
		for (int i = 0; i < std::min(len(), o.len()); i++) {
			(*this)[i] -= o[i];
		}
		return *this;
	}

	vec& operator *= (const T& n) {
		for (auto& v : *this) v *= n;
		return *this;
	}
	friend vec operator * (const vec& a, const T& n) {
		vec r(a.size());
		for (int i = 0; i < a.len(); i++) {
			r[i] = a[i] * n;
		}
		return r;
	}
	friend vec operator * (const T& n, const vec& a) {
		vec r(a.size());
		for (int i = 0; i < a.len(); i++) {
			r[i] = n * a[i];
		}
		return r;
	}

	vec& operator *= (const vec& o) {
		return *this = (*this) * o;
	}
};

template <fft::engine E> using exact = vec<E, true>;
template <fft::engine E> using trunc = vec<E, false>;

// Series-like concepts: the binary operators below are written once as constrained
// templates and dispatch on which memoized transforms an operand carries.
// A series-like type exposes its engine/exactness and its coefficients as a
// span borrow of exactly len() coefficients; cached wrappers additionally
// expose their transform caches (filling them is logically const).
template <typename S>
concept like = fft::engine<typename S::engine_t> && requires(const S& s, int i) {
	{ S::exact_v } -> std::convertible_to<bool>;
	{ s.len() } -> std::same_as<int>;
	{ s[i] } -> std::convertible_to<const typename S::engine_t::value_type&>;
	// borrows straight into the engine primitives
	{ std::span<const typename S::engine_t::value_type>(s) };
	// and into the series layer's own span, keeping the exactness tag
	requires std::convertible_to<const S&, span<typename S::engine_t, S::exact_v>>;
	// first(n): the first n coefficients, borrowed; requires n <= len().
	// The result is itself like (concepts can't self-reference).
	// Cached types keep a cache in the result only when it still serves the whole borrow.
	{ s.first(i) } -> std::convertible_to<span<typename S::engine_t, S::exact_v>>;
};
template <typename S>
concept exact_like = like<S> && S::exact_v;
template <typename S>
concept trunc_like = like<S> && !S::exact_v;

// carries one extendable transform of the whole coefficient sequence
template <typename S>
concept has_cache = like<S> && requires(const S& s) {
	{ s.cache() } -> std::same_as<fft::transformed<typename S::engine_t>&>;
};

template <fft::engine E, bool exact_>
struct maybe_cached;

// A borrowed series paired with the transform serving it: the
// normalized operand form fed to the cached fft:: entry points. Models has_cache.
template <fft::engine E, bool exact_>
struct cached_span {
	using engine_t = E;
	static constexpr bool exact_v = exact_;

	span<E, exact_> s;
	std::reference_wrapper<fft::transformed<E>> f;

	cached_span(span<E, exact_> s_, fft::transformed<E>& f_) : s(s_), f(f_) {}
	// exact -> trunc is implicit, trunc -> exact is explicit
	template <bool oe> requires (oe != exact_)
	explicit(oe < exact_) cached_span(cached_span<E, oe> o) : s(span<E, exact_>(o.s)), f(o.f) {}

	int len() const { return s.len(); }
	const E::value_type& operator[](int i) const { return s[i]; }
	operator std::span<const typename E::value_type>() const { return s.coeffs(); }
	operator span<E, exact_>() const { return s; }
	maybe_cached<E, exact_> first(int n) const;
	fft::transformed<E>& cache() const { return f; }
};

// carries a whole-sequence cache only sometimes, decided at runtime
template <typename S>
concept has_cache_opt = like<S> && requires(const S& s) {
	{ s.cache_opt() } -> std::same_as<std::optional<std::reference_wrapper<fft::transformed<typename S::engine_t>>>>;
};

namespace detail {
// the operand's whole cache, if it carries one
template <like S>
std::optional<std::reference_wrapper<fft::transformed<typename S::engine_t>>> cache_of(const S& s) {
	if constexpr (has_cache<S>) return s.cache();
	else if constexpr (has_cache_opt<S>) return s.cache_opt();
	else return std::nullopt;
}
} // namespace detail

// A borrowed series which may carry the transform serving it: the runtime
// counterpart of cached_span in the borrow hierarchy
// prefix_cached/cached -> maybe_cached/cached_span -> span.
template <fft::engine E, bool exact_>
struct maybe_cached {
	using T = E::value_type;
	using engine_t = E;
	static constexpr bool exact_v = exact_;

	span<E, exact_> s;
	std::optional<std::reference_wrapper<fft::transformed<E>>> f;

	explicit maybe_cached(span<E, exact_> s_) : s(s_) {}
	maybe_cached(span<E, exact_> s_, fft::transformed<E>& f_) : s(s_), f(f_) {}
	maybe_cached(cached_span<E, exact_> c) : s(c.s), f(c.f) {}
	// borrow any like operand whole, taking along whatever cache it carries
	template <like S> requires std::same_as<typename S::engine_t, E> && (S::exact_v == exact_)
	maybe_cached(const S& o) : s(o), f(detail::cache_of(o)) {}

	int len() const { return s.len(); }
	const T& operator[](int i) const { return s[i]; }
	operator std::span<const T>() const { return s.coeffs(); }
	operator span<E, exact_>() const { return s; }
	maybe_cached first(int n) const {
		return n == len() ? *this : maybe_cached(s.first(n));
	}
	std::optional<std::reference_wrapper<fft::transformed<E>>> cache_opt() const { return f; }
};

template <fft::engine E, bool exact_>
maybe_cached<E, exact_> cached_span<E, exact_>::first(int n) const {
	return maybe_cached<E, exact_>(*this).first(n);
}

// An owned series copy at an adjusted logical length: the result of with_len.
// Carries a reference to a source cache when it still serves the copied
// coefficients (a zero tail doesn't change the transform), so the source
// must outlive the result.
template <fft::engine E>
struct resized {
	using T = E::value_type;
	using engine_t = E;
	static constexpr bool exact_v = false;

	trunc<E> s;
	std::optional<std::reference_wrapper<fft::transformed<E>>> f;

	int len() const { return s.len(); }
	const T& operator[](int i) const { return s[i]; }
	operator std::span<const T>() const { return std::span<const T>(s); }
	operator span<E, false>() const { return s; }
	maybe_cached<E, false> first(int n) const {
		span<E, false> v = s;
		if (n == len() && f) return {v, f->get()};
		return maybe_cached<E, false>(v.first(n));
	}
	std::optional<std::reference_wrapper<fft::transformed<E>>> cache_opt() const { return f; }
};

// copy any operand to logical length n: extending zero-fills, shrinking truncates
template <like S>
resized<typename S::engine_t> with_len(const S& s, int n) {
	using E = S::engine_t;
	using T = E::value_type;
	auto p = s.first(std::min(n, s.len()));
	resized<E> r;
	r.s = trunc<E>(n, T{});
	std::span<const T> pc(p);
	std::copy(pc.begin(), pc.end(), r.s.begin());
	r.f = detail::cache_of(p);
	return r;
}

// carries memoized transforms of power-of-two prefixes (see prefix_cached):
// product operands truncate to a covered scale to reuse them.
// Trunc-only: an exact operand participates whole, so has_cache covers it.
template <typename S>
concept has_prefix_cache = like<S> && !S::exact_v && requires(const S& s, int n) {
	{ s.prefix_cache(n) } -> std::same_as<fft::transformed<typename S::engine_t>&>;
};

// Wrapper around vec which caches the transform of the whole series.
// Ops exploit the cache whenever the whole span participates; a trunc series'
// whole-sequence transform is still useful for middle products and repeated
// full-precision use.
template <fft::engine E, bool exact_>
struct cached {
	using T = E::value_type;
	using engine_t = E;
	static constexpr bool exact_v = exact_;

	cached() = default;
	// moving coefficients in or out is free: implicit on rvalues, explicit copy otherwise
	cached(vec<E, exact_>&& s_) : s(std::move(s_)) {}
	explicit cached(const vec<E, exact_>& s_) : s(s_) {}
	operator vec<E, exact_>() && { return std::move(s); }

	int len() const { return s.len(); }
	// unwrap to the owned coefficients
	const vec<E, exact_>& uncached() const { return s; }
	const T& operator[](int i) const { return s[size_t(i)]; }
	auto begin() const { return s.begin(); }
	auto end() const { return s.end(); }
	operator span<E, exact_>() const { return s; }
	maybe_cached<E, exact_> first(int n) const {
		return n == len() ? maybe_cached<E, exact_>(s, f) : maybe_cached<E, exact_>(s.first(n));
	}
	// the transform of the coefficients, fed to the cached fft:: entry points alongside them
	fft::transformed<E>& cache() const { return f; }

	template <like S>
	friend bool operator==(const cached& a, const S& b) {
		span<E, S::exact_v> bs = b;
		return a.len() == bs.len() && std::equal(a.s.begin(), a.s.end(), bs.begin());
	}

private:
	vec<E, exact_> s;
	mutable fft::transformed<E> f; // memoized transform: filling it is logically const
};

template <fft::engine E> using cached_exact = cached<E, true>;
template <fft::engine E> using cached_trunc = cached<E, false>;

namespace detail {
// Normalize a whole-span operand to a cached_span: the coefficients borrowed
// together with the cache serving them (the operand's own, or tmp otherwise).
// The whole-span multiply/square/middle_product paths run entirely on this form.
template <like S>
cached_span<typename S::engine_t, S::exact_v> as_cached_span(const S& s, fft::transformed<typename S::engine_t>& tmp) {
	auto co = cache_of(s);
	return {s, co ? co->get() : tmp};
}
} // namespace detail

// Newton inversion: 1/a mod x^a.len(). Generic over any engine; per doubling step
// n -> m = 2n this is 5 transforms of size m, reusing b's transform for both circular
// products; in each product the wraparound only contaminates coefficients [0, n)
// which are already known.
//
// This is correct for non-commutative rings.
// TODO: reuse/populate the operand's whole/prefix transform caches
template <trunc_like S>
trunc<typename S::engine_t> ps_inv(const S& a) {
	using E = S::engine_t;
	using T = E::value_type;
	int N = a.len();
	trunc<E> r(N, T{});
	if (N == 0) return r;
	int s = nextPow2(N);
	wala::vec<T> b(s, T{});
	b[0] = inv(a[0]);
	for (int n = 1; n < N; n *= 2) {
		int m = 2 * n;
		auto ta = E::transform(a.first(std::min(N, m)), m);
		auto tb = E::transform(std::span<const T>(b).first(n), m);
		// e = a*b mod x^m; only e[n..m) is needed (and is wraparound-free).
		auto e = fft::buffer_pool<T>::get(m);
		E::finish(E::mul(ta, tb, m), e.span());
		for (int i = 0; i < n; i++) e[i] = T{};
		auto te = E::transform(std::span<const T>(e.span()), m);
		auto c = fft::buffer_pool<T>::get(m);
		// b' = 2b - b*(a*b): keep b on the left of e = a*b
		E::finish(E::mul(tb, te, m), c.span());
		for (int i = n; i < std::min(m, N); i++) b[i] = -c[i];
	}
	std::copy(b.begin(), b.begin() + N, r.begin());
	return r;
}
// TODO: operator / can be done slightly faster than ps_inv:
// we only need the n/2 terms of ps_inv(), and can do the last Newton step directly on the quotient

// Both consume whole-sequence transforms by nature (the full span always
// participates), so only whole caches apply, never prefix caches.
template <like A>
auto square(const A& a) {
	using E = A::engine_t;
	using T = E::value_type;
	fft::transformed<E> ta_;
	auto av = detail::as_cached_span(a, ta_);
	if constexpr (A::exact_v) {
		// like operator*, an exact square returns has_cache, adopting the
		// pointwise product as the result's transform when the engine supports it
		wala::vec<T> coeffs;
		fft::transformed<E> f;
		fft::square_cached<E>(av, av.cache(), coeffs, f);
		cached<E, true> w(exact<E>(std::move(coeffs)));
		w.cache() = std::move(f);
		return w;
	} else {
		trunc<E> r(a.len(), T{});
		fft::square<E>(av, av.cache(), std::span<T>(r));
		return r;
	}
}

// a*b + c*d, all exact; returns has_cache, adopting the summed pointwise
// product as the result's transform when the engine supports it. Reuses each
// operand's whole cache. Requires a*b and c*d to have equal length.
template <like A, like B, like C, like D>
	requires fft::same_engine<A, B> && fft::same_engine<A, C> && fft::same_engine<A, D>
		&& A::exact_v && B::exact_v && C::exact_v && D::exact_v
cached<typename A::engine_t, true> multiply_add2(
		const A& a, const B& b, const C& c, const D& d) {
	using E = A::engine_t;
	using T = E::value_type;
	fft::transformed<E> ta_, tb_, tc_, td_;
	auto av = detail::as_cached_span(a, ta_), bv = detail::as_cached_span(b, tb_);
	auto cv = detail::as_cached_span(c, tc_), dv = detail::as_cached_span(d, td_);
	wala::vec<T> coeffs;
	fft::transformed<E> f;
	fft::multiply_add2_cached<E>(
		av, av.cache(),
		bv, bv.cache(),
		cv, cv.cache(),
		dv, dv.cache(),
		coeffs, f
	);
	cached<E, true> w(exact<E>(std::move(coeffs)));
	w.cache() = std::move(f);
	return w;
}

// coefficients [b.len()-1, a.len()) of a*b; requires a.len() >= b.len() > 0.
// The kernel b participates whole, so it must be exact; the result mirrors a's kind.
template <like A, exact_like B> requires fft::same_engine<A, B>
vec<typename A::engine_t, A::exact_v> middle_product(const A& a, const B& b) {
	using E = A::engine_t;
	fft::transformed<E> ta_, tb_;
	auto av = detail::as_cached_span(a, ta_);
	auto bv = detail::as_cached_span(b, tb_);
	return vec<E, A::exact_v>(fft::middle_product<E>(
		av, av.cache(),
		bv, bv.cache()
	));
}

namespace detail {
template <bool ea, bool eb> int product_prec(int la, int lb) {
	if constexpr (ea && eb) return la > 0 && lb > 0 ? la + lb - 1 : 0;
	else return ea ? lb : eb ? la : std::min(la, lb);
}

// Normalize a product operand at the given precision to a borrowed series + the
// whole cache serving it: a prefix cache at scale nextPow2(prec), or the whole
// span with the operand's own cache, or a truncated span with the caller's
// throwaway cache.
// A whole cache of an over-length operand (len > prec, which pins the other,
// necessarily trunc, operand's span at exactly prec) is only worth using when
// the untruncated span doesn't grow the transform size: a 2x'd inverse
// transform costs more than the saved forward transform.
template <like S>
auto product_operand(const S& s, int prec, fft::transformed<typename S::engine_t>& tmp) {
	using E = S::engine_t;
	if constexpr (has_prefix_cache<S>) {
		return s.first(std::min(s.len(), nextPow2(prec)));
	} else {
		span<E, S::exact_v> v = s;
		int used = std::min(v.len(), prec);
		if (auto co = cache_of(s)) {
			if (s.len() <= prec || fft::detail::conv_size_for(s.len() + prec - 1).n
					== fft::detail::conv_size_for(2 * prec - 1).n) {
				return cached_span<E, S::exact_v>{v, co->get()};
			}
		}
		return cached_span<E, S::exact_v>{v.first(used), tmp};
	}
}
} // namespace detail

template <like A, like B> requires fft::same_engine<A, B>
vec<typename A::engine_t, A::exact_v && B::exact_v> operator + (const A& a, const B& b) {
	using T = A::engine_t::value_type;
	int n = (A::exact_v && B::exact_v) ? std::max(a.len(), b.len())
		: A::exact_v ? b.len() : B::exact_v ? a.len() : std::min(a.len(), b.len());
	vec<typename A::engine_t, A::exact_v && B::exact_v> r(size_t(n), T(0));
	for (int i = 0; i < n; i++) {
		r[i] = (i < a.len() ? a[i] : T(0)) + (i < b.len() ? b[i] : T(0));
	}
	return r;
}
template <like A, like B> requires fft::same_engine<A, B>
vec<typename A::engine_t, A::exact_v && B::exact_v> operator - (const A& a, const B& b) {
	using T = A::engine_t::value_type;
	int n = (A::exact_v && B::exact_v) ? std::max(a.len(), b.len())
		: A::exact_v ? b.len() : B::exact_v ? a.len() : std::min(a.len(), b.len());
	vec<typename A::engine_t, A::exact_v && B::exact_v> r(size_t(n), T(0));
	for (int i = 0; i < n; i++) {
		r[i] = (i < a.len() ? a[i] : T(0)) - (i < b.len() ? b[i] : T(0));
	}
	return r;
}

// The single multiplication operator: each operand is normalized to a borrowed
// series + whole cache (see detail::product_operand), then multiplied once.
// An exact x exact product returns a has_cache result, going through
// fft::multiply_cached so the pointwise product is adopted as the result's
// transform whenever the engine supports it.
template <like A, like B> requires fft::same_engine<A, B>
auto operator * (const A& a, const B& b) {
	using E = A::engine_t;
	using T = E::value_type;
	constexpr bool ea = A::exact_v, eb = B::exact_v;
	int prec = detail::product_prec<ea, eb>(a.len(), b.len());
	if (prec == 0 || a.len() == 0 || b.len() == 0) {
		if constexpr (ea && eb) return cached<E, true>{};
		else return trunc<E>(prec, T(0));
	}
	fft::transformed<E> ta_, tb_;
	auto va = detail::product_operand(a, prec, ta_);
	auto vb = detail::product_operand(b, prec, tb_);
	if constexpr (ea && eb) {
		wala::vec<T> coeffs;
		fft::transformed<E> f;
		auto ca = detail::as_cached_span(va, ta_), cb = detail::as_cached_span(vb, tb_);
		fft::multiply_cached<E>(
			ca, ca.cache(),
			cb, cb.cache(),
			coeffs, f
		);
		cached<E, true> w(exact<E>(std::move(coeffs)));
		w.cache() = std::move(f);
		return w;
	} else {
		trunc<E> r(prec, T(0));
		auto ca = detail::as_cached_span(va, ta_), cb = detail::as_cached_span(vb, tb_);
		fft::multiply<E>(
			ca, ca.cache(),
			cb, cb.cache(),
			std::span<T>(r)
		);
		return r;
	}
}

// Wrapper around trunc which caches transform(s[:2^k]) for all k,
// matching the doubling shape of ps_inv/exp so they can populate the caches.
// TODO: make ps_inv/exp populate these
template <fft::engine E>
struct prefix_cached {
	using T = E::value_type;

	using engine_t = E;
	static constexpr bool exact_v = false;

	prefix_cached() = default;
	// moving coefficients in or out is free: implicit on rvalues, explicit copy otherwise
	prefix_cached(trunc<E>&& s_) : s(std::move(s_)) {}
	explicit prefix_cached(const trunc<E>& s_) : s(s_) {}
	operator trunc<E>() && { return std::move(s); }

	int len() const { return s.len(); }
	// unwrap to the owned coefficients
	const trunc<E>& uncached() const { return s; }
	const T& operator[](int i) const { return s[i]; }
	auto begin() const { return s.begin(); }
	auto end() const { return s.end(); }
	operator span<E, false>() const { return s; }

	// extend precision: appends coefficients, keeping all covering caches valid
	void append(std::span<const T> tail) {
		s.append_range(tail);
	}

	// The first k coefficients, with the covering prefix cache when one lines
	// up with k (k a power of two, or k == len()); uncached otherwise.
	maybe_cached<E, false> first(int k) const {
		assert(k <= len());
		span<E, false> v = s.first(k);
		int n = nextPow2(k);
		if (std::min(n, len()) == k) return {v, prefix_cache(n)};
		return maybe_cached<E, false>(v);
	}
	// the whole-sequence transform: the prefix cache covering all of len()
	fft::transformed<E>& cache() const { return prefix_cache(nextPow2(len())); }
	// cache over the prefix of length min(n, len()); n a power of two
	fft::transformed<E>& prefix_cache(int n) const {
		assert(n > 0 && !(n & (n-1)));
		int k = __builtin_ctz(unsigned(n));
		if (k >= sz(caches)) caches.grow_to(k + 1);
		auto& c = caches[k];
		int e = std::min(n, len());
		if (c.len != e) {
			c.t = E::transform(s.first(e), 2 * n);
			c.len = e;
		}
		return c.t;
	}

private:
	trunc<E> s;
	// memoized transforms: logically const; len tracks how much of s each covers
	struct entry { fft::transformed<E> t; int len = 0; };
	mutable growable_vec<entry> caches;
};

} // namespace wala::series
#line 11 "/home/andrew/dev/cp-book/src/fft/series.hpp"

// ==== analytic ops ====
// Free functions over series-like operands; each borrows the operand's span
// and writes a fresh result.
// TODO: reuse/populate the operands' whole/prefix transform caches

namespace wala::series {

template <like S>
vec<typename S::engine_t, S::exact_v> stretch(const S& a_, int n) {
	using E = S::engine_t;
	span<E, S::exact_v> a = a_;
	vec<E, S::exact_v> r(a.len());
	for (int i = 0; i*n < a.len(); i++) {
		r[i*n] = a[i];
	}
	return r;
}
template <like S>
vec<typename S::engine_t, S::exact_v> deriv_shift(const S& a_) {
	using E = S::engine_t;
	span<E, S::exact_v> a = a_;
	vec<E, S::exact_v> r(a);
	for (int i = 0; i < r.len(); i++) {
		r[i] *= i;
	}
	return r;
}
template <like S>
vec<typename S::engine_t, S::exact_v> integ_shift(const S& a_) {
	using E = S::engine_t;
	using T = E::value_type;
	span<E, S::exact_v> a = a_;
	assert(a[0] == 0);
	vec<E, S::exact_v> r(a);
	T f = 1;
	for (int i = 1; i < r.len(); i++) {
		r[i] *= f;
		f *= i;
	}
	f = inv(f);
	for (int i = r.len() - 1; i > 0; i--) {
		r[i] *= f;
		f *= i;
	}
	return r;
}
template <like S>
vec<typename S::engine_t, S::exact_v> integ_shift_offset(const S& a_, int offset) {
	using E = S::engine_t;
	using T = E::value_type;
	span<E, S::exact_v> a = a_;
	vec<E, S::exact_v> r(a);
	T f = 1;
	for (int i = 0; i < r.len(); i++) {
		r[i] *= f;
		f *= i + offset;
	}
	assert(f != 0);
	f = inv(f);
	for (int i = r.len() - 1; i >= 0; i--) {
		r[i] *= f;
		f *= i + offset;
	}
	return r;
}
// OGF <-> EGF of the same sequence: x^k -> x^k / k! and its inverse.
// Diagonal, so valid at any precision.
template <like S>
vec<typename S::engine_t, S::exact_v> ogf_to_egf(const S& a_) {
	using E = S::engine_t;
	using T = E::value_type;
	span<E, S::exact_v> a = a_;
	vec<E, S::exact_v> r(a);
	T f = 1;
	for (int i = 1; i < r.len(); i++) f *= i;
	f = inv(f);
	for (int i = r.len() - 1; i > 0; i--) {
		r[i] *= f;
		f *= i;
	}
	return r;
}
template <like S>
vec<typename S::engine_t, S::exact_v> egf_to_ogf(const S& a_) {
	using E = S::engine_t;
	using T = E::value_type;
	span<E, S::exact_v> a = a_;
	vec<E, S::exact_v> r(a);
	T f = 1;
	for (int i = 1; i < r.len(); i++) {
		f *= i;
		r[i] *= f;
	}
	return r;
}
template <trunc_like S>
trunc<typename S::engine_t> deriv_shift_log(const S& a) {
	return deriv_shift(a) * ps_inv(a);
}
template <trunc_like S>
trunc<typename S::engine_t> ps_log(const S& a) {
	assert(a[0] == 1);
	return integ_shift(deriv_shift_log(a));
}
template <trunc_like S>
trunc<typename S::engine_t> ps_exp(const S& a_) {
	// See https://mathexp.eu/bostan/publications/BoSc09a.pdf for details
	using E = S::engine_t;
	using T = E::value_type;
	span<E, false> a = a_;
	assert(a.len() >= 1);
	assert(a[0] == 0);
	trunc<E> r(1, T(1)); r.reserve_for(a.len());
	trunc<E> invR(1, T(1)); invR.reserve_for(a.len());
	while (r.len() < a.len()) {
		int o_sz = r.len();
		int n_sz = std::min(o_sz * 2, a.len());
		trunc<E> t = deriv_shift(a.first(o_sz));
		fft::multiply_circular<E>(std::span<const T>(t), std::span<const T>(r).first(o_sz), std::span<T>(t), o_sz);
		t = deriv_shift(r) - t;
		t *= invR;
		t.shrink(n_sz - o_sz);
		trunc<E> v(std::from_range, a.coeffs().subspan(o_sz, n_sz - o_sz));
		v -= integ_shift_offset(t, o_sz);
		v *= r;
		r.extend(n_sz);
		std::copy(v.begin(), v.end(), r.begin() + o_sz);
		if (r.len() < a.len()) {
			// double invR via a Newton step
			assert(r.len() == 2 * invR.len());
			int n = invR.len();
			int nn = r.len();
			trunc<E> tmp(4 * n);
			fft::square<E>(std::span<const T>(invR).first(n), std::span<T>(tmp));
			fft::multiply<E>(std::span<const T>(tmp).first(nn), std::span<const T>(r).first(nn), std::span<T>(tmp));
			invR.extend(nn);
			for (int i = n; i < nn; i++) invR[i] = -tmp[i];
		}
	}
	return r;
}
template <trunc_like S>
trunc<typename S::engine_t> ps_pow_monic(const S& a_, typename S::engine_t::value_type k) {
	using E = S::engine_t;
	span<E, false> a = a_;
	if (a.len() == 0) return {};
	assert(a[0] == 1);
	trunc<E> l = ps_log(a_);
	l *= k;
	return ps_exp(l);
}
template <trunc_like S>
trunc<typename S::engine_t> ps_pow(const S& a_, int64_t k) {
	using E = S::engine_t;
	using T = E::value_type;
	span<E, false> a = a_;
	assert(k >= 0);
	if (k == 0) {
		trunc<E> r(a.len(), T(0));
		if (r.len() > 0) r[0] = T(1);
		return r;
	}

	int st = 0;
	while (st < a.len() && a[st] == 0) st++;

	if (st > 0 && k > (a.len() - 1) / st) {
		return trunc<E>(a.len(), T(0));
	}

	trunc<E> r(std::from_range, a.coeffs().subspan(st, a.len() - st * k));
	T leading_coeff = r[0];
	r *= inv(leading_coeff);
	r = ps_pow_monic(r, T(k));
	r *= power(leading_coeff, k);
	r.insert_n(0, int(st * k), T(0));
	assert(r.len() == a.len());
	return r;
}

template <trunc_like S>
trunc<typename S::engine_t> to_newton_sums(const S& a, int deg) {
	auto r = deriv_shift_log(a);
	r[0] = deg;
	for (int i = 1; i < r.len(); i++) r[i] = -r[i];
	return r;
}
template <trunc_like S>
trunc<typename S::engine_t> from_newton_sums(const S& s_, int deg) {
	using E = S::engine_t;
	span<E, false> s = s_;
	assert(s[0] == deg);
	trunc<E> r(s);
	r[0] = 0;
	for (int i = 1; i < r.len(); i++) r[i] = -r[i];
	return ps_exp(integ_shift(std::move(r)));
}

// Calculates prod 1/(1-x^i)^{a[i]}
template <trunc_like S>
trunc<typename S::engine_t> euler_transform(const S& a) {
	using E = S::engine_t;
	trunc<E> r = deriv_shift(a);
	wala::vec<bool> is_prime(r.len(), true);
	for (int p = 2; p < r.len(); p++) {
		if (!is_prime[p]) continue;
		for (int i = 1; i*p < r.len(); i++) {
			r[i*p] += r[i];
			is_prime[i*p] = false;
		}
	}
	return ps_exp(integ_shift(r));
}
template <trunc_like S>
trunc<typename S::engine_t> inverse_euler_transform(const S& a) {
	using E = S::engine_t;
	trunc<E> r = deriv_shift(ps_log(a));
	wala::vec<bool> is_prime(r.len(), true);
	for (int p = 2; p < r.len(); p++) {
		if (!is_prime[p]) continue;
		for (int i = (r.len()-1)/p; i >= 1; i--) {
			r[i*p] -= r[i];
			is_prime[i*p] = false;
		}
	}
	return integ_shift(r);
}

// Helper packed bivariate buffer for Kinoshita-Li composition (arXiv:2404.05177).
//
// The motivation is performing Bostan-Mori (Graeffe root-squaring) to compute
// something like [x^n] P / Q_0(x, y) with deg_y(Q_0) = 1 and deg_x(Q_0) = n.
//
// In each step, we want to compute Q_{i+1}(x^2, y) = Q_i(x, y) * Q_i(-x, y).
// This doubles the degree of y and also lets us truncate x at half the previous
// degree, leaving the total size invariant.
//
// We will store Q as a packed buffer with x as the inner dimension to facilitate easy Q(-x) substitution.
// The inner span will be 2*deg(x), and the outer span will be 2*deg(y).
// As we advance, we will also return the cached transform of Q_i(-x, y) for the caller to use in the numerator.
template <fft::engine E> struct packed_bivariate {
	using T = E::value_type;
	int L, l;
	wala::vec<T> c;

	// Q_0 = 1 - y g(x), deg g < n <= 2^L
	packed_bivariate(int L_, std::span<const T> g) : L(L_), l(0), c(4 << L) {
		c[0] = T(1);
		for (int i = 0; i < sz(g); i++) c[(2 << L) + i] = -g[i];
	}

	fft::transformed<E> advance() {
		int B = 4 << L;
		auto tq = E::transform(std::span<const T>(c), B);
		auto tn = E::negate_arg(tq, B);
		E::finish(
			E::downsample(E::mul(tq, tn, B), B/2, false),
			std::span<T>(c).first(B/2)
		);
		l++;
		// undo the circular wraparound using monicity in y
		for (int i = 0; i < (2 << (L - l)); i++) {
			c[(2 << L) + i] = c[i];
			c[i] = T(0);
		}
		c[2 << L] -= T(1);
		c[0] = T(1);
		// zero x coefficients beyond the level's truncation mod x^(2^(L-l))
		std::fill(c.begin() + (2 << L) + (1 << (L - l)), c.end(), T(0));
		for (int i = 0; i < (2 << L); i += 2 << (L - l)) {
			for (int j = 0; j < (1 << (L - l)); j++) {
				c[i + (1 << (L - l)) + j] = T(0);
			}
		}
		return tn;
	}
};

// Calculates f(g(x)) mod x^n where deg(g) == n
template <trunc_like SF, trunc_like SG> requires fft::same_engine<SF, SG>
trunc<typename SF::engine_t> ps_compose(const SF& f_, const SG& g_) {
	using E = SF::engine_t;
	using T = E::value_type;
	span<E, false> f = f_;
	span<E, false> g = g_;
	if (g.len() == 0) return {};

	int m = f.len();
	int n = g.len();

	// https://arxiv.org/pdf/2404.05177
	// Consider P(y) = f(1/y) has terms from y^{-(m-1)}...y^0 (Laurent series)
	// We want [y^0] P(y) / (1 - y g(x))
	// Let Q_0 = 1 - yg(x)
	// Q_{i+1}(x^2, y) = Q_i(x, y) * Q_i(-x, y) mod x^{ceil(n / 2^i)}
	// deg_y(Q_i) = 2^i, deg_x(Q_i) = ceil(n / 2^i) - 1
	//
	// [y^0] P(y) / Q_l(x^2^l, y) * Q_{l-1}(-x^2^{l-1}, y) * Q_{l-2}(-x^2^{l-2}, y) * ... * Q_0(-x, y)
	// The total y deg of Q_{k-1} ... Q_0 is 2^k-1
	int L = __builtin_ctz(unsigned(nextPow2(n)));
	int B = 4 << L;
	packed_bivariate<E> Q(L, g.coeffs());
	// tneg[l] is the transform of Q_l(-x, y), reused by the pushdown pass below
	bounded_vec<fft::transformed<E>> tneg(with_capacity{L});
	for (int l = 1; l <= L; l++) tneg.push_back(Q.advance());
	trunc<E> P;
	{
		P = trunc<E>(f);
		std::reverse(P.begin(), P.end());
		trunc<E> QL((1 << L) + 1);
		for (int i = 0; i <= (1 << L); i++) {
			QL[i] = Q.c[2 * i];
		}
		QL.resize(m);
		P *= ps_inv(QL);
		std::reverse(P.begin(), P.end());
		P.resize(1 << L);
		std::reverse(P.begin(), P.end());
		P.extend(B);
		for (int i = (1 << L) - 1; i > 0; i--) {
			P[2*i] = P[i];
			P[i] = T(0);
		}
	}
	for (int l = L-1; l >= 0; l--) {
		for (int i = 0; i < (2 << L); i++) {
			if ((2*i) & (1 << (L-l))) P[i] = 0;
		}
		auto tp = E::upsample(E::transform(std::span<const T>(P).first(B/2), B/2), B, false);
		E::finish(E::mul(tneg[l], tp, B), std::span<T>(P));
		for (int i = 0; i < (2 << L); i++) {
			P[i] = P[(2 << L) + i];
			P[(2 << L) + i] = T(0);
		}
	}
	return trunc<E>(P.first(n));
}

// [x^k] p(x)/q(x) (Bostan-Mori) for an exact rational function.
template <exact_like P, exact_like Q> requires fft::same_engine<P, Q>
P::engine_t::value_type kth_term_of_rational_function(
	const P& p,
	const Q& q,
	uint64_t k
) {
	using E = P::engine_t;
	using T = E::value_type;

	assert(q.len() > 0 && q[0] != T(0));
	// Check this here so we avoid accessing p[0]
	if (p.len() == 0) return T(0);

	// Size up in a pretty conservative way
	int d = std::max(p.len() + 1, q.len());
	assert(d >= 2);

	int n = nextPow2((d-1) + d - 1); // >= d

	// Seed the loop transforms from any whole caches; the buffers below hold the
	// current p, q (zero-padded, which extend_to tolerates).
	fft::transformed<E> tq, tp;
	if (auto cq = detail::cache_of(q)) { E::extend_to(cq->get(), n, q); tq = cq->get(); }
	if (auto cp = detail::cache_of(p)) { E::extend_to(cp->get(), n, p); tp = cp->get(); }

	wala::vec<T> p_buf(d-1, T(0));
	std::ranges::copy(std::span<const T>(p), p_buf.begin());
	wala::vec<T> q_buf(d, T(0));
	std::ranges::copy(std::span<const T>(q), q_buf.begin());

	while (k > 0) {
		E::extend_to(tq, n, q_buf);
		auto tnq = E::negate_arg(tq, n);
		E::extend_to(tp, n, p_buf);

		// P <- downsample(P(x) * Q(-x))
		auto ntp = E::downsample(E::mul(tp, tnq, n), n/2, bool(k & 1));
		assert(ntp.size() == n/2);
		if constexpr (std::same_as<typename E::product, typename E::transformed>) {
			tp = ntp;
		} else {
			tp = {};
		}
		E::finish(std::move(ntp), std::span(p_buf));
		k >>= 1;

		// Save the last iteration if we're done
		if (!k) {
			// HACK: fix the constant coefficient of q only
			q_buf[0] *= q_buf[0];
			break;
		}

		// Q <- downsample(Q(x) * Q(-x))
		auto ntq = E::downsample(E::mul(tq, tnq, n), n/2, false);
		assert(ntq.size() == n/2);
		if constexpr (std::same_as<typename E::product, typename E::transformed>) {
			tq = ntq;
		} else {
			tq = {};
		}
		if (n/2 == d-1) {
			// Fix the wraparound
			T v0 = q_buf[0] * q_buf[0];
			E::finish(std::move(ntq), std::span(q_buf).first(d-1));
			q_buf[d-1] = std::exchange(q_buf[0], v0) - v0;
		} else {
			E::finish(std::move(ntq), std::span(q_buf));
		}
	}
	return p_buf[0] * inv(q_buf[0]);
}

// Find the kth term of linearly recurrent sequence S with char poly Q and len(S) >= len(Q)-1
template <trunc_like S, exact_like Q> requires fft::same_engine<S, Q>
S::engine_t::value_type kth_term_of_linear_recurrence(
	const S& s,
	const Q& q,
	uint64_t k
) {
	using E = S::engine_t;
	using T = E::value_type;

	assert(q.len() > 0 && q[0] != T(0));
	assert(s.len() >= q.len()-1);

	// Don't even bother with P so we don't have to do truncation checks
	// TODO: Could use generic multiply for this whole part?
	fft::transformed<E> tq;
	auto q_cached = detail::as_cached_span(q, tq);

	// Compute the prefix and then hard-cast it to exact
	span<E, false> sv = s;
	auto p = exact<E>(sv.first(q.len()-1) * q_cached);
	return kth_term_of_rational_function(p, q_cached, k);
}

} // namespace wala::series
#line 2956 __BASE_FILE__
#endif // endregion

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);
	using num = wala::modnum<998244353>;
	using E = wala::fft::engines::ntt<num>;
	using trunc = wala::series::trunc<E>;


	auto solve = [&](int N, int K) -> num {
		assert(1 <= K && K < N);
		if (K == 1) {
			return 1;
		}

		trunc fact(2*N+1);
		fact[0] = 1;
		for (int i = 1; i <= 2*N; i++) fact[i] = fact[i-1] * num(i);
		trunc ifact(2*N+1);
		ifact[2*N] = inv(fact[2*N]);
		for (int i = 2*N; i >= 1; i--) ifact[i-1] = ifact[i] * num(i);

		trunc one(N+1);
		one[0] = 1;

		trunc c(N+1);
		for (int i = 1; i <= K; i++) {
			c[i] = fact[2*(i-1)] * ifact[i-1] * ifact[i];
		}
		trunc den = ps_inv(one - c);
		trunc r = c * c - c;
		r[1] += num(1);
		r *= den * den;
		r = ps_log(one - r);
		return -r[N] * num(N);
	};
	auto slow = [&](int N, int K) -> num {
		std::vector<int> perm(N);
		for (int i = 0; i < N; i++) {
		}
	};

	int T; std::cin >> T;
	while (T--) {
		int N, K; std::cin >> N >> K;
		std::cout << solve(N, K) << '\n';
	}

	return 0;
}

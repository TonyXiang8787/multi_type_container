#pragma once

#include <type_traits>
#include <cstddef>
#include <array>
#include <utility>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <limits>
#include <iterator>
#include <exception>

namespace multi_type_container {

using IDType = std::make_signed_t<size_t>;

// get index of class in classes
template <class T, class... Ts> struct get_cls_pos;
template <class T, class... Ts>
struct get_cls_pos<T, T, Ts...> : std::integral_constant<size_t, 0> {};
template <class T, class T2, class... Ts>
struct get_cls_pos<T, T2, Ts...> :
	std::integral_constant<std::size_t, 1 + get_cls_pos<d, ds...>::value> {};
template <class T, class... Ts> 
constexpr size_t get_cls_pos_v = get_cls_pos<T, Ts...>::value;

// index in the container
struct Idx {
	size_t idx_vec;
	size_t idx_pos;
};


// main class
template<class... T>
class Container {
public:
	// default constructor

	// emplace stuff
	template<class U, class... Args>
	void emplace(IDType id, Args&&... args) {
		size_t const idx_vec = get_cls_pos_v<U, T...>;
		std::vector<U>& vec = std::get<std::vector<U>>(vectors_);
		size_t const idx_pos = vec.size();

	}

private:
	std::tuple<std::vector<T>...> vectors_;
	std::unordered_map<IDType, Idx> map_;
};

}
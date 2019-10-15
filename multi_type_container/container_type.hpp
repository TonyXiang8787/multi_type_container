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

struct C {
	C(int a1) :a{ a1 } {}
	int a;
};

struct C1 : C {
	C1(int a1, double b1) : C{ a1 }, b{ b1 } {}
	double b;
};

struct C2 : C {
	C2(int a1, uint16_t b1) : C{ a1 }, b{ b1 } {}
	uint16_t b;
};

namespace multi_type_container {

using IDType = std::make_signed_t<size_t>;

// get index of class in classes
template <class T, class... Ts> struct get_cls_pos;
template <class T, class... Ts>
struct get_cls_pos<T, T, Ts...> : std::integral_constant<size_t, 0> {};
template <class T, class T2, class... Ts>
struct get_cls_pos<T, T2, Ts...> :
	std::integral_constant<std::size_t, 1 + get_cls_pos<T, Ts...>::value> {};
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
	// default constructor, assignment operator
	// emplace stuff
	template<class U, class... Args>
	void emplace(IDType id, Args&&... args) {
		// throw if id already exist
		if (map_.find(id) != map_.end()) {
			throw std::exception{};
		}
		size_t const idx_vec = get_cls_pos_v<U, T...>;
		std::vector<U>& vec = std::get<std::vector<U>>(vectors_);
		size_t const idx_pos = vec.size();
		// create object and insert idx to map
		vec.emplace_back(std::forward<Args>(args)...);
		map_[id] = Idx{ idx_vec, idx_pos };
	}

private:
	std::tuple<std::vector<T>...> vectors_;
	std::unordered_map<IDType, Idx> map_;
};

}
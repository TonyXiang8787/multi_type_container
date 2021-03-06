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
#include <functional>

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

using ID = std::make_signed_t<size_t>;

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
	static constexpr size_t N = sizeof...(T);
	// default constructor, assignment operator
	// emplace stuff
	template<class U, class... Args>
	void emplace(ID id, Args&&... args) {
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

	// get item
	template<class U>
	U& get_item(Idx idx) {
		constexpr std::array func_arr{ &Container::get_item_type<U, T>... };
		return std::invoke(func_arr[idx.idx_vec], *this, idx.idx_pos);
	}
	template<class U>
	U& get_item(ID id) {
		auto found = map_.find(id);
		if (found == map_.end()) {
			throw std::exception{};
		}
		return get_item<U>(found->second);
	}

	// size of a vector
	template<class U>
	size_t size_per_type() {
		return std::get<std::vector<U>>(vectors_).size();
	}
	size_t size_per_idx(size_t idx_vec) {
		constexpr std::array func_arr{ &Container::size_per_type<T>... };
		return std::invoke(func_arr[idx_vec], *this);
	}
	// get raw vector
	template<class U>
	std::vector<U>& get_vector() {
		return std::get<std::vector<U>>(vectors_);
	}

private:
	std::tuple<std::vector<T>...> vectors_;
	std::unordered_map<ID, Idx> map_;

	// get item per type
	template<class U1, class U2>
	U1& get_item_type(size_t idx_pos) {
		if constexpr (std::is_base_of_v<U1, U2>) {
			return std::get<std::vector<U2>>(vectors_)[idx_pos];
		}
		else {
			throw std::exception{};
		}
	}

	
};

}
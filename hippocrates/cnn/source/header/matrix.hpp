﻿#pragma once
#include <vector>
#include <cmath>

namespace Convolutional {

class Matrix {
public:
	using element_t = double;
	using elements_t = std::vector<element_t>;
	struct Position {
		std::size_t x = 0;
		std::size_t y = 0;
	};
	struct Size {
		auto operator==(const Size& other) const { return width == other.width && height == other.height; }
		auto operator!=(const Size& other) const { return !operator==(other); }
		template<typename T>
		auto operator+(T amount) { 
			width += static_cast<std::size_t>(amount);
			height += static_cast<std::size_t>(amount); 
			return *this;
		}
		template<typename T>
		auto operator-(T amount) {
			width -= static_cast<std::size_t>(amount);
			height -= static_cast<std::size_t>(amount);
			return *this;
		}
		template<typename T>
		auto operator/(T amount) {
			width = static_cast<std::size_t>(std::round(static_cast<T>(width) / amount));
			height = static_cast<std::size_t>(std::round(static_cast<T>(height) / amount));
			return *this;
		}

		std::size_t width = 0;
		std::size_t height = 0;
	};

public:
	explicit Matrix(Size size);
	explicit Matrix(Size size, element_t value);
	Matrix(const Matrix&) = default;
	Matrix(Matrix&&) = default;

	Matrix& operator= (const Matrix&) = default;
	Matrix& operator= (Matrix&&) = default;

	Matrix& operator+= (const Matrix& other);

	auto AddZeroPadding(Matrix::Size paddingAmount) -> void;
	auto GetSubmatrix(Position position, Size size) const -> Matrix;
	auto ElementAt(Position position) const -> const element_t&;
	auto ElementAt(Position position) -> element_t&;
	auto ElementAt(std::size_t position) const -> const element_t&;
	auto ElementAt(std::size_t position) -> element_t&;

	auto GetSize() const noexcept { return size; }
	auto GetElementCount() const noexcept { return GetSize().height * GetSize().width; }
	
	auto begin() noexcept { return elements.begin(); }
	auto begin() const noexcept { return elements.begin(); }
	auto end() noexcept { return elements.end(); }
	auto end() const noexcept { return elements.end(); }

private:
	Size size;
	elements_t elements;
};

inline auto operator+ (Matrix lhs, const Matrix& rhs) { return lhs += rhs; }


}

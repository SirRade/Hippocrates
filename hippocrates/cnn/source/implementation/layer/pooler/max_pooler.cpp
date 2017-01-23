﻿#include <algorithm>
#include "layer/pooler/max_pooler.hpp"

using namespace Convolutional;
using namespace Convolutional::Layer::Pooler;

auto MaxPooler::ProcessMultiMatrix(const MultiMatrix& multiMatrix) -> MultiMatrix {
	MultiMatrix::dimensions_t dimensions;
	dimensions.reserve(multiMatrix.GetDimensionCount());
	for (auto& submatrix : multiMatrix) {
		const auto size = submatrix.GetSize();
		const auto receptiveField = GetReceptiveField(size);
		const auto stride = GetStride(size);
		Matrix pooledMatrix(GetSizeAfterPooling(size));
		auto currPooledElement = pooledMatrix.begin();
		Matrix::Position pos;
		for (pos.y = 0; pos.y < size.height - receptiveField.height; pos.y += stride.height) {
			for (pos.x = 0; pos.x < size.width - receptiveField.width; pos.x += stride.width) {
				auto receptedMatrix = submatrix.GetSubmatrix(pos, receptiveField);
				*currPooledElement = ProcessMatrix(std::move(receptedMatrix));
				++currPooledElement;
			}
		}
		dimensions.emplace_back(std::move(pooledMatrix));
	}
	return MultiMatrix(dimensions);
}

auto MaxPooler::GetDimensionalityAfterProcessing(MultiMatrix::Dimensionality dimensionality) const noexcept -> MultiMatrix::Dimensionality {
	const auto size = dimensionality.size;
	const auto r = GetReceptiveField(size);
	const auto s = GetStride(size);

	MultiMatrix::Dimensionality newDim;
	newDim.size.height = (size.width - r.width) / s.width + 1;
	newDim.size.width = (size.height - r.height) / s.height + 1;
	newDim.dimensionCount = dimensionality.dimensionCount;
	return newDim;
}

auto MaxPooler::ProcessMatrix(const Matrix& matrix) const -> Matrix::element_t {
	return *std::max_element(matrix.begin(), matrix.end());
}

auto MaxPooler::GetSizeAfterPooling(Matrix::Size originalSize) const -> Matrix::Size {
	const auto recep{GetReceptiveField(originalSize)};
	// TODO jnf: factor in overlapping stride
	return {originalSize.height / recep.height, originalSize.width /recep.width};
}

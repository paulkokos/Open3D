// ----------------------------------------------------------------------------
// -                        Open3D: www.open3d.org                            -
// ----------------------------------------------------------------------------
// The MIT License (MIT)
//
// Copyright (c) 2018 www.open3d.org
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
// ----------------------------------------------------------------------------

#include "open3d/core/TensorKey.h"

#include <sstream>

#include "open3d/core/Tensor.h"
#include "open3d/utility/Console.h"

namespace open3d {
namespace core {

std::string TensorKey::ToString() const { return "bad"; }

TensorKey TensorKey::Index(int64_t index) { return TensorKeyIndex(index); }

TensorKey TensorKey::Slice(utility::optional<int64_t> start,
                           utility::optional<int64_t> stop,
                           utility::optional<int64_t> step) {
    return TensorKeySlice(start, stop, step);
}

TensorKey TensorKey::IndexTensor(const Tensor& index_tensor) {
    return TensorKeyIndexTensor(index_tensor);
}

TensorKey::TensorKeyMode TensorKey::GetMode() const { return mode_; }

int64_t TensorKey::GetIndex() const {
    if (const TensorKeyIndex* tk = dynamic_cast<const TensorKeyIndex*>(this)) {
        return tk->GetIndex();
    } else {
        utility::LogError(
                "TensorKey::GetIndex() is invalid since the TensorKey is not a "
                "TensorKeyIndex instance.");
    }
}

int64_t TensorKey::GetStart() const {
    if (const TensorKeySlice* tk = dynamic_cast<const TensorKeySlice*>(this)) {
        return tk->GetStart();
    } else {
        utility::LogError(
                "TensorKey::GetStart() is invalid since the TensorKey is not a "
                "TensorKeySlice instance.");
    }
}

int64_t TensorKey::GetStop() const {
    if (const TensorKeySlice* tk = dynamic_cast<const TensorKeySlice*>(this)) {
        return tk->GetStop();
    } else {
        utility::LogError(
                "TensorKey::GetStop() is invalid since the TensorKey is not a "
                "TensorKeySlice instance.");
    }
}

int64_t TensorKey::GetStep() const {
    if (const TensorKeySlice* tk = dynamic_cast<const TensorKeySlice*>(this)) {
        return tk->GetStep();
    } else {
        utility::LogError(
                "TensorKey::GetStep() is invalid since the TensorKey is not a "
                "TensorKeySlice instance.");
    }
}

TensorKey TensorKey::UpdateWithDimSize(int64_t dim_size) const {
    if (const TensorKeySlice* tk = dynamic_cast<const TensorKeySlice*>(this)) {
        return tk->UpdateWithDimSize(dim_size);
    } else {
        utility::LogError(
                "TensorKey::UpdateWithDimSize() is invalid since the TensorKey "
                "is not a TensorKeySlice instance.");
    }
}

std::shared_ptr<Tensor> TensorKey::GetIndexTensor() const {
    if (const TensorKeyIndexTensor* tk =
                dynamic_cast<const TensorKeyIndexTensor*>(this)) {
        return tk->GetIndexTensor();
    } else {
        utility::LogError(
                "TensorKey::GetIndexTensor() is invalid since the TensorKey is "
                "not a TensorKeyIndexTensor instance.");
    }
}

int64_t TensorKeyIndex::GetIndex() const { return index_; }

std::string TensorKeyIndex::ToString() const {
    std::stringstream ss;
    ss << "TensorKey::Index(" << index_ << ")";
    return ss.str();
}

int64_t TensorKeySlice::GetStart() const {
    if (start_.has_value()) {
        return start_.value();
    } else {
        utility::LogError("TensorKeyMode::Slice: start is None.");
    }
}

int64_t TensorKeySlice::GetStop() const {
    if (stop_.has_value()) {
        return stop_.value();
    } else {
        utility::LogError("TensorKeyMode::Slice: stop is None.");
    }
}

int64_t TensorKeySlice::GetStep() const {
    if (step_.has_value()) {
        return step_.value();
    } else {
        utility::LogError("TensorKeyMode::Slice: step is None.");
    }
}

TensorKey TensorKeySlice::UpdateWithDimSize(int64_t dim_size) const {
    return TensorKeySlice(start_.has_value() ? start_.value() : 0,
                          stop_.has_value() ? stop_.value() : dim_size,
                          step_.has_value() ? step_.value() : 1);
}

std::string TensorKeySlice::ToString() const {
    std::stringstream ss;
    ss << "TensorKey::Slice(";
    if (start_.has_value()) {
        ss << start_.value();
    } else {
        ss << "None";
    }
    ss << ", ";
    if (stop_.has_value()) {
        ss << stop_.value();
    } else {
        ss << "None";
    }
    ss << ", ";
    if (step_.has_value()) {
        ss << step_.value();
    } else {
        ss << "None";
    }
    ss << ")";
    return ss.str();
}

std::shared_ptr<Tensor> TensorKeyIndexTensor::GetIndexTensor() const {
    return index_tensor_;
}

std::string TensorKeyIndexTensor::ToString() const {
    std::stringstream ss;
    ss << "TensorKey::IndexTensor(" << index_tensor_->ToString() << ")";
    return ss.str();
}

}  // namespace core
}  // namespace open3d

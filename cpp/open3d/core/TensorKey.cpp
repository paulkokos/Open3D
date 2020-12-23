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

TensorKey TensorKey::Index(int64_t index) {
    return TensorKey(TensorKeyMode::Index, index, None, None, None, Tensor());
}

TensorKey TensorKey::Slice(utility::optional<int64_t> start,
                           utility::optional<int64_t> stop,
                           utility::optional<int64_t> step) {
    return TensorKey(TensorKeyMode::Slice, 0, start, stop, step, Tensor());
}

TensorKey TensorKey::IndexTensor(const Tensor& index_tensor) {
    return TensorKey(TensorKeyMode::IndexTensor, 0, None, None, None,
                     index_tensor);
}

TensorKey::TensorKeyMode TensorKey::GetMode() const { return mode_; }

int64_t TensorKey::GetIndex() const {
    AssertMode(TensorKeyMode::Index);
    return index_;
}

int64_t TensorKey::GetStart() const {
    AssertMode(TensorKeyMode::Slice);
    if (start_.has_value()) {
        return start_.value();
    } else {
        utility::LogError("TensorKeyMode::Slice: start is None.");
    }
}

int64_t TensorKey::GetStop() const {
    AssertMode(TensorKeyMode::Slice);
    if (stop_.has_value()) {
        return stop_.value();
    } else {
        utility::LogError("TensorKeyMode::Slice: stop is None.");
    }
}

int64_t TensorKey::GetStep() const {
    AssertMode(TensorKeyMode::Slice);
    if (step_.has_value()) {
        return step_.value();
    } else {
        utility::LogError("TensorKeyMode::Slice: step is None.");
    }
}

std::shared_ptr<Tensor> TensorKey::GetIndexTensor() const {
    AssertMode(TensorKeyMode::IndexTensor);
    return index_tensor_;
}

TensorKey TensorKey::UpdateWithDimSize(int64_t dim_size) const {
    AssertMode(TensorKeyMode::Slice);
    return TensorKey(TensorKeyMode::Slice, 0,
                     start_.has_value() ? start_.value() : 0,
                     stop_.has_value() ? stop_.value() : dim_size,
                     step_.has_value() ? step_.value() : 1, Tensor());
}

TensorKey::TensorKey(TensorKeyMode mode,
                     int64_t index,
                     utility::optional<int64_t> start,
                     utility::optional<int64_t> stop,
                     utility::optional<int64_t> step,
                     const Tensor& index_tensor)
    : mode_(mode),
      index_(index),
      start_(start),
      stop_(stop),
      step_(step),
      index_tensor_(std::make_shared<Tensor>(index_tensor)) {}

std::string TensorKey::ToString() const {
    std::stringstream ss;
    if (mode_ == TensorKeyMode::Index) {
        ss << "TensorKey::Index(" << index_ << ")";
    } else if (mode_ == TensorKeyMode::Slice) {
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
    } else if (mode_ == TensorKeyMode::IndexTensor) {
        ss << "TensorKey::IndexTensor(" << index_tensor_->ToString() << ")";
    } else {
        utility::LogError("Wrong TensorKeyMode.");
    }
    return ss.str();
};

std::string TensorKeyIndex::ToString() const {
    std::stringstream ss;
    ss << "TensorKey::Index(" << index_ << ")";
    return ss.str();
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

std::string TensorKeyIndexTensor::ToString() const {
    std::stringstream ss;
    ss << "TensorKey::IndexTensor(" << index_tensor_->ToString() << ")";
    return ss.str();
}

}  // namespace core
}  // namespace open3d

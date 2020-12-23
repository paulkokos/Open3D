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

utility::nullopt_t None = utility::nullopt;

TensorKey TensorKey::Index(int64_t index) {
    return TensorKey(TensorKeyMode::Index, index, 0, 0, 0, false, false, false,
                     Tensor());
}

TensorKey TensorKey::Slice(utility::optional<int64_t> start,
                           utility::optional<int64_t> stop,
                           utility::optional<int64_t> step) {
    return Slice(start.has_value() ? start.value() : 0,
                 stop.has_value() ? stop.value() : 0,
                 step.has_value() ? step.value() : 0, !start.has_value(),
                 !stop.has_value(), !step.has_value());
}

TensorKey TensorKey::IndexTensor(const Tensor& index_tensor) {
    return TensorKey(TensorKeyMode::IndexTensor, 0, 0, 0, 0, false, false,
                     false, index_tensor);
}

std::shared_ptr<Tensor> TensorKey::GetIndexTensor() const {
    AssertMode(TensorKeyMode::IndexTensor);
    return index_tensor_;
}

TensorKey TensorKey::UpdateWithDimSize(int64_t dim_size) const {
    AssertMode(TensorKeyMode::Slice);
    return TensorKey(
            TensorKeyMode::Slice, 0, StartIsNone() ? 0 : start_.value(),
            StopIsNone() ? dim_size : stop_.value(),
            StepIsNone() ? 1 : step_.value(), false, false, false, Tensor());
}

TensorKey TensorKey::Slice(int64_t start,
                           int64_t stop,
                           int64_t step,
                           bool start_is_none,
                           bool stop_is_none,
                           bool step_is_none) {
    return TensorKey(TensorKeyMode::Slice, 0, start, stop, step, start_is_none,
                     stop_is_none, step_is_none, Tensor());
}

TensorKey::TensorKey(TensorKeyMode mode,
                     int64_t index,
                     int64_t start,
                     int64_t stop,
                     int64_t step,
                     bool start_is_none,
                     bool stop_is_none,
                     bool step_is_none,
                     const Tensor& index_tensor)
    : mode_(mode),
      index_(index),
      index_tensor_(std::make_shared<Tensor>(index_tensor)) {
    if (start_is_none) {
        start_ = utility::nullopt;
    } else {
        start_ = start;
    }
    if (stop_is_none) {
        stop_ = utility::nullopt;
    } else {
        stop_ = stop;
    }
    if (step_is_none) {
        step_ = utility::nullopt;
    } else {
        step_ = step;
    }
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
        if (StartIsNone()) {
            ss << "None";
        } else {
            ss << start_.value();
        }
        ss << ", ";
        if (StopIsNone()) {
            ss << "None";
        } else {
            ss << stop_.value();
        }
        ss << ", ";
        if (StepIsNone()) {
            ss << "None";
        } else {
            ss << step_.value();
        }
        ss << ")";
    } else if (mode_ == TensorKeyMode::IndexTensor) {
        ss << "TensorKey::IndexTensor(" << index_tensor_->ToString() << ")";
    } else {
        utility::LogError("Wrong TensorKeyMode.");
    }
    return ss.str();
};

}  // namespace core
}  // namespace open3d

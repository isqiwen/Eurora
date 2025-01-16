#include <memory>
#include <numeric>
#include <stdexcept>
#include <unsupported/Eigen/CXX11/Tensor>
#include <vector>

#include "ndarray.h"

namespace eurora::core {

template <typename T>
class NDArrayEigen : public NDArray<T> {
public:
    using typename NDArray<T>::element_type;

    NDArrayEigen() = default;

    ~NDArrayEigen() override { Clear(); }

    void Create(const std::vector<size_t>& dimensions) override {
        if (dimensions.empty() || std::any_of(dimensions.begin(), dimensions.end(), [](size_t d) { return d == 0; })) {
            throw std::runtime_error("Invalid dimensions: dimensions must be non-empty and greater than 0");
        }

        dimensions_    = dimensions;
        size_          = std::accumulate(dimensions.begin(), dimensions.end(), size_t(1), std::multiplies<>());
        manage_memory_ = true;

        data_ = new T[size_];
        SetupTensor();
    }

    void Create(const std::vector<size_t>& dimensions, T* data, bool manage_memory = false) override {
        dimensions_    = dimensions;
        size_          = std::accumulate(dimensions.begin(), dimensions.end(), size_t(1), std::multiplies<>());
        manage_memory_ = manage_memory;

        data_ = data;
        SetupTensor();
    }

    void Clear() override {
        if (manage_memory_ && data_) {
            delete[] data_;
        }

        data_ = nullptr;
        dimensions_.clear();
        size_          = 0;
        manage_memory_ = false;
    }

    void Reshape(const std::vector<size_t>& dimensions) {
        size_t new_size = std::accumulate(dimensions.begin(), dimensions.end(), size_t(1), std::multiplies<>());
        if (new_size != size_) {
            throw std::runtime_error("NDArrayEigen::Reshape: Number of elements cannot change.");
        }
        dimensions_ = dimensions;
        UpdateTensorMap();
    }

    void Squeeze() {
        std::vector<size_t> squeezed_dimensions;
        for (auto dim : dimensions_) {
            if (dim != 1) {
                squeezed_dimensions.push_back(dim);
            }
        }
        dimensions_ = squeezed_dimensions;
        UpdateTensorMap();
    }

    T& operator[](size_t index) {
        if (index >= size_) {
            throw std::runtime_error("NDArrayEigen::[]: element index out of range.");
        }

        return data_[index];
    }

    const T& operator[](size_t index) const {
        if (index >= size_) {
            throw std::runtime_error("NDArrayEigen::[]: element index out of range.");
        }
    }

    T* Data() { return data_; }

    const T* Data() const { return data_; }

    size_t Size() const { return size_; }

    size_t Size(size_t dimension) const {
        if (dimension >= dimensions_.size()) {
            throw std::runtime_error("NDArrayEigen::Size: Dimension index out of range.");
        }
        return dimensions_[dimension];
    }

    const std::vector<size_t>& Dimensions() const { return dimensions_; }

    size_t CalculateOffset(const std::vector<size_t>& indices) const {
        if (indices.size() != dimensions_.size()) {
            throw std::runtime_error("NDArrayEigen::CalculateOffset: Dimension mismatch.");
        }
        return std::inner_product(indices.begin(), indices.end(), offset_factors_.begin(), size_t(0));
    }

    std::vector<size_t> CalculateIndex(size_t offset) const {
        if (offset >= size_) {
            throw std::runtime_error("NDArrayEigen::CalculateIndex: Offset out of range.");
        }
        std::vector<size_t> indices(dimensions_.size(), 0);
        for (size_t i = dimensions_.size(); i > 0; --i) {
            indices[i - 1] = offset / offset_factors_[i - 1];
            offset %= offset_factors_[i - 1];
        }
        return indices;
    }

    std::shared_ptr<NDArray<T>> Subarray(const std::vector<size_t>& start, const std::vector<size_t>& size) const override {
        if (start.size() != dimensions_.size() || size.size() != dimensions_.size()) {
            throw std::runtime_error("NDArrayEigen::subarray: Dimension mismatch");
        }

        for (size_t i = 0; i < dimensions_.size(); ++i) {
            if (start[i] + size[i] > dimensions_[i]) {
                throw std::runtime_error("NDArrayEigen::subarray: Out of bounds");
            }
        }

        auto view            = std::make_shared<NDArrayEigen<T>>();
        view->dimensions_    = size;
        view->size_          = std::accumulate(size.begin(), size.end(), size_t(1), std::multiplies<>());
        view->data_          = data_ + CalculateOffset(start);
        view->manage_memory_ = false;

        view->SetupTensor();
        return view;
    }

    std::shared_ptr<NDArray<T>> ReshapeView(const std::vector<size_t>& dimensions) const override {
        size_t new_elements = std::accumulate(dimensions.begin(), dimensions.end(), size_t(1), std::multiplies<>());
        if (new_elements != size_) {
            throw std::runtime_error("NDArrayEigen::reshape_view: Element count mismatch");
        }

        auto view            = std::make_shared<NDArrayEigen<T>>();
        view->dimensions_    = dimensions;
        view->size_          = size_;
        view->data_          = data_;
        view->manage_memory_ = false;

        view->SetupTensor();
        return view;
    }

private:
    void UpdateOffsetFactors() {
        offset_factors_.resize(dimensions_.size());
        size_t factor = 1;
        for (size_t i = dimensions_.size(); i > 0; --i) {
            offset_factors_[i - 1] = factor;
            factor *= dimensions_[i - 1];
        }
    }

    void UpdateTensorMap() {
        Eigen::array<Eigen::Index, Eigen::Dynamic> eigen_dims(dimensions_.size());
        for (size_t i = 0; i < dimensions_.size(); ++i) {
            eigen_dims[i] = static_cast<Eigen::Index>(dimensions_[i]);
        }
        tensor_ = Eigen::TensorMap<Eigen::Tensor<T, Eigen::Dynamic>>(data_, eigen_dims);
    }

    void SetupTensor() {
        Eigen::array<Eigen::Index, Eigen::Dynamic> eigen_dims(dimensions_.size());
        for (size_t i = 0; i < dimensions_.size(); ++i) {
            eigen_dims[i] = static_cast<Eigen::Index>(dimensions_[i]);
        }
        tensor_ = Eigen::TensorMap<Eigen::Tensor<T, Eigen::Dynamic>>(data_, eigen_dims);
    }

    std::vector<size_t> dimensions_;
    std::vector<size_t> offset_factors_;
    size_t size_        = 0;
    T* data_            = nullptr;
    bool manage_memory_ = false;
    Eigen::TensorMap<Eigen::Tensor<T, Eigen::Dynamic>> tensor_;
};

}  // namespace eurora::core

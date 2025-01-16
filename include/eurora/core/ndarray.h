#pragma once

#include <cstddef>
#include <stdexcept>
#include <vector>

namespace eurora::core {

template <typename T>
class NDArray {
public:
    NDArray() : data_(0), elements_(0), delete_data_on_destruct_(true) {}

    virtual ~NDArray() {}

    virtual void Create(const std::vector<size_t>& dimensions);
    virtual void Create(const std::vector<size_t>& dimensions, T* data, bool delete_data_on_destruct = false);

    virtual void Squeeze();

    virtual void Reshape(const std::vector<int64_t>& dims) = 0;

    /**
    * Reshapes the array to the given dimensions.
    * One of the dimensions can be -1, in which case that dimension will be calculated based on the other.
    * @param dims
    */
    void Reshape(std::initializer_list<std::int64_t> dims);

    template <typename... INDICES>
    void Reshape(INDICES... dims) {
        static_assert((std::is_integral_v<INDICES> && ...), "All indices must be integral types.");

        std::vector<size_t> dimensions{static_cast<size_t>(dims)...};

        this->Reshape(dimensions);
    }

    bool DimensionsEqual(const std::vector<size_t>* d) const { return this->DimensionsEqual(*d); }

    bool DimensionsEqual(const std::vector<size_t>& d) const;

    template <class S>
    bool DimensionsEqual(const NDArray<S>* a) const {
        return this->DimensionsEqual(*a);
    }

    template <class S>
    bool DimensionsEqual(const NDArray<S>& a) const {
        std::vector<size_t> dim;
        a.get_dimensions(dim);

        if (this->dimensions_.size() != dim.size())
            return false;

        size_t NDim = this->dimensions_.size();
        for (size_t d = 0; d < NDim; d++) {
            if (this->dimensions_[d] != dim[d])
                return false;
        }

        return true;
    }

    size_t GetNumberOfDimensions() const;

    size_t GetSize(size_t dimension) const;

    std::vector<size_t> GetDimensions() const;
    void GetDimensions(std::vector<size_t>& dim) const;

    std::vector<size_t> const& Dimensions() const;

    const T* GetDataPtr() const;
    T* GetDataPtr();

    const T* Data() const;
    T* Data();

    size_t Size() const;
    size_t GetNumberOfElements() const;

    bool Empty() const;

    size_t GetNumberOfBytes() const;

    bool DeleteDataOnDestruct() const;
    void DeleteDataOnDestruct(bool d);

    size_t CalculateOffset(const std::vector<size_t>& ind) const;
    static size_t CalculateOffset(const std::vector<size_t>& ind, const std::vector<size_t>& offset_factors);

    size_t CalculateOffset(size_t x, size_t y) const;
    size_t CalculateOffset(size_t x, size_t y, size_t z) const;
    size_t CalculateOffset(size_t x, size_t y, size_t z, size_t s) const;
    size_t CalculateOffset(size_t x, size_t y, size_t z, size_t s, size_t p) const;
    size_t CalculateOffset(size_t x, size_t y, size_t z, size_t s, size_t p, size_t r) const;
    size_t CalculateOffset(size_t x, size_t y, size_t z, size_t s, size_t p, size_t r, size_t a) const;
    size_t CalculateOffset(size_t x, size_t y, size_t z, size_t s, size_t p, size_t r, size_t a, size_t q) const;
    size_t CalculateOffset(size_t x, size_t y, size_t z, size_t s, size_t p, size_t r, size_t a, size_t q, size_t u) const;

    size_t GetOffsetFactor(size_t dim) const;
    void GetOffsetFactor(std::vector<size_t>& offset) const;
    std::vector<size_t> GetOffsetFactor() const;

    size_t GetOffsetFactorLastDim() const;

    void CalculateOffsetFactors(const std::vector<size_t>& dimensions);
    static void CalculateOffsetFactors(const std::vector<size_t>& dimensions, std::vector<size_t>& offset_factors);

    std::vector<size_t> CalculateIndex(size_t offset) const;
    void CalculateIndex(size_t offset, std::vector<size_t>& index) const;

    void Clear();

    bool PointInRange(const std::vector<size_t>& ind) const;
    bool PointInRange(size_t x) const;
    bool PointInRange(size_t x, size_t y) const;
    bool PointInRange(size_t x, size_t y, size_t z) const;
    bool PointInRange(size_t x, size_t y, size_t z, size_t s) const;
    bool PointInRange(size_t x, size_t y, size_t z, size_t s, size_t p) const;
    bool PointInRange(size_t x, size_t y, size_t z, size_t s, size_t p, size_t r) const;
    bool PointInRange(size_t x, size_t y, size_t z, size_t s, size_t p, size_t r, size_t a) const;
    bool PointInRange(size_t x, size_t y, size_t z, size_t s, size_t p, size_t r, size_t a, size_t q) const;
    bool PointInRange(size_t x, size_t y, size_t z, size_t s, size_t p, size_t r, size_t a, size_t q, size_t u) const;

    virtual T& operator[](size_t index)             = 0;
    virtual const T& operator[](size_t index) const = 0;

    virtual T* Data()             = 0;
    virtual const T* Data() const = 0;

    virtual size_t Size() const                           = 0;
    virtual size_t Size(size_t dimension) const           = 0;
    virtual const std::vector<size_t>& Dimensions() const = 0;

    virtual size_t CalculateOffset(const std::vector<size_t>& indices) const = 0;
    size_t CalculateOffset(size_t x, size_t y, size_t z) const               = 0;
    size_t CalculateOffset(size_t x, size_t y, size_t z) const               = 0;

    virtual std::vector<size_t> CalculateIndex(size_t offset) const = 0;

    virtual std::shared_ptr<NDArray<T>> Subarray(const std::vector<size_t>& start, const std::vector<size_t>& size) const = 0;
    virtual std::shared_ptr<NDArray<T>> ReshapeView(const std::vector<size_t>& dimensions) const                          = 0;

protected:
    virtual void AllocateMemory()   = 0;
    virtual void DeallocateMemory() = 0;

protected:
    std::vector<size_t> dimensions_;
    std::vector<size_t> offset_factors_;
    size_t elements_    = 0;
    T* data_            = nullptr;
    bool manage_memory_ = false;
};

template <typename T>
inline void NDArray<T>::Create(const std::vector<size_t>& dimensions) {
    dimensions_ = dimensions;
    AllocateMemory();
    CalculateOffsetFactors(dimensions_);
}

template <typename T>
void NDArray<T>::Create(const std::vector<size_t>& dimensions, T* data, bool delete_data_on_destruct) {
    if (!data)
        throw std::runtime_error("NDArray<T>::Create(): 0x0 pointer provided");
    dimensions_                    = dimensions;
    this->data_                    = data;
    this->delete_data_on_destruct_ = delete_data_on_destruct;
    this->elements_                = 1;
    for (size_t i = 0; i < this->dimensions_.size(); i++) {
        this->elements_ *= this->dimensions_[i];
    }
    CalculateOffsetFactors(dimensions_);
}

template <typename T>
void NDArray<T>::Reshape(std::initializer_list<std::int64_t> dims) {
    std::vector<std::int64_t> dim_vec(dims);
    auto negatives = std::count(dims.begin(), dims.end(), -1);
    if (negatives > 1)
        throw std::runtime_error("Only a single reshape dimension can be negative");

    if (negatives == 1) {
        auto elements    = std::accumulate(dims.begin(), dims.end(), -1, std::multiplies<std::int64_t>());
        auto neg_element = std::find(dim_vec.begin(), dim_vec.end(), -1);
        *neg_element     = this->size_ / elements;
    }

    auto new_dims = std::vector<size_t>(dim_vec.begin(), dim_vec.end());
    this->Reshape(new_dims);
}

template <typename T>
inline bool NDArray<T>::DimensionsEqual(const std::vector<size_t>& d) const {
    if (this->dimensions_.size() != d.size())
        return false;

    size_t NDim = this->dimensions_.size();
    for (size_t ii = 0; ii < NDim; ii++) {
        if (this->dimensions_[ii] != d[ii])
            return false;
    }

    return true;
}

template <typename T>
inline size_t NDArray<T>::GetNumberOfDimensions() const {
    return (size_t)dimensions_.size();
}

template <typename T>
inline size_t NDArray<T>::GetSize(size_t dimension) const {
    if (dimension >= dimensions_.size()) {
        return 1;
    } else {
        return dimensions_[dimension];
    }
}

template <typename T>
inline std::vector<size_t> NDArray<T>::GetDimensions() const {
    return dimensions_;
}

template <typename T>
inline void NDArray<T>::GetDimensions(std::vector<size_t>& dim) const {
    dim = dimensions_;
}

template <class T>
inline std::vector<size_t> const& NDArray<T>::Dimensions() const {
    return dimensions_;
}

template <typename T>
inline const T* NDArray<T>::GetDataPtr() const {
    return data_;
}

template <typename T>
inline T* NDArray<T>::GetDataPtr() {
    return data_;
}

template <typename T>
const T* NDArray<T>::Data() const {
    return data_;
}

template <typename T>
T* NDArray<T>::Data() {
    return data_;
}

template <class T>
inline size_t NDArray<T>::Size() const {
    return elements_;
}

template <class T>
inline bool NDArray<T>::Empty() const {
    return elements_ == 0;
}

template <class T>
inline size_t NDArray<T>::GetNumberOfElements() const {
    return Size();
}

template <typename T>
inline size_t NDArray<T>::GetNumberOfBytes() const {
    return Size() * sizeof(T);
}

template <typename T>
inline bool NDArray<T>::DeleteDataOnDestruct() const {
    return delete_data_on_destruct_;
}

template <typename T>
inline void NDArray<T>::DeleteDataOnDestruct(bool d) {
    delete_data_on_destruct_ = d;
}

template <typename T>
size_t NDArray<T>::CalculateOffset(const std::vector<size_t>& ind, const std::vector<size_t>& offset_factors) {
    if (ind.size() != offset_factors.size()) {
        throw std::invalid_argument("Dimension mismatch between indices and offset factors.");
    }

    return std::inner_product(indices.begin(), indices.end(), offset_factors.begin(), size_t(0));
}

template <typename T>
inline size_t NDArray<T>::CalculateOffset(const std::vector<size_t>& ind) const {
    return CalculateOffset(ind, offset_factors_);
}

template <typename T>
inline size_t NDArray<T>::CalculateOffset(size_t x, size_t y) const {
    return x * offset_factors_[0] + y;
}

template <typename T>
inline size_t NDArray<T>::CalculateOffset(size_t x, size_t y, size_t z) const {
    return x * offset_factors_[0] + y * offset_factors_[1] + z;
}

template <typename T>
inline size_t NDArray<T>::CalculateOffset(size_t x, size_t y, size_t z, size_t s) const {
    return x * offset_factors_[0] + y * offset_factors_[1] + z * offset_factors_[2] + s;
}

template <typename T>
inline size_t NDArray<T>::CalculateOffset(size_t x, size_t y, size_t z, size_t s, size_t p) const {
    return x * offset_factors_[0] + y * offset_factors_[1] + z * offset_factors_[2] + s * offset_factors_[3] + p;
}

template <typename T>
inline size_t NDArray<T>::CalculateOffset(size_t x, size_t y, size_t z, size_t s, size_t p, size_t r) const {
    return x * offset_factors_[0] + y * offset_factors_[1] + z * offset_factors_[2] + s * offset_factors_[3] + p * offset_factors_[4] + r;
}

template <typename T>
inline size_t NDArray<T>::CalculateOffset(size_t x, size_t y, size_t z, size_t s, size_t p, size_t r, size_t a) const {
    return x * offset_factors_[0] + y * offset_factors_[1] + z * offset_factors_[2] + s * offset_factors_[3] + p * offset_factors_[4] + r * offset_factors_[5] +
           a;
}

template <typename T>
inline size_t NDArray<T>::CalculateOffset(size_t x, size_t y, size_t z, size_t s, size_t p, size_t r, size_t a, size_t q) const {
    return x * offset_factors_[0] + y * offset_factors_[1] + z * offset_factors_[2] + s * offset_factors_[3] + p * offset_factors_[4] + r * offset_factors_[5] +
           a * offset_factors_[6] + q;
}

template <typename T>
inline size_t NDArray<T>::CalculateOffset(size_t x, size_t y, size_t z, size_t s, size_t p, size_t r, size_t a, size_t q, size_t u) const {
    return x * offset_factors_[0] + y * offset_factors_[1] + z * offset_factors_[2] + s * offset_factors_[3] + p * offset_factors_[4] + r * offset_factors_[5] +
           a * offset_factors_[6] + q * offset_factors_[7] + u;
}

template <typename T>
inline size_t NDArray<T>::GetOffsetFactor(size_t dim) const {
    if (dim >= dimensions_.size())
        throw std::runtime_error("NDArray<T>::GetOffsetFactor : index out of range");
    return offset_factors_[dim];
}

template <typename T>
inline void NDArray<T>::GetOffsetFactor(std::vector<size_t>& offset) const {
    offset = std::vector<size_t>(offset_factors_.begin(), offset_factors_.end());
}

template <typename T>
inline size_t NDArray<T>::GetOffsetFactorLastDim() const {
    if (dimensions_.size() == 0)
        throw std::runtime_error("NDArray<T>::GetOffsetFactorLastDim : array is empty");

    return GetOffsetFactor(dimensions_.size() - 1);
}

template <typename T>
inline std::vector<size_t> NDArray<T>::GetOffsetFactor() const {
    return std::vector<size_t>(offset_factors_.begin(), offset_factors_.end());
}

template <typename T>
void NDArray<T>::CalculateOffsetFactors(const std::vector<size_t>& dimensions, std::vector<size_t>& offset_factors) {
    size_t num_dimensions = dimensions.size();
    offset_factors.resize(num_dimensions);

    size_t current_factor = 1;
    for (size_t i = num_dimensions; i > 0; --i) {
        offset_factors[i - 1] = current_factor;
        current_factor *= dimensions[i - 1];
    }
}

template <typename T>
inline void NDArray<T>::CalculateOffsetFactors(const std::vector<size_t>& dimensions) {
    CalculateOffsetFactors(dimensions, offset_factors_);
}

template <typename T>
inline std::vector<size_t> NDArray<T>::CalculateIndex(size_t offset) const {
    if (dimensions_.size() == 0)
        throw std::runtime_error("NDArray<T>::CalculateIndex : array is empty");

    std::vector<size_t> index(dimensions_.size());
    for (size_t i = 0; i < dimensions_.size(); ++i) {
        index[i] = offset / offset_factors_[i];
        offset %= offset_factors_[i];
    }

    return index;
}

template <typename T>
inline void NDArray<T>::CalculateIndex(size_t offset, std::vector<size_t>& index) const {
    if (dimensions_.size() == 0)
        throw std::runtime_error("NDArray<T>::CalculateIndex : array is empty");

    index = CalculateIndex(offset);
}

template <typename T>
void NDArray<T>::Clear() {
    if (this->delete_data_on_destruct_) {
        this->DeallocateMemory();
    } else {
        throw std::runtime_error("NDArray<T>::clear : trying to reallocate memory not owned by array.");
    }

    this->data_     = 0;
    this->elements_ = 0;
    this->dimensions_.clear();
    this->offset_factors_.clear();
}

template <typename T>
inline bool NDArray<T>::PointInRange(const std::vector<size_t>& ind) const {
    unsigned int D = dimensions_.size();
    if (ind.size() != D)
        return false;

    unsigned int ii;
    for (ii = 0; ii < D; ii++) {
        if (ind[ii] >= dimensions_[ii]) {
            return false;
        }
    }

    return true;
}

template <typename T>
inline bool NDArray<T>::PointInRange(size_t x) const {
    return (x < dimensions_[0]);
}

template <typename T>
inline bool NDArray<T>::PointInRange(size_t x, size_t y) const {
    return ((x < dimensions_[0]) && (y < dimensions_[1]));
}

template <typename T>
inline bool NDArray<T>::PointInRange(size_t x, size_t y, size_t z) const {
    return ((x < dimensions_[0]) && (y < dimensions_[1]) && (z < dimensions_[2]));
}

template <typename T>
inline bool NDArray<T>::PointInRange(size_t x, size_t y, size_t z, size_t s) const {
    return ((x < dimensions_[0]) && (y < dimensions_[1]) && (z < dimensions_[2]) && (s < dimensions_[3]));
}

template <typename T>
inline bool NDArray<T>::PointInRange(size_t x, size_t y, size_t z, size_t s, size_t p) const {
    return ((x < dimensions_[0]) && (y < dimensions_[1]) && (z < dimensions_[2]) && (s < dimensions_[3]) && (p < dimensions_[4]));
}

template <typename T>
inline bool NDArray<T>::PointInRange(size_t x, size_t y, size_t z, size_t s, size_t p, size_t r) const {
    return ((x < dimensions_[0]) && (y < dimensions_[1]) && (z < dimensions_[2]) && (s < dimensions_[3]) && (p < dimensions_[4]) && (r < dimensions_[5]));
}

template <typename T>
inline bool NDArray<T>::PointInRange(size_t x, size_t y, size_t z, size_t s, size_t p, size_t r, size_t a) const {
    return ((x < dimensions_[0]) && (y < dimensions_[1]) && (z < dimensions_[2]) && (s < dimensions_[3]) && (p < dimensions_[4]) && (r < dimensions_[5]) &&
            (a < dimensions_[6]));
}

template <typename T>
inline bool NDArray<T>::PointInRange(size_t x, size_t y, size_t z, size_t s, size_t p, size_t r, size_t a, size_t q) const {
    return ((x < dimensions_[0]) && (y < dimensions_[1]) && (z < dimensions_[2]) && (s < dimensions_[3]) && (p < dimensions_[4]) && (r < dimensions_[5]) &&
            (a < dimensions_[6]) && (q < dimensions_[7]));
}

template <typename T>
inline bool NDArray<T>::PointInRange(size_t x, size_t y, size_t z, size_t s, size_t p, size_t r, size_t a, size_t q, size_t u) const {
    return ((x < dimensions_[0]) && (y < dimensions_[1]) && (z < dimensions_[2]) && (s < dimensions_[3]) && (p < dimensions_[4]) && (r < dimensions_[5]) &&
            (a < dimensions_[6]) && (q < dimensions_[7]) && (u < dimensions_[8]));
}

}  // namespace eurora::core

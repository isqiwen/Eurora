#pragma once

#include <complex>
#include <optional>
#include <tuple>
#include <type_traits>
#include <utility>
#include <variant>

#include <NumCpp.hpp>

#include "ismrmrd/ismrmrd.h"
#include "ismrmrd/meta.h"

namespace eurora::core {

// Acquisition structure with NumCpp arrays
struct Acquisition {
    ISMRMRD::AcquisitionHeader header;
    nc::NdArray<std::complex<float>> kspace_data;
    std::optional<nc::NdArray<float>> trajectory;

    Acquisition(ISMRMRD::AcquisitionHeader hdr, nc::NdArray<std::complex<float>> kspace, std::optional<nc::NdArray<float>> traj = std::nullopt)
        : header(std::move(hdr)), kspace_data(std::move(kspace)), trajectory(std::move(traj)) {}
};

// Waveform structure with NumCpp arrays
struct Waveform {
    ISMRMRD::WaveformHeader header;
    nc::NdArray<uint32_t> data;

    Waveform(ISMRMRD::WaveformHeader hdr, nc::NdArray<uint32_t> wave_data) : header(std::move(hdr)), data(std::move(wave_data)) {}
};

// Generic Image structure with NumCpp arrays
template <typename T>
struct Image {
    ISMRMRD::ImageHeader header;
    nc::NdArray<T> image_data;
    std::optional<ISMRMRD::MetaContainer> metadata;

    Image(ISMRMRD::ImageHeader hdr, nc::NdArray<T> img_data, std::optional<ISMRMRD::MetaContainer> meta = std::nullopt)
        : header(std::move(hdr)), image_data(std::move(img_data)), metadata(std::move(meta)) {}
};

// Type alias for handling various image types
using AnyImage = std::variant<Image<int16_t>, Image<uint16_t>, Image<float>, Image<double>, Image<std::complex<float>>, Image<std::complex<double>>>;

// Utility function for creating images
template <typename T>
Image<T> MakeImage(const ISMRMRD::ImageHeader& header, nc::NdArray<T> data, std::optional<ISMRMRD::MetaContainer> metadata = std::nullopt) {
    return Image<T>{header, std::move(data), std::move(metadata)};
}

// Apply a callable to each element in a tuple
template <typename Tuple, typename Func>
void ForEachInTuple(Tuple&& tuple, Func&& func) {
    std::apply([&func](auto&&... args) { (..., func(std::forward<decltype(args)>(args))); }, std::forward<Tuple>(tuple));
}

// Generic Apply function to invoke a callable with tuple elements
template <typename Callable, typename Tuple>
constexpr decltype(auto) Apply(Callable&& callable, Tuple&& tuple) {
    constexpr auto tuple_size = std::tuple_size_v<std::remove_reference_t<Tuple>>;
    return std::apply(std::forward<Callable>(callable), std::forward<Tuple>(tuple));
}

}  // namespace eurora::core

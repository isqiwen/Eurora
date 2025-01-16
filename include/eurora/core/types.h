#pragma once

#include <optional>
#include <tuple>
#include <type_traits>
#include <utility>
#include <variant>

namespace detail {

template <typename Callable, typename Tuple, std::size_t... Indices>
constexpr decltype(auto) ApplyImpl(Callable&& callable, Tuple&& tuple, std::index_sequence<Indices...>) {
    return std::forward<Callable>(callable)(std::get<Indices>(std::forward<Tuple>(tuple))...);
}
}  // namespace detail

namespace eurora::core {

struct Acquisition {
    ISMRMRD::AcquisitionHeader header_;
    hoNDArray<std::complex<float>> kspace_data_;
    std::optional<hoNDArray<float>> trajectory_;

    Acquisition(ISMRMRD::AcquisitionHeader hdr, hoNDArray<std::complex<float>> kspace, optional<hoNDArray<float>> traj = std::nullopt)
        : header_(std::move(hdr)), kspace_data_(std::move(kspace)), trajectory_(std::move(traj)) {}
};

struct Waveform {
    ISMRMRD::WaveformHeader header_;
    hoNDArray<uint32_t> data_;

    Waveform(ISMRMRD::WaveformHeader hdr, hoNDArray<uint32_t> wave_data) : header_(std::move(hdr)), data_(std::move(wave_data)) {}
};

template <class T>
struct Image {
    ISMRMRD::ImageHeader header_;
    hoNDArray<T> image_data_;
    std::optional<ISMRMRD::MetaContainer> metadata_;

    Image(ISMRMRD::ImageHeader hdr, hoNDArray<T> img_data, optional<ISMRMRD::MetaContainer> meta = std::nullopt)
        : header_(std::move(hdr)), image_data_(std::move(img_data)), metadata_(std::move(meta)) {}
};

using AnyImage = std::variant<Image<short>, Image<unsigned short>, Image<float>, Image<double>, Image<std::complex<float>>, Image<std::complex<double>>>;

template <typename Tuple, typename Func>
void ForEachInTuple(Tuple&& tuple, Func&& func) {
    std::apply([&func](auto&&... args) { (..., func(std::forward<decltype(args)>(args))); }, std::forward<Tuple>(tuple));
}

template <typename T>
Image<T> MakeImage(const ISMRMRD::ImageHeader& header, hoNDArray<T> data, optional<ISMRMRD::MetaContainer> metadata = std::nullopt) {
    return Image<T>{header, std::move(data), std::move(metadata)};
}

template <typename Callable, typename Tuple>
constexpr decltype(auto) Apply(Callable&& callable, Tuple&& tuple) {
    static_assert(std::tuple_size<std::remove_reference_t<Tuple>>::value > 0, "Tuple must be compatible with std::tuple_size and std::get.");

    constexpr auto tuple_size = std::tuple_size<std::remove_reference_t<Tuple>>::value;
    return detail::ApplyImpl(std::forward<Callable>(callable), std::forward<Tuple>(tuple), std::make_index_sequence<tuple_size>{});
}

}  // namespace eurora::core

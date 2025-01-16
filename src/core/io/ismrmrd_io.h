#pragma once

#include <ismrmrd/ismrmrd.h>
#include <ismrmrd/meta.h>
#include <ismrmrd/waveform.h>
#include <ismrmrd/xml.h>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include "Types.h"
#include "complext.h"

namespace Gadgetron::Core::IO {

// Type mapping from C++ types to ISMRMRD data types
template <class T>
constexpr uint16_t ismrmrd_data_type() {
    static_assert(!std::is_same_v<T, T>, "Unsupported type for ISMRMRD data type mapping");
}

template <>
constexpr uint16_t ismrmrd_data_type<unsigned short>() {
    return ISMRMRD::ISMRMRD_USHORT;
}

template <>
constexpr uint16_t ismrmrd_data_type<short>() {
    return ISMRMRD::ISMRMRD_SHORT;
}

template <>
constexpr uint16_t ismrmrd_data_type<unsigned int>() {
    return ISMRMRD::ISMRMRD_UINT;
}

template <>
constexpr uint16_t ismrmrd_data_type<int>() {
    return ISMRMRD::ISMRMRD_INT;
}

template <>
constexpr uint16_t ismrmrd_data_type<float>() {
    return ISMRMRD::ISMRMRD_FLOAT;
}

template <>
constexpr uint16_t ismrmrd_data_type<double>() {
    return ISMRMRD::ISMRMRD_DOUBLE;
}

template <>
constexpr uint16_t ismrmrd_data_type<std::complex<float>>() {
    return ISMRMRD::ISMRMRD_CXFLOAT;
}

template <>
constexpr uint16_t ismrmrd_data_type<std::complex<double>>() {
    return ISMRMRD::ISMRMRD_CXDOUBLE;
}

template <>
constexpr uint16_t ismrmrd_data_type<complext<float>>() {
    return ISMRMRD::ISMRMRD_CXFLOAT;
}

template <>
constexpr uint16_t ismrmrd_data_type<complext<double>>() {
    return ISMRMRD::ISMRMRD_CXDOUBLE;
}

// Forward declarations for read and write functions
void read(std::istream& stream, ISMRMRD::IsmrmrdHeader& header);
void write(std::ostream& stream, const ISMRMRD::IsmrmrdHeader& header);

void write(std::ostream& stream, const ISMRMRD::MetaContainer& meta);
void read(std::istream& stream, ISMRMRD::MetaContainer& meta);

void write(std::ostream& stream, const ISMRMRD::Waveform& wave);
void read(std::istream& stream, ISMRMRD::Waveform& wave);

template <class T>
void read(std::istream& stream, Image<T>& img);

template <class T>
void write(std::ostream& stream, const Image<T>& img);

}  // namespace Gadgetron::Core::IO

#include "primitives.h"

namespace Gadgetron::Core::IO {

template <class T>
void read(std::istream& stream, Image<T>& img) {
    auto& [header, data, meta] = img;
    header                     = IO::read<ISMRMRD::ImageHeader>(stream);

    if (header.attribute_string_len > 0) {
        meta = ISMRMRD::MetaContainer();
        read(stream, *meta);
    } else {
        auto _ = read_string_from_stream(stream);  // Consume empty attribute string
    }

    data = hoNDArray<T>(header.matrix_size[0], header.matrix_size[1], header.matrix_size[2], header.channels);
    IO::read(stream, data.data(), data.size());
}

template <class T>
void write(std::ostream& stream, const Image<T>& img) {
    const auto& [header, data, meta] = img;
    std::string serialized_meta;
    uint64_t meta_size = 0;

    if (meta) {
        std::stringstream meta_stream;
        ISMRMRD::serialize(*meta, meta_stream);
        serialized_meta = meta_stream.str();
        meta_size       = serialized_meta.size() + 1;
    }

    ISMRMRD::ImageHeader corrected_header = header;
    corrected_header.data_type            = ismrmrd_data_type<T>();
    corrected_header.attribute_string_len = meta_size;

    if ((header.matrix_size[0] * header.matrix_size[1] * header.matrix_size[2] * header.channels) != data.size()) {
        throw std::runtime_error("Image data size does not match header dimensions");
    }

    IO::write(stream, corrected_header);
    IO::write(stream, meta_size);
    stream.write(serialized_meta.c_str(), meta_size);
    IO::write(stream, data.data(), data.size());
}

void write(std::ostream& stream, const ISMRMRD::MetaContainer& meta) {
    std::stringstream meta_stream;
    ISMRMRD::serialize(meta, meta_stream);
    write_string_to_stream(stream, meta_stream.str());
}

void read(std::istream& stream, ISMRMRD::MetaContainer& meta) {
    auto meta_string = read_string_from_stream(stream);
    ISMRMRD::deserialize(meta_string.c_str(), meta);
}

void write(std::ostream& stream, const ISMRMRD::Waveform& wave) {
    IO::write(stream, wave.head);
    IO::write(stream, wave.begin_data(), wave.size());
}

void read(std::istream& stream, ISMRMRD::Waveform& wave) {
    auto header = IO::read<ISMRMRD::WaveformHeader>(stream);
    wave        = ISMRMRD::Waveform(header.number_of_samples, header.channels);
    IO::read(stream, wave.begin_data(), wave.size());
    wave.head = header;
}

void read(std::istream& stream, ISMRMRD::IsmrmrdHeader& header) {
    auto xml = read_string_from_stream(stream);
    ISMRMRD::deserialize(xml.c_str(), header);
}

void write(std::ostream& stream, const ISMRMRD::IsmrmrdHeader& header) {
    std::stringstream output_stream;
    ISMRMRD::serialize(header, output_stream);
    write_string_to_stream(stream, output_stream.str());
}

}  // namespace Gadgetron::Core::IO

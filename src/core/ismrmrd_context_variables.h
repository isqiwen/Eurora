#pragma once

#include <string>

#include <ismrmrd/xml.h>

namespace eurora::core {

/**
 * Used to extract subject, device, session, and measurement IDs from an ISMRMRD
 * XML header, falling back to extracting values from the measurement ID element if
 * it is formatted as DEVICE_SUBJECT_SESSION_MEASUREMENT.
 */
class IsmrmrdContextVariables {
public:
    IsmrmrdContextVariables(const ISMRMRD::IsmrmrdHeader& head);

    IsmrmrdContextVariables(const std::string& measurement_id);

    IsmrmrdContextVariables(const std::string& subject_id, const std::string& device_id, const std::string& session_id, const std::string& measurement_id)
        : subject_id_(subject_id), device_id_(device_id), session_id_(session_id), measurement_id_(measurement_id) {};

    std::string const& SubjectId() const { return subject_id_; }

    std::string const& DeviceId() const { return device_id_; }

    std::string const& SessionId() const { return session_id_; }

    std::string const& MeasurementId() const { return measurement_id_; }

private:
    void UpdateFromMeasurementId();

    std::string subject_id_     = "";
    std::string device_id_      = "";
    std::string session_id_     = "";
    std::string measurement_id_ = "";
};

}  // namespace eurora::core

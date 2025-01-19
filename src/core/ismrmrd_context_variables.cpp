#include "ismrmrd_context_variables.h"

#include <optional>
#include <regex>

#include "eurora/utils/logger.h"

namespace eurora::core {

IsmrmrdContextVariables::IsmrmrdContextVariables(const ISMRMRD::IsmrmrdHeader& head) {
    if (head.acquisitionSystemInformation.is_present()) {
        auto acq_system = head.acquisitionSystemInformation.get();
        if (acq_system.deviceID.has_value()) {
            device_id_ = acq_system.deviceID.get();
        }
    }

    if (head.measurementInformation.is_present()) {
        auto meas_info = head.measurementInformation.get();
        if (meas_info.measurementID.is_present()) {
            measurement_id_ = meas_info.measurementID.get();
        }
    }

    if (head.subjectInformation.is_present()) {
        auto subject_info = head.subjectInformation.get();
        if (subject_info.patientID.is_present()) {
            subject_id_ = subject_info.patientID.get();
        }
    }

    if (head.studyInformation.is_present()) {
        auto study_info = head.studyInformation.get();
        if (study_info.studyID.is_present()) {
            session_id_ = study_info.studyID.get();
        }
    }

    if (measurement_id_.empty() || (!session_id_.empty() && !device_id_.empty() && !subject_id_.empty())) {
        return;
    }

    // We were unable to find all that we need, we can attempt to extract from measurement id.
    UpdateFromMeasurementId();
}

IsmrmrdContextVariables::IsmrmrdContextVariables(const std::string& measurement_id) : measurement_id_(measurement_id) { UpdateFromMeasurementId(); }

void IsmrmrdContextVariables::UpdateFromMeasurementId() {
    static std::regex reg("^([^_]*)_([^_]*)_([^_]*)_([^_]*)$");
    std::smatch match;

    if (!std::regex_match(measurement_id_, match, reg)) {
        STREAM_WARN() << "Attempted to extract context variables from measurement id, but failed.";
        return;
    }

    if (device_id_.empty()) {
        device_id_ = match[1];
    }

    if (subject_id_.empty()) {
        subject_id_ = match[2];
    }

    if (session_id_.empty()) {
        session_id_ = match[3];
    }
}

}  // namespace eurora::core

#pragma once

#include <chrono>
#include <filesystem>
#include <future>
#include <istream>
#include <map>
#include <memory>
#include <optional>
#include <ostream>
#include <variant>

#include "IsmrmrdContextVariables.h"
#include "io/primitives.h"

namespace eurora::core {

struct StorageItemTags {
    std::string subject;
    std::optional<std::string> device;
    std::optional<std::string> session;
    std::optional<std::string> name;
    std::multimap<std::string, std::string> custom_tags;

    class Builder {
    public:
        explicit Builder(std::string_view subject) : tags{.subject = std::string(subject)} {}

        Builder& with_device(std::string_view device) {
            tags.device = std::string(device);
            return *this;
        }

        Builder& with_session(std::string_view session) {
            tags.session = std::string(session);
            return *this;
        }

        Builder& with_name(std::string_view name) {
            tags.name = std::string(name);
            return *this;
        }

        Builder& with_custom_tag(std::string_view tag_name, std::string_view tag_value) {
            tags.custom_tags.emplace(std::string(tag_name), std::string(tag_value));
            return *this;
        }

        StorageItemTags build() const { return tags; }

    private:
        StorageItemTags tags;
    };
};

struct StorageItem {
    StorageItemTags tags;
    std::string location;
    std::string contentType;
    std::chrono::time_point<std::chrono::system_clock> lastModified;
    std::optional<std::chrono::time_point<std::chrono::system_clock>> expires;
    std::string data;
};

struct StorageItemList {
    std::vector<StorageItem> items;
    bool complete = false;
    std::string continuation;
};

class StorageClient {
public:
    explicit StorageClient(std::string_view base_url) : base_url(std::string(base_url)) {
        if (base_url.back() == '/') {
            this->base_url.pop_back();  // Ensure no trailing slash
        }
    }

    virtual ~StorageClient() = default;

    virtual StorageItemList list_items(const StorageItemTags& tags, size_t limit = 20);
    virtual StorageItemList get_next_page_of_items(const StorageItemList& page);
    virtual std::shared_ptr<std::istream> get_latest_item(const StorageItemTags& tags);
    virtual std::shared_ptr<std::istream> get_item_by_url(std::string_view url);
    virtual StorageItem store_item(const StorageItemTags& tags, std::istream& data, std::optional<std::chrono::seconds> ttl = {});
    virtual std::optional<std::string> health_check();

private:
    std::string base_url;
};

class StorageSpace {
public:
    StorageSpace(std::shared_ptr<StorageClient> client, IsmrmrdContextVariables context_vars, std::chrono::seconds default_duration)
        : client(std::move(client)), context_vars(std::move(context_vars)), default_duration(default_duration) {}

    virtual ~StorageSpace() = default;

    template <typename T>
    void store(const std::string& key, const T& value) {
        store(key, value, default_duration);
    }

    template <typename T, typename Rep, typename Period>
    void store(const std::string& key, const T& value, std::chrono::duration<Rep, Period> duration) {
        auto tags = get_tag_builder(true).with_name(key).build();
        std::stringstream stream;
        Core::IO::write(stream, value);
        client->store_item(tags, stream, std::chrono::duration_cast<std::chrono::seconds>(duration));
    }

protected:
    virtual StorageItemTags::Builder get_tag_builder(bool for_write) = 0;

    template <typename T>
    std::optional<T> get_latest(const StorageItemTags& tags) const {
        auto data = client->get_latest_item(tags);
        if (data) {
            return Core::IO::read<T>(*data);
        }
        return {};
    }

    std::shared_ptr<StorageClient> client;
    IsmrmrdContextVariables context_vars;
    std::chrono::seconds default_duration;
};

class StorageSpaceWithDefaultRead : public StorageSpace {
public:
    using StorageSpace::StorageSpace;

    template <typename T>
    std::optional<T> get_latest(const std::string& key) {
        try {
            auto tags = get_tag_builder(false).with_name(key).build();
            return StorageSpace::get_latest<T>(tags);
        } catch (const std::exception& e) {
            std::cerr << "Context incomplete: " << e.what() << '\n';
            return {};
        }
    }
};

class SessionSpace : public StorageSpaceWithDefaultRead {
protected:
    StorageItemTags::Builder get_tag_builder(bool for_write) override {
        if (context_vars.subject_id().empty()) {
            throw std::runtime_error("Missing subject ID in ISMRMRD header.");
        }
        auto builder = StorageItemTags::Builder(context_vars.subject_id());
        if (!context_vars.device_id().empty()) {
            builder.with_device(context_vars.device_id());
        }
        return builder;
    }
};

class ScannerSpace : public StorageSpaceWithDefaultRead {
protected:
    StorageItemTags::Builder get_tag_builder(bool for_write) override {
        if (context_vars.device_id().empty()) {
            throw std::runtime_error("Missing device ID in ISMRMRD header.");
        }
        return StorageItemTags::Builder("$null").with_device(context_vars.device_id());
    }
};

class MeasurementSpace : public StorageSpace {
public:
    template <typename T>
    std::optional<T> get_latest(const std::string& measurement_id, const std::string& key) {
        try {
            auto tags = get_tag_builder(false).with_name(key).with_custom_tag("measurement", measurement_id).build();
            return StorageSpace::get_latest<T>(tags);
        } catch (const std::exception& e) {
            std::cerr << "Context incomplete: " << e.what() << '\n';
            return {};
        }
    }

protected:
    StorageItemTags::Builder get_tag_builder(bool for_write) override {
        if (context_vars.subject_id().empty()) {
            throw std::runtime_error("Missing subject ID in ISMRMRD header.");
        }
        auto builder = StorageItemTags::Builder(context_vars.subject_id());
        if (for_write && context_vars.measurement_id().empty()) {
            throw std::runtime_error("Missing measurement ID in ISMRMRD header.");
        }
        if (for_write) {
            builder.with_custom_tag("measurement", context_vars.measurement_id());
        }
        return builder;
    }
};

struct StorageSpaces {
    std::shared_ptr<SessionSpace> session;
    std::shared_ptr<ScannerSpace> scanner;
    std::shared_ptr<MeasurementSpace> measurement;
};

}  // namespace eurora::core

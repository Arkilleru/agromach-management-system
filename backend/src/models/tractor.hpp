#pragma once
#include <string>
#include <optional>
#include <userver/formats/json/value.hpp>
#include <userver/formats/json/value_builder.hpp>

namespace agromach::models {

struct Tractor {
    std::string id;
    double lat;
    double lon;
    int fuel_level;
    std::string status;
};


inline Tractor Parse(const userver::formats::json::Value& json, 
              userver::formats::parse::To<Tractor>) {
    return Tractor{
        json["id"].As<std::string>(),
        json["lat"].As<double>(),
        json["lon"].As<double>(),
        json["fuel_level"].As<int>(),
        json["status"].As<std::string>()
    };
}

inline userver::formats::json::Value Serialize(const Tractor& tractor, 
                                        userver::formats::serialize::To<userver::formats::json::Value>) {
    userver::formats::json::ValueBuilder builder;
    builder["id"] = tractor.id;
    builder["lat"] = tractor.lat;
    builder["lon"] = tractor.lon;
    builder["fuel_level"] = tractor.fuel_level;
    builder["status"] = tractor.status;
    return builder.ExtractValue();
}

} // namespace agromach::models
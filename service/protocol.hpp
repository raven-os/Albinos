//
// Created by milerius on 20/10/18.
//

#pragma once

#include <string>
#include "json.hpp"

namespace raven
{
    namespace json = nlohmann;

    //! Keywords
    static inline constexpr const char *request_keyword = "REQUEST_NAME";

    //! Protocol Constants
    static inline constexpr const char *config_name_keyword = "CONFIG_NAME";

    struct config_create
    {
        const std::string order{"CONFIG_CREATE"};
        std::string config_name;
    };

    inline void from_json(const raven::json::json &json_data, config_create &cfg)
    {
        cfg.config_name = json_data.at(config_name_keyword).get<std::string>();
    }
}

//
// Created by milerius on 19/10/18.
//

#pragma once

#include <iomanip>
#include <string_view>
#include <iostream>
#include <filesystem>
#include <memory>
#include <uvw.hpp>
#include <sqlite_modern_cpp.h>
#include "protocol.hpp"

namespace raven
{
  class service
  {
  private:
    //! Helpers
    template <typename Request>
    Request fill_request(json::json &json_data)
    {
        Request request;
        from_json(json_data, request);
        std::cout << "json receive:\n" << std::setw(4) << json_data << std::endl;
        return request;
    }

  public:
    //! Callbacks
    void create_config(json::json &json_data)
    {
        auto cfg = fill_request<config_create>(json_data);
        std::cout << "cfg.config_name: " << cfg.config_name << std::endl;
        // TODO: Insert in SQL
    }

    void load_config(json::json &json_data)
    {
        auto cfg = fill_request<config_load>(json_data);
        if (cfg.config_key) {
            std::cout << "cfg.config_key: " << cfg.config_key.value() << std::endl;
        }
        if (cfg.config_read_only_key) {
            std::cout << "cfg.config_read_only_key: " << cfg.config_read_only_key.value() << std::endl;
        }
    }

    void unload_config(json::json &json_data)
    {
        auto cfg = fill_request<config_unload>(json_data);
        std::cout << "cfg.id: " << cfg.id << std::endl;
    }

    void include_config(json::json &json_data)
    {
        auto cfg = fill_request<config_include>(json_data);
        std::cout << "cfg.id: " << cfg.id << std::endl;
        std::cout << "cfg.src: " << cfg.src << std::endl;
        std::cout << "cfg.dst: " << cfg.dst << std::endl;
    }

    void update_setting(json::json &json_data)
    {
        auto cfg = fill_request<setting_update>(json_data);
        std::cout << "cfg.id: " << cfg.id << std::endl;
        std::cout << "cfg.setting_name: " << cfg.setting_name << std::endl;
        std::cout << "cfg.setting_value: " << cfg.setting_value << std::endl;
    }

    void remove_setting(json::json &json_data)
    {
        auto cfg = fill_request<setting_remove>(json_data);
        std::cout << "cfg.id: " << cfg.id << std::endl;
        std::cout << "cfg.setting_name: " << cfg.setting_name << std::endl;
    }

    void get_setting(json::json &json_data)
    {
        auto cfg = fill_request<setting_get>(json_data);
        std::cout << "cfg.id: " << cfg.id << std::endl;
        std::cout << "cfg.setting_name: " << cfg.setting_name << std::endl;
    }

    void set_alias(json::json &json_data)
    {
        auto cfg = fill_request<alias_set>(json_data);
        std::cout << "cfg.id: " << cfg.id << std::endl;
        std::cout << "cfg.setting_name: " << cfg.setting_name << std::endl;
        std::cout << "cfg.alias_name: " << cfg.alias_name << std::endl;
    }

    void unset_alias(json::json &json_data)
    {
        auto cfg = fill_request<alias_unset>(json_data);
        std::cout << "cfg.id: " << cfg.id << std::endl;
        std::cout << "cfg.alias_name: " << cfg.alias_name << std::endl;
    }

    void subscribe_setting(json::json &json_data)
    {
        auto cfg = fill_request<setting_subscribe>(json_data);
        std::cout << "cfg.id: " << cfg.id << std::endl;
        if (cfg.setting_name) {
            std::cout << "cfg.setting_name: " << cfg.setting_name.value() << std::endl;
        }
        if (cfg.alias_name) {
            std::cout << "cfg.alias_name: " << cfg.alias_name.value() << std::endl;
        }
    }

    void unsubscribe_setting(json::json &json_data)
    {
        auto cfg = fill_request<setting_subscribe>(json_data);
        std::cout << "cfg.id: " << cfg.id << std::endl;
        if (cfg.setting_name) {
            std::cout << "cfg.setting_name: " << cfg.setting_name.value() << std::endl;
        }
        if (cfg.alias_name) {
            std::cout << "cfg.alias_name: " << cfg.alias_name.value() << std::endl;
        }
    }

    //! Constructor
    service()
    {
        server_->on<uvw::ErrorEvent>([](auto const &, auto &) { /* TODO: Fill it */ });
        server_->on<uvw::ListenEvent>([this](uvw::ListenEvent const &, uvw::PipeHandle &handle) {
            std::shared_ptr<uvw::PipeHandle> socket = handle.loop().resource<uvw::PipeHandle>();
            socket->on<uvw::CloseEvent>(
                [](uvw::CloseEvent const &, uvw::PipeHandle &) { std::cout << "socket closed." << std::endl; });

            socket->on<uvw::EndEvent>([](const uvw::EndEvent &, uvw::PipeHandle &sock) {
                std::cout << "end event received" << std::endl;
                sock.close();
            });

            socket->on<uvw::DataEvent>([this](const uvw::DataEvent &data, uvw::PipeHandle &sock) {
                static const std::unordered_map<std::string, std::function<void(json::json &)>>
                    order_registry
                    {
                        {
                            "CONFIG_CREATE",       [this](json::json &json_data) {
                            this->create_config(json_data);
                        }},
                        {
                            "CONFIG_LOAD",         [this](json::json &json_data) {
                            this->load_config(json_data);
                        }},
                        {
                            "CONFIG_UNLOAD",       [this](json::json &json_data) {
                            this->unload_config(json_data);
                        }},
                        {
                            "CONFIG_INCLUDE",      [this](json::json &json_data) {
                            this->include_config(json_data);
                        }},
                        {
                            "SETTING_UPDATE",      [this](json::json &json_data) {
                            this->update_setting(json_data);
                        }},
                        {
                            "SETTING_REMOVE",      [this](json::json &json_data) {
                            this->remove_setting(json_data);
                        }},
                        {
                            "SETTING_GET",         [this](json::json &json_data) {
                            this->get_setting(json_data);
                        }},
                        {
                            "ALIAS_SET",           [this](json::json &json_data) {
                            this->set_alias(json_data);
                        },
                        },
                        {
                            "ALIAS_UNSET",         [this](json::json &json_data) {
                            this->unset_alias(json_data);
                        },
                        },
                        {
                            "SUBSCRIBE_SETTING",   [this](json::json &json_data) {
                            this->subscribe_setting(json_data);
                        },
                        },
                        {
                            "UNSUBSCRIBE_SETTING", [this](json::json &json_data) {
                            this->unsubscribe_setting(json_data);
                        },
                        }
                    };

                std::string_view data_str(data.data.get(), data.length);
                try {
                    auto json_data = json::json::parse(data_str);
                    std::string command_order = json_data.at(raven::request_keyword).get<std::string>();
                    order_registry.at(command_order)(json_data);
                }
                catch (const json::json::exception &error) {
                    std::cerr << "error in received data: " << error.what() << std::endl;
                }
            });

            handle.accept(*socket);
            std::cout << "socket connected" << std::endl;
            socket->read();
        });
    }

    void run() noexcept
    {
        clean_socket();
        std::string socket = (std::filesystem::temp_directory_path() / "raven-os_service_libconfig.sock").string();
        std::cout << "binding to socket: " << socket << std::endl;
        server_->bind(socket);
        server_->listen();
        uv_loop_->run();
    }

    //! In case that the service have been stopped, we want to remove the old socket and create a new one.
    void clean_socket() noexcept
    {
        auto socket_path = std::filesystem::temp_directory_path() / "raven-os_service_libconfig.sock";
        if (std::filesystem::exists(socket_path)) {
            std::cout << "socket: " << socket_path.string() << " already exist, removing" << std::endl;
            std::filesystem::remove(socket_path);
        }
    }

  private:
    std::shared_ptr<uvw::Loop> uv_loop_{uvw::Loop::getDefault()};
    std::shared_ptr<uvw::PipeHandle> server_{uv_loop_->resource<uvw::PipeHandle>()};
    sqlite::database database_{sqlite::database("libconfig_db.sqlite")};
  };
}

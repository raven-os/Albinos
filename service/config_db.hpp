//
// Created by riz on 29/01/19.
//

#pragma once

#include <cstring>
#include <vector>
#include <sqlite_modern_cpp.h>

namespace raven::db
{
  template<typename T>
  class blob
  {
  public:
    //! getters
    template<typename vtype>
    std::enable_if_t<std::is_fundamental<vtype>::value, vtype> get_value() const noexcept
    {
        vtype ret;
        std::memcpy(&ret, data_.data(), sizeof(vtype));
        return ret;
    }

    std::string get_value() const noexcept
    {
        return (std::string(data_.begin(), data_.end()));
    }

    const std::vector<T> &get_data() const noexcept
    {
        return data_;
    }

    std::vector<T> &get_data() noexcept
    {
        return data_;
    }

    //! Constructors
    template<typename vtype, typename std::enable_if_t<std::is_fundamental<T>::value, int> = 0>
    explicit blob(vtype value) noexcept
    {
        data_.resize(sizeof(vtype) / sizeof(T));
        std::memcpy(data_.data(), &value, sizeof(vtype));
    }

    explicit blob(std::vector<T> value) noexcept
    :data_{std::move(value)}
    {
    }

    explicit blob(const std::string &value) noexcept
    :data_{value.begin(), value.end()}
    {
    }

    explicit blob(const char *value) noexcept
    :blob{std::string(value)}
    {
    }

  private:
    std::vector<T> data_;
  };

  template<typename T>
  inline sqlite::database_binder &operator<<(sqlite::database_binder &db, const blob<T> &value)
  {
    return db << value.get_data();
  }

  class config_db
  {
  public:
    //! Constructor
    config_db(const std::string &db_path)
    :database_{sqlite::database(db_path)}
    {
        database_ <<
        "create table if not exist configIndex ("
        "   id integer primary key autoincrement not null,"
        "   name text"
        ");";
    }

  private:
    sqlite::database database_;
  };
}
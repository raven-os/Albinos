//
// Created by riz on 29/01/19.
//

#pragma once

#include <cstring>
#include <vector>
#include <sstream>
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

    // le compilo me laisse pas faire une template specifier pour std::string je sais pas trop pourquoi ( je suis surement un abrutit mais la il est tard je suis fatigué) à investiguer
    std::string get_value_string() const noexcept
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
    //! Operations
    long create_config(const std::string &name)
    {
        int maxId;
        database_ << "select coalesce(max(id), 0) from configIndex;" >> maxId; // TODO: change to get real increment count
        std::ostringstream new_table_name("");
        new_table_name << name << "_" << maxId + 1;
        database_ << "create table if not exists " + new_table_name.str() + " (" +
                     "  id integer primary key autoincrement not null,"
                     "  setting_name text,"
                     "  setting_value blob"
                     ");";
        database_ << "insert into configIndex (name) values (?);" << new_table_name.str();
        return database_.last_insert_rowid();

        // TODO cas d'une config a inclusion
    }

    void delete_config(long id)
    {
        bool config_exist;
        database_ << "select exists(select 1 from configIndex where id = ?);" << id >> config_exist;
        if (config_exist) {
            std::string table_name;
            database_ << "select name from configIndex where id = ?" << id >> table_name;
            database_ << "drop table if exists " + table_name;
            database_ << "delete from configIndex where id = ?" << id;
        } // else {
            // TODO : gestion d'erreur quand la table `id` n'existe pas
        //}
    }

    template<typename valueType>
    void update_setting(long config_id, const std::string &name, const valueType &value)
    {
        bool config_exist;
        database_ << "select exists(select 1 from configIndex where id = ?);" << config_id >> config_exist;
        if (config_exist) {
            std::string table_name;
            database_ << "select name from configIndex where id = ?" << config_id >> table_name;
            bool setting_exist;
            database_ << "select exists(select 1 from " + table_name + " where setting_name = ?);" << name
                      >> setting_exist;
            if (setting_exist) {
                database_ << "update " + table_name + " set setting_value = ? where setting_name = ?;"
                          << blob<char>(value) << name;
            } else {
                database_ << "insert into " + table_name + "(setting_name, setting_value) values (?,?);"
                          << name << blob<char>(value);
            }
        } //else {
            // TODO gestion d'erreur quand la config `config_id` n'existe pas
        //}
    }

    template<typename T>
    void update_setting(long config_id, const std::string &name, const std::vector<T> &value)
    {
        bool config_exist;
        database_ << "select exists(select 1 from configIndex where id = ?);" << config_id >> config_exist;
        if (config_exist) {
            std::string table_name;
            database_ << "select name from configIndex where id = ?" << config_id >> table_name;
            bool setting_exist;
            database_ << "select exists(select 1 from " + table_name + " where setting_name = ?);" << name
                      >> setting_exist;
            if (setting_exist) {
                database_ << "update " + table_name + " set setting_value = ? where setting_name = ?;"
                          << blob<T>(value) << name;
            } else {
                database_ << "insert into " + table_name + "(setting_name, setting_value) values (?,?);"
                          << name << blob<T>(value);
            }
        } //else {
        // TODO gestion d'erreur quand la config `config_id` n'existe pas
        //}
    }

    template<typename valueType>
    valueType get_setting(long config_id, const std::string &name)
    {
        valueType res;
        bool config_exist;
        database_ << "select exists(select 1 from configIndex where id = ?);" << config_id >> config_exist;
        if (config_exist) {
            std::string table_name;
            database_ << "select name from configIndex where id = ?" << config_id >> table_name;
            bool setting_exist;
            database_ << "select exists(select 1 from " + table_name + " where setting_name = ?);" << name
                      >> setting_exist;
            if (setting_exist) {
                std::vector<char> tmpres;
                database_ << "select setting_value from " + table_name + " where setting_name= ?;" << name >> tmpres;
                res = blob<char>(tmpres).get_value<valueType>();
            }
        } //else {
        // TODO gestion d'erreur quand la config `config_id` n'existe pas
        //}
        return res;
    }

    std::string get_setting_string(long config_id, const std::string &name)
    {
        std::string res;
        bool config_exist;
        database_ << "select exists(select 1 from configIndex where id = ?);" << config_id >> config_exist;
        if (config_exist) {
            std::string table_name;
            database_ << "select name from configIndex where id = ?" << config_id >> table_name;
            bool setting_exist;
            database_ << "select exists(select 1 from " + table_name + " where setting_name = ?);" << name
                      >> setting_exist;
            if (setting_exist) {
                std::vector<char> tmpres;
                database_ << "select setting_value from " + table_name + " where setting_name= ?;" << name >> tmpres;
                res = blob<char>(tmpres).get_value_string();
            }
        } //else {
        // TODO gestion d'erreur quand la config `config_id` n'existe pas
        //}
        return res;
    }

    template<typename T>
    blob<T> get_setting_blob(long config_id, const std::string &name)
    {
        bool config_exist;
        database_ << "select exists(select 1 from configIndex where id = ?);" << config_id >> config_exist;
        if (config_exist) {
            std::string table_name;
            database_ << "select name from configIndex where id = ?" << config_id >> table_name;
            bool setting_exist;
            database_ << "select exists(select 1 from " + table_name + " where setting_name = ?);" << name
                      >> setting_exist;
            if (setting_exist) {
                std::vector<T> tmpres;
                database_ << "select setting_value from " + table_name + " where setting_name= ?;" << name >> tmpres;
                return blob<T>(tmpres);
            }
        } //else {
        // TODO gestion d'erreur quand la config `config_id` n'existe pas
        //}
        return blob<T>(std::vector<T>{});
    }

    void delete_setting(long config_id, const std::string &name)
    {
        bool config_exist;
        database_ << "select exists(select 1 from configIndex where id = ?);" << config_id >> config_exist;
        if (config_exist) {
            std::string table_name;
            database_ << "select name from configIndex where id = ?" << config_id >> table_name;
            bool setting_exist;
            database_ << "select exists(select 1 from " + table_name + " where setting_name = ?);" << name
                      >> setting_exist;
            if (setting_exist) {
                database_ << "delete from " + table_name + " where setting_name = ?;" << name;
            }
        } //else {
        // TODO gestion d'erreur quand la config `config_id` n'existe pas
        //}
    }

    //! Getter
    sqlite::database &get_sqlite_db() noexcept
    {
        return database_;
    }

    //! Constructor
    config_db(const std::string &db_path)
    :database_{sqlite::database(db_path)}
    {
        database_ <<
        "create table if not exists configIndex ("
        "   id integer primary key autoincrement not null,"
        "   name text"
        ");";
    }

  private:
    sqlite::database database_;
  };
}

//
// Created by riz on 29/01/19.
//

#include <gtest/gtest.h>
#include <filesystem>
#include "config_db.hpp"

class config_db_test : public ::testing::Test
{
protected:
  void TearDown() override
  {
      std::filesystem::remove("lala.db");
  }

  raven::db::config_db db{"lala.db"};
};

TEST_F(config_db_test, create_config)
{
    int id;
    int selectId;
    std::string select_name;
    id = db.create_config("lala1");
    db.get_sqlite_db() << "select id from configIndex ORDER BY id DESC LIMIT 1;" >> selectId;
    ASSERT_EQ(id, 1);
    ASSERT_EQ(selectId, 1);
    db.get_sqlite_db() << "select name from configIndex where id = ?;" << id >> select_name;
    ASSERT_STREQ(select_name.c_str(), "lala1_1");

    id = db.create_config("lala2");
    db.get_sqlite_db() << "select id from configIndex ORDER BY id DESC LIMIT 1;" >> selectId;
    ASSERT_EQ(id, 2);
    ASSERT_EQ(selectId, 2);
    db.get_sqlite_db() << "select name from configIndex where id = ?;" << id >> select_name;
    ASSERT_STREQ(select_name.c_str(), "lala2_2");

    id = db.create_config("lala3");
    db.get_sqlite_db() << "select id from configIndex ORDER BY id DESC LIMIT 1;" >> selectId;
    ASSERT_EQ(id, 3);
    ASSERT_EQ(selectId, 3);
    db.get_sqlite_db() << "select name from configIndex where id = ?;" << id >> select_name;
    ASSERT_STREQ(select_name.c_str(), "lala3_3");

    db.get_sqlite_db() << "drop table lala2_2;";

    id = db.create_config("lala4");
    db.get_sqlite_db() << "select id from configIndex ORDER BY id DESC LIMIT 1;" >> selectId;
    ASSERT_EQ(id, 4);
    ASSERT_EQ(selectId, 4);
    db.get_sqlite_db() << "select name from configIndex where id = ?;" << id >> select_name;
    ASSERT_STREQ(select_name.c_str(), "lala4_4");

    db.get_sqlite_db() << "drop table lala4_4;";

    id = db.create_config("lala5");
    db.get_sqlite_db() << "select id from configIndex ORDER BY id DESC LIMIT 1;" >> selectId;
    ASSERT_EQ(id, 5);
    ASSERT_EQ(selectId, 5);
    db.get_sqlite_db() << "select name from configIndex where id = ?;" << id >> select_name;
    ASSERT_STREQ(select_name.c_str(), "lala5_5");

    //! test with config that share the same name

    id = db.create_config("lala");
    db.get_sqlite_db() << "select id from configIndex ORDER BY id DESC LIMIT 1;" >> selectId;
    ASSERT_EQ(id, 6);
    ASSERT_EQ(selectId, 6);
    db.get_sqlite_db() << "select name from configIndex where id = ?;" << id >> select_name;
    ASSERT_STREQ(select_name.c_str(), "lala_6");

    id = db.create_config("lala");
    db.get_sqlite_db() << "select id from configIndex ORDER BY id DESC LIMIT 1;" >> selectId;
    ASSERT_EQ(id, 7);
    ASSERT_EQ(selectId, 7);
    db.get_sqlite_db() << "select name from configIndex where id = ?;" << id >> select_name;
    ASSERT_STREQ(select_name.c_str(), "lala_7");


    //! cleanup
    db.get_sqlite_db() << "drop table if exists lala1_1;";
    db.get_sqlite_db() << "drop table if exists lala2_2;";
    db.get_sqlite_db() << "drop table if exists lala3_3;";
    db.get_sqlite_db() << "drop table if exists lala4_4;";
    db.get_sqlite_db() << "drop table if exists lala5_5;";
}

TEST_F(config_db_test, delete_config)
{
    int id1;
    int id2;

    int selectId1;
    int selectId2;

    id1 = db.create_config("lala1");
    id2 = db.create_config("lala2");
    db.get_sqlite_db() << "select id from configIndex where id = ?;"<< id1 >> selectId1;
    db.get_sqlite_db() << "select id from configIndex where id = ?;"<< id2 >> selectId2;
    ASSERT_EQ(selectId1, id1);
    ASSERT_EQ(selectId2, id2);

    db.delete_config(id1);

    selectId1 = -1;
    selectId2 = -1;
    try {
        db.get_sqlite_db() << "select id from configIndex where id = ?;" << id1 >> selectId1;
    } catch (std::exception &e) {}
    try {
        db.get_sqlite_db() << "select id from configIndex where id = ?;"<< id2 >> selectId2;
    } catch (std::exception &e) {}
    ASSERT_EQ(selectId1, -1);
    ASSERT_EQ(selectId2, id2);

    //! cleanup
    db.get_sqlite_db() << "drop table if exists lala1_1;";
    db.get_sqlite_db() << "drop table if exists lala2_2;";
}

TEST_F(config_db_test, create_setting)
{
    int id;

    std::vector<char> selectPouf;
    std::vector<char>  selectPif;
    std::vector<char> selectPof;

    id = db.create_config("lala");
    std::string table_name;
    db.get_sqlite_db() << "select name from configIndex where id = ?;" << id >> table_name;
    db.update_setting(id, "pouf", 7355608);
    db.update_setting(id, "pif", 42.42);
    db.update_setting(id, "pof", "une jolie config :)");

    db.get_sqlite_db() << "select setting_value from " + table_name + " where setting_name = ?;" << "pouf" >> selectPouf;
    db.get_sqlite_db() << "select setting_value from " + table_name + " where setting_name = ?;" << "pif" >> selectPif;
    db.get_sqlite_db() << "select setting_value from " + table_name + " where setting_name = ?;" << "pof" >> selectPof;
    ASSERT_EQ(raven::db::blob<char>(selectPouf).get_value<int>(), 7355608);
    ASSERT_EQ(raven::db::blob<char>(selectPif).get_value<double>(), 42.42);
    ASSERT_STREQ(raven::db::blob<char>(selectPof).get_value_string().c_str(), "une jolie config :)");

    //! cleanup
    db.get_sqlite_db() << "drop table if exists " + table_name + ";";
}

TEST_F(config_db_test, update_setting)
{
    int id;

    std::vector<char> selectPouf;

    id = db.create_config("lala");
    std::string table_name;
    db.get_sqlite_db() << "select name from configIndex where id = ?;" << id >> table_name;

    db.update_setting(id, "pouf", 7355608);
    db.get_sqlite_db() << "select setting_value from " + table_name + " where setting_name = ?;" << "pouf" >> selectPouf;
    ASSERT_EQ(raven::db::blob<char>(selectPouf).get_value<int>(), 7355608);

    db.update_setting(id, "pouf", 424242);
    db.get_sqlite_db() << "select setting_value from " + table_name + " where setting_name = ?;" << "pouf" >> selectPouf;
    ASSERT_EQ(raven::db::blob<char>(selectPouf).get_value<int>(), 424242);

    db.update_setting(id, "pouf", "une jolie config :)");
    db.get_sqlite_db() << "select setting_value from " + table_name + " where setting_name = ?;" << "pouf" >> selectPouf;
    ASSERT_EQ(raven::db::blob<char>(selectPouf).get_value_string(), "une jolie config :)");


    //! cleanup
    db.get_sqlite_db() << "drop table if exists " + table_name + ";";
}

TEST_F(config_db_test, get_setting)
{
    int id;

    int intPouf;
    double doublePouf;
    std::string strPouf;

    id = db.create_config("lala");
    std::string table_name;
    db.get_sqlite_db() << "select name from configIndex where id = ?;" << id >> table_name;

    db.update_setting(id, "pouf", 7355608);
    intPouf = db.get_setting<int>(id, "pouf");
    ASSERT_EQ(intPouf, 7355608);

    db.update_setting(id, "pouf", 42.42);
    doublePouf = db.get_setting<double>(id, "pouf");
    ASSERT_EQ(doublePouf, 42.42);

    db.update_setting(id, "pouf", "tema tes sandales");
    strPouf = db.get_setting_string(id, "pouf");
    ASSERT_STREQ(strPouf.c_str(), "tema tes sandales");

    //! cleanup
    db.get_sqlite_db() << "drop table if exists " + table_name + ";";
}

TEST_F(config_db_test, delete_setting)
{
    int id;

    std::vector<char> selectPouf;

    id = db.create_config("lala");
    std::string table_name;
    db.get_sqlite_db() << "select name from configIndex where id = ?;" << id >> table_name;

    bool exist;
    db.update_setting(id, "pouf", 7355608);
    db.update_setting(id, "paf", 424242);
    db.get_sqlite_db() << "select exists(select 1 from " + table_name + " where setting_name = ?);" << "pouf" >> exist;
    ASSERT_TRUE(exist);
    db.get_sqlite_db() << "select exists(select 1 from " + table_name + " where setting_name = ?);" << "paf" >> exist;
    ASSERT_TRUE(exist);

    db.delete_setting(id, "paf");
    db.get_sqlite_db() << "select exists(select 1 from " + table_name + " where setting_name = ?);" << "pouf" >> exist;
    ASSERT_TRUE(exist);
    db.get_sqlite_db() << "select exists(select 1 from " + table_name + " where setting_name = ?);" << "paf" >> exist;
    ASSERT_FALSE(exist);

    //! cleanup
    db.get_sqlite_db() << "drop table if exists " + table_name + ";";
}

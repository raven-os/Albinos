//
// Created by riz on 29/01/19.
//

#include <gtest/gtest.h>
#include "config_db.hpp"

class blob_test : public ::testing::Test
{
protected:
  void SetUp() override
  {
    db_ <<
        "create table if not exists blob ("
        "   _id integer primary key autoincrement not null,"
        "   name text,"
        "   data blob"
        ");";
  }

  void TearDown() override
  {
    db_ << "drop table blob;";
  }

  sqlite::database db_{sqlite::database("blob_test.db")};
};

TEST_F(blob_test, int_blob)
{
    raven::db::blob<char> value(7355608);
    std::vector<char> value_after;
    db_ << "insert into blob (name, data) values (?, ?)" << "dablob" << value;
    db_ << "select data from blob where name = ?" << "dablob" >>
    [&](std::vector<char> data) { value_after = data; };
    raven::db::blob<char> lala(value_after);
    ASSERT_EQ(lala.get_value<int>(), 7355608);
}

TEST_F(blob_test, float_blob)
{
    float leblobflottant = 42.42;
    raven::db::blob<char> value(leblobflottant);
    std::vector<char> value_after;
    db_ << "insert into blob (name, data) values (?, ?)" << "dablob" << value;
    db_ << "select data from blob where name = ?" << "dablob" >>
        [&](std::vector<char> data) { value_after = data; };
    raven::db::blob<char> lala(value_after);
    ASSERT_EQ(sizeof(lala.get_value<float>()), sizeof(float));
    ASSERT_EQ(lala.get_value<float>(), leblobflottant);
}

TEST_F(blob_test, double_blob)
{
    raven::db::blob<char> value(42.42);
    std::vector<char> value_after;
    db_ << "insert into blob (name, data) values (?, ?)" << "dablob" << value;
    db_ << "select data from blob where name = ?" << "dablob" >>
        [&](std::vector<char> data) { value_after = data; };
    raven::db::blob<char> lala(value_after);
    ASSERT_EQ(sizeof(lala.get_value<double>()), sizeof(double));
    ASSERT_EQ(lala.get_value<double>(), 42.42);
}

TEST_F(blob_test, str_blob)
{
    raven::db::blob<char> value("il est jolie mon blob");
    std::vector<char> value_after;
    db_ << "insert into blob (name, data) values (?, ?)" << "dablob" << value;
    db_ << "select data from blob where name = ?" << "dablob" >>
        [&](std::vector<char> data) { value_after = data; };
    raven::db::blob<char> lala(value_after);
    ASSERT_STREQ(lala.get_value().c_str(), "il est jolie mon blob");
}
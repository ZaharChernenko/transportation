#include "User.h"

User::User(const drogon::orm::Row& row)
    : first_name {row["first_name"].as<std::string>()}, last_name {row["last_name"].as<std::string>()},
      email {row["email"].as<std::string>()}, password {row["password"].as<std::string>()} {}

User::User(const std::string& first_name, const std::string& last_name, const std::string& email,
           const std::string& password)
    : first_name {first_name}, last_name {last_name}, email {email}, password {password} {}

#ifdef DEBUG_USER
void User::print() const { LOG_INFO << first_name << ' ' << last_name << ' ' << email << ' ' << password << '\n'; }
#endif

std::string User::getInsertQuery() const {
    return "INSERT INTO user (first_name, last_name, email, password) VALUES ('" + first_name + SQL_SEP + last_name +
           SQL_SEP + email + SQL_SEP + password + "');";
}

std::string User::getUserId() const { return "SELECT id FROM user WHERE email = '" + email + "';"; }
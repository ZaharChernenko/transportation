/*
CREATE TABLE user (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    first_name TEXT NOT NULL,
    last_name TEXT NOT NULL,
    email TEXT UNIQUE NOT NULL,
    password TEXT NOT NULL
);
CREATE INDEX idx_user_email ON user (email);
*/
#pragma once

#include <drogon/drogon.h>
#include <drogon/orm/DbClient.h>
#include <exception>
#include <string>

#define DEBUG_USER
#define SQL_SEP "', '"

class User;
class UserException;

// default user class with initialization from Row
class User {
  public:
    User(const drogon::orm::Row& row);
    User(const std::string& first_name, const std::string& last_name, const std::string& email,
         const std::string& password);
    std::string getInsertQuery() const;
    std::string getUserId() const;

#ifdef DEBUG_USER
    void print() const;
#endif

  protected:
    const std::string first_name, last_name, email, password;
};

class UserException : public std::exception {
  private:
    std::string message;

  public:
    UserException(const std::string& msg = "failed to create user") : message(msg) {}
    std::string what() { return message; }
};

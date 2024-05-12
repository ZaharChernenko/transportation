// all registration and login functions with database implemented here
#pragma once

#include "../algorithms/hash.h"
#include "../models/User.h"
#include <cstdint>
#include <drogon/drogon.h>
#include <drogon/orm/DbClient.h>
#include <future>
#include <regex>

#define ASYNC_AUTH
// #define DEBUG_AUTH

using Callback = std::function<void(const drogon::HttpResponsePtr&)>;

// checks if table user contains current email return true if yes
bool checkEmail(drogon::orm::DbClientPtr client, const std::string& email);

// inserts user into user table
void insertUser(drogon::orm::DbClientPtr client, const User& user);

u_int32_t getUserId(drogon::orm::DbClientPtr client, const User& user);

/*
example:
curl -X POST http://localhost:8848/register \
  -H 'Content-Type: application/x-www-form-urlencoded' \
  -d 'firstName=Петр&lastName=Петров&email=ivan@examp.com&password=12121212&confirmPassword=12121212'
*/
void registerUser(const drogon::HttpRequestPtr& request, Callback&& callback);
void loginUser(const drogon::HttpRequestPtr& request, Callback&& callback);
void rootHandler(const drogon::HttpRequestPtr& request, Callback&& callback);
void indexHandler(const drogon::HttpRequestPtr& request, Callback&& callback);
void loginHandler(const drogon::HttpRequestPtr& request, Callback&& callback);
void registerHandler(const drogon::HttpRequestPtr& request, Callback&& callback);

#pragma once

#include "../algorithms/cyrillic.h"
#include "../models/Order.h"
#include "business.h"
#include <drogon/drogon.h>
#include <drogon/orm/DbClient.h>
#include <functional>
#include <future>
#include <string>
#include <unordered_set>

// #define BUSINESS_HANDLER_DEBUG

using Callback = std::function<void(const drogon::HttpResponsePtr&)>;

void countHandler(const drogon::HttpRequestPtr& request, Callback&& callback, const std::string& departure,
                  const std::string& arrival, const std::string& car, const std::string& insurance);
void deleteHandler(const drogon::HttpRequestPtr& request, Callback&& callback, const std::string& id);
void addHandler(const drogon::HttpRequestPtr& request, Callback&& callback, const std::string& id);
void getOrdersHandler(const drogon::HttpRequestPtr& request, Callback&& callback);
void logoutHandler(const drogon::HttpRequestPtr& request, Callback&& callback);
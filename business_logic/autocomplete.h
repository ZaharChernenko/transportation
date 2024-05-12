#pragma once

#include "../algorithms/cyrillic.h"
#include <drogon/drogon.h>
#include <drogon/orm/DbClient.h>
#include <functional>
#include <string>

using Callback = std::function<void(const drogon::HttpResponsePtr&)>;

void autocompleteIndexInputs(const drogon::HttpRequestPtr& req, Callback&& callback, const std::string& city_name);
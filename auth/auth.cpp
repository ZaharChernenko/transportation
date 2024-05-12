#include "auth.h"

bool checkEmail(drogon::orm::DbClientPtr client, const std::string& email) {
#ifdef ASYNC_AUTH
    std::future<drogon::orm::Result> coroutine {
        client->execSqlAsyncFuture("SELECT EXISTS(SELECT 1 FROM user WHERE email = $1)", email)};
    bool check {coroutine.get().front()[0].as<bool>()};
    // method front() of Result class returns Row. Row supports indexation with size_t and std::string.
#else
    bool check {client->execSqlSync("SELECT EXISTS(SELECT 1 FROM user WHERE email = $1)", email).front()[0].as<bool>()};
#endif
#ifdef DEBUG_AUTH
    LOG_INFO << "checkEmail: " << email << (check ? " is used" : " is not used, create new account") << '\n';
#endif
    return check;
}

void insertUser(drogon::orm::DbClientPtr client, const User& user) {
    client->execSqlAsync(
        user.getInsertQuery(), [](const drogon::orm::Result& result) {},
        [](const drogon::orm::DrogonDbException& e) { throw e; });
}

u_int32_t getUserId(drogon::orm::DbClientPtr client, const User& user) {
    drogon::orm::Result res {client->execSqlSync(user.getUserId())};
    return res.front()[0].as<int>();
}

const std::regex EMAIL_PATTERN {R"(([\w-]+@)((?:\w+\.)+)([a-z]{2,5}))"};
void registerUser(const drogon::HttpRequestPtr& request, Callback&& callback) {
    drogon::orm::DbClientPtr client = drogon::app().getDbClient("data");
    Json::Value json;
    std::string first_name {request->getParameter("firstName")};
    if (first_name.empty()) {
        json["message"] = "Поле имя не должно быть пустым";
        drogon::HttpResponsePtr response {drogon::HttpResponse::newHttpJsonResponse(json)};
        response->setStatusCode(drogon::HttpStatusCode::k400BadRequest);
        callback(response);
        return;
    }

    std::string last_name {request->getParameter("lastName")};
    if (last_name.empty()) {
        json["message"] = "Поле фамилия не должно быть пустым";
        drogon::HttpResponsePtr response {drogon::HttpResponse::newHttpJsonResponse(json)};
        response->setStatusCode(drogon::HttpStatusCode::k400BadRequest);
        callback(response);
        return;
    }

    std::string email = request->getParameter("email");
    if (email.empty()) {
        json["message"] = "Поле email не должно быть пустым";
        drogon::HttpResponsePtr response {drogon::HttpResponse::newHttpJsonResponse(json)};
        response->setStatusCode(drogon::HttpStatusCode::k400BadRequest);
        callback(response);
        return;
    }

    std::string password {request->getParameter("password")};
    if (password.empty()) {
        json["message"] = "Поле пароль не должно быть пустым";
        drogon::HttpResponsePtr response {drogon::HttpResponse::newHttpJsonResponse(json)};
        response->setStatusCode(drogon::HttpStatusCode::k400BadRequest);
        callback(response);
        return;
    }

    if (password.size() < 7) {
        json["message"] = "Длина пароля должна быть не меньше 7 символов";
        drogon::HttpResponsePtr response {drogon::HttpResponse::newHttpJsonResponse(json)};
        response->setStatusCode(drogon::HttpStatusCode::k400BadRequest);
        callback(response);
        return;
    }

    std::string confirm_password {request->getParameter("confirmPassword")};
    if (password != confirm_password) {
        json["message"] = "Поле подтверждения пароля не совпадает с полем пароль";
        drogon::HttpResponsePtr response {drogon::HttpResponse::newHttpJsonResponse(json)};
        response->setStatusCode(drogon::HttpStatusCode::k400BadRequest);
        callback(response);
        return;
    }

    if (!std::regex_match(email, EMAIL_PATTERN)) {
        json["message"] = "Невалидное значение поля email";
        drogon::HttpResponsePtr response {drogon::HttpResponse::newHttpJsonResponse(json)};
        response->setStatusCode(drogon::HttpStatusCode::k400BadRequest);
        callback(response);
        return;
    }
    try {
        if (checkEmail(client, email)) {
            // Email already exists
            json["message"] = "Аккаунт с таким email уже существует";
            drogon::HttpResponsePtr response {drogon::HttpResponse::newHttpJsonResponse(json)};
            response->setStatusCode(drogon::HttpStatusCode::k409Conflict);
            callback(response);
            return;
        }
        // Email is available, proceed with registration
        User new_user {first_name, last_name, email, sha256(password)};
        insertUser(client, new_user);
        json["message"] = "Пользователь зарегистрирован";
        drogon::HttpResponsePtr response {drogon::HttpResponse::newHttpJsonResponse(json)};
        response->setStatusCode(drogon::HttpStatusCode::k201Created);
        uint32_t id {getUserId(client, new_user)};
        request->session()->insert("userId", id);
#ifdef DEBUG_AUTH
        LOG_INFO << "User with email: " << email << " has logged in\n";
#endif
        callback(response);
    } catch (const drogon::orm::DrogonDbException& e) {
        LOG_ERROR << e.base().what() << '\n';
        json["message"] = "Проблема на стороне сервера, невозможно создать учетную запись";
        drogon::HttpResponsePtr response {drogon::HttpResponse::newHttpJsonResponse(json)};
        response->setStatusCode(drogon::HttpStatusCode::k500InternalServerError);
        callback(response);
        return;
    }
}

void loginUser(const drogon::HttpRequestPtr& request, Callback&& callback) {
    drogon::orm::DbClientPtr client = drogon::app().getDbClient("data");
    std::string email {request->getParameter("email")};
    std::string password {sha256(request->getParameter("password"))};

    Json::Value json;
    drogon::HttpResponsePtr response;

    std::future<drogon::orm::Result> coroutine {
        client->execSqlAsyncFuture("SELECT id FROM user WHERE email = $1 AND password = $2", email, password)};
    try {
        drogon::orm::Result res {coroutine.get()};
        if (res.empty()) {
            json["message"] = "Неверный email или пароль";
            response = drogon::HttpResponse::newHttpJsonResponse(json);
            response->setStatusCode(drogon::HttpStatusCode::k401Unauthorized);
#ifdef DEBUG_AUTH
            LOG_INFO << "Trying to sign in with email: " << email << '\n';
#endif
        } else {
            json["message"] = "Успешный вход";
            response = drogon::HttpResponse::newHttpJsonResponse(json);
            response->setStatusCode(drogon::HttpStatusCode::k200OK);
            u_int32_t id = res.front()[0].as<u_int32_t>();
            request->session()->insert("userId", id);
#ifdef DEBUG_AUTH
            LOG_INFO << "User with email: " << email << " has logged in\n";
#endif
        }
        callback(response);
    } catch (const drogon::orm::DrogonDbException& e) {
        LOG_INFO << e.base().what() << '\n';
        json["message"] = "Проблема на стороне сервера, невозможно войти в учетную запись";
        drogon::HttpResponsePtr response {drogon::HttpResponse::newHttpJsonResponse(json)};
        response->setStatusCode(drogon::HttpStatusCode::k500InternalServerError);
        callback(response);
        return;
    }
}

void rootHandler(const drogon::HttpRequestPtr& request, Callback&& callback) {
    u_int32_t user_id {request->session()->getOptional<u_int32_t>("userId").value_or(0)};
    drogon::HttpResponsePtr response;
    if (!user_id)
        response = drogon::HttpResponse::newRedirectionResponse("/login.html");
    else
        response = drogon::HttpResponse::newRedirectionResponse("/index.html");

    callback(response);
}

void indexHandler(const drogon::HttpRequestPtr& request, Callback&& callback) {
    u_int32_t user_id {request->session()->getOptional<u_int32_t>("userId").value_or(0)};
    drogon::HttpResponsePtr response;
    if (!user_id)
        response = drogon::HttpResponse::newRedirectionResponse("/login.html");
    else
        response = drogon::HttpResponse::newHttpViewResponse("Index");
    callback(response);
}

void loginHandler(const drogon::HttpRequestPtr& request, Callback&& callback) {
    u_int32_t user_id {request->session()->getOptional<u_int32_t>("userId").value_or(0)};
    drogon::HttpResponsePtr response;
    if (!user_id)
        response = drogon::HttpResponse::newHttpViewResponse("Login");
    else
        response = drogon::HttpResponse::newRedirectionResponse("/index.html");
    callback(response);
}

void registerHandler(const drogon::HttpRequestPtr& request, Callback&& callback) {
    u_int32_t user_id {request->session()->getOptional<u_int32_t>("userId").value_or(0)};
    drogon::HttpResponsePtr response;
    if (!user_id)
        response = drogon::HttpResponse::newHttpViewResponse("Register");
    else
        response = drogon::HttpResponse::newRedirectionResponse("/index.html");
    callback(response);
}
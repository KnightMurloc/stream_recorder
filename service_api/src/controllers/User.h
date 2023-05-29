#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

class User : public drogon::HttpController<User>
{
public:


    METHOD_LIST_BEGIN
        METHOD_ADD(User::login, "/login",Post, "JsonCheck");
        METHOD_ADD(User::info, "/info?token={token}", Get, "TokenCheck");
        METHOD_ADD(User::logout,"/logout?token={token}",Get, "TokenCheck");
        METHOD_ADD(User::register_user, "/register", Post, "JsonCheck");
        METHOD_ADD(User::create_tmp,"/create_tmp",Get);
    METHOD_LIST_END


    // your declaration of processing function maybe like this:
    // void get(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, int p1, std::string p2);
    // void your_method_name(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, double p1, int p2) const;

protected:
    void login(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);
    void test(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);
    void info(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, const std::string& token);
    void logout(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, const std::string& token);
    void register_user(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);
    void create_tmp(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);
};

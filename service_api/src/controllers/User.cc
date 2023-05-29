#include "User.h"
#include <iostream>
#include <random>
#include <chrono>

#include <drogon/orm/DbClient.h>
#include "../models/Sessions.h"
#include "../models/Users.h"
#include "../models/Records.h"
#include "../utils.h"
#include <openssl/sha.h>

void sql_err(const orm::DrogonDbException &e){
    LOG_ERROR << e.base().what();
}

std::string random_string()
{
    std::string str("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");

    std::random_device rd;
    std::mt19937 generator(rd());

    std::shuffle(str.begin(), str.end(), generator);

    auto hash_content = std::to_string(clock()) + str.substr(0, 10);
    unsigned char hash[SHA256_DIGEST_LENGTH]; // == 32
    SHA256((unsigned char*) hash_content.c_str(),hash_content.length() - 1,hash);

    std::stringstream ss;
    for(int i=0; i<SHA_DIGEST_LENGTH; ++i)
        ss << std::hex << (int)hash[i];

    return ss.str();
}

std::string hash_string(std::string_view str){
    unsigned char hash[SHA256_DIGEST_LENGTH]; // == 32
    SHA256((unsigned char*) str.data(),str.length(),hash);

    std::stringstream ss;
    for(int i=0; i<SHA256_DIGEST_LENGTH; ++i)
        ss << std::hex << (int)hash[i];
    return ss.str();
}

void User::login(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {

    auto data = req->getJsonObject();
    auto clientPtr = drogon::app().getDbClient("postgres");
    LOG_INFO << data->operator[]("username").asString();

    if(data->operator[]("username").asString().empty() ||
		   data->operator[]("password").asString().empty()){
        Json::Value resp_json;
        resp_json["error"] = "username or password is empty";
        auto resp = HttpResponse::newHttpJsonResponse(resp_json);
        callback(resp);
        return;
    }

    clientPtr->execSqlAsync("select id, password from users where email=$1 limit 1",[data, callback, clientPtr](const drogon::orm::Result &result){
        Json::Value resp_json;

        if(result.empty()){
            resp_json["error"] = "user not found";
            auto resp = HttpResponse::newHttpJsonResponse(resp_json);
            callback(resp);
            return;
        }

				LOG_INFO << hash_string(data->operator[]("password").asString());
				LOG_INFO << data->operator[]("password").asString();

        if(hash_string(data->operator[]("password").asString()) == result.front()[1].as<std::string>()){
            resp_json["token"] = random_string();
            //clientPtr->execSqlSync(
            //        "insert into sessions(token, user_id) values($1, $2)",
            //        resp_json["token"],
            //        result.front()[0].as<int>());

						*clientPtr << "insert into sessions(token, user_id) values($1, $2)"
											 << resp_json["token"].asString()
										   << result.front()[0].as<int>() >> sql_err;

            auto resp = HttpResponse::newHttpJsonResponse(resp_json);
            callback(resp);
            return;
        }

        resp_json["error"] = "wrong password";
        auto resp = HttpResponse::newHttpJsonResponse(resp_json);
        callback(resp);

    },sql_err,data->operator[]("username").asString());
}

void User::test(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    auto resp = HttpResponse::newHttpResponse();
    callback(resp);
}

void User::info(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, const std::string& token) {
    LOG_INFO << "User/Info: " + token;
    auto clientPtr = drogon::app().getDbClient("postgres");

    using namespace orm;
    using drogon_model::main::Records;
    using drogon_model::main::Users;
    using drogon_model::main::Sessions;

    Mapper<Sessions> sessions(clientPtr);
    Mapper<Users> users(clientPtr);
    Mapper<Records> records(clientPtr);

    Users user = users.findByPrimaryKey(sessions.findByPrimaryKey(token).getValueOfUserId());

    auto size_future = clientPtr->execSqlAsyncFuture("select sum(size) from records where user_id = $1;",
																										  user.getValueOfId());

    Json::Value resp_json;
    resp_json["username"] = user.getValueOfEmail();
    resp_json["is_tmp"] = user.getValueOfIsTmp();
    resp_json["total_size"] = size_future.get().front()[0].as<int>();

    callback(HttpResponse::newHttpJsonResponse(resp_json));

//    user.getRecords(clientPtr,[](const std::vector<Records>& records){
//
//    },SQL_ERROR_RESPONSE);

//    clientPtr->execSqlAsync(
//            "SELECT username, is_tmp from users WHERE id=(SELECT user_id from sessions where token=$1) limit 1",
//            [callback](const drogon::orm::Result &result){
//        if(result.empty()){
//            Json::Value resp_json;
//            resp_json["error"] = "invalid token";
//            auto resp = HttpResponse::newHttpJsonResponse(resp_json);
//            callback(resp);
//            return;
//        }
//
//        auto user = result.front();
//
//        Json::Value resp_json;
//        resp_json["username"] = user[0].as<std::string>();
//        resp_json["is_tmp"] = user[1].as<bool>();
//        LOG_INFO << resp_json["username"].asString();
//
//        auto resp = HttpResponse::newHttpJsonResponse(resp_json);
//        callback(resp);
//
//    },sql_err,token);
}

void User::logout(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback,
                  const std::string &token) {
    LOG_INFO << "logout: " + token;
    
		auto clientPtr = drogon::app().getDbClient("postgres");
    clientPtr->execSqlSync("delete from sessions where token=$1", token);
    auto resp = HttpResponse::newHttpResponse();
    callback(resp);

}

void User::register_user(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    auto data = req->getJsonObject();

    auto clientPtr = drogon::app().getDbClient("postgres");

    if(not (data->operator []("password").isString() and
       data->operator []("password_rep").isString() and
       data->operator []("username").isString())){
        response400(callback, "wrong arguments");
        return;
    }

    clientPtr->execSqlAsync("select id from users where email = $1", [callback, data](const drogon::orm::Result &result){
        if(result.empty()){

            if(data->operator[]("password").asString() != data->operator[]("password_rep").asString()){
                Json::Value response;
                response["error"] = "password not match";
                auto resp = HttpResponse::newHttpJsonResponse(response);
                callback(resp);
                return;
            }

            auto clientPtr = drogon::app().getDbClient("postgres");



            *clientPtr << "insert into users(email, password) values($1, $2)"
                       << data->operator[]("username")
                       << hash_string(data->operator[]("password").asString())
                       >> sql_err;

//            clientPtr->execSqlSync("insert into users(username, password) values($1, $2)",
//                    data->operator[]("username"),
//                    data->operator[]("password"));

//            Json::Value value;

//            auto resp = HttpResponse::newHttpJsonResponse(value);
            auto resp = HttpResponse::newHttpResponse();
            callback(resp);
        }else{
            Json::Value response;
            response["error"] = "username already used";
            auto resp = HttpResponse::newHttpJsonResponse(response);
            callback(resp);
        }
    },sql_err,data->operator[]("username"));
}

void User::create_tmp(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    auto clientPtr = drogon::app().getDbClient("postgres");

    int id;
    std::string tmp_username = "tmp_" + random_string();

    try {
        id = clientPtr->execSqlSync("insert into users(email, is_tmp) values($1, true) returning id",
                                    tmp_username).front()[0].as<int>();
    }catch(orm::DrogonDbException&e) {
        Json::Value resp_json;
        resp_json["error"] = "create tmp user failed";
        auto resp = HttpResponse::newHttpJsonResponse(resp_json);
        callback(resp);
        return;
    }

    Json::Value resp_json;
    resp_json["token"] = random_string();
    clientPtr->execSqlSync(
            "insert into sessions(token, user_id) values($1, $2)",
            resp_json["token"],
            id);

    auto resp = HttpResponse::newHttpJsonResponse(resp_json);
    callback(resp);

}

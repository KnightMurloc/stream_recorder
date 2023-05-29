//
// Created by victor on 15.03.2023.
//

#ifndef DRAGON2_TEST_UTILS_H
#define DRAGON2_TEST_UTILS_H

#include <drogon/drogon.h>

#define SQL_ERROR_RESPONSE [callback](const orm::DrogonDbException& e){ \
 LOG_WARN << e.base().what();\
 auto response = HttpResponse::newHttpResponse();\
 response->setStatusCode(k500InternalServerError);\
 callback(response);\
}

static void response400(std::function<void(const drogon::HttpResponsePtr &)> &callback, const std::string& error_text){
    Json::Value error;
    error["error"] = error_text;
    auto response = drogon::HttpResponse::newHttpJsonResponse(error);
    response->setStatusCode(drogon::k400BadRequest);
    callback(response);
}

static void response500(std::function<void(const drogon::HttpResponsePtr &)> &callback){
    auto response = drogon::HttpResponse::newHttpResponse();
    response->setStatusCode(drogon::k500InternalServerError);
    callback(response);
}

#endif //DRAGON2_TEST_UTILS_H

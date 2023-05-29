/**
 *
 *  TokenCheck.cc
 *
 */

#include <drogon/HttpAppFramework.h>
#include "TokenCheck.h"
#include "drogon/orm/DbClient.h"

void TokenCheck::doFilter(const HttpRequestPtr &req,
                         FilterCallback &&fcb,
                         FilterChainCallback &&fccb)
{
    auto clientPtr = drogon::app().getDbClient("postgres");

    try {
        auto result = clientPtr->execSqlSync("select 1 from sessions where token = $1 limit 1",req->getParameter("token"));
        if(!result.empty()){
            fccb();
        }else{
            Json::Value response;
            response["error"] = "invalid token";

            auto res = drogon::HttpResponse::newHttpJsonResponse(response);
            res->setStatusCode(k400BadRequest);
            fcb(res);
        }
    }catch(orm::DrogonDbException&e) {
        LOG_INFO << e.base().what();
        auto res = drogon::HttpResponse::newHttpResponse();
        res->setStatusCode(k500InternalServerError);
        fcb(res);
    }

//    clientPtr->execSqlAsync("select id from sessions where token = $1 limit 1",[fcb, fccb](const drogon::orm::Result &result){
//        if(!result.empty()){
//            fccb();
//            return;
//        }else{
//            Json::Value response;
//            response["error"] = "invalid token";
//
//            auto res = drogon::HttpResponse::newHttpJsonResponse(response);
//            res->setStatusCode(k400BadRequest);
//            fcb(res);
//        }
//    },[](const orm::DrogonDbException &e){
//        LOG_INFO << e.base().what();
//    },req->getParameter("token"));
}

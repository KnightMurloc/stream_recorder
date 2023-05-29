/**
 *
 *  fillters/JsonCheck.cc
 *
 */

#include "JsonCheck.h"

using namespace drogon;

void JsonCheck::doFilter(const HttpRequestPtr &req,
                         FilterCallback &&fcb,
                         FilterChainCallback &&fccb)
{
    if (req->jsonObject())
    {
        //Passed
        fccb();
        return;
    }
    LOG_INFO << req->getJsonError();

    //Check failed
    auto res = drogon::HttpResponse::newHttpResponse();
    res->setStatusCode(k400BadRequest);
    fcb(res);
}

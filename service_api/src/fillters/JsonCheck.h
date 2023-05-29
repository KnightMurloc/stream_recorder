/**
 *
 *  fillters/JsonCheck.h
 *
 */

#pragma once

#include <drogon/HttpFilter.h>
using namespace drogon;


class JsonCheck : public HttpFilter<JsonCheck>
{
  public:
    JsonCheck() {}
    void doFilter(const HttpRequestPtr &req,
                  FilterCallback &&fcb,
                  FilterChainCallback &&fccb) override;
};


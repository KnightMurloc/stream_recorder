/**
 *
 *  TokenCheck.h
 *
 */

#pragma once

#include <drogon/HttpFilter.h>
using namespace drogon;


class TokenCheck : public HttpFilter<TokenCheck>
{
  public:
    TokenCheck() {}
    void doFilter(const HttpRequestPtr &req,
                  FilterCallback &&fcb,
                  FilterChainCallback &&fccb) override;
};


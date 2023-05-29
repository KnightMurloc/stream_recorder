#pragma once

#include <drogon/HttpController.h>
#include <drogon/HttpClient.h>

using namespace drogon;

class Record : public drogon::HttpController<Record>
{
private:
    HttpClientPtr client_record = HttpClient::newHttpClient(std::getenv("BALANCER"));
    HttpClientPtr client_info = HttpClient::newHttpClient(std::getenv("GET_INFO"));

public:

    METHOD_LIST_BEGIN

    METHOD_ADD(Record::make,"/make?token={token}",Post,"TokenCheck", "JsonCheck");
    METHOD_ADD(Record::list, "/list?token={token}",Get,"TokenCheck");
    METHOD_ADD(Record::remove, "/remove?token={token}",Post,"TokenCheck","JsonCheck");

    METHOD_LIST_END

    // your declaration of processing function maybe like this:
    // void get(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, int p1, std::string p2);
    // void your_method_name(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, double p1, int p2) const;
    void make(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, const std::string& token);
    void list(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, const std::string& token);
    void remove(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, const std::string& token);
};

#pragma once

#include <drogon/HttpController.h>
#include <drogon/HttpClient.h>
#include "concurrentqueue/concurrentqueue.h"
#include <thread>
#include <list>
#include <limits>

using namespace drogon;

using moodycamel::ConcurrentQueue;

class queue : public drogon::HttpController<queue>
{
private:

    struct Client{
        float load;
        time_t time;
        std::string host;
        HttpClientPtr p_client;
    };

    ConcurrentQueue<Json::Value> qq;

    std::thread thread;

    [[noreturn]] void process();

//    HttpClientPtr client = HttpClient::newHttpClient("http://localhost:7000");

//    std::multimap<float,HttpClientPtr> clients;

    std::list<Client> clients;


    HttpClientPtr get_priority_client();
public:

    METHOD_LIST_BEGIN

    METHOD_ADD(queue::push, "/push",Post);
    METHOD_ADD(queue::heart_beat, "/heart_beat?address={address}&load={load}", Get);
    METHOD_ADD(queue::remove, "/remove",Post);

    METHOD_LIST_END

    queue();

    void push(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);
    void remove(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);
    void heart_beat(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback,
            const std::string& address,
            float load);
};
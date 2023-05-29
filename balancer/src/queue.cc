#include "queue.h"
#include <string>
#include <unistd.h>
// Add definition of your processing function here
void queue::push(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {

    auto data = req->getJsonObject();
    if(data == nullptr){
        LOG_INFO << req->getJsonError();
        auto response = HttpResponse::newHttpResponse();
        response->setStatusCode(HttpStatusCode::k400BadRequest);
        callback(response);
        return;
    }

    auto url = data->operator[]("url").asString();
    data->operator[]("type") = "push";
    qq.enqueue(*data);
    auto response = HttpResponse::newHttpResponse();
    callback(response);

}

queue::queue() {
    LOG_INFO << "queue created";
 
    thread = std::thread(&queue::process,this);
}

[[noreturn]] void queue::process() {
    while(true){
        Json::Value package;
        if(qq.try_dequeue(package)){

            if(package["type"] == "push") {

                std::cout << package["url"] << " " << package["id"] << std::endl;

                auto request = HttpRequest::newHttpJsonRequest(package);
                request->setMethod(Post);
                request->setPath("/start_record");
                auto client = get_priority_client();
                if(client) {
                    client->sendRequest(request);
                }else {
                    LOG_INFO << "no available record servers";
                    qq.enqueue(package);
                }

            }else if(package["type"] == "remove"){
                LOG_INFO << "test";
                auto request = HttpRequest::newHttpJsonRequest(package);
                request->setMethod(Post);
                request->setPath("/delete");
                auto client = get_priority_client();

                if(client) {
                    client->sendRequest(request);
                }else {
                    LOG_INFO << "no available record servers";
                    qq.enqueue(package);
                }
            }
        }else{
            usleep(1000);
        }
    }
}

void queue::heart_beat(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, const std::string& address,
                       float load) {
    std::cout << "heart_beat: " << address << std::endl;
    for(auto& cl : clients){
        if(cl.host == address){
            cl.load = load;
            cl.time = clock();
            callback(HttpResponse::newHttpResponse());
            return;
        }
    }

    clients.push_back(Client{
        .load = load,
        .time = clock(),
        .host = address,
        .p_client = HttpClient::newHttpClient(address)
    });
    callback(HttpResponse::newHttpResponse());

}

HttpClientPtr queue::get_priority_client() {
    HttpClientPtr p_client;
    float min_load = std::numeric_limits<float>::max();
    for(const auto& client : clients){
        if(client.load < min_load){
            min_load = client.load;
            p_client = client.p_client;
        }
    }
    return p_client;
}

void queue::remove(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    auto data = req->getJsonObject();
    if(data == nullptr){
        LOG_INFO << req->getJsonError();
        auto response = HttpResponse::newHttpResponse();
        response->setStatusCode(HttpStatusCode::k400BadRequest);
        callback(response);
        return;
    }
		LOG_INFO << "remove";
		data->operator[]("type") = "remove";
    qq.enqueue(*data);
    auto response = HttpResponse::newHttpResponse();
    callback(response);
}

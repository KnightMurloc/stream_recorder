#include "Record.h"
#include <regex>
#include "../utils.h"
#include "../models/Users.h"
#include "../models/Records.h"
#include "../models/Sessions.h"
#include <iostream>

#define GIGABIT 8388608

bool validate(const std::string& url){
    static std::regex expr_is_youtube(
R"(^((?:https?:)?\/\/)?((?:www|m)\.)?((?:youtube\.com|youtu.be))(\/(?:[\w\-]+\?v=|embed\/|v\/)?)([\w\-]+)(\S+)?$)");
    static std::regex expr_is_twitch(R"(https?://www.twitch.tv/(#)?[a-zA-Z0-9][\w]{2,24}$)");

    return !(!std::regex_match(url, expr_is_youtube) && !std::regex_match(url, expr_is_twitch));
}

std::string get_video_id(const std::string& url){
    return url.substr(url.find('=') + 1);
}

// Add definition of your processing function here
void Record::make(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, const std::string& token) {

    using orm::Mapper;
    using drogon_model::main::Sessions;
    using drogon_model::main::Users;

    auto data = req->getJsonObject();

    if(not (data->operator[]("url").isString() && not data->operator[]("url").empty() &&
            data->operator[]("format_id").isString() && not data->operator[]("format_id").empty() &&
            data->operator[]("time").isString() && not data->operator[]("time").empty())){
        response400(callback,"wrong_arguments");
        return;
    }
    std::cout << "argument check fine!" << std::endl;
    auto url = data->operator[]("url").asString();
    auto record_time = data->operator[]("time").asString();
    auto format_id = data->operator[]("format_id").asString();
    LOG_INFO << url;
    std::cout << url << std::endl;
    if(not validate(url)){
        response400(callback,"url not valid");
    }
    auto clientPtr = drogon::app().getDbClient("postgres");

    Mapper<Sessions> sessions(clientPtr);
    Mapper<Users> users(clientPtr);

    auto session = sessions.findByPrimaryKey(token);
    auto user = users.findByPrimaryKey(session.getValueOfUserId());

    if(user.getValueOfIsTmp()){
        struct tm tm;
        strptime(record_time.c_str(), "%H:%M:%S", &tm);
        if (tm.tm_hour > 0 or tm.tm_min > 10){
            response400(callback,"record to long");
            return;
        }
    }
    std::cout << "user check fine!" << std::endl;

    std::future<orm::Result> record_id;
    if(not data->operator[]("is_wait").asBool()){

        auto size_future = clientPtr->execSqlAsyncFuture("select sum(size) from records where user_id = $1;",
                user.getValueOfId());

        auto request = HttpRequest::newHttpRequest();
        request->setParameter("url", url);
        request->setParameter("time", record_time);
        request->setParameter("format_id", format_id);

        request->setPath("/get_size");
        auto response = client_info->sendRequest(request);
        std::cout << "size request fine!" << std::endl;
        if(response.first != ReqResult::Ok || response.second->getStatusCode() != k200OK){
            response400(callback,(std::string) response.second->getBody());
            return;
        }

        size_t size = response.second->getJsonObject()->operator[]("size").asUInt64();
        std::cout << size << std::endl;
        if(size > GIGABIT or size_future.get().front()[0].as<int>() + size > 10 * GIGABIT){
            response400(callback,"record too long");
            return;
        }
        std::cout << "size check fine!" << std::endl;
        record_id = clientPtr->execSqlAsyncFuture(
                "insert into records(url,user_id) values ($1, $2) returning id",
                url,user.getValueOfId());
    }else{

        record_id = clientPtr->execSqlAsyncFuture(
         "insert into records(url,user_id,status,thumbnail) values ($1, $2, $3,$4) returning id",
         url, user.getValueOfId(), "wait", "https://i.ytimg.com/vi/" + get_video_id(url) + "/maxresdefault.jpg");
    
    }



    {
        Json::Value req_json;
        req_json["url"] = url;
        req_json["time"] = record_time;
        req_json["id"] = record_id.get().front()[0].as<int>();
        req_json["format_id"] = format_id;
        req_json["is_wait"] = data->operator[]("is_wait").asBool();

        auto request = HttpRequest::newHttpJsonRequest(req_json);
        request->setMethod(Post);
        request->setPath("/queue/push");
        client_record->sendRequest(request);
        std::cout << "push to queue fine!" << std::endl;
    }
    auto response = HttpResponse::newHttpResponse();
    callback(response);
    std::cout << "response fine!" << std::endl;
}

void Record::list(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback,
                  const std::string &token) {
    using orm::Mapper;
    using drogon_model::main::Users;
    using drogon_model::main::Sessions;
    using drogon_model::main::Records;
    using orm::Criteria;
    using orm::CompareOperator;

    auto clientPtr = drogon::app().getDbClient("postgres");

    Mapper<Sessions> sessions(clientPtr);
    Mapper<Records> records(clientPtr);

    auto session = sessions.findByPrimaryKey(token);

    int user_id = session.getValueOfUserId();

    records.orderBy(Records::Cols::_id, orm::SortOrder::DESC).findBy(
            Criteria(Records::Cols::_user_id, CompareOperator::EQ,user_id) &&
            Criteria(Records::Cols::_thumbnail, CompareOperator::NE,""),
            [callback](const std::vector<Records>& records){
                Json::Value response;
                Json::Value records_array;
                for(const auto& record : records){
                    Json::Value record_json;
                    record_json["id"] = record.getValueOfId();
                    record_json["title"] = record.getValueOfTitle();
                    record_json["thumbnail"] = record.getValueOfThumbnail();
//                    record_json["store_server"] = record.getValueOfStoreServer();
                    record_json["record_file"] = record.getValueOfRecordFile();
                    record_json["status"] = record.getValueOfStatus();
                    records_array.append(record_json);
                }

                response["records"] = records_array;
                callback(HttpResponse::newHttpJsonResponse(response));
            },
            SQL_ERROR_RESPONSE);

//    auto clientPtr = drogon::app().getDbClient("postgres");
//    clientPtr->execSqlAsync("select title,thumbnail,store_server,record_file,status from records"
//                            " where user_id = (select user_id from sessions where token = $1) order by id desc;",
//            [callback](const drogon::orm::Result &result){
//        Json::Value response;
//        Json::Value records_array;
//        for(const auto& row : result){
//            Json::Value record;
//            record["title"] = row[0].as<std::string>();
//            record["thumbnail"] = row[1].as<std::string>();
//            record["store_server"] = row[2].as<std::string>();
//            record["record_file"] = row[3].as<std::string>();
//            record["status"] = row[4].as<std::string>();
//            records_array.append(record);
//        }
//
//        response["records"] = records_array;
//
//        auto resp = HttpResponse::newHttpJsonResponse(response);
//        callback(resp);
//
//    },[](const orm::DrogonDbException &e){
//        LOG_ERROR << e.base().what();
//    }, token);
}

void Record::remove(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback,
                    const std::string &token) {
    using namespace orm;
    using drogon_model::main::Users;
    using drogon_model::main::Sessions;

    auto data = req->getJsonObject();

    if(not data->operator[]("id").isInt()){
        response400(callback,"wrong_arguments");
        return;
    }

    auto clientPtr = drogon::app().getDbClient("postgres");
    Mapper<Sessions> sessions(clientPtr);

    auto session = sessions.findByPrimaryKey(token);

    int user_id = session.getValueOfUserId();

    bool record_exist = clientPtr->execSqlSync(
						"select exists(select 1 from records where user_id = $1 and id = $2 limit 1)",
            user_id, data->operator []("id").asInt())
            .front()[0].as<bool>();
    if(not record_exist){
        response400(callback,"access denied");
        return;
    }


		LOG_INFO << data->operator[]("id").asInt();

    {

        Json::Value req_json;
        req_json["id"] = data->operator[]("id").asInt();

        auto request = HttpRequest::newHttpJsonRequest(req_json);
        request->setMethod(Post);
        request->setPath("/queue/remove");
        client_record->sendRequest(request);

    }


    callback(HttpResponse::newHttpResponse());
}

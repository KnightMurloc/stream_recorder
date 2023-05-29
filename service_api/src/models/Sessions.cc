/**
 *
 *  Sessions.cc
 *  DO NOT EDIT. This file is generated by drogon_ctl
 *
 */

#include "Sessions.h"
#include "Users.h"
#include <drogon/utils/Utilities.h>
#include <string>

using namespace drogon;
using namespace drogon::orm;
using namespace drogon_model::main;

const std::string Sessions::Cols::_token = "token";
const std::string Sessions::Cols::_user_id = "user_id";
const std::string Sessions::Cols::_create_time = "create_time";
const std::string Sessions::primaryKeyName = "token";
const bool Sessions::hasPrimaryKey = true;
const std::string Sessions::tableName = "sessions";

const std::vector<typename Sessions::MetaData> Sessions::metaData_={
{"token","std::string","character varying",0,0,1,1},
{"user_id","int32_t","integer",4,0,0,1},
{"create_time","::trantor::Date","timestamp without time zone",0,0,0,1}
};
const std::string &Sessions::getColumnName(size_t index) noexcept(false)
{
    assert(index < metaData_.size());
    return metaData_[index].colName_;
}
Sessions::Sessions(const Row &r, const ssize_t indexOffset) noexcept
{
    if(indexOffset < 0)
    {
        if(!r["token"].isNull())
        {
            token_=std::make_shared<std::string>(r["token"].as<std::string>());
        }
        if(!r["user_id"].isNull())
        {
            userId_=std::make_shared<int32_t>(r["user_id"].as<int32_t>());
        }
        if(!r["create_time"].isNull())
        {
            auto timeStr = r["create_time"].as<std::string>();
            struct tm stm;
            memset(&stm,0,sizeof(stm));
            auto p = strptime(timeStr.c_str(),"%Y-%m-%d %H:%M:%S",&stm);
            time_t t = mktime(&stm);
            size_t decimalNum = 0;
            if(p)
            {
                if(*p=='.')
                {
                    std::string decimals(p+1,&timeStr[timeStr.length()]);
                    while(decimals.length()<6)
                    {
                        decimals += "0";
                    }
                    decimalNum = (size_t)atol(decimals.c_str());
                }
                createTime_=std::make_shared<::trantor::Date>(t*1000000+decimalNum);
            }
        }
    }
    else
    {
        size_t offset = (size_t)indexOffset;
        if(offset + 3 > r.size())
        {
            LOG_FATAL << "Invalid SQL result for this model";
            return;
        }
        size_t index;
        index = offset + 0;
        if(!r[index].isNull())
        {
            token_=std::make_shared<std::string>(r[index].as<std::string>());
        }
        index = offset + 1;
        if(!r[index].isNull())
        {
            userId_=std::make_shared<int32_t>(r[index].as<int32_t>());
        }
        index = offset + 2;
        if(!r[index].isNull())
        {
            auto timeStr = r[index].as<std::string>();
            struct tm stm;
            memset(&stm,0,sizeof(stm));
            auto p = strptime(timeStr.c_str(),"%Y-%m-%d %H:%M:%S",&stm);
            time_t t = mktime(&stm);
            size_t decimalNum = 0;
            if(p)
            {
                if(*p=='.')
                {
                    std::string decimals(p+1,&timeStr[timeStr.length()]);
                    while(decimals.length()<6)
                    {
                        decimals += "0";
                    }
                    decimalNum = (size_t)atol(decimals.c_str());
                }
                createTime_=std::make_shared<::trantor::Date>(t*1000000+decimalNum);
            }
        }
    }

}

Sessions::Sessions(const Json::Value &pJson, const std::vector<std::string> &pMasqueradingVector) noexcept(false)
{
    if(pMasqueradingVector.size() != 3)
    {
        LOG_ERROR << "Bad masquerading vector";
        return;
    }
    if(!pMasqueradingVector[0].empty() && pJson.isMember(pMasqueradingVector[0]))
    {
        dirtyFlag_[0] = true;
        if(!pJson[pMasqueradingVector[0]].isNull())
        {
            token_=std::make_shared<std::string>(pJson[pMasqueradingVector[0]].asString());
        }
    }
    if(!pMasqueradingVector[1].empty() && pJson.isMember(pMasqueradingVector[1]))
    {
        dirtyFlag_[1] = true;
        if(!pJson[pMasqueradingVector[1]].isNull())
        {
            userId_=std::make_shared<int32_t>((int32_t)pJson[pMasqueradingVector[1]].asInt64());
        }
    }
    if(!pMasqueradingVector[2].empty() && pJson.isMember(pMasqueradingVector[2]))
    {
        dirtyFlag_[2] = true;
        if(!pJson[pMasqueradingVector[2]].isNull())
        {
            auto timeStr = pJson[pMasqueradingVector[2]].asString();
            struct tm stm;
            memset(&stm,0,sizeof(stm));
            auto p = strptime(timeStr.c_str(),"%Y-%m-%d %H:%M:%S",&stm);
            time_t t = mktime(&stm);
            size_t decimalNum = 0;
            if(p)
            {
                if(*p=='.')
                {
                    std::string decimals(p+1,&timeStr[timeStr.length()]);
                    while(decimals.length()<6)
                    {
                        decimals += "0";
                    }
                    decimalNum = (size_t)atol(decimals.c_str());
                }
                createTime_=std::make_shared<::trantor::Date>(t*1000000+decimalNum);
            }
        }
    }
}

Sessions::Sessions(const Json::Value &pJson) noexcept(false)
{
    if(pJson.isMember("token"))
    {
        dirtyFlag_[0]=true;
        if(!pJson["token"].isNull())
        {
            token_=std::make_shared<std::string>(pJson["token"].asString());
        }
    }
    if(pJson.isMember("user_id"))
    {
        dirtyFlag_[1]=true;
        if(!pJson["user_id"].isNull())
        {
            userId_=std::make_shared<int32_t>((int32_t)pJson["user_id"].asInt64());
        }
    }
    if(pJson.isMember("create_time"))
    {
        dirtyFlag_[2]=true;
        if(!pJson["create_time"].isNull())
        {
            auto timeStr = pJson["create_time"].asString();
            struct tm stm;
            memset(&stm,0,sizeof(stm));
            auto p = strptime(timeStr.c_str(),"%Y-%m-%d %H:%M:%S",&stm);
            time_t t = mktime(&stm);
            size_t decimalNum = 0;
            if(p)
            {
                if(*p=='.')
                {
                    std::string decimals(p+1,&timeStr[timeStr.length()]);
                    while(decimals.length()<6)
                    {
                        decimals += "0";
                    }
                    decimalNum = (size_t)atol(decimals.c_str());
                }
                createTime_=std::make_shared<::trantor::Date>(t*1000000+decimalNum);
            }
        }
    }
}

void Sessions::updateByMasqueradedJson(const Json::Value &pJson,
                                            const std::vector<std::string> &pMasqueradingVector) noexcept(false)
{
    if(pMasqueradingVector.size() != 3)
    {
        LOG_ERROR << "Bad masquerading vector";
        return;
    }
    if(!pMasqueradingVector[0].empty() && pJson.isMember(pMasqueradingVector[0]))
    {
        if(!pJson[pMasqueradingVector[0]].isNull())
        {
            token_=std::make_shared<std::string>(pJson[pMasqueradingVector[0]].asString());
        }
    }
    if(!pMasqueradingVector[1].empty() && pJson.isMember(pMasqueradingVector[1]))
    {
        dirtyFlag_[1] = true;
        if(!pJson[pMasqueradingVector[1]].isNull())
        {
            userId_=std::make_shared<int32_t>((int32_t)pJson[pMasqueradingVector[1]].asInt64());
        }
    }
    if(!pMasqueradingVector[2].empty() && pJson.isMember(pMasqueradingVector[2]))
    {
        dirtyFlag_[2] = true;
        if(!pJson[pMasqueradingVector[2]].isNull())
        {
            auto timeStr = pJson[pMasqueradingVector[2]].asString();
            struct tm stm;
            memset(&stm,0,sizeof(stm));
            auto p = strptime(timeStr.c_str(),"%Y-%m-%d %H:%M:%S",&stm);
            time_t t = mktime(&stm);
            size_t decimalNum = 0;
            if(p)
            {
                if(*p=='.')
                {
                    std::string decimals(p+1,&timeStr[timeStr.length()]);
                    while(decimals.length()<6)
                    {
                        decimals += "0";
                    }
                    decimalNum = (size_t)atol(decimals.c_str());
                }
                createTime_=std::make_shared<::trantor::Date>(t*1000000+decimalNum);
            }
        }
    }
}

void Sessions::updateByJson(const Json::Value &pJson) noexcept(false)
{
    if(pJson.isMember("token"))
    {
        if(!pJson["token"].isNull())
        {
            token_=std::make_shared<std::string>(pJson["token"].asString());
        }
    }
    if(pJson.isMember("user_id"))
    {
        dirtyFlag_[1] = true;
        if(!pJson["user_id"].isNull())
        {
            userId_=std::make_shared<int32_t>((int32_t)pJson["user_id"].asInt64());
        }
    }
    if(pJson.isMember("create_time"))
    {
        dirtyFlag_[2] = true;
        if(!pJson["create_time"].isNull())
        {
            auto timeStr = pJson["create_time"].asString();
            struct tm stm;
            memset(&stm,0,sizeof(stm));
            auto p = strptime(timeStr.c_str(),"%Y-%m-%d %H:%M:%S",&stm);
            time_t t = mktime(&stm);
            size_t decimalNum = 0;
            if(p)
            {
                if(*p=='.')
                {
                    std::string decimals(p+1,&timeStr[timeStr.length()]);
                    while(decimals.length()<6)
                    {
                        decimals += "0";
                    }
                    decimalNum = (size_t)atol(decimals.c_str());
                }
                createTime_=std::make_shared<::trantor::Date>(t*1000000+decimalNum);
            }
        }
    }
}

const std::string &Sessions::getValueOfToken() const noexcept
{
    const static std::string defaultValue = std::string();
    if(token_)
        return *token_;
    return defaultValue;
}
const std::shared_ptr<std::string> &Sessions::getToken() const noexcept
{
    return token_;
}
void Sessions::setToken(const std::string &pToken) noexcept
{
    token_ = std::make_shared<std::string>(pToken);
    dirtyFlag_[0] = true;
}
void Sessions::setToken(std::string &&pToken) noexcept
{
    token_ = std::make_shared<std::string>(std::move(pToken));
    dirtyFlag_[0] = true;
}
const typename Sessions::PrimaryKeyType & Sessions::getPrimaryKey() const
{
    assert(token_);
    return *token_;
}

const int32_t &Sessions::getValueOfUserId() const noexcept
{
    const static int32_t defaultValue = int32_t();
    if(userId_)
        return *userId_;
    return defaultValue;
}
const std::shared_ptr<int32_t> &Sessions::getUserId() const noexcept
{
    return userId_;
}
void Sessions::setUserId(const int32_t &pUserId) noexcept
{
    userId_ = std::make_shared<int32_t>(pUserId);
    dirtyFlag_[1] = true;
}

const ::trantor::Date &Sessions::getValueOfCreateTime() const noexcept
{
    const static ::trantor::Date defaultValue = ::trantor::Date();
    if(createTime_)
        return *createTime_;
    return defaultValue;
}
const std::shared_ptr<::trantor::Date> &Sessions::getCreateTime() const noexcept
{
    return createTime_;
}
void Sessions::setCreateTime(const ::trantor::Date &pCreateTime) noexcept
{
    createTime_ = std::make_shared<::trantor::Date>(pCreateTime);
    dirtyFlag_[2] = true;
}

void Sessions::updateId(const uint64_t id)
{
}

const std::vector<std::string> &Sessions::insertColumns() noexcept
{
    static const std::vector<std::string> inCols={
        "token",
        "user_id",
        "create_time"
    };
    return inCols;
}

void Sessions::outputArgs(drogon::orm::internal::SqlBinder &binder) const
{
    if(dirtyFlag_[0])
    {
        if(getToken())
        {
            binder << getValueOfToken();
        }
        else
        {
            binder << nullptr;
        }
    }
    if(dirtyFlag_[1])
    {
        if(getUserId())
        {
            binder << getValueOfUserId();
        }
        else
        {
            binder << nullptr;
        }
    }
    if(dirtyFlag_[2])
    {
        if(getCreateTime())
        {
            binder << getValueOfCreateTime();
        }
        else
        {
            binder << nullptr;
        }
    }
}

const std::vector<std::string> Sessions::updateColumns() const
{
    std::vector<std::string> ret;
    if(dirtyFlag_[0])
    {
        ret.push_back(getColumnName(0));
    }
    if(dirtyFlag_[1])
    {
        ret.push_back(getColumnName(1));
    }
    if(dirtyFlag_[2])
    {
        ret.push_back(getColumnName(2));
    }
    return ret;
}

void Sessions::updateArgs(drogon::orm::internal::SqlBinder &binder) const
{
    if(dirtyFlag_[0])
    {
        if(getToken())
        {
            binder << getValueOfToken();
        }
        else
        {
            binder << nullptr;
        }
    }
    if(dirtyFlag_[1])
    {
        if(getUserId())
        {
            binder << getValueOfUserId();
        }
        else
        {
            binder << nullptr;
        }
    }
    if(dirtyFlag_[2])
    {
        if(getCreateTime())
        {
            binder << getValueOfCreateTime();
        }
        else
        {
            binder << nullptr;
        }
    }
}
Json::Value Sessions::toJson() const
{
    Json::Value ret;
    if(getToken())
    {
        ret["token"]=getValueOfToken();
    }
    else
    {
        ret["token"]=Json::Value();
    }
    if(getUserId())
    {
        ret["user_id"]=getValueOfUserId();
    }
    else
    {
        ret["user_id"]=Json::Value();
    }
    if(getCreateTime())
    {
        ret["create_time"]=getCreateTime()->toDbStringLocal();
    }
    else
    {
        ret["create_time"]=Json::Value();
    }
    return ret;
}

Json::Value Sessions::toMasqueradedJson(
    const std::vector<std::string> &pMasqueradingVector) const
{
    Json::Value ret;
    if(pMasqueradingVector.size() == 3)
    {
        if(!pMasqueradingVector[0].empty())
        {
            if(getToken())
            {
                ret[pMasqueradingVector[0]]=getValueOfToken();
            }
            else
            {
                ret[pMasqueradingVector[0]]=Json::Value();
            }
        }
        if(!pMasqueradingVector[1].empty())
        {
            if(getUserId())
            {
                ret[pMasqueradingVector[1]]=getValueOfUserId();
            }
            else
            {
                ret[pMasqueradingVector[1]]=Json::Value();
            }
        }
        if(!pMasqueradingVector[2].empty())
        {
            if(getCreateTime())
            {
                ret[pMasqueradingVector[2]]=getCreateTime()->toDbStringLocal();
            }
            else
            {
                ret[pMasqueradingVector[2]]=Json::Value();
            }
        }
        return ret;
    }
    LOG_ERROR << "Masquerade failed";
    if(getToken())
    {
        ret["token"]=getValueOfToken();
    }
    else
    {
        ret["token"]=Json::Value();
    }
    if(getUserId())
    {
        ret["user_id"]=getValueOfUserId();
    }
    else
    {
        ret["user_id"]=Json::Value();
    }
    if(getCreateTime())
    {
        ret["create_time"]=getCreateTime()->toDbStringLocal();
    }
    else
    {
        ret["create_time"]=Json::Value();
    }
    return ret;
}

bool Sessions::validateJsonForCreation(const Json::Value &pJson, std::string &err)
{
    if(pJson.isMember("token"))
    {
        if(!validJsonOfField(0, "token", pJson["token"], err, true))
            return false;
    }
    else
    {
        err="The token column cannot be null";
        return false;
    }
    if(pJson.isMember("user_id"))
    {
        if(!validJsonOfField(1, "user_id", pJson["user_id"], err, true))
            return false;
    }
    else
    {
        err="The user_id column cannot be null";
        return false;
    }
    if(pJson.isMember("create_time"))
    {
        if(!validJsonOfField(2, "create_time", pJson["create_time"], err, true))
            return false;
    }
    return true;
}
bool Sessions::validateMasqueradedJsonForCreation(const Json::Value &pJson,
                                                  const std::vector<std::string> &pMasqueradingVector,
                                                  std::string &err)
{
    if(pMasqueradingVector.size() != 3)
    {
        err = "Bad masquerading vector";
        return false;
    }
    try {
      if(!pMasqueradingVector[0].empty())
      {
          if(pJson.isMember(pMasqueradingVector[0]))
          {
              if(!validJsonOfField(0, pMasqueradingVector[0], pJson[pMasqueradingVector[0]], err, true))
                  return false;
          }
        else
        {
            err="The " + pMasqueradingVector[0] + " column cannot be null";
            return false;
        }
      }
      if(!pMasqueradingVector[1].empty())
      {
          if(pJson.isMember(pMasqueradingVector[1]))
          {
              if(!validJsonOfField(1, pMasqueradingVector[1], pJson[pMasqueradingVector[1]], err, true))
                  return false;
          }
        else
        {
            err="The " + pMasqueradingVector[1] + " column cannot be null";
            return false;
        }
      }
      if(!pMasqueradingVector[2].empty())
      {
          if(pJson.isMember(pMasqueradingVector[2]))
          {
              if(!validJsonOfField(2, pMasqueradingVector[2], pJson[pMasqueradingVector[2]], err, true))
                  return false;
          }
      }
    }
    catch(const Json::LogicError &e)
    {
      err = e.what();
      return false;
    }
    return true;
}
bool Sessions::validateJsonForUpdate(const Json::Value &pJson, std::string &err)
{
    if(pJson.isMember("token"))
    {
        if(!validJsonOfField(0, "token", pJson["token"], err, false))
            return false;
    }
    else
    {
        err = "The value of primary key must be set in the json object for update";
        return false;
    }
    if(pJson.isMember("user_id"))
    {
        if(!validJsonOfField(1, "user_id", pJson["user_id"], err, false))
            return false;
    }
    if(pJson.isMember("create_time"))
    {
        if(!validJsonOfField(2, "create_time", pJson["create_time"], err, false))
            return false;
    }
    return true;
}
bool Sessions::validateMasqueradedJsonForUpdate(const Json::Value &pJson,
                                                const std::vector<std::string> &pMasqueradingVector,
                                                std::string &err)
{
    if(pMasqueradingVector.size() != 3)
    {
        err = "Bad masquerading vector";
        return false;
    }
    try {
      if(!pMasqueradingVector[0].empty() && pJson.isMember(pMasqueradingVector[0]))
      {
          if(!validJsonOfField(0, pMasqueradingVector[0], pJson[pMasqueradingVector[0]], err, false))
              return false;
      }
    else
    {
        err = "The value of primary key must be set in the json object for update";
        return false;
    }
      if(!pMasqueradingVector[1].empty() && pJson.isMember(pMasqueradingVector[1]))
      {
          if(!validJsonOfField(1, pMasqueradingVector[1], pJson[pMasqueradingVector[1]], err, false))
              return false;
      }
      if(!pMasqueradingVector[2].empty() && pJson.isMember(pMasqueradingVector[2]))
      {
          if(!validJsonOfField(2, pMasqueradingVector[2], pJson[pMasqueradingVector[2]], err, false))
              return false;
      }
    }
    catch(const Json::LogicError &e)
    {
      err = e.what();
      return false;
    }
    return true;
}
bool Sessions::validJsonOfField(size_t index,
                                const std::string &fieldName,
                                const Json::Value &pJson,
                                std::string &err,
                                bool isForCreation)
{
    switch(index)
    {
        case 0:
            if(pJson.isNull())
            {
                err="The " + fieldName + " column cannot be null";
                return false;
            }
            if(!pJson.isString())
            {
                err="Type error in the "+fieldName+" field";
                return false;
            }
            break;
        case 1:
            if(pJson.isNull())
            {
                err="The " + fieldName + " column cannot be null";
                return false;
            }
            if(!pJson.isInt())
            {
                err="Type error in the "+fieldName+" field";
                return false;
            }
            break;
        case 2:
            if(pJson.isNull())
            {
                err="The " + fieldName + " column cannot be null";
                return false;
            }
            if(!pJson.isString())
            {
                err="Type error in the "+fieldName+" field";
                return false;
            }
            break;
        default:
            err="Internal error in the server";
            return false;
    }
    return true;
}
void Sessions::getUser(const DbClientPtr &clientPtr,
                       const std::function<void(Users)> &rcb,
                       const ExceptionCallback &ecb) const
{
    const static std::string sql = "select * from users where id = $1";
    *clientPtr << sql
               << *userId_
               >> [rcb = std::move(rcb), ecb](const Result &r){
                    if (r.size() == 0)
                    {
                        ecb(UnexpectedRows("0 rows found"));
                    }
                    else if (r.size() > 1)
                    {
                        ecb(UnexpectedRows("Found more than one row"));
                    }
                    else
                    {
                        rcb(Users(r[0]));
                    }
               }
               >> ecb;
}
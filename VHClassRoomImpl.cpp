#include "VHClassRoomImpl.h"
#include <mutex>
//#include "vhall_log.h"
#include "./Utility/vhall_classSdk_log.h"

#include <string>
#include <sstream>
#include <stdio.h>    
#include <iostream> 
#include "src/sio_client.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <strstream>
#include <time.h>
#include "SystemUtility.h"
#include "Md5.h"
//#include "VHPaasRoomImpl.h"
#include <QJsonObject> 
#include <QJsonDocument>
#include <QByteArray>
#include <QString>
#include <QJsonValue> 
#include "./PaasSDK/vh_room_msg.h"

using namespace std;

#define RESP_SUCCESS_CODE 200
#define SUCESS_MSG  "success"

static std::mutex gSDKMutex;
static VHClassRoomImpl* gClassSDKInstance = nullptr;

HANDLE VHClassRoomImpl::mStatic_TaskEvent;   //
HANDLE VHClassRoomImpl::mStatic_ThreadExitEvent;


VHClassRoomImpl::VHClassRoomImpl() {
   mIsHost = true;
   bIsSettinglayout = false;
}

VHClassRoomImpl::~VHClassRoomImpl() {
   LOGD("VHClassRoomImpl::~VHClassRoomImpl");
   if (mpHttpManagerInterface) {
      //mpHttpManagerInterface ;
      //DestoryHttpManagerInstance();
      mpHttpManagerInterface = nullptr;
   }
   if (mpWebRtcSDKInterface) {
      delete mpWebRtcSDKInterface;
      mpWebRtcSDKInterface = nullptr;
   }

   if (mpPaasInteraction) {
      DestoryPaasSDKInstance();
      //delete mpPaasInteraction;
      //mpPaasInteraction = nullptr;
   }

   if (mpVHWebSocketInterface) {
      mpVHWebSocketInterface->DisConnectWebSocket();
	   LOGD(" mpVHWebSocketInterface->DisConnectWebSocket");
      mbDisConnectWebSocket = true;
	  LOGD(" mpVHWebSocketInterface->SyncDisConnectServer");
      mpVHWebSocketInterface->SyncDisConnectServer();
	  LOGD(" mpVHWebSocketInterface->SyncDisConnectServer2");
      delete mpVHWebSocketInterface;
      mpVHWebSocketInterface = nullptr;
   }
}

void VHClassRoomImpl::OnOpen() {
   LOGD("OnOpen");
}

void VHClassRoomImpl::OnFail() {
   LOGD("OnFail");
   if (mpVHRoomMonitor) {
      mpVHRoomMonitor->OnJoinClassRoom(4000, "network open fail");
   }
}

void VHClassRoomImpl::OnReconnecting() {
   LOGD("OnReconnecting");
   if (mpVHRoomMonitor) {
      mpVHRoomMonitor->OnCallbackEvent(RoomEvent_OnNetWork_Reconnecting, 4001, "network reconneting");
   }
}

void VHClassRoomImpl::OnReconnect(unsigned, unsigned) {
   LOGD("OnReconnect");
   if (mpVHRoomMonitor) {
      mpVHRoomMonitor->OnCallbackEvent(RoomEvent_OnNetWork_Reconnect);
   }
}
void VHClassRoomImpl::OnClose() {
   LOGD("OnClose");
   //不是主动断开的，要重连
   if (mpVHRoomMonitor && !mbDisConnectWebSocket) {
      mpVHRoomMonitor->OnCallbackEvent(RoomEvent_OnNetWork_Close);
   }
}

void VHClassRoomImpl::OnSocketOpen(std::string const& nsp) {
   LOGD("OnSocketOpen msg:%s", nsp.c_str());
   if (mpVHRoomMonitor) {
      mpVHRoomMonitor->OnJoinClassRoom(RESP_SUCCESS_CODE, SUCESS_MSG);
   };
}

void VHClassRoomImpl::OnSocketClose(std::string const& nsp) {
   LOGD("OnSocketClose  nsp:%s", nsp.c_str());
}

void VHClassRoomImpl::OnServiceMsg(std::string type, const std::wstring user_id, const std::wstring nickname, const std::string rolename, bool is_banned, int devType, int uv /*= 0*/) {
   //if (mpVHRoomMonitor) {
   //    mpVHRoomMonitor->OnUserOnLineState(online, user_id);
   //}
}

void VHClassRoomImpl::OnRecvAllMsg(const char*, int length) {

}

void VHClassRoomImpl::InitNetWorkProtocol(std::string app_key, std::string app_secret, const std::string& domain, VHRoomMonitor* monitor, std::wstring logPath) {
   InitLog(logPath);
   LOGD("InitSDK PAASSDK_VER : %s", CLASS_SSDK_VER);
   mpVHRoomMonitor = monitor;
   mlogPath = logPath;
   if (mpHttpManagerInterface == nullptr) {
      mpHttpManagerInterface = GetHttpManagerInstance();
      LOGD("new HttpManager");
      if (mpHttpManagerInterface) {
         LOGD("Init HttpManager");
         mpHttpManagerInterface->Init(logPath);
         mLogReport.RegisterHttpSender(mpHttpManagerInterface);
         //UploadReportSDKInit();
      }
   }

   if (mpVHWebSocketInterface == nullptr) {
      mpVHWebSocketInterface = CreateVHWebSocket();
      LOGD("CreateVHWebSocket");
      mpVHWebSocketInterface->RegisterCallback(this);
   }

   mAappKey = app_key;
   mAppSecret = app_secret;
   char lpszCpu[128] = { 0 };
   SystemUtility::GetBaseBoardByCmd(lpszCpu, 128);
   mDevUniqueId = std::string(lpszCpu);
   if (mDevUniqueId.length() == 0) {
      mDevUniqueId = std::to_string(rand());
   }
   mURLGenerator.SetDoMain(domain);
   mURLGenerator.SetPublicParam(mAappKey, mAppSecret, mDevUniqueId);
   LOGD("end");
}

void VHClassRoomImpl::InitSDK(const int & iPlayType)
{
   miPlayType = iPlayType;
   LOGD(" Enter PlayType %d （0 Flash 1 H5）", miPlayType);

   //miPlayType = eClassPlayType_H5;
   if (eClassPlayType_Flash == miPlayType)
   {
      LOGD(" Enter Class SDK", miPlayType);
      if (mpWebRtcSDKInterface == nullptr) {
         mpWebRtcSDKInterface = GetWebRtcSDKInstance();
         LOGD("GetWebRtcSDKInstance");
      }
      if (mpWebRtcSDKInterface) {
         LOGD("RegisterCallBackObj");
         mpWebRtcSDKInterface->InitSDK(mpVHRoomMonitor, mlogPath);
      }
   }
   else if (eClassPlayType_H5 == miPlayType)
   {
      LOGD(" Enter Paas SDK  JoinPaasRoom", miPlayType);
      if (mpPaasInteraction == nullptr) {
         mpPaasInteraction = GetPaasSDKInstance();
      }

      if (nullptr != mpPaasInteraction) {
         LOGD("RegisterCallBackObj");
         mpPaasInteraction->InitSDK(mpVHRoomMonitor, mpVHRoomMonitor, mpVHRoomMonitor, mlogPath);
      }
      JoinPaasRoom();
   }

}

void VHClassRoomImpl::InitWebRtcSDK() {
	if (mpWebRtcSDKInterface == nullptr) {
		mpWebRtcSDKInterface = GetWebRtcSDKInstance();
		LOGD("GetWebRtcSDKInstance");
	}
	if (mpWebRtcSDKInterface) {
		LOGD("RegisterCallBackObj");
		mpWebRtcSDKInterface->InitSDK(mpVHRoomMonitor, mlogPath);
	}
}

void VHClassRoomImpl::SetDebugLogAddr(const std::string& addr) {
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      LOGD("  Class SDK SetDebugLogAddr", miPlayType);
      mpWebRtcSDKInterface->SetDebugLogAddr(addr);
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction)
   {
      LOGD("  Paas SDK SetDebugLogAddr", miPlayType);
      mpPaasInteraction->SetDebugLogAddr(addr);
   }
}

std::string VHClassRoomImpl::GetDevId() {
   return mDevUniqueId;
}

double VHClassRoomImpl::GetPushDesktopVideoLostRate()
{
   double dRef = 0.0;

   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      
      dRef = mpWebRtcSDKInterface->GetPushDesktopVideoLostRate();
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction)
   {
      
      dRef = mpPaasInteraction->GetPushDesktopVideoLostRate();
   }
   return dRef;
}

bool VHClassRoomImpl::isConnetWebRtcRoom()
{
   bool bRef = false;

   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      LOGD("  Class SDK SetDebugLogAddr", miPlayType);
      bRef = mpWebRtcSDKInterface->isConnetWebRtcRoom();
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction)
   {
      LOGD("  Paas SDK SetDebugLogAddr", miPlayType);
      bRef = mpPaasInteraction->isConnetWebRtcRoom();
   }
return bRef;
}

void VHClassRoomImpl::GetWebinarDetailInfo(const std::string& webinar_id) {
   mClassRoomBaseInfo.Reset();
   LOGD("GetWebinarDetailInfo webinar_id:%s", webinar_id.c_str());
   //mWebinarId = webinar_id;
   HTTP_GET_REQUEST httpGet(mURLGenerator.GetWebinarDetailInfoUrl(webinar_id));
   if (mpHttpManagerInterface) {
      mpHttpManagerInterface->HttpGetRequest(httpGet, [&](std::string data, int code, const std::string userData)->void {
         LOGD("data:%s code:%d", data.c_str(), code);
         AnalysisGetWebinarDetailInfo(data, code);
      });
   }
   LOGD("end");
}

void VHClassRoomImpl::AnalysisGetWebinarDetailInfo(std::string data, int code) {
   if (code != 0) {
      if (mpVHRoomMonitor) {
         LOGD("data:%s code:%d", data.c_str(), code);
         mpVHRoomMonitor->OnGetWebinarDetailInfo(code, data);
      }
   }
   else {
      //解析课堂基本数据
      ParamToGetWebinarDetailInfoResp(data);
   }
   LOGD("end");
}


void VHClassRoomImpl::ParamToGetWebinarDetailInfoResp(const std::string& msg) {
   LOGD("msg:%s", msg.c_str());
   int nCode = RESP_SUCCESS_CODE;
   std::string strNoticeMsg;
   rapidjson::Document doc;
   doc.Parse<0>(msg.c_str());
   if (doc.HasParseError()) {
      LOGE("VHTokenRespMsg ParseError%d\n", doc.GetParseError());
      return;
   }

   if (doc.IsObject()) {
      if (doc.HasMember("code") && doc["code"].IsInt()) {
         nCode = doc["code"].GetInt();
      }
      if (doc.HasMember("msg") && doc["msg"].IsString()) {
         strNoticeMsg = doc["msg"].GetString();
      }
      if (RESP_SUCCESS_CODE == nCode) {
         if (doc.HasMember("data") && doc["data"].IsObject()) {
            rapidjson::Value& resp = doc["data"];
            if (resp.HasMember("webinar_id") && resp["webinar_id"].IsString()) {
               mCourseInfo.webinar_id = resp["webinar_id"].GetString();
            }
            if (resp.HasMember("subject") && resp["subject"].IsString()) {
               mCourseInfo.webinar_subject = resp["subject"].GetString();
            }
            if (resp.HasMember("course_type") && resp["course_type"].IsInt()) {
               mCourseInfo.webinar_course_type = (CLASS_COURSE_TYPE)resp["course_type"].GetInt();
            }
            if (resp.HasMember("type") && resp["type"].IsInt()) {
               mCourseInfo.webinar_status = (CLASS_STATUS)resp["type"].GetInt();
            }
            if (resp.HasMember("start_time") && resp["start_time"].IsString()) {
               mCourseInfo.start_time = resp["start_time"].GetString();
            }
            if (resp.HasMember("end_time") && resp["end_time"].IsString()) {
               mCourseInfo.end_time = resp["end_time"].GetString();
            }
            if (resp.HasMember("duration") && resp["duration"].IsInt()) {
               mCourseInfo.duration = resp["duration"].GetInt();
            }
            if (resp.HasMember("desc") && resp["desc"].IsString()) {
               mCourseInfo.desc = resp["desc"].GetString();
            }
            if (resp.HasMember("logo") && resp["desc"].IsString()) {
               mCourseInfo.logo = resp["logo"].GetString();
            }
            if (resp.HasMember("cover_img") && resp["cover_img"].IsString()) {
               mCourseInfo.cover_img = resp["cover_img"].GetString();
            }
            if (resp.HasMember("uid") && resp["uid"].IsString()) {
               mCourseInfo.uid = resp["uid"].GetString();
            }
            if (resp.HasMember("detail") && resp["detail"].IsString()) {
               mCourseInfo.detail = resp["detail"].GetString(); 
            }
			if (resp.HasMember("viewing_conditions") && resp["viewing_conditions"].IsInt()) {
				mCourseInfo.viewing_conditions = resp["viewing_conditions"].GetInt(); 
			}
         }
      }
   }

   if (mpVHRoomMonitor) {
      LOGD(" data:%s code:%d", strNoticeMsg.c_str(), nCode);
      mpVHRoomMonitor->OnGetWebinarDetailInfo(nCode, strNoticeMsg);
   }
}

void VHClassRoomImpl::JoinClassRoom(std::string nickName, std::string password, std::string phone) {
   LOGD("nickName:%s password %s phone %s", nickName.c_str(), password.c_str(),phone.c_str());

   HTTP_GET_REQUEST httpGet(mURLGenerator.GetJoinClassRoomUrl(mCourseInfo.webinar_id, nickName, password, phone));
   if (mpHttpManagerInterface) {
      mpHttpManagerInterface->HttpGetRequest(httpGet, [&](std::string data, int code, const std::string userData)->void {
         LOGD("Request data:%s code:%d", data.c_str(), code);
         AnalysisJoinClassRoomDetailInfo(data, code);
      });
   }
   LOGD("end");
}

void VHClassRoomImpl::JoinClassRoom(std::string cmd)
{
   LOGD("JoinClassRoom cmd:%s ", cmd.c_str());
   std::string strUrl = mURLGenerator.GetJoinClassRoomUrl(cmd);
   LOGD("JoinClassRoom URL:%s ", strUrl.c_str());
   HTTP_GET_REQUEST httpGet(strUrl);
   if (mpHttpManagerInterface) {
      mpHttpManagerInterface->HttpGetRequest(httpGet, [&](std::string data, int code, const std::string userData)->void {
         LOGD("data:%s code:%d", data.c_str(), code);
         AnalysisJoinClassRoomDetailInfo(data, code);
      });
   }
   LOGD("end");
}

void VHClassRoomImpl::StartCoutDownTool(const int& iTime, const int& iIsPlayEndSounds)
{
   LOGD("%s iTime:%d ", __FUNCTION__, iTime);

   std::string strUrl = mURLGenerator.StartCoutDownToolUrl(mCourseInfo.webinar_id, mClassRoomBaseInfo.mUserInfo.join_id, mClassToken, iTime, iIsPlayEndSounds);
   LOGD("StartCoutDownTool URL:%s ", strUrl.c_str());
   HTTP_GET_REQUEST httpGet(strUrl);
   if (mpHttpManagerInterface) {
      mpHttpManagerInterface->HttpGetRequest(httpGet, [&](std::string data, int code, const std::string userData)->void {
         LOGD("data:%s code:%d", data.c_str(), code);
         //AnalysisJoinClassRoomDetailInfo(data, code);
      });
   }

   LOGD("%s end ", __FUNCTION__);
}

void VHClassRoomImpl::CancleCoutDownTool()
{
   LOGD("%s  ", __FUNCTION__);

   std::string strUrl = mURLGenerator.CancleCoutDownToolUrl(mCourseInfo.webinar_id, mClassRoomBaseInfo.mUserInfo.join_id, mClassToken);
   LOGD("CancleCoutDownTool URL:%s ", strUrl.c_str());
   HTTP_GET_REQUEST httpGet(strUrl);
   if (mpHttpManagerInterface) {
      mpHttpManagerInterface->HttpGetRequest(httpGet, [&](std::string data, int code, const std::string userData)->void {
         LOGD("data:%s code:%d", data.c_str(), code);
         //AnalysisJoinClassRoomDetailInfo(data, code);
      });
   }
   LOGD("%s end ", __FUNCTION__);
}

void VHClassRoomImpl::AddSign()
{
   LOGD("%s", __FUNCTION__);
   HTTP_GET_REQUEST httpGet(mURLGenerator.GetAddSign(mClassRoomBaseInfo.mWebinarInfo.id, mClassRoomBaseInfo.mUserInfo.join_id, mClassToken));
   httpGet.SetHttpPost(true);
   if (mpHttpManagerInterface) {
      mpHttpManagerInterface->HttpGetRequest(httpGet, [&](std::string data, int code, const std::string userData)->void {
         LOGD("data:%s code:%d", data.c_str(), code);
         //AnalysisJoinClassRoomDetailInfo(data, code);
      });
   }
   LOGD("%s end ", __FUNCTION__);
}

void VHClassRoomImpl::SignIn(const std::string& strAddSignId)
{
   LOGD("%s", __FUNCTION__);
   HTTP_GET_REQUEST httpGet(mURLGenerator.GetSignIn(mClassRoomBaseInfo.mWebinarInfo.id, mClassRoomBaseInfo.mUserInfo.join_id, mClassToken, strAddSignId));
   httpGet.SetHttpPost(true);
   if (mpHttpManagerInterface) {
      mpHttpManagerInterface->HttpGetRequest(httpGet, [&](std::string data, int code, const std::string userData)->void {
         LOGD("data:%s code:%d", data.c_str(), code);
         //AnalysisJoinClassRoomDetailInfo(data, code);
         //RESP_SUCCESS_CODE 返回值 code
      });
   }
   LOGD("%s end ", __FUNCTION__);
}

void VHClassRoomImpl::GetSignRecord(const std::string& strAddSignId)
{
   LOGD("%s", __FUNCTION__);
   HTTP_GET_REQUEST httpGet(mURLGenerator.GetSignRecord(mClassRoomBaseInfo.mWebinarInfo.id, mClassRoomBaseInfo.mUserInfo.join_id, mClassToken, strAddSignId));
   httpGet.SetHttpPost(true);
   if (mpHttpManagerInterface) {
      mpHttpManagerInterface->HttpGetRequest(httpGet, [&](std::string data, int code, const std::string userData)->void {
         LOGD("data:%s code:%d", data.c_str(), code);
         AnalysisSignRecord(data, code);
      });
   }
   LOGD("%s end ", __FUNCTION__);
}


void VHClassRoomImpl::SignStop(const std::string& strAddSignId)
{
   LOGD("%s", __FUNCTION__);
   HTTP_GET_REQUEST httpGet(mURLGenerator.GetSignStop(mClassRoomBaseInfo.mWebinarInfo.id, mClassRoomBaseInfo.mUserInfo.join_id, mClassToken, strAddSignId));
   httpGet.SetHttpPost(true);
   if (mpHttpManagerInterface) {
      mpHttpManagerInterface->HttpGetRequest(httpGet, [&](std::string data, int code, const std::string userData)->void {
         LOGD("data:%s code:%d", data.c_str(), code);
         //AnalysisSignRecord(data, code);
      });
   }
   LOGD("%s end ", __FUNCTION__);
}

void VHClassRoomImpl::StartAnswer(const std::string& strContent) {
   LOGD("%s", __FUNCTION__);
   HTTP_GET_REQUEST httpGet(mURLGenerator.GetStartAnswer(mClassRoomBaseInfo.mWebinarInfo.id, mClassRoomBaseInfo.mUserInfo.join_id, mClassToken, strContent));
   httpGet.SetHttpPost(true);
   if (mpHttpManagerInterface) {
      mpHttpManagerInterface->HttpGetRequest(httpGet, [&](std::string data, int code, const std::string userData)->void {
         LOGD("data:%s code:%d", data.c_str(), code);
			AnalysisStartAnswer(data.c_str(), code);
      });
   }
   LOGD("%s end ", __FUNCTION__);
}

void VHClassRoomImpl::AnswerIn(const std::string& strAnswer, const std::string& strQuesId) {
   LOGD("%s", __FUNCTION__);
   HTTP_GET_REQUEST httpGet(mURLGenerator.GetAnswerIn(mClassRoomBaseInfo.mWebinarInfo.id, mClassRoomBaseInfo.mUserInfo.join_id, mClassToken, strAnswer, strQuesId));
   httpGet.SetHttpPost(true);
   if (mpHttpManagerInterface) {
      mpHttpManagerInterface->HttpGetRequest(httpGet, [&](std::string data, int code, const std::string userData)->void {
         LOGD("data:%s code:%d", data.c_str(), code);
         //AnalysisSignRecord(data, code);
      });
   }
   LOGD("%s end ", __FUNCTION__);
}

void VHClassRoomImpl::AnalysisJoinClassRoomDetailInfo(std::string data, int code) {
   if (code != 0) {
      if (mpVHRoomMonitor) {
         LOGD("data:%s code:%d", data.c_str(), code);
         mpVHRoomMonitor->OnJoinClassRoom(code, data);
      }
   }
   else {
      ParamToJoinClassRoomDetailInfoResp(data);
   }
}

void VHClassRoomImpl::AnalysisJoinPaasRoom(std::string data, int code)
{
   if (code != 0) {
      if (mpVHRoomMonitor) {
         LOGD("data:%s code:%d", data.c_str(), code);
         mpVHRoomMonitor->OnJoinClassRoom(code, data);
      }
   }
   else {
      //ParamToJoinClassRoomDetailInfoResp(data);
      ParamToJoinPaasRoom(data);
   }
}

void VHClassRoomImpl::AnalysisSignRecord(std::string data, int code) {
   if (code != 0) {
      if (mpVHRoomMonitor) {
         LOGD("data:%s code:%d", data.c_str(), code);
         //mpVHRoomMonitor->OnSignRecord(code, data);
      }
   }
   else {
      ParamToOnSignRecordResp(data);
   }
}

void VHClassRoomImpl::AnalysisAnswerDetail(std::string data, int code) {
	if (code != 0) {
		if (mpVHRoomMonitor) {
			LOGD("data:%s code:%d", data.c_str(), code);
			mpVHRoomMonitor->OnRecvAnswerDetailMsg(code, data);
		}
	}
	else {
		ParamToAnswerDetail(data);
	}
}

void VHClassRoomImpl::AnalysisStartAnswer(std::string data, int code) {
	if (code != 0) {
		if (mpVHRoomMonitor) {
			LOGD("data:%s code:%d", data.c_str(), code);
			mpVHRoomMonitor->OnRecvStartAnswerMsg(code, data);
		}
	}
	else {
		ParamToOnAnswerStart(data);
	}
}

void VHClassRoomImpl::AnnounceAnswer(const std::string& strQuesId) {
	LOGD("%s", __FUNCTION__);
	HTTP_GET_REQUEST httpGet(mURLGenerator.GetAnnounceAnswer(mClassRoomBaseInfo.mWebinarInfo.id, mClassRoomBaseInfo.mUserInfo.join_id, mClassToken, strQuesId));
	httpGet.SetHttpPost(true);
	if (mpHttpManagerInterface) {
		mpHttpManagerInterface->HttpGetRequest(httpGet, [&](std::string data, int code, const std::string userData)->void {
			LOGD("data:%s code:%d", data.c_str(), code);
			//AnalysisSignRecord(data, code);
		});
	}
	LOGD("%s end ", __FUNCTION__);
}

void VHClassRoomImpl::StopAnswer(const std::string& strQuesId) {
	LOGD("%s", __FUNCTION__);
	HTTP_GET_REQUEST httpGet(mURLGenerator.GetStopAnswer(mClassRoomBaseInfo.mWebinarInfo.id, mClassRoomBaseInfo.mUserInfo.join_id, mClassToken, strQuesId));
	httpGet.SetHttpPost(true);
	if (mpHttpManagerInterface) {
		mpHttpManagerInterface->HttpGetRequest(httpGet, [&](std::string data, int code, const std::string userData)->void {
			LOGD("data:%s code:%d", data.c_str(), code);
			//AnalysisSignRecord(data, code);
		});
	}
	LOGD("%s end ", __FUNCTION__);
}

void VHClassRoomImpl::AnswerDetail(const std::string& strQuesId) {
	LOGD("%s", __FUNCTION__);
	HTTP_GET_REQUEST httpGet(mURLGenerator.GetAnswerDetail(mClassRoomBaseInfo.mWebinarInfo.id, mClassRoomBaseInfo.mUserInfo.join_id, mClassToken, strQuesId));
	httpGet.SetHttpPost(true);
	if (mpHttpManagerInterface) {
		mpHttpManagerInterface->HttpGetRequest(httpGet, [&](std::string data, int code, const std::string userData)->void {
			LOGD("data:%s code:%d", data.c_str(), code);
			AnalysisAnswerDetail(data, code);
		});
	}
	LOGD("%s end ", __FUNCTION__);
}



void VHClassRoomImpl::ParamToJoinClassRoomDetailInfoResp(const std::string& msg) {
   LOGD("%s msg:%s", __FUNCTION__, msg.c_str());
   int nCode = RESP_SUCCESS_CODE;
   std::string strNoticeMsg;
   rapidjson::Document doc;
   doc.Parse<0>(msg.c_str());
   if (doc.HasParseError()) {
      LOGE("%s ParseError%d\n", __FUNCTION__, doc.GetParseError());
      return;
   }

   int iCurLabelNumber = 1;
   if (doc.IsObject()) {
      if (doc.HasMember("code") && doc["code"].IsInt()) {
         nCode = doc["code"].GetInt();
      }
      if (doc.HasMember("msg") && doc["msg"].IsString()) {
         strNoticeMsg = doc["msg"].GetString();
      }
      if (RESP_SUCCESS_CODE == nCode) {
         if (doc.HasMember("data") && doc["data"].IsObject()) {
            rapidjson::Value& resp = doc["data"];
            if (resp.HasMember("webinar") && resp.IsObject()) {
               rapidjson::Value& webinar = resp["webinar"];
               if (webinar.HasMember("id") && webinar["id"].IsString()) {
                  mClassRoomBaseInfo.mWebinarInfo.id = webinar["id"].GetString();
                  mCourseInfo.webinar_id = mClassRoomBaseInfo.mWebinarInfo.id;
               }
               if (webinar.HasMember("subject") && webinar["subject"].IsString()) {
                  mCourseInfo.webinar_subject = mClassRoomBaseInfo.mWebinarInfo.subject = webinar["subject"].GetString();
               }
               if (webinar.HasMember("user_id") && webinar["user_id"].IsString()) {
                  mCourseInfo.uid = mClassRoomBaseInfo.mWebinarInfo.user_id = webinar["user_id"].GetString();
               }
               if (webinar.HasMember("course_type") && webinar["course_type"].IsInt()) {
                  mClassRoomBaseInfo.mWebinarInfo.course_type = webinar["course_type"].GetInt();
                  mCourseInfo.webinar_course_type = (CLASS_COURSE_TYPE)mClassRoomBaseInfo.mWebinarInfo.course_type;
               }
               if (webinar.HasMember("logo") && webinar["logo"].IsString()) {
                  mCourseInfo.logo = webinar["logo"].GetString();
               }
               if (webinar.HasMember("start_time") && webinar["start_time"].IsString()) {
                  mCourseInfo.start_time = webinar["start_time"].GetString();
               }
               if (webinar.HasMember("end_time") && webinar["end_time"].IsString()) {
                  mCourseInfo.end_time = webinar["end_time"].GetString();
               }
               if (webinar.HasMember("type") && webinar["type"].IsInt()) {
                  int iType = webinar["type"].GetInt();
                  mClassRoomBaseInfo.mWebinarInfo.type = iType;
                  LOGD("%s mWebinarInfo.type:%d", __FUNCTION__, iType);
                  mCourseInfo.webinar_status = (CLASS_STATUS)(iType);
               }

               if (webinar.HasMember("is_broadcast_now")) {
                  if (webinar["is_broadcast_now"].IsInt()) {
                     int iType = webinar["is_broadcast_now"].GetInt();
                     mClassRoomBaseInfo.mWebinarInfo.bRrlay = iType==1;
                     LOGD("%s mWebinarInfo.bRrlay:%d", __FUNCTION__, iType);
                  }
                  else if (webinar["is_broadcast_now"].IsString()) {
                     std::string strNum = doc["playEndSound"].GetString();
                     int iType  =atoi(strNum.c_str());
                     mClassRoomBaseInfo.mWebinarInfo.bRrlay = iType == 1;
                     LOGD("%s mWebinarInfo.bRrlay:%d", __FUNCTION__, iType);
                  }
               } 

               if (webinar.HasMember("real_duration") && webinar["real_duration"].IsInt()) {
                  mClassRoomBaseInfo.mWebinarInfo.real_duration = webinar["real_duration"].GetInt();
               }
               if (mClassRoomBaseInfo.mWebinarInfo.type != CLASS_STATUS_IN_CLASS) {
                  mClassRoomBaseInfo.mWebinarInfo.real_duration = 0;
               }
               if (webinar.HasMember("publishUrl") && webinar["publishUrl"].IsString()) {//原来  
                  mPublishUrl = webinar["publishUrl"].GetString();
               }
               if (webinar.HasMember("layout")) {
                  if(webinar["layout"].IsInt()){
                     mClassRoomBaseInfo.mWebinarInfo.layout = webinar["layout"].GetInt();
                  }
                  else{
                     mClassRoomBaseInfo.mWebinarInfo.layout = atoi(webinar["layout"].GetString());
                  }
               }
               if (webinar.HasMember("reportUrl") && webinar["reportUrl"].IsString()) {// 替换  
                  mReportUrl = webinar["reportUrl"].GetString();
               }
               if (webinar.HasMember("introduction") && webinar["introduction"].IsString()) {
                  mClassRoomBaseInfo.mWebinarInfo.introduction = webinar["introduction"].GetString();
                  mCourseInfo.desc = mClassRoomBaseInfo.mWebinarInfo.introduction;
               }
               if (webinar.HasMember("answing_question_info") && webinar["answing_question_info"].IsObject()) {
						rapidjson::Value& answing_question_info = webinar["answing_question_info"];
						if (answing_question_info.HasMember("answing_question_id") && answing_question_info["answing_question_id"].IsInt()) {
							mClassRoomBaseInfo.mWebinarInfo.answing_question_id = webinar["answing_question_id"].GetInt();
						}
						if (answing_question_info.HasMember("bu") && answing_question_info["bu"].IsString()) {
								std::string bu = answing_question_info["bu"].GetString();
								if (bu == "answercard") {
									mClassRoomBaseInfo.mWebinarInfo.answing_question_id = -1;
								}
							}
               }
               if (webinar.HasMember("linkmic_apply_type") && webinar["linkmic_apply_type"].IsInt()) {
                  mClassRoomBaseInfo.mWebinarInfo.linkmic_apply_type = webinar["linkmic_apply_type"].GetInt();
               }
               if (webinar.HasMember("interstitial") && webinar["interstitial"].IsInt()) {
                  mClassRoomBaseInfo.mWebinarInfo.mInterstitialInfo.interstitial = webinar["interstitial"].GetInt();
               }
               if (webinar.HasMember("interstitial_progress_rate") && webinar["interstitial_progress_rate"].IsObject()) {
                  rapidjson::Value& progress = webinar["interstitial_progress_rate"];
                  if (progress.HasMember("record_id") && progress["record_id"].IsString()) {
                     mClassRoomBaseInfo.mWebinarInfo.mInterstitialInfo.record_id = progress["record_id"].GetString();
                  }
                  if (progress.HasMember("progress_rate") && progress["progress_rate"].IsString()) {
                     mClassRoomBaseInfo.mWebinarInfo.mInterstitialInfo.progress_rate = progress["progress_rate"].GetString();
                  }
                  if (progress.HasMember("is_pause") && progress["is_pause"].IsString()) {
                     mClassRoomBaseInfo.mWebinarInfo.mInterstitialInfo.is_pause = progress["is_pause"].GetString();
                  }
               }

               if (webinar.HasMember("user_setting") && webinar["user_setting"].IsObject()) {
                  rapidjson::Value& user_setting = webinar["user_setting"];
                  //if (user_setting.HasMember("VIDEO_PROFILE") && user_setting["VIDEO_PROFILE"].IsString()) {
                  //    std::string VIDEO_PROFILE = user_setting["VIDEO_PROFILE"].GetString();
                  //    mClassRoomBaseInfo.mWebinarInfo.user_setting.video_profile = atoi(VIDEO_PROFILE.c_str());
                  //}
                  //if (user_setting.HasMember("BROADCAST_VIDEO_PROFILE") && user_setting["BROADCAST_VIDEO_PROFILE"].IsString()) {
                  //    std::string  BROADCAST_VIDEO_PROFILE = user_setting["BROADCAST_VIDEO_PROFILE"].GetString();
                  //    mClassRoomBaseInfo.mWebinarInfo.user_setting.broadcast_video_profile = atoi(BROADCAST_VIDEO_PROFILE.c_str());
                  //}
                  if (user_setting.HasMember("MIXFLOW_LAYOUT") && user_setting["MIXFLOW_LAYOUT"].IsString()) {
                     std::string mixlayout = user_setting["MIXFLOW_LAYOUT"].GetString();
                     mClassRoomBaseInfo.mWebinarInfo.videoPushAndMixSetting.share_desktop_mixed_flow_layout = mixlayout == "standard" ? 0 : 1;
                  }
               }

               if (webinar.HasMember("label_number") && webinar["label_number"].IsInt()) {
                  iCurLabelNumber = webinar["label_number"].GetInt();
               }
               else if (webinar.HasMember("label_number") && webinar["label_number"].IsString()){
                  iCurLabelNumber = atoi(webinar["label_number"].GetString());
               }

               if (webinar.HasMember("mixed_flow_layout_mode") && webinar["mixed_flow_layout_mode"].IsInt()) {
                  mClassRoomBaseInfo.mWebinarInfo.mixedLayout = (eMixedLayoutMode)webinar["mixed_flow_layout_mode"].GetInt();
               }
               else if (webinar.HasMember("mixed_flow_layout_mode") && webinar["mixed_flow_layout_mode"].IsString()) {
                  mClassRoomBaseInfo.mWebinarInfo.mixedLayout = (eMixedLayoutMode)atoi(webinar["mixed_flow_layout_mode"].GetString());
               }

			   if (iCurLabelNumber < 1 || iCurLabelNumber >4) {
				   iCurLabelNumber = 1;
			   }
               /*else if (webinar.HasMember("LABEL_NUMBER") && webinar["LABEL_NUMBER"].IsString()) {
                  iCurLabelNumber = atoi(webinar["LABEL_NUMBER"].GetString());
                  LOGE("%s LABEL_NUMBER  IsString() %d\n", __FUNCTION__, iCurLabelNumber);
               }
               else if (webinar.HasMember("LABEL_NUMBER") && webinar["LABEL_NUMBER"].IsInt()) {
                  iCurLabelNumber = webinar["LABEL_NUMBER"].GetInt();
                  LOGE("%s LABEL_NUMBER  IsInt() %d\n", __FUNCTION__, iCurLabelNumber);
               }*/

               if (webinar.HasMember("course_live_sharpness") && webinar["course_live_sharpness"].IsArray()) {
                  rapidjson::Value& course_live_sharpness = webinar["course_live_sharpness"];

						mClassRoomBaseInfo.mWebinarInfo.videoPushAndMixSetting.mixSettings.clear();
                  for (int i = 0; i < course_live_sharpness.Size(); i++) {

                     UserSettings setting;
                     rapidjson::Value& sharpness = course_live_sharpness[i];
                     if (sharpness.HasMember("VIDEO_PROFILE") && sharpness["VIDEO_PROFILE"].IsString()) {
                        setting.video_profile = atoi(sharpness["VIDEO_PROFILE"].GetString());
                     }

                     if (sharpness.HasMember("CONNECT_VIDEO_PROFILE") && sharpness["CONNECT_VIDEO_PROFILE"].IsString()) {
                        setting.Interaction_profile = atoi(sharpness["CONNECT_VIDEO_PROFILE"].GetString());
                     }
                     if (sharpness.HasMember("DOUBLE_VRTC_VIDEO_PROFILE") && sharpness["DOUBLE_VRTC_VIDEO_PROFILE"].IsString()) {
                        setting.DoublePush_profile = atoi(sharpness["DOUBLE_VRTC_VIDEO_PROFILE"].GetString());
                     }
                     if (sharpness.HasMember("DESKTOP_SCREEN_PROFILE") && sharpness["DESKTOP_SCREEN_PROFILE"].IsString()) {
                        setting.Desktop_profile = atoi(sharpness["DESKTOP_SCREEN_PROFILE"].GetString());
                     }

                     if (sharpness.HasMember("BROADCAST_VIDEO_PROFILE") && sharpness["BROADCAST_VIDEO_PROFILE"].IsString()) {
                        setting.broadcast_video_profile = atoi(sharpness["BROADCAST_VIDEO_PROFILE"].GetString());
                     }

                     if (sharpness.HasMember("CN_DESC") && sharpness["CN_DESC"].IsString()) {
                        setting.desc = sharpness["CN_DESC"].GetString();
                     }

                     if (sharpness.HasMember("LABEL_NUMBER") && sharpness["LABEL_NUMBER"].IsString()) {
                        setting.label_number  = atoi(sharpness["LABEL_NUMBER"].GetString());
                     }
                     else if (sharpness.HasMember("LABEL_NUMBER") && sharpness["LABEL_NUMBER"].IsInt()) {
                        setting.label_number = sharpness["LABEL_NUMBER"].GetInt();
                     }
                     

                     if (sharpness.HasMember("IS_ALLOW_LIVE_USE") && sharpness["IS_ALLOW_LIVE_USE"].IsString()) {
                        setting.mbEnable = ( 1==atoi(sharpness["IS_ALLOW_LIVE_USE"].GetString()) );
                     }
                     else if (sharpness.HasMember("IS_ALLOW_LIVE_USE") && sharpness["IS_ALLOW_LIVE_USE"].IsInt()) {
                        setting.mbEnable = ( 1 == sharpness["IS_ALLOW_LIVE_USE"].GetInt() );
                     }


                     mClassRoomBaseInfo.mWebinarInfo.videoPushAndMixSetting.mixSettings.push_back(setting);
                  }
               }

               if (webinar.HasMember("countdowninfo") && webinar["countdowninfo"].IsObject())
               {
                  rapidjson::Value& countdown_info = webinar["countdowninfo"];
                  if (countdown_info.HasMember("starttime") && countdown_info["starttime"].IsInt()) {
                     mClassRoomBaseInfo.mWebinarInfo.countdown_starttime = countdown_info["starttime"].GetInt();
                  }
                  if (countdown_info.HasMember("timer") && countdown_info["timer"].IsInt()) {
                     mClassRoomBaseInfo.mWebinarInfo.countdown_time = countdown_info["timer"].GetInt();
                  }


               }
               if (webinar.HasMember("interactive_mode") && webinar["interactive_mode"].IsInt()) {
                  mClassRoomBaseInfo.mWebinarInfo.speedmode = webinar["interactive_mode"].GetInt();
               }
					else if (webinar.HasMember("interactive_mode") && webinar["interactive_mode"].IsString()) {
						mClassRoomBaseInfo.mWebinarInfo.speedmode = atoi(webinar["interactive_mode"].GetString());
					}



               //考试相关信息获取
               if (webinar.HasMember("exam_init_info") && webinar["exam_init_info"].IsObject()) {
                  rapidjson::Value& examInitInfo = webinar["exam_init_info"];

                  if (examInitInfo.HasMember("examing") && examInitInfo["examing"].IsString()) {
                     mClassRoomBaseInfo.mExamInitInfo.strExaming  = examInitInfo["examing"].GetString();
                  }

                  if (examInitInfo.HasMember("naire_status") && examInitInfo["naire_status"].IsString()) {
                     mClassRoomBaseInfo.mExamInitInfo.naire_status = atoi(examInitInfo["naire_status"].GetString());
                  }
                  else if (examInitInfo.HasMember("naire_status") && examInitInfo["naire_status"].IsInt()) {
                     mClassRoomBaseInfo.mExamInitInfo.naire_status = examInitInfo["naire_status"].GetInt();
                  }

                  if (examInitInfo.HasMember("user_status") && examInitInfo["user_status"].IsString()) {
                     mClassRoomBaseInfo.mExamInitInfo.user_status = atoi(examInitInfo["user_status"].GetString());
                  }
                  else if (examInitInfo.HasMember("user_status") && examInitInfo["user_status"].IsInt()) {
                     mClassRoomBaseInfo.mExamInitInfo.user_status = examInitInfo["user_status"].GetInt();
                  }
               }

               //if (webinar.HasMember("play_type") && webinar["play_type"].IsInt()) {
               //   mClassRoomBaseInfo.play_type = (eClassPlayType)webinar["play_type"].GetInt();
               //}
               //else if (webinar.HasMember("play_type") && webinar["play_type"].IsString()) {
               //   std::string strPlayType = webinar["play_type"].GetString();
               //   mClassRoomBaseInfo.play_type = (eClassPlayType)atoi(strPlayType.c_str());
               //}

               //if (webinar.HasMember("desktop_share_layout_mode") && webinar["desktop_share_layout_mode"].IsInt()) {
               //   mClassRoomBaseInfo.mWebinarInfo.desktopShareLayout = webinar["desktop_share_layout_mode"].GetInt();
               //}
               //else if (webinar.HasMember("desktop_share_layout_mode") && webinar["desktop_share_layout_mode"].IsString()) {
               //   mClassRoomBaseInfo.mWebinarInfo.desktopShareLayout = atoi(webinar["desktop_share_layout_mode"].GetString());
               //}


               if (webinar.HasMember("desktop_share_layout_mode") && webinar["desktop_share_layout_mode"].IsInt()) {
                  mClassRoomBaseInfo.mWebinarInfo.desktop_share_layout_mode = webinar["desktop_share_layout_mode"].GetInt();
               }
               else if (webinar.HasMember("desktop_share_layout_mode") && webinar["desktop_share_layout_mode"].IsString()) {
                  mClassRoomBaseInfo.mWebinarInfo.desktop_share_layout_mode = atoi(webinar["desktop_share_layout_mode"].GetString());
               }
            }

            if (resp.HasMember("join") && resp.IsObject()) {
               rapidjson::Value& join = resp["join"];
               if (join.HasMember("id") && join["id"].IsString()) {
                  mClassRoomBaseInfo.mUserInfo.join_id = join["id"].GetString();
               }
               if (join.HasMember("nick_name") && join["nick_name"].IsString()) {
                  mClassRoomBaseInfo.mUserInfo.nick_name = join["nick_name"].GetString();
               }

               if (join.HasMember("role_type") && join["role_type"].IsInt()) {
                  mClassRoomBaseInfo.mUserInfo.role_type = (JOIN_ROLE)join["role_type"].GetInt();
               }
               if (join.HasMember("role_type") && join["role_type"].IsString()) {
                  std::string strRole = join["role_type"].GetString();
                  mClassRoomBaseInfo.mUserInfo.role_type = (JOIN_ROLE)atoi(strRole.c_str());
               }

               if (join.HasMember("session_id") && join["session_id"].IsString()) {
                  mClassRoomBaseInfo.mUserInfo.session_id = join["session_id"].GetString();
               }
               if (join.HasMember("banchat") && join["banchat"].IsInt()) {
                  mClassRoomBaseInfo.mUserInfo.banchat = join["banchat"].GetInt();
               }
               if (join.HasMember("no_mic") && join["no_mic"].IsInt()) {
                  mClassRoomBaseInfo.mUserInfo.no_mic = join["no_mic"].GetInt();
               }

               if (join.HasMember("reward_num") && join["reward_num"].IsInt()) {
                  mClassRoomBaseInfo.mUserInfo.iRewardNum = join["reward_num"].GetInt();
               }
               else if (join.HasMember("reward_num") && join["reward_num"].IsString())
               {
                  std::string strNum = join["reward_num"].GetString();
                  mClassRoomBaseInfo.mUserInfo.iRewardNum = atoi(strNum.c_str());
               }

            }

            if (resp.HasMember("msg") && resp.IsObject()) {//替换  msg
               rapidjson::Value& message = resp["msg"];
               if (message.HasMember("srv") && message["srv"].IsString()) {
                  mMessageInfo.srv = message["srv"].GetString();
               }
               if (message.HasMember("msg_token") && message["msg_token"].IsString()) {//替换 msg_token
                  mMessageInfo.token = message["msg_token"].GetString();
               }
            }

            if (resp.HasMember("chat") && resp.IsObject()) {
               rapidjson::Value& chat = resp["chat"];
               if (chat.HasMember("srv") && chat["srv"].IsString()) {
                  mMessageInfo.chat = chat["srv"].GetString();
               }
               if (chat.HasMember("publish_url") && chat["publish_url"].IsString()) {
                  mMessageInfo.publish_url = chat["publish_url"].GetString();
               }
            }
            if (resp.HasMember("view_page_urls") && resp.IsObject()) {
               rapidjson::Value& view_page_urls = resp["view_page_urls"];

               if (view_page_urls.HasMember("doc") && view_page_urls["doc"].IsString()) {
                  mClassRoomBaseInfo.urls.doc = view_page_urls["doc"].GetString();
               }
               if (view_page_urls.HasMember("board") && view_page_urls["board"].IsString()) {
                  mClassRoomBaseInfo.urls.board = view_page_urls["board"].GetString();
               }
               if (view_page_urls.HasMember("answer") && view_page_urls["answer"].IsString()) {
                  mClassRoomBaseInfo.urls.answer = view_page_urls["answer"].GetString();
               }
               if (view_page_urls.HasMember("mediaplayer") && view_page_urls["mediaplayer"].IsString()) {
                  mClassRoomBaseInfo.urls.mediaplayer = view_page_urls["mediaplayer"].GetString();
               }
               if (view_page_urls.HasMember("chat") && view_page_urls["chat"].IsString()) {
                  mClassRoomBaseInfo.urls.chat = view_page_urls["chat"].GetString();
               }
               if (view_page_urls.HasMember("record") && view_page_urls["record"].IsString()) {
                  mClassRoomBaseInfo.urls.record = view_page_urls["record"].GetString();
               }
               if (view_page_urls.HasMember("mixvideo") && view_page_urls["mixvideo"].IsString()) {
                  mClassRoomBaseInfo.urls.mixvideo = view_page_urls["mixvideo"].GetString();
               }

               if (view_page_urls.HasMember("questionnaire") && view_page_urls["questionnaire"].IsString()) {
                  mClassRoomBaseInfo.urls.questionnaire = view_page_urls["questionnaire"].GetString();
               }

            }

            if (resp.HasMember("doc") && resp.IsObject()) {
               rapidjson::Value& doc = resp["doc"];
               if (doc.HasMember("srv") && doc["srv"].IsString()) {
                  mClassRoomBaseInfo.mDocInfo.srv = doc["srv"].GetString();
               }
               if (doc.HasMember("curr_step") && doc["curr_step"].IsString()) {
                  mClassRoomBaseInfo.mDocInfo.curr_step = doc["curr_step"].GetString();
               }
               if (doc.HasMember("curr_page") && doc["curr_page"].IsString()) {
                  mClassRoomBaseInfo.mDocInfo.curr_page = doc["curr_page"].GetString();
               }
               if (doc.HasMember("curr_file") && doc["curr_file"].IsString()) {
                  mClassRoomBaseInfo.mDocInfo.curr_file = doc["curr_file"].GetString();
               }
               if (doc.HasMember("doc_type") && doc["doc_type"].IsInt()) {
                  mClassRoomBaseInfo.mDocInfo.doc_type = doc["doc_type"].GetInt();
               }
               if (doc.HasMember("total_page") && doc["total_page"].IsString()) {
                  mClassRoomBaseInfo.mDocInfo.total_page = doc["total_page"].GetString();
               }
               if (doc.HasMember("converted_age") && doc["converted_age"].IsInt()) {
                  mClassRoomBaseInfo.mDocInfo.converted_age = doc["converted_age"].GetInt();
               }
               if (doc.HasMember("docMode") ) {//替换 
                  if (doc["docMode"].IsInt()) {
                     mClassRoomBaseInfo.mDocInfo.doc_mode = doc["docMode"].GetInt();
                  }
                  else if (doc["docMode"].IsString()) {
                     std::string strdocMode = doc["docMode"].GetString();
                     mClassRoomBaseInfo.mDocInfo.doc_mode = atoi(strdocMode.c_str());
                  }
                  
               }
               else 

               if (doc.HasMember("swf_url") && doc["swf_url"].IsString()) {
                  mClassRoomBaseInfo.mDocInfo.swf_url = doc["swf_url"].GetString();
               }
               if (doc.HasMember("skin_url") && doc["skin_url"].IsString()) {
                  mClassRoomBaseInfo.mDocInfo.skin_url = doc["skin_url"].GetString();
               }
               if (doc.HasMember("whiteboard_url") && doc["whiteboard_url"].IsString()) {
                  mClassRoomBaseInfo.mDocInfo.skin_url = doc["whiteboard_url"].GetString();
               }
            }

            if (resp.HasMember("config") && resp.IsObject()) {
               rapidjson::Value& config = resp["config"];
               if (config.HasMember("doc") && config["doc"].IsInt()) {
                  mClassRoomBaseInfo.mToolConfig.doc = config["doc"].GetInt();
               }
               if (config.HasMember("board") && config["board"].IsInt()) {
                  mClassRoomBaseInfo.mToolConfig.board = config["board"].GetInt();
               }
               if (config.HasMember("desktop") && config["desktop"].IsInt()) {
                  mClassRoomBaseInfo.mToolConfig.desktop = config["desktop"].GetInt();
               }
               if (config.HasMember("answer") && config["answer"].IsInt()) {
                  mClassRoomBaseInfo.mToolConfig.answer = config["answer"].GetInt();
               }
               if (config.HasMember("memberList") && config["memberList"].IsInt()) {
                  mClassRoomBaseInfo.mToolConfig.memberList = config["memberList"].GetInt();
               }
               if (config.HasMember("uploadDoc") && config["uploadDoc"].IsInt()) {
                  mClassRoomBaseInfo.mToolConfig.uploadDoc = config["uploadDoc"].GetInt();
               }
               if (config.HasMember("mediaplayer") && config["mediaplayer"].IsInt()) {
                  mClassRoomBaseInfo.mToolConfig.mediaplayer = config["mediaplayer"].GetInt();
               }

               if (config.HasMember("interact") && config["interact"].IsInt()) {
                  mClassRoomBaseInfo.mToolConfig.InteractionTool = config["interact"].GetInt();
               }

               if (config.HasMember("sign") && config["sign"].IsObject()) {

                  rapidjson::Value& sign = config["sign"];


                  if (sign.HasMember("rest_time") && sign["rest_time"].IsInt()) {
                     mClassRoomBaseInfo.mStruSign.irest_time = sign["rest_time"].GetInt();
                  }
                  else if (sign.HasMember("rest_time") && sign["rest_time"].IsString())
                  {
                     std::string strRestTime = sign["rest_time"].GetString();
                     mClassRoomBaseInfo.mStruSign.irest_time = atoi(strRestTime.c_str());
                  }


                  if (sign.HasMember("sign_status") && sign["sign_status"].IsString()) {
                     mClassRoomBaseInfo.mStruSign.strSignStatus = sign["sign_status"].GetString();
                  }

                  if (sign.HasMember("sign_id") && sign["sign_id"].IsString()) {
                     mClassRoomBaseInfo.mStruSign.strSignId = sign["sign_id"].GetString();
                  }
                  else if (sign.HasMember("sign_id") && sign["sign_id"].IsInt()) {
                     int isign_id = sign["sign_id"].GetInt();
                     char string[32];
                     mClassRoomBaseInfo.mStruSign.strSignId = itoa(isign_id, string, 10);
                  }

                  if (sign.HasMember("sign_user_count") && sign["sign_user_count"].IsInt()) {
                     mClassRoomBaseInfo.mStruSign.iSignUserCount = sign["sign_user_count"].GetInt();
                  }
                  else if (sign.HasMember("sign_user_count") && sign["sign_user_count"].IsString())
                  {
                     std::string strRestTime = sign["sign_user_count"].GetString();
                     mClassRoomBaseInfo.mStruSign.iSignUserCount = atoi(strRestTime.c_str());
                  }
               }

            }

            if (resp.HasMember("mic_token") && resp["mic_token"].IsString()) {
               mClassRoomBaseInfo.mic_token = resp["mic_token"].GetString();
            }
            if (resp.HasMember("class_token") && resp["class_token"].IsString()) {
               mClassToken = resp["class_token"].GetString();
            }
            if (resp.HasMember("stream_token") && resp["stream_token"].IsString()) {
               mStreamToken = resp["stream_token"].GetString();
            }
            if (resp.HasMember("openhand") && resp["openhand"].IsInt()) {
               mClassRoomBaseInfo.openhand = resp["openhand"].GetInt() == 1 ? true : false;
            }
            else if (resp.HasMember("openhand") && resp["openhand"].IsString())
            {
               std::string strOpenhand = resp["openhand"].GetString();
               mClassRoomBaseInfo.openhand = atoi(strOpenhand.c_str());
            }

            if (resp.HasMember("big_screen") && resp["big_screen"].IsInt()) {
               mClassRoomBaseInfo.bigScreen = resp["big_screen"].GetInt();
            }
            else if (resp.HasMember("big_screen") && resp["big_screen"].IsString())
            {
               std::string strOpenhand = resp["big_screen"].GetString();
               mClassRoomBaseInfo.bigScreen = atoi(strOpenhand.c_str());
            }

            if (resp.HasMember("current_speaker") && resp["current_speaker"].IsString()) {
               mClassRoomBaseInfo.current_speaker = resp["current_speaker"].GetString();
            }
            if (resp.HasMember("banchatall") && resp["banchatall"].IsInt()) {
               mClassRoomBaseInfo.banchatall = resp["banchatall"].GetInt();
            }

            //if(mClassRoomBaseInfo.mUserInfo.role_type == JOIN_ROLE_Student && mClassRoomBaseInfo.mWebinarInfo.course_type == CLASS_TYPE_MULTI){
            //    if(mClassRoomBaseInfo.bigScreen != std::atoi(mClassRoomBaseInfo.current_speaker.c_str()) && mClassRoomBaseInfo.bigScreen != std::atoi(mClassRoomBaseInfo.mUserInfo.join_id.c_str())) {
            //       mClassRoomBaseInfo.bigScreen = std::atoi(mClassRoomBaseInfo.mUserInfo.join_id.c_str());
            //    }
            //}

            if (resp.HasMember("mutingall") && resp["mutingall"].IsString()) {
               mClassRoomBaseInfo.mutingAll = atoi(resp["mutingall"].GetString());
            }

            //用户类型
            if (resp.HasMember("user_type") && resp["user_type"].IsInt()) { 
               mClassRoomBaseInfo.play_type = (eClassPlayType)resp["user_type"].GetInt();
            }
            else if (resp.HasMember("user_type") && resp["user_type"].IsString()) {
               std::string strPlayType = resp["user_type"].GetString();
               mClassRoomBaseInfo.play_type = (eClassPlayType)atoi(strPlayType.c_str());
            }

            if (resp.HasMember("ready_list") && resp["ready_list"].IsString()) {
               std::string ready_list = resp["ready_list"].GetString();
               size_t dstLen = 8192;
               char dstBuffer[8192] = { 0 };

               SystemUtility::url_decode(dstBuffer, dstLen, ready_list.c_str(), ready_list.length());
               //[{"join_id":"70759","nick_name":"739703","role":1,"is_quiet":0,"is_frame":0}]
               int nCode = RESP_SUCCESS_CODE;
               std::string strNoticeMsg;
               std::list<VHRoomMember> memberList;
               rapidjson::Document docReadyList;
               docReadyList.Parse<0>(dstBuffer);
               if (docReadyList.IsArray()) {
                  for (int i = 0; i < docReadyList.Size(); i++) {
                     rapidjson::Value& cur_members = docReadyList[i];
                     PublishUser vhMember;
                     if (cur_members.HasMember("join_id") && cur_members["join_id"].IsString()) {
                        vhMember.join_id = cur_members["join_id"].GetString();
                     }
                     if (cur_members.HasMember("nick_name") && cur_members["nick_name"].IsString()) {
                        vhMember.nick_name = cur_members["nick_name"].GetString();
                     }
                     if(docReadyList.Size() == 1 && mClassRoomBaseInfo.mUserInfo.join_id == vhMember.join_id && mClassRoomBaseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher){
                        mClassRoomBaseInfo.bigScreen = 0;
                     }
                     if (cur_members.HasMember("is_quiet") && cur_members["is_quiet"].IsInt()) {
                        vhMember.is_quiet = cur_members["is_quiet"].GetInt();
                     }
                     if (cur_members.HasMember("role") && cur_members["role"].IsInt()) {
                        vhMember.role = cur_members["role"].GetInt();
                     }
                     if (cur_members.HasMember("is_frame") && cur_members["is_frame"].IsInt()) {
                        vhMember.is_frame = cur_members["is_frame"].GetInt();
                     }

                     if (cur_members.HasMember("reward_num") && cur_members["reward_num"].IsInt()) {
                        vhMember.iRewardNum = cur_members["reward_num"].GetInt();
                     }
                     else if (cur_members.HasMember("reward_num") && cur_members["reward_num"].IsString()) {
                        std::string strRewardNum = cur_members["reward_num"].GetString();
                        vhMember.iRewardNum = atoi(strRewardNum.c_str());
                     }

                     mClassRoomBaseInfo.ready_list.push_back(vhMember);
                  }
               }
            }

            if (mClassRoomBaseInfo.ready_list.size() == 0 && mClassRoomBaseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher) {
               mClassRoomBaseInfo.bigScreen = 0;
            }

            if (resp.HasMember("muting_teacher") && resp["muting_teacher"].IsString()) {
               //mClassRoomBaseInfo.muting_teacher = resp["muting_teacher"].GetString();
            }
            if (resp.HasMember("muting_student") && resp["muting_student"].IsString()) {
               //mClassRoomBaseInfo.muting_student = resp["muting_student"].GetString();
            }
            if (resp.HasMember("muting_student") && resp["muting_student"].IsString()) {
               //mClassRoomBaseInfo.muting_student = resp["muting_student"].GetString();
            }
            if (resp.HasMember("close_screen_teacher") && resp["close_screen_teacher"].IsString()) {
               //mClassRoomBaseInfo.closeScreenTeacher = resp["close_screen_teacher"].GetString();
            }
            if (resp.HasMember("close_screen_student") && resp["close_screen_student"].IsString()) {
               //mClassRoomBaseInfo.closeScreenStudent = resp["close_screen_student"].GetString();
            }
            if (resp.HasMember("brush") && resp["brush"].IsString()) {
               mClassRoomBaseInfo.brush = atoi(resp["brush"].GetString());
            }

            if (resp.HasMember("record") && resp.IsObject()) {
               rapidjson::Value& record = resp["record"];
               if (record.HasMember("view_url") && record["view_url"].IsString()) {
                  mClassRoomBaseInfo.record_view_url = record["view_url"].GetString();
               }
            }

            if (resp.HasMember("report") && resp.IsObject()) {
               rapidjson::Value& report = resp["report"];
  
               if (report.HasMember("vid") && report["vid"].IsString()) {
                  mClassRoomBaseInfo.strVid = report["vid"].GetString();
               }
               else if (report.HasMember("vid") && report["vid"].IsInt()) {
                  int iVid = report["vid"].GetInt();
                  char string[32];
                  mClassRoomBaseInfo.strVid = itoa(iVid, string, 10);
                  mClassRoomBaseInfo.strVid = string;

               }

               if (report.HasMember("vfid") && report["vfid"].IsString()) {
                  mClassRoomBaseInfo.strVfid = report["vfid"].GetString();
               }
               else if (report.HasMember("vfid") && report["vfid"].IsInt()) {
                  int iVfid = report["vfid"].GetInt();
                  char string[32];
                  mClassRoomBaseInfo.strVfid = itoa(iVfid, string, 10);
                  mClassRoomBaseInfo.strVfid = string;

                }
            }

         }
      }
   }

   if (nCode != RESP_SUCCESS_CODE) {
      if (mpVHRoomMonitor) {
         LOGD(" data:%s code:%d", strNoticeMsg.c_str(), nCode);
         //iCurLabelNumber;
         mpVHRoomMonitor->OnJoinClassRoom(nCode, strNoticeMsg);
      }
   }
   else {
      //mpVHRoomMonitor->OnInitQualityNumber(iCurLabelNumber);
      if (mpVHWebSocketInterface) {
         LOGD("chat and socketio connect");

         mpVHWebSocketInterface->On("cmd", [&](const std::string& name, const std::string& msg)->void {
            LOGD("Inav msg:%s", msg.c_str());
            ParamRecvInavMsg(msg);
         });
         mpVHWebSocketInterface->On("flashMsg", [&](const std::string& name, const std::string& msg)->void {
            HandleRecvFlashMsg(msg);
         });

         std::string url = "ws://" + mMessageInfo.srv;
         url += std::string("/socket.io/");
         mpVHWebSocketInterface->ConnectServer(url.c_str(), mMessageInfo.token.c_str());
         LOGD("url :%s ", url.c_str());
      }
   }
}
                              
void VHClassRoomImpl::ParamToJoinPaasRoom(const std::string & msg)
{
   LOGD("%s msg:%s", __FUNCTION__, msg.c_str());
   std::string strNoticeMsg;
   rapidjson::Document doc;
   doc.Parse<0>(msg.c_str());
   if (doc.HasParseError()) {
      LOGE("%s ParseError%d\n", __FUNCTION__, doc.GetParseError());
   }
   else {
      int nCode = RESP_SUCCESS_CODE;
      int iCurLabelNumber = 1;
      if (doc.IsObject()) {
         if (doc.HasMember("code") && doc["code"].IsInt()) {
            nCode = doc["code"].GetInt();
         }
         if (doc.HasMember("msg") && doc["msg"].IsString()) {
            strNoticeMsg = doc["msg"].GetString();
         }
         if (RESP_SUCCESS_CODE == nCode) {
            if (doc.HasMember("data") && doc["data"].IsObject()) {
               rapidjson::Value& data = doc["data"];

               std::string accesstoken;
               std::string appid;
               std::string thrid_user_id;
               std::string roomid;
               std::string live_roomid;
               std::list<std::string> listenChannel;
               std::string accountId;
               std::string  context;

               if (data.HasMember("app_id") && data["app_id"].IsString()) {
                  appid = data["app_id"].GetString();
               }

               if (data.HasMember("join_id") && data["join_id"].IsString()) {
                  thrid_user_id = data["join_id"].GetString();
               }

               if (data.HasMember("access_token") && data["access_token"].IsString()) {
                  accesstoken = data["access_token"].GetString();
               }

               if (data.HasMember("pass_room_id") && data["pass_room_id"].IsString()) {
                  roomid = data["pass_room_id"].GetString();
               }

               if (data.HasMember("pass_inav_id") && data["pass_inav_id"].IsString()) {
                  live_roomid = data["pass_inav_id"].GetString();
               }

               if (data.HasMember("pass_channel_id") && data["pass_channel_id"].IsString()) {
                  std::string channel = data["pass_channel_id"].GetString();
                  listenChannel.push_back(channel);
               }

               if (data.HasMember("accountId") && data["accountId"].IsString()) {
                  accountId = data["accountId"].GetString(); 
               }
               else if (data.HasMember("accountId") && data["accountId"].IsInt()) {
                  char string[32];
                  accountId = itoa(doc["accountId"].GetInt(), string, 10);
                  accountId = string;
               }


               if (mpPaasInteraction != nullptr) {
                  mpPaasInteraction->Login(accesstoken,
                     appid,
                     thrid_user_id,
                     roomid,
                     live_roomid,
                     listenChannel,
                     context);

                  //InavInfo inavInfo;         //mInavInfo
 

                  //inavInfo.inav_token = accesstoken;        //权限id
                  //inavInfo.third_party_user_id = thrid_user_id;   //第三方用户ID
                  ////inavInfo.push_address;  //旁路推流地址
                  //inavInfo.account_id = accountId;              //开发者ID	

                  //mpPaasInteraction->InitRtcRoomParameter(inavInfo, mReportUrl, live_roomid, appid);
                  //QJsonObject obj;
                  //obj["join_uid"] = QString::fromStdString(mClassRoomBaseInfo.mUserInfo.join_id);
                  //obj["join_uname"] = QString::fromStdString(mClassRoomBaseInfo.mUserInfo.nick_name);
                  //obj["join_role"] = mClassRoomBaseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher == mClassRoomBaseInfo.mUserInfo.role_type ? QString("host") : QString("user");
                  //QJsonDocument document = QJsonDocument(obj);
                  //QByteArray array = document.toJson();
                  //std::string userData = array.toStdString();
                  //mpPaasInteraction->JoinRTCRoom(userData, true);
               }
            }
         }
         else {
            if (mpVHRoomMonitor) {
               LOGD(" data:%s code:%d", strNoticeMsg.c_str(), nCode);
               //iCurLabelNumber;
               mpVHRoomMonitor->OnJoinClassRoom(nCode, strNoticeMsg);
            }
         }
      }
   }
   
}

int VHClassRoomImpl::ReconnectNetWork(){
   std::string url = "ws://" + mMessageInfo.srv;
   url += std::string("/socket.io/");
   mpVHWebSocketInterface->ConnectServer(url.c_str(), mMessageInfo.token.c_str());
   LOGD("url :%s ", url.c_str());
   return 0;
}

int VHClassRoomImpl::ClearPassSdkInfo()
{
   if (mpPaasInteraction != nullptr) {
      mpPaasInteraction->ClearInitInfo();
   }
   return 0;
}

void VHClassRoomImpl::ParamToOnAnswerStart(const std::string& msg) {
	LOGD("%s msg:%s", __FUNCTION__, msg.c_str());
	int nCode = RESP_SUCCESS_CODE;
	std::string strNoticeMsg;

	rapidjson::Document doc;
	doc.Parse<0>(msg.c_str());
	if (doc.HasParseError()) {
		LOGE("%s ParseError%d\n", __FUNCTION__, doc.GetParseError());
		return;
	}
	if (doc.IsObject()) {
		if (doc.HasMember("code") && doc["code"].IsInt()) {
			nCode = doc["code"].GetInt();
		}
		if (nCode == 200 && mpVHRoomMonitor) {
			mpVHRoomMonitor->OnRecvStartAnswerMsg(nCode, msg);
		}
	}

}

void VHClassRoomImpl::ParamToAnswerDetail(const std::string& msg) {
	LOGD("%s msg:%s", __FUNCTION__, msg.c_str());
	int nCode = RESP_SUCCESS_CODE;
	std::string strNoticeMsg;

	rapidjson::Document doc;
	doc.Parse<0>(msg.c_str());
	if (doc.HasParseError()) {
		LOGE("%s ParseError%d\n", __FUNCTION__, doc.GetParseError());
		return;
	}
	if (doc.IsObject()) {
		if (doc.HasMember("code") && doc["code"].IsInt()) {
			nCode = doc["code"].GetInt();
		}
		if (nCode == 200 && mpVHRoomMonitor) {
			mpVHRoomMonitor->OnRecvAnswerDetailMsg(nCode, msg);
		}
	}
}


void VHClassRoomImpl::ParamToOnSignRecordResp(const std::string& msg) {
   LOGD("%s msg:%s", __FUNCTION__, msg.c_str());
   int nCode = RESP_SUCCESS_CODE;
   std::string strNoticeMsg;

   rapidjson::Document doc;
   doc.Parse<0>(msg.c_str());
   if (doc.HasParseError()) {
      LOGE("%s ParseError%d\n", __FUNCTION__, doc.GetParseError());
      return;
   }
   if (doc.IsObject()) {
      if (doc.HasMember("code") && doc["code"].IsInt()) {
         nCode = doc["code"].GetInt();
      }
      if (doc.HasMember("msg") && doc["msg"].IsString()) {
         strNoticeMsg = doc["msg"].GetString();
      }

      if (RESP_SUCCESS_CODE == nCode) {
         if (doc.HasMember("data") && doc["data"].IsObject()) {
            rapidjson::Value& data = doc["data"];
            std::list<std::string> stlistsignUsers;
            std::list<std::string> stlistNoSignUsers;

            //签到学员
            if (data.HasMember("sign_users") && data["sign_users"].IsArray()) {
               rapidjson::Value& signUsers = data["sign_users"];

               if (signUsers.IsArray() && !signUsers.Empty())
               {
                  std::string strName;
                  for (rapidjson::SizeType i = 0; i < signUsers.Size(); i++)
                  {
                     rapidjson::Value& signUser = signUsers[i];
                     if (signUser.IsObject() && signUser.HasMember("signer_nickname") && signUser["signer_nickname"].IsString())
                     {
                        strName = signUser["signer_nickname"].GetString();

                        stlistsignUsers.push_back(strName);
                     }
                  }
               }
            }

            //未签到学员
            if (data.HasMember("no_sign_users") && data["no_sign_users"].IsArray()) {
               rapidjson::Value& signUsers = data["no_sign_users"];

               if (signUsers.IsArray() && !signUsers.Empty())
               {
                  std::string strName;
                  for (rapidjson::SizeType i = 0; i < signUsers.Size(); i++)
                  {
                     rapidjson::Value& signUser = signUsers[i];
                     if (signUser.IsObject() && signUser.HasMember("nick_name") && signUser["nick_name"].IsString())
                     {
                        strName = signUser["nick_name"].GetString();

                        stlistNoSignUsers.push_back(strName);
                     }
                  }
               }
            }
            mpVHRoomMonitor->OnSignUsersMsg(stlistsignUsers, stlistNoSignUsers);
         }
      }
   }
}

void VHClassRoomImpl::HandleRecvFlashMsg(const std::string& msg) {
   rapidjson::Document jsonDoc;
   rapidjson::Document::AllocatorType &allocator = jsonDoc.GetAllocator();
   rapidjson::Value userIdJson(rapidjson::kArrayType);
   rapidjson::Value flashMsg(StringRef("flashMsg"));

   rapidjson::Document doc;
   doc.Parse<0>(msg.c_str());

   userIdJson.PushBack(flashMsg, allocator);
   userIdJson.PushBack(doc, allocator);

   StringBuffer buffer;
   rapidjson::Writer<StringBuffer> writer(buffer);
   userIdJson.Accept(writer);
   std::string data = buffer.GetString();

   mpVHRoomMonitor->OnRecvFlahsMsg(data);
}

void VHClassRoomImpl::HandleSetBrushMsg(const std::string& msg) {
   rapidjson::Document jsonDoc;
   rapidjson::Document::AllocatorType &allocator = jsonDoc.GetAllocator();
   rapidjson::Value userIdJson(rapidjson::kArrayType);
   rapidjson::Value flashMsg(StringRef("cmd"));

   rapidjson::Document doc;
   doc.Parse<0>(msg.c_str());

   userIdJson.PushBack(flashMsg, allocator);
   userIdJson.PushBack(doc, allocator);

   StringBuffer buffer;
   rapidjson::Writer<StringBuffer> writer(buffer);
   userIdJson.Accept(writer);
   std::string data = buffer.GetString();

   mpVHRoomMonitor->OnRecvSetBrushMsg(data, msg);
}

void VHClassRoomImpl::HandleDocUploadMsg(const std::string& msg) {
   rapidjson::Document jsonDoc;
   rapidjson::Document::AllocatorType &allocator = jsonDoc.GetAllocator();
   rapidjson::Value userIdJson(rapidjson::kArrayType);
   rapidjson::Value flashMsg(StringRef("cmd"));

   rapidjson::Document doc;
   doc.Parse<0>(msg.c_str());

   userIdJson.PushBack(flashMsg, allocator);
   userIdJson.PushBack(doc, allocator);

   StringBuffer buffer;
   rapidjson::Writer<StringBuffer> writer(buffer);
   userIdJson.Accept(writer);
   std::string data = buffer.GetString();

   mpVHRoomMonitor->OnRecvDocUploadMsg(data);
}

void VHClassRoomImpl::HandleRecvMediaMsg(const std::string& msg) {
   rapidjson::Document jsonDoc;
   rapidjson::Document::AllocatorType &allocator = jsonDoc.GetAllocator();
   rapidjson::Value userIdJson(rapidjson::kArrayType);
   rapidjson::Value flashMsg(StringRef("cmd"));

   rapidjson::Document doc;
   doc.Parse<0>(msg.c_str());

   userIdJson.PushBack(flashMsg, allocator);
   userIdJson.PushBack(doc, allocator);

   StringBuffer buffer;
   rapidjson::Writer<StringBuffer> writer(buffer);
   userIdJson.Accept(writer);
   std::string data = buffer.GetString();

   mpVHRoomMonitor->OnRecvMediaMsg(data);
}

void VHClassRoomImpl::HandleAnswerMsg(const std::string& msg) {
   rapidjson::Document jsonDoc;
   rapidjson::Document::AllocatorType &allocator = jsonDoc.GetAllocator();
   rapidjson::Value userIdJson(rapidjson::kArrayType);
   rapidjson::Value flashMsg(StringRef("cmd"));

   rapidjson::Document doc;
   doc.Parse<0>(msg.c_str());

   userIdJson.PushBack(flashMsg, allocator);
   userIdJson.PushBack(doc, allocator);

   StringBuffer buffer;
   rapidjson::Writer<StringBuffer> writer(buffer);
   userIdJson.Accept(writer);
   std::string data = buffer.GetString();

   mpVHRoomMonitor->OnRecvAnswerMsg(data);
}

void VHClassRoomImpl::HandleAnswerToolMsg(const std::string& msg) {
   rapidjson::Document jsonDoc;
   rapidjson::Document::AllocatorType &allocator = jsonDoc.GetAllocator();
   rapidjson::Value userIdJson(rapidjson::kArrayType);
   rapidjson::Value flashMsg(StringRef("cmd"));

   rapidjson::Document doc;
   doc.Parse<0>(msg.c_str());

   userIdJson.PushBack(flashMsg, allocator);
   userIdJson.PushBack(doc, allocator);

   StringBuffer buffer;
   rapidjson::Writer<StringBuffer> writer(buffer);
   userIdJson.Accept(writer);
   std::string data = buffer.GetString();

   if (mpVHRoomMonitor) {
      mpVHRoomMonitor->OnRecvAnswerToolMsg(data);
   }
}

CourseInfo VHClassRoomImpl::GetCourseInfo() const {
   return mCourseInfo;
}

ClassRoomBaseInfo VHClassRoomImpl::GetClassRoomBaseInfo() const {
   return mClassRoomBaseInfo;
}

void VHClassRoomImpl::ChangeDocMode(int iMode)
{
   mClassRoomBaseInfo.mDocInfo.doc_mode = iMode;
}

std::string VHClassRoomImpl::GetClassToken() const
{
   return mClassToken;
}

void VHClassRoomImpl::ConnectVHMediaRoom(/*const int& iPlayType*/) {
   LOGD("start");
   //miPlayType = iPlayType;
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
         WebRtcRoomOption option;
         option.role = mClassRoomBaseInfo.mUserInfo.role_type;
         option.classType = mClassRoomBaseInfo.mWebinarInfo.course_type == CLASS_TYPE_SMALL ? 1 : 2;

         option.strRoomToken = SystemUtility::String2WString(mStreamToken);
         option.strUserId = SystemUtility::String2WString(mClassRoomBaseInfo.mUserInfo.join_id);
         option.strLogUpLogUrl = SystemUtility::String2WString(mReportUrl);
         option.strThirdPushStreamUrl = SystemUtility::String2WString(mPublishUrl);
         option.strAccountId = SystemUtility::String2WString(mAppSecret);
         option.strRoomId = SystemUtility::String2WString(mCourseInfo.webinar_id);
         option.strBusinesstype = L"2";  //class
         option.strappid = SystemUtility::String2WString(mAappKey);
         mpWebRtcSDKInterface->ConnetWebRtcRoom(option);
         mpWebRtcSDKInterface->EnableSimulCast(true);
         LOGD("Class SDK ConnetWebRtcRoom mStreamToken:%s mPublishUrl:%s", mStreamToken.c_str(), mPublishUrl.c_str());

   }
   else if (eClassPlayType_H5 == miPlayType )
   {
      //JoinPaasRoom();
      mProcessThreadJoinRtcRoom = new std::thread(VHClassRoomImpl::ThreadProcessJoinRTCRoom, this);
      if (mProcessThreadJoinRtcRoom) {
         LOGD("create task thread success");
         mStatic_TaskEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
         mStatic_ThreadExitEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
      }
      
      //JoinPaasRoom();

   }
   LOGD("end");
}

void VHClassRoomImpl::JoinPaasRoom()
{
   LOGD("%s ", __FUNCTION__);
   std::string strUrl = mURLGenerator.GetPaasRoomInfoUrl(mClassRoomBaseInfo.mWebinarInfo.id, mClassRoomBaseInfo.mUserInfo.join_id, mClassToken);
   LOGD("JoinClassRoom URL:%s ", strUrl.c_str());
   HTTP_GET_REQUEST httpGet(strUrl);
   if (mpHttpManagerInterface) {
      mpHttpManagerInterface->HttpGetRequest(httpGet, [&](std::string data, int code, const std::string userData)->void {
         LOGD("data:%s code:%d", data.c_str(), code);
         AnalysisJoinPaasRoom(data, code);
      });
   }
   LOGD("end");
}

void VHClassRoomImpl::EnableSubScribeStream() {
   LOGD("start");
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      LOGD("  Class SDK EnableSubScribeStream");

      mpWebRtcSDKInterface->EnableSubScribeStream();
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      LOGD("  Paas SDK EnableSubScribeStream");
      mpPaasInteraction->EnableSubScribeStream();
   }

   LOGD("end");
}

void VHClassRoomImpl::DisableSubScribeStream() {
   LOGD("start");
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      LOGD("  Class SDK DisableSubScribeStream");
      mpWebRtcSDKInterface->DisableSubScribeStream();
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      LOGD("  Paas SDK DisableSubScribeStream");
      mpPaasInteraction->DisableSubScribeStream();
   }
   LOGD("end");
}

//void VHClassRoomImpl::DisConnectVHMediaRoom() {
//   LOGD("start");
//   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
//      mpWebRtcSDKInterface->DisConnetWebRtcRoom();
//   }
//   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
//      mpPaasInteraction->LeaveRTCRoom();
//   }
//   LOGD("end");
//}

bool VHClassRoomImpl::LeaveVHMediaRoom() {
   bool bRet = false;
   LOGD("VHClassRoomImpl::LeaveVHMediaRoom Start", );
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      LOGD("  Class SDK DisConnetWebRtcRoom");
      bRet = mpWebRtcSDKInterface->DisConnetWebRtcRoom();
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      LOGD("  Paas SDK LeaveRTCRoom");
      bRet = mpPaasInteraction->LeaveRTCRoom();
   }
   LOGD("end");

   return bRet;
}

bool VHClassRoomImpl::LeaveClassRoom() {
   LOGD("LeaveClassRoom");
   bool bRet = true;
   if (mpVHWebSocketInterface) {
      bRet = LeaveVHMediaRoom();
      mbDisConnectWebSocket = true;
      bRet = mpVHWebSocketInterface->SyncDisConnectServer();
   }
   LOGD("end");
   return bRet;
}

int VHClassRoomImpl::StartClass(const int& iDefinitionId) {
   LOGD("%s iDefinitionId %d", __FUNCTION__, iDefinitionId);
   if (mClassRoomBaseInfo.mUserInfo.role_type != JOIN_ROLE_Teacher) {
      return ROLE_TYPE_ERROR;
   }
   HTTP_GET_REQUEST httpGet(mURLGenerator.GetStartClassUrl(mCourseInfo.webinar_id, mClassRoomBaseInfo.mUserInfo.join_id, mClassToken,
      mClassRoomBaseInfo.mWebinarInfo.course_type, mClassRoomBaseInfo.mUserInfo.role_type,iDefinitionId, mClassRoomBaseInfo.play_type));
   if (mpHttpManagerInterface) {
      mpHttpManagerInterface->HttpGetRequest(httpGet, [&](std::string data, int code, const std::string userData)->void {
         LOGD("data:%s code:%d", data.c_str(), code);
         if (code != 0) {
            if (mpVHRoomMonitor) {
               LOGD(" data:%s code:%d", data.c_str(), code);
               mpVHRoomMonitor->OnStartClass((int)mClassRoomBaseInfo.play_type, code, data);
            }
         }
         else {
            int nCode;
            std::string strMsg;
            ParamEventToResp(data, nCode, strMsg);
            if (nCode == 200) {
               mClassRoomBaseInfo.mWebinarInfo.type.store((int)CLASS_STATUS_IN_CLASS);
               LOGD("%s mWebinarInfo.type:%d", __FUNCTION__, (int)mClassRoomBaseInfo.mWebinarInfo.type);
               mClassRoomBaseInfo.mWebinarInfo.real_duration = 0;
            }
            mpVHRoomMonitor->OnStartClass((int)mClassRoomBaseInfo.play_type, nCode, strMsg);
         }
      });
   }
   LOGD("end");
   return 0;
}

int VHClassRoomImpl::StopClass(int createDefaultRecord) {
   LOGD("");
   if (mClassRoomBaseInfo.mUserInfo.role_type != JOIN_ROLE_Teacher) {
      return ROLE_TYPE_ERROR;
   }
   HTTP_GET_REQUEST httpGet(mURLGenerator.GetStopClassUrl(mCourseInfo.webinar_id, mClassRoomBaseInfo.mUserInfo.join_id, mClassToken, createDefaultRecord));
   if (mpHttpManagerInterface) {
      mpHttpManagerInterface->HttpGetRequest(httpGet, [&, createDefaultRecord](std::string data, int code, const std::string userData)->void {
         LOGD("data:%s code:%d", data.c_str(), code);
         if (code != 0) {
            if (mpVHRoomMonitor) {
               LOGD(" data:%s code:%d", data.c_str(), code);
               mpVHRoomMonitor->OnStopClass(createDefaultRecord, (int)mClassRoomBaseInfo.play_type, code, data);
            }
         }
         else {
            int nCode;
            std::string strMsg;
            ParamEventToResp(data, nCode, strMsg);
            if (nCode == 200) {
               mClassRoomBaseInfo.mWebinarInfo.type.store((int)CLASS_STATUS_OVER);
               LOGD("%s mWebinarInfo.type:%d", __FUNCTION__, (int)mClassRoomBaseInfo.mWebinarInfo.type);
               mClassRoomBaseInfo.mWebinarInfo.real_duration = 0;
            }
            mpVHRoomMonitor->OnStopClass(createDefaultRecord, (int)mClassRoomBaseInfo.play_type, nCode, strMsg);
         }
      });
   }
   LOGD("end");
   return 0;
}

//int VHClassRoomImpl::syncMixedLaoutMode(int LayoutMode)
//{
//   HTTP_GET_REQUEST httpGet(mURLGenerator.GetOnlineUserUrl(mClassRoomBaseInfo.mUserInfo.join_id, mClassRoomBaseInfo.mWebinarInfo.id, mClassToken, curPage, pageSize));
//   if (mpHttpManagerInterface) {
//      httpGet.SetHttpPost(true);
//      mpHttpManagerInterface->HttpGetRequest(httpGet, [&](std::string data, int code, const std::string userData)->void {
//         LOGD("data:%s code:%d", data.c_str(), code);
//         AnalysisGetOnLineUserInfo(data, code);
//      });
//   }
//
//   return 0;
//}

int VHClassRoomImpl::AsynGetClassRoomMembers(int pageSize, int curPage) {
   //if (mClassRoomBaseInfo.mUserInfo.role_type != JOIN_ROLE_Teacher) {
   //    return ROLE_TYPE_ERROR;
   //}
   if (pageSize > 50) {
      return ROLE_TYPE_REQUEST_SIZE;
   }
   HTTP_GET_REQUEST httpGet(mURLGenerator.GetOnlineUserUrl(mClassRoomBaseInfo.mUserInfo.join_id, mClassRoomBaseInfo.mWebinarInfo.id, mClassToken, curPage, pageSize));
   if (mpHttpManagerInterface) {
      httpGet.SetHttpPost(true);
      mpHttpManagerInterface->HttpGetRequest(httpGet, [&](std::string data, int code, const std::string userData)->void {
         LOGD("data:%s code:%d", data.c_str(), code);
         AnalysisGetOnLineUserInfo(data, code);
      });
   }
   return 0;
}

void VHClassRoomImpl::AnalysisGetOnLineUserInfo(std::string data, int code) {
   if (code != 0) {
      if (mpVHRoomMonitor) {
         LOGD("data:%s code:%d", data.c_str(), code);
         mpVHRoomMonitor->OnCallbackEvent(RoomEvent_OnLineUserList, code, data);
      }
   }
   else {
      ParamToGetOnLineUserInfoResp(data);
   }
}

void VHClassRoomImpl::ParamToGetOnLineUserInfoResp(const std::string& msg) {
   int nCode = RESP_SUCCESS_CODE;
   std::string strNoticeMsg;
   std::list<VHRoomMember> memberList;
   rapidjson::Document doc;
   doc.Parse<0>(msg.c_str());
   ClassOnLineUserInfo onLineUserInfo;
   if (doc.HasParseError()) {
      LOGE("VHTokenRespMsg ParseError%d\n", doc.GetParseError());
      return;
   }
   if (doc.IsObject()) {
      if (doc.HasMember("code") && doc["code"].IsInt()) {
         nCode = doc["code"].GetInt();
      }
      if (doc.HasMember("msg") && doc["msg"].IsString()) {
         strNoticeMsg = doc["msg"].GetString();
      }
      if (RESP_SUCCESS_CODE == nCode) {
         if (doc.HasMember("data") && doc["data"].IsObject()) {
            rapidjson::Value& resp = doc["data"];
            if (resp.HasMember("total") && resp["total"].IsInt()) {
               onLineUserInfo.total = resp["total"].GetInt();
            }
            if (resp.HasMember("total_page") && resp["total_page"].IsInt()) {
               onLineUserInfo.total_page = resp["total_page"].GetInt();
            }
            if (resp.HasMember("curr_page") && resp["curr_page"].IsInt()) {
               onLineUserInfo.curr_page = resp["curr_page"].GetInt();
            }

            if (resp.HasMember("data") && resp["data"].IsArray()) {
               rapidjson::Value& members = resp["data"];
               for (int i = 0; i < members.Size(); i++) {
                  rapidjson::Value& cur_members = members[i];
                  ClassMember vhMember;
                  if (cur_members.HasMember("id") && cur_members["id"].IsString()) {
                     vhMember.user_id = cur_members["id"].GetString();
                  }
                  if (cur_members.HasMember("nick_name") && cur_members["nick_name"].IsString()) {
                     vhMember.nick_name = cur_members["nick_name"].GetString();
                  }
                  if (cur_members.HasMember("status") && cur_members["status"].IsInt()) {
                     vhMember.status = cur_members["status"].GetInt();
                  }
                  if (cur_members.HasMember("role_type") && cur_members["role_type"].IsInt()) {
                     vhMember.role_type = cur_members["role_type"].GetInt();
                  }
                  if (cur_members.HasMember("no_mic") && cur_members["no_mic"].IsInt()) {
                     vhMember.no_mic = cur_members["no_mic"].GetInt();
                  }
                  onLineUserInfo.curr_page_users.push_back(vhMember);
               }
            }
         }
      }
   }

   if (nCode == RESP_SUCCESS_CODE) {
      if (mpVHRoomMonitor) {
         mpVHRoomMonitor->OnGetOnLineUserRoomMembers(onLineUserInfo);
      }
   }
   else {
      if (mpVHRoomMonitor) {
         mpVHRoomMonitor->OnCallbackEvent(RoomEvent_OnLineUserList, nCode, strNoticeMsg);
      }
   }
}

int VHClassRoomImpl::GetSilencedUserList() {
   if (mClassRoomBaseInfo.mUserInfo.role_type != JOIN_ROLE_Teacher) {
      return ROLE_TYPE_ERROR;
   }
   HTTP_GET_REQUEST httpGet(mURLGenerator.GetSilencedUserUrl(mClassRoomBaseInfo.mUserInfo.join_id, mClassRoomBaseInfo.mWebinarInfo.id, mClassToken));
   if (mpHttpManagerInterface) {
      httpGet.SetHttpPost(true);
      mpHttpManagerInterface->HttpGetRequest(httpGet, [&](std::string data, int code, const std::string userData)->void {
         LOGD("data:%s code:%d ", data.c_str(), code);
         AnalysisSilencedUserInfo(data, code);
      });
   }
   return 0;
}


void VHClassRoomImpl::AnalysisSilencedUserInfo(std::string data, int code) {
   if (code != 0) {
      if (mpVHRoomMonitor) {
         LOGD("data:%s code:%d", data.c_str(), code);
         mpVHRoomMonitor->OnCallbackEvent(RoomEvent_GetSilencedList, code, data);
      }
   }
   else {
      ParamToSilencedUserInfoResp(data);
   }
}

void VHClassRoomImpl::ParamToSilencedUserInfoResp(const std::string& msg) {
   int nCode = RESP_SUCCESS_CODE;
   std::string strNoticeMsg;
   std::list<VHRoomMember> memberList;
   rapidjson::Document doc;
   doc.Parse<0>(msg.c_str());
   vlive::ClassOnLineUserInfo SilencedUserInfo;
   if (doc.HasParseError()) {
      LOGE("VHTokenRespMsg ParseError%d\n", doc.GetParseError());
      return;
   }
   if (doc.IsObject()) {
      if (doc.HasMember("code") && doc["code"].IsInt()) {
         nCode = doc["code"].GetInt();
      }
      if (doc.HasMember("msg") && doc["msg"].IsString()) {
         strNoticeMsg = doc["msg"].GetString();
      }
      if (RESP_SUCCESS_CODE == nCode) {
         if (doc.HasMember("data") && doc["data"].IsArray()) {
            rapidjson::Value& members = doc["data"];
            for (int i = 0; i < members.Size(); i++) {
               rapidjson::Value& cur_members = members[i];
               ClassMember vhMember;
               if (cur_members.HasMember("id") && cur_members["id"].IsString()) {
                  vhMember.user_id = cur_members["id"].GetString();
               }
               if (cur_members.HasMember("nick_name") && cur_members["nick_name"].IsString()) {
                  vhMember.nick_name = cur_members["nick_name"].GetString();
               }
               if (cur_members.HasMember("status") && cur_members["status"].IsInt()) {
                  vhMember.status = cur_members["status"].GetInt();
               }
               if (cur_members.HasMember("role_type") && cur_members["role_type"].IsInt()) {
                  vhMember.role_type = cur_members["role_type"].GetInt();
               }
               if (cur_members.HasMember("no_mic") && cur_members["no_mic"].IsInt()) {
                  vhMember.no_mic = cur_members["no_mic"].GetInt();
               }
               SilencedUserInfo.curr_page_users.push_back(vhMember);
            }
         }
      }
   }

   if (nCode == RESP_SUCCESS_CODE) {
      if (mpVHRoomMonitor) {
         mpVHRoomMonitor->OnGetSilencedMembers(SilencedUserInfo);
      }
   }
   else {
      if (mpVHRoomMonitor) {
         mpVHRoomMonitor->OnCallbackEvent(RoomEvent_GetSilencedList, nCode, strNoticeMsg);
      }
   }
}


int VHClassRoomImpl::GetAlreadyHandsUpUserList() {
   //if (mClassRoomBaseInfo.mUserInfo.role_type != JOIN_ROLE_Teacher) {
   //   return ROLE_TYPE_ERROR;
   //}
   HTTP_GET_REQUEST httpGet(mURLGenerator.GetHandsUpUserUrl(mClassRoomBaseInfo.mUserInfo.join_id, mClassRoomBaseInfo.mWebinarInfo.id, mClassToken));
   if (mpHttpManagerInterface) {
      httpGet.SetHttpPost(true);
      mpHttpManagerInterface->HttpGetRequest(httpGet, [&](std::string data, int code, const std::string userData)->void {
         LOGD("data:%s code:%d ", data.c_str(), code);
         AnalysisGetHandsUpUserInfo(data, code);
      });
   }
   return 0;
}

void VHClassRoomImpl::AnalysisGetHandsUpUserInfo(std::string data, int code) {
   if (code != 0) {
      if (mpVHRoomMonitor) {
         LOGD("data:%s code:%d", data.c_str(), code);
         mpVHRoomMonitor->OnCallbackEvent(RoomEvent_GetHandUpList, code, data);
      }
   }
   else {
      ParamToHandsUpUserInfoResp(data);
   }
}

void VHClassRoomImpl::ParamToHandsUpUserInfoResp(const std::string& msg) {
   int nCode = RESP_SUCCESS_CODE;
   std::string strNoticeMsg;
   std::list<VHRoomMember> memberList;
   rapidjson::Document doc;
   doc.Parse<0>(msg.c_str());
   ClassOnLineUserInfo onLineUserInfo;
   if (doc.HasParseError()) {
      LOGE("VHTokenRespMsg ParseError%d\n", doc.GetParseError());
      return;
   }
   if (doc.IsObject()) {
      if (doc.HasMember("code") && doc["code"].IsInt()) {
         nCode = doc["code"].GetInt();
      }
      if (doc.HasMember("msg") && doc["msg"].IsString()) {
         strNoticeMsg = doc["msg"].GetString();
      }
      if (RESP_SUCCESS_CODE == nCode) {
         if (doc.HasMember("data") && doc["data"].IsArray()) {
            rapidjson::Value& members = doc["data"];
            for (int i = 0; i < members.Size(); i++) {
               rapidjson::Value& cur_members = members[i];
               ClassMember vhMember;
               if (cur_members.HasMember("id") && cur_members["id"].IsString()) {
                  vhMember.user_id = cur_members["id"].GetString();
               }
               if (cur_members.HasMember("nick_name") && cur_members["nick_name"].IsString()) {
                  vhMember.nick_name = cur_members["nick_name"].GetString();
               }
               if (cur_members.HasMember("status") && cur_members["status"].IsInt()) {
                  vhMember.status = cur_members["status"].GetInt();
               }
               if (cur_members.HasMember("status") && cur_members["status"].IsString()) {
                  vhMember.status = atoi(cur_members["status"].GetString());
               }
               if (cur_members.HasMember("role_type") && cur_members["role_type"].IsInt()) {
                  vhMember.role_type = cur_members["role_type"].GetInt();
               }
               if (cur_members.HasMember("no_mic") && cur_members["no_mic"].IsInt()) {
                  vhMember.no_mic = cur_members["no_mic"].GetInt();
               }

               if (cur_members.HasMember("is_joinmic") && cur_members["is_joinmic"].IsInt()) {
                  vhMember.is_joinmic = cur_members["is_joinmic"].GetInt();
               }
               else if (cur_members.HasMember("is_joinmic") && cur_members["is_joinmic"].IsString()) {
                  vhMember.is_joinmic = atoi(cur_members["is_joinmic"].GetString());
               }

               onLineUserInfo.curr_page_users.push_back(vhMember);
            }
         }
      }
   }

   if (nCode == RESP_SUCCESS_CODE) {
      if (mpVHRoomMonitor) {
         mpVHRoomMonitor->OnGetHandUpsMembers(onLineUserInfo);
      }
   }
   else {
      if (mpVHRoomMonitor) {
         mpVHRoomMonitor->OnCallbackEvent(RoomEvent_GetHandUpList, nCode, strNoticeMsg);
      }
   }
}

int VHClassRoomImpl::GetKickInavList() {
   LOGD("");
   if (mClassRoomBaseInfo.mUserInfo.role_type != JOIN_ROLE_Teacher) {
      return ROLE_TYPE_ERROR;
   }
   HTTP_GET_REQUEST httpGet(mURLGenerator.GetKickOutUserUrl(mClassRoomBaseInfo.mUserInfo.join_id, mClassRoomBaseInfo.mWebinarInfo.id, mClassToken));
   if (mpHttpManagerInterface) {
      httpGet.SetHttpPost(true);
      mpHttpManagerInterface->HttpGetRequest(httpGet, [&](std::string data, int code, const std::string userData)->void {
         LOGD("data:%s code:%d ", data.c_str(), code);
         AnalysisGetKickOutRoomUserInfo(data, code);
      });
   }
   LOGD("end");
   return 0;
}

void VHClassRoomImpl::AnalysisGetKickOutRoomUserInfo(std::string data, int code) {
   if (code != 0) {
      if (mpVHRoomMonitor) {
         LOGD("data:%s code:%d", data.c_str(), code);
         mpVHRoomMonitor->OnCallbackEvent(RoomEvent_GetKickInavList, code, data);
      }
   }
   else {
      ParamToKickOutRoomUserInfoResp(data);
   }
}
void VHClassRoomImpl::ParamToKickOutRoomUserInfoResp(const std::string& msg) {
   int nCode = RESP_SUCCESS_CODE;
   std::string strNoticeMsg;
   std::list<VHRoomMember> memberList;
   rapidjson::Document doc;
   doc.Parse<0>(msg.c_str());
   ClassOnLineUserInfo onLineUserInfo;
   if (doc.HasParseError()) {
      LOGE("VHTokenRespMsg ParseError%d\n", doc.GetParseError());
      return;
   }
   if (doc.IsObject()) {
      if (doc.HasMember("code") && doc["code"].IsInt()) {
         nCode = doc["code"].GetInt();
      }
      if (doc.HasMember("msg") && doc["msg"].IsString()) {
         strNoticeMsg = doc["msg"].GetString();
      }
      if (RESP_SUCCESS_CODE == nCode) {
         if (doc.HasMember("data") && doc["data"].IsArray()) {
            rapidjson::Value& members = doc["data"];
            for (int i = 0; i < members.Size(); i++) {
               rapidjson::Value& cur_members = members[i];
               ClassMember vhMember;
               if (cur_members.HasMember("id") && cur_members["id"].IsString()) {
                  vhMember.user_id = cur_members["id"].GetString();
               }
               if (cur_members.HasMember("nick_name") && cur_members["nick_name"].IsString()) {
                  vhMember.nick_name = cur_members["nick_name"].GetString();
               }
               if (cur_members.HasMember("status") && cur_members["status"].IsInt()) {
                  vhMember.status = cur_members["status"].GetInt();
               }
               if (cur_members.HasMember("status") && cur_members["status"].IsString()) {
                  vhMember.status = atoi(cur_members["status"].GetString());
               }
               if (cur_members.HasMember("role_type") && cur_members["role_type"].IsInt()) {
                  vhMember.role_type = cur_members["role_type"].GetInt();
               }
               if (cur_members.HasMember("no_mic") && cur_members["no_mic"].IsInt()) {
                  vhMember.no_mic = cur_members["no_mic"].GetInt();
               }
               onLineUserInfo.curr_page_users.push_back(vhMember);
            }
         }
      }
   }

   if (nCode == RESP_SUCCESS_CODE) {
      if (mpVHRoomMonitor) {
         mpVHRoomMonitor->OnGetKickOutRoomMembers(onLineUserInfo);
      }
   }
   else {
      if (mpVHRoomMonitor) {
         mpVHRoomMonitor->OnCallbackEvent(RoomEvent_GetKickInavList, nCode, strNoticeMsg);
      }
   }
}




int VHClassRoomImpl::OpenUserHandsUp(bool open) {
   if (mClassRoomBaseInfo.mUserInfo.role_type != JOIN_ROLE_Teacher) {
      return ROLE_TYPE_ERROR;
   }
   HTTP_GET_REQUEST httpGet(mURLGenerator.GetEnableHandsUpUrl(mClassRoomBaseInfo.mUserInfo.join_id, mClassRoomBaseInfo.mWebinarInfo.id, mClassToken, open == true ? 1 : 0));
   if (mpHttpManagerInterface) {
      httpGet.SetHttpPost(true);
      mpHttpManagerInterface->HttpGetRequest(httpGet, [&, open](std::string data, int code, const std::string userData)->void {
         LOGD("data:%s code:%d", data.c_str(), code);
         if (code != 0) {
            if (mpVHRoomMonitor) {
               LOGD(" data:%s code:%d", data.c_str(), code);
               mpVHRoomMonitor->OnOpenHandsUp(open, code, data);
            }
         }
         else {
            int nCode;
            std::string strMsg;
            ParamEventToResp(data, nCode, strMsg);
            mpVHRoomMonitor->OnOpenHandsUp(open, nCode, strMsg);
         }
      });
   }
   return 0;
}

int VHClassRoomImpl::CanclePublish()
{
   HTTP_GET_REQUEST httpGet(mURLGenerator.GetCancelHandUrl(mClassRoomBaseInfo.mWebinarInfo.id, mClassRoomBaseInfo.mUserInfo.join_id, mClassToken));

   if (mpHttpManagerInterface) {
      httpGet.SetHttpPost(true);
      mpHttpManagerInterface->HttpGetRequest(httpGet, [&](std::string data, int code, const std::string userData)->void {
         LOGD("data:%s code:%d ", data.c_str(), code);
         if (code != 0) {
            if (mpVHRoomMonitor) {
               LOGD(" data:%s code:%d", data.c_str(), code);
               mpVHRoomMonitor->CanclePublish(code, data);
            }
         }
         else {
            int nCode;
            std::string strMsg;
            ParamEventToResp(data, nCode, strMsg);
            mpVHRoomMonitor->CanclePublish(nCode, strMsg);
         }
      });
   }

   return VhallLive_OK;
}

int VHClassRoomImpl::RewardStudent(const std::string& targetId)
{
   HTTP_GET_REQUEST httpGet(mURLGenerator.RewardStudent(mClassRoomBaseInfo.mUserInfo.join_id, targetId, mClassToken));

   if (mpHttpManagerInterface) {
      //httpGet.SetHttpPost(true);
      mpHttpManagerInterface->HttpGetRequest(httpGet, [&](std::string data, int code, const std::string userData)->void {
         LOGD("data:%s code:%d ", data.c_str(), code);

         //{"code":400,"msg":"\u64cd\u4f5c\u592a\u9891\u7e41\uff01","data":[]}
         LOGD(" data:%s code:%d", data.c_str(), code);

         rapidjson::Document doc;
         doc.Parse<0>(data.c_str());
         if (doc.HasParseError()) {
            LOGE("VHTokenRespMsg ParseError%d\n", doc.GetParseError());
            return;
         }
         if (doc.IsObject()) {
            int iCode = 0;
            if (doc.HasMember("code") && doc["code"].IsString()) {
               std::string strCode = doc["code"].GetString();
               iCode = atoi(strCode.c_str());
            }
            else if (doc.HasMember("code") && doc["code"].IsInt()) {
               iCode = doc["code"].GetInt();
            }

            if (200 != iCode && doc.HasMember("msg") && doc["msg"].IsString()) {
               std::string inav_event = doc["msg"].GetString();
               if (mpVHRoomMonitor) {
                  mpVHRoomMonitor->RewardError(code, inav_event);
               }

            }

         }


      });
   }

   return VhallLive_OK;
}

int VHClassRoomImpl::CheckUserIsPublish(const std::string& user_id) {
   HTTP_GET_REQUEST httpGet(mURLGenerator.GetPublishListUrl(mClassRoomBaseInfo.mUserInfo.join_id, mCourseInfo.webinar_id, mClassToken));
   if (mpHttpManagerInterface) {
      httpGet.SetHttpPost(true);
      mpHttpManagerInterface->HttpGetRequest(httpGet, [&, user_id](std::string data, int code, const std::string userData)->void {
         LOGD("data:%s code:%d ", data.c_str(), code);
         if (code != 0) {
            if (mpVHRoomMonitor) {
               LOGD(" data:%s code:%d", data.c_str(), code);
               mpVHRoomMonitor->OnCheckUserIsPublish(user_id, code, data);
            }
         }
         else {
            int nCode;
            std::string strMsg;
            bool is_publish = IsUserPublish(user_id, data, nCode, strMsg);
            if (nCode == 200) {
               if (is_publish) {
                  nCode = USER_IS_ALREADY_PUBLISH;
                  strMsg = "user is publishing stream";
               }
               else {
                  nCode = USER_NOT_PUBLISH;
                  strMsg = "user not publish stream";
               }
            }
            mpVHRoomMonitor->OnCheckUserIsPublish(user_id, nCode, strMsg);
         }
      });
   }
   return VhallLive_OK;
}

int VHClassRoomImpl::ApplyPublish() {
   rapidjson::Document jsonDoc;
   rapidjson::Document::AllocatorType &allocator = jsonDoc.GetAllocator();

   rapidjson::Value userIdJson(rapidjson::kObjectType);
   userIdJson.AddMember("join_id", rapidjson::StringRef(mClassRoomBaseInfo.mUserInfo.join_id.c_str()), allocator);
   userIdJson.AddMember("nick_name", rapidjson::StringRef(mClassRoomBaseInfo.mUserInfo.nick_name.c_str()), allocator);

   rapidjson::Value CmdRoot(rapidjson::kObjectType);
   rapidjson::Value content(StringRef("*hand"));
   CmdRoot.AddMember("type", content, allocator);
   CmdRoot.AddMember("userID", userIdJson, allocator);

   StringBuffer buffer;
   rapidjson::Writer<StringBuffer> writer(buffer);
   CmdRoot.Accept(writer);
   std::string data = buffer.GetString();

   HTTP_GET_REQUEST httpGet(mURLGenerator.GetSendNoticeUrl(mMessageInfo.token, "cmd", data));
   if (mpHttpManagerInterface) {
      httpGet.SetHttpPost(true);
      mpHttpManagerInterface->HttpGetRequest(httpGet, [&](std::string data, int code, const std::string userData)->void {
         LOGD("data:%s code:%d ", data.c_str(), code);
         if (code != 0) {
            if (mpVHRoomMonitor) {
               LOGD(" data:%s code:%d", data.c_str(), code);
               mpVHRoomMonitor->OnApplyPublish(code, data);
            }
         }
         else {
            int nCode;
            std::string strMsg;
            ParamEventToResp(data, nCode, strMsg);
            mpVHRoomMonitor->OnApplyPublish(nCode, strMsg);
         }
      });
   }
   return VhallLive_OK;
}

int VHClassRoomImpl::AuditPublish(const std::string& audit_user_id) {
   LOGD("%s AuditPublish audit_user_id:%s ", __FUNCTION__, audit_user_id.c_str());
   if (mClassRoomBaseInfo.mUserInfo.role_type != JOIN_ROLE_Teacher) {
      return ROLE_TYPE_ERROR;
   }
   HTTP_GET_REQUEST httpGet(mURLGenerator.GetCheckJoinMics(mClassRoomBaseInfo.mUserInfo.join_id, mClassRoomBaseInfo.mWebinarInfo.id, mClassToken, 1, atoi(audit_user_id.c_str())));
   if (mpHttpManagerInterface) {
      httpGet.SetHttpPost(true);
      mpHttpManagerInterface->HttpGetRequest(httpGet, [&, audit_user_id](std::string data, int code, const std::string userData)->void {
         LOGD("data:%s code:%d  userId:%s ", data.c_str(), code, audit_user_id.c_str());
         if (code != 0) {
            if (mpVHRoomMonitor) {
               LOGD(" data:%s code:%d", data.c_str(), code);
               mpVHRoomMonitor->OnAuditPublish(audit_user_id, code, data);
            }
         }
         else {
            //解析课堂基本数据
            int nCode;
            std::string strMsg;
            ParamEventToResp(data, nCode, strMsg);
            mpVHRoomMonitor->OnAuditPublish(audit_user_id, nCode, strMsg);
         }

      });
   }
   return VhallLive_OK;
}

int VHClassRoomImpl::AskforPublish(const std::string& audit_user_id) {
   LOGD("%s AskforPublish audit_user_id:%s ",  __FUNCTION__,audit_user_id.c_str());
   if (mClassRoomBaseInfo.mUserInfo.role_type != JOIN_ROLE_Teacher) {
      return ROLE_TYPE_ERROR;
   }
   HTTP_GET_REQUEST httpGet(mURLGenerator.GetCheckJoinMics(mClassRoomBaseInfo.mUserInfo.join_id, mClassRoomBaseInfo.mWebinarInfo.id, mClassToken, 1, atoi(audit_user_id.c_str())));
   if (mpHttpManagerInterface) {
      httpGet.SetHttpPost(true);
      mpHttpManagerInterface->HttpGetRequest(httpGet, [&, audit_user_id](std::string data, int code, const std::string userData)->void {
         LOGD("data:%s code:%d  userId:%s ", data.c_str(), code, audit_user_id.c_str());
         if (code != 0) {
            if (mpVHRoomMonitor) {
               LOGD(" data:%s code:%d", data.c_str(), code);
               mpVHRoomMonitor->OnAskforPublish(audit_user_id, code, data);
            }
         }
         else {
            //解析课堂基本数据
            int nCode;
            std::string strMsg;
            ParamEventToResp(data, nCode, strMsg);
            mpVHRoomMonitor->OnAskforPublish(audit_user_id, nCode, strMsg);
         }
      });
   }
   return VhallLive_OK;
}

int VHClassRoomImpl::UserPublishCallback(ClassPushStreamEvent type, std::string target_id /*= std::string()*/) {
   
   LOGD("%s   id : %s UserPublishCallback type:%d", __FUNCTION__, mClassRoomBaseInfo.mUserInfo.join_id.c_str(),type);
   switch (type)
   {
   case vlive::PushStreamEvent_UnPublish:
   case vlive::PushStreamEvent_Publish: {
      std::string targetId = target_id;
      if (targetId.empty()) {
         targetId = mClassRoomBaseInfo.mUserInfo.join_id;
      }

      LOGD("%s   id : %s UserPublishCallback type:%d  targetId %s", 
         __FUNCTION__, mClassRoomBaseInfo.mUserInfo.join_id.c_str(), type, targetId.c_str());

      HTTP_GET_REQUEST httpGet(mURLGenerator.GetStartPublishUrl(mClassRoomBaseInfo.mUserInfo.join_id, mClassRoomBaseInfo.mWebinarInfo.id, mClassToken, type, targetId));
      if (mpHttpManagerInterface) {
         httpGet.SetHttpPost(true);
         mpHttpManagerInterface->HttpGetRequest(httpGet, [&, type, targetId](std::string data, int code, const std::string userData)->void {
            LOGD("data:%s code:%d  type:%d ", data.c_str(), code, type);
            if (code != 0) {
               if (mpVHRoomMonitor) {
                  LOGD(" data:%s code:%d", data.c_str(), code);
                  mpVHRoomMonitor->OnUserPublishCallback(type, targetId, code, data);
               }
            }
            else {
               int nCode;
               std::string strMsg;
               ParamEventToResp(data, nCode, strMsg);
               mpVHRoomMonitor->OnUserPublishCallback(type, targetId, nCode, strMsg);
            }
         });
      }
   }
                                        break;
   case vlive::PushStreamEvent_Refused:
      break;
   default:
      break;
   }
   return VhallLive_OK;

}

int VHClassRoomImpl::UserPublishFailedCallback(ClassPushStreamEvent type) {
   LOGD("UserPublishFailedCallback type:%d", type);
   switch (type)
   {
   case vlive::PushStreamEvent_UnPublish:
   case vlive::PushStreamEvent_Publish: {
      HTTP_GET_REQUEST httpGet(mURLGenerator.GetPublishCallbackUrl(mClassRoomBaseInfo.mUserInfo.join_id, mClassRoomBaseInfo.mWebinarInfo.id, mClassToken, type, mClassRoomBaseInfo.mUserInfo.join_id));
      if (mpHttpManagerInterface) {
         httpGet.SetHttpPost(true);
         mpHttpManagerInterface->HttpGetRequest(httpGet, [&, type](std::string data, int code, const std::string userData)->void {
            LOGD("data:%s code:%d  type:%d ", data.c_str(), code, type);
            if (code != 0) {
               if (mpVHRoomMonitor) {
                  LOGD(" data:%s code:%d", data.c_str(), code);
                  mpVHRoomMonitor->OnUserPublishFailedCallback(type, code, data);
               }
            }
            else {
               int nCode;
               std::string strMsg;
               ParamEventToResp(data, nCode, strMsg);
               mpVHRoomMonitor->OnUserPublishFailedCallback(type, nCode, strMsg);
            }
         });
      }
   }
                                        break;
   case vlive::PushStreamEvent_Refused:
      break;
   default:
      break;
   }
   return VhallLive_OK;

}

int VHClassRoomImpl::UpdatePublishAbility(const bool& enablePublish, const bool& enableDoublePublish) {
   //是否无上麦能力 1 无 0有
   int no_mic = enablePublish == true ? 0 : 1;
   int iEnablePub = enableDoublePublish == true ? 0 : 1;
   HTTP_GET_REQUEST httpGet(mURLGenerator.GetUpdatePublishAbility(mClassRoomBaseInfo.mUserInfo.join_id,
      mClassRoomBaseInfo.mWebinarInfo.id, mClassToken, no_mic, iEnablePub));
   if (mpHttpManagerInterface) {
      httpGet.SetHttpPost(true);
      mpHttpManagerInterface->HttpGetRequest(httpGet, [&, no_mic](std::string data, int code, const std::string userData)->void {
         LOGD("data:%s code:%d  enablePublish:%d ", data.c_str(), code, no_mic);
         if (code != 0) {
            if (mpVHRoomMonitor) {
               mpVHRoomMonitor->OnUpdatePublishAbility(enablePublish ? 1 : 0, enableDoublePublish ? 1 : 0, code, data);
            }
         }
         else {
            int nCode;
            std::string strMsg;
            ParamEventToResp(data, nCode, strMsg);
            mpVHRoomMonitor->OnUpdatePublishAbility(enablePublish ? 1 : 0, enableDoublePublish ? 1 : 0, nCode, strMsg);
         }
      });
   }
   return 0;
}


int VHClassRoomImpl::SendMicsDevice(const std::string &audio_id, const std::string& camera_id, const std::string& double_camera_id, const int roleType)
{
   HTTP_GET_REQUEST httpGet(mURLGenerator.GetSendMicsDeviceUrl(mClassRoomBaseInfo.mUserInfo.join_id, mClassRoomBaseInfo.mWebinarInfo.id, mClassToken,
      audio_id, camera_id, double_camera_id, roleType));
   if (mpHttpManagerInterface) {
      httpGet.SetHttpPost(true);
      mpHttpManagerInterface->HttpGetRequest(httpGet, [&](std::string data, int code, const std::string userData)->void {
         LOGD("data:%s code:%d  enablePublish:%d ", data.c_str(), code);
         if (code != 0) {
            if (mpVHRoomMonitor) {
               mpVHRoomMonitor->OnSendMicsDeviceCallback(code, data);
            }
         }
         else {
            int nCode;
            std::string strMsg;
            ParamEventToResp(data, nCode, strMsg);
				if (nCode != 200) {
					mpVHRoomMonitor->OnInviteDoubleVrtcCallback(nCode, strMsg);
				}
         }
      });
   }
   return 0;
}


int VHClassRoomImpl::inviteDoubleVrtc(const std::string &target_id)
{
   HTTP_GET_REQUEST httpGet(mURLGenerator.GetInviteDoubleVrtcUrl(mClassRoomBaseInfo.mUserInfo.join_id, mClassRoomBaseInfo.mWebinarInfo.id, mClassToken,
      target_id));
   if (mpHttpManagerInterface) {
      httpGet.SetHttpPost(true);
      mpHttpManagerInterface->HttpGetRequest(httpGet, [&](std::string data, int code, const std::string userData)->void {
         LOGD("data:%s code:%d  enablePublish:%d ", data.c_str(), code);
         if (code != 0) {
            if (mpVHRoomMonitor) {
               mpVHRoomMonitor->OnInviteDoubleVrtcCallback(code, data);
            }
         }
         else {
            int nCode;
            std::string strMsg;
            ParamEventToResp(data, nCode, strMsg);
				if (nCode != 200) {
					mpVHRoomMonitor->OnInviteDoubleVrtcCallback(nCode, strMsg);
				}         
         }
      });
   }
   return 0;
}

int VHClassRoomImpl::inviteDesktopVrtc(const std::string &target_id)
{
   HTTP_GET_REQUEST httpGet(mURLGenerator.GetInviteDesktopVrtcUrl(mClassRoomBaseInfo.mUserInfo.join_id, mClassRoomBaseInfo.mWebinarInfo.id, mClassToken,
      target_id));
   if (mpHttpManagerInterface) {
      httpGet.SetHttpPost(true);
      mpHttpManagerInterface->HttpGetRequest(httpGet, [&](std::string data, int code, const std::string userData)->void {
         LOGD("data:%s code:%d  enablePublish:%d ", data.c_str(), code);
         if (code != 0) {
            if (mpVHRoomMonitor) {
               mpVHRoomMonitor->OnInviteDoubleVrtcCallback(code, data);
            }
         }
         else {
            int nCode;
            std::string strMsg;
            ParamEventToResp(data, nCode, strMsg);
				if (nCode != 200) {
					mpVHRoomMonitor->OnInviteDoubleVrtcCallback(nCode, strMsg);
				}
         }
      });
   }
   return 0;
}

int VHClassRoomImpl::startDoubleVrtc()
{
   LOGD("%s Enter ", __FUNCTION__);
   HTTP_GET_REQUEST httpGet(mURLGenerator.GetStartDoubleVrtcUrl(mClassRoomBaseInfo.mUserInfo.join_id, mClassRoomBaseInfo.mWebinarInfo.id,
      mClassToken));
   if (mpHttpManagerInterface) {
      httpGet.SetHttpPost(true);
      mpHttpManagerInterface->HttpGetRequest(httpGet, [&](std::string data, int code, const std::string userData)->void {
         LOGD("%s data:%s code:%d  enablePublish:%d ", __FUNCTION__, data.c_str(), code);
         if (code != 0) {
            if (mpVHRoomMonitor) {
               mpVHRoomMonitor->OnStartDoubleVrtcCallback(code, data);
            }
         }
         else {
            int nCode;
            std::string strMsg;
            ParamEventToResp(data, nCode, strMsg);
            //if (nCode != 200) {
            mpVHRoomMonitor->OnStartDoubleVrtcCallback(nCode, strMsg);
            //}
    //        else{
    //           mpVHRoomMonitor->OnStartDoubleVrtcCallback(nCode, strMsg);
    //     }
         }
      });
   }
   LOGD("%s Leave ", __FUNCTION__);
   return 0;
}

int VHClassRoomImpl::stopDoubleVrtc(const std::string &target_id)
{
   HTTP_GET_REQUEST httpGet(mURLGenerator.GetStopDoubleVrtcUrl(target_id, mClassRoomBaseInfo.mUserInfo.join_id, mClassRoomBaseInfo.mWebinarInfo.id,
      mClassToken));
   if (mpHttpManagerInterface) {
      httpGet.SetHttpPost(true);
      mpHttpManagerInterface->HttpGetRequest(httpGet, [&](std::string data, int code, const std::string userData)->void {
         LOGD("data:%s code:%d  enablePublish:%d ", data.c_str(), code);
         if (code != 0) {
            if (mpVHRoomMonitor) {
               mpVHRoomMonitor->OnInviteDoubleVrtcCallback(code, data);
            }
         }
         else {
            int nCode;
            std::string strMsg;
            ParamEventToResp(data, nCode, strMsg);
				if (nCode != 200) {
					mpVHRoomMonitor->OnInviteDoubleVrtcCallback(nCode, strMsg);
				}
				else {
					mpVHRoomMonitor->OnStopDoubleVrtcCallback(nCode, strMsg);
				}		
         }
      });
   }
   return 0;
}

int VHClassRoomImpl::stopDesktopSharing(const std::string &target_id)
{
   HTTP_GET_REQUEST httpGet(mURLGenerator.GetStopDesktopSharingUrl(mClassRoomBaseInfo.mUserInfo.join_id, mClassRoomBaseInfo.mWebinarInfo.id,
      mClassToken, target_id));
   if (mpHttpManagerInterface) {
      httpGet.SetHttpPost(true);
      mpHttpManagerInterface->HttpGetRequest(httpGet, [&](std::string data, int code, const std::string userData)->void {
         LOGD("data:%s code:%d  enablePublish:%d ", data.c_str(), code);
         if (code != 0) {
            if (mpVHRoomMonitor) {
               mpVHRoomMonitor->OnInviteDoubleVrtcCallback(code, data);
            }
         }
         else {
            int nCode;
            std::string strMsg;
            ParamEventToResp(data, nCode, strMsg);
				if (nCode != 200) {
					mpVHRoomMonitor->OnInviteDoubleVrtcCallback(nCode, strMsg);
				}
				else {
					mpVHRoomMonitor->OnStopDoubleVrtcCallback(nCode, strMsg);
				}

         }
      });
   }
   return 0;
}

int VHClassRoomImpl::doubleVrtcUsers()
{
   HTTP_GET_REQUEST httpGet(mURLGenerator.GetDoubleVrtcUsersUrl(mClassRoomBaseInfo.mUserInfo.join_id, mClassRoomBaseInfo.mWebinarInfo.id,
      mClassToken));
   if (mpHttpManagerInterface) {
      httpGet.SetHttpPost(true);
      mpHttpManagerInterface->HttpGetRequest(httpGet, [&](std::string data, int code, const std::string userData)->void {
         LOGD("data:%s code:%d  enablePublish:%d ", data.c_str(), code);
         if (code != 0) {
            if (mpVHRoomMonitor) {
               mpVHRoomMonitor->OnDoubleVrtcUsersCallback(code, data);
            }
         }
         else {
            ParamToGetDoubleVrtcUsers(data);
            //mpVHRoomMonitor->OnDoubleVrtcUsersCallback(nCode, strMsg);
         }
      });
   }
   return 0;
}

void VHClassRoomImpl::ParamToGetDoubleVrtcUsers(const std::string& msg)
{
   int nCode = RESP_SUCCESS_CODE;
   std::string strNoticeMsg;

   rapidjson::Document doc;
   doc.Parse<0>(msg.c_str());
   std::list<ClassDoubleVrtcUsers> curr_page_users;

   if (doc.HasParseError()) {
      LOGE("VHTokenRespMsg ParseError%d\n", doc.GetParseError());
      return;
   }

   if (doc.IsObject()) {
      if (doc.HasMember("code") && doc["code"].IsInt()) {
         nCode = doc["code"].GetInt();
      }

      if (doc.HasMember("msg") && doc["msg"].IsString()) {
         strNoticeMsg = doc["msg"].GetString();
      }

      if (RESP_SUCCESS_CODE == nCode) {
         //{"code":200, "data" : [{"id":"88365", "webinar_id" : "edu_65d66e32", "name" : "", "nick_name" : "\u5b89\u5168\u536b\u58eb", "no_double_vrtc" : 0}], "msg" : "success"}
 /*        if (doc.HasMember("data") && doc["data"].IsObject()) {
            rapidjson::Value& resp = doc["data"];
*/

            if (doc.HasMember("data") && doc["data"].IsArray()) {
               rapidjson::Value& members = doc["data"];

               for (int i = 0; i < members.Size(); i++) {
                  rapidjson::Value& cur_members = members[i];

                  std::string webinar_id;
                  if (cur_members.HasMember("webinar_id") && cur_members["webinar_id"].IsString()) {
                     webinar_id = cur_members["webinar_id"].GetString();

                     ClassRoomBaseInfo baseInfo = GetClassRoomBaseInfo();
                     if (webinar_id.compare(baseInfo.mWebinarInfo.id) == 0) {
                        ClassDoubleVrtcUsers vhMember;

                        if (cur_members.HasMember("id") && cur_members["id"].IsString()) {
                           vhMember.user_id = cur_members["id"].GetString();
                        }
                        if (cur_members.HasMember("nick_name") && cur_members["nick_name"].IsString()) {
                           vhMember.nick_name = cur_members["nick_name"].GetString();
                        }

                        if (cur_members.HasMember("no_double_vrtc") && cur_members["no_double_vrtc"].IsInt()) {
                           vhMember.no_double_vrtc = cur_members["no_double_vrtc"].GetInt();
                        }
                        else if (cur_members.HasMember("no_double_vrtc") && cur_members["no_double_vrtc"].IsString()) {
                           vhMember.no_double_vrtc = atoi(cur_members["no_double_vrtc"].GetString());
                        }


                        curr_page_users.push_back(vhMember);
                     }

                  }


               }
            }
        //end “data”
      }
   }

   if (nCode == RESP_SUCCESS_CODE) {
      if (mpVHRoomMonitor) {
         mpVHRoomMonitor->OnGetDoubleVrtcUsers(curr_page_users);
      }
   }
   else {
      if (mpVHRoomMonitor) {
         mpVHRoomMonitor->OnDoubleVrtcUsersCallback(nCode, strNoticeMsg);
      }
   }
}

int VHClassRoomImpl::KickUserPublish(const std::string& kick_user_id) {
   LOGD("%s KickUserPublish kick_user_id:%s ", __FUNCTION__,kick_user_id.c_str());
   if (mClassRoomBaseInfo.mUserInfo.role_type != JOIN_ROLE_Teacher) {
      return ROLE_TYPE_ERROR;
   }
   HTTP_GET_REQUEST httpGet(mURLGenerator.GetCheckJoinMics(mClassRoomBaseInfo.mUserInfo.join_id, mClassRoomBaseInfo.mWebinarInfo.id, mClassToken, 0, atoi(kick_user_id.c_str())));
   if (mpHttpManagerInterface) {
      httpGet.SetHttpPost(true);
      mpHttpManagerInterface->HttpGetRequest(httpGet, [&, kick_user_id](std::string data, int code, const std::string userData)->void {
         LOGD("data:%s code:%d  userId:%s ", data.c_str(), code, kick_user_id.c_str());
         if (code != 0) {
            if (mpVHRoomMonitor) {
               mpVHRoomMonitor->OnKickUserPublish(kick_user_id, code, data);
            }
         }
         else {
            //解析课堂基本数据
            int nCode;
            std::string strMsg;
            ParamEventToResp(data, nCode, strMsg);
            mpVHRoomMonitor->OnKickUserPublish(kick_user_id, nCode, strMsg);
         }
      });
   }
   return VhallLive_OK;
}

int VHClassRoomImpl::KickAllUnPublish() {
   LOGD("");
   if (mClassRoomBaseInfo.mUserInfo.role_type != JOIN_ROLE_Teacher) {
      return ROLE_TYPE_ERROR;
   }
   HTTP_GET_REQUEST httpGet(mURLGenerator.GetKickAllUserUnPublishUrl(mClassRoomBaseInfo.mWebinarInfo.id, mClassRoomBaseInfo.mUserInfo.join_id, mClassToken));
   if (mpHttpManagerInterface) {
      httpGet.SetHttpPost(true);
      mpHttpManagerInterface->HttpGetRequest(httpGet, [&](std::string data, int code, const std::string userData)->void {
         LOGD("data:%s code:%d", data.c_str(), code);
         if (code != 0) {
            if (mpVHRoomMonitor) {
               mpVHRoomMonitor->OnKickAllUnPublish(code, data);
            }
         }
         else {
            //解析课堂基本数据
            int nCode;
            std::string strMsg;
            ParamEventToResp(data, nCode, strMsg);
            mpVHRoomMonitor->OnKickAllUnPublish(nCode, strMsg);
         }
      });
   }
   return VhallLive_OK;
}

int VHClassRoomImpl::KickOutClassRoom(const std::string& kick_user_id, int type) {
   LOGD("KickOutClassRoom kick_user_id:%s type:%d", kick_user_id.c_str(), type);
   if (mClassRoomBaseInfo.mUserInfo.role_type != JOIN_ROLE_Teacher) {
      return ROLE_TYPE_ERROR;
   }

   HTTP_GET_REQUEST httpGet(mURLGenerator.GetKickOutRoomUrl(mClassRoomBaseInfo.mWebinarInfo.id, mClassRoomBaseInfo.mUserInfo.join_id, type, mClassToken, kick_user_id));
   if (mpHttpManagerInterface) {
      httpGet.SetHttpPost(true);
      mpHttpManagerInterface->HttpGetRequest(httpGet, [&, type, kick_user_id](std::string data, int code, const std::string userData)->void {
         LOGD("data:%s code:%d ", data.c_str(), code);
         if (code != 0) {
            if (mpVHRoomMonitor) {
               LOGD(" data:%s code:%d", data.c_str(), code);
               mpVHRoomMonitor->OnKickOutClassRoom(kick_user_id, type, code, data);
            }
         }
         else {
            int nCode;
            std::string strMsg;
            ParamEventToResp(data, nCode, strMsg);
            mpVHRoomMonitor->OnKickOutClassRoom(kick_user_id, type, nCode, strMsg);
         }
      });
   }

   return 0;
}

int VHClassRoomImpl::ForbiddenChat(std::string userId, bool forbid) {
   LOGD("userid:%s auth:%d", userId.c_str(), forbid);
   if (mClassRoomBaseInfo.mUserInfo.role_type != JOIN_ROLE_Teacher) {
      return ROLE_TYPE_ERROR;
   }

   HTTP_GET_REQUEST httpGet(mURLGenerator.GetForbidChatUrl(mClassRoomBaseInfo.mWebinarInfo.id, mClassRoomBaseInfo.mUserInfo.join_id, forbid == true ? 1 : 0, mClassToken, userId));
   if (mpHttpManagerInterface) {
      httpGet.SetHttpPost(true);
      mpHttpManagerInterface->HttpGetRequest(httpGet, [&, userId, forbid](std::string data, int code, const std::string userData)->void {
         LOGD("data:%s code:%d ", data.c_str(), code);
         if (code != 0) {
            if (mpVHRoomMonitor) {
               LOGD(" data:%s code:%d", data.c_str(), code);
               mpVHRoomMonitor->OnForbiddenChat(userId, forbid, code, data);
            }
         }
         else {
            int nCode;
            std::string strMsg;
            ParamEventToResp(data, nCode, strMsg);
            mpVHRoomMonitor->OnForbiddenChat(userId, forbid, nCode, strMsg);
         }
      });
   }
   return 0;
}


int VHClassRoomImpl::ForbiddenChatAll(bool forbid) {
   LOGD("ForbiddenChatAll forbid:%d", forbid);
   if (mClassRoomBaseInfo.mUserInfo.role_type != JOIN_ROLE_Teacher) {
      return ROLE_TYPE_ERROR;
   }
   HTTP_GET_REQUEST httpGet(mURLGenerator.GetForbidChatAllUrl(mClassRoomBaseInfo.mWebinarInfo.id, mClassRoomBaseInfo.mUserInfo.join_id, forbid == true ? 1 : 0, mClassToken));
   if (mpHttpManagerInterface) {
      httpGet.SetHttpPost(true);
      mpHttpManagerInterface->HttpGetRequest(httpGet, [&, forbid](std::string data, int code, const std::string userData)->void {
         LOGD("data:%s code:%d  forbid:%d", data.c_str(), code, forbid);
         if (code != 0) {
            if (mpVHRoomMonitor) {
               LOGD(" data:%s code:%d", data.c_str(), code);
               mpVHRoomMonitor->OnForbiddenChatAll(forbid, code, data);
            }
         }
         else {
            int nCode;
            std::string strMsg;
            ParamEventToResp(data, nCode, strMsg);
            mpVHRoomMonitor->OnForbiddenChatAll(forbid, nCode, strMsg);
         }
      });
   }
   return 0;
}




int VHClassRoomImpl::OperateUserMic(std::string userId, bool close) {
   HTTP_GET_REQUEST httpGet(mURLGenerator.GetMuteAudioUrl(mClassRoomBaseInfo.mWebinarInfo.id, mClassRoomBaseInfo.mUserInfo.join_id, close == true ? 1 : 0, mClassToken, userId));
   if (mpHttpManagerInterface) {
      httpGet.SetHttpPost(true);
      mpHttpManagerInterface->HttpGetRequest(httpGet, [&, close, userId](std::string data, int code, const std::string userData)->void {
         LOGD("data:%s code:%d userId:%s open:%d", data.c_str(), code, userId.c_str(), (int)close);
         if (code != 0) {
            if (mpVHRoomMonitor) {
               LOGD(" data:%s code:%d", data.c_str(), code);
               mpVHRoomMonitor->OnOperateUserMic(userId, close, code, data);
            }
         }
         else {
            int nCode;
            std::string strMsg;
            ParamEventToResp(data, nCode, strMsg);
            mpVHRoomMonitor->OnOperateUserMic(userId, close, nCode, strMsg);
         }
      });
   }
   return 0;
}

int VHClassRoomImpl::OperateAllUserMic(int mute) {
   if (mClassRoomBaseInfo.mUserInfo.role_type != JOIN_ROLE_Teacher) {
      return ROLE_TYPE_ERROR;
   }

   HTTP_GET_REQUEST httpGet(mURLGenerator.GetMuteAudioAllUrl(mClassRoomBaseInfo.mWebinarInfo.id, mClassRoomBaseInfo.mUserInfo.join_id, mute, mClassToken));
   if (mpHttpManagerInterface) {
      httpGet.SetHttpPost(true);
      mpHttpManagerInterface->HttpGetRequest(httpGet, [&, mute](std::string data, int code, const std::string userData)->void {
         LOGD("data:%s code:%d open:%d", data.c_str(), code, mute);
         if (code != 0) {
            if (mpVHRoomMonitor) {
               LOGD(" data:%s code:%d", data.c_str(), code);
               mpVHRoomMonitor->OnOperateAllUserMic(mute, code, data);
            }
         }
         else {
            int nCode;
            std::string strMsg;
            ParamEventToResp(data, nCode, strMsg);
            mpVHRoomMonitor->OnOperateAllUserMic(mute, nCode, strMsg);
            mClassRoomBaseInfo.mutingAll = mute;
         }
      });
   }
   return 0;
}

int VHClassRoomImpl::OperateUserCamera(std::string userId, bool close) {
   HTTP_GET_REQUEST httpGet(mURLGenerator.GetMuteVideoUrl(mClassRoomBaseInfo.mWebinarInfo.id, mClassRoomBaseInfo.mUserInfo.join_id, close == true ? 1 : 0, mClassToken, userId));
   if (mpHttpManagerInterface) {
      httpGet.SetHttpPost(true);
      mpHttpManagerInterface->HttpGetRequest(httpGet, [&, close, userId](std::string data, int code, const std::string userData)->void {
         LOGD("data:%s code:%d userId:%s open:%d", data.c_str(), code, userId.c_str(), (int)close);
         if (code != 0) {
            if (mpVHRoomMonitor) {
               LOGD(" data:%s code:%d", data.c_str(), code);
               mpVHRoomMonitor->OnOperateUserCamera(userId, close, code, data);
            }
         }
         else {
            int nCode;
            std::string strMsg;
            ParamEventToResp(data, nCode, strMsg);
            mpVHRoomMonitor->OnOperateUserCamera(userId, close, nCode, strMsg);
         }
      });
   }
   return 0;
}

int VHClassRoomImpl::ChangeUserPenAuth(std::string userId, bool auth) {
   LOGD("userid:%s auth:%d", userId.c_str(), auth);
   if (mClassRoomBaseInfo.mUserInfo.role_type != JOIN_ROLE_Teacher) {
      return ROLE_TYPE_ERROR;
   }
   HTTP_GET_REQUEST httpGet(mURLGenerator.GetSetBrushUrl(mClassRoomBaseInfo.mWebinarInfo.id, mClassRoomBaseInfo.mUserInfo.join_id, auth == true ? 1 : 0, mClassToken, userId));
   if (mpHttpManagerInterface) {
      httpGet.SetHttpPost(true);
      mpHttpManagerInterface->HttpGetRequest(httpGet, [&, userId, auth](std::string data, int code, const std::string userData)->void {
         LOGD("data:%s code:%d  userId:%s open:%d", data.c_str(), code, userId.c_str(), auth);
         if (code != 0) {
            if (mpVHRoomMonitor) {
               LOGD(" data:%s code:%d", data.c_str(), code);
               mpVHRoomMonitor->OnChangeUserPenAuth(userId, auth, code, data);
            }
         }
         else {
            int nCode;
            std::string strMsg;
            ParamEventToResp(data, nCode, strMsg);
            mpVHRoomMonitor->OnChangeUserPenAuth(userId, auth, nCode, strMsg);
         }
      });
   }
   return 0;
}

void VHClassRoomImpl::DesktopShareNotice(bool open, const std::string &target_id) {
   HTTP_GET_REQUEST httpGet(mURLGenerator.GetScreensharingUrl(mClassRoomBaseInfo.mUserInfo.join_id, mClassRoomBaseInfo.mWebinarInfo.id, mClassToken, open == true ? 1 : 0, target_id));
   if (mpHttpManagerInterface) {
      httpGet.SetHttpPost(true);
      mpHttpManagerInterface->HttpGetRequest(httpGet, [&](std::string data, int code, const std::string userData)->void {
         LOGD("data:%s code:%d ", data.c_str(), code);
      });
   }
}

int VHClassRoomImpl::SendNoticeMsg(std::string noticeMsg) {
   if (mClassRoomBaseInfo.mUserInfo.role_type != JOIN_ROLE_Teacher) {
      return ROLE_TYPE_ERROR;
   }

   rapidjson::Document document;
   document.SetObject();
   rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

   rapidjson::Value content(StringRef(noticeMsg.c_str()));
   document.AddMember("content", content, allocator);
   rapidjson::Value announcement("*announcement");
   document.AddMember("type", announcement, allocator);

   StringBuffer buffer;
   rapidjson::Writer<StringBuffer> writer(buffer);
   document.Accept(writer);
   std::string data = buffer.GetString();

   HTTP_GET_REQUEST httpGet(mURLGenerator.GetSendNoticeUrl(mMessageInfo.token, "cmd", data));
   if (mpHttpManagerInterface) {
      httpGet.SetHttpPost(true);
      mpHttpManagerInterface->HttpGetRequest(httpGet, [&](std::string data, int code, const std::string userData)->void {
         LOGD("data:%s code:%d ", data.c_str(), code);
      });
   }
   return 0;
}

VideoProfileIndex VHClassRoomImpl::GetVideoProfileByWebinarInfo(int index, const int iVideoProFileType /*= -1*/) {
   VideoProfileIndex profileIndex = RTC_VIDEO_PROFILE_360P_4x3_M/*VIDEO_PROFILE_360P_0_15F*/;
   if (iVideoProFileType <= 0) {
      switch (index)
      {
      case 240:
         profileIndex = RTC_VIDEO_PROFILE_240P_4x3_M/*VIDEO_PROFILE_240P_0_15F*/;
         break;
      case 360:
         profileIndex = RTC_VIDEO_PROFILE_360P_4x3_M/*VIDEO_PROFILE_360P_0_15F*/;
         break;
      case 480:
         profileIndex = RTC_VIDEO_PROFILE_480P_4x3_M/*VIDEO_PROFILE_480P_0_15F*/;
         break;
      case 720:
         profileIndex = RTC_VIDEO_PROFILE_720P_4x3_M/*VIDEO_PROFILE_720P_0_15F*/;
         break;
      case 1080:
         profileIndex = RTC_VIDEO_PROFILE_1080P_4x3_M/*VIDEO_PROFILE_1080P_0_15F*/;
         break;
      default:
         break;
      }
   }
   else {
      profileIndex = RTC_VIDEO_PROFILE_360P_16x9_M;
      switch (index)
      {
      case 240:
         profileIndex = RTC_VIDEO_PROFILE_240P_16x9_M/*VIDEO_PROFILE_240P_0_15F*/;
         break;
      case 360:
         profileIndex = RTC_VIDEO_PROFILE_360P_16x9_M/*VIDEO_PROFILE_360P_0_15F*/;
         break;
      case 480:
         profileIndex = RTC_VIDEO_PROFILE_480P_16x9_M/*VIDEO_PROFILE_480P_0_15F*/;
         break;
      case 720:
         profileIndex = RTC_VIDEO_PROFILE_720P_16x9_M/*VIDEO_PROFILE_720P_0_15F*/;
         break;
      case 1080:
         profileIndex = RTC_VIDEO_PROFILE_1080P_16x9_M/*VIDEO_PROFILE_1080P_0_15F*/;
         break;
      default:
         break;
      }
   }
   return profileIndex;
}

int VHClassRoomImpl::StartBroadCastLayout(LayoutMode layoutMode, BROAD_CAST_DPI dip, SetBroadCastEventCallback callback/*= nullptr*/) {
   LOGD("%s Enter", __FUNCTION__);
   if (bIsSettinglayout) {
      LOGD("VhallLive_SETING_ALREDY");
      return vlive::VhallLive_IS_PROCESSING;
   }
   if (mClassRoomBaseInfo.mUserInfo.role_type != JOIN_ROLE_Teacher) {
      return ROLE_TYPE_ERROR;
   }
   int bRet = VhallLive_OK;


   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      if (!mpWebRtcSDKInterface->IsEnableConfigMixStream()) {
         LOGD("VhallLive_SERVER_NOT_READY");
         return vlive::VhallLive_SERVER_NOT_READY;
      }

      LOGD("%s layoutMode:%d  dip:%d ", __FUNCTION__,layoutMode,  dip);
      mCurrentBroadCastInfo.layoutMode = layoutMode;
      mCurrentBroadCastInfo.dpi = dip;

      BroadCastVideoProfileIndex broadCastProfileIndex = Dpi2ProfileIndex(dip);
      LOGD("  Class SDK StartConfigBroadCast");
      bRet = mpWebRtcSDKInterface->StartConfigBroadCast(layoutMode, broadCastProfileIndex,
         true, std::string("0x666666"), std::string("0x000000"), callback);
      //   [&, layoutMode, callback](const std::string& result, const std::string& msg)->void {
      //   LOGD("SetBroadCastLayout result:%s msg:%s", result.c_str(), msg.c_str());
      //   //SetBroadCastCallBackWithParam(layoutMode, result, msg);
      //   callback(result, msg);
      //});
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      if (!mpPaasInteraction->IsEnableConfigMixStream()) {
         LOGD("VhallLive_SERVER_NOT_READY");
         return vlive::VhallLive_SERVER_NOT_READY;
      }

      LOGD("layoutMode:%d  dip:%d ", layoutMode, dip);
      mCurrentBroadCastInfo.layoutMode = layoutMode;
      mCurrentBroadCastInfo.dpi = dip;

      BroadCastVideoProfileIndex broadCastProfileIndex = Dpi2ProfileIndex(dip);

      VHPaasInteractionRoom *pPaasRoomImpl = mpPaasInteraction;
      LOGD("  Paas SDK StartConfigBroadCast");
      mCurrentBroadCastInfo.layoutMode = layoutMode;
      bRet = mpPaasInteraction->StartConfigBroadCast(layoutMode, broadCastProfileIndex,
         true, std::string("0x666666"), std::string("0x000000"),
         [&, pPaasRoomImpl, layoutMode, callback](const std::string& result, const std::string& msg)->void {
         LOGD("SetBroadCastLayout result:%s msg:%s", result.c_str(), msg.c_str());
         callback(result, msg);
         //pPaasRoomImpl->implementTask(eTask::eTask_BroadCast);
      });
   }

   LOGD("%s Leave", __FUNCTION__);
   return bRet;
}

int VHClassRoomImpl::CheckPicEffectiveness(const std::string filePath) {
   LOGD("filePath %s", filePath.c_str());
   int nRet = VhallLive_NO_OBJ;

   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      LOGD("  Class SDK CheckPicEffectiveness");
      nRet = mpWebRtcSDKInterface->CheckPicEffectiveness(filePath);
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      LOGD("  Paas SDK CheckPicEffectiveness");
      nRet = mpPaasInteraction->CheckPicEffectiveness(filePath);
   }
   else {
      //return VhallLive_NO_OBJ;
   }
   return nRet;
}

int VHClassRoomImpl::SetConfigBroadCastLayOut(LayoutMode mode, SetBroadCastEventCallback fun) {
   LOGD("mode %d", mode);
   //if (mpWebRtcSDKInterface == nullptr) {
   //   return VhallLive_NO_OBJ;
   //}
   int bRet = VhallLive_NO_OBJ;

   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      if (!mpWebRtcSDKInterface->IsEnableConfigMixStream()) {
         LOGD("Class SDK  VhallLive_SERVER_NOT_READY");
         bRet = vlive::VhallLive_SERVER_NOT_READY;
      }
      else {
         LOGD("  Class SDK SetConfigBroadCastLayOut");

         bRet = mpWebRtcSDKInterface->SetConfigBroadCastLayOut(mode,
            [&, fun](const std::string& result, const std::string& msg)->void {
            LOGD("SetBroadCastLayout result:%s msg:%s", result.c_str(), msg.c_str());
            if (fun) {
               fun(result, msg);
            }
         });
      }

   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      if (!mpPaasInteraction->IsEnableConfigMixStream()) {
         LOGD(" Paas SDK VhallLive_SERVER_NOT_READY");
         bRet = vlive::VhallLive_SERVER_NOT_READY;
      }
      else {
         LOGD("  Paas SDK SetConfigBroadCastLayOut");
         bRet = mpPaasInteraction->SetConfigBroadCastLayOut(mode,
            [&, fun](const std::string& result, const std::string& msg)->void {
            LOGD("SetBroadCastLayout result:%s msg:%s", result.c_str(), msg.c_str());
            if (fun) {
               fun(result, msg);
            }
         });
      }
   }

   return bRet;
}

int VHClassRoomImpl::SetScreenLayout( bool hasCamera, const int& iLayoutMode) {
   int ShareScreenLayout = hasCamera == true ? 1 : 3;
   HTTP_GET_REQUEST httpGet(mURLGenerator.GetSetScreenLayoutUrl(mClassRoomBaseInfo.mUserInfo.join_id, 
      mClassRoomBaseInfo.mWebinarInfo.id, mClassToken, ShareScreenLayout, iLayoutMode));
   if (mpHttpManagerInterface) {
      int iMixedLayout = iLayoutMode;
      ClassRoomBaseInfo* pInfo =  &mClassRoomBaseInfo;
      mpHttpManagerInterface->HttpGetRequest(httpGet, [&, iMixedLayout, ShareScreenLayout, pInfo](std::string data, int code, const std::string userData)->void {
         LOGD("Request data:%s code:%d", data.c_str(), code);
         if (0 == code && iMixedLayout) {
            pInfo->mWebinarInfo.mixedLayout = (eMixedLayoutMode)iMixedLayout;
            pInfo->mWebinarInfo.desktop_share_layout_mode = ShareScreenLayout;
         }
      });
   }
   LOGD("end");
   return 0;
}


int VHClassRoomImpl::SetBigScreen(const std::string& user_id, SetBigScreenCallback cb_fun) {
   LOGD("%s ", user_id.c_str());
   HTTP_GET_REQUEST httpGet(mURLGenerator.GetSetBigScreenUrl(mClassRoomBaseInfo.mUserInfo.join_id, user_id, mClassRoomBaseInfo.mWebinarInfo.id, mClassToken));
   if (mpHttpManagerInterface) {
      mpHttpManagerInterface->HttpGetRequest(httpGet, [&, cb_fun, user_id](std::string data, int code, const std::string userData)->void {
         LOGD("Request data:%s code:%d", data.c_str(), code);
         int nCode = code;
         std::string strMsg = data;
         ParamEventToResp(data, nCode, strMsg);
         if (cb_fun) {
            cb_fun(user_id, nCode, strMsg);
         }
         if (nCode == 200) {
            mClassRoomBaseInfo.bigScreen = atoi(user_id.c_str());
         }
      });
   }
   LOGD("end");
   return 0;
}

int VHClassRoomImpl::SetMainView(std::string stream_id) {
   LOGD("%s stream_id %s",__FUNCTION__, stream_id.c_str());
   if (stream_id.length() == 0) {
      LOGD("%s VhallLive_PARAM_ERR", __FUNCTION__);
      return VhallLive_PARAM_ERR;
   }
   //if (mpWebRtcSDKInterface == nullptr) {
      int bRet = VhallLive_NO_OBJ;
   //}
      VHRoomMonitor * pRoomMonitor = mpVHRoomMonitor;
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      if (!mpWebRtcSDKInterface->IsEnableConfigMixStream()) {
         LOGD("%s  VhallLive_SERVER_NOT_READY", __FUNCTION__);
         return vlive::VhallLive_SERVER_NOT_READY;
      }
      if (mClassRoomBaseInfo.mUserInfo.role_type != JOIN_ROLE_Teacher) {
         return VhallLive_NO_PERMISSION;
      }
      bRet = mpWebRtcSDKInterface->SetMixLayoutMainView(stream_id,
         [&, pRoomMonitor, stream_id](const std::string& result, const std::string& msg)->void {
         LOGD("%s SetBroadCastLayout result:%s msg:%s", __FUNCTION__, result.c_str(), msg.c_str());
         //如果设置布局成功
         if (result.compare("success") == 0) {
            if (pRoomMonitor) {
               LOGD("%s msg:%s", __FUNCTION__, msg.c_str());
               pRoomMonitor->OnCallbackEvent(RoomEvent_SetMainView, CALL_BACK_SUC, msg, stream_id);
            }
         }
         else {
            if (pRoomMonitor) {
               LOGD("%s msg:%s", __FUNCTION__, msg.c_str());
               pRoomMonitor->OnCallbackEvent(RoomEvent_SetMainView, -1, msg, stream_id);
            }
         }
      });
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      bRet = mpPaasInteraction->SetMainView(stream_id,
         [&, pRoomMonitor,stream_id](const std::string& result, const std::string& msg)->void {
         LOGD("%s SetBroadCastLayout result:%s msg:%s", __FUNCTION__, result.c_str(), msg.c_str());
         //如果设置布局成功
         if (result.compare("success") == 0) {
            if (pRoomMonitor) {
               LOGD("%s msg:%s", __FUNCTION__, msg.c_str());
               pRoomMonitor->OnCallbackEvent(RoomEvent_SetMainView, CALL_BACK_SUC, msg, stream_id);
            }
         }
         else {
            if (pRoomMonitor) {
               LOGD("%s msg:%s", __FUNCTION__, msg.c_str());
               pRoomMonitor->OnCallbackEvent(RoomEvent_SetMainView, -1, msg, stream_id);
            }
         }
      });
   }

   return bRet;
}

/*
*   停止旁路直播
*   live_room_id: 直播房间id
*/
int VHClassRoomImpl::StopPublishInavAnother(std::string live_room_id) {
   //api状态上报

   return 0;
}

bool VHClassRoomImpl::ParamEventToResp(std::string data, int& respCode, std::string& respMsg) {
   LOGD("ParamRoomEvent data:%s ", data.c_str());
   rapidjson::Document doc;
   doc.Parse<0>(data.c_str());
   if (doc.HasParseError()) {
      LOGE("VHTokenRespMsg ParseError%d\n", doc.GetParseError());
      return false;
   }
   if (doc.IsObject()) {
      if (doc.HasMember("code") && doc["code"].IsInt()) {
         respCode = doc["code"].GetInt();
      }
      if (doc.HasMember("msg") && doc["msg"].IsString()) {
         respMsg = doc["msg"].GetString();
      }
      return true;
   }
   else {
      return false;
   }
}

bool VHClassRoomImpl::IsUserPublish(const std::string user_id, std::string data, int& respCode, std::string& respMsg) {
   LOGD("ParamRoomEvent data:%s ", data.c_str());
   rapidjson::Document doc;
   doc.Parse<0>(data.c_str());
   if (doc.HasParseError()) {
      LOGE("VHTokenRespMsg ParseError%d\n", doc.GetParseError());
      return false;
   }
   if (doc.IsObject()) {
      if (doc.HasMember("code") && doc["code"].IsInt()) {
         respCode = doc["code"].GetInt();
      }
      if (doc.HasMember("msg") && doc["msg"].IsString()) {
         respMsg = doc["msg"].GetString();
      }
      if (respCode == 200) {
         if (doc.HasMember("data") && doc["data"].IsArray()) {
            rapidjson::Value& publish_users = doc["data"];
            for (int i = 0; i < publish_users.Size(); i++) {
               rapidjson::Value& users = publish_users[i];
               if (users.HasMember("id") && users["id"].IsString()) {
                  std::string pushlisher = users["id"].GetString();
                  if (pushlisher == user_id) {
                     return true;
                  }
               }
            }
         }
      }
      return false;
   }
   else {
      return false;
   }
}


bool VHClassRoomImpl::ParamRoomEvent(ClassRoomEvent event, std::string data, int code, std::string userData) {
   LOGD("ParamRoomEvent data:%s code:%d userData:%s ", data.c_str(), code, userData.c_str());
   int codeNum = code;
   string msg = "no define";
   rapidjson::Document doc;
   doc.Parse<0>(data.c_str());
   if (doc.HasParseError()) {
      LOGE("VHTokenRespMsg ParseError%d\n", doc.GetParseError());
      return false;
   }
   if (doc.IsObject()) {
      if (doc.HasMember("code") && doc["code"].IsInt()) {
         codeNum = doc["code"].GetInt();
      }
      if (doc.HasMember("msg") && doc["msg"].IsString()) {
         msg = doc["msg"].GetString();
      }
   }

   if (codeNum == 200) {
      if (mpVHRoomMonitor) {
         mpVHRoomMonitor->OnCallbackEvent(event, CALL_BACK_SUC, msg, userData);
      }
      return true;
   }
   else {
      if (mpVHRoomMonitor) {
         mpVHRoomMonitor->OnCallbackEvent(event, codeNum, msg, userData);
      }
      //混流失败之后不能再次调用混流接口，等待底层上报ON_STREAM_MIXED消息
      if (RoomEvent_Start_MixStream == event) {
         if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
            mpWebRtcSDKInterface->SetEnableConfigMixStream(false);
         }
         else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
            mpPaasInteraction->SetEnableConfigMixStream(false);
         } 
      }
   }
   return false;
}

//void VHClassRoomImpl::CheckPermission(VHStreamType streamType, std::string data, int code) {
//    LOGD("ParamRoomEvent data:%s code:%d ", data.c_str(), code);
//    int codeNum = code;
//    string msg = "no define";
//    rapidjson::Document doc;
//    doc.Parse<0>(data.c_str());
//    if (doc.HasParseError()) {
//        LOGE("VHTokenRespMsg ParseError%d\n", doc.GetParseError());
//        return;
//    }
//    if (doc.IsObject()) {
//        if (doc.HasMember("code") && doc["code"].IsInt()) {
//            codeNum = doc["code"].GetInt();
//        }
//        if (doc.HasMember("msg") && doc["msg"].IsString()) {
//            msg = doc["msg"].GetString();
//        }
//    }
//
//    if (codeNum == 200) {
//        if (doc.HasMember("data") && doc["data"].IsObject()) {
//            rapidjson::Value& dataMsg = doc["data"];
//            if (dataMsg.HasMember("has_permission") && dataMsg["has_permission"].IsInt()) {
//                int permission = dataMsg["has_permission"].GetInt();
//                if (permission == 1) {
//                    //有权限
//                    switch (streamType)
//                    {
//					case VHStreamType_AUDIO:
//					case VHStreamType_VIDEO:
//                    case VHStreamType_AVCapture:
//                        if (mpWebRtcSDKInterface) {
//                            mpWebRtcSDKInterface->StartPushLocalStream();
//                        }
//                        break;
//                    case VHStreamType_Desktop:
//                        if (mpWebRtcSDKInterface) {
//                            mpWebRtcSDKInterface->StartPushDesktopStream();
//                        }
//                        break;
//                    case VHStreamType_MediaFile:
//                        if (mpWebRtcSDKInterface) {
//                            mpWebRtcSDKInterface->StartPushMediaFileStream();
//                        }
//                        break;
//                    case VHStreamType_Count:
//                        break;
//                    default:
//                        break;
//                    }
//
//                }
//                else {
//                    if (mpVHRoomMonitor) {
//                        mpVHRoomMonitor->OnPushStreamError(streamType, permission, "no permission");
//                    }
//                }
//            }
//
//        }
//    }
//    else {
//        if (mpVHRoomMonitor) {
//            mpVHRoomMonitor->OnPushStreamError(streamType, codeNum, msg);
//        }
//    }
//}

void VHClassRoomImpl::ParamRecvInavMsg(const std::string data) {
   LOGD("ParamRecvInavMsg data:%s", data.c_str());
   //http://doc.vhall.com/docs/show/1201

   int codeNum = 0;
   string msg = "no define";
   rapidjson::Document doc;
   doc.Parse<0>(data.c_str());
   if (doc.HasParseError()) {
      LOGE("VHTokenRespMsg ParseError%d\n", doc.GetParseError());
      return;
   }
   if (doc.IsObject()) {
      if (doc.HasMember("type") && doc["type"].IsString()) {
         std::string inav_event = doc["type"].GetString();
         if (inav_event == "*announcement") {
            if (doc.HasMember("content") && doc["content"].IsString()) {
               std::string content = doc["content"].GetString();
               if (mClassRoomBaseInfo.mUserInfo.role_type != JOIN_ROLE_Teacher) {
                  mpVHRoomMonitor->OnRecvNoticeMsg(content);
               }
            }
         }
         if (inav_event == "*openHand") {
            //举手开关 {"status":1,"type":"*openHand"}
            int status;
            if (doc.HasMember("status") && doc["status"].IsInt()) {
               status = doc["status"].GetInt();
            }
            if (doc.HasMember("status") && doc["status"].IsString()) {
               status = atoi(doc["status"].GetString());
            }
            mClassRoomBaseInfo.openhand = status;
            if (mpVHRoomMonitor) {
               mpVHRoomMonitor->OnRecvOpenHandMsg(status);
            }
         }
         else if ("*setBigscreen" == inav_event) {
            HandleSetBigScreen(doc);//公开课，讲师学员切换大屏消息。
         }
         else if ("*publishStart" == inav_event) {
            //上课消息
            //{"join_id":"69640","nick_name":"626792","online":[],"role":1,"type":"*publishStart"}
            std::string nick_name;
            std::string join_id;
            if (doc.HasMember("nick_name") && doc["nick_name"].IsString()) {
               nick_name = doc["nick_name"].GetString();
            }
            if (doc.HasMember("join_id") && doc["join_id"].IsString()) {
               join_id = doc["join_id"].GetString();
            }

            int iCurLabelNumber = 1;
            if (doc.HasMember("label_number") && doc["label_number"].IsInt()) {
               iCurLabelNumber = doc["label_number"].GetInt();
            }
            else if (doc.HasMember("label_number") && doc["label_number"].IsString()) {
               iCurLabelNumber = atoi(doc["label_number"].GetString());
            }

			   if (iCurLabelNumber < 1 ||  iCurLabelNumber >4) {
				   iCurLabelNumber = 1;
			   }

            eClassPlayType StartPlayType = eClassPlayType_Flash;
               if (doc.HasMember("play_type") && doc["play_type"].IsInt()) {
                  StartPlayType = (eClassPlayType)doc["play_type"].GetInt();
               }
               else if (doc.HasMember("play_type") && doc["play_type"].IsString()) {
                  std::string strPlayType = doc["play_type"].GetString();
                  StartPlayType = (eClassPlayType)atoi(strPlayType.c_str());
               }

            mClassRoomBaseInfo.mWebinarInfo.type = CLASS_STATUS_IN_CLASS;
            LOGD("%s mWebinarInfo.type:%d", __FUNCTION__, (int)mClassRoomBaseInfo.mWebinarInfo.type);
            mCourseInfo.webinar_status = CLASS_STATUS_IN_CLASS;
            if (mClassRoomBaseInfo.mUserInfo.role_type != JOIN_ROLE_Teacher) {
               mpVHRoomMonitor->OnRecvStartClassMsg(
                  SystemUtility::String2WString(join_id), 
                  SystemUtility::String2WString(nick_name), 
                  iCurLabelNumber, (int)StartPlayType);
            }

            rapidjson::Document jsonDoc;
            rapidjson::Document::AllocatorType &allocator = jsonDoc.GetAllocator();
            rapidjson::Value cmdJson(rapidjson::kArrayType);
            rapidjson::Value cmdMsg(StringRef("cmd"));

            rapidjson::Document cmdDoc;
            cmdDoc.Parse<0>(data.c_str());

            cmdJson.PushBack(cmdMsg, allocator);
            cmdJson.PushBack(cmdDoc, allocator);

            StringBuffer buffer;
            rapidjson::Writer<StringBuffer> writer(buffer);
            cmdJson.Accept(writer);
            std::string data = buffer.GetString();
            if (mpVHRoomMonitor) {
               mpVHRoomMonitor->OnRecvFlahsMsg(data);
            }
         }
         else if ("*over" == inav_event) {
            //下课消息
            mClassRoomBaseInfo.mWebinarInfo.type = CLASS_STATUS_OVER;
            mClassRoomBaseInfo.mDocInfo.doc_mode = 0;
            LOGD("%s mWebinarInfo.type:%d", __FUNCTION__, (int)mClassRoomBaseInfo.mWebinarInfo.type);
            mCourseInfo.webinar_status = CLASS_STATUS_OVER;
            mClassRoomBaseInfo.mWebinarInfo.real_duration = 0;
            mClassRoomBaseInfo.mutingAll = 0;
    //        if (mClassRoomBaseInfo.mUserInfo.role_type != JOIN_ROLE_Teacher) {
				//mpVHRoomMonitor->OnRecvStopClassMsg();
    //        }
			mpVHRoomMonitor->OnRecvStopClassMsg((int)mClassRoomBaseInfo.play_type);
            rapidjson::Document jsonDoc;
            rapidjson::Document::AllocatorType &allocator = jsonDoc.GetAllocator();
            rapidjson::Value cmdJson(rapidjson::kArrayType);
            rapidjson::Value cmdMsg(StringRef("cmd"));

            rapidjson::Document cmdDoc;
            cmdDoc.Parse<0>(data.c_str());

            cmdJson.PushBack(cmdMsg, allocator);
            cmdJson.PushBack(cmdDoc, allocator);

            StringBuffer buffer;
            rapidjson::Writer<StringBuffer> writer(buffer);
            cmdJson.Accept(writer);
            std::string data = buffer.GetString();
            if (mpVHRoomMonitor) {
               mpVHRoomMonitor->OnRecvFlahsMsg(data);
            }
         }
         else if (inav_event == "*syncInterstitialProgressRate" || inav_event == "*pauseInterstitial" || inav_event == "*stopInterstitial") {
            std::string join_id;
            if (doc.HasMember("join_id") && doc["join_id"].IsString()) {
               join_id = doc["join_id"].GetString();
            }
            if (join_id != mClassRoomBaseInfo.mUserInfo.join_id) {
               HandleRecvMediaMsg(data);
            }
         }
         else if ("doc_convert_jpeg" == inav_event || "doc_convert" == inav_event || "file_convert" == inav_event) {
            HandleDocUploadMsg(data);
            return;
         }
         else if (inav_event == "*prepareJoinMics") {
            //上麦：{"join_id":"69640","status":"1","target_id":"69640","type":"*prepareJoinMics"}
            //下麦：{"join_id":"69640","status":"0","target_id":"69640","type":"*prepareJoinMics"}
            //收到邀请上麦消息 和 下麦消息
            std::string join_id;
            std::string target_id;
            int  status;
            if (doc.HasMember("join_id") && doc["join_id"].IsString()) {
               join_id = doc["join_id"].GetString();
            }
            if (doc.HasMember("target_id") && doc["target_id"].IsString()) {
               target_id = doc["target_id"].GetString();
            }
            if (doc.HasMember("status") && doc["status"].IsInt()) {
               status = doc["status"].GetInt();
            }
            if (doc.HasMember("status") && doc["status"].IsString()) {
               status = atoi(doc["status"].GetString());
            }

            if (mClassRoomBaseInfo.mUserInfo.role_type == JOIN_ROLE_Student && mClassRoomBaseInfo.mWebinarInfo.course_type == CLASS_TYPE_MULTI) {
               if (mClassRoomBaseInfo.bigScreen != std::atoi(mClassRoomBaseInfo.current_speaker.c_str()) && mClassRoomBaseInfo.bigScreen != std::atoi(mClassRoomBaseInfo.mUserInfo.join_id.c_str())) {
                  mClassRoomBaseInfo.bigScreen = std::atoi(mClassRoomBaseInfo.current_speaker.c_str());
               }
            }

            if (target_id == mClassRoomBaseInfo.mUserInfo.join_id) {
               mpVHRoomMonitor->OnRecvAskforInavPublishMsg(SystemUtility::String2WString(join_id), SystemUtility::String2WString(target_id), status);
            }
         }
         else if (inav_event == "*closeScreen") {
            //"join_id":"70759","status":1,"target_id":"70759","type":"*closeScreen"}
            std::string join_id;
            std::string target_id;
            int  status;
            if (doc.HasMember("join_id") && doc["join_id"].IsString()) {
               join_id = doc["join_id"].GetString();
            }
            if (doc.HasMember("target_id") && doc["target_id"].IsString()) {
               target_id = doc["target_id"].GetString();
            }
            if (doc.HasMember("status") && doc["status"].IsInt()) {
               status = doc["status"].GetInt();
            }
            if (doc.HasMember("status") && doc["status"].IsString()) {
               status = atoi(doc["status"].GetString());
            }
            if (mpVHRoomMonitor && join_id != mClassRoomBaseInfo.mUserInfo.join_id) {
               mpVHRoomMonitor->OnRecvMuteVideoMsg(target_id, status);
            }
         }
         else if (inav_event == "*quiet") {
            //静音：["cmd", { type: "*quiet", join_id : "67821", target_id : "67821", status : "0" }]
            std::string target_id;
            std::string join_id;
            int status;
            if (doc.HasMember("target_id") && doc["target_id"].IsString()) {
               target_id = doc["target_id"].GetString();
            }
            if (doc.HasMember("join_id") && doc["join_id"].IsString()) {
               join_id = doc["join_id"].GetString();
            }
            if (doc.HasMember("status") && doc["status"].IsInt()) {
               status = doc["status"].GetInt();
            }
            if (doc.HasMember("status") && doc["status"].IsString()) {
               status = atoi(doc["status"].GetString());
            }

            LOGD("mpVHRoomMonitor->OnRecvMuteMicMsg");
            //不处理自己发的
            if (mpVHRoomMonitor && join_id != mClassRoomBaseInfo.mUserInfo.join_id) {
               mpVHRoomMonitor->OnRecvMuteMicMsg(target_id, status);
            }
         }
         else if (inav_event == "*create_question" || inav_event == "*push_question" || inav_event == "*update_answer" ||
            inav_event == "*publish_answer" || inav_event == "*stop_answer" || inav_event == "*clear_question" ||
            inav_event == "online" || inav_event == "offline" || inav_event == "*new_push_question" || inav_event == "*new_publish_answer") {
            //{"bu":"answercard","join_id":"82843","type":"*clear_question"}答题器
            //{"bu":"question","join_id":"82843","type":"*clear_question"}	答题
            LOGD("msg %s", __FUNCTION__, data.c_str());
            std::string strBu = "";

				if (doc.HasMember("bu") && doc["bu"].IsString()) {
					strBu = doc["bu"].GetString();
				}
				if (strBu == "answercard"){
					if (inav_event == "*new_push_question" || inav_event == "*update_answer" || inav_event == "*new_publish_answer" || inav_event == "*stop_answer") {
						HandleAnswerToolMsg(data);
					}
				}
				else{
					std::string join_id;
					if (doc.HasMember("join_id") && doc["join_id"].IsString()) {
						join_id = doc["join_id"].GetString();
					}
					HandleAnswerMsg(data);
				}
         }
         else if (inav_event == "*batchDownMic") {
            //下麦所有用户{"type":"*batchDownMic","join_id":"67821"}
            if (mClassRoomBaseInfo.mUserInfo.role_type != JOIN_ROLE_Teacher) {
               //mpVHRoomMonitor->OnRecvDownAllUserPublishMsg();
            }
         }
         else if (inav_event == "*hand") {
            //接收到学员或嘉宾的举手消息
            std::string nick_name;
            std::string join_id;
            if (doc.HasMember("userID") && doc["userID"].IsObject()) {
               rapidjson::Value& userID = doc["userID"];
               if (userID.HasMember("join_id") && userID["join_id"].IsString()) {
                  join_id = userID["join_id"].GetString();
               }
               if (userID.HasMember("nick_name") && userID["nick_name"].IsString()) {
                  nick_name = userID["nick_name"].GetString();
               }
               if (join_id != mClassRoomBaseInfo.mUserInfo.join_id && mClassRoomBaseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher) {
                  mpVHRoomMonitor->OnRecvApplyInavPublishMsg(SystemUtility::String2WString(join_id), SystemUtility::String2WString(nick_name));
               }
            }
         }
         else if (inav_event == "*mutingAll") {
            //全体静音：["cmd", { "type":"*mutingAll","status" : "1" }]
            int status;
            if (doc.HasMember("status") && doc["status"].IsInt()) {
               status = doc["status"].GetInt();
            }
            if (doc.HasMember("status") && doc["status"].IsString()) {
               status = atoi(doc["status"].GetString());
            }
            if (mpVHRoomMonitor && mClassRoomBaseInfo.mUserInfo.role_type != JOIN_ROLE_Teacher) {
               mpVHRoomMonitor->OnRecvAllMuteMicMsg(status);
               mClassRoomBaseInfo.mutingAll = status;
            }
         }
         else if (inav_event == "*frame") {
            //["cmd",{"type":"*frame","join_id":"67821","target_id":"67821","status":"1"}]
            //["cmd", { "type":"*frame","join_id" : "67821","target_id" : "67821","status" : "0" }]
            std::string target_id;
            std::string join_id;
            int status;
            if (doc.HasMember("target_id") && doc["target_id"].IsString()) {
               target_id = doc["target_id"].GetString();
            }
            if (doc.HasMember("join_id") && doc["join_id"].IsString()) {
               join_id = doc["join_id"].GetString();
            }
            if (doc.HasMember("status") && doc["status"].IsInt()) {
               status = doc["status"].GetInt();
            }
            if (doc.HasMember("status") && doc["status"].IsString()) {
               status = atoi(doc["status"].GetString());
            }
            //不处理自发的消息。
            if (mpVHRoomMonitor && join_id != mClassRoomBaseInfo.mUserInfo.join_id) {
               mpVHRoomMonitor->OnRecvMuteVideoMsg(target_id, status);
            }
         }
         else if (inav_event == "*kickout") {
            //踢出：["cmd",{"type":"*kickout","target_id":"70149"}]
            std::string target_id;
            if (doc.HasMember("target_id") && doc["target_id"].IsString()) {
               target_id = doc["target_id"].GetString();
            }
            else if (doc.HasMember("target_id") && doc["target_id"].IsInt()) {
               char string[32];
               target_id = itoa(doc["target_id"].GetInt(), string, 10);
               target_id = string;
            }

            if (mpVHRoomMonitor) {
               mpVHRoomMonitor->OnRecvKickOutRoomMsg(target_id, 1);
            }
         }
         else if (inav_event == "*kickoutrestore") {
            //取消踢出：["cmd",{"type":"*kickoutrestore","target_id":"67821"}]
            std::string target_id;
            if (doc.HasMember("target_id") && doc["target_id"].IsString()) {
               target_id = doc["target_id"].GetString();
            }
            if (mpVHRoomMonitor) {
               mpVHRoomMonitor->OnRecvKickOutRoomMsg(target_id, 0);
            }
         }
         else if (inav_event == "*disablechat" || inav_event == "*permitchat") {
            //禁言和取消禁言用户
            std::string target_id;
            if (doc.HasMember("target_id") && doc["target_id"].IsString()) {
               target_id = doc["target_id"].GetString();
            }
            if (mpVHRoomMonitor) {
               mpVHRoomMonitor->OnRecvForbiddenChat(inav_event == "*disablechat" ? 1 : 0, target_id, data);
            }
         }
         else if (inav_event == "*banchatall") {
            //禁言和取消禁言用户
            int status;
            if (doc.HasMember("status") && doc["status"].IsInt()) {
               status = doc["status"].GetInt();
            }
            else if (doc.HasMember("status") && doc["status"].IsString())
            {
               status = atoi(doc["status"].GetString());
            }

            if (mpVHRoomMonitor) {
               mpVHRoomMonitor->OnRecvForbiddenAllChat(status, data);
            }
         }
         else if (inav_event == "*UpdateMic") {
            //无上麦能力为：["cmd",{"type":"*UpdateMic","join_id":"67821","status":1}]
            //有上麦能力为：["cmd", { "type":"*UpdateMic","join_id" : "67821","status" : 0 }]
            int status;
            std::string join_id;
            if (doc.HasMember("status") && doc["status"].IsInt()) {
               status = doc["status"].GetInt();
            }
            else if (doc.HasMember("status") && doc["status"].IsString()) {
               status = atoi(doc["status"].GetString());
            }


            if (doc.HasMember("join_id") && doc["join_id"].IsString()) {
               join_id = doc["join_id"].GetString();
            }
            if (mpVHRoomMonitor) {
               mpVHRoomMonitor->OnRecvUpdatePublishAbility(join_id, status);
            }
         }
         else if (inav_event == "*switchMic") {
            //上麦为：["cmd",{"type":"*switchMic","join_id":"67821","target_id":"70149","status":"1","role_type":2,"device_type":1,"user":{"nick_name":"jack"}}]
            //下麦为：["cmd", { "type":"*switchMic","join_id" : "67821","target_id" : "70149","status" : "0","role_type" : 2,"device_type" : 1,"user" : {"nick_name":"jack"} }]
        //{"device_type":"4", "join_id" : "80270", "role_type" : 2, "status" : 1, "target_id" : "80270", "type" : "*switchMic", "user" : {"nick_name":"qyy", "reward_num" : 0}}
            std::string nick_name;
            std::string join_id;
            int iRewardNum = -1;
            int status;
            int role_type;
            int device_type;
            if (doc.HasMember("user") && doc["user"].IsObject()) {
               rapidjson::Value& user = doc["user"];
               if (user.HasMember("nick_name") && user["nick_name"].IsString()) {
                  nick_name = user["nick_name"].GetString();
               }
            }
            if (doc.HasMember("userInfo") && doc["userInfo"].IsObject()) {
               rapidjson::Value& user = doc["userInfo"];
               if (user.HasMember("reward_num") && user["reward_num"].IsString()) {
                  std::string strReward = user["reward_num"].GetString();
                  iRewardNum = atoi(strReward.c_str());
               }
               else if (user.HasMember("reward_num") && user["reward_num"].IsInt()) {
                  iRewardNum = user["reward_num"].GetInt();
               }
            }

            if (doc.HasMember("target_id") && doc["target_id"].IsString()) {
               join_id = doc["target_id"].GetString();
            }
            if (doc.HasMember("status") && doc["status"].IsInt()) {
               status = doc["status"].GetInt();
            }
            if (doc.HasMember("status") && doc["status"].IsString()) {
               status = atoi(doc["status"].GetString());
            }
            if (doc.HasMember("role_type") && doc["role_type"].IsInt()) {
               role_type = doc["role_type"].GetInt();
            }
            if (doc.HasMember("device_type") && doc["device_type"].IsInt()) {
               device_type = doc["device_type"].GetInt();
            }
            if (doc.HasMember("device_type") && doc["device_type"].IsString()) {
               device_type = atoi(doc["device_type"].GetString());
            }

            int iNoDoubleVrtc = -1;
            if (doc.HasMember("no_double_vrtc") && doc["no_double_vrtc"].IsInt()) {
               iNoDoubleVrtc = doc["no_double_vrtc"].GetInt();
            }
            else if (doc.HasMember("no_double_vrtc") && doc["no_double_vrtc"].IsString()) {
               iNoDoubleVrtc = atoi(doc["no_double_vrtc"].GetString());
            }

            if (mpVHRoomMonitor && join_id != mClassRoomBaseInfo.mUserInfo.join_id) {
               mpVHRoomMonitor->OnRecvUserPublishCallback(join_id, nick_name, (ClassPushStreamEvent)status, role_type, device_type, iRewardNum, iNoDoubleVrtc);
            }
         }
         else if (inav_event == "*failJoinMic") {
            //上麦失败为：["cmd", { "type":"*failJoinMic","join_id" : "67821","target_id" : "70149","status" : "1","role_type" : 2,"device_type" : 1,"nick_name" : "jack" }]
            //下麦失败为：["cmd", { "type":"*failJoinMic","join_id" : "67821","target_id" : "70149","status" : "0","role_type" : 2,"device_type" : 1,"nick_name" : "jack" }]
            std::string nick_name;
            std::string join_id;
            std::string target_id;
            int role_type;
            int app_type;
            int status;
            if (doc.HasMember("join_id") && doc["join_id"].IsString()) {
               join_id = doc["join_id"].GetString();
            }
            if (doc.HasMember("target_id") && doc["target_id"].IsString()) {
               target_id = doc["target_id"].GetString();
            }
            if (doc.HasMember("nick_name") && doc["nick_name"].IsString()) {
               nick_name = doc["nick_name"].GetString();
            }
            if (doc.HasMember("status") && doc["status"].IsInt()) {
               status = doc["status"].GetInt();
            }
            if (doc.HasMember("status") && doc["status"].IsString()) {
               status = atoi(doc["status"].GetString());
            }
            if (doc.HasMember("role_type") && doc["role_type"].IsInt()) {
               role_type = doc["role_type"].GetInt();
            }
            if (doc.HasMember("device_type") && doc["device_type"].IsInt()) {
               app_type = doc["device_type"].GetInt();
            }
            if (mpVHRoomMonitor && target_id != mClassRoomBaseInfo.mUserInfo.join_id) {
               mpVHRoomMonitor->OnRecvPublishFailedMsg(target_id, status, nick_name, role_type, app_type);
            }
         }
         else if ("*docMode" == inav_event)
         {
            //["cmd",{"type":"*docMode","status":"0","publish_release_time":"2019-03-08 19:06:41"}]
            //["cmd",{"type":"*docMode","status":"1","publish_release_time":"2019-03-08 19:07:21"}]
            int status;
            if (doc.HasMember("status") && doc["status"].IsInt()) {
               status = doc["status"].GetInt();
            }
            if (doc.HasMember("status") && doc["status"].IsString()) {
               status = atoi(doc["status"].GetString());
            }

            if (mpVHRoomMonitor) {
               ToolPage ipage;
               if (0 == status)
                  ipage = ToolPage_Doc;
               else
                  ipage = ToolPage_Board;
               mpVHRoomMonitor->OnChangeToToolPage(ipage, 0, "");
            }
         }
         else if ("*startSign" == inav_event)
         {
            //开始签到 {'type':'*startSign', 'webinar_id' : 'edu_xxxx', 'sign_id' : '签到id', 'user_id' : '商户id', 'sign_show_time' : 30}
            std::string strWebinar;
            std::string sign_id;
            int signShowTime = 0;
            if (doc.HasMember("webinar_id") && doc["webinar_id"].IsString()) {
               strWebinar = doc["webinar_id"].GetString();
            }
            if (doc.HasMember("sign_id") && doc["sign_id"].IsString()) {
               sign_id = doc["sign_id"].GetString();
            }
            else if (doc.HasMember("sign_id") && doc["sign_id"].IsInt()) {
               int isign_id = doc["sign_id"].GetInt();
               char string[32];
               sign_id = itoa(isign_id, string, 10);
            }

            if (doc.HasMember("sign_show_time") && doc["sign_show_time"].IsInt()) {
               signShowTime = doc["sign_show_time"].GetInt();
            }
            else if (doc.HasMember("sign_show_time") && doc["sign_show_time"].IsString()) {
               std::string  sTime = doc["sign_show_time"].GetString();
               signShowTime = atoi(sTime.c_str());
            }

            if (mpVHRoomMonitor) {
               mpVHRoomMonitor->OnRecvStartSign(strWebinar, sign_id, signShowTime);
            }
         }
         else if ("*signIn" == inav_event)
         {
            //签到 {'type':'*signIn','webinar_id':'edu_xxxx','sign_id':'签到id','signer_id':'9823','signer_nickname':'xxxx'}
            std::string strWebinar;
            std::string sign_id;
            std::string signer;
            std::string signerNickname;
            if (doc.HasMember("webinar_id") && doc["webinar_id"].IsString()) {
               strWebinar = doc["webinar_id"].GetString();
            }
            if (doc.HasMember("sign_id") && doc["sign_id"].IsString()) {
               sign_id = doc["sign_id"].GetString();
            }
            else if (doc.HasMember("sign_id") && doc["sign_id"].IsInt()) {
               int isign_id = doc["sign_id"].GetInt();
               char string[32];
               sign_id = itoa(isign_id, string, 10);
            }

            if (doc.HasMember("signer_id") && doc["signer_id"].IsString()) {
               signer = doc["signer_id"].GetString();
            }
            else if (doc.HasMember("signer_id") && doc["signer_id"].IsInt()) {
               int isign_id = doc["signer_id"].GetInt();
               char string[32];
               signer = itoa(isign_id, string, 10);
            }

            if (doc.HasMember("signer_nickname") && doc["signer_nickname"].IsString()) {
               signerNickname = doc["signer_nickname"].GetString();
            }

            if (mpVHRoomMonitor) {
               mpVHRoomMonitor->OnRecvSignIn(strWebinar, sign_id, signer, signerNickname);
            }
         }
         else if ("*stopSign" == inav_event)
         {
            //停止签到{'type':'*stopSign', 'webinar_id' : 'edu_xxxx', 'sign_id' : '签到id'}
            std::string strWebinar;
            std::string sign_id;
            if (doc.HasMember("webinar_id") && doc["webinar_id"].IsString()) {
               strWebinar = doc["webinar_id"].GetString();
            }
            if (doc.HasMember("sign_id") && doc["sign_id"].IsString()) {
               sign_id = doc["sign_id"].GetString();
            }
            else if (doc.HasMember("sign_id") && doc["sign_id"].IsInt()) {
               int isign_id = doc["sign_id"].GetInt();
               char string[32];
               sign_id = itoa(isign_id, string, 10);
            }

            if (mpVHRoomMonitor) {
               mpVHRoomMonitor->OnRecvSignStop(sign_id, strWebinar);
            }
         }
         else if ("*create_question" == inav_event)
         {
            mpVHRoomMonitor->OnChangeToToolPage(ToolPage_Answer_Open, 0, "");
         }

         else if ("*startBroadcast" == inav_event)
         {
            mClassRoomBaseInfo.mWebinarInfo.bRrlay = true;
            mpVHRoomMonitor->OnChangeRrlayType();
         }
         else if ("*stopBroadcast" == inav_event)
         {
            mClassRoomBaseInfo.mWebinarInfo.bRrlay = false;
            mpVHRoomMonitor->OnChangeRrlayType();
         }
         else if ("*clear_question" == inav_event)
         {
            mpVHRoomMonitor->OnChangeToToolPage(ToolPage_Answer_Close, 0, "");
         }
         else if ("*openScreenSharing" == inav_event)
         {
            //开启桌面共享为：["cmd", { "type":"*openScreenSharing","status" : 1 }]
            //关闭桌面共享为：["cmd", { "type":"*openScreenSharing","status" : 0 }]
            int iState = 1;
            if (doc.HasMember("status") && doc["status"].IsString()) {
               //iState = atoi( doc["status"].GetString().c_str() );
               std::string strNum = doc["status"].GetString();
               iState = atoi(strNum.c_str());
            }
            else if (doc.HasMember("status") && doc["status"].IsInt()) {
               iState = doc["status"].GetInt();
            }

            if (mpVHRoomMonitor && 0== iState/* && IsPushingStream(VHStreamType_Desktop)*/) {
               mpVHRoomMonitor->OnRecvStopDesktopSharing();
            }

         }
         else if ("*setBrush" == inav_event) {
            HandleSetBrushMsg(data);
         }
         else if ("*up_hand_wait_join_mic_delay" == inav_event) {//举手30s超时
            //HandleHandDelay(data);
            //{"join_id":"76457", "type" : "*up_hand_wait_join_mic_delay"}

            std::string target_id;
            if (doc.HasMember("join_id") && doc["join_id"].IsString()) {
               target_id = doc["join_id"].GetString();
            }
            else if (doc.HasMember("join_id") && doc["join_id"].IsInt()) {
               char string[32];
               target_id = itoa(doc["join_id"].GetInt(), string, 10);
               target_id = string;
            }

            if (mpVHRoomMonitor) {
               mpVHRoomMonitor->OnRecvHandDelayMsg(target_id);
            }
         }
         else if ("*startCountdown" == inav_event) {//主讲人发起倒计时 
            //{"type":"*startCountdown","join_id":395758,"time":120}
            std::string join_id;
            if (doc.HasMember("join_id") && doc["join_id"].IsString()) {
               join_id = doc["join_id"].GetString();
            }
            else if (doc.HasMember("join_id") && doc["join_id"].IsInt()) {
               char string[32];
               join_id = itoa(doc["join_id"].GetInt(), string, 10);
               join_id = string;
            }

            int iTime = 0;
            if (doc.HasMember("time") && doc["time"].IsString()) {
               std::string strNum = doc["time"].GetString();
               iTime = atoi(strNum.c_str());
            }
            else if (doc.HasMember("time") && doc["time"].IsInt()) {
               char string[32];
               iTime = doc["time"].GetInt();
            }

            int iplayEndSound = 0;
            if (doc.HasMember("playEndSound") && doc["playEndSound"].IsString()) {
               std::string strNum = doc["playEndSound"].GetString();
               iplayEndSound = atoi(strNum.c_str());
            }
            else if (doc.HasMember("playEndSound") && doc["playEndSound"].IsInt()) {
               char string[32];
               iplayEndSound = doc["playEndSound"].GetInt();
            }


            if (mpVHRoomMonitor) {
               mpVHRoomMonitor->OnRecvStartCountdown(join_id, iplayEndSound, iTime);
            }
         }
         else if ("*cancelCountdown" == inav_event) {//取消倒计时  
            //{"type":"*cancelCountdown","join_id":395758}

            std::string join_id;
            if (doc.HasMember("join_id") && doc["join_id"].IsString()) {
               join_id = doc["join_id"].GetString();
            }
            else if (doc.HasMember("join_id") && doc["join_id"].IsInt()) {
               char string[32];
               join_id = itoa(doc["join_id"].GetInt(), string, 10);
               join_id = string;
            }

            if (mpVHRoomMonitor) {
               mpVHRoomMonitor->OnRecvCancelCountdown(join_id);
            }
         }
         else if ("*rewardStudent" == inav_event) {//奖杯数量增加
            //HandleHandDelay(data);
            //{"type":"*rewardStudent","nick_name":"xxxxxx","target_join_id":"78915","reward_num":"1"}

            std::string target_id;
            std::string nickName;
            if (doc.HasMember("target_join_id") && doc["target_join_id"].IsString()) {
               target_id = doc["target_join_id"].GetString();
            }
            else if (doc.HasMember("target_join_id") && doc["target_join_id"].IsInt()) {
               char string[32];
               target_id = itoa(doc["target_join_id"].GetInt(), string, 10);
               target_id = string;
            }

            if (doc.HasMember("nick_name") && doc["nick_name"].IsString()) {
               nickName = doc["nick_name"].GetString();
            }

            int iFavorNum = 0;
            if (doc.HasMember("reward_num") && doc["reward_num"].IsString()) {
               std::string strNum = doc["reward_num"].GetString();
               iFavorNum = atoi(strNum.c_str());
            }
            else if (doc.HasMember("reward_num") && doc["reward_num"].IsInt()) {
               iFavorNum = doc["reward_num"].GetInt();
            }

            if (mpVHRoomMonitor) {
               mpVHRoomMonitor->OnRecvHandRewardStu(target_id, nickName, iFavorNum);
            }
         }
         else if ("*cancelHand" == inav_event) {//取消举手
            //handelCancelHand(doc);//handelCancelHand(const rapidjson::Document& doc) ;
            //["cmd", { "type":"*cancelHand","join_id" : 395758,"role_type" : 2,"nick_name" : "test","publish_release_time" : "2019-07-26 16:58:09" }]
            std::string strJoinId;
            int iRoleType;
            std::string strNickName;
            std::string strTime;

            if (doc.HasMember("join_id") && doc["join_id"].IsString()) {
               strJoinId = doc["join_id"].GetString();
            }
            else if (doc.HasMember("join_id") && doc["join_id"].IsInt())
            {
               int iJoinId = doc["join_id"].GetInt();
               strJoinId = std::to_string(iJoinId);
            }

            if (doc.HasMember("role_type") && doc["role_type"].IsInt())
            {
               iRoleType = doc["role_type"].GetInt();
            }
            else if (doc.HasMember("role_type") && doc["role_type"].IsString()) {
               std::string strRoleType = doc["role_type"].GetString();
               iRoleType = std::stoi(strRoleType);
            }

            if (doc.HasMember("nick_name") && doc["nick_name"].IsString()) {
               strNickName = doc["nick_name"].GetString();
            }

            if (doc.HasMember("publish_release_time") && doc["publish_release_time"].IsString()) {
               strTime = doc["publish_release_time"].GetString();
            }

            if (mpVHRoomMonitor) {


               mpVHRoomMonitor->OnRecvCancelHand(strJoinId,
                  iRoleType,
                  strNickName,
                  strTime);
            }
         }
         else if ("*startDoubleVrtc" == inav_event) {//开始双推广播
            //{"type":"*startDoubleVrtc","join_id":395758,"user":{"nick_name":"张三"}}

            std::string join_id;
            std::string nickName;
            if (doc.HasMember("join_id") && doc["join_id"].IsString()) {
               join_id = doc["join_id"].GetString();
            }
            else if (doc.HasMember("join_id") && doc["join_id"].IsInt()) {

               int iJoin_id = doc["join_id"].GetInt();
               join_id = std::to_string(iJoin_id);
            }


            if (doc.HasMember("user") && doc["user"].IsObject()) {
               rapidjson::Value& user = doc["user"];

               if (user.HasMember("nick_name") && user["nick_name"].IsString()) {
                  nickName = user["nick_name"].GetString();

               }
            }

            if (mpVHRoomMonitor) {
               mpVHRoomMonitor->OnRecvStartDoubleVrtc(join_id, nickName);
            }
         }
         else if ("*inviteDoubleVrtc" == inav_event) {//开始双推广播
            //{"type":"*inviteDoubleVrtc","join_id":395758,"target_id",111111,"user":{"nick_name":"张三"}}

            //std::string join_id;
            //std::string nickName;
            //if (doc.HasMember("join_id") && doc["join_id"].IsString()) {
            //   join_id = doc["join_id"].GetString();
            //}
            //else if (doc.HasMember("join_id") && doc["join_id"].IsInt()) {

            //   int iJoin_id = doc["join_id"].GetInt();
            //   join_id = std::to_string(iJoin_id);
            //}

            std::string target_id;

            if (doc.HasMember("target_id") && doc["target_id"].IsString()) {
               target_id = doc["target_id"].GetString();
            }
            else if (doc.HasMember("target_id") && doc["target_id"].IsInt()) {
               int iJoin_id = doc["target_id"].GetInt();
               target_id = std::to_string(iJoin_id);
            }


            /*          if (doc.HasMember("user") && doc["user"].IsObject()) {
                         rapidjson::Value& user = doc["user"];

                         if (user.HasMember("nick_name") && user["nick_name"].IsString()) {
                            std::string nickName = user["nick_name"].GetString();

                         }
                      }*/

            if (mpVHRoomMonitor) {
               mpVHRoomMonitor->OnRecvInviteDoubleVrtc(target_id);
            }
         }
         else if ("*inviteScreenSharing" == inav_event) {//邀请桌面共享双推
            //{"type":"*inviteScreenSharing", "join_id" : 395758, "target_id", 111111}

            //std::string join_id;
            //std::string nickName;
            //if (doc.HasMember("join_id") && doc["join_id"].IsString()) {
            //   join_id = doc["join_id"].GetString();
            //}
            //else if (doc.HasMember("join_id") && doc["join_id"].IsInt()) {

            //   int iJoin_id = doc["join_id"].GetInt();
            //   join_id = std::to_string(iJoin_id);
            //}

            std::string target_id;

            if (doc.HasMember("target_id") && doc["target_id"].IsString()) {
               target_id = doc["target_id"].GetString();
            }
            else if (doc.HasMember("target_id") && doc["target_id"].IsInt()) {
               int iJoin_id = doc["target_id"].GetInt();
               target_id = std::to_string(iJoin_id);
            }

            if (mpVHRoomMonitor) {
               mpVHRoomMonitor->OnRecvInviteScreenSharing(target_id);
            }
         }
         else if ("*stopDoubleVrtc" == inav_event) {//讲师结束指定学员双推
            //  ["cmd",{"type":"*stopDoubleVrtc","join_id":395758,"target_id",12312312,"user":{"nick_name":"张三"}}]

            std::string target_id;
            std::string nickName;
            if (doc.HasMember("target_id") && doc["target_id"].IsString()) {
               target_id = doc["target_id"].GetString();
            }
            else if (doc.HasMember("target_id") && doc["target_id"].IsInt()) {
               int iJoin_id = doc["target_id"].GetInt();
               target_id = std::to_string(iJoin_id);
            }


            if (doc.HasMember("user") && doc["user"].IsObject()) {
               rapidjson::Value& user = doc["user"];

               if (user.HasMember("nick_name") && user["nick_name"].IsString()) {
                  std::string nickName = user["nick_name"].GetString();

               }
            }

            if (mpVHRoomMonitor) {
               mpVHRoomMonitor->OnRecvEndDoubleVrtc(target_id, nickName);
            }
         }
         else if ("*push_questionnaire" == inav_event) {//推送考试
            if (mpVHRoomMonitor) {
               mpVHRoomMonitor->OnRecvExamination();
            }
         }
		 else if ("*whiteListUserJoinRoom" == inav_event) {
		    std::string phone;
		    std::string class_token;
		    if (doc.HasMember("phone") && doc["phone"].IsString()) {
				phone = doc["phone"].GetString();
		    }
			if (doc.HasMember("class_token") && doc["class_token"].IsString()) {
				class_token = doc["class_token"].GetString();
			}
			if(mPhone == phone && mClassToken != class_token)
			   mpVHRoomMonitor->OnRecvWhiteListUserJoinRoom(phone, class_token);

		 }
         //else if ("*switch_naire_tool_view" == inav_event) {//推送关闭考试

         //if (mpVHRoomMonitor) {
         //   mpVHRoomMonitor->OnRecvExamination();
         //}
         //}

      }
   }
}

void VHClassRoomImpl::HandleSetBigScreen(const rapidjson::Document& doc) {
   std::string join_id;
   std::string target_id;
   int  status;
   if (doc.HasMember("join_id") && doc["join_id"].IsString()) {
      join_id = doc["join_id"].GetString();
   }
   if (doc.HasMember("target_id") && doc["target_id"].IsString()) {
      target_id = doc["target_id"].GetString();
   }
   if(mClassRoomBaseInfo.mUserInfo.role_type != JOIN_ROLE_Teacher){
      mClassRoomBaseInfo.bigScreen = atoi(target_id.c_str());
      //讲师通过接口调用判断是否切换成。
      //此处非讲师收到广播消息进行画面切换。
      if (mpVHRoomMonitor) {
         mpVHRoomMonitor->OnRecvSetBigScreen(target_id);
      }
   }
}

BroadCastVideoProfileIndex VHClassRoomImpl::Dpi2ProfileIndex(const BROAD_CAST_DPI & dip)
{
   BroadCastVideoProfileIndex broadCastProfileIndex = BROADCAST_VIDEO_PROFILE_720P_4x3_25F /*BROADCAST_VIDEO_PROFILE_720P_0*/;
   switch (dip)
   {
   case BROAD_CAST_DPI_1920_1080: {
      broadCastProfileIndex = BROADCAST_VIDEO_PROFILE_1080P_16x9_25F;
      break;
   }
   case BROAD_CAST_DPI_1440_1080: {
      broadCastProfileIndex = BROADCAST_VIDEO_PROFILE_1080P_4x3_25F;
      break;
   }
   case BROAD_CAST_DPI_1280_960: {
      broadCastProfileIndex = BROADCAST_VIDEO_PROFILE_960P_4x3_25F;
      break;
   }
   default:
      broadCastProfileIndex = BROADCAST_VIDEO_PROFILE_720P_4x3_25F;
      break;
   }
   return broadCastProfileIndex;
}



std::string  VHClassRoomImpl::GetLayoutDesc(LayoutMode layout) {
   switch (layout)
   {
   case CANVAS_LAYOUT_PATTERN_GRID_1:
      return "CANVAS_LAYOUT_PATTERN_GRID_1";
   case CANVAS_LAYOUT_PATTERN_GRID_2_H:
      return "CANVAS_LAYOUT_PATTERN_GRID_2_H";
   case CANVAS_LAYOUT_PATTERN_GRID_3_E:
      return "CANVAS_LAYOUT_PATTERN_GRID_3_E";
   case CANVAS_LAYOUT_PATTERN_GRID_3_D:
      return "CANVAS_LAYOUT_PATTERN_GRID_3_D";
   case CANVAS_LAYOUT_PATTERN_GRID_4_M:
      return "CANVAS_LAYOUT_PATTERN_GRID_4_M";
   case CANVAS_LAYOUT_PATTERN_GRID_5_D:
      return "CANVAS_LAYOUT_PATTERN_GRID_5_D";
   case CANVAS_LAYOUT_PATTERN_GRID_6_E:
      return "CANVAS_LAYOUT_PATTERN_GRID_6_E";
   case CANVAS_LAYOUT_PATTERN_GRID_9_E:
      return "CANVAS_LAYOUT_PATTERN_GRID_9_E";
   case CANVAS_LAYOUT_PATTERN_FLOAT_2_1DR:
      return "CANVAS_LAYOUT_PATTERN_FLOAT_2_1DR";
   case CANVAS_LAYOUT_PATTERN_FLOAT_2_1DL:
      return "CANVAS_LAYOUT_PATTERN_FLOAT_2_1DL";
   case CANVAS_LAYOUT_PATTERN_FLOAT_3_2DL:
      return "CANVAS_LAYOUT_PATTERN_FLOAT_3_2DL";
   case CANVAS_LAYOUT_PATTERN_FLOAT_6_5D:
      return "CANVAS_LAYOUT_PATTERN_FLOAT_6_5D";
   case CANVAS_LAYOUT_PATTERN_FLOAT_6_5T:
      return "CANVAS_LAYOUT_PATTERN_FLOAT_6_5T";
   case CANVAS_LAYOUT_PATTERN_TILED_5_1T4D:
      return "CANVAS_LAYOUT_PATTERN_TILED_5_1T4D";
   case CANVAS_LAYOUT_PATTERN_TILED_5_1D4T:
      return "CANVAS_LAYOUT_PATTERN_TILED_5_1D4T";
   case CANVAS_LAYOUT_PATTERN_TILED_5_1L4R:
      return "CANVAS_LAYOUT_PATTERN_TILED_5_1L4R";
   case CANVAS_LAYOUT_PATTERN_TILED_5_1R4L:
      return "CANVAS_LAYOUT_PATTERN_TILED_5_1R4L";
   case CANVAS_LAYOUT_PATTERN_TILED_6_1T5D:
      return "CANVAS_LAYOUT_PATTERN_TILED_6_1T5D";
   case CANVAS_LAYOUT_PATTERN_TILED_6_1D5T:
      return "CANVAS_LAYOUT_PATTERN_TILED_6_1D5T";
   case CANVAS_LAYOUT_PATTERN_TILED_9_1L8R:
      return "CANVAS_LAYOUT_PATTERN_TILED_9_1L8R";
   case CANVAS_LAYOUT_PATTERN_TILED_9_1R8L:
      return "CANVAS_LAYOUT_PATTERN_TILED_9_1R8L";
   case CANVAS_LAYOUT_PATTERN_TILED_13_1L12R:
      return "CANVAS_LAYOUT_PATTERN_TILED_13_1L12R";
   case CANVAS_LAYOUT_PATTERN_TILED_17_1TL16GRID:
      return "CANVAS_LAYOUT_PATTERN_TILED_17_1TL16GRID";
   case CANVAS_LAYOUT_PATTERN_TILED_9_1D8T:
      return "CANVAS_LAYOUT_PATTERN_TILED_9_1D8T";
   case CANVAS_LAYOUT_PATTERN_TILED_13_1TL12GRID:
      return "CANVAS_LAYOUT_PATTERN_TILED_13_1TL12GRID";
   case CANVAS_LAYOUT_PATTERN_TILED_17_1TL16GRID_E:
      return "CANVAS_LAYOUT_PATTERN_TILED_17_1TL16GRID_E";
   case CANVAS_LAYOUT_PATTERN_CUSTOM:
      return "CANVAS_LAYOUT_PATTERN_CUSTOM";
   default:
      break;
   }
   return std::string();
}

void VHClassRoomImpl::SetBroadCastCallBack(const std::string result, const std::string& msg) {
   LOGD("msg:%s", msg.c_str());
   if (result.compare("success") == 0) {
      if (mpVHRoomMonitor) {
         LOGD("msg:%s", msg.c_str());
         mpVHRoomMonitor->OnCallbackEvent(RoomEvent_Start_MixStream);
      }
      //api状态上报

   }
   else {
      if (mpVHRoomMonitor) {
         LOGD("msg:%s", msg.c_str());
         mpVHRoomMonitor->OnCallbackEvent(RoomEvent_Start_MixStream, -1, msg);
      }
   }
}

void VHClassRoomImpl::SetWhiteListPhone(const std::string& phone) {
	mPhone = phone;
}

void VHClassRoomImpl::SetBroadCastCallBackWithParam(int layOutType, const std::string result, const std::string& msg) {
   //如果设置布局成功
   if (result.compare("success") == 0) {
      //如果没带有大画面的处理
      if (mCurrentBroadCastInfo.stream_id.length() == 0) {
         if (mpVHRoomMonitor) {
            LOGD("msg:%s", msg.c_str());
            mpVHRoomMonitor->OnSetConfigBroadEvent(layOutType, CALL_BACK_SUC, result);
         }
         //api状态上报

      }
      else {
         VHRoomMonitor* pMonitor = mpVHRoomMonitor;
         if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
            //带有设置大画面
            mpWebRtcSDKInterface->SetMixLayoutMainView(mCurrentBroadCastInfo.stream_id, 
               [&, pMonitor](const std::string& result, const std::string& msg)->void {
                  LOGD("%s msg:%s", __FUNCTION__, msg.c_str());
                  if (result.compare("success") == 0) {
                     //api状态上报

                     if (pMonitor) {
                        LOGD("%s msg:%s", __FUNCTION__, msg.c_str());
                        pMonitor->OnSetConfigBroadEvent(layOutType, 200, msg);
                     }
                  }
                  else {
                     if (pMonitor) {
                        LOGD("%s msg:%s", __FUNCTION__, msg.c_str());
                        pMonitor->OnSetConfigBroadEvent(layOutType, -1, msg);
                     }
                  }
               }
            );
         }
         else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
            mpWebRtcSDKInterface->SetMixLayoutMainView(mCurrentBroadCastInfo.stream_id, [&, pMonitor](const std::string& result, const std::string& msg)->void {
               LOGD("%s msg:%s", __FUNCTION__, msg.c_str());
               if (result.compare("success") == 0) {
                  //api状态上报

                  if (pMonitor) {
                     LOGD("%s msg:%s", __FUNCTION__, msg.c_str());
                     pMonitor->OnSetConfigBroadEvent(layOutType, 200, msg);
                  }
               }
               else {
                  if (pMonitor) {
                     LOGD("%s msg:%s", __FUNCTION__, msg.c_str());
                     pMonitor->OnSetConfigBroadEvent(layOutType, -1, msg);
                  }
               }
            });
         }
      }
   }
   else {
      if (mpVHRoomMonitor) {
         LOGD("msg:%s", msg.c_str());
         mpVHRoomMonitor->OnSetConfigBroadEvent(layOutType, -1, msg);
      }
   }
}

bool VHClassRoomImpl::IsVHMediaConnected() {
   LOGD("IsVHMediaConnected");
   bool bRet = false;
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      bRet = mpWebRtcSDKInterface->IsWebRtcRoomConnected();
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      bRet = mpPaasInteraction->IsWebRtcRoomConnected();
   }
   return bRet;
}
/*  开始摄像头\麦克风采集
*   回调检测 OnStartLocalCapture OnCameraCaptureErr  OnLocalMicOpenErr
*/
int VHClassRoomImpl::StartLocalCapture(std::string devId, VideoProfileIndex index) {
   LOGD("%s %d", __FUNCTION__, index);
   int nRet = false;
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      nRet = mpWebRtcSDKInterface->StartLocalCapture(devId, index, true);
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      nRet = mpPaasInteraction->StartLocalCapture(devId, index, true);
   }
   videoProfile = index;
   LOGD("StartLocalCapture :%d", nRet);
   return nRet;
}

int VHClassRoomImpl::StartLocalCapturePicture(const std::string filePath, VideoProfileIndex index) {
   LOGD("StartLocalCapturePicture %d", index);
   int nRet = false;
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      nRet = mpWebRtcSDKInterface->StartLocalCapturePicture(filePath, index, false);
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      nRet = mpPaasInteraction->StartLocalCapturePicture(filePath, index, false);
   }
   videoProfile = index;
   LOGD("StartLocalCapturePicture :%d", nRet);
   return nRet;
}

int VHClassRoomImpl::StartLocalAuxiliaryCapture(const std::string devId, VideoProfileIndex index) {
   LOGD("%s %d", __FUNCTION__, index);
   int nRet = false;
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      nRet = mpWebRtcSDKInterface->StartLocalAuxiliaryCapture(devId, index);
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      nRet = mpPaasInteraction->StartLocalAuxiliaryCapture(devId, index);
   }
   LOGD("StartLocalAuxiliaryCapture :%d", nRet);
   return nRet;
}

void VHClassRoomImpl::StopLocalAuxiliaryCapture() {
   LOGD("StopLocalAuxiliaryCapture");
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      mpWebRtcSDKInterface->StopLocalAuxiliaryCapture();
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      mpPaasInteraction->StopLocalAuxiliaryCapture();
   }
   LOGD("StopLocalAuxiliaryCapture end");
   return;
}

/*停止摄像头\麦克风采集*/
void VHClassRoomImpl::StopLocalCapture() {
   LOGD("StopLocalCapture");
   //bool bRet = false;
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      mpWebRtcSDKInterface->StopLocalCapture();
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      mpPaasInteraction->StopLocalCapture();
   }
}

/*开始摄像头数据推流  回调检测：OnStartPushLocalStream*/
int VHClassRoomImpl::StartPushLocalStream() {
   LOGD("enter");
   int nRet = VhallLive_OK;
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      if (!mpWebRtcSDKInterface->IsWebRtcRoomConnected()) {
         LOGD("VhallLive_ROOM_DISCONNECT ");
         return VhallLive_ROOM_DISCONNECT;
      }
        nRet = mpWebRtcSDKInterface->StartPushLocalStream();
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      nRet = mpPaasInteraction->StartPushLocalStream();
   }

   return nRet;
}

/*停止摄像头数据推流 回调检测：OnStopPushLocalStream*/
int VHClassRoomImpl::StopPushLocalStream() {
   LOGD("enter");
   int bRet = VhallLiveErrCode::VhallLive_ERROR;
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      bRet = mpWebRtcSDKInterface->StopPushLocalStream();
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      bRet = mpPaasInteraction->StopPushLocalStream();
   }
   return bRet;
}

int VHClassRoomImpl::StartPushLocalAuxiliaryStream(std::string context) {
   LOGD("enter");
   int bRet = VhallLiveErrCode::VhallLive_ERROR;
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      bRet = mpWebRtcSDKInterface->StartPushLocalAuxiliaryStream(context);
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      bRet = mpPaasInteraction->StartPushLocalAuxiliaryStream(context);
   }
   LOGD("end");
   return bRet;
}
/*
*   停止辅助摄像头数据推流
*   回调检测：OnStopPushLocalAuxiliaryStream
*/
int VHClassRoomImpl::StopPushLocalAuxiliaryStream() {
   LOGD("enter");
   int bRet = VhallLiveErrCode::VhallLive_ERROR;
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      bRet = mpWebRtcSDKInterface->StopPushLocalAuxiliaryStream();
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      bRet = mpPaasInteraction->StopPushLocalAuxiliaryStream();
   }
   LOGD("end");
   return bRet;
}

bool VHClassRoomImpl::IsPushingStream(VHStreamType streamType) {
   LOGD("enter");
   bool bRet = false;
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      bRet = mpWebRtcSDKInterface->IsPushingStream(streamType);
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      bRet = mpPaasInteraction->IsPushingStream(streamType);
   }
   LOGD("end");
   return bRet;
}

bool VHClassRoomImpl::IsExitSubScribeStream(const vlive::VHStreamType& streamType)
{
   LOGD("enter");
   bool bRet = false;
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      bRet = mpWebRtcSDKInterface->IsExitSubScribeStream(streamType);
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      bRet = mpPaasInteraction->IsExitSubScribeStream(streamType);
   }
   return bRet;
}

bool VHClassRoomImpl::IsExitSubScribeStream(const std::string& id, const vlive::VHStreamType& streamType)
{
   LOGD("enter");
   bool bRet = false;
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      bRet = mpWebRtcSDKInterface->IsExitSubScribeStream(id, streamType);
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      bRet = mpPaasInteraction->IsExitSubScribeStream(id, streamType);
   }
   return bRet;
}

bool VHClassRoomImpl::IsCapturingStream(VHStreamType streamType) {
   LOGD("enter");
   bool bRet = false;
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      bRet = mpWebRtcSDKInterface->IsCapturingStream(streamType);
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      bRet = mpPaasInteraction->IsCapturingStream(streamType);
   }
   return bRet;
}

int VHClassRoomImpl::ChangeToToolPage(ToolPage pageIndex) {
   if (mClassRoomBaseInfo.mUserInfo.role_type != JOIN_ROLE_Teacher) {
      return ROLE_TYPE_ERROR;
   }
   switch (pageIndex)
   {
   case vlive::ToolPage_Doc:
   case vlive::ToolPage_Board: {
      rapidjson::Document jsonDoc;
      rapidjson::Document::AllocatorType &allocator = jsonDoc.GetAllocator();
      rapidjson::Value CmdRoot(rapidjson::kObjectType);
      rapidjson::Value type(StringRef("*docMode"));
      rapidjson::Value status(vlive::ToolPage_Doc == pageIndex ? StringRef("0") : StringRef("1"));
      CmdRoot.AddMember("type", type, allocator);
      CmdRoot.AddMember("status", status, allocator);
      //rapidjson::Value ApplyRoot(rapidjson::kObjectType);
      //ApplyRoot.AddMember("cmd", CmdRoot, allocator);

      StringBuffer buffer;
      rapidjson::Writer<StringBuffer> writer(buffer);
      CmdRoot.Accept(writer);
      std::string data = buffer.GetString();

      HTTP_GET_REQUEST httpGet(mURLGenerator.GetSendNoticeUrl(mMessageInfo.token, "cmd", data));
      if (mpHttpManagerInterface) {
         httpGet.SetHttpPost(true);
         mpHttpManagerInterface->HttpGetRequest(httpGet, [&](std::string data, int code, const std::string userData)->void {
            LOGD("data:%s code:%d ", data.c_str(), code);
         });
      }
      break;
   }
   case vlive::ToolPage_Answer_Open:
   case vlive::ToolPage_Answer_Close: {
      rapidjson::Document jsonDoc;
      rapidjson::Document::AllocatorType &allocator = jsonDoc.GetAllocator();
      rapidjson::Value CmdRoot(rapidjson::kObjectType);
      rapidjson::Value type(ToolPage_Answer_Open == pageIndex ? StringRef("*create_question") : StringRef("*clear_question"));
      rapidjson::Value join_id(StringRef(mClassRoomBaseInfo.mUserInfo.join_id.c_str()));
      CmdRoot.AddMember("type", type, allocator);
      CmdRoot.AddMember("join_id", join_id, allocator);
      //rapidjson::Value ApplyRoot(rapidjson::kObjectType);
      //ApplyRoot.AddMember("cmd", CmdRoot, allocator);

      StringBuffer buffer;
      rapidjson::Writer<StringBuffer> writer(buffer);
      CmdRoot.Accept(writer);
      std::string data = buffer.GetString();

      HTTP_GET_REQUEST httpGet(mURLGenerator.GetSendNoticeUrl(mMessageInfo.token, "cmd", data));
      if (mpHttpManagerInterface) {
         httpGet.SetHttpPost(true);
         mpHttpManagerInterface->HttpGetRequest(httpGet, [&](std::string data, int code, const std::string userData)->void {
            LOGD("data:%s code:%d ", data.c_str(), code);
         });
      }
      break;
   }
                                      break;
   default:
      break;
   }
   return 0;
}

///**
//*   当前互动房间是否存在桌面共享视频流
//*/
//bool VHClassRoomImpl::IsUserPushingDesktopStream() {
//    LOGD("enter");
//    bool bRet = false;
//    if (mpWebRtcSDKInterface) {
//        bRet = mpWebRtcSDKInterface->IsUserPushingDesktopStream();
//    }
//    return bRet;
//}
///**
//*   当前互动房间是否存在插播视频流
//*/
//bool VHClassRoomImpl::IsUserPushingMediaFileStream() {
//    LOGD("enter");
//    bool bRet = false;
//    if (mpWebRtcSDKInterface) {
//        bRet = mpWebRtcSDKInterface->IsUserPushingMediaFileStream();
//    }
//    return bRet;
//}

std::string VHClassRoomImpl::GetSubScribeStreamId(const std::wstring& user, vlive::VHStreamType streamType) {
   LOGD("enter  streamType:%d", streamType);
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      return mpWebRtcSDKInterface->GetSubScribeStreamId(user, streamType);
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      return mpPaasInteraction->GetSubScribeStreamId(user, streamType);
   }
}

int VHClassRoomImpl::GetSubScribeStreamFormat(const std::wstring& user, vlive::VHStreamType streamType, bool &hasVideo, bool &hasAudio) {
   LOGD("enter  streamType:%d", streamType);
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      return mpWebRtcSDKInterface->GetSubScribeStreamFormat(user, streamType, hasVideo, hasAudio);
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      return mpPaasInteraction->GetSubScribeStreamFormat(user, streamType, hasVideo, hasAudio);
   }
   LOGD("end streamType:%d", streamType);
   return -1;
}

bool VHClassRoomImpl::StartRenderLocalStream(vlive::VHStreamType streamType, void* wnd) {
   LOGD("enter  streamType:%d wnd %p", streamType, wnd);
   bool bRet = false;
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      bRet = mpWebRtcSDKInterface->StartRenderLocalStream(streamType, wnd);
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      bRet = mpPaasInteraction->StartRenderLocalStream(streamType, wnd);
   }
   return bRet;
}

bool VHClassRoomImpl::StartRenderLocalStreamByInterface(vlive::VHStreamType streamType, std::shared_ptr<vhall::VideoRenderReceiveInterface> receive)
{
   LOGD("enter  streamType:%d", streamType/*, receive, receive*/);
   bool bRet = false;
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      bRet = mpWebRtcSDKInterface->StartRenderLocalStreamByInterface(streamType, receive);
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      bRet = mpPaasInteraction->StartRenderLocalStreamByInterface(streamType, receive);
   }
   return bRet;
}

bool VHClassRoomImpl::IsStreamExit(std::string id)
{
   //LOGD("enter  streamType:%d", streamType/*, receive, receive*/);
   bool bRet = false;
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      bRet = mpWebRtcSDKInterface->IsStreamExit(id);
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      bRet = mpPaasInteraction->IsStreamExit(id);
   }
   return bRet;
}

bool VHClassRoomImpl::IsRemoteStreamIsExist(const std::wstring & user, vlive::VHStreamType streamType)
{
   LOGD("enter %s  streamType:%d",__FUNCTION__, streamType/*, receive, receive*/);
   bool bRet = false;
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      bRet = mpWebRtcSDKInterface->IsRemoteStreamIsExist(user, streamType);
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      bRet = mpPaasInteraction->IsRemoteStreamIsExist(user, streamType);
   }
   return bRet;
}

bool VHClassRoomImpl::StartRenderRemoteStream(const std::wstring& user, vlive::VHStreamType streamType, void* wnd) {
   LOGD("enter user:%ws streamType:%d", user.c_str(), streamType);
   bool bRet = false;
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      bRet = mpWebRtcSDKInterface->StartRenderRemoteStream(user, streamType, wnd);
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      bRet = mpPaasInteraction->StartRenderSubscribeStream(user, streamType, wnd);
   }
   return bRet;
}

bool VHClassRoomImpl::StartRenderRemoteStreamByInterface(const std::wstring& user, vlive::VHStreamType streamType, std::shared_ptr<vhall::VideoRenderReceiveInterface> receive)
{
   LOGD("enter user:%ws streamType:%d", user.c_str(), streamType);
   bool bRet = false;
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      bRet = mpWebRtcSDKInterface->StartRenderRemoteStreamByInterface(user, streamType, receive);
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      bRet = mpPaasInteraction->StartRenderRemoteStreamByInterface(user, streamType, receive);
   }
   return bRet;
}

/*
*   停止渲染远端流
*/
bool VHClassRoomImpl::StopRenderRemoteStream(const std::wstring& user, const std::string streamId, vlive::VHStreamType streamType) {
   LOGD("enter user:%ws streamType:%d", user.c_str(), streamType);
   bool bRet = false;
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      bRet = mpWebRtcSDKInterface->StopRenderRemoteStream(user, streamId, streamType);
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      bRet = mpPaasInteraction->StopRenderRemoteStream(user, streamId, streamType);
   }
   return bRet;
}

bool VHClassRoomImpl::ChangeSubScribeUserSimulCast(const std::wstring& user_id, vlive::VHStreamType streamType, VHSimulCastType type) {
   LOGD("user:%ws  type:%d", user_id.c_str(), type);
   bool bRet = false;
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      bRet = mpWebRtcSDKInterface->ChangeSubScribeUserSimulCast(user_id, streamType, type);
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      bRet = mpPaasInteraction->ChangeSubScribeUserSimulCast(user_id, streamType, type);
   }
   return bRet;
}

/*开始桌面共享采集  回调检测：OnStartDesktopCaptureSuc OnStartDesktopCaptureErr*/
int VHClassRoomImpl::StartDesktopCapture(int64_t index, VideoProfileIndex profileIndex) {
   LOGD("enter %s index:%d profileIndex %d", __FUNCTION__, index, profileIndex);
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      return mpWebRtcSDKInterface->StartDesktopCapture(index, profileIndex);
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      return mpPaasInteraction->StartDesktopCapture(index, profileIndex);
   }
   return VhallLive_NO_OBJ;
}

/*停止桌面共享采集*/
void VHClassRoomImpl::StopDesktopCapture() {
   LOGD("enter");
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      //return 
      mpWebRtcSDKInterface->StopDesktopCapture();
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      mpPaasInteraction->StopDesktopCapture();
   }
}
/*开始桌面共享采集推流 回调检测：OnStartPushDesktopStream*/
int VHClassRoomImpl::StartPushDesktopStream(std::string context /*= std::string()*/) {
   LOGD("enter");
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      if ( !mpWebRtcSDKInterface->IsWebRtcRoomConnected()) {
         LOGD("VhallLive_ROOM_DISCONNECT ");
         return VhallLive_ROOM_DISCONNECT;
      }
      else {
         //推流之前判断自己是否拥有权限。
         mpWebRtcSDKInterface->StartPushDesktopStream(context);
      }
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      mpPaasInteraction->StartPushDesktopStream(context);
   }
   return VhallLive_OK;
}

/*停止桌面共享采集推流 */
int VHClassRoomImpl::StopPushDesktopStream() {
   LOGD("enter");
   int bRet = VhallLive_ERROR;
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      bRet = mpWebRtcSDKInterface->StopPushDesktopStream();
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      bRet = mpPaasInteraction->StopPushDesktopStream();
   }
   return bRet;
}

/*开始插播文件*/
int VHClassRoomImpl::StartPlayMediaFile(std::string filePath) {
   LOGD("enter file:%s", filePath.c_str());
   int bRet = VhallLive_ERROR;
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      bRet = mpWebRtcSDKInterface->StartPlayMediaFile(filePath);
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      bRet = mpPaasInteraction->StartPlayMediaFile(filePath);
   }
   return bRet;
}

bool VHClassRoomImpl::IsSupportMediaFormat(CaptureStreamAVType type) {
   LOGD("enter type:%d", type);
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      return mpWebRtcSDKInterface->IsSupportMediaFormat(type);
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      return mpPaasInteraction->IsSupportMediaFormat(type);
   }
   return false;
}

/*停止插播文件*/
void VHClassRoomImpl::StopMediaFileCapture() {
   LOGD("enter ");
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      mpWebRtcSDKInterface->StopMediaFileCapture();
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      mpPaasInteraction->StopMediaFileCapture();
   }
}
/*开始插播文件推流 */
int VHClassRoomImpl::StartPushMediaFileStream() {
   LOGD("enter ");
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      if ( mpWebRtcSDKInterface->IsWebRtcRoomConnected()) {
         GetWebRtcSDKInstance()->StartPushMediaFileStream();
      }
      else if ( !mpWebRtcSDKInterface->IsWebRtcRoomConnected())
      {
         LOGD("VhallLive_ROOM_DISCONNECT ");
         return VhallLive_ROOM_DISCONNECT;
      }
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      return mpPaasInteraction->StartPushMediaFileStream();
   }
   return 0;
}
/*停止插播文件推流 回调检测：OnStopPushMediaFileStream*/
void VHClassRoomImpl::StopPushMediaFileStream() {
   LOGD("enter ");
   //int bRet = VhallLive_ERROR;
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      mpWebRtcSDKInterface->StopPushMediaFileStream();
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      mpPaasInteraction->StopPushMediaFileStream();
   }
}
//插播文件暂停处理
void VHClassRoomImpl::MediaFilePause() {
   LOGD("enter ");
   //nt bRet = VhallLive_ERROR;
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      mpWebRtcSDKInterface->MediaFilePause();
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      mpPaasInteraction->MediaFilePause();
   }
   //return bRet;
}
//插播文件恢复处理
void VHClassRoomImpl::MediaFileResume() {
   LOGD("enter ");
   //int bRet = VhallLive_ERROR;
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      //bRet = 
      mpWebRtcSDKInterface->MediaFileResume();
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      mpPaasInteraction->MediaFileResume();
   }
}
//插播文件快进处理
void VHClassRoomImpl::MediaFileSeek(const unsigned long long& seekTimeInMs) {
   LOGD("enter ");
   
   //int bRet = VhallLive_ERROR;
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      mpWebRtcSDKInterface->MediaFileSeek(seekTimeInMs);
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      mpPaasInteraction->MediaFileSeek(seekTimeInMs);
   }
}

//插播文件总时长
const long long VHClassRoomImpl::MediaFileGetMaxDuration() {
   long long nRet = 0;
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      nRet = mpWebRtcSDKInterface->MediaFileGetMaxDuration();
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      nRet = mpPaasInteraction->MediaFileGetMaxDuration();
   }
   return nRet;
}
//插播文件当前时长
const long long VHClassRoomImpl::MediaFileGetCurrentDuration() {
   long long nRet = 0;
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      nRet = mpWebRtcSDKInterface->MediaFileGetCurrentDuration();
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      nRet = mpPaasInteraction->MediaFileGetCurrentDuration();
   }
   return nRet;
}
//插播文件的当前状态 
//返回值：播放状态  MEDIA_FILE_PLAY_STATE
int VHClassRoomImpl::MediaGetPlayerState() {
   int nRet = 0;
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      nRet = mpWebRtcSDKInterface->MediaGetPlayerState();
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      nRet = mpPaasInteraction->MediaGetPlayerState();
   }
   return nRet;
}
/**获取摄像头列表**/
void VHClassRoomImpl::GetCameraDevices(std::list<vhall::VideoDevProperty> &cameras) {
   LOGD("enter ");
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      mpWebRtcSDKInterface->GetCameraDevices(cameras);
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      mpPaasInteraction->GetCameraDevices(cameras);
   }
}
/**获取麦克风列表**/
void VHClassRoomImpl::GetMicDevices(std::list<vhall::AudioDevProperty> &micDevList) {
   LOGD("enter ");
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      mpWebRtcSDKInterface->GetMicDevices(micDevList);
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      mpPaasInteraction->GetMicDevices(micDevList);
   }
}
/**获取扬声器列表**/
void VHClassRoomImpl::GetPlayerDevices(std::list<vhall::AudioDevProperty> &playerDevList) {
   LOGD("enter ");
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      mpWebRtcSDKInterface->GetPlayerDevices(playerDevList);
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      mpPaasInteraction->GetPlayerDevices(playerDevList);
   }
}

bool VHClassRoomImpl::HasVideoOrAudioDev() {
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      return mpWebRtcSDKInterface->HasVideoOrAudioDev();
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      return mpPaasInteraction->HasVideoOrAudioDev();
   }
   return false;
}

bool VHClassRoomImpl::HasVideoDev()
{
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      return mpWebRtcSDKInterface->HasVideoDev();
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      return mpPaasInteraction->HasVideoDev();
   }
   return false;
}

bool VHClassRoomImpl::HasAudioDev()
{
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      return mpWebRtcSDKInterface->HasAudioDev();
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      return mpPaasInteraction->HasAudioDev();
   }
   return false;
}

/*
*  设置使用的麦克风
*  index: GetMicDevices获取的列表中 VhallLiveDeviceInfo中的 devIndex
*/
void VHClassRoomImpl::SetUsedMic(int index, std::string devId, std::wstring desktopCaptureId) {
   LOGD("enter index:%d", index);
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      mpWebRtcSDKInterface->SetUsedMic(index, devId, desktopCaptureId);
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      mpPaasInteraction->SetUsedMic(index, devId, desktopCaptureId);
   }
}

int VHClassRoomImpl::StartLocalCapturePlayer(const int dev_index, const std::wstring dev_id, const int volume) {
   LOGD("enter index:%d", dev_index);
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      mpWebRtcSDKInterface->StartLocalCapturePlayer(dev_index, dev_id, volume);
      return 0;
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      return mpPaasInteraction->StartLocalCapturePlayer(dev_index, dev_id, volume);
   }
   return -1;
}

int VHClassRoomImpl::StopLocalCapturePlayer() {
   int iRef = -1;
   //LOGD("enter index:%d", dev_index);
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
		mpWebRtcSDKInterface->StopLocalCapturePlayer();
      iRef = 0;
	}
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      iRef = mpPaasInteraction->StopLocalCapturePlayer();
   }
	return iRef;
}

int VHClassRoomImpl::MuteAllSubScribeAudio(bool mute) {
   int iRef = -1;
   LOGD("mute:%d", mute);
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      mpWebRtcSDKInterface->MuteAllSubScribeAudio(mute);
      iRef = 0;
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      iRef = mpPaasInteraction->MuteAllSubScribeAudio(mute);
   }
   return iRef;
}

bool VHClassRoomImpl::GetMuteAllAudio()
{
   bool bRef = false;
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      bRef = mpWebRtcSDKInterface->GetMuteAllAudio();
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      bRef = mpPaasInteraction->GetMuteAllAudio();
   }
   return bRef;
}

void VHClassRoomImpl::GetStreams(std::list<std::string>& streams)
{
   LOGD("enter index:%d");
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      mpWebRtcSDKInterface->GetStreams(streams);
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      mpPaasInteraction->GetStreams(streams);
   }
}

void VHClassRoomImpl::GetCurrentMic(int &index, std::string& devId) {
   LOGD("enter index:%d", index);
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      mpWebRtcSDKInterface->GetCurrentMic(index, devId);
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      mpPaasInteraction->GetCurrentMic(index, devId);
   }
}

void VHClassRoomImpl::GetCurrentCamera(std::string& devId) {
   LOGD("enter devId:%s", devId.c_str());
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      mpWebRtcSDKInterface->GetCurrentCamera(devId);
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      mpPaasInteraction->GetCurrentCamera(devId);
   }
}

void VHClassRoomImpl::ChangeCamera() {
   LOGD(" enter ");
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      std::list<vhall::VideoDevProperty> mCameraList;
      mpWebRtcSDKInterface->GetCameraDevices(mCameraList);
      int index = 0;
      std::string devId;
      mpWebRtcSDKInterface->GetCurrentMic(index, devId);

      //只有一个摄像头或者没有，不进行切换。
      if (mCameraList.size() == 0 || mCameraList.size() == 1) {
         LOGD(" only on return ");
         return;
      }
      std::string curCamerDevId;
      std::wstring playerCaptureId = mpWebRtcSDKInterface->GetCurrentDesktopPlayCaptureId();
      mpWebRtcSDKInterface->GetCurrentCamera(curCamerDevId);
      //当选中的摄像头还有下一个设备，则切换。
      int nCheckCount = 1;
      std::list<vhall::VideoDevProperty>::iterator iter = mCameraList.begin();

      LOGD("  The num of Camera is %d", mCameraList.size());
      while (iter != mCameraList.end()) {
         vhall::VideoDevProperty dev = *iter;
         LOGD("  The cur index of Camera is %d", nCheckCount);
         if (dev.mDevId == curCamerDevId || curCamerDevId.length() == 0) {
            //找到当前正在使用的摄像头。
            //1:如果还有下一个，则切换；
            if (nCheckCount < mCameraList.size() && iter->mDevId.size() > 0) {
               iter++;
               vhall::VideoDevProperty nexDev = *iter;
               StopLocalCapture();
               mpWebRtcSDKInterface->SetUsedMic(index, devId, playerCaptureId);
               mpWebRtcSDKInterface->StartLocalCapture(nexDev.mDevId, videoProfile, true);
               return;
            }
            //2:已经是最后一个,选取第一个；
            else if (nCheckCount == mCameraList.size()) {
               iter = mCameraList.begin();
               if (iter->mDevId.size() > 0) {
                  vhall::VideoDevProperty nexDev = *iter;
                  StopLocalCapture();
                  mpWebRtcSDKInterface->SetUsedMic(index, devId, playerCaptureId);
                  mpWebRtcSDKInterface->StartLocalCapture(nexDev.mDevId, videoProfile, true);
                  return;
               }
            }
         }
         nCheckCount++;
         iter++;
      }
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      mpPaasInteraction->ChangeNextCamera(videoProfile);
   }
   LOGD(" leave ");
}

void VHClassRoomImpl::SubScribeRemoteStream(const std::string &stream_id, int delayTimeOut) {
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      mpWebRtcSDKInterface->SubScribeRemoteStream(stream_id, delayTimeOut);
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      mpPaasInteraction->SubScribeRemoteStream(stream_id, delayTimeOut);
   }
}

/*
*  设置使用的扬声器
*  index: GetPlayerDevices获取的列表中 vhall::AudioDevProperty中的 devIndex
*/
void VHClassRoomImpl::SetUsePlayer(int index, std::string devId) {
   LOGD("enter index:%d", index);
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      mpWebRtcSDKInterface->SetUsedPlay(index, devId);
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      mpPaasInteraction->SetUsePlayer(index, devId);
   }
}

void VHClassRoomImpl::GetCurrentCameraInfo(int &index, std::string& devId) {
   LOGD("enter index:%d", index);
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      mpWebRtcSDKInterface->GetCurrentPlayOut(index, devId);
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      mpPaasInteraction->GetCurrentCameraInfo(index, devId);
   }
}

/**
* 摄像头画面预览，当预览结束之后需要停止预览，否则摄像头将被一直占用
*/
int VHClassRoomImpl::PreviewCamera(void* wnd, const std::string& devGuid, VideoProfileIndex index, int micIndex /*= -1*/) {
   int nRet = 0;
   LOGD("enter index:%d", index);
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      nRet = mpWebRtcSDKInterface->PreviewCamera(wnd, devGuid, index, micIndex);
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      nRet = mpPaasInteraction->PreviewCamera(wnd, devGuid, index, micIndex);
   }
   return nRet;
}


void VHClassRoomImpl::ChangePreViewMic(int micIndex) {
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      mpWebRtcSDKInterface->ChangePreViewMic(micIndex);
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      mpPaasInteraction->ChangePreViewMic(micIndex);
   }
}

int VHClassRoomImpl::GetMicVolumValue() {
   int nRet = 0;
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      nRet = mpWebRtcSDKInterface->GetMicVolumValue();
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      nRet = mpPaasInteraction->GetMicVolumValue();
   }
   return nRet;
}

/*
* 停止摄像头预览
*/
int  VHClassRoomImpl::StopPreviewCamera() {
   int nRet = 0;
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      nRet = mpWebRtcSDKInterface->StopPreviewCamera();
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      nRet = mpPaasInteraction->StopPreviewCamera();
   }
   return nRet;
}

/**设置当前使用的麦克风音量**/
bool VHClassRoomImpl::SetCurrentMicVol(int vol) {
   bool bRef = false;
   LOGD("enter vol:%d", vol);
   if (vol < 0 || vol > 100) {   
   }
   else {

      if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
         mpWebRtcSDKInterface->SetCurrentMicVol(vol);
         bRef = true;
      }
      else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
         bRef = mpPaasInteraction->SetCurrentMicVol(vol);
      }
   }
   return bRef;
}
/**获取当前使用的麦克风音量**/
int VHClassRoomImpl::GetCurrentMicVol() {
   int nRet = 0;
   LOGD("enter nRet:%d", nRet);
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      nRet = mpWebRtcSDKInterface->GetCurrentMicVol();
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      nRet = mpPaasInteraction->GetCurrentMicVol();
   }
   
   return nRet;
}
/**设置当前使用的扬声器音量**/
bool VHClassRoomImpl::SetCurrentPlayVol(int vol) {
   bool bRef = false;
   if (vol < 0 || vol > 100) {
   }
   else {
      LOGD("enter vol:%d", vol);
      if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
         mpWebRtcSDKInterface->SetCurrentPlayVol(vol);
         bRef = true;
      }
      else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
         bRef = mpPaasInteraction->SetCurrentPlayVol(vol);
      }
   }
   return bRef;
}

/**获取当前使用的扬声器音量**/
int VHClassRoomImpl::GetCurrentPlayVol() {
   int nRet = 0;
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      nRet = mpWebRtcSDKInterface->GetCurrentPlayVol();
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      nRet = mpPaasInteraction->GetCurrentPlayVol();
   }
   LOGD("enter nRet:%d", nRet);
   return nRet;
}
//关闭摄像头
bool VHClassRoomImpl::CloseCamera() {
   bool bRet = false;
   LOGD("enter CloseCamera");
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {     
      bRet = mpWebRtcSDKInterface->CloseCamera();
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      bRet = mpPaasInteraction->CloseCamera();
   }
   return bRet;
}
//打开摄像头
bool VHClassRoomImpl::OpenCamera() {
   bool bRet = false;
   LOGD("enter OpenCamera");
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      bRet = mpWebRtcSDKInterface->OpenCamera();
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      bRet = mpPaasInteraction->OpenCamera();
   }
   return bRet;
}

//关闭摄像头
bool VHClassRoomImpl::IsCameraOpen()
{
   bool isOpen = false;
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      isOpen = mpWebRtcSDKInterface->IsCameraOpen();
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      isOpen = mpPaasInteraction->IsCameraOpen();
   }
   return isOpen;
}

bool VHClassRoomImpl::CloseMic() {
   bool bRet = false;
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      bRet = mpWebRtcSDKInterface->CloseMic();
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      bRet = mpPaasInteraction->CloseMic();
   }
   return bRet;
}

/*
*  动态切换当前使用的麦克风.注意使用此接口时是针对已经触发了开始采集。
*/
int VHClassRoomImpl::ChangeUserMicDev(int index) {
   int bRet = 0;
   //if (mpWebRtcSDKInterface)
   //{
   //    bRet = mpWebRtcSDKInterface->ChangeUserMicDev(index);
   //}
   return bRet;
}

/*
*  动态切换当前使用的扬声器 .注意使用此接口时是针对已经触发了开始采集。
*/
int VHClassRoomImpl::ChangeUserPlayDev(int index) {
   int bRet = 0;
   //if (mpWebRtcSDKInterface)
   //{
   //    bRet = mpWebRtcSDKInterface->ChangeUserPlayDev(index);
   //}
   return bRet;
}

/*
*   打开麦克风
*/
bool VHClassRoomImpl::OpenMic() {
   bool bRet = false;
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      bRet = mpWebRtcSDKInterface->OpenMic();
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      bRet = mpPaasInteraction->OpenMic();
   }
   return bRet;
}

//打开摄像头
bool VHClassRoomImpl::IsMicOpen()
{
   bool isOpen = false;
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      isOpen = mpWebRtcSDKInterface->IsMicOpen();
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      isOpen = mpPaasInteraction->IsMicOpen();
   }
   return isOpen;
}

int VHClassRoomImpl::PlayAudioFile(std::string fileName, int devIndex) {
   int nRet = VhallLive_NO_OBJ;
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      nRet = mpWebRtcSDKInterface->PlayAudioFile(fileName, devIndex);
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      nRet = mpPaasInteraction->PlayAudioFile(fileName, devIndex);
   }
   return nRet;
}

int VHClassRoomImpl::GetPlayAudioFileVolum() {
   int nRet = VhallLive_NO_OBJ;
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      nRet = mpWebRtcSDKInterface->GetPlayAudioFileVolum();
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      nRet = mpPaasInteraction->GetPlayAudioFileVolum();
   }
   return nRet;
}

int VHClassRoomImpl::StopPlayAudioFile() {
   int nRet = VhallLive_NO_OBJ;
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      nRet = mpWebRtcSDKInterface->StopPlayAudioFile();
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      nRet = mpPaasInteraction->StopPlayAudioFile();
   }
   return nRet;
}

/*
*    获取窗口共享和桌面共享列表
**  vlive::VHStreamType      // 3:Screen,5:window
*/
std::vector<DesktopCaptureSource> VHClassRoomImpl::GetDesktops(int streamType) {
   std::vector<DesktopCaptureSource> desktops;
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      desktops = mpWebRtcSDKInterface->GetDesktops(streamType);
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      desktops = mpPaasInteraction->GetDesktops(streamType);
   }
   return desktops;
}

/*设置选择的软件源*/
int VHClassRoomImpl::SelectSource(int64_t id)
{
   int iRef = VhallLive_ERROR;
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      iRef = mpWebRtcSDKInterface->SelectSource(id);
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      iRef = mpPaasInteraction->SelectSource(id);
   }
   return iRef;
}

/*停止软件源共享采集*/
void VHClassRoomImpl::StopSoftwareCapture()
{
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      mpWebRtcSDKInterface->StopSoftwareCapture();
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      mpPaasInteraction->StopSoftwareCapture();
   }
}

int VHClassRoomImpl::SetLocalCapturePlayerVolume(const int volume) {
   int iRef = VhallLive_ERROR;
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      iRef = mpWebRtcSDKInterface->SetLocalCapturePlayerVolume(volume);
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      iRef = mpPaasInteraction->SetLocalCapturePlayerVolume(volume);
   }
   return iRef;
}

/*停止软件共享采集推流*/
int VHClassRoomImpl::StopPushSoftWareStream()
{
   int iRef = VhallLive_ERROR;
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      iRef = mpWebRtcSDKInterface->StopPushSoftWareStream();
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      iRef = mpPaasInteraction->StopPushSoftWareStream();
   }
   return iRef;
}
/*开始软件共享采集推流*/
int VHClassRoomImpl::StartPushSoftWareStream()
{
   int iRef = VhallLive_ERROR;
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      iRef = mpWebRtcSDKInterface->StartPushSoftWareStream();
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      iRef = mpPaasInteraction->StartPushSoftWareStream();
   }
   return iRef;
}
///**
//*   当前互动房间是否存在软件视频流
//*/
//bool VHClassRoomImpl::IsUserPushingSoftwareStream()
//{
//	bool bRef = false;
//	if (mpWebRtcSDKInterface)
//	{
//		bRef = mpWebRtcSDKInterface->IsUserPushingSoftwareStream();
//	}
//	return bRef;
//
//}

void VHClassRoomImpl::UploadReportSDKLogin(std::string appid, std::string thrid_user_id) {
   LARGE_INTEGER currentTime;
   QueryPerformanceCounter(&currentTime);

   unsigned long long timeId = currentTime.LowPart;
   char timeStamp[64] = { 0 };
   sprintf_s(timeStamp, "%lld", timeId);
   rapidjson::Document document;
   document.SetObject();
   rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

   std::string system = "windows";
   rapidjson::Value userSystemname(StringRef(system.c_str()));
   document.AddMember("imei", userSystemname, allocator);

   std::string systemPf = "6";
   rapidjson::Value userSystemPf(StringRef(systemPf.c_str()));
   document.AddMember("pf", userSystemPf, allocator);

   std::string ver = CLASS_SSDK_VER;
   rapidjson::Value sdkVer(StringRef(ver.c_str()));
   document.AddMember("cv", sdkVer, allocator);

   rapidjson::Value sdkAppid(StringRef(appid.c_str()));
   document.AddMember("app_id", sdkAppid, allocator);


   StringBuffer buffer;
   rapidjson::Writer<StringBuffer> writer(buffer);
   document.Accept(writer);
   std::string data = buffer.GetString();
   LOGD("OnSocketOpen  SendMsg:%s", data.c_str());

   //mLogReport.SendLog(SDK_LOGIN, timeStamp, thrid_user_id, "1", data);
}

void VHClassRoomImpl::UploadReportSDKInit() {
   LARGE_INTEGER currentTime;
   QueryPerformanceCounter(&currentTime);

   unsigned long long timeId = currentTime.LowPart;
   char timeStamp[64] = { 0 };
   sprintf_s(timeStamp, "%lld", timeId);
   rapidjson::Document document;
   document.SetObject();
   rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

   std::string system = "windows";
   rapidjson::Value userSystemname(StringRef(system.c_str()));
   document.AddMember("imei", userSystemname, allocator);

   std::string systemPf = "6";
   rapidjson::Value userSystemPf(StringRef(systemPf.c_str()));
   document.AddMember("pf", userSystemPf, allocator);

   std::string ver = CLASS_SSDK_VER;
   rapidjson::Value sdkVer(StringRef(ver.c_str()));
   document.AddMember("cv", sdkVer, allocator);

   StringBuffer buffer;
   rapidjson::Writer<StringBuffer> writer(buffer);
   document.Accept(writer);
   std::string data = buffer.GetString();
   LOGD("OnSocketOpen  SendMsg:%s", data.c_str());
   //mLogReport.SendLog(SDK_INIT, timeStamp, "defualt", "1", data);
}

void VHClassRoomImpl::UpLoadLog(const std::string& url) {
#ifdef QT_NO_DEBUG
   if (url.empty()) return;
   HTTP_GET_REQUEST httpGet(url);
   if (mpHttpManagerInterface) {
      mpHttpManagerInterface->HttpGetRequest(httpGet, [&](std::string data, int code, const std::string userData)->void {

      });
   }
#endif // QT_NO_DEBUG
}

VHStreamType  VHClassRoomImpl::CalcStreamType(const bool& bAudio, const bool& bVedio)
{
   VHStreamType eRef = VHStreamType_Count;
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      eRef = mpWebRtcSDKInterface->CalcStreamType(bAudio, bVedio);
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      eRef = mpPaasInteraction->CalcStreamType(bAudio, bVedio);
   }
   return eRef;
}

std::string VHClassRoomImpl::GetAuxiliaryId()
{
   std::string strRef;
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      strRef = mpWebRtcSDKInterface->GetAuxiliaryId( );
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      strRef = mpPaasInteraction->GetAuxiliaryId();
   }
   return strRef;
}

std::string VHClassRoomImpl::GetLocalAuxiliaryId()
{
   std::string strRef;
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      strRef = mpWebRtcSDKInterface->GetLocalAuxiliaryId();
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      strRef = mpPaasInteraction->GetLocalAuxiliaryId();
   }
   return strRef;
}

void VHClassRoomImpl::ClearSubScribeStream()
{
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      mpWebRtcSDKInterface->ClearSubScribeStream();
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      mpPaasInteraction->ClearSubScribeStream();
   }
}

void VHClassRoomImpl::StopRecvAllRemoteStream()
{
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      mpWebRtcSDKInterface->StopRecvAllRemoteStream();
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      mpPaasInteraction->StopRecvAllRemoteStream();
   }
}

int VHClassRoomImpl::LoginPlayType()
{
   return miPlayType;
}

DWORD __stdcall VHClassRoomImpl::ThreadProcessJoinRTCRoom(LPVOID p)
{
   LOGD("ThreadProcess");
   bool bConned = false;
   while (!bConned) {
      LOGD("ThreadProcess bConned :%d", bConned);
      if (mStatic_TaskEvent) {
         DWORD ret = WaitForSingleObject(mStatic_TaskEvent, 100);
         if (p) {
            VHClassRoomImpl* sdk = (VHClassRoomImpl*)(p);
            if (sdk) {
               bConned = sdk->ProcessTaskJoinRTCRoom();
               LOGD("ThreadProcess  JoinRTCRoom bConned :%d", bConned);
            }
         }
      }
   }

   if (mStatic_ThreadExitEvent) {
      ::SetEvent(mStatic_ThreadExitEvent);
   }

   LOGD("exit ThreadProcess");
   return 0;
}

bool VHClassRoomImpl::ProcessTaskJoinRTCRoom()
{
   QJsonObject obj;
   obj["join_uid"] = QString::fromStdString(mClassRoomBaseInfo.mUserInfo.join_id);
   obj["join_uname"] = QString::fromStdString(mClassRoomBaseInfo.mUserInfo.nick_name);
   obj["join_role"] = mClassRoomBaseInfo.mUserInfo.role_type == JOIN_ROLE_Teacher  ? QString("host") : QString("user");
   QJsonDocument document = QJsonDocument(obj);
   QByteArray array = document.toJson();
   std::string userData = array.toStdString();
   int iClassType = 1;
   if (CLASS_TYPE_MULTI == mClassRoomBaseInfo.mWebinarInfo.course_type)
   {
      iClassType = 2;
   }

   LOGD("  Paas SDK JoinRTCRoom miPlayType %d, strVid %s, strVfid %s, mWebinarInfo.id  %s",
      miPlayType, mClassRoomBaseInfo.strVid.c_str(), mClassRoomBaseInfo.strVfid.c_str(), mClassRoomBaseInfo.mWebinarInfo.id.c_str());
   return mpPaasInteraction->JoinRTCRoom(userData, true, mClassRoomBaseInfo.strVid, mClassRoomBaseInfo.strVfid, mClassRoomBaseInfo.mWebinarInfo.id,
   (int)mClassRoomBaseInfo.mUserInfo.role_type, iClassType);
}

std::string VHClassRoomImpl::LocalStreamId()
{
   LOGD("enter");
   if (eClassPlayType_Flash == miPlayType && mpWebRtcSDKInterface) {
      return mpWebRtcSDKInterface->LocalStreamId();
   }
   else if (eClassPlayType_H5 == miPlayType && mpPaasInteraction) {
      return mpPaasInteraction->LocalStreamId();
   }
}

namespace vlive {
   VHCLASS_SDK_EXPORT VHClassRoomInterface* GetClassSDKInstance() {
      std::unique_lock<std::mutex> lock(gSDKMutex);
      if (gClassSDKInstance == nullptr) {
         gClassSDKInstance = new VHClassRoomImpl();
      }

      return (VHClassRoomInterface*)gClassSDKInstance;
   }

   VHCLASS_SDK_EXPORT void DestoryClassSDKInstance() {
      std::unique_lock<std::mutex> lock(gSDKMutex);
      if (gClassSDKInstance) {
         delete gClassSDKInstance;
         gClassSDKInstance = nullptr;
      }
   }
}
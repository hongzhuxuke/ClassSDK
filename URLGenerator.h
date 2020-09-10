#pragma once
#include <string>

////#define HOST_URL "http://dev-api-class.e.vhall.com"
////测试环境
//#define HOST_URL "http://t-api-class.e.vhall.com"
////线上环境
////#define HOST_URL "http://api-class.e.vhall.com"
class URLGenerator
{
public:
   URLGenerator();
   ~URLGenerator();

   void SetDoMain(const std::string& domain);

   void SetPublicParam(std::string app_key, std::string appSecret, std::string devId);

public:
   // pc-获取课程信息
   std::string GetWebinarDetailInfoUrl(std::string webinarId);
   // pc-课堂口令登录
   std::string GetJoinClassRoomUrl(std::string webinarId, std::string nickName, std::string password, std::string phone);
   std::string GetJoinClassRoomUrl(std::string cmd);
   std::string GetPaasRoomInfoUrl(std::string webinarId, std::string join_id, std::string token);


   std::string StartCoutDownToolUrl(std::string webinarId, std::string  join_id, std::string token, const int& time, const int& iIsPlayEndSounds);
   std::string CancleCoutDownToolUrl(std::string webinarId, std::string  join_id, std::string token);
   //发起签到
   std::string GetAddSign(std::string webinarId, std::string  join_id, std::string token);
   std::string GetSignIn(std::string webinarId, std::string  join_id, std::string token, const std::string& AddSignId);
   std::string GetSignRecord(std::string webinarId, std::string  join_id, std::string token, const std::string& AddSignId);
   std::string GetSignStop(std::string webinarId, std::string  join_id, std::string token, const std::string& AddSignId);
   //发起答题器
	std::string GetStartAnswer(std::string webinarId, std::string  join_id, std::string token, std::string content);
	std::string GetAnswerIn(std::string webinarId, std::string  join_id, std::string token, std::string answer, std::string ques_id);
	std::string GetAnnounceAnswer(std::string webinarId, std::string  join_id, std::string token, std::string ques_id);
	std::string GetStopAnswer(std::string webinarId, std::string  join_id, std::string token, std::string ques_id);
	std::string GetAnswerDetail(std::string webinarId, std::string  join_id, std::string token, std::string ques_id);
   // pc-获取直播间互动token
   std::string GetInteractiveTokenUrl(std::string webinar_id, std::string join_id, std::string token);
   // pc-上课
   std::string GetStartClassUrl(std::string webinar_id, std::string join_id, std::string token, int course_type, int role, const int& iDefinitionId,
      const int& iplayType);
   // pc-下课
   std::string GetStopClassUrl(std::string webinar_id, std::string join_id, std::string token, int type);
   // pc-上下麦
   std::string GetStartPublishUrl(std::string join_id, std::string webinar_id, std::string class_token, int status, std::string target_id);
   // pc-上下麦失败返回
   std::string GetPublishCallbackUrl(std::string join_id, std::string webinar_id, std::string class_token, int status, std::string target_id);
   // token	string	是	token
   // event	string	是	msg
   // msg	    string	是	公告内容
   std::string GetSendNoticeUrl(std::string token, std::string event, std::string msg);
   //取消举手
   std::string GetCancelHandUrl(std::string webinar_id, std::string join_id, std::string token);
   //奖励（讲师点赞学员）
   std::string RewardStudent(std::string join_id, std::string targetid, std::string token);
   // pc-举手,请求上麦
   std::string GetHandUpUrl(std::string webinar_id);
   // pc-禁言
   std::string GetForbidChatUrl(std::string webinar_id, std::string join_id, int status, std::string token, std::string target_id);
   // pc-全体禁言
   std::string GetForbidChatAllUrl(std::string webinar_id, std::string join_id, int status, std::string token);
   // pc-踢出
   std::string GetKickOutRoomUrl(std::string webinar_id, std::string join_id, int status, std::string token, std::string target_id);
   // pc-下麦所有用户
   std::string GetKickAllUserUnPublishUrl(std::string webinar_id, std::string join_id, std::string token);
   // pc-静音
   //  webinar_id	string	是	课程ID
   //  join_id	int	是	用户ID
   //  status	int	是	状态（1静音、0取消静音）
   //  token	string	是	令牌
   //  target_id	int	是	指定用户的join_id
   std::string GetMuteAudioUrl(std::string webinar_id, std::string join_id, int status, std::string token, std::string target_id);
   // pc-全员静音
   std::string GetMuteAudioAllUrl(std::string webinar_id, std::string join_id, int status, std::string token);
   // pc-关闭画面
   std::string GetMuteVideoUrl(std::string webinar_id, std::string join_id, int status, std::string token, std::string target_id);
   // pc-授权画笔
   std::string GetSetBrushUrl(std::string webinar_id, std::string join_id, int status, std::string token, std::string target_id);
   // pc - 是否允许学员举手 状态（1：开启，0：关闭）
   std::string GetEnableHandsUpUrl(std::string join_id, std::string webinar_id, std::string token, int status);
   //**** pc - 获取成员列表  *********
   std::string GetOnlineUserUrl(std::string join_id, std::string webinar_id, std::string token, int curr_page, int pagesize);
   //**** pc - 获取踢出成员列表  *********
   std::string GetKickOutUserUrl(std::string join_id, std::string webinar_id, std::string token);
   //**** pc - 获取举手成员列表  *********
   std::string GetHandsUpUserUrl(std::string join_id, std::string webinar_id, std::string token);
   //**** pc - 获取禁言成员列表  *********
   std::string GetSilencedUserUrl(std::string join_id, std::string webinar_id, std::string class_token);
   //预上麦处理。即邀请上麦与下麦处理
   std::string GetCheckJoinMics(std::string join_id, std::string webinar_id, std::string class_token, int status, int target_id);
   //更新上麦能力、双推能力
   std::string GetUpdatePublishAbility(std::string join_id, std::string webinar_id, std::string class_token, int no_ablility, int no_ablity_double);
   //桌面共享
   std::string GetScreensharingUrl(std::string join_id, std::string webinar_id, std::string class_token, int status, const std::string &target_id);
   //上报当前直播、双推摄像头信息
   std::string GetSendMicsDeviceUrl(std::string join_id, std::string webinar_id, std::string class_token, const std::string &audio_id, const std::string& camera_id, const std::string& double_camera_id, const int roleType);
   //邀请学员上麦
   std::string GetInviteDoubleVrtcUrl(std::string join_id, std::string webinar_id, std::string class_token, const std::string &target_id);
   //邀请学员桌面共享
   std::string URLGenerator::GetInviteDesktopVrtcUrl(std::string join_id, std::string webinar_id, std::string class_token, const std::string &target_id);
   //双推学员列表
   std::string GetStopDesktopSharingUrl(std::string join_id, std::string webinar_id, std::string class_token, std::string target_id);

   //开始双推
   std::string GetStartDoubleVrtcUrl(std::string join_id, std::string webinar_id, std::string class_token);
   //结束双推
   std::string GetStopDoubleVrtcUrl(std::string target_id, std::string join_id, std::string webinar_id, std::string class_token);
   //双推学员列表
   std::string GetDoubleVrtcUsersUrl(std::string join_id, std::string webinar_id, std::string class_token);

   std::string GetPublishListUrl(std::string join_id, std::string webinar_id, std::string class_token);
   std::string GetSetBigScreenUrl(std::string join_id, std::string target_id, std::string webinar_id, std::string class_token) ;
   std::string GetSetScreenLayoutUrl(std::string join_id, std::string webinar_id, std::string class_token,int desktop_share_layout_mode, const int& iLayoutMode);

   //std::string GetsyncMixedLaoutModeUrl(std::string join_id, std::string webinar_id, std::string class_token, int desktop_share_layout_mode);

private:
   void createBaseInfo(std::string& baseInfo, std::string join_id, std::string webinar_id, std::string class_token);

   std::string mDomain;

   std::string mAppKey;
   std::string mDevId;
   std::string mAppSecret;

};


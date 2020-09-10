#pragma once
#include <string>

////#define HOST_URL "http://dev-api-class.e.vhall.com"
////���Ի���
//#define HOST_URL "http://t-api-class.e.vhall.com"
////���ϻ���
////#define HOST_URL "http://api-class.e.vhall.com"
class URLGenerator
{
public:
   URLGenerator();
   ~URLGenerator();

   void SetDoMain(const std::string& domain);

   void SetPublicParam(std::string app_key, std::string appSecret, std::string devId);

public:
   // pc-��ȡ�γ���Ϣ
   std::string GetWebinarDetailInfoUrl(std::string webinarId);
   // pc-���ÿ����¼
   std::string GetJoinClassRoomUrl(std::string webinarId, std::string nickName, std::string password, std::string phone);
   std::string GetJoinClassRoomUrl(std::string cmd);
   std::string GetPaasRoomInfoUrl(std::string webinarId, std::string join_id, std::string token);


   std::string StartCoutDownToolUrl(std::string webinarId, std::string  join_id, std::string token, const int& time, const int& iIsPlayEndSounds);
   std::string CancleCoutDownToolUrl(std::string webinarId, std::string  join_id, std::string token);
   //����ǩ��
   std::string GetAddSign(std::string webinarId, std::string  join_id, std::string token);
   std::string GetSignIn(std::string webinarId, std::string  join_id, std::string token, const std::string& AddSignId);
   std::string GetSignRecord(std::string webinarId, std::string  join_id, std::string token, const std::string& AddSignId);
   std::string GetSignStop(std::string webinarId, std::string  join_id, std::string token, const std::string& AddSignId);
   //���������
	std::string GetStartAnswer(std::string webinarId, std::string  join_id, std::string token, std::string content);
	std::string GetAnswerIn(std::string webinarId, std::string  join_id, std::string token, std::string answer, std::string ques_id);
	std::string GetAnnounceAnswer(std::string webinarId, std::string  join_id, std::string token, std::string ques_id);
	std::string GetStopAnswer(std::string webinarId, std::string  join_id, std::string token, std::string ques_id);
	std::string GetAnswerDetail(std::string webinarId, std::string  join_id, std::string token, std::string ques_id);
   // pc-��ȡֱ���以��token
   std::string GetInteractiveTokenUrl(std::string webinar_id, std::string join_id, std::string token);
   // pc-�Ͽ�
   std::string GetStartClassUrl(std::string webinar_id, std::string join_id, std::string token, int course_type, int role, const int& iDefinitionId,
      const int& iplayType);
   // pc-�¿�
   std::string GetStopClassUrl(std::string webinar_id, std::string join_id, std::string token, int type);
   // pc-������
   std::string GetStartPublishUrl(std::string join_id, std::string webinar_id, std::string class_token, int status, std::string target_id);
   // pc-������ʧ�ܷ���
   std::string GetPublishCallbackUrl(std::string join_id, std::string webinar_id, std::string class_token, int status, std::string target_id);
   // token	string	��	token
   // event	string	��	msg
   // msg	    string	��	��������
   std::string GetSendNoticeUrl(std::string token, std::string event, std::string msg);
   //ȡ������
   std::string GetCancelHandUrl(std::string webinar_id, std::string join_id, std::string token);
   //��������ʦ����ѧԱ��
   std::string RewardStudent(std::string join_id, std::string targetid, std::string token);
   // pc-����,��������
   std::string GetHandUpUrl(std::string webinar_id);
   // pc-����
   std::string GetForbidChatUrl(std::string webinar_id, std::string join_id, int status, std::string token, std::string target_id);
   // pc-ȫ�����
   std::string GetForbidChatAllUrl(std::string webinar_id, std::string join_id, int status, std::string token);
   // pc-�߳�
   std::string GetKickOutRoomUrl(std::string webinar_id, std::string join_id, int status, std::string token, std::string target_id);
   // pc-���������û�
   std::string GetKickAllUserUnPublishUrl(std::string webinar_id, std::string join_id, std::string token);
   // pc-����
   //  webinar_id	string	��	�γ�ID
   //  join_id	int	��	�û�ID
   //  status	int	��	״̬��1������0ȡ��������
   //  token	string	��	����
   //  target_id	int	��	ָ���û���join_id
   std::string GetMuteAudioUrl(std::string webinar_id, std::string join_id, int status, std::string token, std::string target_id);
   // pc-ȫԱ����
   std::string GetMuteAudioAllUrl(std::string webinar_id, std::string join_id, int status, std::string token);
   // pc-�رջ���
   std::string GetMuteVideoUrl(std::string webinar_id, std::string join_id, int status, std::string token, std::string target_id);
   // pc-��Ȩ����
   std::string GetSetBrushUrl(std::string webinar_id, std::string join_id, int status, std::string token, std::string target_id);
   // pc - �Ƿ�����ѧԱ���� ״̬��1��������0���رգ�
   std::string GetEnableHandsUpUrl(std::string join_id, std::string webinar_id, std::string token, int status);
   //**** pc - ��ȡ��Ա�б�  *********
   std::string GetOnlineUserUrl(std::string join_id, std::string webinar_id, std::string token, int curr_page, int pagesize);
   //**** pc - ��ȡ�߳���Ա�б�  *********
   std::string GetKickOutUserUrl(std::string join_id, std::string webinar_id, std::string token);
   //**** pc - ��ȡ���ֳ�Ա�б�  *********
   std::string GetHandsUpUserUrl(std::string join_id, std::string webinar_id, std::string token);
   //**** pc - ��ȡ���Գ�Ա�б�  *********
   std::string GetSilencedUserUrl(std::string join_id, std::string webinar_id, std::string class_token);
   //Ԥ������������������������
   std::string GetCheckJoinMics(std::string join_id, std::string webinar_id, std::string class_token, int status, int target_id);
   //��������������˫������
   std::string GetUpdatePublishAbility(std::string join_id, std::string webinar_id, std::string class_token, int no_ablility, int no_ablity_double);
   //���湲��
   std::string GetScreensharingUrl(std::string join_id, std::string webinar_id, std::string class_token, int status, const std::string &target_id);
   //�ϱ���ǰֱ����˫������ͷ��Ϣ
   std::string GetSendMicsDeviceUrl(std::string join_id, std::string webinar_id, std::string class_token, const std::string &audio_id, const std::string& camera_id, const std::string& double_camera_id, const int roleType);
   //����ѧԱ����
   std::string GetInviteDoubleVrtcUrl(std::string join_id, std::string webinar_id, std::string class_token, const std::string &target_id);
   //����ѧԱ���湲��
   std::string URLGenerator::GetInviteDesktopVrtcUrl(std::string join_id, std::string webinar_id, std::string class_token, const std::string &target_id);
   //˫��ѧԱ�б�
   std::string GetStopDesktopSharingUrl(std::string join_id, std::string webinar_id, std::string class_token, std::string target_id);

   //��ʼ˫��
   std::string GetStartDoubleVrtcUrl(std::string join_id, std::string webinar_id, std::string class_token);
   //����˫��
   std::string GetStopDoubleVrtcUrl(std::string target_id, std::string join_id, std::string webinar_id, std::string class_token);
   //˫��ѧԱ�б�
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


#pragma once
#include "vh_room_msg.h"
#include "VHClassRoomInterface.h"
#include "VhallNetWorkInterface.h"
#include "WebRtcSDKInterface.h"
#include "VHWebSocketInterface.h"
#include <atomic>
#include "LogReport.h"
#include "URLGenerator.h"
#include "lib/rapidjson/include/rapidjson/rapidjson.h"
#include "lib/rapidjson/include/rapidjson/stringbuffer.h"
#include "lib/rapidjson/include/rapidjson/writer.h"
#include "lib/rapidjson/include/rapidjson/reader.h"
#include "lib/rapidjson/include/rapidjson/document.h"
#include "../VhallPaasSDK/VHPaasRoomImpl.h"

using namespace rapidjson;
////��������
//#define HOST_URL  http://dev-class.e.vhall.com/
////���Ի���
////#define HOST_URL "http://t-api-class.e.vhall.com"
////���ϻ���
////#define HOST_URL "http://api-class.e.vhall.com"
#define PRE_URL  "api/sdk/"

#define CLASS_SSDK_VER    "2.1.0.0"
#define DIP_1080P   "1080P"
#define DIP_720P    "720P"
#define DIP_540P    "540P"
#define DIP_480P    "480P"
#define DIP_240P    "240P"

//�����壩
#define DPI_SD      "SD"
//�����壩
#define DPI_HD      "HD"
// �����壩
#define DPI_UHD     "UHD"
//CUSTOM
#define DPI_CUSTOM     "CUSTOM"

class ClassBroadCastInfo {
public:
   std::string live_room_id;
   LayoutMode layoutMode;
   int width;
   int height;
   int fps;
   int bitrate;
   std::string stream_id;
   BROAD_CAST_DPI dpi;
   std::string dpiDesc;
};

using namespace vlive;

class Document;
//class VHPaasInteractionRoom;

class VHClassRoomImpl :public VHClassRoomInterface, public WebSocketCallBackInterface {
public:
   VHClassRoomImpl();
   virtual ~VHClassRoomImpl();

public:
   //WebSocketCallBackInterface
   virtual void OnOpen();
   virtual void OnFail();
   virtual void OnReconnecting();
   virtual void OnReconnect(unsigned, unsigned);
   virtual void OnClose();
   virtual void OnSocketOpen(std::string const& nsp);
   virtual void OnSocketClose(std::string const& nsp);
   virtual void OnServiceMsg(std::string type, const std::wstring user_id, const std::wstring nickname, const std::string rolename, bool is_banned, int devType = -1, int uv = 0);
   //virtual void OnServiceMsg(std::string type, const std::wstring user_id, const std::wstring nickname, const std::string rolename, bool is_banned, int devType = -1, int uv = 0) = 0;

   virtual void OnRecvAllMsg(const char*, int length);

   virtual int ReconnectNetWork();
   virtual int ClearPassSdkInfo();
   virtual std::string GetDevId();
   virtual bool isConnetWebRtcRoom();
   //��ʼ�����ӿڱ�����main�����е��á�
   virtual void InitNetWorkProtocol(std::string app_key, std::string app_secret, const std::string& domain, VHRoomMonitor* monitor, std::wstring logPath = std::wstring());
  
   virtual void InitSDK(const int& iPlayType);
   virtual void InitWebRtcSDK();
   //virtual void InitSDK(std::string app_key, std::string app_secret, const std::string& domain, VHRoomMonitor* monitor, std::wstring logPath = std::wstring());
   virtual void SetDebugLogAddr(const std::string& addr);
   /*
   *   �ӿ�˵������ȡ�γ�����
   *   �ӿڲ�����
   *      webinar_id���γ�ID
   *   �ص�����������RoomEvent::RoomEvent_Login�¼�
   */
   virtual void GetWebinarDetailInfo(const std::string& webinar_id);
   /**
   *  �ӿ�˵����������÷���
   *  �ص�����������RoomEvent_JoinClassRoomö���¼�
   *  ����˵����
   *       nickName���ǳ�
   *       password������
   **/
   virtual void JoinClassRoom(std::string nickName, std::string password, std::string phone);
   virtual void JoinClassRoom(std::string cmd);
   virtual void JoinPaasRoom();
   /**
   *  �ӿ�˵���������˿�ʼ/ȡ������ʱ
   *  �ص�������
    ["cmd",{"type":"*startCountdown","join_id":395758,"time":120}]
    ["cmd",{"type":"*cancelCountdown","join_id":395758}]
   *  ����˵����
   * iTime ����ʱ����
   *iIsPlayEndSounds �Ƿ񲥷Ž�����Ч 1 ���� 0 ������
   **/
   virtual void StartCoutDownTool(const int& iTime, const int& iIsPlayEndSounds);
   virtual void CancleCoutDownTool();
   /**
   *  �ӿ�˵��������ǩ��
   *  �ص�������
    * {'type':'*startSign','webinar_id':'edu_xxxx','sign_id':'ǩ��id','user_id':'�̻�id','sign_show_time':30}
   **/
   virtual void AddSign();
   virtual void SignIn(const std::string& strAddSignId);
   virtual void GetSignRecord(const std::string& strAddSignId);
   virtual void SignStop(const std::string& strAddSignId);
   /**
   *  �ӿ�˵�������������
   *  �ص�������
    *  {"subject":"������1574131026546","options":[{"subject":"A","is_answer":0,"key":"A"},{"subject":"B","is_answer":1,"key":"B"},{"subject":"C","is_answer":1,"key":"C"},{"subject":"D","is_answer":0,"key":"D"}],"type":"scantron"}
   **/
	virtual void StartAnswer(const std::string& strContent);
	virtual void AnswerIn(const std::string& strAnswer, const std::string& strQuesId);
	virtual void AnnounceAnswer(const std::string& strQuesId);
	virtual void StopAnswer(const std::string& strQuesId);
	virtual void AnswerDetail(const std::string& strQuesId);
   /*
   *  �ӿ�˵������GetWebinarDetailInfo���óɹ�֮�󣬿��Ի�ȡ���������Ϣ��
   *  ����ֵ����ǰ���û�����Ϣ��
   */
   virtual CourseInfo GetCourseInfo() const;

   virtual int SetBigScreen(const std::string& user_id, SetBigScreenCallback cb_fun);

   virtual int SetScreenLayout(bool hasCamera, const int& iLayoutMode = -1);
   /*
   *  �ӿ�˵������GetWebinarDetailInfo���óɹ�֮�󣬿��Ի�ȡ���������Ϣ��
   *  �ص�����������RoomEvent::RoomEvent_Logout�¼�
   */
   virtual ClassRoomBaseInfo GetClassRoomBaseInfo() const;
   virtual void ChangeDocMode(int iMode);//0 �ĵ�  1 �װ�
   virtual std::string GetClassToken() const;
   /*
   *   �ӿ�˵��������ý�巿��
   *   �ص�����������RoomEvent_ConnectMediaRoomö���¼�
   */
   virtual void ConnectVHMediaRoom(/*const int& iPlayType*/);

   /*
*  �ӿ�˵�����˳�����ý�巿��
*  ����ֵ��true �˳��ɹ���false �˳�ʧ��
*/
   bool LeaveVHMediaRoom();
   //virtual void DisConnectVHMediaRoom();
   /*
   *   ʹ�ܶ���������ʼ���ķ����е�Զ����
   */
   virtual void EnableSubScribeStream();
   /*
   *  ��ֹ��������ȡ������Զ����
   */
   virtual void DisableSubScribeStream();
   /*
   *  �ӿ�˵�����˳����÷��䷿��
   *  ����ֵ��true �˳��ɹ���false �˳�ʧ��
   */
   virtual bool LeaveClassRoom();
   /*
   *   �ӿ�˵�����Ͽ�
   **/
   virtual int StartClass(const int& iDefinitionId);
   /*
   *   �ӿ�˵��: �¿�
   **/
   virtual int StopClass(int createDefaultRecord);
   ///*
   //*   �ӿ�˵��: �������湲����(������)���������(С����) PC��
   //*       LayoutMode   ������  1: ����ƽ�� 2:��������
   //**/
   //virtual int syncMixedLaoutMode(int LayoutMode);
   /*
   * SDK����������Ա�б�
   **/
   virtual int AsynGetClassRoomMembers(int pageSize = 50, int curPage = -1);
   /*
   *  �ӿ�˵���� ��ȡ�����û��б�
   **/
   virtual int GetSilencedUserList();
   /**
   *   �ӿ�˵������ȡ�����б�
   *   �ص�������
   *       �ɹ���http�������RoomEvent_GetHandUpList���ص���������OnGetVHRoomMembers
   *   ����ֵ��ERROR_CODE
   **/
   virtual int GetAlreadyHandsUpUserList();
   /**
   *   �ӿ�˵�����������ѧԱ���ֿ���
   *   ����˵����
   *       enable: true ���� false ������
   */
   virtual int OpenUserHandsUp(bool enable);
   /*
   *   ����˵��������/�ر����湲��֪ͨ��
   *   �ӿڲ�����
   *       open : true ������ false �ر�
   ***/
   virtual void DesktopShareNotice(bool open, const std::string &target_id);
   /**
  *  �ӿ�˵��������û��Ƿ��������С�
  *  ����˵����
  *   user_id��  �������û�ID
  *   ����ֵ��   ERROR_CODE
  *   �ص�������OnCheckUserIsPublish
  **/
   virtual int CheckUserIsPublish(const std::string& user_id);
   /**
   * ��������
   */
   virtual int ApplyPublish();
   /**
   * ȡ������
   */
   virtual int CanclePublish();
   //��������ʦ��ѧԱ������
   virtual int RewardStudent(const std::string& targetId);
   /**
   * ͬ����������
   * audit_user_id: ������˵������û�ID
   * ����ֵ��VhallLiveErrCode
   **/
   virtual int AuditPublish(const std::string& audit_user_id);
   /**
   * ��������
   * audit_user_id: ������˵������û�ID
   **/
   virtual int AskforPublish(const std::string& audit_user_id);
   /**
   *  �ӿ�˵����ѧԱ���α���/��/�ܾ�����״̬��ִ��
   *  ����˵����
   *       target_id: ������Ŀ���û����������д��target_id�ڲ�Ĭ���ǵ�¼�û��λ�id
   *       type:      PushStreamEvent
   *   �ص�������OnUserPublishCallback
   */
   virtual int UserPublishCallback(ClassPushStreamEvent type, std::string target_id = std::string());
   /**
   *  �ӿ�˵����ѧԱ���α���/����ʧ��֪ͨ��ѧԱ���α��ȷ����������ʹ�á�
   *  ����˵����
   *       stream_id: ��������ID
   *       type:      PushStreamEvent
   *  �ص������¼���RoomEvent::RoomEvent_UserPublishCallback
   */
   virtual int UserPublishFailedCallback(ClassPushStreamEvent type);

   virtual int StartLocalCapturePlayer(const int dev_index, const std::wstring dev_id, const int volume);

   virtual int SetLocalCapturePlayerVolume(const int volume);

	virtual int StopLocalCapturePlayer();
   /**
   *  �ӿ�˵��������ָ���û�
   *  ����˵����
   *     kick_user_id	string	��	���߳��ĵ������û�ID
   *  �ص������¼���RoomEvent::RoomEvent_KickInavStream
   *  ����ֵ��VhallLiveErrCode
   **/
   virtual int KickUserPublish(const std::string& kick_user_id);
   /**
   *  �ӿڲ�������ʦ���������û�
   **/
   virtual int KickAllUnPublish();
   /**
   *  �ӿ�˵�����߳����÷���/ȡ���߳����÷���
   *  ����˵����
   *    kick_user_id:���߳��ĵ������û�ID
   *  �ص������¼���RoomEvent::RoomEvent_KickInav
   *  ����ֵ��VhallLiveErrCode
   **/
   virtual int KickOutClassRoom(const std::string& kick_user_id, int type);
   /*
   *  �ӿ�˵�������ԡ�ȡ������
   *  ����˵����
   */
   virtual int ForbiddenChat(std::string userId, bool forbid);
   /*
   *  �ӿ�˵����ȫ����ԡ�ȡ������
   *  ����˵����
   */
   virtual int ForbiddenChatAll(bool forbid);
   /*
   * ��ȡ���߳������������б�
   */
   virtual int GetKickInavList();
   /***
   *   �ӿ�˵�����رա����û���˷�
   *   ����˵����
   *       userId���û�id
   *       close: true �رգ� false ��
   */
   virtual int OperateUserMic(std::string userId, bool close);
   /***
   *   �ӿ�˵�����رա����û�����ͷ
   *   ����˵����
   *       userId���û�id
   *       close: true �رգ� false ��
   */
   virtual int OperateUserCamera(std::string userId, bool close);
   /*
      *   �ӿ�˵������ʦ��ʦ����ȫ���û�
      *       mute: 1 ������ 0ȡ������
      *   �ص�������OnOperateAllUserMic
      */
   virtual int OperateAllUserMic(int mute);
   /***
   *   �ӿ�˵������Ȩ�û�����
   *   ����˵����
   *       userId���û�id
   *       auth: true �����û�����Ȩ�ޣ� false �ջ��û�����Ȩ��
   */
   virtual int ChangeUserPenAuth(std::string userId, bool auth);
   /**
   *   �ӿ�˵�������͹�����Ϣ
   *   ����˵����
   *       noticeMsg; ���͵Ĺ�����Ϣ
   */
   virtual int SendNoticeMsg(std::string noticeMsg);

   //////////////////////////////////////////////////////////////////////////////////////////////////////

   /*          ��Ҫ�Ż�����
   *  �ӿ�˵����ѧԱ��α��ϱ�����������ͨ����ȡ��˷�������豸�б��鿴�Ƿ����豸��������豸�ϱ�״̬�������˲�������˳�Ա����
   *  ����˵����
   *      bool enablePublish; true ������false ��������
   */
   virtual int UpdatePublishAbility(const bool& enablePublish, const bool& enableDoublePublish);

   //////////////////////                     ����              //////////////////////
   /**
   *   �ӿ�˵�����ϱ������豸��Ϣ
   *   ����˵����
   *       audio_id; ����mic�豸
   *       camera_id; ��������ͷ�豸
   *       double_camera_id; ˫���豸id
   *       roleType; ����˫�����
   */
   virtual int SendMicsDevice(const std::string &audio_id = "", const std::string& camera_id = "", const std::string& double_camera_id = "", const int roleType = -1);

   /**
     *   �ӿ�˵��������˫��
     *   ����˵����
     *       target_id; ������Ŀ��id
     */
   virtual int inviteDoubleVrtc(const std::string &target_id);

   /**
 *   �ӿ�˵�����������湲��
 *   ����˵����
 *       target_id; ������Ŀ��id
 */
   virtual int inviteDesktopVrtc(const std::string &target_id);
   /**
    *   �ӿ�˵����˫��-��ʼ
    *   ����˵����
    *
    */
   virtual int startDoubleVrtc();
   /**
     *   �ӿ�˵����˫��-����
     *   ����˵����
     *
     */
   virtual int stopDoubleVrtc(const std::string &target_id);
   /**
  *   �ӿ�˵����ѧԱ˫����Ļ����-����
  *   ����˵����
  *
  */
   virtual int stopDesktopSharing(const std::string &target_id);
   /**
    *   �ӿ�˵����֧��˫���û��б�
    *   ����˵����
    *
    */
   virtual int doubleVrtcUsers();

   //////////////////////////////////////////////////////////////////////////////////////////////////////

   virtual VideoProfileIndex GetVideoProfileByWebinarInfo(int index, const int iVideoProFileType = -1);
   /*
   *   ������·ֱ��
   *   live_room_id: ֱ������id
   *   width�� �����˲��ֵĿ�
   *   height�� �����˲��ֵĸ�
   *   fps��֡��
   *   bitrate�� ����
   *   stream_id�� ������id
   */
   virtual int StartBroadCastLayout(LayoutMode layoutMode, BROAD_CAST_DPI dip = BROAD_CAST_DPI_960_720, SetBroadCastEventCallback callback = nullptr);
   virtual int SetConfigBroadCastLayOut(LayoutMode mode, SetBroadCastEventCallback fun = nullptr);
   /**
   *  ��������·ֱ��֮�󣬿��Խ�ĳ���û����������ڻ�������������浱��
   **/
   virtual int SetMainView(std::string stream_id);

   virtual int CheckPicEffectiveness(const std::string filePath);


   /*
   *   ֹͣ��·ֱ��
   *   live_room_id: ֱ������id
   */
   virtual int StopPublishInavAnother(std::string live_room_id);


   virtual int StartLocalCapturePicture(const std::string filePath, VideoProfileIndex index);

   virtual bool IsVHMediaConnected();
   /*  ��ʼ����ͷ\��˷�ɼ�
   *   �ص���� OnStartLocalCapture OnCameraCaptureErr  OnLocalMicOpenErr
   */
   virtual int StartLocalCapture(std::string devId, VideoProfileIndex index);
   /*
   *   ��������˫������ͷ�ɼ�
   */
   virtual int StartLocalAuxiliaryCapture(const std::string devId, VideoProfileIndex index);
   /*
   *   ֹͣ����˫������ͷ�ɼ�
   */
   virtual void StopLocalAuxiliaryCapture();

   /*ֹͣ����ͷ\��˷�ɼ�*/
   virtual void StopLocalCapture();

   /*��ʼ����ͷ��������  �ص���⣺OnStartPushLocalStream*/
   virtual int StartPushLocalStream();
   /*ֹͣ����ͷ�������� �ص���⣺OnStopPushLocalStream*/
   virtual int StopPushLocalStream();
   /*
*   ��ʼ��������ͷ��������
*   context �� ������ӵ��Զ����ֶ�
*   �ص���⣺OnStartPushLocalAuxiliaryStream
*/

    virtual int StartPushLocalAuxiliaryStream(std::string context = std::string());
    /*
    *   ֹͣ��������ͷ��������
    *   �ص���⣺OnStopPushLocalAuxiliaryStream
    */
    virtual int StopPushLocalAuxiliaryStream();
    /**
    *   ��ǰ�ɼ������Ƿ���������
    */
    virtual bool IsPushingStream(VHStreamType streamType);
    /*��ǰ�����Ƿ���ڸ������͵�Զ����*/
    virtual bool IsExitSubScribeStream(const vlive::VHStreamType& streamType);
    virtual bool IsExitSubScribeStream(const std::string& id, const vlive::VHStreamType& streamType);
    /*
    *   ��ǰ�ɼ������Ƿ����ڽ��б�������Դ�ɼ�
    */
    virtual bool IsCapturingStream(VHStreamType streamType);
    /**
    *  ����˵������ʦ�л���������ǩ
    **/
    virtual int ChangeToToolPage(ToolPage pageIndex);

    /*��ʼ���湲��ɼ�  �ص���⣺OnStartDesktopCaptureSuc OnStartDesktopCaptureErr*/
    virtual int StartDesktopCapture(int64_t index, VideoProfileIndex profileIndex) ;
    /*ֹͣ���湲��ɼ�*/
    virtual void StopDesktopCapture() ;
    /*��ʼ���湲��ɼ����� �ص���⣺OnStartPushDesktopStream*/
    virtual int StartPushDesktopStream(std::string context = std::string()) ;
    /*ֹͣ���湲��ɼ����� �ص���⣺OnStopPushDesktopStream*/
    virtual int StopPushDesktopStream() ;
    /*��ʼ�岥�ļ�*/
    virtual int StartPlayMediaFile(std::string filePath) ;
    /**
    *   ��ȡ��ID
    *   ����ֵ���������id������id ���򷵻ؿ��ַ���
    */
    virtual std::string GetSubScribeStreamId(const std::wstring& user, vlive::VHStreamType streamType) ;
    
    /**
    *   ��ȡ�������������ͣ��Ƿ��������Ƶ���ݡ�
    *   ����ֵ��0 ���ҳɹ�������ʧ��
    */
    virtual int GetSubScribeStreamFormat(const std::wstring& user, vlive::VHStreamType streamType, bool &hasVideo, bool &hasAudio);
    /**
    *   ��ʼ��Ⱦ"����"�����豸�����湲���ļ��岥ý��������.
    */
    virtual bool StartRenderLocalStream(vlive::VHStreamType streamType, void* wnd) ;
    virtual bool StartRenderLocalStreamByInterface(vlive::VHStreamType streamType, std::shared_ptr<vhall::VideoRenderReceiveInterface> receive);
    virtual bool IsStreamExit(std::string id);

    /**
    *   ��ʼ��Ⱦ"Զ��"�����豸�����湲���ļ��岥ý��������.
    */
    virtual bool StartRenderRemoteStream(const std::wstring& user, vlive::VHStreamType streamType, void* wnd);
    virtual bool StartRenderRemoteStreamByInterface(const std::wstring& user, vlive::VHStreamType streamType, std::shared_ptr<vhall::VideoRenderReceiveInterface> receive);
    virtual bool IsRemoteStreamIsExist(const std::wstring& user, vlive::VHStreamType streamType);
    /*
    *   ֹͣ��ȾԶ����
    */
    virtual bool StopRenderRemoteStream(const std::wstring& user, const std::string streamId, vlive::VHStreamType streamType);
    /*
    *  �ӿ�˵���������յ�Զ������ͷ���ݺ󣬿����޸Ķ��ĵĻ���Ϊ������С��
    */
    virtual bool ChangeSubScribeUserSimulCast(const std::wstring& user_id, vlive::VHStreamType streamType, VHSimulCastType type);

    /*
    *   �ӿ�˵�����鿴��ǰ���ŵ��ļ��Ƿ�֧����Ƶ����Ƶ��ʽ��
	�����жϵ�ǰ���ŵ�����Ƶ�ļ�������Ƶ�ļ�
    *   �ӿڲ�����
    type����Ƶ����Ƶ
    *   ����ֵ��true ֧��/ false ��֧��
    */
    virtual bool IsSupportMediaFormat(CaptureStreamAVType type);
    /*ֹͣ�岥�ļ�*/
    virtual void StopMediaFileCapture();
    /*��ʼ�岥�ļ�����  �ص���⣺OnStartPushMediaFileStream*/
    virtual int StartPushMediaFileStream() ;
    /*ֹͣ�岥�ļ����� �ص���⣺OnStopPushMediaFileStream*/
    virtual void StopPushMediaFileStream() ;
    //�岥�ļ���ͣ����
    virtual void MediaFilePause() ;
    //�岥�ļ��ָ�����
    virtual void MediaFileResume() ;
    //�岥�ļ��������
    virtual void MediaFileSeek(const unsigned long long& seekTimeInMs) ;
    //�岥�ļ���ʱ��
    virtual const long long MediaFileGetMaxDuration() ;
    //�岥�ļ���ǰʱ��
    virtual const long long MediaFileGetCurrentDuration();
    //�岥�ļ��ĵ�ǰ״̬ 
    //����ֵ������״̬  MEDIA_FILE_PLAY_STATE
    virtual int MediaGetPlayerState();
    /**��ȡ����ͷ�б�**/
    virtual void GetCameraDevices(std::list<vhall::VideoDevProperty> &cameras);
    /**��ȡ��˷��б�**/
    virtual void GetMicDevices(std::list<vhall::AudioDevProperty> &micDevList) ;
    /**��ȡ�������б�**/
    virtual void GetPlayerDevices(std::list<vhall::AudioDevProperty> &playerDevList);
    /*
    *  �Ƿ������Ƶ����Ƶ�豸��
    *  ����ֵ��ֻҪ����һ���豸 ����true, �������Ƶ�豸��û���򷵻�false
    **/
    virtual bool HasVideoOrAudioDev();
    virtual bool HasVideoDev();
    virtual bool HasAudioDev();
    /*
    *  ����ʹ�õ���˷�
    *  index: GetMicDevices��ȡ���б��� vhall::VideoDevProperty�е� devIndex
    */
    virtual void SetUsedMic(int index, std::string devId, std::wstring desktopCaptureId);
    /**
    *  ��ȡ��ǰʹ�õ���˷��豸��Ϣ
    ***/
    virtual void GetCurrentMic(int &index, std::string& devId);
    /**
    *  ��ȡ��ǰʹ�õ������豸��Ϣ
    ***/
    virtual void GetCurrentCamera(std::string& devId);
    /*
    *  �л�����ͷ�������豸�б����豸�Զ��л�����һ����
    *  �ص������Ƿ��л��ɹ�
    */
    virtual void ChangeCamera();
    /*
    *  ����ʹ�õ�������
    *  index: GetPlayerDevices��ȡ���б��� VhallLiveDeviceInfo�е� devIndex
    */
    virtual void SetUsePlayer(int index, std::string devId);
    /**
    *  ��ȡ��ǰʹ�õ������豸��Ϣ
    ***/
    virtual void GetCurrentCameraInfo(int &index, std::string& devId);
    /**
    * ����ͷ����Ԥ������Ԥ������֮����ҪֹͣԤ������������ͷ����һֱռ��
    */
    virtual int PreviewCamera(void* wnd, const std::string& devGuid, VideoProfileIndex index, int micIndex = -1);
    /*
    *   �л�Ԥ����˷�
    */
    virtual void ChangePreViewMic(int micIndex);
    virtual int GetMicVolumValue();
    /*
    * ֹͣ����ͷԤ��
    */
    virtual int StopPreviewCamera();
    /**���õ�ǰʹ�õ���˷�����**/
    virtual bool SetCurrentMicVol(int vol);
    /**��ȡ��ǰʹ�õ���˷�����**/
    virtual int GetCurrentMicVol();
    /**���õ�ǰʹ�õ�����������**/
    virtual bool SetCurrentPlayVol(int vol);
    /**��ȡ��ǰʹ�õ�����������**/
    virtual int GetCurrentPlayVol();
    //�ر�����ͷ
    virtual bool CloseCamera();
    //������ͷ
    virtual bool OpenCamera();
	//�ر�����ͷ
	virtual bool IsCameraOpen();
    virtual bool CloseMic();

    virtual void SubScribeRemoteStream(const std::string &stream_id, int delayTimeOut = 0);
    /*
    *   ����˷�
    */
    virtual bool OpenMic();
	//������ͷ
	virtual bool IsMicOpen();
    /*
    *  ��̬�л���ǰʹ�õ���˷�.ע��ʹ�ô˽ӿ�ʱ������Ѿ������˿�ʼ�ɼ���
    */
    virtual int ChangeUserMicDev(int index) ;
    /*
    *  ��̬�л���ǰʹ�õ������� .ע��ʹ�ô˽ӿ�ʱ������Ѿ������˿�ʼ�ɼ���
    */
    virtual int ChangeUserPlayDev(int index);
    /*
    *   ��Ӳ�����Ƶ�ļ��ӿ�
    */
    virtual int PlayAudioFile(std::string fileName,int devIndex);

    virtual int GetPlayAudioFileVolum();
    
    virtual int StopPlayAudioFile();
    /*
    *    ��ȡ���ڹ�������湲���б�
    **  vlive::VHStreamType      // 3:Screen,5:window
    */
    virtual std::vector<DesktopCaptureSource> GetDesktops(int streamType);
	/*����ѡ������Դ*/
	virtual int SelectSource(int64_t id);
	/*ֹͣ���Դ����ɼ�*/
	virtual void StopSoftwareCapture();
	/*ֹͣ�������ɼ�����*/
	virtual int StopPushSoftWareStream();
	/*��ʼ�������ɼ�����*/
	virtual int StartPushSoftWareStream();

	/**
	*   ��ǰ���������Ƿ���������Ƶ��
	*/
	//virtual bool IsUserPushingSoftwareStream();
	///**

//*   ��ǰ���������Ƿ�������湲����Ƶ��
//*/
//virtual bool IsUserPushingDesktopStream();
/**
*   ��ǰ���������Ƿ���ڲ岥��Ƶ��
*/
//virtual bool IsUserPushingMediaFileStream();
       /**
    *  �򿪻�ر����ж��ĵ���Ƶ
    */
   virtual int MuteAllSubScribeAudio(bool mute);
   virtual bool GetMuteAllAudio();
   virtual void GetStreams(std::list<std::string>& streams);
   /**
   *   Http ��־�ϱ�
   */
   virtual void UpLoadLog(const std::string& url);
   VHStreamType  CalcStreamType(const bool& bAudio, const bool& bVedio);
   std::string GetAuxiliaryId();
   virtual std::string GetLocalAuxiliaryId();
   virtual void ClearSubScribeStream();
   virtual void StopRecvAllRemoteStream();//ֹͣ����Զ����

   virtual int LoginPlayType();

   static DWORD WINAPI ThreadProcessJoinRTCRoom(LPVOID p);
   bool ProcessTaskJoinRTCRoom();
   virtual std::string LocalStreamId();
   /**
*  ��ȡ������Ƶ������.3���һ��
*/
   virtual double GetPushDesktopVideoLostRate();
private:
   std::string GetLayoutDesc(LayoutMode layout);

   void SetBroadCastCallBack(const std::string result, const std::string& msg);
   void SetBroadCastCallBackWithParam(int layout, const std::string result, const std::string& msg);
   virtual  void SetWhiteListPhone(const std::string& phone);

private:

   void UploadReportSDKInit();
   void UploadReportSDKLogin(std::string sdkAppid, std::string thrid_user_id);

private:
   void ParamToGetDoubleVrtcUsers(const std::string& msg);
   void AnalysisGetWebinarDetailInfo(std::string data, int code);
   void ParamToGetWebinarDetailInfoResp(const std::string& msg);
   void AnalysisSignRecord(std::string data, int code);
   void AnalysisJoinClassRoomDetailInfo(std::string data, int code);
   void AnalysisJoinPaasRoom(std::string data, int code);
   void ParamToJoinClassRoomDetailInfoResp(const std::string& msg);
   void ParamToJoinPaasRoom(const std::string& msg);
   void ParamToOnSignRecordResp(const std::string& msg);
	void ParamToOnAnswerStart(const std::string& msg);
	void ParamToAnswerDetail(const std::string& msg);

   void AnalysisGetOnLineUserInfo(std::string data, int code);
   void ParamToGetOnLineUserInfoResp(const std::string& msg);

	void AnalysisAnswerDetail(std::string data, int code);
	void AnalysisStartAnswer(std::string data, int code);

   void AnalysisGetKickOutRoomUserInfo(std::string data, int code);
   void ParamToKickOutRoomUserInfoResp(const std::string& msg);

   void AnalysisGetHandsUpUserInfo(std::string data, int code);
   void ParamToHandsUpUserInfoResp(const std::string& msg);

   void AnalysisSilencedUserInfo(std::string data, int code);
   void ParamToSilencedUserInfoResp(const std::string& msg);

   bool ParamRoomEvent(ClassRoomEvent event, std::string data, int code, std::string userData);
   //void CheckPermission(VHStreamType streamType,std::string data, int code);
   void ParamRecvInavMsg(const std::string msg);

   bool ParamEventToResp(std::string data, int& respCode, std::string& respMsg);
   bool IsUserPublish(const std::string user_id, std::string data, int& respCode, std::string& respMsg);

   void HandleRecvFlashMsg(const std::string& msg);
   void HandleRecvMediaMsg(const std::string& msg);
	void HandleAnswerMsg(const std::string& msg);
	void HandleAnswerToolMsg(const std::string& msg);
   void HandleSetBrushMsg(const std::string& msg);
   //void HandleHandDelay(const std::string& msg);
   void HandleDocUploadMsg(const std::string& msg);

   void HandleSetBigScreen(const rapidjson::Document& obj);

   //private:
   //    void GetVHRoomInfo();

private:

   BroadCastVideoProfileIndex Dpi2ProfileIndex(const BROAD_CAST_DPI& dip);
   //void handelCancelHand(const rapidjson::Document& doc);
   std::string mAappKey;
   std::string mAppSecret;
   //std::string mWebinarId;
   std::string mClassToken;
   std::string mStreamToken;
   std::string mPublishUrl;      //������ַ
   std::string mReportUrl;      //��־�ϱ���ַ
   std::string mDevUniqueId;     //cpu Ψһ��
   std::string mPhone;     //�������ֻ���

   bool mIsHost = true;
   bool bIsSettinglayout = false;

   CourseInfo mCourseInfo;
   ClassRoomBaseInfo mClassRoomBaseInfo;
   JoinClassRoomMessage mMessageInfo;
   VideoProfileIndex videoProfile = RTC_VIDEO_PROFILE_480P_4x3_H;

   VHRoomMonitor * mpVHRoomMonitor = nullptr;                  //�ص�����
   std::shared_ptr<HttpManagerInterface> mpHttpManagerInterface = nullptr;     //http���������
   VHWebSocketInterface* mpVHWebSocketInterface = nullptr;     //socketIo/WebSocket��

   WebRtcSDKInterface* mpWebRtcSDKInterface = nullptr;         //���� ����SDK��  flash
   VHPaasInteractionRoom* mpPaasInteraction = nullptr;            //paasSdk ����SDK��   H5
   
   ClassBroadCastInfo mCurrentBroadCastInfo;
   LogReport mLogReport;

   URLGenerator mURLGenerator;                     //URL������
   std::atomic_bool mbDisConnectWebSocket = false;

   std::wstring mlogPath;

   //enum ePlayType
   //{
   //   eClassPlayType_Flash = 0,
   //   eClassPlayType_H5,
   //};
   int miPlayType = eClassPlayType_Flash; // 0 flash(�ɻ������sdk)   1 H5 ���»��paasSdk��
   
   std::thread* mProcessThreadJoinRtcRoom = nullptr;
   static HANDLE mStatic_TaskEvent;   //
   static HANDLE mStatic_ThreadExitEvent;
};
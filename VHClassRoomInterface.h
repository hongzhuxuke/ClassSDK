#pragma once
#include <string>
#include <list>
#include <Windows.h>
#include "class_callback_monitor.h"
#include "./WebRtcSDK/Interactive_live_SDK_win/vhall_webrtc_sdk/signalling/tool/VideoRenderReceiver.h"

#ifdef  VHCLASS_SDK_EXPORT
#define VHCLASS_SDK_EXPORT     __declspec(dllimport)
#else
#define VHCLASS_SDK_EXPORT     __declspec(dllexport)
#endif


typedef std::function<void(const std::string&, const std::string&)> SetBroadCastEventCallback;
typedef std::function<void(const std::string&, int, const std::string&)> SetBigScreenCallback;

namespace vlive {

   class VHClassRoomInterface {
   public:


      /*
      *   �ӿ�˵������ʼ�����ӿڱ������������е��á�
      *   ����˵����
      *       app_key ����SDK���ɵ�app_key
      *       app_secret ����SDK���ɵ�app_secret
      *       domain: ����
      *       monitor �ص��ӿ���
      *       logPath ��־���·����Ĭ�ϴ����C:\Users\�û�\AppData\RoamingĿ¼��
      */
      virtual void InitNetWorkProtocol(std::string app_key, std::string app_secret, const std::string& domain, VHRoomMonitor* monitor, std::wstring logPath = std::wstring()) = 0;
      //virtual void InitSDK(std::string app_key, std::string app_secret, const std::string& domain, VHRoomMonitor* monitor, std::wstring logPath = std::wstring()) = 0;

      virtual void InitSDK(const int& iPlayType) = 0;
      virtual bool isConnetWebRtcRoom() = 0;
	  virtual void InitWebRtcSDK() = 0;
     /*��ճ�ʼ����passInfo*/
     virtual int ClearPassSdkInfo() = 0;
      virtual void SetDebugLogAddr(const std::string& addr) = 0;
      //��ȡ�豸id
      virtual std::string GetDevId() = 0;
      /*
       *   �ӿ�˵������ȡ�γ�����
       *   �ӿڲ�����webinar_id �γ�ID
       *   �ص�����������OnGetWebinarDetailInfo
       */
      virtual void GetWebinarDetailInfo(const std::string& webinar_id) = 0;
      /**
      *  �ӿ�˵����������÷��䡣
      *  ����˵����
      *       nickName���ǳ�
      *       password������
      *  �ص�����������OnJoinClassRoom
      **/
      virtual void JoinClassRoom(std::string nickName, std::string password, std::string phone) = 0;
      virtual void JoinClassRoom(std::string cmd) = 0;
      /**
      *  �ӿ�˵���������˿�ʼ/ȡ������ʱ
      *  �ص�������
       ["cmd",{"type":"*startCountdown","join_id":395758,"time":120}]
       ["cmd",{"type":"*cancelCountdown","join_id":395758}]
      *  ����˵����
      * iTime ����ʱ����
      **/
      virtual void StartCoutDownTool(const int& iTime, const int& iIsPlayEndSounds) = 0;
      virtual void CancleCoutDownTool() = 0;
      /**
      *  �ӿ�˵��������ǩ��
      *  �ص�������
       * {'type':'*startSign','webinar_id':'edu_xxxx','sign_id':'ǩ��id','user_id':'�̻�id','sign_show_time':30}
      **/
      virtual void AddSign() = 0;
      virtual void SignIn(const std::string& strAddSignId) = 0;
      virtual void GetSignRecord(const std::string& strAddSignId) = 0;
      virtual void SignStop(const std::string& strAddSignId) = 0;
      /**
      *  �ӿ�˵�������������
      *  �ص�������
       *  {"subject":"������1574131026546","options":[{"subject":"A","is_answer":0,"key":"A"},{"subject":"B","is_answer":1,"key":"B"},{"subject":"C","is_answer":1,"key":"C"},{"subject":"D","is_answer":0,"key":"D"}],"type":"scantron"}
      **/
		virtual void StartAnswer(const std::string& strContent) = 0;
		virtual void AnswerIn(const std::string& strAnswer, const std::string& strQuesId) = 0;
		virtual void AnnounceAnswer(const std::string& strQuesId) = 0;
		virtual void StopAnswer(const std::string& strQuesId) = 0;
		virtual void AnswerDetail(const std::string& strQuesId) = 0;
      /*
       *  �ӿ�˵������GetWebinarDetailInfo���óɹ�֮�󣬿��Ի�ȡ���������Ϣ��
       *  ����ֵ����ǰ���û�����Ϣ��
       */
      virtual CourseInfo GetCourseInfo() const = 0;
      /*
      *  �ӿ�˵������JoinClassRoom���óɹ�֮�󣬿��Ի�ȡ���������Ϣ��
      *  ����ֵ����ǰ���û�����Ϣ��
      */
      virtual ClassRoomBaseInfo GetClassRoomBaseInfo() const = 0;
      virtual void ChangeDocMode(int iMode) = 0;
      virtual std::string GetClassToken() const = 0;
      /*
      * ����Ͽ������ӿ�
      */
      virtual int ReconnectNetWork() = 0;
      /*
      *   �ӿ�˵�������ӻ���ý�巿��,Ĭ���Զ�����Զ����������ͨ��EnableSubScribeStream/DisableSubScribeStream�����Ƿ��Զ�����
      *   �ص�����������RoomEvent_ConnectMediaRoomö���¼���ص�OnWebRtcRoomConnetEventCallback
      */
      virtual void ConnectVHMediaRoom(/*const int& iPlayType*/) = 0;
      /*
      *   ʹ�ܶ���������ʼ���ķ����е�Զ����
      */
      virtual void EnableSubScribeStream() = 0;
      /*
      *  ��ֹ��������ȡ������Զ����
      */
      virtual void DisableSubScribeStream() = 0;
      /*
      *  �ӿ�˵�����˳�����ý�巿��
      *  ����ֵ��true �˳��ɹ���false �˳�ʧ��
      */
      virtual bool LeaveVHMediaRoom() = 0;
      ///**
      //*  �Ͽ���������
      //*/
      //virtual void DisConnectVHMediaRoom() = 0;
      /*
      *  �ӿ�˵�����˳����÷��䷿��
      *  ����ֵ��true �˳��ɹ���false �˳�ʧ��
      */
      virtual bool LeaveClassRoom() = 0;
      /*
      *   �ӿ�˵������ʦ������ʼ�ϿΡ�
      *   ����ֵ��ERROR_CODE
      *   �ص�������OnStartClass
      **/
      virtual int StartClass(const int& iDefinitionId) = 0;
      /*
      *   �ӿ�˵��: ��ʦ�����¿�
      *        createDefaultRecord: �Ƿ�����Ĭ�ϻط� 1�� 0��
      *   ����ֵ��ERROR_CODE
      *   �ص�������OnStopClass
      **/
      virtual int StopClass(int createDefaultRecord) = 0;

      //      /*
      //*   �ӿ�˵��: �������湲����(������)���������(С����) PC��
      //*       LayoutMode   ������  1: ����ƽ�� 2:��������
      //**/
      //virtual int syncMixedLaoutMode(int LayoutMode) = 0;
      /*
      *  �ӿ�˵������ʦ��ȡ������Ա�б�
      *  ����˵����
      *          curPage����ǰ��Ѱ�ڼ�ҳ
      *          pageSize����ǰҲ���ض������ݣ�ÿҳչʾ������Ĭ��10��������ܳ���50
      *  �ص�������
      *       �ɹ���http�������RoomEvent_OnLineUserList���ص���������OnGetVHRoomMembers
      *   ����ֵ��ERROR_CODE
      */
      virtual int AsynGetClassRoomMembers(int pageSize = 50, int curPage = -1) = 0;
      /**
      *   �ӿ�˵������ȡ�����б�
      *   �ص�������
      *       �ɹ���http�������RoomEvent_GetHandUpList���ص���������OnGetVHRoomMembers
      *   ����ֵ��ERROR_CODE
      **/
      virtual int GetAlreadyHandsUpUserList() = 0;
      /*
      *  �ӿ�˵������ȡ���߳������������б�
      *  �ص�������OnGetVHRoomKickOutMembers��OnRoomFailedEvent
      */
      virtual int GetKickInavList() = 0;
      /*
      *  �ӿ�˵���� ��ȡ�����û��б�
      **/
      virtual int GetSilencedUserList() = 0;
      /**
      *   �ӿ�˵������ʦ����ѧԱ���ֿ���
      *   ����˵����
      *       open: true ���� false ������
      *   ����ֵ��ERROR_CODE
      *   �ص�������OnOpenUserHandsUp
      */
      virtual int OpenUserHandsUp(bool open) = 0;
      /**
      *  �ӿ�˵��������û��Ƿ��������С�
      *  ����˵����
      *   user_id��  �������û�ID
      *   ����ֵ��   ERROR_CODE
      *   �ص�������OnCheckUserIsPublish
      **/
      virtual int CheckUserIsPublish(const std::string& user_id) = 0;
      /**
      *  �ӿ�˵������ʦͬ�������������������ʹ�á�
      *  ����˵����
      *    audit_user_id: ������˵������û�ID
      *   ����ֵ��ERROR_CODE
      *   �ص�������OnAuditPublish
      **/
      virtual int AuditPublish(const std::string& audit_user_id) = 0;
      /**
      *  �ӿ�˵������ʦ����ָ���û������������ʹ�á�
      *  ����˵����
      *     kick_user_id	string	��	���߳��ĵ������û�ID
      *   ����ֵ��ERROR_CODE
      *   �ص�������OnKickUserPublish
      **/
      virtual int KickUserPublish(const std::string& kick_user_id) = 0;
      /**
      *  �ӿڲ�������ʦ���������û�
      **/
      virtual int KickAllUnPublish() = 0;
      /**
      *  �ӿ�˵������ʦ����ѧԱ���α��������������ʹ�á�
      *  ����˵����
      *    audit_user_id: ������˵������û�ID
      *   ����ֵ��ERROR_CODE
      *   �ص�������OnAskforPublish
      **/
      virtual int AskforPublish(const std::string& audit_user_id) = 0;
      /**
      *  �ӿ�˵������ʦ��ѧԱ�߳����÷���/ȡ���߳����÷���
      *  ����˵����
      *    kick_user_id:���߳��ĵ������û�ID
      *    type : 1 �߳� 0 ȡ���߳�
      *   ����ֵ��ERROR_CODE
      *   �ص�������OnKickOutClassRoom
      **/
      virtual int KickOutClassRoom(const std::string& kick_user_id, int type) = 0;
      /*
      *   �ӿ�˵������ʦ��Ȩ�û�����
      *   ����˵����
      *       userId���û�id
      *       auth: true �����û�����Ȩ�ޣ� false �ջ��û�����Ȩ��
      *   ����ֵ��ERROR_CODE
      *   �ص�������OnChangeUserPenAuth
      */
      virtual int ChangeUserPenAuth(std::string userId, bool auth) = 0;
      /*
      *  �ӿ�˵������ʦ���ԡ�ȡ������ѧԱ���α�
      *   ����˵����
      *       userId���û�id
      *       forbid: true ���ã� false ȡ������
      *   ����ֵ��ERROR_CODE
      *   �ص�������OnForbiddenChat
      */
      virtual int ForbiddenChat(std::string userId, bool forbid) = 0;
      /*
      *  �ӿ�˵������ʦȫ����ԡ�ȡ������
      *       forbid: true ���ã� false ȡ������
      *   ����ֵ��ERROR_CODE
      *   �ص�������OnForbiddenChatAll
      */
      virtual int ForbiddenChatAll(bool forbid) = 0;
      /*
      *   ���ô��沼��
      */
      virtual int SetBigScreen(const std::string& user_id, SetBigScreenCallback cb_fun) = 0;
      /*
      *  �ϱ����湲��ʱ���񲼾�����  �����ϱ� ��������  ����
      */
      virtual int SetScreenLayout( bool hasCamera, const int& iLayoutMode = -1) = 0;
      /*
      *   �ӿ�˵������ʦ����ѧԱ��α�
      *   ����˵����
      *       userId���û�id
      *       close: true �رգ� false ��
      *   �ص�������OnOperateUserMic
      */
      virtual int OperateUserMic(std::string userId, bool close) = 0;
      /*
      *   �ӿ�˵������ʦ��ʦ����ȫ���û�
      *       mute: 1 ������ 0ȡ������
      *   �ص�������OnOperateAllUserMic
      */
      virtual int OperateAllUserMic(int mute) = 0;
      /*
      *   �ӿ�˵������ʦ�رա����û�����ͷ
      *   ����˵����
      *       userId���û�id
      *       close: true �رգ� false ��
      *   �ص�������OnOperateUserCamera
      */
      virtual int OperateUserCamera(std::string userId, bool close) = 0;
      /**
      *   �ӿ�˵������ʦ���͹�����Ϣ
      *   ����˵����
      *       noticeMsg; ���͵Ĺ�����Ϣ
      */
      virtual int SendNoticeMsg(std::string noticeMsg) = 0;
      /**
      *  �ӿ�˵����ѧԱ���α����֣���������
      *   �ص�������OnApplyPublish
      *  ����ֵ��VhallLiveErrCode
      */
      virtual int ApplyPublish() = 0;
      /**
      * ȡ������
      */
      virtual int CanclePublish() = 0;
      virtual int RewardStudent(const std::string& targetId) = 0;
      /**
      *  �ӿ�˵����ѧԱ���α���/��/�ܾ�����״̬��ִ��
      *  ����˵����
      *       target_id: ������Ŀ���û����������д��target_id�ڲ�Ĭ���ǵ�¼�û��λ�id
      *       type:      PushStreamEvent
      *   �ص�������OnUserPublishCallback
      */
      virtual int UserPublishCallback(ClassPushStreamEvent type, std::string target_id = std::string()) = 0;
      /*
      *   ����˵��������/�ر����湲��֪ͨ��
      *   �ӿڲ�����
      *       open : true ������ false �ر�
      ***/
      virtual void DesktopShareNotice(bool open , const std::string &target_id) = 0;
      /**
      *  �ӿ�˵����ѧԱ���α���/����ʧ��֪ͨ��ѧԱ���α��ȷ����������ʹ�á�
      *  ����˵����
      *       stream_id: ��������ID
      *       type:      PushStreamEvent
      *   �ص�������OnUserPublishFailedCallback
      */
      virtual int UserPublishFailedCallback(ClassPushStreamEvent type) = 0;
      /*
      *  �ӿ�˵����ѧԱ��α��ϱ�����������ͨ����ȡ��˷�������豸�б��鿴�Ƿ����豸��������豸�ϱ�״̬�������˲�������˳�Ա����
      *  ����˵����
      *      bool enablePublish; true ������false ��������
      *   �ص�������OnUpdatePublishAbility
      */
      virtual int UpdatePublishAbility(const bool& enablePublish, const bool& enableDoublePublish) = 0;
      /**
      *  ����˵������ʦ�л���������ǩ
      *   �ص�������OnChangeToToolPage
      **/
      virtual int ChangeToToolPage(ToolPage pageIndex) = 0;

      /**
    *   �ӿ�˵�����ϱ������豸��Ϣ
    *   ����˵����
    *       audio_id; ����mic�豸
    *       camera_id; ��������ͷ�豸
    *       double_camera_id; ˫���豸id
    *       roleType; ����˫�����
    */
      virtual int SendMicsDevice(const std::string &audio_id = "", const std::string& camera_id = "", const std::string& double_camera_id = "", const int roleType = -1) = 0;

      /**
        *   �ӿ�˵��������˫��
        *   ����˵����
        *       target_id; ������Ŀ��id
        */
      virtual int inviteDoubleVrtc(const std::string &target_id) = 0;

      /**
       *   �ӿ�˵�����������湲��
       *   ����˵����
       *       target_id; ������Ŀ��id
       */
      virtual int inviteDesktopVrtc(const std::string &target_id) = 0;
      /**
       *   �ӿ�˵����˫��-��ʼ
       *   ����˵����
       *
       */
      virtual int startDoubleVrtc() = 0;
      /**
        *   �ӿ�˵����˫��-����
        *   ����˵����
        *
        */
      virtual int stopDoubleVrtc(const std::string &target_id) = 0;
      /**
      *   �ӿ�˵����ѧԱ˫����Ļ����-����
      *   ����˵����
      *
      */
      virtual int stopDesktopSharing(const std::string &target_id) = 0;
      /**
       *   �ӿ�˵����˫��-����
       *   ����˵����
       *
       */
      virtual int doubleVrtcUsers() = 0;

    /**
    *   ��ʼ����ɼ���ɼ���������
    *   ������
    *       dev_index:�豸����
    *       dev_id;�豸id
    *       float:�ɼ����� 0-100
    **/
    virtual int StartLocalCapturePlayer(const int dev_index, const std::wstring dev_id, const int volume) = 0;
	 /**
    *   ֹͣ����ɼ�����
    **/
	 virtual int StopLocalCapturePlayer() = 0;
    /*
    *   �ӿ�˵�������������Ƿ��Ѿ����ӳɹ�
    *   ����ֵ��true�������������ӣ�false�����������ӶϿ�
    */
    virtual bool IsVHMediaConnected() = 0;
    /*
    *   �ӿ�˵������ȡ����ͷ�б�
    *   ����˵�������ػ�ȡ�����豸�б�����豸�б�Ϊ��˵��û�л�ȡ����Ӧ���豸
    */
    virtual void GetCameraDevices(std::list<vhall::VideoDevProperty> &cameras) = 0;
    /*
    *   �ӿ�˵������ȡ��˷��б�
    *   ����˵�������ػ�ȡ�����豸�б�����豸�б�Ϊ��˵��û�л�ȡ����Ӧ���豸
    **/
    virtual void GetMicDevices(std::list<vhall::AudioDevProperty> &micDevList) = 0;
    /**
    *   �ӿ�˵������ȡ�������б�
    *   ����˵�������ػ�ȡ�����豸�б�����豸�б�Ϊ��˵��û�л�ȡ����Ӧ���豸
    **/
    virtual void GetPlayerDevices(std::list<vhall::AudioDevProperty> &playerDevList) = 0;
    /*
    *  �Ƿ������Ƶ����Ƶ�豸��
    *  ����ֵ��ֻҪ����һ���豸 ����true, �������Ƶ�豸��û���򷵻�false
    **/
    virtual bool HasVideoOrAudioDev() = 0;
    virtual bool HasVideoDev() = 0;
    virtual bool HasAudioDev() = 0;
    /*
    *  �ӿ�˵��������ʹ�õ���˷�
    *  ����˵����
    *   index: GetMicDevices��ȡ���б��� VhallLiveDeviceInfo�е� devIndex
    */
    virtual void SetUsedMic(int index, std::string devId, std::wstring desktopCaptureId) = 0;
    /**
    *  ��ȡ��ǰʹ�õ���˷��豸��Ϣ
    ***/
    virtual void GetCurrentMic(int &index, std::string& devId) = 0;
    /**
    *  ��ȡ��ǰʹ�õ������豸��Ϣ
    ***/
    virtual void GetCurrentCamera(std::string& devId) = 0;
    /*
    *  �л�����ͷ�������豸�б����豸�Զ��л�����һ����
    *  �ص������Ƿ��л��ɹ�
    */
    virtual void ChangeCamera() = 0;
      virtual VideoProfileIndex GetVideoProfileByWebinarInfo(int index, const int iVideoProFileType = -1) = 0;
      /*
      *  �ӿ�˵������ʦ������·ֱ��,�˽ӿ��ǻ�����������·����Ȩ�޵��û����е��õ�,���������湲���岥ʱ�������ʾ����������Ҫ����SetMainView�ӿڣ�
                    ����Ӧ����ID��Ϊ�����������ã����ֻ����ʾһ·��ͬʱ��Ҫ���ò���ģʽ����Ҫ�����ص�
      *  ����˵����
      *     layoutMode:�����˲���ģʽ
      *  �ص�������RoomEvent::RoomEvent_Start_MixStream
      *  ����ֵ��VhallLiveErrCode
      */
      virtual int StartBroadCastLayout(LayoutMode layoutMode, BROAD_CAST_DPI dip = BROAD_CAST_DPI_960_720, SetBroadCastEventCallback callback = nullptr) = 0;
      /*
      *  ������·����ģʽ
      */
      virtual int SetConfigBroadCastLayOut(LayoutMode mode, SetBroadCastEventCallback fun = nullptr) = 0;
      /**
      *  �ӿ�˵������ʦ������·ֱ��֮�󣬿��Խ�ĳ���û����������ڻ�������������浱�У��˽ӿ��ǻ�����������·����Ȩ�޵��û����е���
      *  �ص�������RoomEvent::RoomEvent_SetMainView
      *  ����ֵ��VhallLiveErrCode
      **/
      virtual int SetMainView(std::string stream_id) = 0;
      /*
      *   �ӿ�˵����ֹͣ��·ֱ�����˽ӿ��ǻ�����������·����Ȩ�޵��û����е��õ�
      *   ����˵����
      *       live_room_id: ֱ������id
      *   �ص�������RoomEvent::RoomEvent_Stop_PublishInavAnother
      *   ����ֵ��VhallLiveErrCode
      */
      virtual int StopPublishInavAnother(std::string live_room_id) = 0;
      /*
      *  ����������Ƶ�ɼ�����
      **/
      virtual int SetLocalCapturePlayerVolume(const int volume) = 0;


      virtual void SubScribeRemoteStream(const std::string &stream_id, int delayTimeOut = 0) = 0;
      /*
      *  �ӿ�˵��������ʹ�õ�������
      *  ����˵����
      *   index: GetPlayerDevices��ȡ���б��� VhallLiveDeviceInfo�е� devIndex
      */
      virtual void SetUsePlayer(int index, std::string devId) = 0;
      /**
      *  ��ȡ��ǰʹ�õ������豸��Ϣ
      ***/
      virtual void GetCurrentCameraInfo(int &index, std::string& devId) = 0;
      /**
      * ����ͷ����Ԥ������Ԥ������֮����ҪֹͣԤ������������ͷ����һֱռ��
      */
      virtual int PreviewCamera(void* wnd, const std::string& devGuid, VideoProfileIndex index, int micIndex = -1) = 0;
      /*
      *   �л�Ԥ����˷�
      */
      virtual void ChangePreViewMic(int micIndex) = 0;
      /*
      *   ��ȡԤ��ʱ��˷�������
      *   ����ֵ������ֵ��
      */
      virtual int GetMicVolumValue() = 0;
      /*
      * ֹͣ����ͷԤ��
      */
      virtual int StopPreviewCamera() = 0;
      /*
      *   �ӿ�˵�������õ�ǰʹ�õ���˷�����
      *   ����˵����
      *       vol:������Χ 0~100
      **/
      virtual bool SetCurrentMicVol(int vol) = 0;
      /*
      *   �ӿ�˵������ȡ��ǰʹ�õ���˷�����
      *   ����ֵ����ǰ����
      **/
      virtual int GetCurrentMicVol() = 0;
      /*
      *   �ӿ�˵�������õ�ǰʹ�õ�����������
      *   ����˵����
      *       vol:������Χ 0~100
      **/
      virtual bool SetCurrentPlayVol(int vol) = 0;
      /*
      *   �ӿ�˵������ȡ��ǰʹ�õ�����������
      *   ����ֵ�����ص�ǰ������ֵ
      **/
      virtual int GetCurrentPlayVol() = 0;
      /*
      *   �ӿ�˵�����ر�����ͷ
      *   ����ֵ��true �رճɹ�/ false �ر�ʧ��
      */
      virtual bool CloseCamera() = 0;
      /*
      *   �ӿ�˵�������Ѿ���ʼ�ɼ�������ͷ
      *   ����ֵ��true �򿪳ɹ�/ false ��ʧ��
      */
      virtual bool OpenCamera() = 0;
      /*
      *   �ӿ�˵��������ͷ�Ƿ�ر�
      *   ����ֵ��true �Ѿ��ر�/ false δ�ر�
      */
      virtual bool IsCameraOpen() = 0;
      /*
      *   �ӿ�˵�����ر���˷�
      *   ����ֵ��true �رճɹ�/ false �ر�ʧ��
      */
      virtual bool CloseMic() = 0;
      /*
      *   �ӿ�˵��������˷�
      *   ����ֵ��true �򿪳ɹ�/ false ��ʧ��
      */
      virtual bool OpenMic() = 0;
      /*
      *  ��̬�л���ǰʹ�õ���˷�.ע��ʹ�ô˽ӿ�ʱ������Ѿ������˿�ʼ�ɼ���
      */
      virtual int ChangeUserMicDev(int index) = 0;
      /*
      *  ��̬�л���ǰʹ�õ������� .ע��ʹ�ô˽ӿ�ʱ������Ѿ������˿�ʼ�ɼ���
      */
      virtual int ChangeUserPlayDev(int index) = 0;
      /*
      *   �ӿ�˵������˷��Ƿ��
      *   ����ֵ��true �Ѿ��ر�/ false δ�ر�
      */
      virtual bool IsMicOpen() = 0;
      /*
      *   �ж�ͼƬ��Ч��
      */
      virtual int CheckPicEffectiveness(const std::string filePath) = 0;
      /*
      *   �ӿ�˵������ʼ����ͷ\��˷�ɼ�
      *   ����˵����
      *       VideoProfileIndex��ͨ����ȡ�豸��Ϣʱ�õ�VideoDevProperty::mDevFormatList���豸֧�ֵķֱ��ʣ����ݷֱ�����Ϣѡ����ӦVideoProfileIndex
      *   ������OnOpenCaptureCallback
      *   ����ֵ��VhallLiveErrCode
      */
      virtual int StartLocalCapture(std::string devId, VideoProfileIndex index) = 0;
      /*
      *   ��ʼ�ɼ�ͼƬ����
      *   ������
      *   �ص���� OnOpenCaptureCallback
      **/
      virtual int StartLocalCapturePicture(const std::string filePath, VideoProfileIndex index) = 0;
      /*
      *   ��������˫������ͷ�ɼ�
      */
      virtual int StartLocalAuxiliaryCapture(const std::string devId, VideoProfileIndex index) = 0;
      /*
      *   ֹͣ����˫������ͷ�ɼ�
      */
      virtual void StopLocalAuxiliaryCapture() = 0;
      /*
      *   �ӿ�˵����ֹͣ����ͷ\��˷�ɼ�  ����Ѿ���ʼ������ͬʱֹͣ����
      */
      virtual void StopLocalCapture() = 0;
      /*
      *   �ӿ�˵������ʼ����ͷ�������� �������ȴ�����ͷ֮���������
      *   �ص�������OnPushStreamSuc OnPushStreamError
      */
      virtual int StartPushLocalStream() = 0;
      /*
      *   �ӿ�˵����ֹͣ����ͷ��������
      *   �ص������� OnStopPushStreamCallback
      */
      virtual int StopPushLocalStream() = 0;
      /*
      *   ��ʼ��������ͷ��������
      *   context �� ������ӵ��Զ����ֶ�
      *   �ص���⣺OnStartPushLocalAuxiliaryStream
      */
      virtual int StartPushLocalAuxiliaryStream(std::string context = std::string()) = 0;
      /*
      *   ֹͣ��������ͷ��������
      *   �ص���⣺OnStopPushLocalAuxiliaryStream
      */
      virtual int StopPushLocalAuxiliaryStream() = 0;
      /*
      *   �ӿ�˵������ǰ�ɼ������Ƿ����ڽ��б�������Դ�ɼ�
      */
      virtual bool IsCapturingStream(VHStreamType streamType) = 0;
      /**
      *   �ӿ�˵������ǰ���������Ƿ�������湲����Ƶ��
      */
      //virtual bool IsUserPushingDesktopStream() = 0;
      /**
      *   �ӿ�˵������ǰ���������Ƿ���ڲ岥��Ƶ��
      *   ����ֵ��ture�ɹ���falseʧ��
      */
      //virtual bool IsUserPushingMediaFileStream() = 0;
      /**
      *   �ӿ�˵������ǰ�ɼ������Ƿ���������
      *   ����ֵ��ture�ɹ���falseʧ��
      */
      virtual bool IsPushingStream(VHStreamType streamType) = 0;
      /*��ǰ�����Ƿ���ڸ������͵�Զ����*/
      virtual bool IsExitSubScribeStream(const vlive::VHStreamType& streamType) = 0;
      virtual bool IsExitSubScribeStream(const std::string& id, const vlive::VHStreamType& streamType) = 0;
      /**
      *   ��ȡ��ID
      *   ����ֵ���������id������id ���򷵻ؿ��ַ���
      */
      virtual std::string GetSubScribeStreamId(const std::wstring& user, vlive::VHStreamType streamType) = 0;
      /**
      *   ��ȡ�������������ͣ��Ƿ��������Ƶ���ݡ�
      *   ����ֵ��0 ���ҳɹ�������ʧ��
      */
      virtual int GetSubScribeStreamFormat(const std::wstring& user, vlive::VHStreamType streamType, bool &hasVideo, bool &hasAudio) = 0;
      /**
      *   ��ʼ��Ⱦ"����"�����豸�����湲���ļ��岥ý��������.
      */
      virtual bool StartRenderLocalStream(vlive::VHStreamType streamType, void* wnd) = 0;
      virtual bool StartRenderLocalStreamByInterface(vlive::VHStreamType streamType, std::shared_ptr<vhall::VideoRenderReceiveInterface> receive) = 0;
      virtual bool IsStreamExit(std::string id) = 0;
      /**
      *   ��ʼ��Ⱦ"Զ��"�����豸�����湲���ļ��岥ý��������.
      */
      virtual bool StartRenderRemoteStream(const std::wstring& user, vlive::VHStreamType streamType, void* wnd) = 0;
      virtual bool StartRenderRemoteStreamByInterface(const std::wstring& user, vlive::VHStreamType streamType, std::shared_ptr<vhall::VideoRenderReceiveInterface> receive) = 0;
      virtual bool IsRemoteStreamIsExist(const std::wstring& user, vlive::VHStreamType streamType) = 0;

      /*
      *   ֹͣ��ȾԶ����
      */
      virtual bool StopRenderRemoteStream(const std::wstring& user, const std::string streamId, vlive::VHStreamType streamType) = 0;

      /*
      *  �ӿ�˵���������յ�Զ������ͷ���ݺ󣬿����޸Ķ��ĵĻ���Ϊ������С��
      */
      virtual bool ChangeSubScribeUserSimulCast(const std::wstring& user_id, vlive::VHStreamType streamType, VHSimulCastType type) = 0;

      /*
      *   �ӿ�˵������ʼ���湲��ɼ�
      *   ������OnOpenCaptureCallback
      *   ����ֵ��VhallLiveErrCode
      */
      virtual int StartDesktopCapture(int64_t index, VideoProfileIndex profileIndex) = 0;
      /*
      *   �ӿ�˵����ֹͣ���湲��ɼ�  ����Ѿ���ʼ������ͬʱֹͣ����
      */
      virtual void StopDesktopCapture() = 0;
      /*
      *   �ӿ�˵������ʼ���湲��ɼ�����
      *   �ص���⣺OnPushStreamSuc OnPushStreamError
      */
      virtual int StartPushDesktopStream(std::string context = std::string()) = 0;
      /*
      *   �ӿ�˵����ֹͣ���湲��ɼ�����
      *   ��Ҫ�ص����:OnStopPushStreamCallback
      *   ����ֵ��VhallLiveErrCode
      */
      virtual int StopPushDesktopStream() = 0;
      /*
      *   �ӿ�˵������ʼ�岥�ļ�,filePath �ļ�·����Ҫ���÷�б�ܷ�ʽ������"G:\\file\\play.mp4"
      *   ����ֵ��VhallLiveErrCode
      *   �ص�������OnOpenCaptureCallback
      */
      virtual int StartPlayMediaFile(std::string filePath) = 0;
      /*
      *   �ӿ�˵�����鿴��ǰ���ŵ��ļ��Ƿ�֧����Ƶ����Ƶ��ʽ�������жϵ�ǰ���ŵ�����Ƶ�ļ�������Ƶ�ļ�
      *   �ӿڲ�����
            type����Ƶ����Ƶ
      *   ����ֵ��true ֧��/ false ��֧��
      */
      virtual bool IsSupportMediaFormat(CaptureStreamAVType type) = 0;
      /*
      *   �ӿ�˵����ֹͣ�岥�ļ�, ����Ѿ���ʼ������ͬʱֹͣ����
      */
      virtual void StopMediaFileCapture() = 0;
      /*
      *   �ӿ�˵������ʼ�岥�ļ�����
      *   ����ֵ��VhallLiveErrCode
      *   �ص���⣺OnPushStreamError  OnPushStreamSuc
      */
      virtual int StartPushMediaFileStream() = 0;
      /*
      *   �ӿ�˵����ֹͣ�岥�ļ�����
      *   �ص���⣺OnStopPushMediaFileStream
      */
      virtual void StopPushMediaFileStream() = 0;
      /*
      *   �ӿ�˵�����岥�ļ���ͣ����
      *   ����ֵ��VhallLiveErrCode
      */
      virtual void MediaFilePause() = 0;
      /*
      *   �岥�ļ��ָ�����
      *   ����ֵ��VhallLiveErrCode
      */
      virtual void MediaFileResume() = 0;
      /*
      *   �ӿ�˵�����岥�ļ��������
      *   ����ֵ��VhallLiveErrCode
      */
      virtual void MediaFileSeek(const unsigned long long& seekTimeInMs) = 0;
      /*
      *   �ӿ�˵�����岥�ļ���ʱ��
      */
      virtual const long long MediaFileGetMaxDuration() = 0;
      /*
      *   �ӿ�˵�����岥�ļ���ǰʱ��
      */
      virtual const long long MediaFileGetCurrentDuration() = 0;
      /*
      *   �ӿ�˵�����岥�ļ��ĵ�ǰ״̬
      *   ����ֵ������״̬  MEDIA_FILE_PLAY_STATE
      */
      virtual int MediaGetPlayerState() = 0;
      /*
      *   ��Ӳ�����Ƶ�ļ��ӿ�
      */
      virtual int PlayAudioFile(std::string fileName, int devIndex) = 0;
      /*
      *   ��ȡ�����ļ�����
      */
      virtual int GetPlayAudioFileVolum() = 0;
      /*
      *   ֹͣ������Ƶ�ļ��ӿ�
      */
      virtual int StopPlayAudioFile() = 0;

      /*
      *    ��ȡ���ڹ�������湲���б�
      **  vlive::VHStreamType      // 3:Screen,5:window
      */
      virtual std::vector<DesktopCaptureSource> GetDesktops(int streamType) = 0;
      /**
      *  �򿪻�ر����ж��ĵ���Ƶ
      */
      virtual int MuteAllSubScribeAudio(bool mute) = 0;
      virtual bool GetMuteAllAudio() = 0;
      /*����ѡ������Դ*/
      virtual int SelectSource(int64_t id) = 0;
      /*ֹͣ���Դ����ɼ�*/
      virtual void StopSoftwareCapture() = 0;
      /*ֹͣ�������ɼ�����*/
      virtual int StopPushSoftWareStream() = 0;
      /*��ʼ�������ɼ�����*/
      virtual int StartPushSoftWareStream() = 0;
      /* ��ǰ���������Ƿ���������Ƶ��*/
      //virtual bool IsUserPushingSoftwareStream() = 0;
       /**
       *   Http ��־�ϱ�
       */
      virtual void UpLoadLog(const std::string& url) = 0;
      virtual VHStreamType  CalcStreamType(const bool& bAudio, const bool& bVedio) = 0;
      virtual std::string GetAuxiliaryId() = 0;
      virtual std::string GetLocalAuxiliaryId() = 0;
	  virtual  void SetWhiteListPhone(const std::string& phone) = 0;
     /*���ص�¼ʱ���ȡ�Ŀ�������*/
     virtual int LoginPlayType() = 0;
     virtual void GetStreams(std::list<std::string>& streams) = 0;

     virtual void ClearSubScribeStream() = 0;
     virtual void StopRecvAllRemoteStream() = 0;//ֹͣ����Զ����
     virtual std::string LocalStreamId() = 0;
     /**
*  ��ȡ������Ƶ������.3���һ��
*/
     virtual double GetPushDesktopVideoLostRate() = 0;
   };

   VHCLASS_SDK_EXPORT VHClassRoomInterface* GetClassSDKInstance();
   VHCLASS_SDK_EXPORT void DestoryClassSDKInstance();

}
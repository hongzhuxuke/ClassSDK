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
      *   接口说明：初始化。接口必须在主函数中调用。
      *   参数说明：
      *       app_key 创建SDK生成的app_key
      *       app_secret 创建SDK生成的app_secret
      *       domain: 域名
      *       monitor 回调接口类
      *       logPath 日志存放路径，默认存放在C:\Users\用户\AppData\Roaming目录下
      */
      virtual void InitNetWorkProtocol(std::string app_key, std::string app_secret, const std::string& domain, VHRoomMonitor* monitor, std::wstring logPath = std::wstring()) = 0;
      //virtual void InitSDK(std::string app_key, std::string app_secret, const std::string& domain, VHRoomMonitor* monitor, std::wstring logPath = std::wstring()) = 0;

      virtual void InitSDK(const int& iPlayType) = 0;
      virtual bool isConnetWebRtcRoom() = 0;
	  virtual void InitWebRtcSDK() = 0;
     /*清空初始化的passInfo*/
     virtual int ClearPassSdkInfo() = 0;
      virtual void SetDebugLogAddr(const std::string& addr) = 0;
      //获取设备id
      virtual std::string GetDevId() = 0;
      /*
       *   接口说明：获取课程详情
       *   接口参数：webinar_id 课程ID
       *   回调监听：监听OnGetWebinarDetailInfo
       */
      virtual void GetWebinarDetailInfo(const std::string& webinar_id) = 0;
      /**
      *  接口说明：进入课堂房间。
      *  参数说明：
      *       nickName：昵称
      *       password：口令
      *  回调监听：监听OnJoinClassRoom
      **/
      virtual void JoinClassRoom(std::string nickName, std::string password, std::string phone) = 0;
      virtual void JoinClassRoom(std::string cmd) = 0;
      /**
      *  接口说明：主讲人开始/取消倒计时
      *  回调监听：
       ["cmd",{"type":"*startCountdown","join_id":395758,"time":120}]
       ["cmd",{"type":"*cancelCountdown","join_id":395758}]
      *  参数说明：
      * iTime 倒计时秒数
      **/
      virtual void StartCoutDownTool(const int& iTime, const int& iIsPlayEndSounds) = 0;
      virtual void CancleCoutDownTool() = 0;
      /**
      *  接口说明：发起签到
      *  回调监听：
       * {'type':'*startSign','webinar_id':'edu_xxxx','sign_id':'签到id','user_id':'商户id','sign_show_time':30}
      **/
      virtual void AddSign() = 0;
      virtual void SignIn(const std::string& strAddSignId) = 0;
      virtual void GetSignRecord(const std::string& strAddSignId) = 0;
      virtual void SignStop(const std::string& strAddSignId) = 0;
      /**
      *  接口说明：发起答题器
      *  回调监听：
       *  {"subject":"答题器1574131026546","options":[{"subject":"A","is_answer":0,"key":"A"},{"subject":"B","is_answer":1,"key":"B"},{"subject":"C","is_answer":1,"key":"C"},{"subject":"D","is_answer":0,"key":"D"}],"type":"scantron"}
      **/
		virtual void StartAnswer(const std::string& strContent) = 0;
		virtual void AnswerIn(const std::string& strAnswer, const std::string& strQuesId) = 0;
		virtual void AnnounceAnswer(const std::string& strQuesId) = 0;
		virtual void StopAnswer(const std::string& strQuesId) = 0;
		virtual void AnswerDetail(const std::string& strQuesId) = 0;
      /*
       *  接口说明：当GetWebinarDetailInfo调用成功之后，可以获取房间基本信息。
       *  返回值：当前课堂基本信息。
       */
      virtual CourseInfo GetCourseInfo() const = 0;
      /*
      *  接口说明：当JoinClassRoom调用成功之后，可以获取房间基本信息。
      *  返回值：当前课堂基本信息。
      */
      virtual ClassRoomBaseInfo GetClassRoomBaseInfo() const = 0;
      virtual void ChangeDocMode(int iMode) = 0;
      virtual std::string GetClassToken() const = 0;
      /*
      * 网络断开重连接口
      */
      virtual int ReconnectNetWork() = 0;
      /*
      *   接口说明：连接互动媒体房间,默认自动订阅远端流。可以通过EnableSubScribeStream/DisableSubScribeStream控制是否自动订阅
      *   回调监听：监听RoomEvent_ConnectMediaRoom枚举事件与回调OnWebRtcRoomConnetEventCallback
      */
      virtual void ConnectVHMediaRoom(/*const int& iPlayType*/) = 0;
      /*
      *   使能订阅流并开始订阅房间中的远端流
      */
      virtual void EnableSubScribeStream() = 0;
      /*
      *  禁止订阅流并取消订阅远端流
      */
      virtual void DisableSubScribeStream() = 0;
      /*
      *  接口说明：退出互动媒体房间
      *  返回值：true 退出成功；false 退出失败
      */
      virtual bool LeaveVHMediaRoom() = 0;
      ///**
      //*  断开互动房间
      //*/
      //virtual void DisConnectVHMediaRoom() = 0;
      /*
      *  接口说明：退出课堂房间房间
      *  返回值：true 退出成功；false 退出失败
      */
      virtual bool LeaveClassRoom() = 0;
      /*
      *   接口说明：讲师操作开始上课。
      *   返回值：ERROR_CODE
      *   回调监听：OnStartClass
      **/
      virtual int StartClass(const int& iDefinitionId) = 0;
      /*
      *   接口说明: 讲师操作下课
      *        createDefaultRecord: 是否生成默认回放 1是 0否
      *   返回值：ERROR_CODE
      *   回调监听：OnStopClass
      **/
      virtual int StopClass(int createDefaultRecord) = 0;

      //      /*
      //*   接口说明: 设置桌面共享布局(公开课)与混流布局(小课堂) PC端
      //*       LayoutMode   必填项  1: 主次平铺 2:均匀排列
      //**/
      //virtual int syncMixedLaoutMode(int LayoutMode) = 0;
      /*
      *  接口说明：讲师获取课堂人员列表
      *  参数说明：
      *          curPage：当前查寻第几页
      *          pageSize：当前也返回多少数据，每页展示条数。默认10条，最大不能超过50
      *  回调监听：
      *       成功：http请求监听RoomEvent_OnLineUserList，回调函数监听OnGetVHRoomMembers
      *   返回值：ERROR_CODE
      */
      virtual int AsynGetClassRoomMembers(int pageSize = 50, int curPage = -1) = 0;
      /**
      *   接口说明：获取举手列表。
      *   回调监听：
      *       成功：http请求监听RoomEvent_GetHandUpList，回调函数监听OnGetVHRoomMembers
      *   返回值：ERROR_CODE
      **/
      virtual int GetAlreadyHandsUpUserList() = 0;
      /*
      *  接口说明：获取被踢出互动房间人列表
      *  回调函数：OnGetVHRoomKickOutMembers、OnRoomFailedEvent
      */
      virtual int GetKickInavList() = 0;
      /*
      *  接口说明： 获取禁言用户列表
      **/
      virtual int GetSilencedUserList() = 0;
      /**
      *   接口说明：讲师允许学员举手开关
      *   参数说明：
      *       open: true 允许， false 不允许
      *   返回值：ERROR_CODE
      *   回调监听：OnOpenUserHandsUp
      */
      virtual int OpenUserHandsUp(bool open) = 0;
      /**
      *  接口说明：检测用户是否在上麦中。
      *  参数说明：
      *   user_id：  第三方用户ID
      *   返回值：   ERROR_CODE
      *   回调监听：OnCheckUserIsPublish
      **/
      virtual int CheckUserIsPublish(const std::string& user_id) = 0;
      /**
      *  接口说明：讲师同意申请上麦，主讲人身份使用。
      *  参数说明：
      *    audit_user_id: 受审核人第三方用户ID
      *   返回值：ERROR_CODE
      *   回调监听：OnAuditPublish
      **/
      virtual int AuditPublish(const std::string& audit_user_id) = 0;
      /**
      *  接口说明：讲师下麦指定用户，主讲人身份使用。
      *  参数说明：
      *     kick_user_id	string	是	被踢出的第三方用户ID
      *   返回值：ERROR_CODE
      *   回调监听：OnKickUserPublish
      **/
      virtual int KickUserPublish(const std::string& kick_user_id) = 0;
      /**
      *  接口参数：讲师下麦所有用户
      **/
      virtual int KickAllUnPublish() = 0;
      /**
      *  接口说明：讲师邀请学员、嘉宾上麦，主讲人身份使用。
      *  参数说明：
      *    audit_user_id: 受审核人第三放用户ID
      *   返回值：ERROR_CODE
      *   回调监听：OnAskforPublish
      **/
      virtual int AskforPublish(const std::string& audit_user_id) = 0;
      /**
      *  接口说明：讲师将学员踢出课堂房间/取消踢出课堂房间
      *  参数说明：
      *    kick_user_id:被踢出的第三方用户ID
      *    type : 1 踢出 0 取消踢出
      *   返回值：ERROR_CODE
      *   回调监听：OnKickOutClassRoom
      **/
      virtual int KickOutClassRoom(const std::string& kick_user_id, int type) = 0;
      /*
      *   接口说明：讲师授权用户画笔
      *   参数说明：
      *       userId：用户id
      *       auth: true 赋予用户画笔权限， false 收回用户画笔权限
      *   返回值：ERROR_CODE
      *   回调监听：OnChangeUserPenAuth
      */
      virtual int ChangeUserPenAuth(std::string userId, bool auth) = 0;
      /*
      *  接口说明：讲师禁言、取消禁言学员、嘉宾
      *   参数说明：
      *       userId：用户id
      *       forbid: true 禁用， false 取消禁用
      *   返回值：ERROR_CODE
      *   回调监听：OnForbiddenChat
      */
      virtual int ForbiddenChat(std::string userId, bool forbid) = 0;
      /*
      *  接口说明：讲师全体禁言、取消禁言
      *       forbid: true 禁用， false 取消禁用
      *   返回值：ERROR_CODE
      *   回调监听：OnForbiddenChatAll
      */
      virtual int ForbiddenChatAll(bool forbid) = 0;
      /*
      *   设置大画面布局
      */
      virtual int SetBigScreen(const std::string& user_id, SetBigScreenCallback cb_fun) = 0;
      /*
      *  上报桌面共享时摄像布局类型  或着上报 混流布局  互斥
      */
      virtual int SetScreenLayout( bool hasCamera, const int& iLayoutMode = -1) = 0;
      /*
      *   接口说明：讲师静音学员或嘉宾
      *   参数说明：
      *       userId：用户id
      *       close: true 关闭， false 打开
      *   回调监听：OnOperateUserMic
      */
      virtual int OperateUserMic(std::string userId, bool close) = 0;
      /*
      *   接口说明：讲师讲师静音全部用户
      *       mute: 1 静音， 0取消静音
      *   回调监听：OnOperateAllUserMic
      */
      virtual int OperateAllUserMic(int mute) = 0;
      /*
      *   接口说明：讲师关闭、打开用户摄像头
      *   参数说明：
      *       userId：用户id
      *       close: true 关闭， false 打开
      *   回调监听：OnOperateUserCamera
      */
      virtual int OperateUserCamera(std::string userId, bool close) = 0;
      /**
      *   接口说明：讲师发送公告消息
      *   参数说明：
      *       noticeMsg; 发送的公告消息
      */
      virtual int SendNoticeMsg(std::string noticeMsg) = 0;
      /**
      *  接口说明：学员、嘉宾举手，申请上麦。
      *   回调监听：OnApplyPublish
      *  返回值：VhallLiveErrCode
      */
      virtual int ApplyPublish() = 0;
      /**
      * 取消举手
      */
      virtual int CanclePublish() = 0;
      virtual int RewardStudent(const std::string& targetId) = 0;
      /**
      *  接口说明：学员、嘉宾上/下/拒绝上麦状态回执。
      *  参数说明：
      *       target_id: 操作的目的用户，如果不填写，target_id内部默认是登录用户参会id
      *       type:      PushStreamEvent
      *   回调监听：OnUserPublishCallback
      */
      virtual int UserPublishCallback(ClassPushStreamEvent type, std::string target_id = std::string()) = 0;
      /*
      *   接收说明：开启/关闭桌面共享通知。
      *   接口参数：
      *       open : true 开启， false 关闭
      ***/
      virtual void DesktopShareNotice(bool open , const std::string &target_id) = 0;
      /**
      *  接口说明：学员、嘉宾上/下麦失败通知。学员、嘉宾等非主讲人身份使用。
      *  参数说明：
      *       stream_id: 操作的流ID
      *       type:      PushStreamEvent
      *   回调监听：OnUserPublishFailedCallback
      */
      virtual int UserPublishFailedCallback(ClassPushStreamEvent type) = 0;
      /*
      *  接口说明：学员或嘉宾上报上麦能力。通过获取麦克风和摄像设备列表，查看是否有设备，如果有设备上报状态，主讲人才能邀请此成员上麦。
      *  参数说明：
      *      bool enablePublish; true 能上麦，false 不能上麦
      *   回调监听：OnUpdatePublishAbility
      */
      virtual int UpdatePublishAbility(const bool& enablePublish, const bool& enableDoublePublish) = 0;
      /**
      *  接收说明：讲师切换工具栏标签
      *   回调监听：OnChangeToToolPage
      **/
      virtual int ChangeToToolPage(ToolPage pageIndex) = 0;

      /**
    *   接口说明：上报推流设备信息
    *   参数说明：
    *       audio_id; 上麦mic设备
    *       camera_id; 上麦摄像头设备
    *       double_camera_id; 双推设备id
    *       roleType; 发起双推身份
    */
      virtual int SendMicsDevice(const std::string &audio_id = "", const std::string& camera_id = "", const std::string& double_camera_id = "", const int roleType = -1) = 0;

      /**
        *   接口说明：邀请双推
        *   参数说明：
        *       target_id; 被邀请目标id
        */
      virtual int inviteDoubleVrtc(const std::string &target_id) = 0;

      /**
       *   接口说明：邀请桌面共享
       *   参数说明：
       *       target_id; 被邀请目标id
       */
      virtual int inviteDesktopVrtc(const std::string &target_id) = 0;
      /**
       *   接口说明：双推-开始
       *   参数说明：
       *
       */
      virtual int startDoubleVrtc() = 0;
      /**
        *   接口说明：双推-结束
        *   参数说明：
        *
        */
      virtual int stopDoubleVrtc(const std::string &target_id) = 0;
      /**
      *   接口说明：学员双向屏幕共享-结束
      *   参数说明：
      *
      */
      virtual int stopDesktopSharing(const std::string &target_id) = 0;
      /**
       *   接口说明：双推-结束
       *   参数说明：
       *
       */
      virtual int doubleVrtcUsers() = 0;

    /**
    *   开始桌面采集与采集音量控制
    *   参数：
    *       dev_index:设备索引
    *       dev_id;设备id
    *       float:采集音量 0-100
    **/
    virtual int StartLocalCapturePlayer(const int dev_index, const std::wstring dev_id, const int volume) = 0;
	 /**
    *   停止桌面采集音量
    **/
	 virtual int StopLocalCapturePlayer() = 0;
    /*
    *   接口说明：互动房间是否已经连接成功
    *   返回值：true互动房间已连接，false互动房间链接断开
    */
    virtual bool IsVHMediaConnected() = 0;
    /*
    *   接口说明：获取摄像头列表
    *   参数说明：返回获取到的设备列表，如果设备列表为空说明没有获取到相应的设备
    */
    virtual void GetCameraDevices(std::list<vhall::VideoDevProperty> &cameras) = 0;
    /*
    *   接口说明：获取麦克风列表
    *   参数说明：返回获取到的设备列表，如果设备列表为空说明没有获取到相应的设备
    **/
    virtual void GetMicDevices(std::list<vhall::AudioDevProperty> &micDevList) = 0;
    /**
    *   接口说明：获取扬声器列表
    *   参数说明：返回获取到的设备列表，如果设备列表为空说明没有获取到相应的设备
    **/
    virtual void GetPlayerDevices(std::list<vhall::AudioDevProperty> &playerDevList) = 0;
    /*
    *  是否存在音频或视频设备。
    *  返回值：只要存在一类设备 返回true, 如果音视频设备都没有则返回false
    **/
    virtual bool HasVideoOrAudioDev() = 0;
    virtual bool HasVideoDev() = 0;
    virtual bool HasAudioDev() = 0;
    /*
    *  接口说明：设置使用的麦克风
    *  参数说明：
    *   index: GetMicDevices获取的列表中 VhallLiveDeviceInfo中的 devIndex
    */
    virtual void SetUsedMic(int index, std::string devId, std::wstring desktopCaptureId) = 0;
    /**
    *  获取当前使用的麦克风设备信息
    ***/
    virtual void GetCurrentMic(int &index, std::string& devId) = 0;
    /**
    *  获取当前使用的摄像设备信息
    ***/
    virtual void GetCurrentCamera(std::string& devId) = 0;
    /*
    *  切换摄像头，按照设备列表中设备自动切换到下一个。
    *  回调监听是否切换成功
    */
    virtual void ChangeCamera() = 0;
      virtual VideoProfileIndex GetVideoProfileByWebinarInfo(int index, const int iVideoProFileType = -1) = 0;
      /*
      *  接口说明：讲师开启旁路直播,此接口是互动房间有旁路推流权限的用户进行调用的,当处理桌面共享或插播时如果想显示在主画面需要调用SetMainView接口，
                    将对应的流ID作为参数进行设置，如果只想显示一路则同时需要设置布局模式。需要监听回调
      *  参数说明：
      *     layoutMode:混流端布局模式
      *  回调监听：RoomEvent::RoomEvent_Start_MixStream
      *  返回值：VhallLiveErrCode
      */
      virtual int StartBroadCastLayout(LayoutMode layoutMode, BROAD_CAST_DPI dip = BROAD_CAST_DPI_960_720, SetBroadCastEventCallback callback = nullptr) = 0;
      /*
      *  设置旁路布局模式
      */
      virtual int SetConfigBroadCastLayOut(LayoutMode mode, SetBroadCastEventCallback fun = nullptr) = 0;
      /**
      *  接口说明：讲师开启旁路直播之后，可以将某个用户的流设置在混流画面的主画面当中，此接口是互动房间有旁路推流权限的用户进行调用
      *  回调监听：RoomEvent::RoomEvent_SetMainView
      *  返回值：VhallLiveErrCode
      **/
      virtual int SetMainView(std::string stream_id) = 0;
      /*
      *   接口说明：停止旁路直播。此接口是互动房间有旁路推流权限的用户进行调用的
      *   参数说明：
      *       live_room_id: 直播房间id
      *   回调监听：RoomEvent::RoomEvent_Stop_PublishInavAnother
      *   返回值：VhallLiveErrCode
      */
      virtual int StopPublishInavAnother(std::string live_room_id) = 0;
      /*
      *  设置桌面音频采集音量
      **/
      virtual int SetLocalCapturePlayerVolume(const int volume) = 0;


      virtual void SubScribeRemoteStream(const std::string &stream_id, int delayTimeOut = 0) = 0;
      /*
      *  接口说明：设置使用的扬声器
      *  参数说明：
      *   index: GetPlayerDevices获取的列表中 VhallLiveDeviceInfo中的 devIndex
      */
      virtual void SetUsePlayer(int index, std::string devId) = 0;
      /**
      *  获取当前使用的摄像设备信息
      ***/
      virtual void GetCurrentCameraInfo(int &index, std::string& devId) = 0;
      /**
      * 摄像头画面预览，当预览结束之后需要停止预览，否则摄像头将被一直占用
      */
      virtual int PreviewCamera(void* wnd, const std::string& devGuid, VideoProfileIndex index, int micIndex = -1) = 0;
      /*
      *   切换预览麦克风
      */
      virtual void ChangePreViewMic(int micIndex) = 0;
      /*
      *   获取预览时麦克风音量。
      *   返回值：音量值。
      */
      virtual int GetMicVolumValue() = 0;
      /*
      * 停止摄像头预览
      */
      virtual int StopPreviewCamera() = 0;
      /*
      *   接口说明：设置当前使用的麦克风音量
      *   参数说明：
      *       vol:参数范围 0~100
      **/
      virtual bool SetCurrentMicVol(int vol) = 0;
      /*
      *   接口说明：获取当前使用的麦克风音量
      *   返回值：当前音量
      **/
      virtual int GetCurrentMicVol() = 0;
      /*
      *   接口说明：设置当前使用的扬声器音量
      *   参数说明：
      *       vol:参数范围 0~100
      **/
      virtual bool SetCurrentPlayVol(int vol) = 0;
      /*
      *   接口说明：获取当前使用的扬声器音量
      *   返回值：返回当前的音量值
      **/
      virtual int GetCurrentPlayVol() = 0;
      /*
      *   接口说明：关闭摄像头
      *   返回值：true 关闭成功/ false 关闭失败
      */
      virtual bool CloseCamera() = 0;
      /*
      *   接口说明：打开已经开始采集的摄像头
      *   返回值：true 打开成功/ false 打开失败
      */
      virtual bool OpenCamera() = 0;
      /*
      *   接口说明：摄像头是否关闭
      *   返回值：true 已经关闭/ false 未关闭
      */
      virtual bool IsCameraOpen() = 0;
      /*
      *   接口说明：关闭麦克风
      *   返回值：true 关闭成功/ false 关闭失败
      */
      virtual bool CloseMic() = 0;
      /*
      *   接口说明：打开麦克风
      *   返回值：true 打开成功/ false 打开失败
      */
      virtual bool OpenMic() = 0;
      /*
      *  动态切换当前使用的麦克风.注意使用此接口时是针对已经触发了开始采集。
      */
      virtual int ChangeUserMicDev(int index) = 0;
      /*
      *  动态切换当前使用的扬声器 .注意使用此接口时是针对已经触发了开始采集。
      */
      virtual int ChangeUserPlayDev(int index) = 0;
      /*
      *   接口说明：麦克风是否打开
      *   返回值：true 已经关闭/ false 未关闭
      */
      virtual bool IsMicOpen() = 0;
      /*
      *   判断图片有效性
      */
      virtual int CheckPicEffectiveness(const std::string filePath) = 0;
      /*
      *   接口说明：开始摄像头\麦克风采集
      *   参数说明：
      *       VideoProfileIndex：通过获取设备信息时得到VideoDevProperty::mDevFormatList中设备支持的分辨率，根据分辨率信息选区对应VideoProfileIndex
      *   监听：OnOpenCaptureCallback
      *   返回值：VhallLiveErrCode
      */
      virtual int StartLocalCapture(std::string devId, VideoProfileIndex index) = 0;
      /*
      *   开始采集图片画面
      *   参数：
      *   回调检测 OnOpenCaptureCallback
      **/
      virtual int StartLocalCapturePicture(const std::string filePath, VideoProfileIndex index) = 0;
      /*
      *   开启辅助双推摄像头采集
      */
      virtual int StartLocalAuxiliaryCapture(const std::string devId, VideoProfileIndex index) = 0;
      /*
      *   停止辅助双推摄像头采集
      */
      virtual void StopLocalAuxiliaryCapture() = 0;
      /*
      *   接口说明：停止摄像头\麦克风采集  如果已经开始推流会同时停止推流
      */
      virtual void StopLocalCapture() = 0;
      /*
      *   接口说明：开始摄像头数据推流 ，必须先打开摄像头之后才能推流
      *   回调监听：OnPushStreamSuc OnPushStreamError
      */
      virtual int StartPushLocalStream() = 0;
      /*
      *   接口说明：停止摄像头数据推流
      *   回调监听： OnStopPushStreamCallback
      */
      virtual int StopPushLocalStream() = 0;
      /*
      *   开始辅助摄像头数据推流
      *   context ： 推流添加的自定义字段
      *   回调检测：OnStartPushLocalAuxiliaryStream
      */
      virtual int StartPushLocalAuxiliaryStream(std::string context = std::string()) = 0;
      /*
      *   停止辅助摄像头数据推流
      *   回调检测：OnStopPushLocalAuxiliaryStream
      */
      virtual int StopPushLocalAuxiliaryStream() = 0;
      /*
      *   接口说明：当前采集类型是否正在进行本地数据源采集
      */
      virtual bool IsCapturingStream(VHStreamType streamType) = 0;
      /**
      *   接口说明：当前互动房间是否存在桌面共享视频流
      */
      //virtual bool IsUserPushingDesktopStream() = 0;
      /**
      *   接口说明：当前互动房间是否存在插播视频流
      *   返回值：ture成功。false失败
      */
      //virtual bool IsUserPushingMediaFileStream() = 0;
      /**
      *   接口说明：当前采集类型是否正在推流
      *   返回值：ture成功。false失败
      */
      virtual bool IsPushingStream(VHStreamType streamType) = 0;
      /*当前房间是否存在给定类型的远端流*/
      virtual bool IsExitSubScribeStream(const vlive::VHStreamType& streamType) = 0;
      virtual bool IsExitSubScribeStream(const std::string& id, const vlive::VHStreamType& streamType) = 0;
      /**
      *   获取流ID
      *   返回值：如果有流id返回流id 否则返回空字符串
      */
      virtual std::string GetSubScribeStreamId(const std::wstring& user, vlive::VHStreamType streamType) = 0;
      /**
      *   获取订阅流中流类型，是否包括音视频数据。
      *   返回值：0 查找成功，其它失败
      */
      virtual int GetSubScribeStreamFormat(const std::wstring& user, vlive::VHStreamType streamType, bool &hasVideo, bool &hasAudio) = 0;
      /**
      *   开始渲染"本地"摄像设备、桌面共享、文件插播媒体数据流.
      */
      virtual bool StartRenderLocalStream(vlive::VHStreamType streamType, void* wnd) = 0;
      virtual bool StartRenderLocalStreamByInterface(vlive::VHStreamType streamType, std::shared_ptr<vhall::VideoRenderReceiveInterface> receive) = 0;
      virtual bool IsStreamExit(std::string id) = 0;
      /**
      *   开始渲染"远端"摄像设备、桌面共享、文件插播媒体数据流.
      */
      virtual bool StartRenderRemoteStream(const std::wstring& user, vlive::VHStreamType streamType, void* wnd) = 0;
      virtual bool StartRenderRemoteStreamByInterface(const std::wstring& user, vlive::VHStreamType streamType, std::shared_ptr<vhall::VideoRenderReceiveInterface> receive) = 0;
      virtual bool IsRemoteStreamIsExist(const std::wstring& user, vlive::VHStreamType streamType) = 0;

      /*
      *   停止渲染远端流
      */
      virtual bool StopRenderRemoteStream(const std::wstring& user, const std::string streamId, vlive::VHStreamType streamType) = 0;

      /*
      *  接口说明：当接收到远端摄像头数据后，可以修改订阅的画面为大流或小流
      */
      virtual bool ChangeSubScribeUserSimulCast(const std::wstring& user_id, vlive::VHStreamType streamType, VHSimulCastType type) = 0;

      /*
      *   接口说明：开始桌面共享采集
      *   监听：OnOpenCaptureCallback
      *   返回值：VhallLiveErrCode
      */
      virtual int StartDesktopCapture(int64_t index, VideoProfileIndex profileIndex) = 0;
      /*
      *   接口说明：停止桌面共享采集  如果已经开始推流会同时停止推流
      */
      virtual void StopDesktopCapture() = 0;
      /*
      *   接口说明：开始桌面共享采集推流
      *   回调检测：OnPushStreamSuc OnPushStreamError
      */
      virtual int StartPushDesktopStream(std::string context = std::string()) = 0;
      /*
      *   接口说明：停止桌面共享采集推流
      *   需要回调检测:OnStopPushStreamCallback
      *   返回值：VhallLiveErrCode
      */
      virtual int StopPushDesktopStream() = 0;
      /*
      *   接口说明：开始插播文件,filePath 文件路径需要采用反斜杠方式。例如"G:\\file\\play.mp4"
      *   返回值：VhallLiveErrCode
      *   回调监听：OnOpenCaptureCallback
      */
      virtual int StartPlayMediaFile(std::string filePath) = 0;
      /*
      *   接口说明：查看当前播放的文件是否支持音频或视频格式，用于判断当前播放的是音频文件还是视频文件
      *   接口参数：
            type：音频、视频
      *   返回值：true 支持/ false 不支持
      */
      virtual bool IsSupportMediaFormat(CaptureStreamAVType type) = 0;
      /*
      *   接口说明：停止插播文件, 如果已经开始推流会同时停止推流
      */
      virtual void StopMediaFileCapture() = 0;
      /*
      *   接口说明：开始插播文件推流
      *   返回值：VhallLiveErrCode
      *   回调检测：OnPushStreamError  OnPushStreamSuc
      */
      virtual int StartPushMediaFileStream() = 0;
      /*
      *   接口说明：停止插播文件推流
      *   回调检测：OnStopPushMediaFileStream
      */
      virtual void StopPushMediaFileStream() = 0;
      /*
      *   接口说明：插播文件暂停处理
      *   返回值：VhallLiveErrCode
      */
      virtual void MediaFilePause() = 0;
      /*
      *   插播文件恢复处理
      *   返回值：VhallLiveErrCode
      */
      virtual void MediaFileResume() = 0;
      /*
      *   接口说明：插播文件快进处理
      *   返回值：VhallLiveErrCode
      */
      virtual void MediaFileSeek(const unsigned long long& seekTimeInMs) = 0;
      /*
      *   接口说明：插播文件总时长
      */
      virtual const long long MediaFileGetMaxDuration() = 0;
      /*
      *   接口说明：插播文件当前时长
      */
      virtual const long long MediaFileGetCurrentDuration() = 0;
      /*
      *   接口说明：插播文件的当前状态
      *   返回值：播放状态  MEDIA_FILE_PLAY_STATE
      */
      virtual int MediaGetPlayerState() = 0;
      /*
      *   添加播放音频文件接口
      */
      virtual int PlayAudioFile(std::string fileName, int devIndex) = 0;
      /*
      *   获取播放文件音量
      */
      virtual int GetPlayAudioFileVolum() = 0;
      /*
      *   停止播放音频文件接口
      */
      virtual int StopPlayAudioFile() = 0;

      /*
      *    获取窗口共享和桌面共享列表
      **  vlive::VHStreamType      // 3:Screen,5:window
      */
      virtual std::vector<DesktopCaptureSource> GetDesktops(int streamType) = 0;
      /**
      *  打开或关闭所有订阅的音频
      */
      virtual int MuteAllSubScribeAudio(bool mute) = 0;
      virtual bool GetMuteAllAudio() = 0;
      /*设置选择的软件源*/
      virtual int SelectSource(int64_t id) = 0;
      /*停止软件源共享采集*/
      virtual void StopSoftwareCapture() = 0;
      /*停止软件共享采集推流*/
      virtual int StopPushSoftWareStream() = 0;
      /*开始软件共享采集推流*/
      virtual int StartPushSoftWareStream() = 0;
      /* 当前互动房间是否存在软件视频流*/
      //virtual bool IsUserPushingSoftwareStream() = 0;
       /**
       *   Http 日志上报
       */
      virtual void UpLoadLog(const std::string& url) = 0;
      virtual VHStreamType  CalcStreamType(const bool& bAudio, const bool& bVedio) = 0;
      virtual std::string GetAuxiliaryId() = 0;
      virtual std::string GetLocalAuxiliaryId() = 0;
	  virtual  void SetWhiteListPhone(const std::string& phone) = 0;
     /*返回登录时候获取的课堂类型*/
     virtual int LoginPlayType() = 0;
     virtual void GetStreams(std::list<std::string>& streams) = 0;

     virtual void ClearSubScribeStream() = 0;
     virtual void StopRecvAllRemoteStream() = 0;//停止接受远端流
     virtual std::string LocalStreamId() = 0;
     /**
*  获取推流视频丢包率.3秒读一次
*/
     virtual double GetPushDesktopVideoLostRate() = 0;
   };

   VHCLASS_SDK_EXPORT VHClassRoomInterface* GetClassSDKInstance();
   VHCLASS_SDK_EXPORT void DestoryClassSDKInstance();

}
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
////开发环境
//#define HOST_URL  http://dev-class.e.vhall.com/
////测试环境
////#define HOST_URL "http://t-api-class.e.vhall.com"
////线上环境
////#define HOST_URL "http://api-class.e.vhall.com"
#define PRE_URL  "api/sdk/"

#define CLASS_SSDK_VER    "2.1.0.0"
#define DIP_1080P   "1080P"
#define DIP_720P    "720P"
#define DIP_540P    "540P"
#define DIP_480P    "480P"
#define DIP_240P    "240P"

//（标清）
#define DPI_SD      "SD"
//（高清）
#define DPI_HD      "HD"
// （超清）
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
   //初始化话接口必须在main函数中调用。
   virtual void InitNetWorkProtocol(std::string app_key, std::string app_secret, const std::string& domain, VHRoomMonitor* monitor, std::wstring logPath = std::wstring());
  
   virtual void InitSDK(const int& iPlayType);
   virtual void InitWebRtcSDK();
   //virtual void InitSDK(std::string app_key, std::string app_secret, const std::string& domain, VHRoomMonitor* monitor, std::wstring logPath = std::wstring());
   virtual void SetDebugLogAddr(const std::string& addr);
   /*
   *   接口说明：获取课程详情
   *   接口参数：
   *      webinar_id：课程ID
   *   回调监听：监听RoomEvent::RoomEvent_Login事件
   */
   virtual void GetWebinarDetailInfo(const std::string& webinar_id);
   /**
   *  接口说明：进入课堂房间
   *  回调监听：监听RoomEvent_JoinClassRoom枚举事件
   *  参数说明：
   *       nickName：昵称
   *       password：口令
   **/
   virtual void JoinClassRoom(std::string nickName, std::string password, std::string phone);
   virtual void JoinClassRoom(std::string cmd);
   virtual void JoinPaasRoom();
   /**
   *  接口说明：主讲人开始/取消倒计时
   *  回调监听：
    ["cmd",{"type":"*startCountdown","join_id":395758,"time":120}]
    ["cmd",{"type":"*cancelCountdown","join_id":395758}]
   *  参数说明：
   * iTime 倒计时秒数
   *iIsPlayEndSounds 是否播放结束音效 1 播放 0 不播放
   **/
   virtual void StartCoutDownTool(const int& iTime, const int& iIsPlayEndSounds);
   virtual void CancleCoutDownTool();
   /**
   *  接口说明：发起签到
   *  回调监听：
    * {'type':'*startSign','webinar_id':'edu_xxxx','sign_id':'签到id','user_id':'商户id','sign_show_time':30}
   **/
   virtual void AddSign();
   virtual void SignIn(const std::string& strAddSignId);
   virtual void GetSignRecord(const std::string& strAddSignId);
   virtual void SignStop(const std::string& strAddSignId);
   /**
   *  接口说明：发起答题器
   *  回调监听：
    *  {"subject":"答题器1574131026546","options":[{"subject":"A","is_answer":0,"key":"A"},{"subject":"B","is_answer":1,"key":"B"},{"subject":"C","is_answer":1,"key":"C"},{"subject":"D","is_answer":0,"key":"D"}],"type":"scantron"}
   **/
	virtual void StartAnswer(const std::string& strContent);
	virtual void AnswerIn(const std::string& strAnswer, const std::string& strQuesId);
	virtual void AnnounceAnswer(const std::string& strQuesId);
	virtual void StopAnswer(const std::string& strQuesId);
	virtual void AnswerDetail(const std::string& strQuesId);
   /*
   *  接口说明：当GetWebinarDetailInfo调用成功之后，可以获取房间基本信息。
   *  返回值：当前课堂基本信息。
   */
   virtual CourseInfo GetCourseInfo() const;

   virtual int SetBigScreen(const std::string& user_id, SetBigScreenCallback cb_fun);

   virtual int SetScreenLayout(bool hasCamera, const int& iLayoutMode = -1);
   /*
   *  接口说明：当GetWebinarDetailInfo调用成功之后，可以获取房间基本信息。
   *  回调监听：监听RoomEvent::RoomEvent_Logout事件
   */
   virtual ClassRoomBaseInfo GetClassRoomBaseInfo() const;
   virtual void ChangeDocMode(int iMode);//0 文档  1 白板
   virtual std::string GetClassToken() const;
   /*
   *   接口说明：连接媒体房间
   *   回调监听：监听RoomEvent_ConnectMediaRoom枚举事件
   */
   virtual void ConnectVHMediaRoom(/*const int& iPlayType*/);

   /*
*  接口说明：退出互动媒体房间
*  返回值：true 退出成功；false 退出失败
*/
   bool LeaveVHMediaRoom();
   //virtual void DisConnectVHMediaRoom();
   /*
   *   使能订阅流并开始订阅房间中的远端流
   */
   virtual void EnableSubScribeStream();
   /*
   *  禁止订阅流并取消订阅远端流
   */
   virtual void DisableSubScribeStream();
   /*
   *  接口说明：退出课堂房间房间
   *  返回值：true 退出成功；false 退出失败
   */
   virtual bool LeaveClassRoom();
   /*
   *   接口说明：上课
   **/
   virtual int StartClass(const int& iDefinitionId);
   /*
   *   接口说明: 下课
   **/
   virtual int StopClass(int createDefaultRecord);
   ///*
   //*   接口说明: 设置桌面共享布局(公开课)与混流布局(小课堂) PC端
   //*       LayoutMode   必填项  1: 主次平铺 2:均匀排列
   //**/
   //virtual int syncMixedLaoutMode(int LayoutMode);
   /*
   * SDK互动房间人员列表
   **/
   virtual int AsynGetClassRoomMembers(int pageSize = 50, int curPage = -1);
   /*
   *  接口说明： 获取禁言用户列表
   **/
   virtual int GetSilencedUserList();
   /**
   *   接口说明：获取举手列表。
   *   回调监听：
   *       成功：http请求监听RoomEvent_GetHandUpList，回调函数监听OnGetVHRoomMembers
   *   返回值：ERROR_CODE
   **/
   virtual int GetAlreadyHandsUpUserList();
   /**
   *   接口说明：允许举手学员举手开关
   *   参数说明：
   *       enable: true 允许， false 不允许
   */
   virtual int OpenUserHandsUp(bool enable);
   /*
   *   接收说明：开启/关闭桌面共享通知。
   *   接口参数：
   *       open : true 开启， false 关闭
   ***/
   virtual void DesktopShareNotice(bool open, const std::string &target_id);
   /**
  *  接口说明：检测用户是否在上麦中。
  *  参数说明：
  *   user_id：  第三方用户ID
  *   返回值：   ERROR_CODE
  *   回调监听：OnCheckUserIsPublish
  **/
   virtual int CheckUserIsPublish(const std::string& user_id);
   /**
   * 申请上麦
   */
   virtual int ApplyPublish();
   /**
   * 取消举手
   */
   virtual int CanclePublish();
   //奖励（讲师给学员奖杯）
   virtual int RewardStudent(const std::string& targetId);
   /**
   * 同意申请上麦
   * audit_user_id: 受审核人第三放用户ID
   * 返回值：VhallLiveErrCode
   **/
   virtual int AuditPublish(const std::string& audit_user_id);
   /**
   * 邀请上麦
   * audit_user_id: 受审核人第三方用户ID
   **/
   virtual int AskforPublish(const std::string& audit_user_id);
   /**
   *  接口说明：学员、嘉宾上/下/拒绝上麦状态回执。
   *  参数说明：
   *       target_id: 操作的目的用户，如果不填写，target_id内部默认是登录用户参会id
   *       type:      PushStreamEvent
   *   回调监听：OnUserPublishCallback
   */
   virtual int UserPublishCallback(ClassPushStreamEvent type, std::string target_id = std::string());
   /**
   *  接口说明：学员、嘉宾上/下麦失败通知。学员、嘉宾等非主讲人身份使用。
   *  参数说明：
   *       stream_id: 操作的流ID
   *       type:      PushStreamEvent
   *  回调监听事件：RoomEvent::RoomEvent_UserPublishCallback
   */
   virtual int UserPublishFailedCallback(ClassPushStreamEvent type);

   virtual int StartLocalCapturePlayer(const int dev_index, const std::wstring dev_id, const int volume);

   virtual int SetLocalCapturePlayerVolume(const int volume);

	virtual int StopLocalCapturePlayer();
   /**
   *  接口说明：下麦指定用户
   *  参数说明：
   *     kick_user_id	string	是	被踢出的第三方用户ID
   *  回调监听事件：RoomEvent::RoomEvent_KickInavStream
   *  返回值：VhallLiveErrCode
   **/
   virtual int KickUserPublish(const std::string& kick_user_id);
   /**
   *  接口参数：讲师下麦所有用户
   **/
   virtual int KickAllUnPublish();
   /**
   *  接口说明：踢出课堂房间/取消踢出课堂房间
   *  参数说明：
   *    kick_user_id:被踢出的第三方用户ID
   *  回调监听事件：RoomEvent::RoomEvent_KickInav
   *  返回值：VhallLiveErrCode
   **/
   virtual int KickOutClassRoom(const std::string& kick_user_id, int type);
   /*
   *  接口说明：禁言、取消禁言
   *  参数说明：
   */
   virtual int ForbiddenChat(std::string userId, bool forbid);
   /*
   *  接口说明：全体禁言、取消禁言
   *  参数说明：
   */
   virtual int ForbiddenChatAll(bool forbid);
   /*
   * 获取被踢出互动房间人列表
   */
   virtual int GetKickInavList();
   /***
   *   接口说明：关闭、打开用户麦克风
   *   参数说明：
   *       userId：用户id
   *       close: true 关闭， false 打开
   */
   virtual int OperateUserMic(std::string userId, bool close);
   /***
   *   接口说明：关闭、打开用户摄像头
   *   参数说明：
   *       userId：用户id
   *       close: true 关闭， false 打开
   */
   virtual int OperateUserCamera(std::string userId, bool close);
   /*
      *   接口说明：讲师讲师静音全部用户
      *       mute: 1 静音， 0取消静音
      *   回调监听：OnOperateAllUserMic
      */
   virtual int OperateAllUserMic(int mute);
   /***
   *   接口说明：授权用户画笔
   *   参数说明：
   *       userId：用户id
   *       auth: true 赋予用户画笔权限， false 收回用户画笔权限
   */
   virtual int ChangeUserPenAuth(std::string userId, bool auth);
   /**
   *   接口说明：发送公告消息
   *   参数说明：
   *       noticeMsg; 发送的公告消息
   */
   virtual int SendNoticeMsg(std::string noticeMsg);

   //////////////////////////////////////////////////////////////////////////////////////////////////////

   /*          需要优化调整
   *  接口说明：学员或嘉宾上报上麦能力。通过获取麦克风和摄像设备列表，查看是否有设备，如果有设备上报状态，主讲人才能邀请此成员上麦。
   *  参数说明：
   *      bool enablePublish; true 能上麦，false 不能上麦
   */
   virtual int UpdatePublishAbility(const bool& enablePublish, const bool& enableDoublePublish);

   //////////////////////                     新增              //////////////////////
   /**
   *   接口说明：上报推流设备信息
   *   参数说明：
   *       audio_id; 上麦mic设备
   *       camera_id; 上麦摄像头设备
   *       double_camera_id; 双推设备id
   *       roleType; 发起双推身份
   */
   virtual int SendMicsDevice(const std::string &audio_id = "", const std::string& camera_id = "", const std::string& double_camera_id = "", const int roleType = -1);

   /**
     *   接口说明：邀请双推
     *   参数说明：
     *       target_id; 被邀请目标id
     */
   virtual int inviteDoubleVrtc(const std::string &target_id);

   /**
 *   接口说明：邀请桌面共享
 *   参数说明：
 *       target_id; 被邀请目标id
 */
   virtual int inviteDesktopVrtc(const std::string &target_id);
   /**
    *   接口说明：双推-开始
    *   参数说明：
    *
    */
   virtual int startDoubleVrtc();
   /**
     *   接口说明：双推-结束
     *   参数说明：
     *
     */
   virtual int stopDoubleVrtc(const std::string &target_id);
   /**
  *   接口说明：学员双向屏幕共享-结束
  *   参数说明：
  *
  */
   virtual int stopDesktopSharing(const std::string &target_id);
   /**
    *   接口说明：支持双推用户列表
    *   参数说明：
    *
    */
   virtual int doubleVrtcUsers();

   //////////////////////////////////////////////////////////////////////////////////////////////////////

   virtual VideoProfileIndex GetVideoProfileByWebinarInfo(int index, const int iVideoProFileType = -1);
   /*
   *   开启旁路直播
   *   live_room_id: 直播房间id
   *   width： 混流端布局的宽
   *   height： 混流端布局的高
   *   fps：帧率
   *   bitrate： 码率
   *   stream_id： 大画面流id
   */
   virtual int StartBroadCastLayout(LayoutMode layoutMode, BROAD_CAST_DPI dip = BROAD_CAST_DPI_960_720, SetBroadCastEventCallback callback = nullptr);
   virtual int SetConfigBroadCastLayOut(LayoutMode mode, SetBroadCastEventCallback fun = nullptr);
   /**
   *  当开启旁路直播之后，可以将某个用户的流设置在混流画面的主画面当中
   **/
   virtual int SetMainView(std::string stream_id);

   virtual int CheckPicEffectiveness(const std::string filePath);


   /*
   *   停止旁路直播
   *   live_room_id: 直播房间id
   */
   virtual int StopPublishInavAnother(std::string live_room_id);


   virtual int StartLocalCapturePicture(const std::string filePath, VideoProfileIndex index);

   virtual bool IsVHMediaConnected();
   /*  开始摄像头\麦克风采集
   *   回调检测 OnStartLocalCapture OnCameraCaptureErr  OnLocalMicOpenErr
   */
   virtual int StartLocalCapture(std::string devId, VideoProfileIndex index);
   /*
   *   开启辅助双推摄像头采集
   */
   virtual int StartLocalAuxiliaryCapture(const std::string devId, VideoProfileIndex index);
   /*
   *   停止辅助双推摄像头采集
   */
   virtual void StopLocalAuxiliaryCapture();

   /*停止摄像头\麦克风采集*/
   virtual void StopLocalCapture();

   /*开始摄像头数据推流  回调检测：OnStartPushLocalStream*/
   virtual int StartPushLocalStream();
   /*停止摄像头数据推流 回调检测：OnStopPushLocalStream*/
   virtual int StopPushLocalStream();
   /*
*   开始辅助摄像头数据推流
*   context ： 推流添加的自定义字段
*   回调检测：OnStartPushLocalAuxiliaryStream
*/

    virtual int StartPushLocalAuxiliaryStream(std::string context = std::string());
    /*
    *   停止辅助摄像头数据推流
    *   回调检测：OnStopPushLocalAuxiliaryStream
    */
    virtual int StopPushLocalAuxiliaryStream();
    /**
    *   当前采集类型是否正在推流
    */
    virtual bool IsPushingStream(VHStreamType streamType);
    /*当前房间是否存在给定类型的远端流*/
    virtual bool IsExitSubScribeStream(const vlive::VHStreamType& streamType);
    virtual bool IsExitSubScribeStream(const std::string& id, const vlive::VHStreamType& streamType);
    /*
    *   当前采集类型是否正在进行本地数据源采集
    */
    virtual bool IsCapturingStream(VHStreamType streamType);
    /**
    *  接收说明：讲师切换工具栏标签
    **/
    virtual int ChangeToToolPage(ToolPage pageIndex);

    /*开始桌面共享采集  回调检测：OnStartDesktopCaptureSuc OnStartDesktopCaptureErr*/
    virtual int StartDesktopCapture(int64_t index, VideoProfileIndex profileIndex) ;
    /*停止桌面共享采集*/
    virtual void StopDesktopCapture() ;
    /*开始桌面共享采集推流 回调检测：OnStartPushDesktopStream*/
    virtual int StartPushDesktopStream(std::string context = std::string()) ;
    /*停止桌面共享采集推流 回调检测：OnStopPushDesktopStream*/
    virtual int StopPushDesktopStream() ;
    /*开始插播文件*/
    virtual int StartPlayMediaFile(std::string filePath) ;
    /**
    *   获取流ID
    *   返回值：如果有流id返回流id 否则返回空字符串
    */
    virtual std::string GetSubScribeStreamId(const std::wstring& user, vlive::VHStreamType streamType) ;
    
    /**
    *   获取订阅流中流类型，是否包括音视频数据。
    *   返回值：0 查找成功，其它失败
    */
    virtual int GetSubScribeStreamFormat(const std::wstring& user, vlive::VHStreamType streamType, bool &hasVideo, bool &hasAudio);
    /**
    *   开始渲染"本地"摄像设备、桌面共享、文件插播媒体数据流.
    */
    virtual bool StartRenderLocalStream(vlive::VHStreamType streamType, void* wnd) ;
    virtual bool StartRenderLocalStreamByInterface(vlive::VHStreamType streamType, std::shared_ptr<vhall::VideoRenderReceiveInterface> receive);
    virtual bool IsStreamExit(std::string id);

    /**
    *   开始渲染"远端"摄像设备、桌面共享、文件插播媒体数据流.
    */
    virtual bool StartRenderRemoteStream(const std::wstring& user, vlive::VHStreamType streamType, void* wnd);
    virtual bool StartRenderRemoteStreamByInterface(const std::wstring& user, vlive::VHStreamType streamType, std::shared_ptr<vhall::VideoRenderReceiveInterface> receive);
    virtual bool IsRemoteStreamIsExist(const std::wstring& user, vlive::VHStreamType streamType);
    /*
    *   停止渲染远端流
    */
    virtual bool StopRenderRemoteStream(const std::wstring& user, const std::string streamId, vlive::VHStreamType streamType);
    /*
    *  接口说明：当接收到远端摄像头数据后，可以修改订阅的画面为大流或小流
    */
    virtual bool ChangeSubScribeUserSimulCast(const std::wstring& user_id, vlive::VHStreamType streamType, VHSimulCastType type);

    /*
    *   接口说明：查看当前播放的文件是否支持音频或视频格式，
	用于判断当前播放的是音频文件还是视频文件
    *   接口参数：
    type：音频、视频
    *   返回值：true 支持/ false 不支持
    */
    virtual bool IsSupportMediaFormat(CaptureStreamAVType type);
    /*停止插播文件*/
    virtual void StopMediaFileCapture();
    /*开始插播文件推流  回调检测：OnStartPushMediaFileStream*/
    virtual int StartPushMediaFileStream() ;
    /*停止插播文件推流 回调检测：OnStopPushMediaFileStream*/
    virtual void StopPushMediaFileStream() ;
    //插播文件暂停处理
    virtual void MediaFilePause() ;
    //插播文件恢复处理
    virtual void MediaFileResume() ;
    //插播文件快进处理
    virtual void MediaFileSeek(const unsigned long long& seekTimeInMs) ;
    //插播文件总时长
    virtual const long long MediaFileGetMaxDuration() ;
    //插播文件当前时长
    virtual const long long MediaFileGetCurrentDuration();
    //插播文件的当前状态 
    //返回值：播放状态  MEDIA_FILE_PLAY_STATE
    virtual int MediaGetPlayerState();
    /**获取摄像头列表**/
    virtual void GetCameraDevices(std::list<vhall::VideoDevProperty> &cameras);
    /**获取麦克风列表**/
    virtual void GetMicDevices(std::list<vhall::AudioDevProperty> &micDevList) ;
    /**获取扬声器列表**/
    virtual void GetPlayerDevices(std::list<vhall::AudioDevProperty> &playerDevList);
    /*
    *  是否存在音频或视频设备。
    *  返回值：只要存在一类设备 返回true, 如果音视频设备都没有则返回false
    **/
    virtual bool HasVideoOrAudioDev();
    virtual bool HasVideoDev();
    virtual bool HasAudioDev();
    /*
    *  设置使用的麦克风
    *  index: GetMicDevices获取的列表中 vhall::VideoDevProperty中的 devIndex
    */
    virtual void SetUsedMic(int index, std::string devId, std::wstring desktopCaptureId);
    /**
    *  获取当前使用的麦克风设备信息
    ***/
    virtual void GetCurrentMic(int &index, std::string& devId);
    /**
    *  获取当前使用的摄像设备信息
    ***/
    virtual void GetCurrentCamera(std::string& devId);
    /*
    *  切换摄像头，按照设备列表中设备自动切换到下一个。
    *  回调监听是否切换成功
    */
    virtual void ChangeCamera();
    /*
    *  设置使用的扬声器
    *  index: GetPlayerDevices获取的列表中 VhallLiveDeviceInfo中的 devIndex
    */
    virtual void SetUsePlayer(int index, std::string devId);
    /**
    *  获取当前使用的摄像设备信息
    ***/
    virtual void GetCurrentCameraInfo(int &index, std::string& devId);
    /**
    * 摄像头画面预览，当预览结束之后需要停止预览，否则摄像头将被一直占用
    */
    virtual int PreviewCamera(void* wnd, const std::string& devGuid, VideoProfileIndex index, int micIndex = -1);
    /*
    *   切换预览麦克风
    */
    virtual void ChangePreViewMic(int micIndex);
    virtual int GetMicVolumValue();
    /*
    * 停止摄像头预览
    */
    virtual int StopPreviewCamera();
    /**设置当前使用的麦克风音量**/
    virtual bool SetCurrentMicVol(int vol);
    /**获取当前使用的麦克风音量**/
    virtual int GetCurrentMicVol();
    /**设置当前使用的扬声器音量**/
    virtual bool SetCurrentPlayVol(int vol);
    /**获取当前使用的扬声器音量**/
    virtual int GetCurrentPlayVol();
    //关闭摄像头
    virtual bool CloseCamera();
    //打开摄像头
    virtual bool OpenCamera();
	//关闭摄像头
	virtual bool IsCameraOpen();
    virtual bool CloseMic();

    virtual void SubScribeRemoteStream(const std::string &stream_id, int delayTimeOut = 0);
    /*
    *   打开麦克风
    */
    virtual bool OpenMic();
	//打开摄像头
	virtual bool IsMicOpen();
    /*
    *  动态切换当前使用的麦克风.注意使用此接口时是针对已经触发了开始采集。
    */
    virtual int ChangeUserMicDev(int index) ;
    /*
    *  动态切换当前使用的扬声器 .注意使用此接口时是针对已经触发了开始采集。
    */
    virtual int ChangeUserPlayDev(int index);
    /*
    *   添加播放音频文件接口
    */
    virtual int PlayAudioFile(std::string fileName,int devIndex);

    virtual int GetPlayAudioFileVolum();
    
    virtual int StopPlayAudioFile();
    /*
    *    获取窗口共享和桌面共享列表
    **  vlive::VHStreamType      // 3:Screen,5:window
    */
    virtual std::vector<DesktopCaptureSource> GetDesktops(int streamType);
	/*设置选择的软件源*/
	virtual int SelectSource(int64_t id);
	/*停止软件源共享采集*/
	virtual void StopSoftwareCapture();
	/*停止软件共享采集推流*/
	virtual int StopPushSoftWareStream();
	/*开始软件共享采集推流*/
	virtual int StartPushSoftWareStream();

	/**
	*   当前互动房间是否存在软件视频流
	*/
	//virtual bool IsUserPushingSoftwareStream();
	///**

//*   当前互动房间是否存在桌面共享视频流
//*/
//virtual bool IsUserPushingDesktopStream();
/**
*   当前互动房间是否存在插播视频流
*/
//virtual bool IsUserPushingMediaFileStream();
       /**
    *  打开或关闭所有订阅的音频
    */
   virtual int MuteAllSubScribeAudio(bool mute);
   virtual bool GetMuteAllAudio();
   virtual void GetStreams(std::list<std::string>& streams);
   /**
   *   Http 日志上报
   */
   virtual void UpLoadLog(const std::string& url);
   VHStreamType  CalcStreamType(const bool& bAudio, const bool& bVedio);
   std::string GetAuxiliaryId();
   virtual std::string GetLocalAuxiliaryId();
   virtual void ClearSubScribeStream();
   virtual void StopRecvAllRemoteStream();//停止接受远端流

   virtual int LoginPlayType();

   static DWORD WINAPI ThreadProcessJoinRTCRoom(LPVOID p);
   bool ProcessTaskJoinRTCRoom();
   virtual std::string LocalStreamId();
   /**
*  获取推流视频丢包率.3秒读一次
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
   std::string mPublishUrl;      //推流地址
   std::string mReportUrl;      //日志上报地址
   std::string mDevUniqueId;     //cpu 唯一码
   std::string mPhone;     //白名单手机号

   bool mIsHost = true;
   bool bIsSettinglayout = false;

   CourseInfo mCourseInfo;
   ClassRoomBaseInfo mClassRoomBaseInfo;
   JoinClassRoomMessage mMessageInfo;
   VideoProfileIndex videoProfile = RTC_VIDEO_PROFILE_480P_4x3_H;

   VHRoomMonitor * mpVHRoomMonitor = nullptr;                  //回调对象
   std::shared_ptr<HttpManagerInterface> mpHttpManagerInterface = nullptr;     //http网络情况库
   VHWebSocketInterface* mpVHWebSocketInterface = nullptr;     //socketIo/WebSocket库

   WebRtcSDKInterface* mpWebRtcSDKInterface = nullptr;         //课堂 互动SDK库  flash
   VHPaasInteractionRoom* mpPaasInteraction = nullptr;            //paasSdk 互动SDK库   H5
   
   ClassBroadCastInfo mCurrentBroadCastInfo;
   LogReport mLogReport;

   URLGenerator mURLGenerator;                     //URL生成器
   std::atomic_bool mbDisConnectWebSocket = false;

   std::wstring mlogPath;

   //enum ePlayType
   //{
   //   eClassPlayType_Flash = 0,
   //   eClassPlayType_H5,
   //};
   int miPlayType = eClassPlayType_Flash; // 0 flash(旧活动、课堂sdk)   1 H5 （新活动，paasSdk）
   
   std::thread* mProcessThreadJoinRtcRoom = nullptr;
   static HANDLE mStatic_TaskEvent;   //
   static HANDLE mStatic_ThreadExitEvent;
};
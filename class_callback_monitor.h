#pragma once
#include "LiveEventInterface.h"
#include "vh_room_common.h"
#include "./VhallPaasSDK/callback_monitor.h"

#define CALL_BACK_SUC   200
//接收到的房间内广播的消息通知
class VHRoomRecvInavMsgCallback {
public:

    /*处理接收flash消息*/
    virtual void OnRecvFlahsMsg(std::string flashMsg) = 0;
    /*接收插播消息*/
    virtual void OnRecvMediaMsg(std::string mediaMsg) = 0;
    /*接收文档消息*/
    virtual void OnRecvAnswerMsg(std::string msg) = 0;
    /*接收画笔消息*/
    virtual void OnRecvSetBrushMsg(std::string data, std::string msg) = 0;
    /*接口文档转换消息*/
    virtual void OnRecvDocUploadMsg(std::string msg) = 0;
    /*
    *   收到申请上麦消息
    */
    virtual void OnRecvApplyInavPublishMsg(std::wstring& third_party_user_id, const std::wstring& nickname) = 0;
    /*
    *   收到审核上麦消息 ,消息为广播消息，收到同意上麦后执行上麦操作
    */
    virtual void OnRecvAuditInavPublishMsg(const std::wstring& third_party_user_id, AuditPublish) = 0;
    /*
    *   邀请上麦消息  消息为广播消息，收到消息后，提示邀请信息
    */
    virtual void OnRecvAskforInavPublishMsg(const std::wstring& join_uid, const std::wstring& target_uid, int status) = 0;// = 0;
    /*
    *   踢出流消息  消息为广播消息，收到消息后，执行踢出流
    */
    virtual void OnRecvKickInavStreamMsg(const std::wstring& third_party_user_id) = 0;
    /*
    *   踢出互动房间
    */
    virtual void OnRecvKickOutRoomMsg(const std::string& third_party_user_id, int status) = 0;
    /*
    *   上/下/拒绝上麦消息 消息为广播消息
    */
    virtual void OnRecvUserPublishCallback(const std::string& third_party_user_id, const std::string& nickname, vlive::ClassPushStreamEvent event, 
       int role_type,int device_type, int iRewardNum, const int& iNoDoubleVrtc) = 0;
    /*
    *   互动房间关闭消息  接受到该消息后，所有在互动房间的人员，全部下麦，退出房间
    */
    virtual void OnRecvInavCloseMsg() = 0;
    /*
    *   用户上下线通知
    *   online： true 用户上线/ false 用户下线
    *   user_id： 用户id
    */
    virtual void OnUserOnLineState(bool online, const std::string user_id,int role_type) = 0;
    /**
    *   接收公告消息
    **/
    virtual void OnRecvNoticeMsg(const std::string msg) = 0;
    /**
    *   收到上课消息
    **/
    virtual void OnRecvStartClassMsg(const std::wstring& join_uid, 
       const std::wstring& nickname, 
       const int& iCurLabelNumber,
       const int& iStartPlayType) = 0;

    /**
    *   收到下课消息
    **/
    virtual void OnRecvStopClassMsg(int play_type) = 0;
    /*
    *   接收到禁言消息、取消禁言消息指定用户
    */
    virtual void OnRecvForbiddenChat(int forbid, std::string user_id, std::string data) = 0;
    /*
    *   接收到所有人禁言消息、所有人取消禁言消息
    */
    virtual void OnRecvForbiddenAllChat(int forbid, std::string user_id) = 0;
    /*
    *   更新上麦能力
    *    join_uid: 用户id
    *     statue:  0 有上麦能力，1 无上麦能力
    */
    virtual void OnRecvUpdatePublishAbility(const std::string& join_uid ,int statue) = 0;
    /*
    *   接收到上下麦失败消息
    *   app_type : 1pc 2h5 3 app 4 win
    *   status: 1 上麦失败，0 下麦失败
    */
    virtual void OnRecvPublishFailedMsg(const std::string& join_uid, int status, const std::string& nick_name, int role_type, int app_type) = 0;
	/*
	*   接收到举手超时消息
	*/
	virtual void  OnRecvHandDelayMsg(const std::string& target_id) = 0;
   //接收桌面共享
   virtual void OnRecvStopDesktopSharing() = 0;
	/*收到开始签到消息*/
	virtual void  OnRecvStartSign(const std::string&strWebinar, const std::string& sign_id, const int& signShowTime) = 0;
	virtual void  OnRecvSignIn(const std::string& strWebinar, const std::string& sign_id, const std::string& signer, const std::string& signerNickname) = 0;
	virtual void OnSignUsersMsg(std::list<std::string> stlistsignUsers, std::list<std::string> stlistNoSignUsers) = 0;
	virtual void  OnRecvSignStop(const std::string& strWebinar, const std::string& sign_id) = 0;
	/*
	*   接收取消举手消息
	*/
	virtual void  OnRecvCancelHand(std::string strJoinId, int iRoleType, std::string strNickName, std::string strTime) = 0;
	/*
    *  接收到静音消息
    *  status 状态（1静音、0取消静音）
    */
    virtual void OnRecvMuteMicMsg(const std::string& join_uid, int status) = 0;
    /**
    *   接收到全员静音消息
    *   status 状态（1静音、0取消静音）
    */
    virtual void OnRecvAllMuteMicMsg(int status) = 0;
    /*
    *  接收到禁画消息
    *  status 状态（1禁画、0取消禁画）
    */
    virtual void OnRecvMuteVideoMsg(const std::string& join_uid, int status) = 0;
    /*   *  接收到开始双推消息   */    virtual void OnRecvStartDoubleVrtc(const std::string& join_uid, const std::string& nickName) = 0;    /*   *  接收到结束双推消息   */    virtual void OnRecvEndDoubleVrtc(const std::string& target_id, const std::string& nickName) = 0;    /*
    *  接收到主讲人设置大画面处理
    */
    virtual void OnRecvSetBigScreen(const std::string& target_id) = 0;    /*
   *  接收被邀请双推消息
   */
    virtual void OnRecvInviteDoubleVrtc(std::string target_id) = 0;
    /*
   *  接收被邀请桌面共享消息
   */
    virtual void OnRecvInviteScreenSharing(std::string target_id) = 0;
    /*
    *  接收到举手开关消息
    *  status 状态（1开、0关）
    */
    virtual void OnRecvOpenHandMsg(int status) = 0;

    /**
    *  接收下麦所有用户消息
    */
    virtual void OnRecvDownAllUserPublishMsg() = 0;
};

/*
**===================================
**
**   互动SDK事件监听回调接口，所有接口回调处于SDK业务线程。
**   如果回调处理app业务事件不建议在回调中进行。
**
**===================================
*/
class VHRoomMonitor :public VHRoomRecvInavMsgCallback, public WebRtcSDKEventInterface, public VHRoomDelegate {
public:
    /*
    *   接口说明：获取课程详情
    */
    virtual void OnGetWebinarDetailInfo(int code, std::string respMsg) = 0;
    /**
    *  接口说明：进入课堂房间。
    **/
    virtual void OnJoinClassRoom(int code, std::string respMsg, const int& iCurLabelNumber = -1) = 0;
    virtual void OnInitQualityNumber(const int& iCurLabelNumber = 1) = 0;
    /*
    *   接口说明：讲师操作开始上课。
    *   返回值：ERROR_CODE
    *   回调监听：RoomEvent
    **/
    virtual void OnStartClass(int iPlayType, int code, std::string respMsg) = 0;
    /*
    *   接口说明: 讲师操作下课
    *        createDefaultRecord: 是否生成默认回放 1是 0否
    *   返回值：ERROR_CODE
    *   回调监听：RoomEvent
    **/
    virtual void OnStopClass(int createDefaultRecord, int iPlayType, int code, std::string respMsg) = 0;
    /*
    *   监听互动房间内HTTP业务API调用成功事件
    *   event: RoomEvent / ClassRoomEvent事件类型 
    *   code: 错误码
    *   msg: 响应消息
    *   userData: 附带的json数据
    */
    virtual void OnCallbackEvent(int event, int code = CALL_BACK_SUC, std::string msg = std::string(), std::string userData = std::string()) = 0;
    /***
    *   成功获取在线成员列表回调
    **/
    virtual void OnGetOnLineUserRoomMembers(const vlive::ClassOnLineUserInfo& users) = 0;
    /***
    *   成功获取踢出成员列表回调
    **/
    virtual void OnGetKickOutRoomMembers(const vlive::ClassOnLineUserInfo& users) = 0;
    /***
    *   成功获取禁言成员列表回调
    **/
    virtual void OnGetSilencedMembers(const vlive::ClassOnLineUserInfo& users) = 0;
    /***
    *   成功获取举手列表回调
    **/
    virtual void OnGetHandUpsMembers(const vlive::ClassOnLineUserInfo& users) = 0;
    /*成功获取可以双推用户列表*/
    virtual void OnGetDoubleVrtcUsers(std::list<vlive::ClassDoubleVrtcUsers> users) = 0;
    /*
    *   举手成功或失败回调
    */
    virtual void OnOpenHandsUp(bool open, int code, std::string respMsg) = 0;
    /*
    *   讲师同意用户上麦
    */
    virtual void OnAuditPublish(std::string uid, int code, std::string respMsg) = 0;
	/**
*  接口说明：奖励学员出错。
*/
	virtual void RewardError(int code, std::string respMsg) = 0;
    /**
    *  接口说明：讲师下麦指定用户，主讲人身份使用。
    **/
    virtual void OnKickUserPublish(const std::string& kick_user_id, int code, std::string respMsg) = 0;
    /**
    *  接口参数：讲师下麦所有用户
    **/
    virtual void OnKickAllUnPublish(int code, std::string respMsg) = 0;
    /**
    *  接口说明：讲师邀请学员、嘉宾上麦，主讲人身份使用。
    **/
    virtual void OnAskforPublish(const std::string& audit_user_id, int code, std::string respMsg) = 0;
    /**
    *  接口说明：学员、嘉宾举手，申请上麦。
    *  回调监听事件：RoomEvent::RoomEvent_Apply_Push
    */
    virtual void OnApplyPublish(int code, std::string respMsg) = 0;
    /**
   *  接口说明：检测用户是否上麦中回调。
   *  参数：user_id 被检测用户id.
   *         code: 错误码 
   *         respMsg: 错误信息
   */
    virtual void OnCheckUserIsPublish(const std::string& user_id, int code, std::string respMsg) = 0;
    
	/**
   *  接口说明：学员、嘉宾举手，取消举手。
   *  回调监听事件：RoomEvent::RoomEvent_Apply_Push
   */
	virtual void CanclePublish(int code, std::string respMsg) = 0;
    /**
    *  接口说明：讲师将学员踢出课堂房间/取消踢出课堂房间
    **/
    virtual void OnKickOutClassRoom(const std::string& kick_user_id, int type, int code, std::string respMsg) = 0;
    /*
    *   接口说明：讲师授权用户画笔
    */
    virtual void OnChangeUserPenAuth(std::string userId, bool auth,int code, std::string respMsg) = 0;
    /*
    *  接口说明：讲师禁言、取消禁言学员、嘉宾
    */
    virtual void OnForbiddenChat(std::string userId, bool forbid, int code, std::string respMsg) = 0;
    /*
    *  接口说明：讲师全体禁言、取消禁言
    */
    virtual void OnForbiddenChatAll(bool forbid, int code, std::string respMsg) = 0;
    /*
    *   接口说明：讲师静音学员或嘉宾
    */
    virtual void OnOperateUserMic(std::string userId, bool close, int code, std::string respMsg) = 0;
    /*
    *   接口说明：讲师讲师静音全部用户
    *       mute: 1 静音， 0 文档
    */
    virtual void OnOperateAllUserMic(int mute , int code, std::string respMsg) = 0;
    /*
    *   接口说明：讲师关闭、打开用户摄像头
    *   参数说明：
    *       userId：用户id
    *       close: true 关闭， false 打开
    */
    virtual void OnOperateUserCamera(std::string userId, bool close, int code, std::string respMsg) = 0;
    /**
    *  接口说明：学员、嘉宾上/下/拒绝上麦状态回执。学员、嘉宾等非主讲人身份使用。
    *  参数说明：
    *       stream_id: 操作的流ID
    *       type:      PushStreamEvent
    *  回调监听事件：RoomEvent::RoomEvent_UserPublishCallback
    */
    virtual void OnUserPublishCallback(vlive::ClassPushStreamEvent type,std::string uid, int code, std::string respMsg) = 0;
    /**
    *  接口说明：学员、嘉宾上/下麦失败通知。学员、嘉宾等非主讲人身份使用。
    *  参数说明：
    *       stream_id: 操作的流ID
    *       type:      PushStreamEvent
    *  回调监听事件：RoomEvent::RoomEvent_UserPublishCallback
    */
    virtual void OnUserPublishFailedCallback(vlive::ClassPushStreamEvent type, int code, std::string respMsg) = 0;
    /*
    *  接口说明：学员或嘉宾上报上麦能力。通过获取麦克风和摄像设备列表，查看是否有设备，如果有设备上报状态，主讲人才能邀请此成员上麦。
    *  参数说明：
    *      bool enablePublish; true 能上麦，false 不能上麦
    *      bool enablePublishDouble;   true 能双推，false 不能双推
    */
    virtual void OnUpdatePublishAbility(int enablePublish, int enablePublishDouble, int code, std::string respMsg) = 0;
    /*
*  接口说明：上报推流设备信息结果请求响应
*  参数说明：
*/
    virtual void OnSendMicsDeviceCallback(int code, std::string respMsg) = 0;
    /*
*  接口说明：邀请学员双推请求响应
*  参数说明：
*/
    virtual void OnInviteDoubleVrtcCallback(int code, std::string respMsg) = 0;
    /*   *  接口说明：邀请学员桌面共享请求响应   *  参数说明：   */    //virtual void OnInviteDesktopVrtcCallback(int code, std::string respMsg) = 0;
    /*
*  接口说明：邀请开始双推请求响应
*  参数说明：
*/
    virtual void OnStartDoubleVrtcCallback(int code, std::string respMsg) = 0;
    /*
*  接口说明：邀请结束双推请求响应
*  参数说明：
*/
    virtual void OnStopDoubleVrtcCallback(int code, std::string respMsg) = 0;
    /*
*  接口说明：学员双推用户请求响应
*  参数说明：
*/
    virtual void OnDoubleVrtcUsersCallback(int code, std::string respMsg) = 0;

    /**
    *  接收说明：讲师切换工具栏标签
    **/
    virtual void OnChangeToToolPage(vlive::ToolPage pageIndex, int code, std::string respMsg) = 0;
    /*
    *  设置混流回调
    *
    */
    virtual void OnSetConfigBroadEvent(int layoutType, int nCode, const std::string &msg) = 0;
    virtual void OnChangeRrlayType() = 0;
    /*
	*   接收奖杯数量增加消息
	*/
	//virtual void OnRecvHandRewardStu(std::string target_id, const int& iFavorNum) = 0;
	virtual void OnRecvHandRewardStu(std::string target_id, std::string nickName, const int& iFavorNum) = 0;

	//主讲人发起倒计时 \主讲人取消倒计时 
	virtual void  OnRecvStartCountdown(const std::string& id, const int&  iplay, const int& iTime) = 0;
	virtual void  OnRecvCancelCountdown(const std::string& id) = 0;

	/*收到开始答题器消息*/
	virtual void  OnRecvAnswerToolMsg(const std::string& msg) = 0;
	virtual void  OnRecvAnswerDetailMsg(int code, const std::string& msg) = 0;
	virtual void  OnRecvStartAnswerMsg(int code, const std::string& msg) = 0;
	virtual void PostDestoryTestPaperDlg() = 0;
   virtual void  OnRecvExamination() = 0;


    //接收白名单登录消息
   virtual void  OnRecvWhiteListUserJoinRoom(const std::string& phone, const std::string& class_token) = 0;

};


#pragma once
#include "LiveEventInterface.h"
#include "vh_room_common.h"
#include "./VhallPaasSDK/callback_monitor.h"

#define CALL_BACK_SUC   200
//���յ��ķ����ڹ㲥����Ϣ֪ͨ
class VHRoomRecvInavMsgCallback {
public:

    /*�������flash��Ϣ*/
    virtual void OnRecvFlahsMsg(std::string flashMsg) = 0;
    /*���ղ岥��Ϣ*/
    virtual void OnRecvMediaMsg(std::string mediaMsg) = 0;
    /*�����ĵ���Ϣ*/
    virtual void OnRecvAnswerMsg(std::string msg) = 0;
    /*���ջ�����Ϣ*/
    virtual void OnRecvSetBrushMsg(std::string data, std::string msg) = 0;
    /*�ӿ��ĵ�ת����Ϣ*/
    virtual void OnRecvDocUploadMsg(std::string msg) = 0;
    /*
    *   �յ�����������Ϣ
    */
    virtual void OnRecvApplyInavPublishMsg(std::wstring& third_party_user_id, const std::wstring& nickname) = 0;
    /*
    *   �յ����������Ϣ ,��ϢΪ�㲥��Ϣ���յ�ͬ�������ִ���������
    */
    virtual void OnRecvAuditInavPublishMsg(const std::wstring& third_party_user_id, AuditPublish) = 0;
    /*
    *   ����������Ϣ  ��ϢΪ�㲥��Ϣ���յ���Ϣ����ʾ������Ϣ
    */
    virtual void OnRecvAskforInavPublishMsg(const std::wstring& join_uid, const std::wstring& target_uid, int status) = 0;// = 0;
    /*
    *   �߳�����Ϣ  ��ϢΪ�㲥��Ϣ���յ���Ϣ��ִ���߳���
    */
    virtual void OnRecvKickInavStreamMsg(const std::wstring& third_party_user_id) = 0;
    /*
    *   �߳���������
    */
    virtual void OnRecvKickOutRoomMsg(const std::string& third_party_user_id, int status) = 0;
    /*
    *   ��/��/�ܾ�������Ϣ ��ϢΪ�㲥��Ϣ
    */
    virtual void OnRecvUserPublishCallback(const std::string& third_party_user_id, const std::string& nickname, vlive::ClassPushStreamEvent event, 
       int role_type,int device_type, int iRewardNum, const int& iNoDoubleVrtc) = 0;
    /*
    *   ��������ر���Ϣ  ���ܵ�����Ϣ�������ڻ����������Ա��ȫ�������˳�����
    */
    virtual void OnRecvInavCloseMsg() = 0;
    /*
    *   �û�������֪ͨ
    *   online�� true �û�����/ false �û�����
    *   user_id�� �û�id
    */
    virtual void OnUserOnLineState(bool online, const std::string user_id,int role_type) = 0;
    /**
    *   ���չ�����Ϣ
    **/
    virtual void OnRecvNoticeMsg(const std::string msg) = 0;
    /**
    *   �յ��Ͽ���Ϣ
    **/
    virtual void OnRecvStartClassMsg(const std::wstring& join_uid, 
       const std::wstring& nickname, 
       const int& iCurLabelNumber,
       const int& iStartPlayType) = 0;

    /**
    *   �յ��¿���Ϣ
    **/
    virtual void OnRecvStopClassMsg(int play_type) = 0;
    /*
    *   ���յ�������Ϣ��ȡ��������Ϣָ���û�
    */
    virtual void OnRecvForbiddenChat(int forbid, std::string user_id, std::string data) = 0;
    /*
    *   ���յ������˽�����Ϣ��������ȡ��������Ϣ
    */
    virtual void OnRecvForbiddenAllChat(int forbid, std::string user_id) = 0;
    /*
    *   ������������
    *    join_uid: �û�id
    *     statue:  0 ������������1 ����������
    */
    virtual void OnRecvUpdatePublishAbility(const std::string& join_uid ,int statue) = 0;
    /*
    *   ���յ�������ʧ����Ϣ
    *   app_type : 1pc 2h5 3 app 4 win
    *   status: 1 ����ʧ�ܣ�0 ����ʧ��
    */
    virtual void OnRecvPublishFailedMsg(const std::string& join_uid, int status, const std::string& nick_name, int role_type, int app_type) = 0;
	/*
	*   ���յ����ֳ�ʱ��Ϣ
	*/
	virtual void  OnRecvHandDelayMsg(const std::string& target_id) = 0;
   //�������湲��
   virtual void OnRecvStopDesktopSharing() = 0;
	/*�յ���ʼǩ����Ϣ*/
	virtual void  OnRecvStartSign(const std::string&strWebinar, const std::string& sign_id, const int& signShowTime) = 0;
	virtual void  OnRecvSignIn(const std::string& strWebinar, const std::string& sign_id, const std::string& signer, const std::string& signerNickname) = 0;
	virtual void OnSignUsersMsg(std::list<std::string> stlistsignUsers, std::list<std::string> stlistNoSignUsers) = 0;
	virtual void  OnRecvSignStop(const std::string& strWebinar, const std::string& sign_id) = 0;
	/*
	*   ����ȡ��������Ϣ
	*/
	virtual void  OnRecvCancelHand(std::string strJoinId, int iRoleType, std::string strNickName, std::string strTime) = 0;
	/*
    *  ���յ�������Ϣ
    *  status ״̬��1������0ȡ��������
    */
    virtual void OnRecvMuteMicMsg(const std::string& join_uid, int status) = 0;
    /**
    *   ���յ�ȫԱ������Ϣ
    *   status ״̬��1������0ȡ��������
    */
    virtual void OnRecvAllMuteMicMsg(int status) = 0;
    /*
    *  ���յ�������Ϣ
    *  status ״̬��1������0ȡ��������
    */
    virtual void OnRecvMuteVideoMsg(const std::string& join_uid, int status) = 0;
    /*   *  ���յ���ʼ˫����Ϣ   */    virtual void OnRecvStartDoubleVrtc(const std::string& join_uid, const std::string& nickName) = 0;    /*   *  ���յ�����˫����Ϣ   */    virtual void OnRecvEndDoubleVrtc(const std::string& target_id, const std::string& nickName) = 0;    /*
    *  ���յ����������ô��洦��
    */
    virtual void OnRecvSetBigScreen(const std::string& target_id) = 0;    /*
   *  ���ձ�����˫����Ϣ
   */
    virtual void OnRecvInviteDoubleVrtc(std::string target_id) = 0;
    /*
   *  ���ձ��������湲����Ϣ
   */
    virtual void OnRecvInviteScreenSharing(std::string target_id) = 0;
    /*
    *  ���յ����ֿ�����Ϣ
    *  status ״̬��1����0�أ�
    */
    virtual void OnRecvOpenHandMsg(int status) = 0;

    /**
    *  �������������û���Ϣ
    */
    virtual void OnRecvDownAllUserPublishMsg() = 0;
};

/*
**===================================
**
**   ����SDK�¼������ص��ӿڣ����нӿڻص�����SDKҵ���̡߳�
**   ����ص�����appҵ���¼��������ڻص��н��С�
**
**===================================
*/
class VHRoomMonitor :public VHRoomRecvInavMsgCallback, public WebRtcSDKEventInterface, public VHRoomDelegate {
public:
    /*
    *   �ӿ�˵������ȡ�γ�����
    */
    virtual void OnGetWebinarDetailInfo(int code, std::string respMsg) = 0;
    /**
    *  �ӿ�˵����������÷��䡣
    **/
    virtual void OnJoinClassRoom(int code, std::string respMsg, const int& iCurLabelNumber = -1) = 0;
    virtual void OnInitQualityNumber(const int& iCurLabelNumber = 1) = 0;
    /*
    *   �ӿ�˵������ʦ������ʼ�ϿΡ�
    *   ����ֵ��ERROR_CODE
    *   �ص�������RoomEvent
    **/
    virtual void OnStartClass(int iPlayType, int code, std::string respMsg) = 0;
    /*
    *   �ӿ�˵��: ��ʦ�����¿�
    *        createDefaultRecord: �Ƿ�����Ĭ�ϻط� 1�� 0��
    *   ����ֵ��ERROR_CODE
    *   �ص�������RoomEvent
    **/
    virtual void OnStopClass(int createDefaultRecord, int iPlayType, int code, std::string respMsg) = 0;
    /*
    *   ��������������HTTPҵ��API���óɹ��¼�
    *   event: RoomEvent / ClassRoomEvent�¼����� 
    *   code: ������
    *   msg: ��Ӧ��Ϣ
    *   userData: ������json����
    */
    virtual void OnCallbackEvent(int event, int code = CALL_BACK_SUC, std::string msg = std::string(), std::string userData = std::string()) = 0;
    /***
    *   �ɹ���ȡ���߳�Ա�б�ص�
    **/
    virtual void OnGetOnLineUserRoomMembers(const vlive::ClassOnLineUserInfo& users) = 0;
    /***
    *   �ɹ���ȡ�߳���Ա�б�ص�
    **/
    virtual void OnGetKickOutRoomMembers(const vlive::ClassOnLineUserInfo& users) = 0;
    /***
    *   �ɹ���ȡ���Գ�Ա�б�ص�
    **/
    virtual void OnGetSilencedMembers(const vlive::ClassOnLineUserInfo& users) = 0;
    /***
    *   �ɹ���ȡ�����б�ص�
    **/
    virtual void OnGetHandUpsMembers(const vlive::ClassOnLineUserInfo& users) = 0;
    /*�ɹ���ȡ����˫���û��б�*/
    virtual void OnGetDoubleVrtcUsers(std::list<vlive::ClassDoubleVrtcUsers> users) = 0;
    /*
    *   ���ֳɹ���ʧ�ܻص�
    */
    virtual void OnOpenHandsUp(bool open, int code, std::string respMsg) = 0;
    /*
    *   ��ʦͬ���û�����
    */
    virtual void OnAuditPublish(std::string uid, int code, std::string respMsg) = 0;
	/**
*  �ӿ�˵��������ѧԱ����
*/
	virtual void RewardError(int code, std::string respMsg) = 0;
    /**
    *  �ӿ�˵������ʦ����ָ���û������������ʹ�á�
    **/
    virtual void OnKickUserPublish(const std::string& kick_user_id, int code, std::string respMsg) = 0;
    /**
    *  �ӿڲ�������ʦ���������û�
    **/
    virtual void OnKickAllUnPublish(int code, std::string respMsg) = 0;
    /**
    *  �ӿ�˵������ʦ����ѧԱ���α��������������ʹ�á�
    **/
    virtual void OnAskforPublish(const std::string& audit_user_id, int code, std::string respMsg) = 0;
    /**
    *  �ӿ�˵����ѧԱ���α����֣���������
    *  �ص������¼���RoomEvent::RoomEvent_Apply_Push
    */
    virtual void OnApplyPublish(int code, std::string respMsg) = 0;
    /**
   *  �ӿ�˵��������û��Ƿ������лص���
   *  ������user_id ������û�id.
   *         code: ������ 
   *         respMsg: ������Ϣ
   */
    virtual void OnCheckUserIsPublish(const std::string& user_id, int code, std::string respMsg) = 0;
    
	/**
   *  �ӿ�˵����ѧԱ���α����֣�ȡ�����֡�
   *  �ص������¼���RoomEvent::RoomEvent_Apply_Push
   */
	virtual void CanclePublish(int code, std::string respMsg) = 0;
    /**
    *  �ӿ�˵������ʦ��ѧԱ�߳����÷���/ȡ���߳����÷���
    **/
    virtual void OnKickOutClassRoom(const std::string& kick_user_id, int type, int code, std::string respMsg) = 0;
    /*
    *   �ӿ�˵������ʦ��Ȩ�û�����
    */
    virtual void OnChangeUserPenAuth(std::string userId, bool auth,int code, std::string respMsg) = 0;
    /*
    *  �ӿ�˵������ʦ���ԡ�ȡ������ѧԱ���α�
    */
    virtual void OnForbiddenChat(std::string userId, bool forbid, int code, std::string respMsg) = 0;
    /*
    *  �ӿ�˵������ʦȫ����ԡ�ȡ������
    */
    virtual void OnForbiddenChatAll(bool forbid, int code, std::string respMsg) = 0;
    /*
    *   �ӿ�˵������ʦ����ѧԱ��α�
    */
    virtual void OnOperateUserMic(std::string userId, bool close, int code, std::string respMsg) = 0;
    /*
    *   �ӿ�˵������ʦ��ʦ����ȫ���û�
    *       mute: 1 ������ 0 �ĵ�
    */
    virtual void OnOperateAllUserMic(int mute , int code, std::string respMsg) = 0;
    /*
    *   �ӿ�˵������ʦ�رա����û�����ͷ
    *   ����˵����
    *       userId���û�id
    *       close: true �رգ� false ��
    */
    virtual void OnOperateUserCamera(std::string userId, bool close, int code, std::string respMsg) = 0;
    /**
    *  �ӿ�˵����ѧԱ���α���/��/�ܾ�����״̬��ִ��ѧԱ���α��ȷ����������ʹ�á�
    *  ����˵����
    *       stream_id: ��������ID
    *       type:      PushStreamEvent
    *  �ص������¼���RoomEvent::RoomEvent_UserPublishCallback
    */
    virtual void OnUserPublishCallback(vlive::ClassPushStreamEvent type,std::string uid, int code, std::string respMsg) = 0;
    /**
    *  �ӿ�˵����ѧԱ���α���/����ʧ��֪ͨ��ѧԱ���α��ȷ����������ʹ�á�
    *  ����˵����
    *       stream_id: ��������ID
    *       type:      PushStreamEvent
    *  �ص������¼���RoomEvent::RoomEvent_UserPublishCallback
    */
    virtual void OnUserPublishFailedCallback(vlive::ClassPushStreamEvent type, int code, std::string respMsg) = 0;
    /*
    *  �ӿ�˵����ѧԱ��α��ϱ�����������ͨ����ȡ��˷�������豸�б��鿴�Ƿ����豸��������豸�ϱ�״̬�������˲�������˳�Ա����
    *  ����˵����
    *      bool enablePublish; true ������false ��������
    *      bool enablePublishDouble;   true ��˫�ƣ�false ����˫��
    */
    virtual void OnUpdatePublishAbility(int enablePublish, int enablePublishDouble, int code, std::string respMsg) = 0;
    /*
*  �ӿ�˵�����ϱ������豸��Ϣ���������Ӧ
*  ����˵����
*/
    virtual void OnSendMicsDeviceCallback(int code, std::string respMsg) = 0;
    /*
*  �ӿ�˵��������ѧԱ˫��������Ӧ
*  ����˵����
*/
    virtual void OnInviteDoubleVrtcCallback(int code, std::string respMsg) = 0;
    /*   *  �ӿ�˵��������ѧԱ���湲��������Ӧ   *  ����˵����   */    //virtual void OnInviteDesktopVrtcCallback(int code, std::string respMsg) = 0;
    /*
*  �ӿ�˵�������뿪ʼ˫��������Ӧ
*  ����˵����
*/
    virtual void OnStartDoubleVrtcCallback(int code, std::string respMsg) = 0;
    /*
*  �ӿ�˵�����������˫��������Ӧ
*  ����˵����
*/
    virtual void OnStopDoubleVrtcCallback(int code, std::string respMsg) = 0;
    /*
*  �ӿ�˵����ѧԱ˫���û�������Ӧ
*  ����˵����
*/
    virtual void OnDoubleVrtcUsersCallback(int code, std::string respMsg) = 0;

    /**
    *  ����˵������ʦ�л���������ǩ
    **/
    virtual void OnChangeToToolPage(vlive::ToolPage pageIndex, int code, std::string respMsg) = 0;
    /*
    *  ���û����ص�
    *
    */
    virtual void OnSetConfigBroadEvent(int layoutType, int nCode, const std::string &msg) = 0;
    virtual void OnChangeRrlayType() = 0;
    /*
	*   ���ս�������������Ϣ
	*/
	//virtual void OnRecvHandRewardStu(std::string target_id, const int& iFavorNum) = 0;
	virtual void OnRecvHandRewardStu(std::string target_id, std::string nickName, const int& iFavorNum) = 0;

	//�����˷��𵹼�ʱ \������ȡ������ʱ 
	virtual void  OnRecvStartCountdown(const std::string& id, const int&  iplay, const int& iTime) = 0;
	virtual void  OnRecvCancelCountdown(const std::string& id) = 0;

	/*�յ���ʼ��������Ϣ*/
	virtual void  OnRecvAnswerToolMsg(const std::string& msg) = 0;
	virtual void  OnRecvAnswerDetailMsg(int code, const std::string& msg) = 0;
	virtual void  OnRecvStartAnswerMsg(int code, const std::string& msg) = 0;
	virtual void PostDestoryTestPaperDlg() = 0;
   virtual void  OnRecvExamination() = 0;


    //���հ�������¼��Ϣ
   virtual void  OnRecvWhiteListUserJoinRoom(const std::string& phone, const std::string& class_token) = 0;

};


#ifndef __VHCLASS_SDK_COMMON_H__
#define __VHCLASS_SDK_COMMON_H__

#include <string>
#include <functional>
#include <iostream>
#include <list>
#include <atomic>
#include "./PaasSDK/paas_common.h"

namespace vlive {

   enum ERROR_CODE {
      SUCCESS_OK = 0,

      ROLE_TYPE_ERROR = 1001,//��ɫ���ʹ���
      ROLE_TYPE_REQUEST_SIZE = 1002,//���������������Χ

      USER_NOT_PUBLISH = 2001,
      USER_IS_ALREADY_PUBLISH = 2002,
   };

   ////��·ֱ������ʱ�ֱ���
   //enum BROAD_CAST_MIXED_DPI {
   //};

   //��·ֱ������֡��
   enum FRAME_FPS {
      //֡�� 15, 20 (Ĭ��), 25 ֡
      FRAME_FPS_15 = 15,
      FRAME_FPS_20 = 20,
      FRAME_FPS_25 = 25,
   };

   //��·ֱ����������
   enum BITRATE {
      //���� 800, 1000 (Ĭ��), 1200
      BITRATE_800 = 800,
      BITRATE_1000 = 1000,
      BITRATE_1200 = 1200,
   };

   enum ClassRoomEvent
   {
      //RoomEvent_OnNetWork_Reconnecting = 0,   //�������ڽ�������
      //RoomEvent_OnNetWork_Reconnect,          //���������ɹ�
      //RoomEvent_GetKickInavList,              //��ȡ���߳������������б�

      RoomEvent_OnNetWork_Close = RoomEvent_Count,              //���类�ر�
      RoomEvent_OnLineUserList,
      RoomEvent_GetSilencedList,              //��ȡ�������б�
      RoomEvent_GetHandUpList,                //��ȡ�����б�
      RoomEvent_Start_MixStream,              //��ʼ��·����
      RoomEvent_Stop_PublishInavAnother,      //ֹͣ��·����
      RoomEvent_SetMainView,                  //���ô���
   };

   ////�û�״̬ 
   //enum MemberStatus {
   //   MemberStatus_Pushing = 1,  // 1 ������
   //   MemberStatus_Watching,     // 2 �ۿ���
   //   MemberStatus_BeInvited,    // 3 ������
   //   MemberStatus_Application,  // 4 ������
   //   MemberStatus_KickOut       // 5 ���߳�
   //};

   //enum AuditPublishType {
   //   AuditPublish_Accept = 1,  //ͬ������
   //   AuditPublish_Refused      //�ܾ�����
   //};

   enum ClassPushStreamEvent {
      //*type	int	��	 0 ���� 1 ����(Ĭ��) 2 �ܾ�����
      PushStreamEvent_UnPublish = 0,
      PushStreamEvent_Publish,
      PushStreamEvent_Refused,
   };

   enum ToolPage {
      ToolPage_Doc = 0,
      ToolPage_Board = 1,
      ToolPage_Answer_Open,
      ToolPage_Answer_Close
   };

   //enum KickStream {
   //   KickStream_KickOutUser = 1,     //1 �߳�����Ĭ�ϣ�
   //   KickStream_CancelKickOutUser   // 2 ȡ���߳���
   //};

   //class VHRoomMember {
   //public:
   //   MemberStatus status;
   //   std::wstring thirdPartyUserId;
   //};

   enum CLASS_STATUS {
      // 1:�Ͽ��� 2 : Ԥ�� 3 : �ط� 4��ת�� 5 : ���¿�
      CLASS_STATUS_IN_CLASS = 1,
      CLASS_STATUS_PRE = 2,
      CLASS_STATUS_PLAY_BACK,
      CLASS_STATUS_RELAY,
      CLASS_STATUS_OVER,
   };

   enum CLASS_LAYOUT {
      CLASS_LAYOUT_ONLY_VIDEO = 1, //���ò��֣��������ȣ�1Ϊ����Ƶ��
      CLASS_LAYOUT_AUDIO_AND_DOC, //��2Ϊ��Ƶ + �ĵ�
      CLASS_LAYOUT_VIDEO_AND_DOC,  //3Ϊ�ĵ� + ��Ƶ
   };

   enum CLASS_COURSE_TYPE {
      //�γ����� 0 : ���� 1 : С��� 2 : ¼���� 3 : ϵ�п�
      CLASS_TYPE_MULTI = 0, //0 : ����
      CLASS_TYPE_SMALL, //1 : С���
      CLASS_TYPE_RECORD,
      CLASS_TYPE_CLASS,
   };
   //��·�������� 1 = >1 / 4��2 = >1 / 6��3 = >1 / 8 ��4 = >1 / 10�� 5 = >1 / 12�� 6 = >1 / 16�� 7 = >1 / 1��
   enum CLASS_JOIN_MEMBER {
      CLASS_JOIN_MEMBER_1V4 = 1,
      CLASS_JOIN_MEMBER_1V6 = 2,
      CLASS_JOIN_MEMBER_1V8 = 3,
      CLASS_JOIN_MEMBER_1V10 = 4,
      CLASS_JOIN_MEMBER_1V12 = 5,
      CLASS_JOIN_MEMBER_1V16 = 6,
      CLASS_JOIN_MEMBER_1V1 = 7,
   };

   enum CLASS_EXIST {
      //��Ƿ����� 1Ϊ������ʾ��2Ϊɾ��
      CLASS_EXIST_OK = 1,
      CLASS_EXIST_DEL,
   };

   class ToolConfig {
   public:
      int doc;    //ֱ��������ĵ��˵��Ƿ���ʾ 1��ʾ 0 ����
      int board;  //ֱ�������װ�˵��Ƿ���ʾ 1��ʾ 0����
      int desktop;//ֱ����������湲��˵��Ƿ���ʾ 1��ʾ 0����
      int answer; //ֱ����������˵��Ƿ���ʾ 1��ʾ 0����
      int memberList;//ֱ�����Ա�б��Ƿ����� 1���� 0 չʾ
      int uploadDoc;//ֱ��������ϴ��ĵ���ť�Ƿ���ʾ 1��ʾ 0����
      int mediaplayer;//ֱ�������岥�˵��Ƿ���ʾ 1��ʾ 0����
      int InteractionTool;//ֱ������໥�����߲˵��Ƿ���ʾ 1��ʾ 0����
   };

   class JoinClassRoomDocInfo {
   public:
      void Reset() {
         srv.clear();            //�ĵ���������ַ
         curr_step.clear();      //�㲥�����ַ
         curr_page.clear();
         curr_file.clear();
         total_page.clear();
         swf_url.clear();
         skin_url.clear();
         whiteboard_url.clear();
         converted_age = 0;
         doc_mode = 0;   //�ĵ�ģʽ 0 �ĵ� 1�װ�
         doc_type = 0;
      }
      std::string srv;            //�ĵ���������ַ
      std::string curr_step;      //�㲥�����ַ
      std::string curr_page;
      std::string curr_file;
      std::string total_page;
      std::string swf_url;
      std::string skin_url;
      std::string whiteboard_url;
      int converted_age;
      int doc_mode;   //�ĵ�ģʽ 0 �ĵ� 1�װ�
      int doc_type;
   };

   class InterstitialInfo {
   public:
      int interstitial; //�Ƿ����ڲ岥 1 �� 0 ��
      std::string progress_rate;              //Ŀǰ���ڲ岥��ԴID���ŵ������� ,���岥δ����ʱΪ���ַ���
      std::string is_pause;                   //Ŀǰ���ڲ岥��ԴID�Ƿ�Ϊ��ͣ״̬1 ��ͣ 0 ����״̬ ,���岥δ����ʱΪ���ַ���
      std::string record_id;                  //Ŀǰ���ڲ岥��ԴID, ���岥δ����ʱΪ���ַ���
   };



   class UserSettings {
   public:
      int video_profile = 360;                  //�����˷ֱ���
      int broadcast_video_profile = 720;  //��·
      int Interaction_profile = 360;        //��������ֱ���
      int DoublePush_profile = 360;      //˫�ƻ���ֱ���
      int Desktop_profile = 1080;            //���湲����ֱ���
      std::string desc;
      int label_number = 1;
      bool mbEnable = true;
   };

   class Definition {
   public:
      Definition() {}
      Definition(const vlive::Definition &right) {
         share_desktop_mixed_flow_layout = right.share_desktop_mixed_flow_layout;
         mixSettings = right.mixSettings;
      }
   public:
      int share_desktop_mixed_flow_layout = 0;//  1 ����ʦ���� (1.8.1�汾����)
      std::list<UserSettings> mixSettings;
   };

   // �����˲���
   enum eMixedLayoutMode
   {
      eMixedLayoutMode_Tiled = 1, //����ƽ��
      eMixedLayoutMode_Grid //��������
   };

   class WebinarInfo {
   public:
      WebinarInfo() {}
      WebinarInfo(const vlive::WebinarInfo &right) {
         course_type = right.course_type;            // �γ����� 0 : ���� 1 : С��� 2 : ¼���� 3 : ϵ�п�
         type.store(right.type);                   // ����״̬ 1:�Ͽ��� 2:Ԥ�� 3:�ط� 4��ת�� 5:���¿�
         real_duration.store(right.real_duration);
         layout = right.layout;                 // ���ò��֣��������ȣ�1Ϊ����Ƶ��2Ϊ��Ƶ+�ĵ���3Ϊ�ĵ�+��Ƶ
         linkmic_apply_type = right.linkmic_apply_type;     // ��·�������� 1 = >1 / 4��2 = >1 / 6��3 = >1 / 8 ��4 = >1 / 10�� 5 = >1 / 12�� 6 = >1 / 16�� 7 = >1 / 1��
         linkmic_can_join_num = right.linkmic_can_join_num;   // ���λụ������
         id = right.id;             // "edu_85864a3a",
         subject = right.subject;        // ���ñ���
         user_id = right.user_id;        // �û�id
         introduction = right.introduction;   //  ���ü��
         mInterstitialInfo = right.mInterstitialInfo;
         answing_question_id = right.answing_question_id;
         videoPushAndMixSetting = right.videoPushAndMixSetting;
         countdown_starttime = right.countdown_starttime;
         countdown_time = right.countdown_time;
         speedmode = right.speedmode;
         desktop_share_layout_mode = right.desktop_share_layout_mode;
         mixedLayout = right.mixedLayout;
         bRrlay = right.bRrlay;
      }
      void Reset() {
         course_type = 0;            // �γ����� 0 : ���� 1 : С��� 2 : ¼���� 3 : ϵ�п�
         type = 0;                   // ����״̬ 1:�Ͽ��� 2:Ԥ�� 3:�ط� 4��ת�� 5:���¿�
         real_duration = 0;          // ����ʱ����λΪ��
         layout = 0;                 // ���ò��֣��������ȣ�1Ϊ����Ƶ��2Ϊ��Ƶ+�ĵ���3Ϊ�ĵ�+��Ƶ
         linkmic_apply_type = 0;     // ��·�������� 1 = >1 / 4��2 = >1 / 6��3 = >1 / 8 ��4 = >1 / 10�� 5 = >1 / 12�� 6 = >1 / 16�� 7 = >1 / 1��
         linkmic_can_join_num = 0;   // ���λụ������
         answing_question_id = 0;// �������ڴ������Ŀʱ ��ֵΪ����0������ ����Ϊ0
         id.clear();             // "edu_85864a3a",
         subject.clear();        // ���ñ���
         user_id.clear();        // �û�id
         introduction.clear();   //  ���ü��
         countdown_starttime = 0;
         countdown_time = 0;
         speedmode = 0;
         desktop_share_layout_mode = 0;
         bRrlay = false;
      }

   public:
      int course_type;            // �γ����� 0 : ���� 1 : С��� 2 : ¼���� 3 : ϵ�п�
      int desktop_share_layout_mode = 0;//// 1 �������л�ģʽ   3.�رջ��л���һ������
      std::atomic_int real_duration;          // ����ʱ����λΪ��
      int layout;                 // ���ò��֣��������ȣ�1Ϊ����Ƶ��2Ϊ��Ƶ+�ĵ���3Ϊ�ĵ�+��Ƶ
      int linkmic_apply_type;     // ��·�������� 1 = >1 / 4��2 = >1 / 6��3 = >1 / 8 ��4 = >1 / 10�� 5 = >1 / 12�� 6 = >1 / 16�� 7 = >1 / 1��
      int linkmic_can_join_num;   // ���λụ������
      int answing_question_id;// �������ڴ������Ŀʱ ��ֵΪ����0������ ����Ϊ0
      std::string id;             // "edu_85864a3a",
      std::string subject;        // ���ñ���
      std::string user_id;        // �û�id
      std::string introduction;   //  ���ü��
      InterstitialInfo mInterstitialInfo;
      Definition videoPushAndMixSetting;
      int countdown_starttime;   //����ʱ starttime������0ʱ ������ʱ�ķ���ʱ��
      int countdown_time;        //timer >0 ʱ������ʱ������
      int speedmode;            //С��μ���ģʽ 1 ����ģʽ 0 �Ǽ���
      //int desktopShareLayout;// 1 �������л�ģʽ   2.�رջ��л���һ������
      eMixedLayoutMode mixedLayout;       //�����˲���
        //UserSettings user_setting;
      bool bRrlay = false; //ת��
      std::atomic_int type;                   // ����״̬ 1:�Ͽ��� 2:Ԥ�� 3:�ط� 4��ת�� 5:���¿�
   };



   class CourseInfo {
   public:
      CLASS_COURSE_TYPE   webinar_course_type;    //�γ����� 0 : ���� 1 : С��� 2 : ¼���� 3 : ϵ�п�
      CLASS_STATUS webinar_status;                //����״̬ 1:�Ͽ��� 2 : Ԥ�� 3 : �ط� 4��ת�� 5 : ���¿�
      CLASS_JOIN_MEMBER join_member_layout;
      int duration;               //�γ�ʱ��
      std::string start_time;             //��ʼʱ��
      std::string end_time;               //����ʱ��
      std::string webinar_id;             // ����ID
      std::string webinar_subject;        // �����
      std::string desc;                   // �γ̼��
      std::string logo;                   // �̻�logo
      std::string cover_img;              // �γ̷���ͼ
      std::string uid;                    // �̻�ID
      std::string detail;                 // �γ�����
	  int viewing_conditions;             // 0 ����ۿ� 1 �����ƹۿ� 2 �������ۿ�
   };

   enum JOIN_ROLE {
      //�λ��ɫ  
      JOIN_ROLE_None = 0,     //��Ч��ɫ
      JOIN_ROLE_Teacher = 1,// 1 : ��ʦ
      JOIN_ROLE_Student,     // 2 ; ѧԱ
      JOIN_ROLE_Assistant,   // 3 : ����
      JOIN_ROLE_Guest,        // 4 : �α�
      JOIN_ROLE_Supervise,    //5 : ���
      JOIN_ROLE_Admin, // 6 : ����Ա
   };

   class UserJoinRoomBaseInfo {
   public:
      void Reset() {
         banchat = false;
         no_mic = false;
         join_id.clear();
         nick_name.clear();
         session_id.clear();
         role_type = JOIN_ROLE_None;
      }

      bool banchat; //�Ƿ񱻽��� true �� false ��
      bool no_mic;  //�Ƿ�����˷� true�� false ��
      std::string join_id;  //�λ�id
      std::string nick_name;  //�λ��ǳ�     
      std::string session_id; //	�Ựid
      JOIN_ROLE role_type;   //�λ��ɫ 1:��ʦ 2:ѧԱ 3:���� 4:�α� 5:��� 6:����Ա
      int iRewardNum = 0;	//��ý���������ʼֵ
   };

   class JoinClassRoomMessage {
   public:
      std::string srv;            //��Ϣ��������ַ
      std::string token;          //������Ϣ����������token
      std::string chat;           //�����������ַ
      std::string publish_url;    //�㲥�����ַ
   };

   class ClassMember {
   public:
      int status;            // ����״̬��Ĭ��Ϊ0�����ߣ�1Ϊ����
      int role_type;
      int no_mic;           //�Ƿ������������  0 �������� 1 ������
      int is_joinmic; //1  ���Ѿ������ (������) 			0  ����δ����ģ����֣�
      std::string user_id;  //�û�ID
      std::string nick_name;
   };

   class ClassDoubleVrtcUsers {
   public:
      //std::string webinar_id;   //����ID
      std::string user_id;         //�û�ID
      std::string nick_name;
      int no_double_vrtc;  //��֧��˫·���� 0:����˫�� 1:������
   };

   class ClassOnLineUserInfo {
   public:
      int total;//������
      int total_page;//��ҳ��
      int curr_page; //�뵱ǰҳ��
      std::list<ClassMember> curr_page_users;
   };


   class PublishUser {
   public:
      std::string join_id;// �λ�ID, 
      std::string nick_name;// ��Ա�ǳ�, 
      int role;// ��Ա��ɫ, 
      int is_quiet;// ��˷��Ƿ񱻽��� 1��0��
      int is_frame;// ����ͷ�Ƿ񱻽�ֹ 1��0��
      int iRewardNum; //��������
   };

   class ViewPageUrls {
   public:
      std::string doc;//�ĵ���ҳurl ��ַ
      std::string board;// �װ���ҳurl ��ַ
      std::string answer;// ����ҳ���ַ
      std::string mediaplayer;// �岥ҳ���ַ
      std::string chat;// ����ҳ���ַ
      std::string record;// �ط�ҳ���ַ
      std::string mixvideo;//ֱ����
      std::string questionnaire;//ֱ����
   };

   struct  StruSign
   {
      int irest_time = 0;// ʣ��ʱ��
      std::string strSignStatus;
      std::string strSignId;
      int iSignUserCount = 0;// ǩ������
   };

   class ExamInitInfo {
   public:
      std::string strExaming;     //yes ���ڿ���  no δ�ڿ���
      //int naire_id; //�Ծ�id int 
      int naire_status;//�Ծ�����״̬0׼������, 1 ����������,2���ͽ���,3������
      int user_status;//�û����״̬ 0 δ����,1��������,2 ��������,
      //int push_user;//���Է�����id
      //std::string push_time; //��������ʱ�� string ���ͻ���null ���п�����ϢʱΪstring ����Ϊnull
   };

   /*ֱ����������*/
   enum eClassPlayType {
      eClassPlayType_Flash = 0, //flash �û�
      eClassPlayType_H5             //h5 �û�
   };

   class ClassRoomBaseInfo {
   public:
      ClassRoomBaseInfo() {}
      ClassRoomBaseInfo(const vlive::ClassRoomBaseInfo &right)
         :mWebinarInfo(right.mWebinarInfo)
         , mUserInfo(right.mUserInfo)
         , mDocInfo(right.mDocInfo)
         , mToolConfig(right.mToolConfig)
         , urls(right.urls) {
         banchatall.store(right.banchatall);
         mutingAll.store(right.mutingAll);
         openhand.store(right.openhand);
         brush.store(right.brush);
         bigScreen.store(right.bigScreen);
         current_speaker = right.current_speaker;
         mic_token = right.mic_token;
         record_view_url = right.record_view_url;
         ready_list = right.ready_list;
         mutingTeacher = right.mutingTeacher;
         mutingStudent = right.mutingStudent;
         closeScreenTeacher = right.closeScreenTeacher;
         closeScreenStudent = right.closeScreenStudent;
         mStruSign = right.mStruSign;
         mExamInitInfo.strExaming = right.mExamInitInfo.strExaming;
         mExamInitInfo.user_status = right.mExamInitInfo.user_status;
         mExamInitInfo.naire_status = right.mExamInitInfo.naire_status;   
         strVid = right.strVid;
         strVfid = right.strVfid;
         play_type = right.play_type;
      };

      void Reset() {
         mWebinarInfo.Reset();
         mUserInfo.Reset();
         banchatall = 0;
         mutingAll = 0;
         openhand = 0;
         brush = 0;
         bigScreen = 0;
         current_speaker.clear();
         mic_token.clear();
         record_view_url.clear();
         ready_list.clear();
         mutingTeacher.clear();
         mutingStudent.clear();
         closeScreenTeacher.clear();
         closeScreenStudent.clear();
         strVid = "";
         strVfid = "";
         play_type = eClassPlayType_Flash;
      }

      ExamInitInfo mExamInitInfo;
      WebinarInfo mWebinarInfo;           //�����λ������Ϣ
      UserJoinRoomBaseInfo mUserInfo;     //������Ϣ
      JoinClassRoomDocInfo mDocInfo;       //�ĵ��װ���Ϣ
      ToolConfig  mToolConfig;            //ֱ���乤����ʾ����
      StruSign mStruSign;
      ViewPageUrls urls;
      eClassPlayType play_type = eClassPlayType_Flash;
      std::atomic_int banchatall;         // �Ƿ�ȫ����� 1�� 0��
      std::atomic_int mutingAll;          // �Ƿ�ȫ�徲�� 1�� 0��
      std::atomic_int openhand;           // �Ƿ������� 1�� 0��
      std::atomic_int brush;              // �л���Ȩ�޵�ѧ���Ĳλ�id Ĭ��Ϊ0
      std::atomic_int bigScreen = 0;          // �����δ������id  
      std::string current_speaker;    //��ǰ�����˲λ�ID

      std::string mic_token;
      std::string record_view_url;
      std::list<PublishUser> ready_list;
      std::list<std::string> mutingTeacher;
      std::list<std::string> mutingStudent;
      std::list<std::string> closeScreenTeacher;
      std::list<std::string> closeScreenStudent;
      std::string strVid = "";
      std::string strVfid = "";
      //bool bRELAY = false;
   };
}


#endif
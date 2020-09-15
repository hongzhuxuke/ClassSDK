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

      ROLE_TYPE_ERROR = 1001,//角色类型错误
      ROLE_TYPE_REQUEST_SIZE = 1002,//请求的数量超出范围

      USER_NOT_PUBLISH = 2001,
      USER_IS_ALREADY_PUBLISH = 2002,
   };

   ////旁路直播混流时分辨率
   //enum BROAD_CAST_MIXED_DPI {
   //};

   //旁路直播混流帧率
   enum FRAME_FPS {
      //帧率 15, 20 (默认), 25 帧
      FRAME_FPS_15 = 15,
      FRAME_FPS_20 = 20,
      FRAME_FPS_25 = 25,
   };

   //旁路直播混流码率
   enum BITRATE {
      //码率 800, 1000 (默认), 1200
      BITRATE_800 = 800,
      BITRATE_1000 = 1000,
      BITRATE_1200 = 1200,
   };

   enum ClassRoomEvent
   {
      //RoomEvent_OnNetWork_Reconnecting = 0,   //网络正在进行重连
      //RoomEvent_OnNetWork_Reconnect,          //网络重连成功
      //RoomEvent_GetKickInavList,              //获取被踢出互动房间人列表

      RoomEvent_OnNetWork_Close = RoomEvent_Count,              //网络被关闭
      RoomEvent_OnLineUserList,
      RoomEvent_GetSilencedList,              //获取被禁言列表
      RoomEvent_GetHandUpList,                //获取举手列表
      RoomEvent_Start_MixStream,              //开始旁路布局
      RoomEvent_Stop_PublishInavAnother,      //停止旁路布局
      RoomEvent_SetMainView,                  //设置大画面
   };

   ////用户状态 
   //enum MemberStatus {
   //   MemberStatus_Pushing = 1,  // 1 推流中
   //   MemberStatus_Watching,     // 2 观看中
   //   MemberStatus_BeInvited,    // 3 受邀中
   //   MemberStatus_Application,  // 4 申请中
   //   MemberStatus_KickOut       // 5 被踢出
   //};

   //enum AuditPublishType {
   //   AuditPublish_Accept = 1,  //同意上麦
   //   AuditPublish_Refused      //拒绝上麦
   //};

   enum ClassPushStreamEvent {
      //*type	int	否	 0 下麦 1 上麦(默认) 2 拒绝上麦
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
   //   KickStream_KickOutUser = 1,     //1 踢出流（默认）
   //   KickStream_CancelKickOutUser   // 2 取消踢出流
   //};

   //class VHRoomMember {
   //public:
   //   MemberStatus status;
   //   std::wstring thirdPartyUserId;
   //};

   enum CLASS_STATUS {
      // 1:上课中 2 : 预告 3 : 回放 4：转播 5 : 已下课
      CLASS_STATUS_IN_CLASS = 1,
      CLASS_STATUS_PRE = 2,
      CLASS_STATUS_PLAY_BACK,
      CLASS_STATUS_RELAY,
      CLASS_STATUS_OVER,
   };

   enum CLASS_LAYOUT {
      CLASS_LAYOUT_ONLY_VIDEO = 1, //课堂布局，三分屏等，1为单视频，
      CLASS_LAYOUT_AUDIO_AND_DOC, //，2为音频 + 文档
      CLASS_LAYOUT_VIDEO_AND_DOC,  //3为文档 + 视频
   };

   enum CLASS_COURSE_TYPE {
      //课程类型 0 : 大班课 1 : 小班课 2 : 录播课 3 : 系列课
      CLASS_TYPE_MULTI = 0, //0 : 大班课
      CLASS_TYPE_SMALL, //1 : 小班课
      CLASS_TYPE_RECORD,
      CLASS_TYPE_CLASS,
   };
   //多路连麦类型 1 = >1 / 4；2 = >1 / 6；3 = >1 / 8 ；4 = >1 / 10； 5 = >1 / 12； 6 = >1 / 16； 7 = >1 / 1；
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
      //活动是否正常 1为正常显示，2为删除
      CLASS_EXIST_OK = 1,
      CLASS_EXIST_DEL,
   };

   class ToolConfig {
   public:
      int doc;    //直播间左侧文档菜单是否显示 1显示 0 隐藏
      int board;  //直播间左侧白板菜单是否显示 1显示 0隐藏
      int desktop;//直播间左侧桌面共享菜单是否显示 1显示 0隐藏
      int answer; //直播间左侧答题菜单是否显示 1显示 0隐藏
      int memberList;//直播间成员列表是否隐藏 1隐藏 0 展示
      int uploadDoc;//直播间课中上传文档按钮是否显示 1显示 0隐藏
      int mediaplayer;//直播间左侧插播菜单是否显示 1显示 0隐藏
      int InteractionTool;//直播间左侧互动工具菜单是否显示 1显示 0隐藏
   };

   class JoinClassRoomDocInfo {
   public:
      void Reset() {
         srv.clear();            //文档服务器地址
         curr_step.clear();      //广播聊天地址
         curr_page.clear();
         curr_file.clear();
         total_page.clear();
         swf_url.clear();
         skin_url.clear();
         whiteboard_url.clear();
         converted_age = 0;
         doc_mode = 0;   //文档模式 0 文档 1白板
         doc_type = 0;
      }
      std::string srv;            //文档服务器地址
      std::string curr_step;      //广播聊天地址
      std::string curr_page;
      std::string curr_file;
      std::string total_page;
      std::string swf_url;
      std::string skin_url;
      std::string whiteboard_url;
      int converted_age;
      int doc_mode;   //文档模式 0 文档 1白板
      int doc_type;
   };

   class InterstitialInfo {
   public:
      int interstitial; //是否正在插播 1 是 0 否
      std::string progress_rate;              //目前正在插播资源ID播放到多少秒 ,当插播未开启时为空字符串
      std::string is_pause;                   //目前正在插播资源ID是否为暂停状态1 暂停 0 播放状态 ,当插播未开启时为空字符串
      std::string record_id;                  //目前正在插播资源ID, 当插播未开启时为空字符串
   };



   class UserSettings {
   public:
      int video_profile = 360;                  //主讲人分辨率
      int broadcast_video_profile = 720;  //旁路
      int Interaction_profile = 360;        //互动画面分辨率
      int DoublePush_profile = 360;      //双推画面分辨率
      int Desktop_profile = 1080;            //桌面共享画面分辨率
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
      int share_desktop_mixed_flow_layout = 0;//  1 带讲师画面 (1.8.1版本增加)
      std::list<UserSettings> mixSettings;
   };

   // 混流端布局
   enum eMixedLayoutMode
   {
      eMixedLayoutMode_Tiled = 1, //主次平铺
      eMixedLayoutMode_Grid //均匀排列
   };

   class WebinarInfo {
   public:
      WebinarInfo() {}
      WebinarInfo(const vlive::WebinarInfo &right) {
         course_type = right.course_type;            // 课程类型 0 : 大班课 1 : 小班课 2 : 录播课 3 : 系列课
         type.store(right.type);                   // 课堂状态 1:上课中 2:预告 3:回放 4：转播 5:已下课
         real_duration.store(right.real_duration);
         layout = right.layout;                 // 课堂布局，三分屏等，1为单视频，2为音频+文档，3为文档+视频
         linkmic_apply_type = right.linkmic_apply_type;     // 多路连麦类型 1 = >1 / 4；2 = >1 / 6；3 = >1 / 8 ；4 = >1 / 10； 5 = >1 / 12； 6 = >1 / 16； 7 = >1 / 1；
         linkmic_can_join_num = right.linkmic_can_join_num;   // 最大参会互动人数
         id = right.id;             // "edu_85864a3a",
         subject = right.subject;        // 课堂标题
         user_id = right.user_id;        // 用户id
         introduction = right.introduction;   //  课堂简介
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
         course_type = 0;            // 课程类型 0 : 大班课 1 : 小班课 2 : 录播课 3 : 系列课
         type = 0;                   // 课堂状态 1:上课中 2:预告 3:回放 4：转播 5:已下课
         real_duration = 0;          // 课堂时长单位为秒
         layout = 0;                 // 课堂布局，三分屏等，1为单视频，2为音频+文档，3为文档+视频
         linkmic_apply_type = 0;     // 多路连麦类型 1 = >1 / 4；2 = >1 / 6；3 = >1 / 8 ；4 = >1 / 10； 5 = >1 / 12； 6 = >1 / 16； 7 = >1 / 1；
         linkmic_can_join_num = 0;   // 最大参会互动人数
         answing_question_id = 0;// 当有正在答题的题目时 此值为大于0的整数 否则为0
         id.clear();             // "edu_85864a3a",
         subject.clear();        // 课堂标题
         user_id.clear();        // 用户id
         introduction.clear();   //  课堂简介
         countdown_starttime = 0;
         countdown_time = 0;
         speedmode = 0;
         desktop_share_layout_mode = 0;
         bRrlay = false;
      }

   public:
      int course_type;            // 课程类型 0 : 大班课 1 : 小班课 2 : 录播课 3 : 系列课
      int desktop_share_layout_mode = 0;//// 1 开启画中画模式   3.关闭画中画，一屏铺满
      std::atomic_int real_duration;          // 课堂时长单位为秒
      int layout;                 // 课堂布局，三分屏等，1为单视频，2为音频+文档，3为文档+视频
      int linkmic_apply_type;     // 多路连麦类型 1 = >1 / 4；2 = >1 / 6；3 = >1 / 8 ；4 = >1 / 10； 5 = >1 / 12； 6 = >1 / 16； 7 = >1 / 1；
      int linkmic_can_join_num;   // 最大参会互动人数
      int answing_question_id;// 当有正在答题的题目时 此值为大于0的整数 否则为0
      std::string id;             // "edu_85864a3a",
      std::string subject;        // 课堂标题
      std::string user_id;        // 用户id
      std::string introduction;   //  课堂简介
      InterstitialInfo mInterstitialInfo;
      Definition videoPushAndMixSetting;
      int countdown_starttime;   //倒计时 starttime当大于0时 代表倒计时的发起时间
      int countdown_time;        //timer >0 时代表倒计时的秒数
      int speedmode;            //小班课极速模式 1 极速模式 0 非极速
      //int desktopShareLayout;// 1 开启画中画模式   2.关闭画中画，一屏铺满
      eMixedLayoutMode mixedLayout;       //混流端布局
        //UserSettings user_setting;
      bool bRrlay = false; //转播
      std::atomic_int type;                   // 课堂状态 1:上课中 2:预告 3:回放 4：转播 5:已下课
   };



   class CourseInfo {
   public:
      CLASS_COURSE_TYPE   webinar_course_type;    //课程类型 0 : 大班课 1 : 小班课 2 : 录播课 3 : 系列课
      CLASS_STATUS webinar_status;                //课堂状态 1:上课中 2 : 预告 3 : 回放 4：转播 5 : 已下课
      CLASS_JOIN_MEMBER join_member_layout;
      int duration;               //课程时长
      std::string start_time;             //开始时间
      std::string end_time;               //结束时间
      std::string webinar_id;             // 课堂ID
      std::string webinar_subject;        // 活动标题
      std::string desc;                   // 课程简介
      std::string logo;                   // 商户logo
      std::string cover_img;              // 课程封面图
      std::string uid;                    // 商户ID
      std::string detail;                 // 课程详情
	  int viewing_conditions;             // 0 口令观看 1 无限制观看 2 白名单观看
   };

   enum JOIN_ROLE {
      //参会角色  
      JOIN_ROLE_None = 0,     //无效角色
      JOIN_ROLE_Teacher = 1,// 1 : 老师
      JOIN_ROLE_Student,     // 2 ; 学员
      JOIN_ROLE_Assistant,   // 3 : 助教
      JOIN_ROLE_Guest,        // 4 : 嘉宾
      JOIN_ROLE_Supervise,    //5 : 监课
      JOIN_ROLE_Admin, // 6 : 管理员
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

      bool banchat; //是否被禁言 true 是 false 否
      bool no_mic;  //是否无麦克风 true是 false 否
      std::string join_id;  //参会id
      std::string nick_name;  //参会昵称     
      std::string session_id; //	会话id
      JOIN_ROLE role_type;   //参会角色 1:老师 2:学员 3:助教 4:嘉宾 5:监课 6:管理员
      int iRewardNum = 0;	//获得奖杯数量初始值
   };

   class JoinClassRoomMessage {
   public:
      std::string srv;            //消息服务器地址
      std::string token;          //连接消息服务器所需token
      std::string chat;           //聊天服务器地址
      std::string publish_url;    //广播聊天地址
   };

   class ClassMember {
   public:
      int status;            // 在线状态，默认为0不在线，1为在线
      int role_type;
      int no_mic;           //是否具有上麦能力  0 有能力。 1 无能力
      int is_joinmic; //1  是已经上麦的 (连麦中) 			0  举手未上麦的（举手）
      std::string user_id;  //用户ID
      std::string nick_name;
   };

   class ClassDoubleVrtcUsers {
   public:
      //std::string webinar_id;   //课堂ID
      std::string user_id;         //用户ID
      std::string nick_name;
      int no_double_vrtc;  //不支持双路连麦 0:可以双推 1:不可以
   };

   class ClassOnLineUserInfo {
   public:
      int total;//总人数
      int total_page;//总页数
      int curr_page; //请当前页数
      std::list<ClassMember> curr_page_users;
   };


   class PublishUser {
   public:
      std::string join_id;// 参会ID, 
      std::string nick_name;// 成员昵称, 
      int role;// 成员角色, 
      int is_quiet;// 麦克风是否被禁音 1是0否，
      int is_frame;// 摄像头是否被禁止 1是0否
      int iRewardNum; //奖杯数量
   };

   class ViewPageUrls {
   public:
      std::string doc;//文档网页url 地址
      std::string board;// 白板网页url 地址
      std::string answer;// 答题页面地址
      std::string mediaplayer;// 插播页面地址
      std::string chat;// 聊天页面地址
      std::string record;// 回放页面地址
      std::string mixvideo;//直播流
      std::string questionnaire;//直播流
   };

   struct  StruSign
   {
      int irest_time = 0;// 剩余时间
      std::string strSignStatus;
      std::string strSignId;
      int iSignUserCount = 0;// 签到人数
   };

   class ExamInitInfo {
   public:
      std::string strExaming;     //yes 正在考试  no 未在考试
      //int naire_id; //试卷id int 
      int naire_status;//试卷推送状态0准备推送, 1 正在推送中,2推送结束,3公布答案
      int user_status;//用户答卷状态 0 未答题,1主动交接,2 被动交卷,
      //int push_user;//考试发起人id
      //std::string push_time; //考试推送时间 string 类型或者null 当有考试信息时为string 否则为null
   };

   /*直播播放类型*/
   enum eClassPlayType {
      eClassPlayType_Flash = 0, //flash 用户
      eClassPlayType_H5             //h5 用户
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
      WebinarInfo mWebinarInfo;           //互动参会基本信息
      UserJoinRoomBaseInfo mUserInfo;     //个人信息
      JoinClassRoomDocInfo mDocInfo;       //文档白板信息
      ToolConfig  mToolConfig;            //直播间工具显示隐藏
      StruSign mStruSign;
      ViewPageUrls urls;
      eClassPlayType play_type = eClassPlayType_Flash;
      std::atomic_int banchatall;         // 是否全体禁言 1是 0否
      std::atomic_int mutingAll;          // 是否全体静音 1是 0否
      std::atomic_int openhand;           // 是否开启举手 1是 0否
      std::atomic_int brush;              // 有画笔权限的学生的参会id 默认为0
      std::atomic_int bigScreen = 0;          // 公开课大画面大画面id  
      std::string current_speaker;    //当前主讲人参会ID

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
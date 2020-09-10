#include "URLGenerator.h"
#include "SystemUtility.h"
#include "Md5.h"


URLGenerator::URLGenerator()
{
    mDomain = "http://api-class.e.vhall.com";
}


URLGenerator::~URLGenerator()
{
}

void URLGenerator::SetDoMain(const std::string& domain) {
    mDomain = domain;
}

void URLGenerator::SetPublicParam(std::string app_key, std::string appSecret, std::string devId) {
    mAppKey = app_key;
	mDevId = devId;
    mAppSecret = appSecret;
}

std::string URLGenerator::GetWebinarDetailInfoUrl(std::string webinarId) {
    std::string baseInfo = mDomain + std::string("/pcv1/webinar/course-info?");

    long long unixTimestamp = SystemUtility::GetPresentUnixTime();
    SystemUtility::InsertUrlParam(baseInfo, "app_key", mAppKey);
    SystemUtility::InsertUrlParam(baseInfo, "&signed_at", SystemUtility::lltoString(unixTimestamp));
    SystemUtility::InsertUrlParam(baseInfo, "&webinar_id", webinarId);
    SystemUtility::InsertUrlParam(baseInfo, "&device_no", mDevId);
    //SystemUtility::InsertUrlParam(baseInfo, "&new_version", "1");
    SystemUtility::InsertUrlParam(baseInfo, "&live_version_h5", "1");


    std::string sign;
    sign += mAppSecret;
    std::vector<std::string> param;
    SystemUtility::InsertParam(param, "app_key", mAppKey);
    SystemUtility::InsertParam(param, "signed_at", SystemUtility::lltoString(unixTimestamp));
    SystemUtility::InsertParam(param, "webinar_id", webinarId);
    SystemUtility::InsertParam(param, "device_no", mDevId);
    //SystemUtility::InsertParam(param, "new_version", "1");
    SystemUtility::InsertParam(param, "live_version_h5", "1");

    std::string sortParam = SystemUtility::SortSignParam(param);
    sign += sortParam;
    sign += mAppSecret;

    CMD5 iMD5;
    iMD5.GenerateMD5((unsigned char*)sign.c_str(), sign.length());
    std::string result = iMD5.ToString();
    SystemUtility::InsertUrlParam(baseInfo, "&sign", result);
    return baseInfo;
}



std::string URLGenerator::GetPaasRoomInfoUrl(std::string webinarId, std::string join_id, std::string token)
{
   std::string baseInfo = mDomain + std::string("/common/room/pass-room-init-info?");

   long long unixTimestamp = SystemUtility::GetPresentUnixTime();
   SystemUtility::InsertUrlParam(baseInfo, "app_key", mAppKey);
   SystemUtility::InsertUrlParam(baseInfo, "&signed_at", SystemUtility::lltoString(unixTimestamp));
   SystemUtility::InsertUrlParam(baseInfo, "&webinar_id", webinarId);
   SystemUtility::InsertUrlParam(baseInfo, "&device_no", mDevId);

   SystemUtility::InsertUrlParam(baseInfo, "&join_id", join_id);
   SystemUtility::InsertUrlParam(baseInfo, "&token", token);
   
   std::string sign;
   sign += mAppSecret;
   std::vector<std::string> param;
   SystemUtility::InsertParam(param, "app_key", mAppKey);
   SystemUtility::InsertParam(param, "signed_at", SystemUtility::lltoString(unixTimestamp));
   SystemUtility::InsertParam(param, "webinar_id", webinarId);
   SystemUtility::InsertParam(param, "device_no", mDevId);
   SystemUtility::InsertParam(param, "join_id", join_id);
   SystemUtility::InsertParam(param, "token", token);
   std::string sortParam = SystemUtility::SortSignParam(param);
   sign += sortParam;
   sign += mAppSecret;

   CMD5 iMD5;
   iMD5.GenerateMD5((unsigned char*)sign.c_str(), sign.length());
   std::string result = iMD5.ToString();
   SystemUtility::InsertUrlParam(baseInfo, "&sign", result);
   return baseInfo;
}

std::string URLGenerator::StartCoutDownToolUrl(std::string webinarId, std::string  join_id, std::string token, const int& time, const int& iIsPlayEndSounds)
{
	std::string baseInfo = mDomain + std::string("/pcv1/countdowntool/start?");
	char courseType[64] = { 0 };
	itoa(time, courseType, 10);

	char szplayEndSound[64] = { 0 };
	itoa(iIsPlayEndSounds, szplayEndSound, 10);

	
	long long unixTimestamp = SystemUtility::GetPresentUnixTime();
	SystemUtility::InsertUrlParam(baseInfo, "app_key", mAppKey);
	SystemUtility::InsertUrlParam(baseInfo, "&signed_at", SystemUtility::lltoString(unixTimestamp));
	SystemUtility::InsertUrlParam(baseInfo, "&webinar_id", webinarId);
	SystemUtility::InsertUrlParam(baseInfo, "&device_no", mDevId);
	
	SystemUtility::InsertUrlParam(baseInfo, "&join_id", join_id);
	SystemUtility::InsertUrlParam(baseInfo, "&class_token", token);
	SystemUtility::InsertUrlParam(baseInfo, "&time", courseType);
	SystemUtility::InsertUrlParam(baseInfo, "&playEndSound", szplayEndSound);
	//SystemUtility::InsertUrlParam(baseInfo, "&debug_host", "172.16.11.39:8088");// //class.vhall.com   //t-class.e.vhall.com  //172.16.11.39:8088

	std::string sign;
	sign += mAppSecret;
	std::vector<std::string> param;
	SystemUtility::InsertParam(param, "app_key", mAppKey);
	SystemUtility::InsertParam(param, "signed_at", SystemUtility::lltoString(unixTimestamp));
	SystemUtility::InsertParam(param, "webinar_id", webinarId);
	SystemUtility::InsertParam(param, "device_no", mDevId);
	SystemUtility::InsertParam(param, "join_id", join_id);
	SystemUtility::InsertParam(param, "class_token", token);
	SystemUtility::InsertParam(param, "time", courseType);
	SystemUtility::InsertParam(param, "playEndSound", szplayEndSound);
	std::string sortParam = SystemUtility::SortSignParam(param);
	sign += sortParam;
	sign += mAppSecret;

	CMD5 iMD5;
	iMD5.GenerateMD5((unsigned char*)sign.c_str(), sign.length());
	std::string result = iMD5.ToString();
	SystemUtility::InsertUrlParam(baseInfo, "&sign", result);
	return baseInfo;

}

std::string URLGenerator::CancleCoutDownToolUrl(std::string webinarId, std::string  join_id, std::string token)
{
	std::string baseInfo = mDomain + std::string("/pcv1/countdowntool/cancel?");

	long long unixTimestamp = SystemUtility::GetPresentUnixTime();
	SystemUtility::InsertUrlParam(baseInfo, "app_key", mAppKey);
	SystemUtility::InsertUrlParam(baseInfo, "&signed_at", SystemUtility::lltoString(unixTimestamp));
	SystemUtility::InsertUrlParam(baseInfo, "&webinar_id", webinarId);
	SystemUtility::InsertUrlParam(baseInfo, "&device_no", mDevId);

	SystemUtility::InsertUrlParam(baseInfo, "&join_id", join_id);
	SystemUtility::InsertUrlParam(baseInfo, "&class_token", token);
	//SystemUtility::InsertUrlParam(baseInfo, "&debug_host", "172.16.11.39:8088");// //class.vhall.com   //t-class.e.vhall.com  //172.16.11.39:8088

	std::string sign;
	sign += mAppSecret;
	std::vector<std::string> param;
	SystemUtility::InsertParam(param, "app_key", mAppKey);
	SystemUtility::InsertParam(param, "signed_at", SystemUtility::lltoString(unixTimestamp));
	SystemUtility::InsertParam(param, "webinar_id", webinarId);
	SystemUtility::InsertParam(param, "device_no", mDevId);
	SystemUtility::InsertParam(param, "join_id", join_id);
	SystemUtility::InsertParam(param, "class_token", token);
	std::string sortParam = SystemUtility::SortSignParam(param);
	sign += sortParam;
	sign += mAppSecret;

	CMD5 iMD5;
	iMD5.GenerateMD5((unsigned char*)sign.c_str(), sign.length());
	std::string result = iMD5.ToString();
	SystemUtility::InsertUrlParam(baseInfo, "&sign", result);
	return baseInfo;
}

std::string URLGenerator::GetAddSign(std::string webinarId, std::string  join_id, std::string token)
{
	std::string baseInfo = mDomain + std::string("/edu/room/add-sign?");

	long long unixTimestamp = SystemUtility::GetPresentUnixTime();
	SystemUtility::InsertUrlParam(baseInfo, "app_key", mAppKey);
	SystemUtility::InsertUrlParam(baseInfo, "&signed_at", SystemUtility::lltoString(unixTimestamp));
	SystemUtility::InsertUrlParam(baseInfo, "&webinar_id", webinarId);
	SystemUtility::InsertUrlParam(baseInfo, "&device_no", mDevId);

	SystemUtility::InsertUrlParam(baseInfo, "&join_id", join_id);
	SystemUtility::InsertUrlParam(baseInfo, "&token", token);

	//SystemUtility::InsertUrlParam(baseInfo, "&debug_host", "172.16.11.39:8088");// //class.vhall.com   //t-class.e.vhall.com  //172.16.11.39:8088

	std::string sign;
	sign += mAppSecret;
	std::vector<std::string> param;
	SystemUtility::InsertParam(param, "app_key", mAppKey);
	SystemUtility::InsertParam(param, "signed_at", SystemUtility::lltoString(unixTimestamp));
	SystemUtility::InsertParam(param, "webinar_id", webinarId);
	SystemUtility::InsertParam(param, "device_no", mDevId);
	SystemUtility::InsertParam(param, "join_id", join_id);
	SystemUtility::InsertParam(param, "token", token);
	
	std::string sortParam = SystemUtility::SortSignParam(param);
	sign += sortParam;
	sign += mAppSecret;

	CMD5 iMD5;
	iMD5.GenerateMD5((unsigned char*)sign.c_str(), sign.length());
	std::string result = iMD5.ToString();
	SystemUtility::InsertUrlParam(baseInfo, "&sign", result);
	return baseInfo;
}

std::string URLGenerator::GetSignIn(std::string webinarId, std::string  join_id, std::string token, const std::string& AddSignId)
{
	std::string baseInfo = mDomain + std::string("/edu/room/sign-in?");

	long long unixTimestamp = SystemUtility::GetPresentUnixTime();
	SystemUtility::InsertUrlParam(baseInfo, "app_key", mAppKey);
	SystemUtility::InsertUrlParam(baseInfo, "&signed_at", SystemUtility::lltoString(unixTimestamp));
	SystemUtility::InsertUrlParam(baseInfo, "&webinar_id", webinarId);
	SystemUtility::InsertUrlParam(baseInfo, "&device_no", mDevId);
	SystemUtility::InsertUrlParam(baseInfo, "&join_id", join_id);
	SystemUtility::InsertUrlParam(baseInfo, "&token", token);
	SystemUtility::InsertUrlParam(baseInfo, "&sign_id", AddSignId);

	std::string sign;
	sign += mAppSecret;
	std::vector<std::string> param;
	SystemUtility::InsertParam(param, "app_key", mAppKey);
	SystemUtility::InsertParam(param, "signed_at", SystemUtility::lltoString(unixTimestamp));
	SystemUtility::InsertParam(param, "webinar_id", webinarId);
	SystemUtility::InsertParam(param, "device_no", mDevId);
	SystemUtility::InsertParam(param, "join_id", join_id);
	SystemUtility::InsertParam(param, "token", token);
	SystemUtility::InsertParam(param, "sign_id", AddSignId);
	std::string sortParam = SystemUtility::SortSignParam(param);
	sign += sortParam;
	sign += mAppSecret;

	CMD5 iMD5;
	iMD5.GenerateMD5((unsigned char*)sign.c_str(), sign.length());
	std::string result = iMD5.ToString();
	SystemUtility::InsertUrlParam(baseInfo, "&sign", result);
	return baseInfo;
}

std::string URLGenerator::GetSignRecord(std::string webinarId, std::string  join_id, std::string token, const std::string& AddSignId)
{
	std::string baseInfo = mDomain + std::string("/edu/room/get-sign-record?");

	long long unixTimestamp = SystemUtility::GetPresentUnixTime();
	SystemUtility::InsertUrlParam(baseInfo, "app_key", mAppKey);
	SystemUtility::InsertUrlParam(baseInfo, "&signed_at", SystemUtility::lltoString(unixTimestamp));
	SystemUtility::InsertUrlParam(baseInfo, "&webinar_id", webinarId);
	SystemUtility::InsertUrlParam(baseInfo, "&device_no", mDevId);

	SystemUtility::InsertUrlParam(baseInfo, "&join_id", join_id);
	SystemUtility::InsertUrlParam(baseInfo, "&token", token);
	SystemUtility::InsertUrlParam(baseInfo, "&sign_id", AddSignId);

	std::string sign;
	sign += mAppSecret;
	std::vector<std::string> param;
	SystemUtility::InsertParam(param, "app_key", mAppKey);
	SystemUtility::InsertParam(param, "signed_at", SystemUtility::lltoString(unixTimestamp));
	SystemUtility::InsertParam(param, "webinar_id", webinarId);
	SystemUtility::InsertParam(param, "device_no", mDevId);
	SystemUtility::InsertParam(param, "join_id", join_id);
	SystemUtility::InsertParam(param, "token", token);
	SystemUtility::InsertParam(param, "sign_id", AddSignId);
	std::string sortParam = SystemUtility::SortSignParam(param);
	sign += sortParam;
	sign += mAppSecret;

	CMD5 iMD5;
	iMD5.GenerateMD5((unsigned char*)sign.c_str(), sign.length());
	std::string result = iMD5.ToString();
	SystemUtility::InsertUrlParam(baseInfo, "&sign", result);
	return baseInfo;
}

std::string URLGenerator::GetSignStop(std::string webinarId, std::string  join_id, std::string token, const std::string& AddSignId)
{
	std::string baseInfo = mDomain + std::string("/edu/room/sign-stop?");

	long long unixTimestamp = SystemUtility::GetPresentUnixTime();
	SystemUtility::InsertUrlParam(baseInfo, "app_key", mAppKey);
	SystemUtility::InsertUrlParam(baseInfo, "&signed_at", SystemUtility::lltoString(unixTimestamp));
	SystemUtility::InsertUrlParam(baseInfo, "&webinar_id", webinarId);
	SystemUtility::InsertUrlParam(baseInfo, "&device_no", mDevId);

	SystemUtility::InsertUrlParam(baseInfo, "&join_id", join_id);
	SystemUtility::InsertUrlParam(baseInfo, "&token", token);
	SystemUtility::InsertUrlParam(baseInfo, "&sign_id", AddSignId);

	std::string sign;
	sign += mAppSecret;
	std::vector<std::string> param;
	SystemUtility::InsertParam(param, "app_key", mAppKey);
	SystemUtility::InsertParam(param, "signed_at", SystemUtility::lltoString(unixTimestamp));
	SystemUtility::InsertParam(param, "webinar_id", webinarId);
	SystemUtility::InsertParam(param, "device_no", mDevId);
	SystemUtility::InsertParam(param, "join_id", join_id);
	SystemUtility::InsertParam(param, "token", token);
	SystemUtility::InsertParam(param, "sign_id", AddSignId);
	std::string sortParam = SystemUtility::SortSignParam(param);
	sign += sortParam;
	sign += mAppSecret;

	CMD5 iMD5;
	iMD5.GenerateMD5((unsigned char*)sign.c_str(), sign.length());
	std::string result = iMD5.ToString();
	SystemUtility::InsertUrlParam(baseInfo, "&sign", result);
	return baseInfo;
}

std::string URLGenerator::GetAnswerIn(std::string webinarId, std::string  join_id, std::string token, std::string answer, std::string ques_id) {

	std::string baseInfo = mDomain + std::string("/pcv1/questionv2/submit-answer?");

	long long unixTimestamp = SystemUtility::GetPresentUnixTime();
	SystemUtility::InsertUrlParam(baseInfo, "app_key", mAppKey);
	SystemUtility::InsertUrlParam(baseInfo, "&signed_at", SystemUtility::lltoString(unixTimestamp));
	SystemUtility::InsertUrlParam(baseInfo, "&webinar_id", webinarId);
	SystemUtility::InsertUrlParam(baseInfo, "&device_no", mDevId);

	SystemUtility::InsertUrlParam(baseInfo, "&join_id", join_id);
	SystemUtility::InsertUrlParam(baseInfo, "&class_token", token);
	SystemUtility::InsertUrlParam(baseInfo, "&answer", answer);
	SystemUtility::InsertUrlParam(baseInfo, "&question_id", ques_id);

	std::string sign;
	sign += mAppSecret;
	std::vector<std::string> param;
	SystemUtility::InsertParam(param, "app_key", mAppKey);
	SystemUtility::InsertParam(param, "signed_at", SystemUtility::lltoString(unixTimestamp));
	SystemUtility::InsertParam(param, "webinar_id", webinarId);
	SystemUtility::InsertParam(param, "device_no", mDevId);
	SystemUtility::InsertParam(param, "join_id", join_id);
	SystemUtility::InsertParam(param, "class_token", token);
	SystemUtility::InsertParam(param, "content", answer);
	SystemUtility::InsertParam(param, "question_id", ques_id);
	std::string sortParam = SystemUtility::SortSignParam(param);
	sign += sortParam;
	sign += mAppSecret;

	CMD5 iMD5;
	iMD5.GenerateMD5((unsigned char*)sign.c_str(), sign.length());
	std::string result = iMD5.ToString();
	SystemUtility::InsertUrlParam(baseInfo, "&sign", result);
	return baseInfo;
}

std::string URLGenerator::GetAnnounceAnswer(std::string webinarId, std::string  join_id, std::string token, std::string ques_id) {
	std::string baseInfo = mDomain + std::string("/pcv1/questionv2/announce-the-question?");

	long long unixTimestamp = SystemUtility::GetPresentUnixTime();
	SystemUtility::InsertUrlParam(baseInfo, "app_key", mAppKey);
	SystemUtility::InsertUrlParam(baseInfo, "&signed_at", SystemUtility::lltoString(unixTimestamp));
	SystemUtility::InsertUrlParam(baseInfo, "&webinar_id", webinarId);
	SystemUtility::InsertUrlParam(baseInfo, "&device_no", mDevId);

	SystemUtility::InsertUrlParam(baseInfo, "&join_id", join_id);
	SystemUtility::InsertUrlParam(baseInfo, "&class_token", token);
	SystemUtility::InsertUrlParam(baseInfo, "&question_id", ques_id);

	std::string sign;
	sign += mAppSecret;
	std::vector<std::string> param;
	SystemUtility::InsertParam(param, "app_key", mAppKey);
	SystemUtility::InsertParam(param, "signed_at", SystemUtility::lltoString(unixTimestamp));
	SystemUtility::InsertParam(param, "webinar_id", webinarId);
	SystemUtility::InsertParam(param, "device_no", mDevId);
	SystemUtility::InsertParam(param, "join_id", join_id);
	SystemUtility::InsertParam(param, "class_token", token);
	SystemUtility::InsertParam(param, "question_id", ques_id);
	std::string sortParam = SystemUtility::SortSignParam(param);
	sign += sortParam;
	sign += mAppSecret;

	CMD5 iMD5;
	iMD5.GenerateMD5((unsigned char*)sign.c_str(), sign.length());
	std::string result = iMD5.ToString();
	SystemUtility::InsertUrlParam(baseInfo, "&sign", result);
	return baseInfo;
}

std::string URLGenerator::GetStopAnswer(std::string webinarId, std::string  join_id, std::string token, std::string ques_id) {
	std::string baseInfo = mDomain + std::string("/pcv1/questionv2/stop-the-answer?");

	long long unixTimestamp = SystemUtility::GetPresentUnixTime();
	SystemUtility::InsertUrlParam(baseInfo, "app_key", mAppKey);
	SystemUtility::InsertUrlParam(baseInfo, "&signed_at", SystemUtility::lltoString(unixTimestamp));
	SystemUtility::InsertUrlParam(baseInfo, "&webinar_id", webinarId);
	SystemUtility::InsertUrlParam(baseInfo, "&device_no", mDevId);

	SystemUtility::InsertUrlParam(baseInfo, "&join_id", join_id);
	SystemUtility::InsertUrlParam(baseInfo, "&class_token", token);
	SystemUtility::InsertUrlParam(baseInfo, "&question_id", ques_id);

	std::string sign;
	sign += mAppSecret;
	std::vector<std::string> param;
	SystemUtility::InsertParam(param, "app_key", mAppKey);
	SystemUtility::InsertParam(param, "signed_at", SystemUtility::lltoString(unixTimestamp));
	SystemUtility::InsertParam(param, "webinar_id", webinarId);
	SystemUtility::InsertParam(param, "device_no", mDevId);
	SystemUtility::InsertParam(param, "join_id", join_id);
	SystemUtility::InsertParam(param, "class_token", token);
	SystemUtility::InsertParam(param, "question_id", ques_id);
	std::string sortParam = SystemUtility::SortSignParam(param);
	sign += sortParam;
	sign += mAppSecret;

	CMD5 iMD5;
	iMD5.GenerateMD5((unsigned char*)sign.c_str(), sign.length());
	std::string result = iMD5.ToString();
	SystemUtility::InsertUrlParam(baseInfo, "&sign", result);
	return baseInfo;
}

std::string URLGenerator::GetAnswerDetail(std::string webinarId, std::string  join_id, std::string token, std::string ques_id) {
	std::string baseInfo = mDomain + std::string("/pcv1/questionv2/webinar-answer-detail?");

	long long unixTimestamp = SystemUtility::GetPresentUnixTime();
	SystemUtility::InsertUrlParam(baseInfo, "app_key", mAppKey);
	SystemUtility::InsertUrlParam(baseInfo, "&signed_at", SystemUtility::lltoString(unixTimestamp));
	SystemUtility::InsertUrlParam(baseInfo, "&webinar_id", webinarId);
	SystemUtility::InsertUrlParam(baseInfo, "&device_no", mDevId);

	SystemUtility::InsertUrlParam(baseInfo, "&join_id", join_id);
	SystemUtility::InsertUrlParam(baseInfo, "&class_token", token);
	SystemUtility::InsertUrlParam(baseInfo, "&question_id", ques_id);

	std::string sign;
	sign += mAppSecret;
	std::vector<std::string> param;
	SystemUtility::InsertParam(param, "app_key", mAppKey);
	SystemUtility::InsertParam(param, "signed_at", SystemUtility::lltoString(unixTimestamp));
	SystemUtility::InsertParam(param, "webinar_id", webinarId);
	SystemUtility::InsertParam(param, "device_no", mDevId);
	SystemUtility::InsertParam(param, "join_id", join_id);
	SystemUtility::InsertParam(param, "class_token", token);
	SystemUtility::InsertParam(param, "question_id", ques_id);
	std::string sortParam = SystemUtility::SortSignParam(param);
	sign += sortParam;
	sign += mAppSecret;

	CMD5 iMD5;
	iMD5.GenerateMD5((unsigned char*)sign.c_str(), sign.length());
	std::string result = iMD5.ToString();
	SystemUtility::InsertUrlParam(baseInfo, "&sign", result);
	return baseInfo;
}

std::string URLGenerator::GetStartAnswer(std::string webinarId, std::string  join_id, std::string token, std::string content) {

	std::string baseInfo = mDomain + std::string("/pcv1/questionv2/create?");

	long long unixTimestamp = SystemUtility::GetPresentUnixTime();
	SystemUtility::InsertUrlParam(baseInfo, "app_key", mAppKey);
	SystemUtility::InsertUrlParam(baseInfo, "&signed_at", SystemUtility::lltoString(unixTimestamp));
	SystemUtility::InsertUrlParam(baseInfo, "&webinar_id", webinarId);
	SystemUtility::InsertUrlParam(baseInfo, "&device_no", mDevId);

	SystemUtility::InsertUrlParam(baseInfo, "&join_id", join_id);
	SystemUtility::InsertUrlParam(baseInfo, "&token", token);
	SystemUtility::InsertUrlParam(baseInfo, "&content", content);

	std::string sign;
	sign += mAppSecret;
	std::vector<std::string> param;
	SystemUtility::InsertParam(param, "app_key", mAppKey);
	SystemUtility::InsertParam(param, "signed_at", SystemUtility::lltoString(unixTimestamp));
	SystemUtility::InsertParam(param, "webinar_id", webinarId);
	SystemUtility::InsertParam(param, "device_no", mDevId);
	SystemUtility::InsertParam(param, "join_id", join_id);
	SystemUtility::InsertParam(param, "token", token);
	SystemUtility::InsertParam(param, "content", content);
	std::string sortParam = SystemUtility::SortSignParam(param);
	sign += sortParam;
	sign += mAppSecret;

	CMD5 iMD5;
	iMD5.GenerateMD5((unsigned char*)sign.c_str(), sign.length());
	std::string result = iMD5.ToString();
	SystemUtility::InsertUrlParam(baseInfo, "&sign", result);
	return baseInfo;
}

std::string URLGenerator::GetJoinClassRoomUrl(std::string webinarId, std::string nickName, std::string password, std::string phone) {
    std::string baseInfo = mDomain + std::string("/pcv1/webinar/join?");

    long long unixTimestamp = SystemUtility::GetPresentUnixTime();
    SystemUtility::InsertUrlParam(baseInfo, "app_key", mAppKey);
    SystemUtility::InsertUrlParam(baseInfo, "&signed_at", SystemUtility::lltoString(unixTimestamp));
    SystemUtility::InsertUrlParam(baseInfo, "&webinar_id", webinarId);
    SystemUtility::InsertUrlParam(baseInfo, "&device_no", mDevId);
    SystemUtility::InsertUrlParam(baseInfo, "&nick_name", nickName);
    SystemUtility::InsertUrlParam(baseInfo, "&password", password);
	SystemUtility::InsertUrlParam(baseInfo, "&phone", phone);
   SystemUtility::InsertUrlParam(baseInfo, "&new_version", "1");
   SystemUtility::InsertUrlParam(baseInfo, "&live_version_h5", "1");
	//SystemUtility::InsertUrlParam(baseInfo, "&debug_host", "172.16.11.39:8088");// //class.vhall.com   //t-class.e.vhall.com  //172.16.11.39:8088

    std::string sign;
    sign += mAppSecret;
    std::vector<std::string> param;
    SystemUtility::InsertParam(param, "app_key", mAppKey);
    SystemUtility::InsertParam(param, "signed_at", SystemUtility::lltoString(unixTimestamp));
    SystemUtility::InsertParam(param, "webinar_id", webinarId);
    SystemUtility::InsertParam(param, "device_no", mDevId);
    SystemUtility::InsertParam(param, "nick_name", nickName);
    SystemUtility::InsertParam(param, "password", password);
	SystemUtility::InsertParam(param, "phone", phone);
   SystemUtility::InsertParam(param, "new_version", "1");
   SystemUtility::InsertParam(param, "live_version_h5", "1");
    std::string sortParam = SystemUtility::SortSignParam(param);
    sign += sortParam;
    sign += mAppSecret;

    CMD5 iMD5;
    iMD5.GenerateMD5((unsigned char*)sign.c_str(), sign.length());
    std::string result = iMD5.ToString();
    SystemUtility::InsertUrlParam(baseInfo, "&sign", result);
    return baseInfo;
}

std::string URLGenerator::GetJoinClassRoomUrl(std::string cmd)
{
   std::string baseInfo = mDomain + std::string("/pcv1/webinar/join?");
   baseInfo += cmd;
   SystemUtility::InsertUrlParam(baseInfo, "&device_no", mDevId);
   SystemUtility::InsertUrlParam(baseInfo, "&new_version", "1");
   SystemUtility::InsertUrlParam(baseInfo, "&live_version_h5", "1");

   std::string sign;
   sign += mAppSecret;
   std::vector<std::string> param;

   SystemUtility::InsertParam(param, "device_no", mDevId);
   SystemUtility::InsertParam(param, "new_version", "1");
   SystemUtility::InsertParam(param, "live_version_h5", "1");

   std::string sortParam = SystemUtility::SortSignParam(param);
   sign += sortParam;
   sign += mAppSecret;

   CMD5 iMD5;
   iMD5.GenerateMD5((unsigned char*)sign.c_str(), sign.length());
   std::string result = iMD5.ToString();
   SystemUtility::InsertUrlParam(baseInfo, "&sign", result);
   return baseInfo;
}

std::string URLGenerator::GetInteractiveTokenUrl(std::string webinar_id, std::string join_id, std::string token) {
    std::string baseInfo = mDomain + std::string("/pcv1/webinar/token?");

    long long unixTimestamp = SystemUtility::GetPresentUnixTime();
    SystemUtility::InsertUrlParam(baseInfo, "app_key", mAppKey);
    SystemUtility::InsertUrlParam(baseInfo, "&signed_at", SystemUtility::lltoString(unixTimestamp));
    SystemUtility::InsertUrlParam(baseInfo, "&device_no", mDevId);
    SystemUtility::InsertUrlParam(baseInfo, "&webinar_id", webinar_id);
    SystemUtility::InsertUrlParam(baseInfo, "&join_id", join_id);
    SystemUtility::InsertUrlParam(baseInfo, "&token", token);


    std::string sign;
    sign += mAppSecret;
    std::vector<std::string> param;
    SystemUtility::InsertParam(param, "app_key", mAppKey);
    SystemUtility::InsertParam(param, "signed_at", SystemUtility::lltoString(unixTimestamp));
    SystemUtility::InsertParam(param, "device_no", mDevId);
    SystemUtility::InsertParam(param, "webinar_id", webinar_id);
    SystemUtility::InsertParam(param, "join_id", join_id);
    SystemUtility::InsertParam(param, "token", token);

    std::string sortParam = SystemUtility::SortSignParam(param);
    sign += sortParam;
    sign += mAppSecret;

    CMD5 iMD5;
    iMD5.GenerateMD5((unsigned char*)sign.c_str(), sign.length());
    std::string result = iMD5.ToString();
    SystemUtility::InsertUrlParam(baseInfo, "&sign", result);
    return baseInfo;
}

std::string URLGenerator::GetStartClassUrl(std::string webinar_id,std::string join_id, std::string token, int course_type, int role, const int& iDefinitionId,
   const int& iplayType) {
    std::string baseInfo = mDomain + std::string("/pcv1/webinar/start?");
    long long unixTimestamp = SystemUtility::GetPresentUnixTime();
    SystemUtility::InsertUrlParam(baseInfo, "app_key", mAppKey);
    SystemUtility::InsertUrlParam(baseInfo, "&signed_at", SystemUtility::lltoString(unixTimestamp));
    SystemUtility::InsertUrlParam(baseInfo, "&device_no", mDevId);
    char definitionId[64] = { 0 };
    char courseType[64] = { 0 };
    char roleType[64] = { 0 };
    char playType[64] = { 0 };
    SystemUtility::InsertUrlParam(baseInfo, "&course_type", itoa(course_type, courseType, 10));
    SystemUtility::InsertUrlParam(baseInfo, "&role", itoa(role, roleType, 10));
    SystemUtility::InsertUrlParam(baseInfo, "&join_id", join_id);
    SystemUtility::InsertUrlParam(baseInfo, "&class_token", token);
    SystemUtility::InsertUrlParam(baseInfo, "&webinar_id", webinar_id);
    SystemUtility::InsertUrlParam(baseInfo, "&definitionId", itoa(iDefinitionId, definitionId, 10));
    SystemUtility::InsertUrlParam(baseInfo, "&play_type", itoa(iplayType, playType, 10));
	SystemUtility::InsertUrlParam(baseInfo, "&live_version_h5", "1");
    std::string sign;
    sign += mAppSecret;
    std::vector<std::string> param;
    SystemUtility::InsertParam(param, "app_key", mAppKey);
    SystemUtility::InsertParam(param, "signed_at", SystemUtility::lltoString(unixTimestamp));
    SystemUtility::InsertParam(param, "device_no", mDevId);
    SystemUtility::InsertParam(param, "course_type", courseType);
    SystemUtility::InsertParam(param, "role", roleType);
    SystemUtility::InsertParam(param, "join_id", join_id);
    SystemUtility::InsertParam(param, "class_token", token);
    SystemUtility::InsertParam(param, "&definitionId", itoa(iDefinitionId, definitionId, 10));
    SystemUtility::InsertUrlParam(baseInfo, "&play_type", playType);
	SystemUtility::InsertUrlParam(baseInfo, "&live_version_h5", "1");

    std::string sortParam = SystemUtility::SortSignParam(param);
    sign += sortParam;
    sign += mAppSecret;

    CMD5 iMD5;
    iMD5.GenerateMD5((unsigned char*)sign.c_str(), sign.length());
    std::string result = iMD5.ToString();
    SystemUtility::InsertUrlParam(baseInfo, "&sign", result);
    return baseInfo;
}

std::string URLGenerator::GetStopClassUrl(std::string webinar_id, std::string join_id, std::string token, int type) {
    std::string baseInfo = mDomain + std::string("/pcv1/webinar/stop?");
    long long unixTimestamp = SystemUtility::GetPresentUnixTime();
    SystemUtility::InsertUrlParam(baseInfo, "app_key", mAppKey);
    SystemUtility::InsertUrlParam(baseInfo, "&signed_at", SystemUtility::lltoString(unixTimestamp));
    SystemUtility::InsertUrlParam(baseInfo, "&device_no", mDevId);
    SystemUtility::InsertUrlParam(baseInfo, "&webinar_id", webinar_id);
    SystemUtility::InsertUrlParam(baseInfo, "&type", SystemUtility::lltoString(type));
    SystemUtility::InsertUrlParam(baseInfo, "&join_id", join_id);
    SystemUtility::InsertUrlParam(baseInfo, "&token", token);


    std::string sign;
    sign += mAppSecret;
    std::vector<std::string> param;
    SystemUtility::InsertParam(param, "app_key", mAppKey);
    SystemUtility::InsertParam(param, "signed_at", SystemUtility::lltoString(unixTimestamp));
    SystemUtility::InsertParam(param, "device_no", mDevId);
    SystemUtility::InsertParam(param, "webinar_id", webinar_id);
    SystemUtility::InsertParam(param, "type", SystemUtility::lltoString(type));
    SystemUtility::InsertParam(param, "join_id", join_id);
    SystemUtility::InsertParam(param, "token", token);

    std::string sortParam = SystemUtility::SortSignParam(param);
    sign += sortParam;
    sign += mAppSecret;

    CMD5 iMD5;
    iMD5.GenerateMD5((unsigned char*)sign.c_str(), sign.length());
    std::string result = iMD5.ToString();
    SystemUtility::InsertUrlParam(baseInfo, "&sign", result);
    return baseInfo;
}

std::string URLGenerator::GetStartPublishUrl(std::string join_id, std::string webinar_id, std::string class_token, int status, std::string target_id) {

    std::string baseInfo = mDomain + std::string("/pcv1/webinar/switch-mics?");
    long long unixTimestamp = SystemUtility::GetPresentUnixTime();
    SystemUtility::InsertUrlParam(baseInfo, "app_key", mAppKey);
    SystemUtility::InsertUrlParam(baseInfo, "&signed_at", SystemUtility::lltoString(unixTimestamp));
    SystemUtility::InsertUrlParam(baseInfo, "&webinar_id", webinar_id);
    SystemUtility::InsertUrlParam(baseInfo, "&device_no", mDevId);
    SystemUtility::InsertUrlParam(baseInfo, "&join_id", join_id);
    SystemUtility::InsertUrlParam(baseInfo, "&class_token", class_token);
    SystemUtility::InsertUrlParam(baseInfo, "&status", SystemUtility::lltoString(status));
    SystemUtility::InsertUrlParam(baseInfo, "&target_id", target_id);

    std::string sign;
    sign += mAppSecret;
    std::vector<std::string> param;
    SystemUtility::InsertParam(param, "app_key", mAppKey);
    SystemUtility::InsertParam(param, "&signed_at", SystemUtility::lltoString(unixTimestamp));
    SystemUtility::InsertParam(param, "&webinar_id", webinar_id);
    SystemUtility::InsertParam(param, "&device_no", mDevId);
    SystemUtility::InsertParam(param, "&join_id", join_id);
    SystemUtility::InsertParam(param, "&class_token", class_token);
    SystemUtility::InsertParam(param, "&status", SystemUtility::lltoString(status));
    SystemUtility::InsertParam(param, "&target_id", target_id);

    std::string sortParam = SystemUtility::SortSignParam(param);
    sign += sortParam;
    sign += mAppSecret;

    CMD5 iMD5;
    iMD5.GenerateMD5((unsigned char*)sign.c_str(), sign.length());
    std::string result = iMD5.ToString();
    SystemUtility::InsertUrlParam(baseInfo, "&sign", result);
    return baseInfo;
}

// pc-ÉÏÏÂÂóÊ§°Ü·µ»Ø
std::string URLGenerator::GetPublishCallbackUrl(std::string join_id, std::string webinar_id, std::string class_token, int status, std::string target_id) {
    std::string baseInfo = mDomain + std::string("/pcv1/webinar/fail-join-mics?");
    long long unixTimestamp = SystemUtility::GetPresentUnixTime();
    SystemUtility::InsertUrlParam(baseInfo, "app_key", mAppKey);
    SystemUtility::InsertUrlParam(baseInfo, "&signed_at", SystemUtility::lltoString(unixTimestamp));
    SystemUtility::InsertUrlParam(baseInfo, "&webinar_id", webinar_id);
    SystemUtility::InsertUrlParam(baseInfo, "&device_no", mDevId);
    SystemUtility::InsertUrlParam(baseInfo, "&join_id", join_id);
    SystemUtility::InsertUrlParam(baseInfo, "&class_token", class_token);
    SystemUtility::InsertUrlParam(baseInfo, "&status", SystemUtility::lltoString(status));
    SystemUtility::InsertUrlParam(baseInfo, "&target_id", target_id);

    std::string sign;
    sign += mAppSecret;
    std::vector<std::string> param;
    SystemUtility::InsertParam(param, "app_key", mAppKey);
    SystemUtility::InsertParam(param, "&signed_at", SystemUtility::lltoString(unixTimestamp));
    SystemUtility::InsertParam(param, "&webinar_id", webinar_id);
    SystemUtility::InsertParam(param, "&device_no", mDevId);
    SystemUtility::InsertParam(param, "&join_id", join_id);
    SystemUtility::InsertParam(param, "&class_token", class_token);
    SystemUtility::InsertParam(param, "&status", SystemUtility::lltoString(status));
    SystemUtility::InsertParam(param, "&target_id", target_id);

    std::string sortParam = SystemUtility::SortSignParam(param);
    sign += sortParam;
    sign += mAppSecret;

    CMD5 iMD5;
    iMD5.GenerateMD5((unsigned char*)sign.c_str(), sign.length());
    std::string result = iMD5.ToString();
    SystemUtility::InsertUrlParam(baseInfo, "&sign", result);
    return baseInfo;
}

std::string URLGenerator::GetCancelHandUrl(std::string webinar_id, std::string join_id, std::string token)
{
	std::string baseInfo = mDomain + std::string("/pcv1/webinar/cancel-hand?");
	long long unixTimestamp = SystemUtility::GetPresentUnixTime();
	SystemUtility::InsertUrlParam(baseInfo, "app_key", mAppKey);
	SystemUtility::InsertUrlParam(baseInfo, "&signed_at", SystemUtility::lltoString(unixTimestamp));
	SystemUtility::InsertUrlParam(baseInfo, "&device_no", mDevId);

	SystemUtility::InsertUrlParam(baseInfo, "&join_id", join_id);
	SystemUtility::InsertUrlParam(baseInfo, "&webinar_id", webinar_id);
	SystemUtility::InsertUrlParam(baseInfo, "&class_token", token);

	std::string sign;
	sign += mAppSecret;
	std::vector<std::string> param;
	SystemUtility::InsertParam(param, "app_key", mAppKey);
	SystemUtility::InsertParam(param, "&signed_at", SystemUtility::lltoString(unixTimestamp));
	SystemUtility::InsertParam(param, "&device_no", mDevId);

	SystemUtility::InsertParam(param, "&join_id", join_id);
	SystemUtility::InsertParam(param, "&webinar_id", webinar_id);
	SystemUtility::InsertParam(param, "&class_token", token);

	std::string sortParam = SystemUtility::SortSignParam(param);
	sign += sortParam;
	sign += mAppSecret;

	CMD5 iMD5;
	iMD5.GenerateMD5((unsigned char*)sign.c_str(), sign.length());
	std::string result = iMD5.ToString();
	SystemUtility::InsertUrlParam(baseInfo, "&sign", result);
	return baseInfo;
}

std::string URLGenerator::RewardStudent(std::string join_id, std::string targetid, std::string token)
{
	std::string baseInfo = mDomain + std::string("/edu/room/reward-student?");
	long long unixTimestamp = SystemUtility::GetPresentUnixTime();
	SystemUtility::InsertUrlParam(baseInfo, "app_key", mAppKey);
	SystemUtility::InsertUrlParam(baseInfo, "&signed_at", SystemUtility::lltoString(unixTimestamp));
	SystemUtility::InsertUrlParam(baseInfo, "&device_no", mDevId);

	SystemUtility::InsertUrlParam(baseInfo, "&join_id", join_id);
	SystemUtility::InsertUrlParam(baseInfo, "&target_join_id", targetid);
	SystemUtility::InsertUrlParam(baseInfo, "&token", token);

	std::string sign;
	sign += mAppSecret;
	std::vector<std::string> param;
	SystemUtility::InsertParam(param, "app_key", mAppKey);
	SystemUtility::InsertParam(param, "&signed_at", SystemUtility::lltoString(unixTimestamp));
	SystemUtility::InsertParam(param, "&device_no", mDevId);

	SystemUtility::InsertParam(param, "&join_id", join_id);
	SystemUtility::InsertParam(param, "&target_join_id", targetid);
	SystemUtility::InsertParam(param, "&token", token);

	std::string sortParam = SystemUtility::SortSignParam(param);
	sign += sortParam;
	sign += mAppSecret;

	CMD5 iMD5;
	iMD5.GenerateMD5((unsigned char*)sign.c_str(), sign.length());
	std::string result = iMD5.ToString();
	SystemUtility::InsertUrlParam(baseInfo, "&sign", result);
	return baseInfo;
}

std::string URLGenerator::GetSendNoticeUrl(std::string token, std::string event, std::string msg) {
    std::string baseInfo = mDomain + std::string("/pcv1/webinar/sendmsg?");
    long long unixTimestamp = SystemUtility::GetPresentUnixTime();
    SystemUtility::InsertUrlParam(baseInfo, "app_key", mAppKey);
    SystemUtility::InsertUrlParam(baseInfo, "&signed_at", SystemUtility::lltoString(unixTimestamp));
    SystemUtility::InsertUrlParam(baseInfo, "&device_no", mDevId);
    SystemUtility::InsertUrlParam(baseInfo, "&event", event);
    SystemUtility::InsertUrlParam(baseInfo, "&token", token);
    SystemUtility::InsertUrlParam(baseInfo, "&msg", msg);
    SystemUtility::InsertUrlParam(baseInfo, "&new_version", "1");

    std::string sign;
    sign += mAppSecret;
    std::vector<std::string> param;
    SystemUtility::InsertParam(param, "app_key", mAppKey);
    SystemUtility::InsertParam(param, "&signed_at", SystemUtility::lltoString(unixTimestamp));
    SystemUtility::InsertParam(param, "&device_no", mDevId);
    SystemUtility::InsertParam(param, "&event", event);
    SystemUtility::InsertParam(param, "&token", token);
    SystemUtility::InsertParam(param, "&msg", msg);
    SystemUtility::InsertParam(param, "new_version", "1");
    std::string sortParam = SystemUtility::SortSignParam(param);
    sign += sortParam;
    sign += mAppSecret;

    CMD5 iMD5;
    iMD5.GenerateMD5((unsigned char*)sign.c_str(), sign.length());
    std::string result = iMD5.ToString();
    SystemUtility::InsertUrlParam(baseInfo, "&sign", result);
    return baseInfo;
}

// pc-¾ÙÊÖ
std::string URLGenerator::GetHandUpUrl(std::string webinar_id) {
    std::string baseInfo = mDomain + std::string("/pcv1/webinar/open-hand?");
    return baseInfo;
}

std::string URLGenerator::GetForbidChatUrl(std::string webinar_id, std::string join_id, int status, std::string token, std::string target_id) {
    std::string baseInfo = mDomain + std::string("/pcv1/webinar/ban-chat?");
    long long unixTimestamp = SystemUtility::GetPresentUnixTime();
    SystemUtility::InsertUrlParam(baseInfo, "app_key", mAppKey);
    SystemUtility::InsertUrlParam(baseInfo, "&signed_at", SystemUtility::lltoString(unixTimestamp));
    SystemUtility::InsertUrlParam(baseInfo, "&webinar_id", webinar_id);
    SystemUtility::InsertUrlParam(baseInfo, "&device_no", mDevId);
    SystemUtility::InsertUrlParam(baseInfo, "&join_id", join_id);
    SystemUtility::InsertUrlParam(baseInfo, "&target_id", target_id);
    SystemUtility::InsertUrlParam(baseInfo, "&class_token", token);
    SystemUtility::InsertUrlParam(baseInfo, "&status", status == 1 ? "1" : "0");

    std::string sign;
    sign += mAppSecret;
    std::vector<std::string> param;
    SystemUtility::InsertParam(param, "app_key", mAppKey);
    SystemUtility::InsertParam(param, "&signed_at", SystemUtility::lltoString(unixTimestamp));
    SystemUtility::InsertParam(param, "&webinar_id", webinar_id);
    SystemUtility::InsertParam(param, "&device_no", mDevId);
    SystemUtility::InsertParam(param, "&join_id", join_id);
    SystemUtility::InsertParam(param, "&target_id", target_id);
    SystemUtility::InsertParam(param, "&token", token);
    SystemUtility::InsertParam(param, "&status", status == 1 ? "1" : "0");

    std::string sortParam = SystemUtility::SortSignParam(param);
    sign += sortParam;
    sign += mAppSecret;

    CMD5 iMD5;
    iMD5.GenerateMD5((unsigned char*)sign.c_str(), sign.length());
    std::string result = iMD5.ToString();
    SystemUtility::InsertUrlParam(baseInfo, "&sign", result);
    return baseInfo;
}

// pc-È«Ìå½ûÑÔ
std::string URLGenerator::GetForbidChatAllUrl(std::string webinar_id, std::string join_id, int status, std::string token) {
    std::string baseInfo = mDomain + std::string("/pcv1/webinar/ban-chat-all?");
    long long unixTimestamp = SystemUtility::GetPresentUnixTime();
    SystemUtility::InsertUrlParam(baseInfo, "app_key", mAppKey);
    SystemUtility::InsertUrlParam(baseInfo, "&signed_at", SystemUtility::lltoString(unixTimestamp));
    SystemUtility::InsertUrlParam(baseInfo, "&webinar_id", webinar_id);
    SystemUtility::InsertUrlParam(baseInfo, "&device_no", mDevId);
    SystemUtility::InsertUrlParam(baseInfo, "&join_id", join_id);
    SystemUtility::InsertUrlParam(baseInfo, "&token", token);
    SystemUtility::InsertUrlParam(baseInfo, "&status", status == 1 ? "1" : "0");

    std::string sign;
    sign += mAppSecret;
    std::vector<std::string> param;
    SystemUtility::InsertParam(param, "app_key", mAppKey);
    SystemUtility::InsertParam(param, "&signed_at", SystemUtility::lltoString(unixTimestamp));
    SystemUtility::InsertParam(param, "&webinar_id", webinar_id);
    SystemUtility::InsertParam(param, "&device_no", mDevId);
    SystemUtility::InsertParam(param, "&join_id", join_id);
    SystemUtility::InsertParam(param, "&token", token);
    SystemUtility::InsertParam(param, "&status", status == 1 ? "1" : "0");

    std::string sortParam = SystemUtility::SortSignParam(param);
    sign += sortParam;
    sign += mAppSecret;

    CMD5 iMD5;
    iMD5.GenerateMD5((unsigned char*)sign.c_str(), sign.length());
    std::string result = iMD5.ToString();
    SystemUtility::InsertUrlParam(baseInfo, "&sign", result);
    return baseInfo;
}

std::string URLGenerator::GetKickOutRoomUrl(std::string webinar_id, std::string join_id, int status, std::string token, std::string target_id) {
    std::string baseInfo = mDomain + std::string("/pcv1/webinar/kick?");
    long long unixTimestamp = SystemUtility::GetPresentUnixTime();
    SystemUtility::InsertUrlParam(baseInfo, "app_key", mAppKey);
    SystemUtility::InsertUrlParam(baseInfo, "&signed_at", SystemUtility::lltoString(unixTimestamp));
    SystemUtility::InsertUrlParam(baseInfo, "&webinar_id", webinar_id);
    SystemUtility::InsertUrlParam(baseInfo, "&device_no", mDevId);
    SystemUtility::InsertUrlParam(baseInfo, "&join_id", join_id);
    SystemUtility::InsertUrlParam(baseInfo, "&class_token", token);
    SystemUtility::InsertUrlParam(baseInfo, "&status", status == 1 ? "1" : "0");
    SystemUtility::InsertUrlParam(baseInfo, "&target_id", target_id);

    std::string sign;
    sign += mAppSecret;
    std::vector<std::string> param;
    SystemUtility::InsertParam(param, "app_key", mAppKey);
    SystemUtility::InsertParam(param, "&signed_at", SystemUtility::lltoString(unixTimestamp));
    SystemUtility::InsertParam(param, "&webinar_id", webinar_id);
    SystemUtility::InsertParam(param, "&device_no", mDevId);
    SystemUtility::InsertParam(param, "&join_id", join_id);
    SystemUtility::InsertParam(param, "&class_token", token);
    SystemUtility::InsertParam(param, "&status", status == 1 ? "1" : "0");
    SystemUtility::InsertParam(param, "&target_id", target_id);

    std::string sortParam = SystemUtility::SortSignParam(param);
    sign += sortParam;
    sign += mAppSecret;

    CMD5 iMD5;
    iMD5.GenerateMD5((unsigned char*)sign.c_str(), sign.length());
    std::string result = iMD5.ToString();
    SystemUtility::InsertUrlParam(baseInfo, "&sign", result);
    return baseInfo;
}

// pc-ÏÂÂóËùÓÐÓÃ»§
std::string URLGenerator::GetKickAllUserUnPublishUrl(std::string webinar_id, std::string join_id, std::string token) {
    std::string baseInfo = mDomain + std::string("/pcv1/webinar/batch-down-mic?");
    long long unixTimestamp = SystemUtility::GetPresentUnixTime();
    SystemUtility::InsertUrlParam(baseInfo, "app_key", mAppKey);
    SystemUtility::InsertUrlParam(baseInfo, "&signed_at", SystemUtility::lltoString(unixTimestamp));
    SystemUtility::InsertUrlParam(baseInfo, "&webinar_id", webinar_id);
    SystemUtility::InsertUrlParam(baseInfo, "&device_no", mDevId);
    SystemUtility::InsertUrlParam(baseInfo, "&join_id", join_id);
    SystemUtility::InsertUrlParam(baseInfo, "&class_token", token);

    std::string sign;
    sign += mAppSecret;
    std::vector<std::string> param;
    SystemUtility::InsertParam(param, "app_key", mAppKey);
    SystemUtility::InsertParam(param, "&signed_at", SystemUtility::lltoString(unixTimestamp));
    SystemUtility::InsertParam(param, "&webinar_id", webinar_id);
    SystemUtility::InsertParam(param, "&device_no", mDevId);
    SystemUtility::InsertParam(param, "&join_id", join_id);
    SystemUtility::InsertParam(param, "&class_token", token);

    std::string sortParam = SystemUtility::SortSignParam(param);
    sign += sortParam;
    sign += mAppSecret;

    CMD5 iMD5;
    iMD5.GenerateMD5((unsigned char*)sign.c_str(), sign.length());
    std::string result = iMD5.ToString();
    SystemUtility::InsertUrlParam(baseInfo, "&sign", result);
    return baseInfo;
}

// pc-¾²Òô
std::string URLGenerator::GetMuteAudioUrl(std::string webinar_id, std::string join_id, int status, std::string token, std::string target_id) {
    std::string baseInfo = mDomain + std::string("/pcv1/webinar/quiet?");
    long long unixTimestamp = SystemUtility::GetPresentUnixTime();
    SystemUtility::InsertUrlParam(baseInfo, "app_key", mAppKey);
    SystemUtility::InsertUrlParam(baseInfo, "&signed_at", SystemUtility::lltoString(unixTimestamp));
    SystemUtility::InsertUrlParam(baseInfo, "&webinar_id", webinar_id);
    SystemUtility::InsertUrlParam(baseInfo, "&device_no", mDevId);
    SystemUtility::InsertUrlParam(baseInfo, "&join_id", join_id);
    SystemUtility::InsertUrlParam(baseInfo, "&token", token);
    SystemUtility::InsertUrlParam(baseInfo, "&status", status == 1 ? "1" : "0");
    SystemUtility::InsertUrlParam(baseInfo, "&target_id", target_id);

    std::string sign;
    sign += mAppSecret;
    std::vector<std::string> param;
    SystemUtility::InsertParam(param, "app_key", mAppKey);
    SystemUtility::InsertParam(param, "&signed_at", SystemUtility::lltoString(unixTimestamp));
    SystemUtility::InsertParam(param, "&webinar_id", webinar_id);
    SystemUtility::InsertParam(param, "&device_no", mDevId);
    SystemUtility::InsertParam(param, "&join_id", join_id);
    SystemUtility::InsertParam(param, "&token", token);
    SystemUtility::InsertParam(param, "&status", status == 1 ? "1" : "0");
    SystemUtility::InsertParam(param, "&target_id", target_id);

    std::string sortParam = SystemUtility::SortSignParam(param);
    sign += sortParam;
    sign += mAppSecret;

    CMD5 iMD5;
    iMD5.GenerateMD5((unsigned char*)sign.c_str(), sign.length());
    std::string result = iMD5.ToString();
    SystemUtility::InsertUrlParam(baseInfo, "&sign", result);
    return baseInfo;
}

std::string URLGenerator::GetMuteAudioAllUrl(std::string webinar_id, std::string join_id, int status, std::string token) {
    std::string baseInfo = mDomain + std::string("/pcv1/webinar/muting-all?");
    long long unixTimestamp = SystemUtility::GetPresentUnixTime();
    SystemUtility::InsertUrlParam(baseInfo, "app_key", mAppKey);
    SystemUtility::InsertUrlParam(baseInfo, "&signed_at", SystemUtility::lltoString(unixTimestamp));
    SystemUtility::InsertUrlParam(baseInfo, "&webinar_id", webinar_id);
    SystemUtility::InsertUrlParam(baseInfo, "&device_no", mDevId);
    SystemUtility::InsertUrlParam(baseInfo, "&join_id", join_id);
    SystemUtility::InsertUrlParam(baseInfo, "&token", token);
    SystemUtility::InsertUrlParam(baseInfo, "&status", status == 1 ? "1" : "0");

    std::string sign;
    sign += mAppSecret;
    std::vector<std::string> param;
    SystemUtility::InsertParam(param, "app_key", mAppKey);
    SystemUtility::InsertParam(param, "&signed_at", SystemUtility::lltoString(unixTimestamp));
    SystemUtility::InsertParam(param, "&webinar_id", webinar_id);
    SystemUtility::InsertParam(param, "&device_no", mDevId);
    SystemUtility::InsertParam(param, "&join_id", join_id);
    SystemUtility::InsertParam(param, "&token", token);
    SystemUtility::InsertParam(param, "&status", status == 1 ? "1" : "0");

    std::string sortParam = SystemUtility::SortSignParam(param);
    sign += sortParam;
    sign += mAppSecret;

    CMD5 iMD5;
    iMD5.GenerateMD5((unsigned char*)sign.c_str(), sign.length());
    std::string result = iMD5.ToString();
    SystemUtility::InsertUrlParam(baseInfo, "&sign", result);
    return baseInfo;
}

// pc-¹Ø±Õ»­Ãæ
std::string URLGenerator::GetMuteVideoUrl(std::string webinar_id, std::string join_id, int status, std::string token, std::string target_id) {
    std::string baseInfo = mDomain + std::string("/pcv1/webinar/frame?");
    long long unixTimestamp = SystemUtility::GetPresentUnixTime();
    SystemUtility::InsertUrlParam(baseInfo, "app_key", mAppKey);
    SystemUtility::InsertUrlParam(baseInfo, "&signed_at", SystemUtility::lltoString(unixTimestamp));
    SystemUtility::InsertUrlParam(baseInfo, "&webinar_id", webinar_id);
    SystemUtility::InsertUrlParam(baseInfo, "&device_no", mDevId);
    SystemUtility::InsertUrlParam(baseInfo, "&join_id", join_id);
    SystemUtility::InsertUrlParam(baseInfo, "&token", token);
    SystemUtility::InsertUrlParam(baseInfo, "&status", status == 1 ? "1" : "0");
    SystemUtility::InsertUrlParam(baseInfo, "&target_id", target_id);

    std::string sign;
    sign += mAppSecret;
    std::vector<std::string> param;
    SystemUtility::InsertParam(param, "app_key", mAppKey);
    SystemUtility::InsertParam(param, "&signed_at", SystemUtility::lltoString(unixTimestamp));
    SystemUtility::InsertParam(param, "&webinar_id", webinar_id);
    SystemUtility::InsertParam(param, "&device_no", mDevId);
    SystemUtility::InsertParam(param, "&join_id", join_id);
    SystemUtility::InsertParam(param, "&token", token);
    SystemUtility::InsertParam(param, "&status", status == 1 ? "1" : "0");
    SystemUtility::InsertParam(param, "&target_id", target_id);

    std::string sortParam = SystemUtility::SortSignParam(param);
    sign += sortParam;
    sign += mAppSecret;

    CMD5 iMD5;
    iMD5.GenerateMD5((unsigned char*)sign.c_str(), sign.length());
    std::string result = iMD5.ToString();
    SystemUtility::InsertUrlParam(baseInfo, "&sign", result);
    return baseInfo;
}

// pc-ÊÚÈ¨»­±Ê
std::string URLGenerator::GetSetBrushUrl(std::string webinar_id, std::string join_id, int status, std::string token, std::string target_id) {
    std::string baseInfo = mDomain + std::string("/pcv1/webinar/set-brush?");
    long long unixTimestamp = SystemUtility::GetPresentUnixTime();
    SystemUtility::InsertUrlParam(baseInfo, "app_key", mAppKey);
    SystemUtility::InsertUrlParam(baseInfo, "&signed_at", SystemUtility::lltoString(unixTimestamp));
    SystemUtility::InsertUrlParam(baseInfo, "&webinar_id", webinar_id);
    SystemUtility::InsertUrlParam(baseInfo, "&device_no", mDevId);
    SystemUtility::InsertUrlParam(baseInfo, "&join_id", join_id);
    SystemUtility::InsertUrlParam(baseInfo, "&target_id", target_id);
    SystemUtility::InsertUrlParam(baseInfo, "&token", token);
    SystemUtility::InsertUrlParam(baseInfo, "&status", status == 1 ? "1" : "0");

    std::string sign;
    sign += mAppSecret;
    std::vector<std::string> param;
    SystemUtility::InsertParam(param, "app_key", mAppKey);
    SystemUtility::InsertParam(param, "&signed_at", SystemUtility::lltoString(unixTimestamp));
    SystemUtility::InsertParam(param, "&webinar_id", webinar_id);
    SystemUtility::InsertParam(param, "&device_no", mDevId);
    SystemUtility::InsertParam(param, "&join_id", join_id);
    SystemUtility::InsertParam(param, "&target_id", target_id);
    SystemUtility::InsertParam(param, "&token", token);
    SystemUtility::InsertParam(param, "&status", status == 1 ? "1" : "0");

    std::string sortParam = SystemUtility::SortSignParam(param);
    sign += sortParam;
    sign += mAppSecret;

    CMD5 iMD5;
    iMD5.GenerateMD5((unsigned char*)sign.c_str(), sign.length());
    std::string result = iMD5.ToString();
    SystemUtility::InsertUrlParam(baseInfo, "&sign", result);
    return baseInfo;
}

// pc - ÊÇ·ñÔÊÐíÑ§Ô±¾ÙÊÖ
std::string URLGenerator::GetEnableHandsUpUrl(std::string join_id, std::string webinar_id, std::string token, int status) {
    std::string baseInfo = mDomain + std::string("/pcv1/webinar/open-hand?");

    long long unixTimestamp = SystemUtility::GetPresentUnixTime();
    SystemUtility::InsertUrlParam(baseInfo, "app_key", mAppKey);
    SystemUtility::InsertUrlParam(baseInfo, "&signed_at", SystemUtility::lltoString(unixTimestamp));
    SystemUtility::InsertUrlParam(baseInfo, "&webinar_id", webinar_id);
    SystemUtility::InsertUrlParam(baseInfo, "&device_no", mDevId);
    SystemUtility::InsertUrlParam(baseInfo, "&join_id", join_id);
    SystemUtility::InsertUrlParam(baseInfo, "&token", token);
    SystemUtility::InsertUrlParam(baseInfo, "&status", status == 1 ? "1" : "0");
	SystemUtility::InsertUrlParam(baseInfo, "&new_version", "1");
    std::string sign;
    sign += mAppSecret;
    std::vector<std::string> param;
    SystemUtility::InsertParam(param, "app_key", mAppKey);
    SystemUtility::InsertParam(param, "&signed_at", SystemUtility::lltoString(unixTimestamp));
    SystemUtility::InsertParam(param, "&webinar_id", webinar_id);
    SystemUtility::InsertParam(param, "&device_no", mDevId);
    SystemUtility::InsertParam(param, "&join_id", join_id);
    SystemUtility::InsertParam(param, "&token", token);
    SystemUtility::InsertParam(param, "&status", status == 1 ? "1" : "0");
	SystemUtility::InsertParam(param, "&new_version", "1");
    std::string sortParam = SystemUtility::SortSignParam(param);
    sign += sortParam;
    sign += mAppSecret;

    CMD5 iMD5;
    iMD5.GenerateMD5((unsigned char*)sign.c_str(), sign.length());
    std::string result = iMD5.ToString();
    SystemUtility::InsertUrlParam(baseInfo, "&sign", result);
    return baseInfo;
}


std::string URLGenerator::GetOnlineUserUrl(std::string join_id, std::string webinar_id, std::string token, int curr_page, int pagesize) {
    std::string baseInfo = mDomain + std::string("/pcv1/webinar/online-user?");

    long long unixTimestamp = SystemUtility::GetPresentUnixTime();
    SystemUtility::InsertUrlParam(baseInfo, "app_key", mAppKey);
    SystemUtility::InsertUrlParam(baseInfo, "&signed_at", SystemUtility::lltoString(unixTimestamp));
    SystemUtility::InsertUrlParam(baseInfo, "&webinar_id", webinar_id);
    SystemUtility::InsertUrlParam(baseInfo, "&device_no", mDevId);
    SystemUtility::InsertUrlParam(baseInfo, "&join_id", join_id);
    SystemUtility::InsertUrlParam(baseInfo, "&class_token", token);
    if (curr_page != -1) {
        SystemUtility::InsertUrlParam(baseInfo, "&curr_page", SystemUtility::lltoString(curr_page));
        SystemUtility::InsertUrlParam(baseInfo, "&pagesize", SystemUtility::lltoString(pagesize));
    }

    std::string sign;
    sign += mAppSecret;
    std::vector<std::string> param;
    SystemUtility::InsertParam(param, "app_key", mAppKey);
    SystemUtility::InsertParam(param, "&signed_at", SystemUtility::lltoString(unixTimestamp));
    SystemUtility::InsertParam(param, "&webinar_id", webinar_id);
    SystemUtility::InsertParam(param, "&device_no", mDevId);
    SystemUtility::InsertParam(param, "&join_id", join_id);
    SystemUtility::InsertParam(param, "&class_token", token);
    if (curr_page != -1) {
        SystemUtility::InsertParam(param, "&curr_page", SystemUtility::lltoString(curr_page));
        SystemUtility::InsertParam(param, "&pagesize", SystemUtility::lltoString(pagesize));
    }

    std::string sortParam = SystemUtility::SortSignParam(param);
    sign += sortParam;
    sign += mAppSecret;

    CMD5 iMD5;
    iMD5.GenerateMD5((unsigned char*)sign.c_str(), sign.length());
    std::string result = iMD5.ToString();
    SystemUtility::InsertUrlParam(baseInfo, "&sign", result);
    return baseInfo;
}

std::string URLGenerator::GetKickOutUserUrl(std::string join_id, std::string webinar_id, std::string class_token) {
    std::string baseInfo = mDomain + std::string("/pcv1/webinar/kicked-user?");

    long long unixTimestamp = SystemUtility::GetPresentUnixTime();
    SystemUtility::InsertUrlParam(baseInfo, "app_key", mAppKey);
    SystemUtility::InsertUrlParam(baseInfo, "&signed_at", SystemUtility::lltoString(unixTimestamp));
    SystemUtility::InsertUrlParam(baseInfo, "&webinar_id", webinar_id);
    SystemUtility::InsertUrlParam(baseInfo, "&device_no", mDevId);
    SystemUtility::InsertUrlParam(baseInfo, "&join_id", join_id);
    SystemUtility::InsertUrlParam(baseInfo, "&class_token", class_token);

    std::string sign;
    sign += mAppSecret;
    std::vector<std::string> param;
    SystemUtility::InsertParam(param, "app_key", mAppKey);
    SystemUtility::InsertParam(param, "&signed_at", SystemUtility::lltoString(unixTimestamp));
    SystemUtility::InsertParam(param, "&webinar_id", webinar_id);
    SystemUtility::InsertParam(param, "&device_no", mDevId);
    SystemUtility::InsertParam(param, "&join_id", join_id);
    SystemUtility::InsertParam(param, "&class_token", class_token);

    std::string sortParam = SystemUtility::SortSignParam(param);
    sign += sortParam;
    sign += mAppSecret;

    CMD5 iMD5;
    iMD5.GenerateMD5((unsigned char*)sign.c_str(), sign.length());
    std::string result = iMD5.ToString();
    SystemUtility::InsertUrlParam(baseInfo, "&sign", result);
    return baseInfo;
}

std::string URLGenerator::GetHandsUpUserUrl(std::string join_id, std::string webinar_id, std::string class_token) {
    std::string baseInfo = mDomain + std::string("/pcv1/webinar/open-hand-users?");

    long long unixTimestamp = SystemUtility::GetPresentUnixTime();
    SystemUtility::InsertUrlParam(baseInfo, "app_key", mAppKey);
    SystemUtility::InsertUrlParam(baseInfo, "&signed_at", SystemUtility::lltoString(unixTimestamp));
    SystemUtility::InsertUrlParam(baseInfo, "&webinar_id", webinar_id);
    SystemUtility::InsertUrlParam(baseInfo, "&device_no", mDevId);
    SystemUtility::InsertUrlParam(baseInfo, "&join_id", join_id);
    SystemUtility::InsertUrlParam(baseInfo, "&class_token", class_token);

    std::string sign;
    sign += mAppSecret;
    std::vector<std::string> param;
    SystemUtility::InsertParam(param, "app_key", mAppKey);
    SystemUtility::InsertParam(param, "&signed_at", SystemUtility::lltoString(unixTimestamp));
    SystemUtility::InsertParam(param, "&webinar_id", webinar_id);
    SystemUtility::InsertParam(param, "&device_no", mDevId);
    SystemUtility::InsertParam(param, "&join_id", join_id);
    SystemUtility::InsertParam(param, "&class_token", class_token);

    std::string sortParam = SystemUtility::SortSignParam(param);
    sign += sortParam;
    sign += mAppSecret;

    CMD5 iMD5;
    iMD5.GenerateMD5((unsigned char*)sign.c_str(), sign.length());
    std::string result = iMD5.ToString();
    SystemUtility::InsertUrlParam(baseInfo, "&sign", result);
    return baseInfo;
}

std::string URLGenerator::GetSilencedUserUrl(std::string join_id, std::string webinar_id, std::string class_token) {
    std::string baseInfo = mDomain + std::string("/pcv1/webinar/silenced-user?");

    long long unixTimestamp = SystemUtility::GetPresentUnixTime();
    SystemUtility::InsertUrlParam(baseInfo, "app_key", mAppKey);
    SystemUtility::InsertUrlParam(baseInfo, "&signed_at", SystemUtility::lltoString(unixTimestamp));
    SystemUtility::InsertUrlParam(baseInfo, "&webinar_id", webinar_id);
    SystemUtility::InsertUrlParam(baseInfo, "&device_no", mDevId);
    SystemUtility::InsertUrlParam(baseInfo, "&join_id", join_id);
    SystemUtility::InsertUrlParam(baseInfo, "&class_token", class_token);

    std::string sign;
    sign += mAppSecret;
    std::vector<std::string> param;
    SystemUtility::InsertParam(param, "app_key", mAppKey);
    SystemUtility::InsertParam(param, "&signed_at", SystemUtility::lltoString(unixTimestamp));
    SystemUtility::InsertParam(param, "&webinar_id", webinar_id);
    SystemUtility::InsertParam(param, "&device_no", mDevId);
    SystemUtility::InsertParam(param, "&join_id", join_id);
    SystemUtility::InsertParam(param, "&class_token", class_token);

    std::string sortParam = SystemUtility::SortSignParam(param);
    sign += sortParam;
    sign += mAppSecret;

    CMD5 iMD5;
    iMD5.GenerateMD5((unsigned char*)sign.c_str(), sign.length());
    std::string result = iMD5.ToString();
    SystemUtility::InsertUrlParam(baseInfo, "&sign", result);
    return baseInfo;
}

std::string URLGenerator::GetScreensharingUrl(std::string join_id, std::string webinar_id, std::string class_token, int status, const std::string &target_id) {
    std::string baseInfo = mDomain + std::string("/pcv1/webinar/open-screensharing?");

    long long unixTimestamp = SystemUtility::GetPresentUnixTime();
    SystemUtility::InsertUrlParam(baseInfo, "app_key", mAppKey);
    SystemUtility::InsertUrlParam(baseInfo, "&signed_at", SystemUtility::lltoString(unixTimestamp));
    SystemUtility::InsertUrlParam(baseInfo, "&webinar_id", webinar_id);
    SystemUtility::InsertUrlParam(baseInfo, "&device_no", mDevId);
    SystemUtility::InsertUrlParam(baseInfo, "&join_id", join_id);
    SystemUtility::InsertUrlParam(baseInfo, "&class_token", class_token);
    SystemUtility::InsertUrlParam(baseInfo, "&status", SystemUtility::lltoString(status));
	 SystemUtility::InsertUrlParam(baseInfo, "&target_id", target_id);

    std::string sign;
    sign += mAppSecret;
    std::vector<std::string> param;
    SystemUtility::InsertParam(param, "app_key", mAppKey);
    SystemUtility::InsertParam(param, "&signed_at", SystemUtility::lltoString(unixTimestamp));
    SystemUtility::InsertParam(param, "&webinar_id", webinar_id);
    SystemUtility::InsertParam(param, "&device_no", mDevId);
    SystemUtility::InsertParam(param, "&join_id", join_id);
    SystemUtility::InsertParam(param, "&class_token", class_token);
    SystemUtility::InsertParam(param, "&status", SystemUtility::lltoString(status));
	 SystemUtility::InsertParam(param, "&target_id", target_id);

    std::string sortParam = SystemUtility::SortSignParam(param);
    sign += sortParam;
    sign += mAppSecret;

    CMD5 iMD5;
    iMD5.GenerateMD5((unsigned char*)sign.c_str(), sign.length());
    std::string result = iMD5.ToString();
    SystemUtility::InsertUrlParam(baseInfo, "&sign", result);
    return baseInfo;
}

std::string URLGenerator::GetStopDesktopSharingUrl(std::string join_id, std::string webinar_id, std::string class_token, std::string target_id)
{
   std::string baseInfo = mDomain + std::string("/pcv1/webinar/open-screensharing?");

   long long unixTimestamp = SystemUtility::GetPresentUnixTime();
   SystemUtility::InsertUrlParam(baseInfo, "app_key", mAppKey);
   SystemUtility::InsertUrlParam(baseInfo, "&signed_at", SystemUtility::lltoString(unixTimestamp));
   SystemUtility::InsertUrlParam(baseInfo, "&webinar_id", webinar_id);
   SystemUtility::InsertUrlParam(baseInfo, "&device_no", mDevId);
   SystemUtility::InsertUrlParam(baseInfo, "&join_id", join_id);
   SystemUtility::InsertUrlParam(baseInfo, "&class_token", class_token);
   SystemUtility::InsertUrlParam(baseInfo, "&status", "0");
   SystemUtility::InsertUrlParam(baseInfo, "&target_id", target_id);
   std::string sign;
   sign += mAppSecret;
   std::vector<std::string> param;
   SystemUtility::InsertParam(param, "app_key", mAppKey);
   SystemUtility::InsertParam(param, "&signed_at", SystemUtility::lltoString(unixTimestamp));
   SystemUtility::InsertParam(param, "&webinar_id", webinar_id);
   SystemUtility::InsertParam(param, "&device_no", mDevId);
   SystemUtility::InsertParam(param, "&join_id", join_id);
   SystemUtility::InsertParam(param, "&class_token", class_token);
   SystemUtility::InsertParam(param, "&status", "0");
   SystemUtility::InsertParam(param, "&target_id", target_id);

   std::string sortParam = SystemUtility::SortSignParam(param);
   sign += sortParam;
   sign += mAppSecret;

   CMD5 iMD5;
   iMD5.GenerateMD5((unsigned char*)sign.c_str(), sign.length());
   std::string result = iMD5.ToString();
   SystemUtility::InsertUrlParam(baseInfo, "&sign", result);

   return baseInfo;
}


std::string URLGenerator::GetCheckJoinMics(std::string join_id, std::string webinar_id, std::string class_token, int status, int target_id) {
    std::string baseInfo = mDomain + std::string("/pcv1/webinar/check-join-mics?");

    long long unixTimestamp = SystemUtility::GetPresentUnixTime();
    SystemUtility::InsertUrlParam(baseInfo, "app_key", mAppKey);
    SystemUtility::InsertUrlParam(baseInfo, "&signed_at", SystemUtility::lltoString(unixTimestamp));
    SystemUtility::InsertUrlParam(baseInfo, "&webinar_id", webinar_id);
    SystemUtility::InsertUrlParam(baseInfo, "&device_no", mDevId);
    SystemUtility::InsertUrlParam(baseInfo, "&join_id", join_id);
    SystemUtility::InsertUrlParam(baseInfo, "&class_token", class_token);
    SystemUtility::InsertUrlParam(baseInfo, "&status", SystemUtility::lltoString(status));
    SystemUtility::InsertUrlParam(baseInfo, "&target_id", SystemUtility::lltoString(target_id));
    SystemUtility::InsertUrlParam(baseInfo, "&new_version", "1");
    std::string sign;
    sign += mAppSecret;
    std::vector<std::string> param;
    SystemUtility::InsertParam(param, "app_key", mAppKey);
    SystemUtility::InsertParam(param, "&signed_at", SystemUtility::lltoString(unixTimestamp));
    SystemUtility::InsertParam(param, "&webinar_id", webinar_id);
    SystemUtility::InsertParam(param, "&device_no", mDevId);
    SystemUtility::InsertParam(param, "&join_id", join_id);
    SystemUtility::InsertParam(param, "&class_token", class_token);
    SystemUtility::InsertParam(param, "&status", SystemUtility::lltoString(status));
    SystemUtility::InsertParam(param, "&target_id", SystemUtility::lltoString(target_id));
    SystemUtility::InsertParam(param, "new_version", "1");

    std::string sortParam = SystemUtility::SortSignParam(param);
    sign += sortParam;
    sign += mAppSecret;

    CMD5 iMD5;
    iMD5.GenerateMD5((unsigned char*)sign.c_str(), sign.length());
    std::string result = iMD5.ToString();
    SystemUtility::InsertUrlParam(baseInfo, "&sign", result);
    return baseInfo;
}


std::string URLGenerator::GetUpdatePublishAbility(std::string join_id, std::string webinar_id, std::string class_token, 
   int no_ablility, int no_ablity_double) {
    std::string baseInfo = mDomain + std::string("/pcv1/webinar/update-mics?");

    long long unixTimestamp = SystemUtility::GetPresentUnixTime();
    SystemUtility::InsertUrlParam(baseInfo, "app_key", mAppKey);
    SystemUtility::InsertUrlParam(baseInfo, "&signed_at", SystemUtility::lltoString(unixTimestamp));
    SystemUtility::InsertUrlParam(baseInfo, "&webinar_id", webinar_id);
    SystemUtility::InsertUrlParam(baseInfo, "&device_no", mDevId);
    SystemUtility::InsertUrlParam(baseInfo, "&join_id", join_id);
    SystemUtility::InsertUrlParam(baseInfo, "&class_token", class_token);
    SystemUtility::InsertUrlParam(baseInfo, "&no_mic", SystemUtility::lltoString(no_ablility));
    SystemUtility::InsertUrlParam(baseInfo, "&no_double_vrtc", SystemUtility::lltoString(no_ablity_double));

    std::string sign;
    sign += mAppSecret;
    std::vector<std::string> param;
    SystemUtility::InsertParam(param, "app_key", mAppKey);
    SystemUtility::InsertParam(param, "&signed_at", SystemUtility::lltoString(unixTimestamp));
    SystemUtility::InsertParam(param, "&webinar_id", webinar_id);
    SystemUtility::InsertParam(param, "&device_no", mDevId);
    SystemUtility::InsertParam(param, "&join_id", join_id);
    SystemUtility::InsertParam(param, "&class_token", class_token);
    SystemUtility::InsertParam(param, "&no_mic", SystemUtility::lltoString(no_ablility));
    SystemUtility::InsertParam(param, "&no_double_vrtc", SystemUtility::lltoString(no_ablity_double));

    std::string sortParam = SystemUtility::SortSignParam(param);
    sign += sortParam;
    sign += mAppSecret;

    CMD5 iMD5;
    iMD5.GenerateMD5((unsigned char*)sign.c_str(), sign.length());
    std::string result = iMD5.ToString();
    SystemUtility::InsertUrlParam(baseInfo, "&sign", result);
    return baseInfo;
}

std::string URLGenerator::GetSendMicsDeviceUrl(std::string join_id, std::string webinar_id, std::string class_token, const std::string &audio_id,
   const std::string& camera_id, const std::string& double_camera_id, const int roleType)
{
   std::string baseInfo = mDomain + std::string("/pcv1/room/mics-device?");

   long long unixTimestamp = SystemUtility::GetPresentUnixTime();
   SystemUtility::InsertUrlParam(baseInfo, "app_key", mAppKey);
   SystemUtility::InsertUrlParam(baseInfo, "&signed_at", SystemUtility::lltoString(unixTimestamp));
   SystemUtility::InsertUrlParam(baseInfo, "&webinar_id", webinar_id);
   SystemUtility::InsertUrlParam(baseInfo, "&device_no", mDevId);
   SystemUtility::InsertUrlParam(baseInfo, "&join_id", join_id);
   SystemUtility::InsertUrlParam(baseInfo, "&class_token", class_token);

   SystemUtility::InsertUrlParam(baseInfo, "&audio_id", audio_id);
   SystemUtility::InsertUrlParam(baseInfo, "&camera_id", camera_id);
   SystemUtility::InsertUrlParam(baseInfo, "&camera_2_id", double_camera_id);
   SystemUtility::InsertUrlParam(baseInfo, "&camera_2_type", SystemUtility::lltoString(roleType));


   std::string sign;
   sign += mAppSecret;
   std::vector<std::string> param;
   SystemUtility::InsertParam(param, "app_key", mAppKey);
   SystemUtility::InsertParam(param, "&signed_at", SystemUtility::lltoString(unixTimestamp));
   SystemUtility::InsertParam(param, "&webinar_id", webinar_id);
   SystemUtility::InsertParam(param, "&device_no", mDevId);
   SystemUtility::InsertParam(param, "&join_id", join_id);
   SystemUtility::InsertParam(param, "&class_token", class_token);

   SystemUtility::InsertParam(param, "&audio_id", audio_id);
   SystemUtility::InsertParam(param, "&camera_id", camera_id);
   SystemUtility::InsertParam(param, "&camera_2_id", double_camera_id);
   SystemUtility::InsertParam(param, "&camera_2_type", SystemUtility::lltoString(roleType));

   std::string sortParam = SystemUtility::SortSignParam(param);
   sign += sortParam;
   sign += mAppSecret;

   CMD5 iMD5;
   iMD5.GenerateMD5((unsigned char*)sign.c_str(), sign.length());
   std::string result = iMD5.ToString();
   SystemUtility::InsertUrlParam(baseInfo, "&sign", result);
   return baseInfo;
}

std::string URLGenerator::GetInviteDoubleVrtcUrl(std::string join_id, std::string webinar_id, 
   std::string class_token, const std::string &target_id)
{
   std::string baseInfo = mDomain + std::string("/pcv1/doublevrtc/invite-double-vrtc?");

   long long unixTimestamp = SystemUtility::GetPresentUnixTime();
   SystemUtility::InsertUrlParam(baseInfo, "app_key", mAppKey);
   SystemUtility::InsertUrlParam(baseInfo, "&signed_at", SystemUtility::lltoString(unixTimestamp));
   SystemUtility::InsertUrlParam(baseInfo, "&webinar_id", webinar_id);
   SystemUtility::InsertUrlParam(baseInfo, "&device_no", mDevId);
   SystemUtility::InsertUrlParam(baseInfo, "&join_id", join_id);
   SystemUtility::InsertUrlParam(baseInfo, "&class_token", class_token);

   SystemUtility::InsertUrlParam(baseInfo, "&target_id", target_id);



   std::string sign;
   sign += mAppSecret;
   std::vector<std::string> param;
   SystemUtility::InsertParam(param, "app_key", mAppKey);
   SystemUtility::InsertParam(param, "&signed_at", SystemUtility::lltoString(unixTimestamp));
   SystemUtility::InsertParam(param, "&webinar_id", webinar_id);
   SystemUtility::InsertParam(param, "&device_no", mDevId);
   SystemUtility::InsertParam(param, "&join_id", join_id);
   SystemUtility::InsertParam(param, "&class_token", class_token);
   SystemUtility::InsertParam(param, "&target_id", target_id);

   std::string sortParam = SystemUtility::SortSignParam(param);
   sign += sortParam;
   sign += mAppSecret;

   CMD5 iMD5;
   iMD5.GenerateMD5((unsigned char*)sign.c_str(), sign.length());
   std::string result = iMD5.ToString();
   SystemUtility::InsertUrlParam(baseInfo, "&sign", result);
   return baseInfo;
}

std::string URLGenerator::GetInviteDesktopVrtcUrl(std::string join_id, std::string webinar_id,
   std::string class_token, const std::string &target_id)
{
   std::string baseInfo = mDomain + std::string("/pcv1/screensharing/invite?");

   long long unixTimestamp = SystemUtility::GetPresentUnixTime();
   SystemUtility::InsertUrlParam(baseInfo, "app_key", mAppKey);
   SystemUtility::InsertUrlParam(baseInfo, "&signed_at", SystemUtility::lltoString(unixTimestamp));
   SystemUtility::InsertUrlParam(baseInfo, "&webinar_id", webinar_id);
   SystemUtility::InsertUrlParam(baseInfo, "&device_no", mDevId);
   SystemUtility::InsertUrlParam(baseInfo, "&join_id", join_id);
   SystemUtility::InsertUrlParam(baseInfo, "&class_token", class_token);
   SystemUtility::InsertUrlParam(baseInfo, "&target_id", target_id);

   std::string sign;
   sign += mAppSecret;
   std::vector<std::string> param;
   SystemUtility::InsertParam(param, "app_key", mAppKey);
   SystemUtility::InsertParam(param, "&signed_at", SystemUtility::lltoString(unixTimestamp));
   SystemUtility::InsertParam(param, "&webinar_id", webinar_id);
   SystemUtility::InsertParam(param, "&device_no", mDevId);
   SystemUtility::InsertParam(param, "&join_id", join_id);
   SystemUtility::InsertParam(param, "&class_token", class_token);
   SystemUtility::InsertParam(param, "&target_id", target_id);

   std::string sortParam = SystemUtility::SortSignParam(param);
   sign += sortParam;
   sign += mAppSecret;

   CMD5 iMD5;
   iMD5.GenerateMD5((unsigned char*)sign.c_str(), sign.length());
   std::string result = iMD5.ToString();
   SystemUtility::InsertUrlParam(baseInfo, "&sign", result);
   return baseInfo;
}
//开始双推
std::string URLGenerator::GetStartDoubleVrtcUrl(std::string join_id, std::string webinar_id, std::string class_token)
{
   std::string baseInfo = mDomain + std::string("/pcv1/doublevrtc/start-double-vrtc?");
   createBaseInfo(baseInfo, join_id, webinar_id, class_token);
   return baseInfo;
}
//结束双推
std::string URLGenerator::GetStopDoubleVrtcUrl(std::string target_id, std::string join_id, std::string webinar_id, std::string class_token)
{
   std::string baseInfo = mDomain + std::string("/pcv1/doublevrtc/stop-double-vrtc?");
   long long unixTimestamp = SystemUtility::GetPresentUnixTime();
   SystemUtility::InsertUrlParam(baseInfo, "app_key", mAppKey);
   SystemUtility::InsertUrlParam(baseInfo, "&signed_at", SystemUtility::lltoString(unixTimestamp));
   SystemUtility::InsertUrlParam(baseInfo, "&webinar_id", webinar_id);
   SystemUtility::InsertUrlParam(baseInfo, "&device_no", mDevId);
   SystemUtility::InsertUrlParam(baseInfo, "&join_id", join_id);
   SystemUtility::InsertUrlParam(baseInfo, "&target_id", target_id);
   SystemUtility::InsertUrlParam(baseInfo, "&class_token", class_token);

   std::string sign;
   sign += mAppSecret;
   std::vector<std::string> param;
   SystemUtility::InsertParam(param, "app_key", mAppKey);
   SystemUtility::InsertParam(param, "&signed_at", SystemUtility::lltoString(unixTimestamp));
   SystemUtility::InsertParam(param, "&webinar_id", webinar_id);
   SystemUtility::InsertParam(param, "&device_no", mDevId);
   SystemUtility::InsertParam(param, "&join_id", join_id);
   SystemUtility::InsertParam(param, "&target_id", target_id);
   SystemUtility::InsertParam(param, "&class_token", class_token);

   std::string sortParam = SystemUtility::SortSignParam(param);
   sign += sortParam;
   sign += mAppSecret;

   CMD5 iMD5;
   iMD5.GenerateMD5((unsigned char*)sign.c_str(), sign.length());
   std::string result = iMD5.ToString();
   SystemUtility::InsertUrlParam(baseInfo, "&sign", result);
   return baseInfo;
}
//双推学员列表
std::string URLGenerator::GetDoubleVrtcUsersUrl(std::string join_id, std::string webinar_id, std::string class_token)
{
   std::string baseInfo = mDomain + std::string("/pcv1/doublevrtc/double-vrtc-users?");
   createBaseInfo(baseInfo, join_id, webinar_id, class_token);
   return baseInfo;
}


std::string URLGenerator::GetSetScreenLayoutUrl(std::string join_id,  std::string webinar_id, std::string class_token, int desktop_share_layout_mode, const int& iLayoutMode) {
   std::string baseInfo = mDomain + std::string("/pcv1/screensharing/sync-share-layout-mode?");

   long long unixTimestamp = SystemUtility::GetPresentUnixTime();
   SystemUtility::InsertUrlParam(baseInfo, "app_key", mAppKey);
   SystemUtility::InsertUrlParam(baseInfo, "&signed_at", SystemUtility::lltoString(unixTimestamp));
   SystemUtility::InsertUrlParam(baseInfo, "&webinar_id", webinar_id);
   SystemUtility::InsertUrlParam(baseInfo, "&device_no", mDevId);
   SystemUtility::InsertUrlParam(baseInfo, "&join_id", join_id);
   SystemUtility::InsertUrlParam(baseInfo, "& token", class_token);
   if (-1==iLayoutMode) {
      
   }
   else
   {
      SystemUtility::InsertUrlParam(baseInfo, "&mixed_flow_layout_mode", std::to_string(iLayoutMode));
   }
   SystemUtility::InsertUrlParam(baseInfo, "&desktop_share_layout_mode", std::to_string(desktop_share_layout_mode));
   std::string sign;
   sign += mAppSecret;
   std::vector<std::string> param;
   SystemUtility::InsertParam(param, "app_key", mAppKey);
   SystemUtility::InsertParam(param, "&signed_at", SystemUtility::lltoString(unixTimestamp));
   SystemUtility::InsertParam(param, "&webinar_id", webinar_id);
   SystemUtility::InsertParam(param, "&device_no", mDevId);
   SystemUtility::InsertParam(param, "&join_id", join_id);
   SystemUtility::InsertParam(param, "& token", class_token);
   if (-1 == iLayoutMode) {
      
   }
   else {
      SystemUtility::InsertParam(param, "&mixed_flow_layout_mode", std::to_string(iLayoutMode));
   }
   SystemUtility::InsertParam(param, "&desktop_share_layout_mode", std::to_string(desktop_share_layout_mode));

   std::string sortParam = SystemUtility::SortSignParam(param);
   sign += sortParam;
   sign += mAppSecret;

   CMD5 iMD5;
   iMD5.GenerateMD5((unsigned char*)sign.c_str(), sign.length());
   std::string result = iMD5.ToString();
   SystemUtility::InsertUrlParam(baseInfo, "&sign", result);
   return baseInfo;
}


std::string URLGenerator::GetSetBigScreenUrl(std::string join_id, std::string target_id, std::string webinar_id, std::string class_token){
   std::string baseInfo = mDomain + std::string("/pcv1/webinar/set-bigscreen?");

   long long unixTimestamp = SystemUtility::GetPresentUnixTime();
   SystemUtility::InsertUrlParam(baseInfo, "app_key", mAppKey);
   SystemUtility::InsertUrlParam(baseInfo, "&signed_at", SystemUtility::lltoString(unixTimestamp));
   SystemUtility::InsertUrlParam(baseInfo, "&webinar_id", webinar_id);
   SystemUtility::InsertUrlParam(baseInfo, "&device_no", mDevId);
   SystemUtility::InsertUrlParam(baseInfo, "&join_id", join_id);
   SystemUtility::InsertUrlParam(baseInfo, "&target_id", target_id);
   SystemUtility::InsertUrlParam(baseInfo, "&class_token", class_token);

   std::string sign;
   sign += mAppSecret;
   std::vector<std::string> param;
   SystemUtility::InsertParam(param, "app_key", mAppKey);
   SystemUtility::InsertParam(param, "&signed_at", SystemUtility::lltoString(unixTimestamp));
   SystemUtility::InsertParam(param, "&webinar_id", webinar_id);
   SystemUtility::InsertParam(param, "&device_no", mDevId);
   SystemUtility::InsertParam(param, "&join_id", join_id);
   SystemUtility::InsertParam(param, "&target_id", target_id);
   SystemUtility::InsertParam(param, "&class_token", class_token);

   std::string sortParam = SystemUtility::SortSignParam(param);
   sign += sortParam;
   sign += mAppSecret;

   CMD5 iMD5;
   iMD5.GenerateMD5((unsigned char*)sign.c_str(), sign.length());
   std::string result = iMD5.ToString();
   SystemUtility::InsertUrlParam(baseInfo, "&sign", result);
   return baseInfo;
}

std::string URLGenerator::GetPublishListUrl(std::string join_id, std::string webinar_id, std::string class_token) {
   std::string baseInfo = mDomain + std::string("/pcv1/webinar/joinmicusers?");

   long long unixTimestamp = SystemUtility::GetPresentUnixTime();
   SystemUtility::InsertUrlParam(baseInfo, "app_key", mAppKey);
   SystemUtility::InsertUrlParam(baseInfo, "&signed_at", SystemUtility::lltoString(unixTimestamp));
   SystemUtility::InsertUrlParam(baseInfo, "&webinar_id", webinar_id);
   SystemUtility::InsertUrlParam(baseInfo, "&device_no", mDevId);
   SystemUtility::InsertUrlParam(baseInfo, "&join_id", join_id);
   SystemUtility::InsertUrlParam(baseInfo, "&class_token", class_token);

   std::string sign;
   sign += mAppSecret;
   std::vector<std::string> param;
   SystemUtility::InsertParam(param, "app_key", mAppKey);
   SystemUtility::InsertParam(param, "&signed_at", SystemUtility::lltoString(unixTimestamp));
   SystemUtility::InsertParam(param, "&webinar_id", webinar_id);
   SystemUtility::InsertParam(param, "&device_no", mDevId);
   SystemUtility::InsertParam(param, "&join_id", join_id);
   SystemUtility::InsertParam(param, "&class_token", class_token);

   std::string sortParam = SystemUtility::SortSignParam(param);
   sign += sortParam;
   sign += mAppSecret;

   CMD5 iMD5;
   iMD5.GenerateMD5((unsigned char*)sign.c_str(), sign.length());
   std::string result = iMD5.ToString();
   SystemUtility::InsertUrlParam(baseInfo, "&sign", result);
   return baseInfo;
}

void URLGenerator::createBaseInfo(std::string& baseInfo, std::string join_id, std::string webinar_id, std::string class_token)
{
   long long unixTimestamp = SystemUtility::GetPresentUnixTime();
   SystemUtility::InsertUrlParam(baseInfo, "app_key", mAppKey);
   SystemUtility::InsertUrlParam(baseInfo, "&signed_at", SystemUtility::lltoString(unixTimestamp));
   SystemUtility::InsertUrlParam(baseInfo, "&webinar_id", webinar_id);
   SystemUtility::InsertUrlParam(baseInfo, "&device_no", mDevId);
   SystemUtility::InsertUrlParam(baseInfo, "&join_id", join_id);
   SystemUtility::InsertUrlParam(baseInfo, "&class_token", class_token);

   std::string sign;
   sign += mAppSecret;
   std::vector<std::string> param;
   SystemUtility::InsertParam(param, "app_key", mAppKey);
   SystemUtility::InsertParam(param, "&signed_at", SystemUtility::lltoString(unixTimestamp));
   SystemUtility::InsertParam(param, "&webinar_id", webinar_id);
   SystemUtility::InsertParam(param, "&device_no", mDevId);
   SystemUtility::InsertParam(param, "&join_id", join_id);
   SystemUtility::InsertParam(param, "&class_token", class_token);

   std::string sortParam = SystemUtility::SortSignParam(param);
   sign += sortParam;
   sign += mAppSecret;

   CMD5 iMD5;
   iMD5.GenerateMD5((unsigned char*)sign.c_str(), sign.length());
   std::string result = iMD5.ToString();
   SystemUtility::InsertUrlParam(baseInfo, "&sign", result);
}
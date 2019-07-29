#ifndef Live_h_
#define Live_h_

#include "ui_Live.h"
#include "VideoRender.h"
#include "WndList.h"

#define MaxShowMembers 50

//房间中用户类型
enum E_RoomUserType
{
	E_RoomUserInvalid = -1,//用户不可用
	E_RoomUserWatcher, //观众
	E_RoomUserCreator, //主播
	E_RoomUserJoiner,  //连麦者
};

//房间成员属性
struct RoomMember
{
	QString szID;//ID
	E_RoomUserType userType;//用户类型
};

//直播窗口
class Live : public QDialog
{
	Q_OBJECT
public:
	Live(QWidget * parent = 0, Qt::WindowFlags f = 0);

	void setRoomID(int roomID);//设置直播房间号
	void setRoomUserType(E_RoomUserType userType);//设置直播房间当前用户类型
	void ChangeRoomUserType();//改变当前用户类型

	void dealMessage(const Message& message);//处理消息
	void parseCusMessage(const std::string& sender,std::string msg);//解析自定义消息
	void dealCusMessage(const std::string& sender, int nUserAction, QString szActionParam);//处理自定义消息
	
	void StartTimer();//启动心跳计时器
	void stopTimer();//停止心跳计时器
	void onMixStream(std::string streamCode);//混流成功，刷新聊天框信息

	static void OnMemStatusChange(E_EndpointEventId event_id, const Vector<String> &ids, void* data);//房间成员事件通知回调
	static void OnRoomDisconnect(int reason, const char *errorinfo, void* data);//主动退出房间回调
	static void OnDeviceDetect(void* data);//设备拔插监听回调
	static void OnLocalVideo(const LiveVideoFrame* video_frame, void* custom_data);//本地视频渲染回调（执行本地视频与屏幕分享渲染）
	static void OnRemoteVideo(const LiveVideoFrame* video_frame, void* custom_data);//远程画面视频渲染回调（执行远程画面渲染）
	static void OnMessage( const Message& msg, void* data );//消息监听回调
	static void OnDeviceOperation(E_DeviceOperationType oper, int retCode, void* data);//设备操作回调
	static void OnQualityParamCallback(const iLiveRoomStatParam& param, void* data);//直播质量报告回调

private slots:
	void OnBtnOpenCamera();
	void OnBtnCloseCamera();
	void OnBtnOpenExternalCapture();
	void OnBtnCloseExternalCapture();
	void OnBtnOpenMic();
	void OnBtnCloseMic();
	void OnBtnOpenPlayer();
	void OnBtnClosePlayer();
	void OnBtnOpenScreenShareArea();
	void OnBtnOpenScreenShareWnd();
	void OnBtnUpdateScreenShare();
	void OnBtnCloseScreenShare();
	void OnBtnOpenSystemVoiceInput();
	void OnBtnCloseSystemVoiceInput();
	void OnBtnSendGroupMsg();//发送 点击
	void OnBtnStartPushStream();//旁路推流-开始推流 点击
	void OnBtnStopPushStream();//旁路推流-停止推流 点击
	void OnBtnPraise();//点赞-为主播点赞 点击
	void OnBtnSelectMediaFile();//文件播放-选择 点击
	void OnBtnPlayMediaFile();//文件播放-播放 点击
	void OnBtnStopMediaFile();//文件播放-停止 点击
	void OnHsPlayerVol(int value);
	void OnSbPlayerVol(int value);
	void OnHsMicVol(int value);
	void OnSbMicVol(int value);
	void OnHsSystemVoiceInputVol(int value);
	void OnSbSystemVoiceInputVol(int value);
	void OnVsSkinSmoothChanged(int value);
	void OnSbSkinSmoothChanged(int value);
	void OnVsSkinWhiteChanged(int value);
	void OnSbSkinWhiteChanged(int value);
	void OnHsMediaFileRateChanged(int value);
	void OnHeartBeatTimer();//触发 心跳计时器
	void OnFillFrameTimer();
	void OnPlayMediaFileTimer();//触发 自动重播计时器
	void OnMemberListMenu(QPoint point);//触发 菜单选项
	void OnActInviteInteract();//菜单-连麦 点击 
	void OnActCancelInteract();//菜单-断开 点击
	void on_btnMix_clicked();//开始混流点击


protected:
	void closeEvent(QCloseEvent* event) override;//关闭窗口

private:
	//自定义私有函数
	void updateCameraList();//更新摄像头列表QComboBox
	VideoRender* findVideoRender(std::string szIdentifier);//根据渲染器ID查找视频渲染器（远程画面）
	VideoRender* getFreeVideoRender(std::string szIdentifier);//从远程画面渲染器集合中获取可用的渲染器
	void freeAllCameraVideoRender();//释放所有视频渲染器（本地画面，辅路视频，远程画面）

	void addMsgLab(QString msg);//增加聊天框消息

	void updateMemberList();//更新直播间用户列表
	void updateMsgs();//更新聊天窗口显示信息

	void updateCameraGB();//更新摄像头组合框QGroupBox
	void updatePlayerGB();//更新扬声器组合框QGroupBox
	void updateExternalCaptureGB();//更新自定义采集组合框QGroupBox
	void updateMicGB();//更新麦克风组合框QGroupBox
	void updateScreenShareGB();//更新屏幕分享组合框QGroupBox
	void updateSystemVoiceInputGB();//更新系统声音采集音量组合框QGroupBox
	void updateMediaFilePlayGB();//更新文件播放组合框QGroupBox
	void updatePushStreamGB();//更新旁路推流组合框QGroupBox
	
	void updatePlayMediaFileProgress();//更新播放本地视频控件参数设定
	void doStartPlayMediaFile();//开始播放文件
	void doPausePlayMediaFile();//暂停播放文件
	void doResumePlayMediaFile();//继续播放文件
	void doStopPlayMediaFile();//停止播放文件

	void doAutoStopPushStream();//自动停止推流

	//设备操作回调
	void OnOpenCameraCB(const int& retCode);
	void OnCloseCameraCB(const int& retCode);

	void OnOpenExternalCaptureCB(const int& retCode);
	void OnCloseExternalCaptureCB(const int& retCode);

	void OnOpenMicCB(const int& retCode);
	void OnCloseMicCB(const int& retCode);

	void OnOpenPlayerCB(const int& retCode);
	void OnClosePlayerCB(const int& retCode);

	void OnOpenScreenShareCB(const int& retCode);
	void OnCloseScreenShareCB(const int& retCode);

	void OnOpenSystemVoiceInputCB(const int& retCode);
	void OnCloseSystemVoiceInputCB(const int& retCode);

	void OnOpenPlayMediaFileCB(const int& retCode);
	void OnClosePlayMediaFileCB(const int& retCode);

	//信令层函数
	void sendInviteInteract();//主播向普通观众发出连麦邀请
	void sendCancelInteract();//主播向连麦中的观众发出断线命令
	static void OnSendInviteInteractSuc(void* data);//连麦成功回调
	static void OnSendInviteInteractErr(const int code, const char *desc, void* data);//连麦失败回调

	void acceptInteract();//普通观众接受连麦邀请
	void refuseInteract();//普通观众拒绝连麦邀请
	void OnAcceptInteract();//接受连麦

	void exitInteract();//连麦观众执行主播发出的断线命令
	void OnExitInteract();//连麦断线
	void onMixTextClick(const QUrl &url);

	void sendQuitRoom();//主播发送退出房间信令

	//随心播服务器请求相关函数
	void sxbCreatorQuitRoom();//向服务器发送主播创离开房间消息
	void sxbWatcherOrJoinerQuitRoom();//向服务器发送观众或连麦者离开房间消息
	void sxbHeartBeat();//向服务器发送心跳
	void sxbRoomIdList();//向服务器发送拉取房间成员列表请求
	static void OnSxbCreatorQuitRoom(int errorCode, QString errorInfo, QVariantMap datamap, void* pCusData);//主播退出房间回调
	static void OnSxbWatcherOrJoinerQuitRoom(int errorCode, QString errorInfo, QVariantMap datamap, void* pCusData);//观众或连麦者退出房间回调
	static void OnSxbHeartBeat(int errorCode, QString errorInfo, QVariantMap datamap, void* pCusData);//心跳回调
	static void OnSxbRoomIdList(int errorCode, QString errorInfo, QVariantMap datamap, void* pCusData);//成员列表请求回调

	//iLiveSDK相关函数
	void iLiveQuitRoom();//退出直播房间
	void iLiveChangeRole(const std::string& szControlRole);//结束推流，改变角色
	int iLiveSetSkinSmoothGrade(int grade);//设置美颜程度
	int iLiveSetSkinWhitenessGrade(int grade);//设置美白程度
	static void OnQuitRoomSuc(void* data);//退出直播房间成功回调
	static void OnQuitRoomErr(int code, const char *desc, void* data);//退出直播房间失败回调
	static void OnChangeRoleSuc(void* data);//结束推流改变角色成功回调
	static void OnChangeRoleErr(int code, const char *desc, void* data);//结束推流改变角色失败回调

	static void OnSendGroupMsgSuc(void* data);//群发消息成功回调
	static void OnSendGroupMsgErr(int code, const char *desc, void* data);//群发消息失败回调

	static void OnStartPushStreamSuc(PushStreamRsp &value, void *data);//开始推流成功回调
	static void OnStartPushStreamErr(int code, const char * desc, void* data);//开始推流失败回调

	static void OnStopPushStreamSuc(void* data);//停止推流成功回调
	static void OnStopPushStreamErr(int code, const char *desc, void* data);//停止推流失败回调
	
private:
	Ui::Live		m_ui;
	
	E_RoomUserType  m_userType;//房间用户类型

	VideoRender*	m_pLocalCameraRender;//本地画面视频窗口
	VideoRender*	m_pScreenShareRender;//辅路视频窗口

	Vector< Pair<String/*id*/, String/*name*/> > m_cameraList;

	std::vector<VideoRender*>	m_pRemoteVideoRenders;//远程画面窗口集合

	int					m_nRoomSize;
	QVector<RoomMember> m_roomMemberList;

	QTimer*			m_pTimer;//心跳计时器
	QTimer*			m_pFillFrameTimer;
	QTimer*			m_pPlayMediaFileTimer;//本地文件播放计时器，到时自动重播
	
	int				m_nCurSelectedMember;
	QMenu*			m_pMenuInviteInteract;
	QMenu*			m_pMenuCancelInteract;

	QString				m_inputRecordName;
	PushStreamOption	m_pushOpt;
	uint64				m_channelId;
	std::list<LiveUrl>	m_pushUrls;

	int32	m_x0;//屏幕分享坐标x0
	int32	m_y0; //屏幕分享坐标有y0
	int32	m_x1; //屏幕分享坐标x1
	int32	m_y1; //屏幕分享坐标y1
	uint32	m_fps;//屏幕分享捕获帧率,取值范围[1,10]。注意: 此参数暂无意义，sdk会根据网络情况，动态调整fps;

	int64	m_n64Pos;//本地视频当前所在时长
	int64	m_n64MaxPos;//本地视频总时长

	bool	m_bRoomDisconnectClose;

	bool	m_bRecording;
	bool	m_bPushing;//当前是否开始推流

	QString	m_szMsgs;//聊天窗口文本
	int mRoomId;//房间号
};

#endif//Live_h_
#ifndef MainWnd_h_
#define MainWnd_h_

#include "ui_MainWindow.h"
#include "Live.h"
#include "Register.h"
#include "RoomListItem.h"
#include "DeviceTest.h"

#define OnePageCout 10 //一页显示房间数量

enum E_LoginState
{
	E_InvalidState = -1,//不可用
	E_Logout,//已登出
	E_Logining,//登录中
	E_Login,//已登录
};

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	MainWindow( QWidget * parent = 0, Qt::WindowFlags flags = 0 );
	QString getUserId();
	QString	getServerUrl();
	QString getToken();
	E_LoginState getLoginState();
	Live*	getLiveView();

	void setCurRoomIdfo(const Room& roominfo);
	const Room& getCurRoomInfo();
	void increasePraise();//增加点赞

	void setUseable(bool bUseable);
	void OnForceOffline();

private:
	void initSDK();//初始化iLiveSDK
	void readConfig();//读取配置信息
	void saveConfig();//保存配置信息
	void connectSignals();
	void switchLoginState(E_LoginState state);//改变登录状态
	static void onForceOffline();//强制下线回调
	static void onUserSigExpired();//用户Sig过期回调

	void clearShowRoomList();
	void updatePageNum();
	void updateRoomList();

	//业务侧服务器相关操作
	void sxbLogin();
	void sxbLogout();
	void sxbCreateRoom();
	void sxbReportroom();
	void sxbCreatorJoinRoom();//主播加入房间
	void sxbRoomList();
	static void OnSxbLogin(int errorCode, QString errorInfo, QVariantMap datamap, void* pCusData);
	static void OnSxbLogout(int errorCode, QString errorInfo, QVariantMap datamap, void* pCusData);
	static void OnSxbCreateRoom(int errorCode, QString errorInfo, QVariantMap datamap, void* pCusData);
	static void OnSxbReportroom(int errorCode, QString errorInfo, QVariantMap datamap, void* pCusData);
	static void OnSxbCreatorJoinRoom(int errorCode, QString errorInfo, QVariantMap datamap, void* pCusData);
	static void OnSxbRoomList(int errorCode, QString errorInfo, QVariantMap datamap, void* pCusData);

	void picDown(QString userId, QString url); //下载图片
	static void OnPicDown(int errorCode, QString errorInfo, QString picPath, void* pCusData);//下载图片回调

	//iLiveSDK相关操作
 	void iLiveLogin();
 	void iLiveLogout();
	void iLiveCreateRoom();
	static void OniLiveLoginSuccess(void* data);
	static void OniLiveLoginError(int code, const char *desc, void* data);
	static void OniLiveLogoutSuccess(void* data);
	static void OniLiveLogoutError(int code, const char *desc, void* data);
	static void OniLiveCreateRoomSuc(void* data);
	static void OniLiveCreateRoomErr(int code, const char *desc, void* data);
	
	static void OnStartDeviceTestSuc(void* data);
	static void OnStartDeviceTestErr(int code, const char *desc, void* data);

protected:
	void closeEvent(QCloseEvent* event) override;

private slots:
	void onBtnLogin();
	void onBtnRegister();
	void onBtnBeginLive();
	void onBtnRefreshLiveList();
	void onBtnLastPage();
	void onBtnNextPage();
	void on_btnDeviceTest_clicked();

private:
	Ui::MainWindow		m_ui;
	
	QSettings*			m_pSetting;//配置信息
	int					m_nAppId;//App_ID
	QString				m_szServerUrl;//服务器Url

	E_LoginState		m_eLoginState;//当前登录状态 

	QString				m_szUserId;//用户ID
	QString				m_szUserPassword;//用户密码
	QString				m_szUserSig;//用户Sig
	QString				m_szUserToken;

	Register*			m_pRegister;//注册窗口指针
	Live*				m_pLive;//直播窗口指针
	DeviceTest*			m_pDeviceTest;//设备测试窗口指针

	int					m_nCurrentPage;//当前页数
	int					m_nTotalPage;//总页数

	QVector<Room>		m_showRooms;//所有房间信息集合
	RoomListItem*		m_pRoomListItem[OnePageCout];//所有直播房间集合

	Room				m_curRoomInfo;//当前选择房间信息
};

#endif //MainWnd_h_
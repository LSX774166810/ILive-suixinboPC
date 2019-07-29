#ifndef MainWnd_h_
#define MainWnd_h_

#include "ui_MainWindow.h"
#include "Live.h"
#include "Register.h"
#include "RoomListItem.h"
#include "DeviceTest.h"

#define OnePageCout 10 //һҳ��ʾ��������

enum E_LoginState
{
	E_InvalidState = -1,//������
	E_Logout,//�ѵǳ�
	E_Logining,//��¼��
	E_Login,//�ѵ�¼
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
	void increasePraise();//���ӵ���

	void setUseable(bool bUseable);
	void OnForceOffline();

private:
	void initSDK();//��ʼ��iLiveSDK
	void readConfig();//��ȡ������Ϣ
	void saveConfig();//����������Ϣ
	void connectSignals();
	void switchLoginState(E_LoginState state);//�ı��¼״̬
	static void onForceOffline();//ǿ�����߻ص�
	static void onUserSigExpired();//�û�Sig���ڻص�

	void clearShowRoomList();
	void updatePageNum();
	void updateRoomList();

	//ҵ����������ز���
	void sxbLogin();
	void sxbLogout();
	void sxbCreateRoom();
	void sxbReportroom();
	void sxbCreatorJoinRoom();//�������뷿��
	void sxbRoomList();
	static void OnSxbLogin(int errorCode, QString errorInfo, QVariantMap datamap, void* pCusData);
	static void OnSxbLogout(int errorCode, QString errorInfo, QVariantMap datamap, void* pCusData);
	static void OnSxbCreateRoom(int errorCode, QString errorInfo, QVariantMap datamap, void* pCusData);
	static void OnSxbReportroom(int errorCode, QString errorInfo, QVariantMap datamap, void* pCusData);
	static void OnSxbCreatorJoinRoom(int errorCode, QString errorInfo, QVariantMap datamap, void* pCusData);
	static void OnSxbRoomList(int errorCode, QString errorInfo, QVariantMap datamap, void* pCusData);

	void picDown(QString userId, QString url); //����ͼƬ
	static void OnPicDown(int errorCode, QString errorInfo, QString picPath, void* pCusData);//����ͼƬ�ص�

	//iLiveSDK��ز���
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
	
	QSettings*			m_pSetting;//������Ϣ
	int					m_nAppId;//App_ID
	QString				m_szServerUrl;//������Url

	E_LoginState		m_eLoginState;//��ǰ��¼״̬ 

	QString				m_szUserId;//�û�ID
	QString				m_szUserPassword;//�û�����
	QString				m_szUserSig;//�û�Sig
	QString				m_szUserToken;

	Register*			m_pRegister;//ע�ᴰ��ָ��
	Live*				m_pLive;//ֱ������ָ��
	DeviceTest*			m_pDeviceTest;//�豸���Դ���ָ��

	int					m_nCurrentPage;//��ǰҳ��
	int					m_nTotalPage;//��ҳ��

	QVector<Room>		m_showRooms;//���з�����Ϣ����
	RoomListItem*		m_pRoomListItem[OnePageCout];//����ֱ�����伯��

	Room				m_curRoomInfo;//��ǰѡ�񷿼���Ϣ
};

#endif //MainWnd_h_
#include "stdafx.h"
#include "MainWindow.h"

MainWindow::MainWindow(QWidget * parent /*= 0*/, Qt::WindowFlags flags /*= 0 */)
{
	m_ui.setupUi(this);
	this->setWindowIcon(QIcon(":/res/Resources/logo.ico"));

	m_pSetting = new QSettings("config.ini", QSettings::IniFormat, this);
	m_pLive = new Live(this, Qt::Dialog | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
	m_pRegister = new Register(this);
	m_pDeviceTest = new DeviceTest(this);

	for (int i = 0; i < OnePageCout; ++i)
	{
		m_pRoomListItem[i] = new RoomListItem(this);
		m_ui.layoutShowRoomList->addWidget(m_pRoomListItem[i], i < 5 ? 0 : 1, i % 5, 1, 1);
		m_pRoomListItem[i]->setVisible(false);
	}

	m_nAppId = 0;

	m_eLoginState = E_Logout;

	m_nCurrentPage = 0;
	m_nTotalPage = 0;

	m_nAppId = SuixinboAppid;
	m_szServerUrl = SuixinboServerUrl;

	readConfig();
	initSDK();
	connectSignals();

	m_ui.lbVersion->setText(QString("iLiveSDK Version: ") + QString(GetILive()->getVersion()));
}

QString MainWindow::getUserId()
{
	return m_szUserId;
}

QString MainWindow::getServerUrl()
{
	return m_szServerUrl;
}

QString MainWindow::getToken()
{
	return m_szUserToken;
}

E_LoginState MainWindow::getLoginState()
{
	return m_eLoginState;
}

Live* MainWindow::getLiveView()
{
	return m_pLive;
}

void MainWindow::setCurRoomIdfo(const Room& roominfo)
{
	m_curRoomInfo = roominfo;
}

const Room& MainWindow::getCurRoomInfo()
{
	return m_curRoomInfo;
}

void MainWindow::increasePraise()
{
	m_curRoomInfo.info.thumbup++;
}

void MainWindow::setUseable(bool bUseable)
{
	m_ui.groupBox->setEnabled(bUseable);
	m_ui.tabWidget->setEnabled(bUseable);
}

void MainWindow::initSDK()
{
	GetILive()->setMessageCallBack(Live::OnMessage, m_pLive);
	GetILive()->setForceOfflineCallback(onForceOffline);
	GetILive()->setUserSigExpiredCallback(onUserSigExpired);
	GetILive()->setRemoteVideoCallBack(Live::OnRemoteVideo, m_pLive);
	GetILive()->setChannelMode(E_ChannelIMSDK);

	iLiveRootView* pView = iLiveCreateRootView();
	E_ColorFormat fmt = (pView->getRootViewType() == ROOT_VIEW_TYPE_D3D) ? COLOR_FORMAT_I420 : COLOR_FORMAT_RGB24;
	GetILive()->setVideoColorFormat(fmt);//iLiveSDK目前的渲染模块，D3D只支持I420格式，GDI只支持RGB24格式;
	pView->destroy();

	int nRet = GetILive()->init(m_nAppId);
	if (nRet != NO_ERR)
	{
		ShowErrorTips("init sdk failed.", this);
		exit(0);
	}
}

void MainWindow::readConfig()
{
	if (m_pSetting->contains("userId"))
	{
		m_ui.edUserName->setText(m_pSetting->value("userId").toString());
	}
	if (m_pSetting->contains("userPwd"))
	{
		m_ui.edPassword->setText(m_pSetting->value("userPwd").toString());
	}
}

void MainWindow::saveConfig()
{
	m_pSetting->setValue("userId", QVariant(m_szUserId));
	m_pSetting->setValue("userPwd", QVariant(m_szUserPassword));
}

void MainWindow::connectSignals()
{
	connect(m_ui.btnLogin, SIGNAL(clicked()), this, SLOT(onBtnLogin()));
	connect(m_ui.btnBeginLive, SIGNAL(clicked()), this, SLOT(onBtnBeginLive()));
	connect(m_ui.btnRegister, SIGNAL(clicked()), this, SLOT(onBtnRegister()));
	connect(m_ui.btnRefreshLiveList, SIGNAL(clicked()), this, SLOT(onBtnRefreshLiveList()));
	connect(m_ui.btnLastPage, SIGNAL(clicked()), this, SLOT(onBtnLastPage()));
	connect(m_ui.btnNextPage, SIGNAL(clicked()), this, SLOT(onBtnNextPage()));
}

void MainWindow::switchLoginState(E_LoginState state)
{
	m_eLoginState = state;
	switch (m_eLoginState)
	{
	case E_Logout:
		m_ui.btnLogin->setText(QString::fromLocal8Bit("登录"));
		m_ui.btnLogin->setEnabled(true);
		m_ui.edUserName->setEnabled(true);
		m_ui.edPassword->setEnabled(true);
		m_ui.btnRegister->setEnabled(true);
		clearShowRoomList();
		break;
	case E_Logining:
		m_ui.btnLogin->setText(QString::fromLocal8Bit("登录中..."));
		m_ui.btnLogin->setEnabled(false);
		m_ui.edUserName->setEnabled(false);
		m_ui.edPassword->setEnabled(false);
		m_ui.btnRegister->setEnabled(false);
		break;
	case E_Login:
		m_ui.btnLogin->setText(QString::fromLocal8Bit("登出"));
		m_ui.btnLogin->setEnabled(true);
		m_ui.edUserName->setEnabled(false);
		m_ui.edPassword->setEnabled(false);
		m_ui.btnRegister->setEnabled(false);
		break;
	default:
		break;
	}
}

void MainWindow::onForceOffline()
{
	ShowTips(FromBits("被踢下线"), FromBits("你的账号在其他地方登陆,点击确认后关闭!"), g_pMainWindow);
	g_pMainWindow->saveConfig();
	TerminateProcess(GetCurrentProcess(), 0);
}

void MainWindow::onUserSigExpired()
{
	ShowTips(FromBits("被踢下线"), FromBits("你的账号sig过期,点击确认后关闭!"), g_pMainWindow);
	g_pMainWindow->saveConfig();
	TerminateProcess(GetCurrentProcess(), 0);
}

void MainWindow::clearShowRoomList()
{
	m_nCurrentPage = 0;
	m_nTotalPage = 0;
	updatePageNum();

	m_showRooms.clear();
	updateRoomList();
}

void MainWindow::updatePageNum()
{
	QString szPageText;
	int nShowPage = m_nTotalPage > 0 ? m_nCurrentPage + 1 : 0;
	szPageText.sprintf("%d/%d", nShowPage, m_nTotalPage);
	m_ui.lbPageNum->setText(szPageText);

	if (m_nCurrentPage == 0)
	{
		m_ui.btnLastPage->setEnabled(false);
	}
	else
	{
		m_ui.btnLastPage->setEnabled(true);
	}

	if (m_nCurrentPage >= m_nTotalPage - 1)
	{
		m_ui.btnNextPage->setEnabled(false);
	}
	else
	{
		m_ui.btnNextPage->setEnabled(true);
	}
}

void MainWindow::updateRoomList()
{
	for (int i = 0; i < OnePageCout; ++i)
	{
		if (i < m_showRooms.size())
		{
			m_pRoomListItem[i]->setRoomParam(m_showRooms[i]);
			m_pRoomListItem[i]->setVisible(true);
		}
		else
		{
			m_pRoomListItem[i]->setVisible(false);
		}
	}
}

void MainWindow::sxbLogin()
{
	m_szUserId = m_ui.edUserName->text();
	m_szUserPassword = m_ui.edPassword->text();

	QVariantMap varmap;
	varmap.insert("id", m_szUserId);
	varmap.insert("pwd", m_szUserPassword);
	varmap.insert("appid", QString::number(m_nAppId));
	SxbServerHelper::request(varmap, "account", "login", OnSxbLogin, this);
}

void MainWindow::sxbLogout()
{
	QVariantMap varmap;
	varmap.insert("token", m_szUserToken);
	SxbServerHelper::request(varmap, "account", "logout", OnSxbLogout, this);
}

void MainWindow::sxbCreateRoom()
{
	QVariantMap varmap;
	varmap.insert("token", m_szUserToken);
	varmap.insert("type", "live");
	SxbServerHelper::request(varmap, "live", "create", OnSxbCreateRoom, this);
}

void MainWindow::sxbReportroom()
{
	QVariantMap varmap;

	varmap.insert("token", m_szUserToken);

	QVariantMap roommap;
	roommap.insert("title", m_curRoomInfo.info.title);//选填
	roommap.insert("roomnum", m_curRoomInfo.info.roomnum);
	roommap.insert("type", "live");
	roommap.insert("groupid", QString::number(m_curRoomInfo.info.roomnum));
	roommap.insert("cover", "");//选填
	roommap.insert("appid", QString::number(m_nAppId));
	roommap.insert("device", 2);//0 IOS 1 Android 2 PC
	roommap.insert("videotype", 0);//0 摄像头 1 屏幕分享
	varmap.insert("room", roommap);

	SxbServerHelper::request(varmap, "live", "reportroom", OnSxbReportroom, this);
}

void MainWindow::sxbCreatorJoinRoom()
{
	QVariantMap varmap;
	varmap.insert("token", m_szUserToken);
	varmap.insert("id", m_szUserId);
	varmap.insert("roomnum", m_curRoomInfo.info.roomnum);
	varmap.insert("role", 1);//主播 1 成员 0 上麦成员 2
	varmap.insert("operate", 0);//进入房间0  退出房间1
	SxbServerHelper::request(varmap, "live", "reportmemid", OnSxbCreatorJoinRoom, this);
}

void MainWindow::sxbRoomList()
{
	QVariantMap varmap;
	varmap.insert("token", m_szUserToken);
	varmap.insert("type", "live");
	varmap.insert("index", m_nCurrentPage*OnePageCout);
	varmap.insert("size", OnePageCout);
	varmap.insert("appid", QString::number(m_nAppId));
	SxbServerHelper::request(varmap, "live", "roomlist", OnSxbRoomList, this);
}

void MainWindow::OnSxbLogin(int errorCode, QString errorInfo, QVariantMap datamap, void* pCusData)
{
	MainWindow* pMainWindow = reinterpret_cast<MainWindow*>(pCusData);

	if (datamap.contains("userSig"))
	{
		pMainWindow->m_szUserSig = datamap.value("userSig").toString();
	}
	if (datamap.contains("token"))
	{
		pMainWindow->m_szUserToken = datamap.value("token").toString();
	}

	if (errorCode == E_SxbOK)
	{
		pMainWindow->iLiveLogin();
	}
	else
	{
		pMainWindow->switchLoginState(E_Logout);
		ShowCodeErrorTips(errorCode, errorInfo, pMainWindow, FromBits("登录服务器出错"));
	}
}

void MainWindow::OnSxbLogout(int errorCode, QString errorInfo, QVariantMap datamap, void* pCusData)
{
	MainWindow* pMainWindow = reinterpret_cast<MainWindow*>(pCusData);

	if (errorCode == E_SxbOK)
	{
		pMainWindow->iLiveLogout();
	}
	else
	{
		pMainWindow->switchLoginState(E_Login);
		ShowCodeErrorTips(errorCode, errorInfo, pMainWindow, FromBits("登出服务器出错"));
	}
}

void MainWindow::OnSxbCreateRoom(int errorCode, QString errorInfo, QVariantMap datamap, void* pCusData)
{
	MainWindow* pMainWindow = reinterpret_cast<MainWindow*>(pCusData);

	if (datamap.contains("roomnum"))
	{
		pMainWindow->m_curRoomInfo.info.roomnum = datamap.value("roomnum").toUInt();
	}
	if (datamap.contains("groupid"))
	{
		QString szRoomId = datamap.value("groupid").toString();
		assert(QString::number(pMainWindow->m_curRoomInfo.info.roomnum) == szRoomId);
	}

	if (errorCode == E_SxbOK)
	{
		pMainWindow->iLiveCreateRoom();
	}
	else
	{
		ShowCodeErrorTips(errorCode, errorInfo, pMainWindow, FromBits("创建房间出错"));
		pMainWindow->setUseable(true);
	}
}

void MainWindow::OnSxbReportroom(int errorCode, QString errorInfo, QVariantMap datamap, void* pCusData)
{
	MainWindow* pMainWindow = reinterpret_cast<MainWindow*>(pCusData);

	if (errorCode == E_SxbOK)
	{
		pMainWindow->sxbCreatorJoinRoom();//随心播服务器要求，创建房间、上报房间信息、主播还需要上报一次自己进入房间;
	}
	else
	{
		ShowCodeErrorTips(errorCode, errorInfo, pMainWindow, FromBits("上报房间失败"));
	}
}

void MainWindow::OnSxbCreatorJoinRoom(int errorCode, QString errorInfo, QVariantMap datamap, void* pCusData)
{
	MainWindow* pMainWindow = reinterpret_cast<MainWindow*>(pCusData);

	if (errorCode == E_SxbOK)
	{
		pMainWindow->m_pLive->StartTimer();//启动计时器
	}
	else
	{
		ShowCodeErrorTips(errorCode, errorInfo, pMainWindow, FromBits("主播创建房间失败"));
	}
}

void MainWindow::OnSxbRoomList(int errorCode, QString errorInfo, QVariantMap datamap, void* pCusData)
{
	MainWindow* pMainWindow = reinterpret_cast<MainWindow*>(pCusData);

	if (errorCode != E_SxbOK)
	{
		//iLiveLog_e("suixinbo", "SxbRoomList Failed.");
		return;
	}

	if (datamap.contains("total"))
	{
		int nTotal = datamap.value("total").toInt();
		pMainWindow->m_nTotalPage = nTotal / OnePageCout;
		if (nTotal%OnePageCout != 0)
		{
			pMainWindow->m_nTotalPage += 1;
		}
		pMainWindow->updatePageNum();
	}

	if (datamap.contains("rooms"))
	{
		QVariantList roomslist = datamap.value("rooms").toList();
		QVector<Room>& showRooms = pMainWindow->m_showRooms;
		showRooms.clear();
		for (int i = 0; i < roomslist.size(); ++i)
		{
			QVariantMap roommap = roomslist[i].toMap();
			Room theroom;

			if (roommap.contains("uid"))
			{
				theroom.szId = roommap.value("uid").toString();
			}
			if (roommap.contains("info"))
			{
				QVariantMap infomap = roommap.value("info").toMap();
				if (infomap.contains("title"))
				{
					theroom.info.title = infomap.value("title").toString();
				}
				if (infomap.contains("roomnum"))
				{
					theroom.info.roomnum = infomap.value("roomnum").toInt();
				}
				if (infomap.contains("type"))
				{
					theroom.info.type = infomap.value("type").toString();
				}
				if (infomap.contains("groupid"))
				{
					theroom.info.groupid = infomap.value("groupid").toString();
				}
				if (infomap.contains("cover"))
				{
					theroom.info.cover = infomap.value("cover").toString();
					if (!theroom.info.cover.isEmpty())
					{
						pMainWindow->picDown(getFileNameByUrl(theroom.info.cover), theroom.info.cover);
					}
				}
				if (infomap.contains("thumbup"))
				{
					theroom.info.thumbup = infomap.value("thumbup").toInt();
				}
				if (infomap.contains("memsize"))
				{
					theroom.info.memsize = infomap.value("memsize").toInt();
				}
			}
			showRooms.push_back(theroom);
		}
		pMainWindow->updateRoomList();
	}
}

void MainWindow::picDown(QString userId, QString url)
{
	PicDownHelper::getPic(url, userId + ".jpg", OnPicDown, this);
}

void MainWindow::OnPicDown(int errorCode, QString errorInfo, QString picPath, void* pCusData)
{
	MainWindow* pMainWindow = reinterpret_cast<MainWindow*>(pCusData);

	if (errorCode == E_PicDownOK)
	{
		pMainWindow->updateRoomList();
	}
	else
	{
		ShowCodeErrorTips(errorCode, errorInfo, pMainWindow);
	}
}

void MainWindow::iLiveLogin()
{
	GetILive()->login(m_szUserId.toStdString().c_str(), m_szUserSig.toStdString().c_str(), OniLiveLoginSuccess, OniLiveLoginError, this);
}

void MainWindow::iLiveLogout()
{
	GetILive()->logout(OniLiveLogoutSuccess, OniLiveLogoutError, this);
}

void MainWindow::iLiveCreateRoom()
{
	GetILive()->setLocalVideoCallBack(Live::OnLocalVideo, m_pLive);
	GetILive()->setDeviceOperationCallback(Live::OnDeviceOperation, m_pLive);
	GetILive()->setDeviceDetectCallback(Live::OnDeviceDetect, m_pLive);
	iLiveRoomOption roomOption;
	roomOption.audioCategory = AUDIO_CATEGORY_MEDIA_PLAY_AND_RECORD;//互动直播场景
	roomOption.roomId = m_curRoomInfo.info.roomnum;
	roomOption.authBits = AUTH_BITS_DEFAULT;
	roomOption.controlRole = LiveMaster;
	roomOption.roomDisconnectListener = Live::OnRoomDisconnect;
	roomOption.memberStatusListener = Live::OnMemStatusChange;
	//roomOption.qualityParamCallback = Live::OnQualityParamCallback;
	roomOption.data = m_pLive;
	GetILive()->createRoom(roomOption, OniLiveCreateRoomSuc, OniLiveCreateRoomErr, this);
}

void MainWindow::OniLiveLoginSuccess(void* data)
{
	MainWindow* pThis = reinterpret_cast<MainWindow*>(data);
	pThis->switchLoginState(E_Login);

	//设置水印
	//GetILive()->setWaterMark("waterMark.png", 0.2f, 0.2f, 0.4f);
}

void MainWindow::OniLiveLoginError(int code, const char *desc, void* data)
{
	MainWindow* pThis = reinterpret_cast<MainWindow*>(data);
	pThis->switchLoginState(E_Logout);
	ShowCodeErrorTips(code, desc, pThis, "iLive Login Error");
}

void MainWindow::OniLiveLogoutSuccess(void* data)
{
	MainWindow* pThis = reinterpret_cast<MainWindow*>(data);
	pThis->switchLoginState(E_Logout);
}

void MainWindow::OniLiveLogoutError(int code, const char *desc, void* data)
{
	MainWindow* pThis = reinterpret_cast<MainWindow*>(data);
	pThis->switchLoginState(E_Login);
	ShowCodeErrorTips(code, desc, pThis, "iLive Logout Error");
}

void MainWindow::OniLiveCreateRoomSuc(void* data)
{
	MainWindow* pThis = reinterpret_cast<MainWindow*>(data);
	pThis->m_pLive->setRoomID(pThis->m_curRoomInfo.info.roomnum);
	pThis->m_pLive->setRoomUserType(E_RoomUserCreator);
	pThis->m_pLive->show();
	pThis->m_curRoomInfo.szId = pThis->m_szUserId;
	pThis->m_curRoomInfo.info.thumbup = 0;
	pThis->sxbReportroom();//上报房间信息给随心播业务侧服务器
}

void MainWindow::OniLiveCreateRoomErr(int code, const char *desc, void* data)
{
	MainWindow* pThis = reinterpret_cast<MainWindow*>(data);
	ShowCodeErrorTips(code, desc, pThis, "Create iLive Room Error.");
	pThis->setUseable(true);
}

void MainWindow::OnStartDeviceTestSuc(void* data)
{
	GetILive()->setDeviceOperationCallback(DeviceTest::OnDeviceOperation, g_pMainWindow->m_pDeviceTest);
	GetILive()->setLocalVideoCallBack(DeviceTest::OnCameraVideo, g_pMainWindow->m_pDeviceTest);
	GetILive()->setDeviceDetectCallback(DeviceTest::OnDeviceDetect, g_pMainWindow->m_pDeviceTest);
	g_pMainWindow->m_pDeviceTest->show();
}

void MainWindow::OnStartDeviceTestErr(int code, const char *desc, void* data)
{
	ShowCodeErrorTips(code, desc, g_pMainWindow);
}

void MainWindow::closeEvent(QCloseEvent* event)
{
	if (m_pLive->isVisible())
	{
		event->ignore();//阻止窗口关闭
		QMessageBox::warning(this, "Tips", FromBits("请先退出直播间"));
	}
	else
	{
		event->accept();
		saveConfig();
		GetILive()->release();
	}
	if (m_pDeviceTest->isVisible())
	{
		m_pDeviceTest->close();
	}
	if (m_pRegister->isVisible())
	{
		m_pRegister->close();
	}
	PicDownHelper::clearPicCache();
}

void MainWindow::onBtnLogin()
{
	switch (m_eLoginState)
	{
	case E_Logout:
		switchLoginState(E_Logining);
		sxbLogin();
		break;
	case E_Login:
		sxbLogout();
		break;
	default:
		break;
	}
}

void MainWindow::onBtnRegister()
{
	m_pRegister->show();
}

void MainWindow::onBtnBeginLive()
{
	if (m_eLoginState != E_Login)
	{
		ShowErrorTips(FromBits("请先登录"), this);
		return;
	}
	m_curRoomInfo.info.title = m_ui.edBLTitle->text();
	setUseable(false);
	sxbCreateRoom();
}

void MainWindow::onBtnRefreshLiveList()
{
	if (m_eLoginState != E_Login)
	{
		ShowErrorTips(FromBits("请先登录"), this);
		return;
	}

	m_nCurrentPage = 0;
	sxbRoomList();
}

void MainWindow::onBtnLastPage()
{
	if (m_eLoginState != E_Login)
	{
		ShowErrorTips(FromBits("请先登录"), this);
		return;
	}

	m_nCurrentPage--;
	sxbRoomList();
}

void MainWindow::onBtnNextPage()
{
	if (m_eLoginState != E_Login)
	{
		ShowErrorTips(FromBits("请先登录"), this);
		return;
	}

	m_nCurrentPage++;
	sxbRoomList();
}

void MainWindow::on_btnDeviceTest_clicked()
{
	GetILive()->startDeviceTest(OnStartDeviceTestSuc, OnStartDeviceTestErr, this);
}


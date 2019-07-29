#ifndef Live_h_
#define Live_h_

#include "ui_Live.h"
#include "VideoRender.h"
#include "WndList.h"

#define MaxShowMembers 50

//�������û�����
enum E_RoomUserType
{
	E_RoomUserInvalid = -1,//�û�������
	E_RoomUserWatcher, //����
	E_RoomUserCreator, //����
	E_RoomUserJoiner,  //������
};

//�����Ա����
struct RoomMember
{
	QString szID;//ID
	E_RoomUserType userType;//�û�����
};

//ֱ������
class Live : public QDialog
{
	Q_OBJECT
public:
	Live(QWidget * parent = 0, Qt::WindowFlags f = 0);

	void setRoomID(int roomID);//����ֱ�������
	void setRoomUserType(E_RoomUserType userType);//����ֱ�����䵱ǰ�û�����
	void ChangeRoomUserType();//�ı䵱ǰ�û�����

	void dealMessage(const Message& message);//������Ϣ
	void parseCusMessage(const std::string& sender,std::string msg);//�����Զ�����Ϣ
	void dealCusMessage(const std::string& sender, int nUserAction, QString szActionParam);//�����Զ�����Ϣ
	
	void StartTimer();//����������ʱ��
	void stopTimer();//ֹͣ������ʱ��
	void onMixStream(std::string streamCode);//�����ɹ���ˢ���������Ϣ

	static void OnMemStatusChange(E_EndpointEventId event_id, const Vector<String> &ids, void* data);//�����Ա�¼�֪ͨ�ص�
	static void OnRoomDisconnect(int reason, const char *errorinfo, void* data);//�����˳�����ص�
	static void OnDeviceDetect(void* data);//�豸�β�����ص�
	static void OnLocalVideo(const LiveVideoFrame* video_frame, void* custom_data);//������Ƶ��Ⱦ�ص���ִ�б�����Ƶ����Ļ������Ⱦ��
	static void OnRemoteVideo(const LiveVideoFrame* video_frame, void* custom_data);//Զ�̻�����Ƶ��Ⱦ�ص���ִ��Զ�̻�����Ⱦ��
	static void OnMessage( const Message& msg, void* data );//��Ϣ�����ص�
	static void OnDeviceOperation(E_DeviceOperationType oper, int retCode, void* data);//�豸�����ص�
	static void OnQualityParamCallback(const iLiveRoomStatParam& param, void* data);//ֱ����������ص�

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
	void OnBtnSendGroupMsg();//���� ���
	void OnBtnStartPushStream();//��·����-��ʼ���� ���
	void OnBtnStopPushStream();//��·����-ֹͣ���� ���
	void OnBtnPraise();//����-Ϊ�������� ���
	void OnBtnSelectMediaFile();//�ļ�����-ѡ�� ���
	void OnBtnPlayMediaFile();//�ļ�����-���� ���
	void OnBtnStopMediaFile();//�ļ�����-ֹͣ ���
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
	void OnHeartBeatTimer();//���� ������ʱ��
	void OnFillFrameTimer();
	void OnPlayMediaFileTimer();//���� �Զ��ز���ʱ��
	void OnMemberListMenu(QPoint point);//���� �˵�ѡ��
	void OnActInviteInteract();//�˵�-���� ��� 
	void OnActCancelInteract();//�˵�-�Ͽ� ���
	void on_btnMix_clicked();//��ʼ�������


protected:
	void closeEvent(QCloseEvent* event) override;//�رմ���

private:
	//�Զ���˽�к���
	void updateCameraList();//��������ͷ�б�QComboBox
	VideoRender* findVideoRender(std::string szIdentifier);//������Ⱦ��ID������Ƶ��Ⱦ����Զ�̻��棩
	VideoRender* getFreeVideoRender(std::string szIdentifier);//��Զ�̻�����Ⱦ�������л�ȡ���õ���Ⱦ��
	void freeAllCameraVideoRender();//�ͷ�������Ƶ��Ⱦ�������ػ��棬��·��Ƶ��Զ�̻��棩

	void addMsgLab(QString msg);//�����������Ϣ

	void updateMemberList();//����ֱ�����û��б�
	void updateMsgs();//�������촰����ʾ��Ϣ

	void updateCameraGB();//��������ͷ��Ͽ�QGroupBox
	void updatePlayerGB();//������������Ͽ�QGroupBox
	void updateExternalCaptureGB();//�����Զ���ɼ���Ͽ�QGroupBox
	void updateMicGB();//������˷���Ͽ�QGroupBox
	void updateScreenShareGB();//������Ļ������Ͽ�QGroupBox
	void updateSystemVoiceInputGB();//����ϵͳ�����ɼ�������Ͽ�QGroupBox
	void updateMediaFilePlayGB();//�����ļ�������Ͽ�QGroupBox
	void updatePushStreamGB();//������·������Ͽ�QGroupBox
	
	void updatePlayMediaFileProgress();//���²��ű�����Ƶ�ؼ������趨
	void doStartPlayMediaFile();//��ʼ�����ļ�
	void doPausePlayMediaFile();//��ͣ�����ļ�
	void doResumePlayMediaFile();//���������ļ�
	void doStopPlayMediaFile();//ֹͣ�����ļ�

	void doAutoStopPushStream();//�Զ�ֹͣ����

	//�豸�����ص�
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

	//����㺯��
	void sendInviteInteract();//��������ͨ���ڷ�����������
	void sendCancelInteract();//�����������еĹ��ڷ�����������
	static void OnSendInviteInteractSuc(void* data);//����ɹ��ص�
	static void OnSendInviteInteractErr(const int code, const char *desc, void* data);//����ʧ�ܻص�

	void acceptInteract();//��ͨ���ڽ�����������
	void refuseInteract();//��ͨ���ھܾ���������
	void OnAcceptInteract();//��������

	void exitInteract();//�������ִ�����������Ķ�������
	void OnExitInteract();//�������
	void onMixTextClick(const QUrl &url);

	void sendQuitRoom();//���������˳���������

	//���Ĳ�������������غ���
	void sxbCreatorQuitRoom();//������������������뿪������Ϣ
	void sxbWatcherOrJoinerQuitRoom();//����������͹��ڻ��������뿪������Ϣ
	void sxbHeartBeat();//���������������
	void sxbRoomIdList();//�������������ȡ�����Ա�б�����
	static void OnSxbCreatorQuitRoom(int errorCode, QString errorInfo, QVariantMap datamap, void* pCusData);//�����˳�����ص�
	static void OnSxbWatcherOrJoinerQuitRoom(int errorCode, QString errorInfo, QVariantMap datamap, void* pCusData);//���ڻ��������˳�����ص�
	static void OnSxbHeartBeat(int errorCode, QString errorInfo, QVariantMap datamap, void* pCusData);//�����ص�
	static void OnSxbRoomIdList(int errorCode, QString errorInfo, QVariantMap datamap, void* pCusData);//��Ա�б�����ص�

	//iLiveSDK��غ���
	void iLiveQuitRoom();//�˳�ֱ������
	void iLiveChangeRole(const std::string& szControlRole);//�����������ı��ɫ
	int iLiveSetSkinSmoothGrade(int grade);//�������ճ̶�
	int iLiveSetSkinWhitenessGrade(int grade);//�������׳̶�
	static void OnQuitRoomSuc(void* data);//�˳�ֱ������ɹ��ص�
	static void OnQuitRoomErr(int code, const char *desc, void* data);//�˳�ֱ������ʧ�ܻص�
	static void OnChangeRoleSuc(void* data);//���������ı��ɫ�ɹ��ص�
	static void OnChangeRoleErr(int code, const char *desc, void* data);//���������ı��ɫʧ�ܻص�

	static void OnSendGroupMsgSuc(void* data);//Ⱥ����Ϣ�ɹ��ص�
	static void OnSendGroupMsgErr(int code, const char *desc, void* data);//Ⱥ����Ϣʧ�ܻص�

	static void OnStartPushStreamSuc(PushStreamRsp &value, void *data);//��ʼ�����ɹ��ص�
	static void OnStartPushStreamErr(int code, const char * desc, void* data);//��ʼ����ʧ�ܻص�

	static void OnStopPushStreamSuc(void* data);//ֹͣ�����ɹ��ص�
	static void OnStopPushStreamErr(int code, const char *desc, void* data);//ֹͣ����ʧ�ܻص�
	
private:
	Ui::Live		m_ui;
	
	E_RoomUserType  m_userType;//�����û�����

	VideoRender*	m_pLocalCameraRender;//���ػ�����Ƶ����
	VideoRender*	m_pScreenShareRender;//��·��Ƶ����

	Vector< Pair<String/*id*/, String/*name*/> > m_cameraList;

	std::vector<VideoRender*>	m_pRemoteVideoRenders;//Զ�̻��洰�ڼ���

	int					m_nRoomSize;
	QVector<RoomMember> m_roomMemberList;

	QTimer*			m_pTimer;//������ʱ��
	QTimer*			m_pFillFrameTimer;
	QTimer*			m_pPlayMediaFileTimer;//�����ļ����ż�ʱ������ʱ�Զ��ز�
	
	int				m_nCurSelectedMember;
	QMenu*			m_pMenuInviteInteract;
	QMenu*			m_pMenuCancelInteract;

	QString				m_inputRecordName;
	PushStreamOption	m_pushOpt;
	uint64				m_channelId;
	std::list<LiveUrl>	m_pushUrls;

	int32	m_x0;//��Ļ��������x0
	int32	m_y0; //��Ļ����������y0
	int32	m_x1; //��Ļ��������x1
	int32	m_y1; //��Ļ��������y1
	uint32	m_fps;//��Ļ������֡��,ȡֵ��Χ[1,10]��ע��: �˲����������壬sdk����������������̬����fps;

	int64	m_n64Pos;//������Ƶ��ǰ����ʱ��
	int64	m_n64MaxPos;//������Ƶ��ʱ��

	bool	m_bRoomDisconnectClose;

	bool	m_bRecording;
	bool	m_bPushing;//��ǰ�Ƿ�ʼ����

	QString	m_szMsgs;//���촰���ı�
	int mRoomId;//�����
};

#endif//Live_h_
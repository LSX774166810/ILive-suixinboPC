#ifndef NetworkHelper_h_
#define NetworkHelper_h_

#define LimitTimeOut 5000 //����ʱʱ��(����)

enum E_NetworkReply
{
	E_NetOK,
	E_NetTimeOut,//��ʱ
	E_NetReplyError,
};

//���պ����ص�
typedef void (*ReceiveFun)(int errCode, const QByteArray& bytes, void* pCusData);

//������Ϣ���ͺͽ���
class NetworkHelper : public QObject
{
	Q_OBJECT
public:
	static void get(const QString& url, ReceiveFun receiver, void* data, int timeout = LimitTimeOut);
	static void post(const QString& url, const QString& content, ReceiveFun receiver, void* data, int timeout = LimitTimeOut);

private slots:
	void OnReplyFinished(QNetworkReply* reply);
	void OnTimer();

private:
	NetworkHelper(QString url, QString content, ReceiveFun receiver, void* pCusData, int timeout);
	~NetworkHelper();
	
	void excuteGet();
	void excutePost();

private:
	QNetworkAccessManager* m_pNetworkAccessManager;
	QTimer*		m_pTimer;//��ʱ��

	QString		m_url;//Ŀ��url
	QString		m_content;//����������	
	ReceiveFun  m_receiver;
	void*		m_pCusdata;//����ָ��
	int			m_timeout;//��ʱʱ�� ms
};

#endif//NetworkHelper_h_
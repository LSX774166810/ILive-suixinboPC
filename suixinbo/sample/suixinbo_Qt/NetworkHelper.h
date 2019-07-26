#ifndef NetworkHelper_h_
#define NetworkHelper_h_

#define LimitTimeOut 5000 //请求超时时间(毫秒)

enum E_NetworkReply
{
	E_NetOK,
	E_NetTimeOut,//超时
	E_NetReplyError,
};

//接收函数回调
typedef void (*ReceiveFun)(int errCode, const QByteArray& bytes, void* pCusData);

//网络信息发送和接受
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
	QTimer*		m_pTimer;//计时器

	QString		m_url;//目标url
	QString		m_content;//待发送数据	
	ReceiveFun  m_receiver;
	void*		m_pCusdata;//对象指针
	int			m_timeout;//超时时间 ms
};

#endif//NetworkHelper_h_
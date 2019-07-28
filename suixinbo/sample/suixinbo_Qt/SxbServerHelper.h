#pragma once
#include "NetworkHelper.h"

enum E_SxbServerRelpy
{
	E_SxbOK,
	E_SxbTimeOut,//超时
	E_SxbNetReplyError,
	E_SxbJsonParseError,//Json解析错误
};

typedef void (*SxbRecFun)(int errorCode, QString errorInfo, QVariantMap datamap, void* pCusData);

//获取Json信息
class SxbServerHelper
{
public:
	static void request( QVariantMap varmap, QString svc, QString cmd, SxbRecFun receiver, void* data );//发送请求

private:
	SxbServerHelper(const QString& url, const QString& content, SxbRecFun receiver, void* data);
	~SxbServerHelper();
	void doRequest();//执行发送请求

	static void OnNetworkReply(int errCode, const QByteArray& bytes, void* pCusData);

private:
	QString	  m_url;//目标url
	QString	  m_content;//待发送信息
	SxbRecFun m_receiver;
	void*	  m_pCusdata;
};


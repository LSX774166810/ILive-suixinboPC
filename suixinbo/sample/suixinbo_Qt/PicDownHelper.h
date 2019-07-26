#pragma once
#include "NetworkHelper.h"

enum E_PicDownRelpy
{
	E_PicDownOK,
	E_PicDownTimeOut,//超时
	E_PicDownNetReplyError,
	E_PicDownUrlOrFileNameNULL,//传入的url或者fileName是空
	E_PicDownOpenFileError,//打开文件失败
};

typedef void (*PicDownFun)(int errorCode, QString errorInfo, QString picPath, void* pCusData);

//图片下载
class PicDownHelper
{
public:
	static void setPicPath(QString path);
	static QString getPicPath();
	static void getPic( const QString& url, const QString& fileName, PicDownFun receiver, void* data );//下载图片
	static bool clearPicCache();//完全删除ms_picPath文件夹

private:
	PicDownHelper(const QString& url, const QString& fileName, PicDownFun receiver, void* data);
	~PicDownHelper();

	void doGetPic();//执行下载
	static void OnNetworkReply(int errCode, const QByteArray& bytes, void* pCusData);//网络连接结束后回调
	static bool	createTempDir();//以ms_picPath创建文件夹

private:
	static QString ms_picPath;//下载的图片保存路径

	QString		m_url;//图片url
	QString		m_fileName;//图片文件名称
	PicDownFun	m_receiver;
	void*		m_pCusdata;//对象指针
};


#pragma once
#include "NetworkHelper.h"

enum E_PicDownRelpy
{
	E_PicDownOK,
	E_PicDownTimeOut,//��ʱ
	E_PicDownNetReplyError,
	E_PicDownUrlOrFileNameNULL,//�����url����fileName�ǿ�
	E_PicDownOpenFileError,//���ļ�ʧ��
};

typedef void (*PicDownFun)(int errorCode, QString errorInfo, QString picPath, void* pCusData);

//ͼƬ����
class PicDownHelper
{
public:
	static void setPicPath(QString path);
	static QString getPicPath();
	static void getPic( const QString& url, const QString& fileName, PicDownFun receiver, void* data );//����ͼƬ
	static bool clearPicCache();//��ȫɾ��ms_picPath�ļ���

private:
	PicDownHelper(const QString& url, const QString& fileName, PicDownFun receiver, void* data);
	~PicDownHelper();

	void doGetPic();//ִ������
	static void OnNetworkReply(int errCode, const QByteArray& bytes, void* pCusData);//�������ӽ�����ص�
	static bool	createTempDir();//��ms_picPath�����ļ���

private:
	static QString ms_picPath;//���ص�ͼƬ����·��

	QString		m_url;//ͼƬurl
	QString		m_fileName;//ͼƬ�ļ�����
	PicDownFun	m_receiver;
	void*		m_pCusdata;//����ָ��
};


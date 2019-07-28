#pragma once

#include "MixStreamHelper.h"
#include <string>
#include <vector>
#include <utility>
#include <functional>




//������
class MixStreamHelper
{
	
public:
	/**
	@brief ����һ���������󣬻����������https://www.qcloud.com/document/product/267/8832
	@param [in] streams ��Ҫ�������Ĭ�ϵ�һ��Ϊ����
	@param [in] sig ��Ѷ��ֱ���������̨�����õ�API��Ȩkey
	@param [in] roomId �����
	@param [in] data �Զ�������
	*/
	MixStreamHelper(std::vector<std::pair<std::string, bool>> streams, const QString sig, int roomId, void *data);
	void doRequest();
	
private:
	QString genSign(QString key, QString time);//����md5
	QString genContent(std::vector<std::pair<std::string, bool>> ids);//��streamsת��Ϊ����������
	static void OnNetworkReply(int errCode, const QByteArray& bytes, void* pCusData);
	std::string genStreamCode(std::string id, bool aux, int roomId, std::string code);//��ȡֱ����
	int getTemplate();

private:
	std::vector<std::pair<std::string, bool>> m_streams;//��Ƶ��
	unsigned bigIndex_;
	int m_roomID;//�����
	QString	  m_url;//Ŀ��url
	QString	  m_content;//����������
	SxbRecFun m_receiver;
	void*	  m_pCusdata;
	static std::string outCode;
	 
	
};


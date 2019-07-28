#pragma once

#include "MixStreamHelper.h"
#include <string>
#include <vector>
#include <utility>
#include <functional>




//混流器
class MixStreamHelper
{
	
public:
	/**
	@brief 创建一个混流请求，混流规则详见https://www.qcloud.com/document/product/267/8832
	@param [in] streams 需要混的流，默认第一个为底流
	@param [in] sig 腾讯云直播管理控制台中设置的API鉴权key
	@param [in] roomId 房间号
	@param [in] data 自定义数据
	*/
	MixStreamHelper(std::vector<std::pair<std::string, bool>> streams, const QString sig, int roomId, void *data);
	void doRequest();
	
private:
	QString genSign(QString key, QString time);//计算md5
	QString genContent(std::vector<std::pair<std::string, bool>> ids);//将streams转换为待发送数据
	static void OnNetworkReply(int errCode, const QByteArray& bytes, void* pCusData);
	std::string genStreamCode(std::string id, bool aux, int roomId, std::string code);//获取直播码
	int getTemplate();

private:
	std::vector<std::pair<std::string, bool>> m_streams;//视频流
	unsigned bigIndex_;
	int m_roomID;//房间号
	QString	  m_url;//目标url
	QString	  m_content;//待发送数据
	SxbRecFun m_receiver;
	void*	  m_pCusdata;
	static std::string outCode;
	 
	
};


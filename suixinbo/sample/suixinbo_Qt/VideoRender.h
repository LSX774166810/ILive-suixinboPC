#ifndef VideoRender_h_
#define VideoRender_h_

//视频渲染器
class VideoRender : public QWidget
{
	Q_OBJECT
public:
	VideoRender(QWidget *parent = NULL);
	~VideoRender();

	bool isFree();
	String getIdentifier();
	void setView(String identifier, E_VideoSrc type);//添加视频显示
	void setBackgroundColor(uint32 argb);
	void doRender(const LiveVideoFrame* frame);//渲染帧
	void remove();//清空所有渲染视频

protected:
	void showEvent(QShowEvent *event) override;
	void paintEvent(QPaintEvent *event) override;
	void mouseDoubleClickEvent(QMouseEvent * event) override;

private:
	void enterFullScreen();	//进入全屏
	void exitFullScreen();	//退出全屏

private:
	iLiveRootView*	m_pRootView;//渲染器
	String			m_identifier;//渲染器识别码 ID
	bool			m_bFree;//渲染器是否可用
	uint32			m_bgColor;//背景颜色

	QWidget*		m_pParentWidget;//父窗口
	QRect			m_Rect;//窗口大小
};

#endif //VideoRender_h_

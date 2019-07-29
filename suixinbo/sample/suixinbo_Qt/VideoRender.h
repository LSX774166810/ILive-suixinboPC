#ifndef VideoRender_h_
#define VideoRender_h_

//��Ƶ��Ⱦ��
class VideoRender : public QWidget
{
	Q_OBJECT
public:
	VideoRender(QWidget *parent = NULL);
	~VideoRender();

	bool isFree();
	String getIdentifier();
	void setView(String identifier, E_VideoSrc type);//�����Ƶ��ʾ
	void setBackgroundColor(uint32 argb);
	void doRender(const LiveVideoFrame* frame);//��Ⱦ֡
	void remove();//���������Ⱦ��Ƶ

protected:
	void showEvent(QShowEvent *event) override;
	void paintEvent(QPaintEvent *event) override;
	void mouseDoubleClickEvent(QMouseEvent * event) override;

private:
	void enterFullScreen();	//����ȫ��
	void exitFullScreen();	//�˳�ȫ��

private:
	iLiveRootView*	m_pRootView;//��Ⱦ��
	String			m_identifier;//��Ⱦ��ʶ���� ID
	bool			m_bFree;//��Ⱦ���Ƿ����
	uint32			m_bgColor;//������ɫ

	QWidget*		m_pParentWidget;//������
	QRect			m_Rect;//���ڴ�С
};

#endif //VideoRender_h_

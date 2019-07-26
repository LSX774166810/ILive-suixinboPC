#ifndef WndList_h_
#define WndList_h_

#include "ui_WndList.h"

//请选择要分享的窗口
class WndList : public QDialog
{
	Q_OBJECT
public:
	static HWND GetSelectWnd(QWidget* parent = NULL);

private:
	WndList(QWidget *parent = 0, Qt::WindowFlags f = 0);

	//刷新列表liWndList显示信息
	int Refresh();

private slots:
	void on_btnOK_clicked();
	void on_btnCancel_clicked();
	void on_liWndList_itemDoubleClicked(QListWidgetItem* item);
	
private:
	static int									ms_nLastIndex;
	Vector< Pair<HWND/*id*/, String/*name*/> >	m_wndList;
	HWND										m_curhwnd;
	Ui::WndList									m_ui;
};

#endif //WndList_h_
#ifndef Util_h_
#define Util_h_

//显示提示框
void ShowTips( const QString& title, const QString& desc, QWidget* parent = NULL );

//显示成功提示框
void ShowSucTips( const QString& desc, QWidget* parent = NULL );

//显示错误提示框
void ShowErrorTips( const QString& desc, QWidget* parent = NULL );

//显示错误码提示框
void ShowCodeErrorTips( const int code, const QString& desc, QWidget* parent = NULL, const QString& title = "Error" );

//通过url获取文件名
QString getFileNameByUrl(QString szUrl);

//将秒级时间转换为本地时间 hh:mm:ss
QString getTimeStrBySeconds(const int64 sec);

#endif//Util_h_

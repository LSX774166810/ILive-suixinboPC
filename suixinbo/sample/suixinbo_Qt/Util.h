#ifndef Util_h_
#define Util_h_

//��ʾ��ʾ��
void ShowTips( const QString& title, const QString& desc, QWidget* parent = NULL );

//��ʾ�ɹ���ʾ��
void ShowSucTips( const QString& desc, QWidget* parent = NULL );

//��ʾ������ʾ��
void ShowErrorTips( const QString& desc, QWidget* parent = NULL );

//��ʾ��������ʾ��
void ShowCodeErrorTips( const int code, const QString& desc, QWidget* parent = NULL, const QString& title = "Error" );

//ͨ��url��ȡ�ļ���
QString getFileNameByUrl(QString szUrl);

//���뼶ʱ��ת��Ϊ����ʱ�� hh:mm:ss
QString getTimeStrBySeconds(const int64 sec);

#endif//Util_h_

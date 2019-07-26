#include "stdafx.h"
#include "Register.h"

Register::Register( QWidget * parent /*= 0*/, Qt::WindowFlags f /*= 0*/ )
	:QDialog(parent, f)
{
	m_ui.setupUi(this);

	QRegExp regExp("[a-zA-Z0-9]+$");
	m_ui.leUserName->setValidator( new QRegExpValidator(regExp, m_ui.leUserName) );
	connect( m_ui.btnRegister, SIGNAL(clicked()), this, SLOT(onBtnRegister()) );
}

void Register::onBtnRegister()
{
	sxbRegister();
}

bool Register::isDigitStr( const QString& str )
{
	bool bDigitStr = true;
	for (int i = 0; i<str.length(); ++i)
	{
		if (str[i] < '0' || str[i] > '9' )
		{
			bDigitStr = false;
			break;
		}
	}
	return bDigitStr;
}

void Register::sxbRegister()
{
	QString szUserName = m_ui.leUserName->text();
	QString szPassword = m_ui.lePassword->text();
	if ( szUserName.isEmpty() || szPassword.isEmpty() )
	{
		ShowErrorTips( FromBits("�������û���������"), this );
		return;
	}
	if ( szUserName.length() < 4 )
	{
		ShowErrorTips( FromBits("�û������Ȳ���С��4"), this );
		return;
	}
	if (szPassword.length() < 8)
	{
		ShowErrorTips( FromBits("���볤�Ȳ���С��8"), this );
		return;
	}
	if ( isDigitStr(szUserName) )
	{
		ShowErrorTips( FromBits("�û�������Ϊ������"), this );
		return;
	}

	QVariantMap varmap;
	varmap.insert("id",szUserName);
	varmap.insert("pwd",szPassword);
	SxbServerHelper::request( varmap, "account", "regist", OnRegister, this);
}

void Register::OnRegister( int errorCode, QString errorInfo, QVariantMap datamap, void* pCusData )
{
	Register* pRegister = reinterpret_cast<Register*>(pCusData);
	
	if (errorCode==E_SxbOK)
	{
		ShowSucTips( "Register succeed!", pRegister );
		//pRegister->close();
	}
	else
	{
		ShowCodeErrorTips( errorCode, errorInfo, pRegister, FromBits("ע��ʧ��") );
	}
}

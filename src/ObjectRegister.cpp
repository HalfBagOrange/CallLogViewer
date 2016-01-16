#include <QCryptographicHash>
#include <QFile>
#include <QSettings>
#include <QTime>

#include "SeriseKey.h"
#include "ObjectRegister.h"

ObjectRegister::ObjectRegister(QObject * parent/* = 0*/)
:QObject(parent), m_timer(0), m_trialTime(0)
{
}

ObjectRegister* ObjectRegister::instance()
{
	static ObjectRegister inst;
	return &inst;
}

bool ObjectRegister::init()
{
	QByteArray text;
	QFile file(".register_key");
	if (file.exists())
	{
		file.open(QIODevice::ReadOnly);
		text = file.readLine(1024);
		file.close();
	}
	if (registerSuccessForever != softRegister(text, false))
	{
		timerEvent(NULL);
	}
	return true;
}

QString ObjectRegister::getSoftSeriseKey()
{
	QString seriseKey = GetSoftSeriseKey();
	QCryptographicHash hash(QCryptographicHash::Md5);
	hash.addData(seriseKey.toStdString().c_str());  //添加数据到加密哈希值
	QByteArray result_byte_array = hash.result();  //返回最终的哈希值
	QString md5 = result_byte_array.toHex();
	return md5;
}

ObjectRegister::RegisterResult ObjectRegister::softRegister(QString registerKey, bool save)
{
	QString seriseKey = getSoftSeriseKey();

	RegisterResult result = registerFail;
	m_trialTime = 7 * 24 * 60;
	if (registerKey == QCryptographicHash::hash(seriseKey.toStdString().c_str(), QCryptographicHash::Sha224).toHex()) //注册成功，可永久使用
	{
		if (0 != m_timer)
		{
			killTimer(m_timer);
			m_timer = 0;
		}
		m_trialTime = 0;
		result = registerSuccessForever;
	}
	else if (registerKey == QCryptographicHash::hash(seriseKey.toStdString().c_str(), QCryptographicHash::Sha1).toHex()) //注册成功,试用1个月
	{
		m_trialTime = 30 * 24 * 60;
		result = registerSuccessMonth;
	}
	else if (registerKey == QCryptographicHash::hash(seriseKey.toStdString().c_str(), QCryptographicHash::Md5).toHex()) //注册成功,试用1周
	{
		m_trialTime = 7 * 24 * 60;
		result = registerSuccessWeek;
	}
	
	if (registerSuccessForever != result && 0 == m_timer)
	{
		m_timer = startTimer(60 * 1000);
	}

	if (save && result != registerFail)
	{
		QFile regKeyFile(".register_key");
		regKeyFile.open(QIODevice::WriteOnly);
		regKeyFile.write(registerKey.toStdString().c_str());
		regKeyFile.flush();
		regKeyFile.close();
		emit signalSuccessRegister();
	}

	return result;
}

void ObjectRegister::timerEvent(QTimerEvent * event)
{
	QSettings reg("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Ext\\Stats\\{A6252761-8F44-4A25-9149-FAD1A3171B55}", QSettings::NativeFormat);
	Q_ASSERT(reg.contains("InstallTime"));

	qint64 installTime = QDateTime::fromString(reg.value("InstallTime").toString(), "dd-MM-yyyy hh:mm:ss").toMSecsSinceEpoch() / 1000;
	qint64 currentTime = QDateTime::currentMSecsSinceEpoch() / 1000;

	if (currentTime <= installTime || (currentTime - installTime) >= m_trialTime * 60)
	{
		emit signalTrialTimeout();
		killTimer(m_timer);
	}
}

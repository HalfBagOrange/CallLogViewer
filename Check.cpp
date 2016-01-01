#include <windows.h>
#include "Check.h"
#include "SeriseKey.h"
#include <QCryptographicHash>
#include <QFile>
#include <QSettings>
#include <QTime>

CCheck::CCheck(QObject * parent/* = 0*/)
:QObject(parent), m_timer(0), m_trialTime(0)
{
}

CCheck* CCheck::instance()
{
	static CCheck inst;
	return &inst;
}

bool CCheck::init()
{
	QByteArray text;
	QFile file(".register_key");
	if (file.exists())
	{
		file.open(QIODevice::ReadOnly);
		text = file.readLine(1024);
		file.close();
	}
	if (regSuccessForever != softRegister(text, false))
	{
		timerEvent(NULL);
	}
	return true;
}

QString CCheck::getSoftSeriseKey()
{
	QString seriseKey = GetSoftSeriseKey();
	QCryptographicHash hash(QCryptographicHash::Md5);
	hash.addData(seriseKey.toStdString().c_str());  //������ݵ����ܹ�ϣֵ
	QByteArray result_byte_array = hash.result();  //�������յĹ�ϣֵ
	QString md5 = result_byte_array.toHex();
	return md5;
}

CCheck::RegResult CCheck::softRegister(QString registerKey, bool save)
{
	QByteArray seriseKey(getSoftSeriseKey().toStdString().c_str());

	RegResult result = regFail;
	m_trialTime = 3 * 24 * 60;
	if (registerKey == seriseKey.toBase64(QByteArray::KeepTrailingEquals)) //ע��ɹ���������ʹ��
	{
		if (0 != m_timer)
		{
			killTimer(m_timer);
			m_timer = 0;
		}
		m_trialTime = 0;
		result = regSuccessForever;
	}
	else if (registerKey == seriseKey.toBase64(QByteArray::Base64Encoding)) //ע��ɹ�,����1����
	{
		m_trialTime = 30 * 24 * 60;
		result = regSuccessMonth;
	}
	else if (registerKey == seriseKey.toBase64(QByteArray::OmitTrailingEquals)) //ע��ɹ�,����1��
	{
		m_trialTime = 5 * 24 * 60;
		result = regSuccessWeek;
	}
	
	if (regSuccessForever != result && 0 == m_timer)
	{
		m_timer = startTimer(60 * 1000);
	}

	if (save && result != regFail)
	{
		QFile regKeyFile(".register_key");
		regKeyFile.open(QIODevice::WriteOnly);
		regKeyFile.write(registerKey.toStdString().c_str());
		regKeyFile.flush();
		regKeyFile.close();
		emit signalSuccessRegisted();
	}

	return result;
}

void CCheck::timerEvent(QTimerEvent * event)
{
	QSettings reg("HKEY_CURRENT_USER\\SOFTWARE\\CallLogViewer", QSettings::NativeFormat);
	Q_ASSERT(reg.contains("InstallTime"));

//	QDateTime dt = QDateTime::fromString(reg.value("InstallTime").toString(), "dd-MM-yyyy hh:mm:ss");
	qint64 installTime = QDateTime::fromString(reg.value("InstallTime").toString(), "dd-MM-yyyy hh:mm:ss").toMSecsSinceEpoch() / 1000;
	qint64 currentTime = QDateTime::currentMSecsSinceEpoch() / 1000;

	if (currentTime <= installTime || (currentTime - installTime) >= m_trialTime)
	{
		emit signalTrialTimeout();
	}
}

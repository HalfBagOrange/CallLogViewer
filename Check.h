#ifndef CHECK_H
#define CHECK_H

#include <QObject>

class CCheck : public QObject
{
	Q_OBJECT
	CCheck(QObject * parent = 0);

public:
	static CCheck* instance();

	QString getSoftSeriseKey();
	enum RegResult
	{
		regFail,
		regSuccessWeek,
		regSuccessMonth,
		regSuccessForever
	};
	RegResult softRegister(QString registerKey, bool save = true);

protected:
	void timerEvent(QTimerEvent * event);

signals:
	void signalTrialTimeout();
	void signalSuccessRegisted();

private:
	int m_timer;
	int m_trialTime;
};

#endif
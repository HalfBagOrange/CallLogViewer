#ifndef OBJECT_REGISTER_H
#define OBJECT_REGISTER_H

#include <QObject>

class ObjectRegister : public QObject
{
	Q_OBJECT
	ObjectRegister(QObject * parent = 0);

public:
	static ObjectRegister* instance();

	QString getSoftSeriseKey();
	enum RegisterResult
	{
		registerFail,
		registerSuccessWeek,
		registerSuccessMonth,
		registerSuccessForever
	};
	RegisterResult softRegister(QString registerKey, bool save = true);
	bool init();

protected:
	void timerEvent(QTimerEvent* event);

signals:
	void signalTrialTimeout();
	void signalSuccessRegister();

private:
	int m_timer;
	int m_trialTime;
};

#endif     //OBJECT_REGISTER_H
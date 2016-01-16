#include <QSqlQuery>
#include <QMessageBox>

#include <QSqlQueryModel>
#include "WindowAnalyseSingle.h"
#include "ObjectRegister.h"

WindowAnalyseSingle::WindowAnalyseSingle(QWidget *parent, Qt::WindowFlags flags)
:QMainWindow(parent, flags)
{
	ui.setupUi(this);

	setWindowState(Qt::WindowMaximized);

	QwtText axisTitle;
	QFont axisTitleFont;
	axisTitleFont.setPixelSize(16);

	axisTitle.setText(QStringLiteral("通话时间(秒)"));
	axisTitle.setFont(axisTitleFont);
	m_timePlot = new CallLogPlot(ui.qwtPlot, this);
	m_timePlot->getPlot()->enableAxis(QwtPlot::xBottom, false);
	m_timePlot->getPlot()->setAxisTitle(QwtPlot::yLeft, axisTitle);

	axisTitle.setText(QStringLiteral("通话次数(次)"));
	axisTitle.setFont(axisTitleFont);
	m_countPlot = new CallLogPlot(ui.qwtPlot_2, this);
	m_countPlot->getPlot()->setAxisTitle(QwtPlot::yLeft, axisTitle);

	ui.dateEdit->setCalendarPopup(true);
	ui.dateEdit_2->setCalendarPopup(true);

	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(slotDateTimeChanged()));
	connect(ObjectRegister::instance(), SIGNAL(signalTrialTimeout()), this, SLOT(slotTrialTimeout()));
	connect(ObjectRegister::instance(), SIGNAL(signalSuccessRegister()), this, SLOT(slotSuccessRegister()));

//	ui.comboBox_2->addItem(QStringLiteral("小时"));
//	ui.comboBox_2->addItem(QStringLiteral("天"));
	ui.label_4->setVisible(false);
	ui.comboBox_2->setVisible(false);

	QSqlQueryModel* modelPhoneNumber = new QSqlQueryModel(this);
	ui.comboBox->setModel(modelPhoneNumber);

	slotCallLogChanged();
}

WindowAnalyseSingle::~WindowAnalyseSingle()
{
}

void WindowAnalyseSingle::slotTrialTimeout()
{
	ui.comboBox->setEnabled(false);
	ui.comboBox_2->setEnabled(false);
	ui.dateEdit->setEnabled(false);
	ui.dateEdit_2->setEnabled(false);
	ui.pushButton->setEnabled(false);
	QMessageBox::about(this, QStringLiteral("警告"), QStringLiteral("超出试用期，请重新注册"));
}

void WindowAnalyseSingle::slotSuccessRegister()
{
	ui.comboBox->setEnabled(true);
	ui.comboBox_2->setEnabled(true);
	ui.dateEdit->setEnabled(true);
	ui.dateEdit_2->setEnabled(true);
	ui.pushButton->setEnabled(true);
}

void WindowAnalyseSingle::slotCallLogChanged()
{
	QSqlQuery query;

	query.exec("SELECT DISTINCT callnumber FROM CallLog");
	((QSqlQueryModel*)ui.comboBox->model())->setQuery(query);
	ui.comboBox->setCurrentIndex(0);

	query.exec("SELECT MIN(starttime) AS starttime FROM CallLog");
	query.first();
	QVariant minDate = query.value("starttime");
	if (minDate.toUInt() == 0)
	{
		minDate = QDateTime::currentDateTime().toTime_t();
	}
	query.exec("SELECT MAX(starttime) AS starttime FROM CallLog");
	query.first();
	QVariant maxDate = query.value("starttime");
	if (maxDate.toUInt() == 0)
	{
		maxDate = QDateTime::currentDateTime().toTime_t();
	}
	ui.dateEdit->setDate(QDateTime::fromTime_t(minDate.toUInt()).date());
	//	ui.dateEdit->setDateRange(QDateTime::fromTime_t(minDate.toUInt()).date(), QDateTime::fromTime_t(maxDate.toUInt()).date());
	ui.dateEdit_2->setDate(QDateTime::fromTime_t(maxDate.toUInt()).date());
	//	ui.dateEdit_2->setDateRange(QDateTime::fromTime_t(minDate.toUInt()).date(), QDateTime::fromTime_t(maxDate.toUInt()).date());
}

void WindowAnalyseSingle::slotDateTimeChanged()
{
	m_timePlot->clear();
	m_countPlot->clear();

	QString phoneNumber = ui.comboBox->currentText();
	QDateTime startDate = ui.dateEdit->dateTime();
	QDateTime endDate = ui.dateEdit_2->dateTime();

	for (; startDate <= endDate; startDate = startDate.addDays(1))
	{
		int count = 0;
		int time = 0;

		QString cmd;
		QTextStream(&cmd) << "SELECT * FROM CallLog WHERE starttime >= "
			<< startDate.toTime_t() << " AND starttime < " << startDate.addDays(1).toTime_t()
			<< " AND callnumber = " << phoneNumber;

		QSqlQuery query(cmd);
		while (query.next())
		{
			count++;
			time += query.value("duration").toInt() / 1000;
		}

		m_timePlot->addBar(time, startDate.date().toString("yyyy/MM/dd"));
		m_countPlot->addBar(count, startDate.date().toString("yyyy/MM/dd"));
	}

	m_timePlot->replot();
	m_countPlot->replot();
}

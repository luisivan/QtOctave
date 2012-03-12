#include "../basewidget.h"
#include "../config.h"
#include <QTextEdit>
#include <QLineEdit>
#include <QTime>
#include <QTimer>
#include <QCheckBox>
#include "../octave_connection.h"

/**
 * If you write a command in Terminal's command line, DynamicHelp shows help of commands.
 */
class Dynamic_help :public BaseWidget
{
	Q_OBJECT
	public:
	Dynamic_help(QString octave_path, QWidget *parent=0);
	~Dynamic_help();
	/** Sets QLineEdit from keyboard signals are listen.
	 */
	void setLineEdit(QLineEdit *lineedit);
	
	/**Sets session.*/
	void setSession(Session *session);
	BaseWidget *copyBaseWidget( QWidget * parent = 0 );
	
	private:
	QTextEdit *textEdit;
	QCheckBox *stop_help;
	OctaveConnection *octave;
	QTime time;
	QTimer timer;
	QString input_text, octave_path;
	
	/**Listen opened session tools.*/
	void listen_tools(WidgetType type);
	
	public slots:
	
	/**
	* Shows help when of text.
	 * @param text Text from QLineEdit.
	*/
	void textChanged ( const QString & text );
	
	/** Reads and shows Octave output.
	 */
	void readyReadStandardOutput (QString line);
	
	/** Updates shown help when QLineEdit changes happend.
	 */
	void update();
	
	/**New tool to listen.*/
	void newTool(WidgetType type, QObject *tool);
};


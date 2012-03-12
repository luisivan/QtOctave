#include "ui_general_menu.h"
#include "../config.h"
#include <QFont>

using namespace Ui;

/** Shows config window for Config/Generan configuration menu. This class is part of Operations module.
 */
class General_Menu: public QDialog
{
	Q_OBJECT
	
	public:
	General_Menu(QWidget *parent=0);
	Ui_GeneralMenu ui;
	void apply_config();
	
	private:
	QFont config_font;
	QColor foreground_color;
	QColor background_color;
	QColor error_color;
	QFont text_edit_font;
	void set_font_label();
	void set_color_label();
	
	public slots:
	void editor_select_button_callback();
	void help_path_select_pushButton_callback();
	void font_button_callback();
	void foreground_button_callback();
	void background_button_callback();
	void error_button_callback();
	void octaveDirRem_button_callback();
	void octaveDirAdd_button_callback();
	void octave_path_button_callback();
	void textEditFont_pushButton_callback();
	void easy_plot_path_button_callback();
};

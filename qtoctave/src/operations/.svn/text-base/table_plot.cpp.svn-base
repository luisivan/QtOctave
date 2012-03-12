#include "table_plot.h"
#include <QGroupBox>
#include <QTextStream>
#include <QClipboard>
#include <QBoxLayout>
#include <QLabel>
#include <QVariant>
#include <QPushButton>
#include <QVector>
#include <QMessageBox>
#include "../config.h"
#include "../table.h"

TablePlot::TablePlot(QWidget *parent, QTableView *table, ComplexNumberTableModel *model, Type type):
  QDialog(parent), table(table), model(model)
{
  setWindowIcon( QIcon(QString(ICON_PATH)+"/print") );

  this->type = type;

  //Title
  switch(type)
  {
  case PLOT:
    setWindowTitle("Plot");
    break;
  case SEMILOGY:
    setWindowTitle("Semilog y");
    break;
  case SEMILOGX:
    setWindowTitle("Semilog x");
    break;
  case LOGLOG:
    setWindowTitle("Log-Log");
    break;
  case POLAR:
    setWindowTitle("Polar");
    break;
  case BAR:
    setWindowTitle("Bar");
  }

  QVBoxLayout *vlayout = new QVBoxLayout;
  setLayout(vlayout);

  // Title
  QHBoxLayout *hlayout = new QHBoxLayout;
  vlayout->addLayout(hlayout);

  QLabel *title_label = new QLabel(tr("Title"), this);
  hlayout->addWidget(title_label);
  title_label->show();

  title_lineedit = new QLineEdit(this);
  hlayout->addWidget(title_lineedit);
  title_lineedit->show();

  //Rows or cols
  QGroupBox *rows_or_cols = new QGroupBox(tr("Use columns or rows"));

  use_cols = new QRadioButton(tr("Columns"));
  use_rows = new QRadioButton(tr("Rows"));
  use_cols->setChecked(true);

  QVBoxLayout *rows_or_cols_box = new QVBoxLayout;
  rows_or_cols_box->addWidget(use_cols);
  rows_or_cols_box->addWidget(use_rows);
  rows_or_cols_box->addStretch(1);

  rows_or_cols->setLayout(rows_or_cols_box);
  vlayout->addWidget(rows_or_cols);

  // X, Y (or angle and radio)
  hlayout = new QHBoxLayout();
  vlayout->addLayout(hlayout);

  QLabel *x_label = new QLabel(tr("x"), this);
  hlayout->addWidget(x_label);
  x_label->show();

  x_lineedit = new QLineEdit(this);
  hlayout->addWidget(x_lineedit);
  x_lineedit->show();

  hlayout = new QHBoxLayout();
  vlayout->addLayout(hlayout);

  QLabel *y_label = new QLabel(tr("y"), this);
  hlayout->addWidget(y_label);
  y_label->show();

  y_lineedit = new QLineEdit(this);
  hlayout->addWidget(y_lineedit);
  y_lineedit->show();

  if(type == POLAR)
  {
    x_label->setText(tr("Angle"));
    y_label->setText(tr("Radio"));
  }

  //X and Y Errors (or none)
  if(type != POLAR && type != BAR)
  {
    hlayout = new QHBoxLayout();
    vlayout->addLayout(hlayout);

    QLabel *x_error_label = new QLabel(tr("x Error"), this);
    hlayout->addWidget(x_error_label);
    x_error_label->show();

    x_error_lineedit = new QLineEdit(this);
    hlayout->addWidget(x_error_lineedit);
    x_error_lineedit->show();

    hlayout = new QHBoxLayout();
    vlayout->addLayout(hlayout);

    QLabel *y_error_label = new QLabel(tr("y Error"), this);
    hlayout->addWidget(y_error_label);
    y_error_label->show();

    y_error_lineedit = new QLineEdit(this);
    hlayout->addWidget(y_error_lineedit);
    y_error_lineedit->show();
  }

  if(type != BAR){
  //Style
  QGroupBox *style_box = new QGroupBox(tr("Style"));
  hlayout = new QHBoxLayout();
  style_box->setLayout(hlayout);
  vlayout->addWidget(style_box);

  //Line style
  QLabel *style_label = new QLabel(tr("Line style"), style_box);
  hlayout->addWidget(style_label);
  style_label->show();

  style_combo = new QComboBox(style_box);
  style_combo->addItem(tr("None"), QVariant(""));
  style_combo->addItem(tr("Lines"), QVariant("-"));
  style_combo->addItem(tr("Dots"), QVariant("."));
  style_combo->addItem(tr("Points"), QVariant("@"));
  style_combo->addItem(tr("Lines and points"), QVariant("-@"));
  style_combo->addItem(tr("Impulses"), QVariant("^"));
  style_combo->addItem(tr("Steps"), QVariant("L"));
  style_combo->addItem(tr("Boxes"), QVariant("#"));
  style_combo->addItem(tr("Error bars"), QVariant("~"));
  style_combo->addItem(tr("Box error bars"), QVariant("#~"));
  hlayout->addWidget(style_combo);
  style_combo->show();

  //Color
  QLabel *color_label = new QLabel(tr("Color"), style_box);
  hlayout->addWidget(color_label);
  color_label->show();

  color_combo = new QComboBox(style_box);
  color_combo->addItem(tr("None"), QVariant(""));
  color_combo->addItem(tr("Red"), QVariant("r"));
  color_combo->addItem(tr("Green"), QVariant("g"));
  color_combo->addItem(tr("Blue"), QVariant("b"));
  color_combo->addItem(tr("Magenta"), QVariant("m"));
  color_combo->addItem(tr("Cyan"), QVariant("c"));
  color_combo->addItem(tr("White"), QVariant("w"));
  hlayout->addWidget(color_combo);
  color_combo->show();

  //Point
  QLabel *point_label = new QLabel(tr("Point style"), style_box);
  hlayout->addWidget(point_label);
  point_label->show();

  point_combo = new QComboBox(style_box);
  point_combo->addItem(tr("None"), QVariant(""));
  point_combo->addItem(tr("+"), QVariant("+"));
  point_combo->addItem(tr("*"), QVariant("*"));
  point_combo->addItem(tr("o"), QVariant("o"));
  point_combo->addItem(tr("x"), QVariant("x"));
  hlayout->addWidget(point_combo);
  point_combo->show();

  }//End if

  //Buttons
  hlayout = new QHBoxLayout();
  vlayout->addLayout(hlayout);

  QPushButton *ok_button = new QPushButton(tr("Ok"), this);
  hlayout->addWidget(ok_button);
  ok_button->show();
  connect(ok_button, SIGNAL(clicked()), this, SLOT(accept()));

  QPushButton *cancel_button = new QPushButton(tr("Cancel"), this);
  hlayout->addWidget(cancel_button);
  cancel_button->show();
  connect(cancel_button, SIGNAL(clicked()), this, SLOT(reject()));
}

QString TablePlot::command()
{
  int i, j, k;

  QString x_values, y_values;
  QTextStream x_values_stream(&x_values), y_values_stream(&y_values);

  QString x_error_values, y_error_values;
  QTextStream x_error_values_stream(&x_error_values), y_error_values_stream(&y_error_values);

  //Get X, Y and error values
  if(this->use_cols)
  {
    j = x_lineedit->text().toInt() - 1;
    k = y_lineedit->text().toInt() - 1;

    //Check index
    if( (j < 0 || j >= model->columnCount()) ||
	(k < 0 || k >= model->columnCount()))
      throw "Wrong column index";

    if(type != POLAR)
    {
      //Get
      for(i = 0;i < model->rowCount(); i++)
      {
	x_values_stream << model->data(i, j) << ",";
	y_values_stream << model->data(i, k) << ",";
      }

      //Errors
      if(type != BAR)
      {
	j = x_error_lineedit->text().toInt() - 1;
	k = y_error_lineedit->text().toInt() - 1;

	for(i = 0; j > -1 && i < model->rowCount(); i++)
	  x_error_values_stream << model->data(i, j) << ",";
	for(i = 0; k > -1 && i < model->rowCount(); i++)
	  y_error_values_stream << model->data(i, k) << ",";
      }
    }else{
      QString a, r;
      //Get
      for(i = 0; i < model->rowCount(); i++)
      {
	a = model->data(i, j);
	r = model->data(i, k);
	x_values_stream << r << ".*cos(" << a << "),";
	y_values_stream << r << ".*sin(" << a << "),";
      }
    }
  }else
  {
    j = x_lineedit->text().toInt() - 1;
    k = y_lineedit->text().toInt() - 1;

    //Check index
    if( (j < 0 || j >= model->rowCount()) ||
	(k < 0 || k >= model->rowCount()))
      throw "Wrong row index";

    if(type != POLAR)
    {
      //Get
      for(i = 0; i < model->columnCount(); i++)
      {
	x_values_stream << model->data(j, i) << ",";
	y_values_stream << model->data(k, i) << ",";
      }

      //Errors
      if(type != BAR)
      {
	j = x_error_lineedit->text().toInt() - 1;
	k = y_error_lineedit->text().toInt() - 1;
	for(i = 0; j > -1 && i < model->columnCount(); i++)
	  x_error_values_stream << model->data(j, i) << ",";
	for(i = 0; k > -1 && i < model->columnCount(); i++)
	  y_error_values_stream << model->data(k, i) << ",";
      }
    }else{
      QString a, r;
      //Get
      for(i = 0; i < model->columnCount(); i++)
      {
	a = model->data(j, i);
	r = model->data(k, i);
	x_values_stream << r << ".*cos(" << a << "),";
	y_values_stream << r << ".*sin(" << a << "),";
      }
    }
  }

  //Command
  QString command;
  QTextStream command_stream(&command);
  switch(type)
  {
   case POLAR:
   case PLOT:
     command_stream << "plot(";
     break;
   case SEMILOGY:
     command_stream << "semilogy(";
     break;
   case SEMILOGX:
     command_stream << "semilogx(";
     break;
   case LOGLOG:
     command_stream << "loglog(";
     break;
  case BAR:
    command_stream << "bar(";
  }

  command_stream << "[" << x_values << "],"
		 << "[" << y_values << "]";

  if(type != BAR)
  {
    QString style, color, point;

    style = style_combo->itemData(style_combo->currentIndex()).toString();
    color = color_combo->itemData(color_combo->currentIndex()).toString();
    point = point_combo->itemData(point_combo->currentIndex()).toString();

    command_stream << ",\"" << style << color << point << "\""
		   << ")";

  }else
    command_stream << ")";

  return command;
}

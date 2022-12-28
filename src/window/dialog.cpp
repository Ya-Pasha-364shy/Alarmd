#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(int _cnt,QWidget *parent)
    : QDialog(parent), ui(new Ui::Dialog)
{
  ui->setupUi(this);
  const std::string fileName = "/var/log/Logfile.log";
  cnt = _cnt+1;
  std::ifstream file(fileName);
  std::string *lines = new std::string[cnt];
  int size = 0;

  /* читаем файл построчно в круговой массив */
  while (file.good())
  {
    getline(file, lines[size % cnt]);
    size++;
  }
  
  /* вычисляем начало кругового массива и его размер */
  int start = size > cnt ? (size % cnt) : 0;
  int count = std::min(cnt, size);
  QString alarms;

  /* выводим элементы в порядке чтения */
  for (int i = 0; i < count; i++)
  {
    alarms += QString::fromStdString(lines[(start + i) % cnt])+"\n";
  }
  ui->labelAlarm->setText(alarms);
}

Dialog::~Dialog()
{
  delete ui;
}

void Dialog::on_btnOK_clicked()
{
  accept();
}

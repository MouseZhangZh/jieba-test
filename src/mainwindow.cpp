#include "mainwindow.h"
#include <QDebug>
#include <QIcon>
#include <QTextCodec>
#include "uchardet/uchardet.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setMinimumSize(500,400);
    this->setWindowIcon(QIcon::fromTheme("inode-directory"));
    m_widget = new QWidget(this);
    m_line = new QLineEdit(m_widget);
    m_edit = new QTextEdit(m_widget);
    m_select_file_button = new QPushButton("select files",m_widget);
    m_start_button = new QPushButton("start",m_widget);

    m_vlayout = new QVBoxLayout(m_widget);
    m_hlayout = new QHBoxLayout;

    m_hlayout->addWidget(m_line);
    m_hlayout->addWidget(m_select_file_button);
    m_hlayout->addWidget(m_start_button);
    m_vlayout->addLayout(m_hlayout);
    m_vlayout->addWidget(m_edit);
    m_widget->setLayout(m_vlayout);
    this->setCentralWidget(m_widget);

    connect(m_select_file_button,&QPushButton::clicked,this,[&](){
        m_file_dialog = new QFileDialog(this);
        m_file_dialog->setFileMode(QFileDialog::ExistingFile);

        QStringList list;
        if (m_file_dialog->exec())
              list = m_file_dialog->selectedFiles();

        m_line->setText(list.first());
    });
    connect(m_start_button,&QPushButton::clicked,this,&MainWindow::seg);
}

MainWindow::~MainWindow()
{

}

void MainWindow::getTxtContent(QString &path, QString &textcontent)
{
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text))
        return;

    QByteArray encodedString = file.readAll();

    uchardet_t chardet = uchardet_new();
    if(uchardet_handle_data(chardet,encodedString.constData(),encodedString.size()) !=0)
        qWarning()<<"Txt file encoding format detect fail!"<<path;

    uchardet_data_end(chardet);
    const char *codec = uchardet_get_charset(chardet);
    uchardet_delete(chardet);

    if(QTextCodec::codecForName(codec) == 0)
        qWarning()<<"Unsupported Text encoding format"<<path;

    QTextStream stream(encodedString,QIODevice::ReadOnly);
    stream.setCodec(codec);

    textcontent = stream.readAll().replace("\n","");

    file.close();
    encodedString.clear();
    chardet = NULL;
    stream.flush();

    return;
}

void MainWindow::seg()
{
    QString path = m_line->text();
    if(path.isEmpty())
        return;
    QString content;
    this->getTxtContent(path, content);
    if(content.isEmpty())
        return;
    m_cs = ChineseSegmentation::getInstance();
    QVector<SKeyWord> term = m_cs->callSegement(content);
    m_edit->clear();
    QString keywords;
    for(int i = 0;i<term.size();++i)
    {
        keywords.append(QString("["));
        keywords.append(QString::fromStdString(term.at(i).word));
        keywords.append(QString(" "));
        keywords.append(QString::number(term.at(i).weight));
        keywords.append(QString("]"));
    }
    m_edit->append(keywords);
}


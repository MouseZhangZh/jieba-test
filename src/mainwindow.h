#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QFileDialog>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>
#include "chinese-segmentation.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void getTxtContent(QString &path, QString &textcontent);
    void seg();
private:
    QFileDialog *m_file_dialog;
    QLineEdit *m_line;
    QTextEdit *m_edit;
    QPushButton *m_select_file_button;
    QPushButton *m_start_button;
    ChineseSegmentation *m_cs;
    QHBoxLayout *m_hlayout;
    QVBoxLayout *m_vlayout;
    QWidget *m_widget;

};
#endif // MAINWINDOW_H

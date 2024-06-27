#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <QFile>
#include <QFileDialog>
#include <QFontDatabase>
#include <QInputDialog>
#include <QFormLayout>
#include <QListWidget>
#include <QLabel>
#include <QDialogButtonBox>
#include <QFontComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QDebug>
#include <QRegularExpression>
#include <QHash>
#include <QTextBrowser>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QString opened_file;// Location of file being edited
    int argc;//           Count of args passed into QApplication
    char ** argv;//       Args passed into QApplication
    int progress_percent;// The percent of questions answered
    QString exam_Data;//The data for the exam
    QVector<QVector<QString>> exam_Questions;//The questions list for the exam
    QVector<bool> answeredList;
    QVector<int> isCorrect;
    int current_Question;//The current question
    QWidget *main;
    int currentQ;
    int selections = 11;
    bool selection[11] = {0,0,0,0,0,0,0,0,0,0,0};
    bool answer[11] = {0,0,0,0,0,0,0,0,0,0,0};
    float questions = 0;
    float answered = 0;
    int correct =0;
    QString qType = "";
    bool exam_Type;
    QString s_answer;
    QString reference;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void open_file(QString);
    void args_handler();
    void parse_Questions();
    void start_Exam();
    void set_Answers(QString);
    void set_Question(int);
    void reset_Selection();
    void reset_Answers();
    QVector<QString> pull_Question(int);
    void pull_Selection();
    QString question_Type(QString);
    void hide_Single();
    void hide_Multiple();
    void show_Single();
    void show_Multiple();
    QString find_Answer();
    void format_Single();
    void format_Multiple();
    void unCheckAll();


private slots:
    void on_actionOpen_triggered();

    void on_actionBegin_Exam_triggered();

    void on_Left_Arrow_clicked();

    void on_Right_Arrow_clicked();

    void on_Submit_Question_clicked();

    void on_End_Exam_clicked();
    void on_actionBegin_Review_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H

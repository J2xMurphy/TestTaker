#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    s_answer = "";
    main = this;
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::args_handler()
{
    std::cout << argc << std::endl;
    std::cout << argv[0] << std::endl;
    if (argc==2){
        open_file(argv[1]);
    }
}

void MainWindow::open_file(QString dir)
{
    //OPENS FILE FROM DIALOG, RETURNS IF ERROR
    // FUNCTION SHOULD HAVE RELAVANT RETURN STATEMENT
    QFile file(dir);
    if (file.open(QIODevice::ReadOnly)!=1){
        std::cout << dir.toStdString() << " could not be opened." << std::endl;
        return;
    }
    hide_Single();
    hide_Multiple();

    //READS DATA FROM OPENED FILE, THEN CLOSES
    opened_file = dir;
    QDataStream f_in(&file);
    char n[file.size()];
    f_in.readRawData(n,file.size());
    file.close();
    main->findChild<QProgressBar*>("progressBar")->setValue(0);

    setWindowTitle(dir);
    exam_Data = QString(n);
    parse_Questions();
}

void MainWindow::on_actionOpen_triggered()
{
    //OPENS A FILE DIALOG
    QString dir;
    dir = QFileDialog::getOpenFileName(this,
                                       "Open...", "", "Filetypes (*.txt)");
    open_file(dir);
}

//This is awful it parses the questions into seperated vectors of pages
void MainWindow::parse_Questions()
{
    QVector<QVector<QString>> tData;
    QVector<QString> singleLine = exam_Data.split("\r\n");
    QVector<QString> *tString = new QVector<QString>;
    int readline = 3;
    while(readline < singleLine.length())
    {
        std::cout << readline << "," << singleLine.length() << ": "<< singleLine[readline].length() << " " <<singleLine[readline].toStdString() << std::endl;
        if (singleLine[readline].contains("Question No:"))
        {
            if (tString->length()>0)
            {
                isCorrect.append(0);
                answeredList.append(false);
                tData.append(*tString);
                tString = new QVector<QString>;
            }
            tString->append(singleLine[readline] + " : " +singleLine[readline+1]);
            readline+=2;
        }
        else{
            if (singleLine[readline].length()>0)
            {
                tString->append(singleLine[readline]);
            }
            readline+=1;
        }
    }
    if (tString->length()>0)
    {
        isCorrect.append(0);
        answeredList.append(false);
        tData.append(*tString);
    }
    exam_Questions = tData;
    questions = exam_Questions.length();
}

void MainWindow::on_actionBegin_Exam_triggered()
{
    exam_Type = true;
    start_Exam();
}

void MainWindow::start_Exam()
{
    currentQ = 0;
    set_Question(currentQ);
}

void MainWindow::set_Question(int currentQ)
{
    std::cout << "Question : " << currentQ << std::endl;
    QString f_answer = find_Answer();
    std::cout << "." << currentQ << std::endl;
    qType = question_Type(f_answer);
    std::cout << qType.toStdString() << std::endl;
    reset_Answers();
    reset_Selection();
    std::cout << "..." << std::endl;
    if (qType == "Single")
    {
        format_Single();
    }
    if (qType == "Multiple")
    {
        format_Multiple();
    }
    std::cout << "..." << std::endl;
}

void MainWindow::format_Single()
{
    std::cout << "..." << std::endl;
    QLabel *Question;
    reference = "";
    hide_Multiple();
    Question = this->findChild<QLabel*>("Question");
    QVector<QString> question = pull_Question(currentQ);
    Question->setText(question[0]);
    int finalAnswer;
    main->findChild<QRadioButton*>("radioButton_8")->setChecked(true);
    std::cout << "..1" << std::endl;
    for(int dataIndex=1;dataIndex<question.length();dataIndex++)
    {
        if (question[dataIndex].startsWith("Answer:"))
        {
            set_Answers(question[dataIndex]);
            finalAnswer = dataIndex;
            continue;
        }
        if (question[dataIndex].startsWith("Reference:"))
        {
            reference = question[dataIndex];
            continue;
        }
        QString tmp = "radioButton_" + QString::number(dataIndex);
        //        std::cout << tmp.toStdString() << std::endl;
        main->findChild<QRadioButton*>(tmp)->setText(question[dataIndex]);
        main->findChild<QRadioButton*>(tmp)->show();
    }
    std::cout << "..." << std::endl;
    for (int i = finalAnswer;i<=8;i++)
    {
        QString tmp = "radioButton_" + QString::number(i);
        std::cout << tmp.toStdString() << std::endl;
        main->findChild<QRadioButton*>(tmp)->hide();
    }
    std::cout << "..." << std::endl;
}

void MainWindow::format_Multiple()
{
    QLabel *Question;
    reference = "";
    hide_Single();
    unCheckAll();
    Question = this->findChild<QLabel*>("Question");
    QVector<QString> question = pull_Question(currentQ);
    Question->setText(question[0]);
    int finalAnswer;
    for(int dataIndex=1;dataIndex<question.length();dataIndex++)
    {
        if (question[dataIndex].startsWith("Answer:"))
        {
            set_Answers(question[dataIndex]);
            finalAnswer = dataIndex;
            break;
        }
        if (question[dataIndex].startsWith("Reference:"))
        {
            reference = question[dataIndex];
            break;
        }
        QString tmp = "checkBox_" + QString::number(dataIndex);
        //        std::cout << tmp.toStdString() << std::endl;
        main->findChild<QCheckBox*>(tmp)->setText(question[dataIndex]);
        main->findChild<QCheckBox*>(tmp)->show();
    }
    for (int i = finalAnswer;i<=8;i++)
    {
        QString tmp = "checkBox_" + QString::number(i);
        //        std::cout << tmp.toStdString() << std::endl;
        main->findChild<QCheckBox*>(tmp)->hide();
    }
}

QVector<QString> MainWindow::pull_Question(int i)
{
    return exam_Questions[i];
}

void MainWindow::on_Left_Arrow_clicked()
{
    currentQ--;
    set_Question(currentQ);
}


void MainWindow::on_Right_Arrow_clicked()
{
    currentQ++;
    set_Question(currentQ);
}

void MainWindow::pull_Selection()
{
    if (qType=="Single")
    {
        for (int i = 0;i<selections;i++)
        {
            selection[i] = main->findChild<QRadioButton*>
                            ("radioButton_"+QString::number(i+1))->isChecked();
        }
    }
    if (qType=="Multiple")
    {
        for (int i = 0;i<selections;i++)
        {
            selection[i] = main->findChild<QCheckBox*>
                           ("checkBox_"+QString::number(i+1))->isChecked();
        }
    }
}

void MainWindow::on_Submit_Question_clicked()
{
    pull_Selection();
    QString tmps = "";
    QString tmpa = "";
    for (bool v : selection)
        tmps.append(QString::number(v)+",");
    std::cout << tmps.toStdString() << std::endl;
    for (auto v : answer)
        tmpa.append(QString::number(v)+",");
    std::cout << tmpa.toStdString() << std::endl;

    std::cout << (tmps == tmpa) << std::endl;
    std::cout << currentQ << "-U-" << answeredList.length() <<std::endl;
    if (answeredList[currentQ] == 1)
    {
        answered++;
    }
    QString amirite;
    answeredList[currentQ] = 1;
    if(tmps == tmpa)
    {
        amirite = "That is the correct answer!";
        isCorrect[currentQ] = 1;
        correct++;
    }
    else
    {
        amirite = "The correct answers is "+s_answer;
        isCorrect[currentQ] = 0;
    }
    std::cout << "!"+reference.toStdString() << std::endl;
    if (reference != "")
    {
    }

    if (exam_Type == 0)
    {
        QDialog report(this);
        QFormLayout layout(&report);

        QLabel * relay = new QLabel;
        relay->setText(amirite);
        layout.addRow(relay);


        QLabel * refer = new QLabel;
        reference.remove(0,11);
        QString link = "<a href=\""+reference+"\">"+reference+"</a>";
        refer->setText(link);
        refer->setTextFormat(Qt::RichText);
        refer->setTextInteractionFlags(Qt::TextBrowserInteraction);
        refer->setOpenExternalLinks(true);
        if (reference != "")
        {
            layout.addRow(refer);
        }

        QPushButton * accept = new QPushButton("Accept");
        layout.addRow(accept);

        QObject::connect(accept,SIGNAL(clicked()),&report,SLOT(accept()));
        report.exec();

        delete relay;
        delete accept;
        delete refer;
    }
    reset_Answers();
    reset_Selection();
    currentQ++;
    if (currentQ>=questions)
    {
        on_End_Exam_clicked();
    }
    else{
        set_Question(currentQ);
    }
    double result = (answered/questions)*100;
    std::cout << "Right " << answered << "/" << questions << "=" << result << std::endl;
    main->findChild<QProgressBar*>("progressBar")->setValue(result);
}

void MainWindow::set_Answers(QString ans)
{
    ans.remove("Answer: ");
    s_answer = ans;
//    std::cout << ans.toStdString() << std::endl;
    for (int i =0; i<ans.length();i++)
    {
        switch (ans[i].toLatin1())
        {
        case 'A':
            answer[0] = true;
            break;
        case 'B':
            answer[1] = true;
            break;
        case 'C':
            answer[2] = true;
            break;
        case 'D':
            answer[3] = true;
            break;
        case 'E':
            answer[4] = true;
            break;
        case 'F':
            answer[5] = true;
            break;
        case 'G':
            answer[6] = true;
            break;
        case 'H':
            answer[7] = true;
            break;
        case 'I':
            answer[8] = true;
            break;
        case 'J':
            answer[9] = true;
            break;
        case 'K':
            answer[10] = true;
            break;
        default:
            break;
        }
    }
    std::cout << answer[0]  << answer[1]  << answer[2]  << answer[3]
               << answer[4] << answer[5] << answer[6] << answer[7]
              << answer[8] << answer[9] << answer[10] << std::endl;
}

void MainWindow::reset_Selection()
{
    for (int i = 0;i<selections;i++)
    {
        selection[i] = 0;
    }
}

void MainWindow::reset_Answers()
{
    for (int i = 0;i<selections;i++)
    {
        answer[i] = 0;
    }
}

QString MainWindow::question_Type(QString fAns)
{
    int maxAnswers = 0;
    std::cout << "String is " << fAns.toStdString() << std::endl;
    for (int i =0; i<fAns.length();i++)
    {
        switch (fAns[i].toLatin1())
        {
        case 'A':
        case 'B':
        case 'C':
        case 'D':
        case 'E':
        case 'F':
        case 'G':
        case 'H':
        case 'I':
        case 'J':
        case 'K':
            maxAnswers++;
            break;
        default:
            break;
        }
    }
    std::cout << "Count is " << maxAnswers << std::endl;
    if (maxAnswers>1)
        return "Multiple";
    return "Single";
}

void MainWindow::hide_Single()
{
    for (int i = 1;i<=selections;i++)
    {
        main->findChild<QRadioButton*>("radioButton_"+QString::number(i))->hide();
    }
}

void MainWindow::show_Single()
{
    for (int i = 1;i<=selections;i++)
    {
        main->findChild<QRadioButton*>("radioButton_"+QString::number(i))->show();
    }
}

void MainWindow::hide_Multiple()
{
    for (int i = 1;i<=selections;i++)
    {
        main->findChild<QCheckBox*>("checkBox_"+QString::number(i))->hide();
    }
}


void MainWindow::unCheckAll()
{
    for (int i = 1;i<=selections;i++)
    {
        main->findChild<QCheckBox*>("checkBox_"+QString::number(i))->setChecked(false);
    }
}

void MainWindow::show_Multiple()
{
    for (int i = 1;i<=selections;i++)
    {
        main->findChild<QCheckBox*>("checkBox_"+QString::number(i))->show();
    }
}

QString MainWindow::find_Answer()
{
    QVector<QString> question = pull_Question(currentQ);
    for(int i=0;i<question.length();i++)
    {
        if (question[i].startsWith("Answer:"))
        {
            QString tmp = question[i].remove("Answer:");
            std::cout << "The answers are " << tmp.toStdString() << std::endl;
            return tmp;
        }
    }
    return "";
}

void MainWindow::on_End_Exam_clicked()
{
    int ct = 0;
    int ta = 0;
    for (auto b:isCorrect)
        ct+=b;
    for (auto b:answeredList)
        ta+=b;
    std::cout << "You got " << ct << " of " << ta <<  " correct!" << std::endl;

    // Choose a tag system ya bum!
    QDialog export_dialog(this);
    export_dialog.setWindowTitle("Score:");
    QVBoxLayout v_layout(&export_dialog);

    //ADDS THE PREVIEW TEXT BOX TO THE SCREEN
    QLabel * preview_text = new QLabel();
    v_layout.addWidget(preview_text);

    //ADDS THE FINALIZE EXPORT AND CANCEL BUTTONS TO THE SCREEN
    QWidget action_buttons;
    QHBoxLayout buttons_layout(&action_buttons);
    QPushButton * return_button = new QPushButton("Return");
    QPushButton * end_button = new QPushButton("End Exam");
    buttons_layout.addWidget(return_button);
    buttons_layout.addWidget(end_button);
    v_layout.addWidget(&action_buttons);

    QObject::connect(return_button,SIGNAL(clicked()),
                     &export_dialog,SLOT(reject()));
    QObject::connect(end_button,SIGNAL(clicked()),
                     this,SLOT(export_save()));
    QObject::connect(this,SIGNAL(export_finished(int)),
                     &export_dialog,SLOT(done(int)));

    //SETS THE TEXT IN THE PREVIEW BOX
    preview_text->setText("You got "+QString::number(ct)+" of "+
                          QString::number(ta)+" correct.");

    //EXECUTES
    export_dialog.exec();

    //CLEANUP
    delete preview_text;
    delete return_button;
    delete end_button;
}

void MainWindow::on_actionBegin_Review_triggered()
{
    exam_Type = false;
    start_Exam();
}


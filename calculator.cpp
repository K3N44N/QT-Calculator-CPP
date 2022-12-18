#include "calculator.h"
#include "./ui_calculator.h"
#include <QRegularExpression>

/*
 TODO LIST

 - Make the MathButtonPressed better: Use either HASHMAPS or just an int LOL. [WARNING DOCUMENT THE INT OPTION VERY WELL]
 - Have MathButtonPressed evaluate the answer continuously, allowing chain operations

*/
double calcVal = 0;
bool divTrigger = false;
bool multTrigger = false;
bool addTrigger = false;
bool subTrigger = false;

Calculator::Calculator(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Calculator)
{
    ui->setupUi(this);

    ui->Display->setText(QString::number(calcVal));
    QPushButton *numButtons[10];

    //this mystic piece of code calls the function NumPressed when a button is pressed
    //TODO Figure out what each line does

    for(int i = 0; i < 10; i++){
        QString butName = "Button" + QString::number(i);
        numButtons[i] = Calculator::findChild<QPushButton *>(butName);
        connect(numButtons[i], SIGNAL(released()), SLOT(NumPressed()));
    }

    connect(ui->PlusButton, SIGNAL(released()), SLOT(MathButtonPressed()));
    connect(ui->MinusButton, SIGNAL(released()), SLOT(MathButtonPressed()));
    connect(ui->MultiplyButton, SIGNAL(released()), SLOT(MathButtonPressed()));
    connect(ui->DivideButton, SIGNAL(released()), SLOT(MathButtonPressed()));

    connect(ui->EqualButton, SIGNAL(released()), SLOT(EqualButton()));
    connect(ui->SignButton, SIGNAL(released()), SLOT(ChangeSign()));
    connect(ui->CeButton, SIGNAL(released()), SLOT(AllClear()));
    connect(ui->CButton, SIGNAL(released()), SLOT(ClearDisp()));
    connect(ui->BackspaceButton, SIGNAL(released()), SLOT(BackspacePressed()));

}

Calculator::~Calculator()
{
    delete ui;
}

void Calculator::NumPressed(){
    QPushButton *button = (QPushButton *)sender();
    QString butVal =  button->text();
    QString displayVal = ui->Display->text();

    if((displayVal.toDouble() == 0) || (displayVal.toDouble() == 0)){
        ui->Display->setText(butVal);
    }
    else {
        QString newVal = displayVal +butVal;
        double dblNewVal = newVal.toDouble();
        //the 16 is to use sci notation after 16 digits
        ui->Display->setText(QString::number(dblNewVal, 'g', 16));
    }
}

void Calculator::MathButtonPressed(){
    //resetBoolValues
    divTrigger = false; multTrigger = false; addTrigger = false; subTrigger = false;

    QString displayVal = ui->Display->text();
    calcVal = displayVal.toDouble();
    QPushButton *button = (QPushButton *)sender();
    QString butVal = button->text();

    //this is a horrible, terrible way to do this but thats what the tutorial did *shrug*
    if(QString::compare(butVal, "/", Qt::CaseInsensitive) == 0) divTrigger = true;
    if(QString::compare(butVal, "x", Qt::CaseInsensitive) == 0) multTrigger = true;
    if(QString::compare(butVal, "-", Qt::CaseInsensitive) == 0) subTrigger = true;
    if(QString::compare(butVal, "+", Qt::CaseInsensitive) == 0) addTrigger = true;

    //empties the screen
    ui->Display->setText("");
}

void Calculator::EqualButton() {
    double solution = 0.0;
    QString displayVal = ui->Display->text();
    double dblDisplayVal = displayVal.toDouble();

    if(addTrigger || subTrigger || multTrigger || divTrigger){

        if(addTrigger) solution = calcVal + dblDisplayVal;
        else if(subTrigger) solution = calcVal - dblDisplayVal;
        else if(multTrigger) solution = calcVal * dblDisplayVal;
        else if(divTrigger) solution = calcVal / dblDisplayVal;

    }

    ui->Display->setText(QString::number(solution));
}

void Calculator::ChangeSign() {
    QString displayVal = ui->Display->text();
    static QRegularExpression reg("[-]?[0-9.]*"); //regex magic
    QRegularExpressionMatch match = reg.match(displayVal);
    if(match.hasMatch()){
        double dblDisplayVal = displayVal.toDouble();
        double dblDisplayValSign = -1 * dblDisplayVal;
        ui->Display->setText(QString::number(dblDisplayValSign));
    }
}

void Calculator::AllClear(){
    calcVal=0;
    ui->Display->setText("0");
}

void Calculator::ClearDisp(){
    ui->Display->setText("0");
}

void Calculator::BackspacePressed(){
    QString val = ui->Display->text();
    val.chop(1);
    ui->Display->setText(val);
}

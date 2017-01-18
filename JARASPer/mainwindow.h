#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#define MEMSIZE 250
#define MAXOPCODE 256
#define MAXMICROC 20
#define BITSVAL 16
#define LOG2BIT log(BITSVAL)/log(2)
#define EXPONENT (int)(4* log(BITSVAL)/log(16))
#define MEM_LOAD_STD "C:\\Users\\Philippe\\AppData\\Roaming\\memtest.txt"
#define OPCODE_LOAD_STD "C:\\Users\\Philippe\\AppData\\Roaming\\optest.txt"

#include <QMainWindow>
#include <QListWidget>
#include <string>
#include <iostream>
#include <fstream>
#include <math.h>

class MainWindow;
class QLabel;

class Component {
public:
  unsigned int value;
  unsigned short bits;
  unsigned short type;
  MainWindow *window;
  QLabel *display;
  Component(){}
  Component(unsigned short, unsigned short);
  void assign(Component);
  void assign(Component, int);
  void assign(unsigned int val);
};
class GeneralPurposeRegister : public Component{
public:
   GeneralPurposeRegister();
    GeneralPurposeRegister(unsigned short, unsigned short);

};
class MemoryAdressRegister : public Component{
public:
  MemoryAdressRegister();
    MemoryAdressRegister(unsigned short, unsigned short);

  void assignToMemory( std::string[], unsigned short); //sÃ³ MDR
};
class MemoryDataRegister : public Component{
public:
  MemoryDataRegister();
    MemoryDataRegister(unsigned short, unsigned short);

  void assignFromMemory( std::string[], unsigned short); //sÃ³ M[MAR]/ /MAR
};
class StackPointer : public Component{
public:
  StackPointer();
    StackPointer(unsigned short, unsigned short);
};
class ProgramCounter : public Component{
public:
  ProgramCounter();
    ProgramCounter(unsigned short, unsigned short);
};
class InstructionRegister : public Component{
public:
  InstructionRegister();
    InstructionRegister(unsigned short, unsigned short);
};
class Incrementer : public Component{
public:
  Incrementer();
    Incrementer(unsigned short, unsigned short);
  // void Incrementer::assign(Component a) { value = a.value + 1; }
  //void Incrementer::increment(){value= value+1;}
};
class ControlUnit : public Component{
public:
  ControlUnit();
    ControlUnit(unsigned short, unsigned short);
};
class ALU : public Component {
public:
  GeneralPurposeRegister *x;
  GeneralPurposeRegister *y;
public:
  int overflow;
  int zero;
  int negative;
  int equal;
public:
  unsigned int r;

  ALU();
  ALU(GeneralPurposeRegister *, GeneralPurposeRegister *);
  void checkFlags(int,unsigned int) ;
  void add() ;
  void sub() ;
  // void ALU::bitShiftL(){x-> value += + pow(2,(x->value));}
  void bitShiftL(unsigned int);
  //  void ALU::bitShiftR(Component a){value = value - 2^(a.value);}
  void bitShiftR(unsigned int);
};

struct ComponentList{
  GeneralPurposeRegister *a;
  GeneralPurposeRegister *b;
  ProgramCounter *pc;
  Incrementer *inc;
  StackPointer *sp;
  InstructionRegister *ir;
  MemoryAdressRegister *mar;
  MemoryDataRegister *mdr;
  GeneralPurposeRegister *ALUx;
  GeneralPurposeRegister *ALUy;
  ALU *alu;
  ControlUnit *cu;
  ComponentList();
  ComponentList(GeneralPurposeRegister &,GeneralPurposeRegister &,ProgramCounter &,Incrementer &,StackPointer &,InstructionRegister &,MemoryAdressRegister &,MemoryDataRegister &,GeneralPurposeRegister &, GeneralPurposeRegister &, ALU &, ControlUnit &);
  Component *getComponent(std::string );
  ALU *getAlu(std::string );
  MemoryDataRegister *getMdr(std::string );
MemoryAdressRegister *getMar(std::string);
  void bindToWindow(MainWindow*);
void reset_values();
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    friend struct ComponentList;
    friend class Component;
    Q_OBJECT

public:
  std::string mem[MEMSIZE],REPLmem[MEMSIZE], opcode[MAXOPCODE][MAXMICROC], Dopcode[100][MAXMICROC];
  GeneralPurposeRegister A, B, ALUx, ALUy;
  MemoryAdressRegister MAR;
  MemoryDataRegister MDR;
  StackPointer SP;
  ProgramCounter PC;
  Incrementer INC;
  InstructionRegister IR;
  ALU alu;
  ControlUnit CU;
  ComponentList cl;
  int opcodeCount, memCount;
  void clean_gui();
  void update_gui(std::string);
  void reset_gui_values();
  void update_all_gui();
  void update_mem(unsigned short);
  void setDisplay(Component);
    void setA(unsigned int x);
    void setB(unsigned int x);
    void setALUx(unsigned int x);
    void setALUy(unsigned int x);
    void setALUr(unsigned int x);
    void setIR(unsigned int x);
    void setPC(unsigned int x);
    void setSP(unsigned int x);
    void setCU(unsigned int x);
    void setINC(unsigned int x);
    void setMAR(unsigned int);
    void setMDR(unsigned int);
    void setOverflow(unsigned int x);
    void setZero(unsigned int x);
    void setEqual(unsigned int x);
    void setNegative(unsigned int x);
  explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_MEM_GO_clicked();

    void on_REPL_INPUT_returnPressed();

    void on_CLEAR_BUTTON_clicked();

    void on_MEM_LOCATION_returnPressed();

    void on_OPCODE_LOCATON_returnPressed();

    void on_MEM_FETCH_clicked();

    void on_MEM_EXECUTE_clicked();

    void on_MEMORY_VIEW_itemChanged(QListWidgetItem *item);

private:
    Ui::MainWindow *ui;
};

int executeOpcode(int, std::string[][MAXMICROC], int, ComponentList, std::string[]);
void fetch(std::string memory[], std::string[][MAXMICROC], int, ComponentList);
void executeMemory(std::string[],std::string[][MAXMICROC], int, ComponentList);
int loadMem(std::string [MEMSIZE],std::string);
void loadOpcode(std::string [MAXOPCODE][MAXMICROC],std::string);
int fromTwoComp(unsigned int, unsigned short);
int checkOverflow(int, unsigned int);
int checkEqual(int,int);
unsigned int toTwoComp(int, unsigned short);
int parseAllOpcodes(std::string[MAXOPCODE][MAXMICROC], std::string[100][MAXMICROC]);
void parseOpcode(std::string[MAXMICROC], std::string[MAXMICROC]);
std::string parseOpcodeLine(std::string, std::string[MAXMICROC], int &);
QString format_value(unsigned int);
#endif // MAINWINDOW_H


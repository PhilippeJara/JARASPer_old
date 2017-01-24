#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QThread>
#include <QCoreApplication>

Component::Component(unsigned short val, unsigned short bts) { value = val; bits = bts; }
void Component::assign(Component a) { value = a.value;
  this->display->setText(QString::number(value,16));
  this->display->setStyleSheet("border: 2px solid green");
  this->window->setDisplay(this);
}
void Component::assign(Component a, int valMod) { value = a.value + valMod;
  this->display->setText(QString::number(value,16));
  this->display->setStyleSheet("border: 2px solid green");
  this->window->setDisplay(this);
}
void Component::assign(unsigned int val) { value = val;
  this->display->setText(QString::number(value,16));
  this->display->setStyleSheet("border: 2px solid green");
  this->window->setDisplay(this);
}

GeneralPurposeRegister::GeneralPurposeRegister(unsigned short val, unsigned short bts):Component(val, bts){value = val; bits = bts;type = 0;}

MemoryAdressRegister::MemoryAdressRegister(unsigned short val, unsigned short bts):Component(val, bts){value = val; bits = bts;type = 1;}
void MemoryAdressRegister::assignToMemory( std::string mem[], unsigned short val) {
  QString temp = format_value(val);
  mem[value] = temp.toStdString();
  window->update_mem(value);
} //sÃ³ MDR

MemoryDataRegister:: MemoryDataRegister(unsigned short val, unsigned short bts):Component(val, bts){value = val; bits = bts;type = 2;}
void MemoryDataRegister::assignFromMemory( std::string mem[], unsigned short memoryAddr) {
  value = std::stoi(mem[memoryAddr],nullptr, 16);
  this->display->setText(QString::number(value,16));
  this->display->setStyleSheet("border: 2px solid green");
  this->window->setDisplay(this);
} //sÃ³ M[MAR]/ /MAR

StackPointer::StackPointer(unsigned short val, unsigned short bts):Component(val, bts){value = val; bits = bts; type = 3;}

ProgramCounter::ProgramCounter(unsigned short val, unsigned short bts):Component(val, bts){value = val; bits = bts; type = 4;}

InstructionRegister::InstructionRegister(unsigned short val, unsigned short bts):Component(val, bts){value = val; bits = bts; type = 5;}

Incrementer::Incrementer(unsigned short val, unsigned short bts):Component(val, bts){value = val; bits = bts; type = 6;}

ControlUnit::ControlUnit(unsigned short val, unsigned short bts):Component(val, bts){value = val; bits = bts; type = 7;}

ALU::ALU(GeneralPurposeRegister *a, GeneralPurposeRegister *b){ x = a; y = b; type = 8; overflow = 0;zero = 0;negative = 0;equal = 0;value = 0;}
void ALU::checkFlags(int resultado,unsigned int bits) {
  overflow = 0;
  zero = 0;
  negative = 0;
  equal = 0;
  if (checkEqual(x->value, y->value)==0){equal = 1;}
  if (checkOverflow(resultado, bits) == 1) { overflow = 1; }
  if (resultado < 0) { negative = 1; }
  if (resultado == 0) { zero = 1; }
}
void ALU::add() {
  int temp = 0;
  std::cout << fromTwoComp(x->value, x->bits)<< " + " << fromTwoComp(y->value, y->bits) << std::endl;
  temp = fromTwoComp(x->value, x->bits) + fromTwoComp(y->value, y->bits);
  checkFlags(temp, x->bits);
  std::cout << temp << std::endl;
  std::cout << negative << std::endl;
  r = toTwoComp(temp, x->bits);
  value = r;
  this->display->setText(QString::number(value,16));
  this->display->setStyleSheet("border: 2px solid green");
  this->window->setDisplay(this);
  this->window->setZero(this->zero);
  this->window->setEqual(this->equal);
  this->window->setNegative(this->negative);
  this->window->setOverflow(this->overflow);
}
void ALU::sub() {
  int temp = 0;
  //if (x->value == y->value) { equal = 1; }
  std::cout << fromTwoComp(x->value, x->bits)<< " - " << fromTwoComp(y->value, y->bits) << std::endl;
  temp = fromTwoComp(x->value, x->bits) - fromTwoComp(y->value, y->bits);
  checkFlags(temp, x->bits);
  std::cout << temp << std::endl;
  std::cout << equal << std::endl;
  r = toTwoComp(temp, x->bits);
  value = r;
  this->display->setText(QString::number(value,16));
  this->display->setStyleSheet("border: 2px solid green");
  this->window->setDisplay(this);
  this->window->setZero(this->zero);
  this->window->setEqual(this->equal);
  this->window->setNegative(this->negative);
  this->window->setOverflow(this->overflow);
}
// void ALU::bitShiftL(){x-> value += + pow(2,(x->value));}
void ALU::bitShiftL(unsigned int val){r = x->value * pow(2,val);
  value = r;
  this->display->setText(QString::number(value,16));
  this->display->setStyleSheet("border: 2px solid green");
  this->window->setDisplay(this);
  this->window->setZero(this->zero);
  this->window->setEqual(this->equal);
  this->window->setNegative(this->negative);
  this->window->setOverflow(this->overflow);
}
//  void ALU::bitShiftR(Component a){value = value - 2^(a.value);}
void ALU::bitShiftR(unsigned int val){r = x->value /  pow(2,val);
  value = r;
  this->display->setText(QString::number(value,16));
  this->display->setStyleSheet("border: 2px solid green");
  this->window->setDisplay(this);
  this->window->setZero(this->zero);
  this->window->setEqual(this->equal);
  this->window->setNegative(this->negative);
  this->window->setOverflow(this->overflow);
}

ComponentList::ComponentList(GeneralPurposeRegister &ap,GeneralPurposeRegister &bp,ProgramCounter &pcp,Incrementer &incp,StackPointer &spp,InstructionRegister &irp,MemoryAdressRegister &marp,MemoryDataRegister &mdrp,GeneralPurposeRegister &ALUxp, GeneralPurposeRegister &ALUyp, ALU &alup, ControlUnit &cup){
  a = &ap;
  b = &bp;
  pc = &pcp;
  inc = &incp;
  sp = &spp;
  ir = &irp;
  mar = &marp;
  mdr = &mdrp;
  ALUx = &ALUxp;
  ALUy = &ALUyp;
  alu = &alup;
  cu = &cup;
}
Component *ComponentList::getComponent(std::string id){
  if (id == "A"){return a;}
  if (id == "B"){return b;}
  if (id == "PC"){return pc;}
  if (id == "INC"){return inc;}
  if (id == "SP"){return sp;}
  if (id == "IR"){return ir;}
  if (id == "MAR"){return mar;}
  if (id == "MDR"){return mdr;}
  if (id == "ALUx"){return ALUx;}
  if (id == "ALUy"){return ALUy;}
  if (id == "ALUr"){return alu;}
  if (id == "ALU"){return alu;}
  if (id == "CU"){return cu;}
}
void ComponentList::bindToWindow(MainWindow *windw){
  a->window = windw; a->display = windw->ui->A_DISPLAY;
  b->window = windw; b->display = windw->ui->B_DISPLAY;
  pc->window = windw; pc->display = windw->ui->PC_DISPLAY;
  inc->window = windw; inc->display = windw->ui->INC_DISPLAY;
  sp->window = windw; sp->display = windw->ui->SP_DISPLAY;
  ir->window = windw; ir->display = windw->ui->IR_DISPLAY;
  mar->window = windw; mar->display = windw->ui->MAR_DISPLAY;
  mdr->window = windw; mdr->display = windw->ui->MDR_DISPLAY;
  ALUx->window = windw; ALUx->display = windw->ui->ALUx_DISPLAY;
  ALUy->window = windw; ALUy->display = windw->ui->ALUy_DISPLAY;
  alu->window = windw; alu->display = windw->ui->ALUr_DISPLAY;
  cu->window = windw; cu->display = windw->ui->CU_DISPLAY;
}
void ComponentList::reset_values(){
  a->value = 0;
  b->value = 0;
  pc->value = 0;
  inc->value = 0;
  sp->value = 0;
  ir->value = 0;
  mar->value = 0;
  mdr->value = 0;
  ALUx->value = 0;
  ALUy->value = 0;
  alu->value = 0;
  alu->r = 0;
  alu->equal = 0;
  alu->negative = 0;
  alu->overflow = 0;
  alu->zero = 0;
  cu->value = 0;
}
ALU *ComponentList::getAlu(std::string id){
  if (id == "ALU" || id == "ALUr"){return alu;}
}
MemoryDataRegister *ComponentList::getMdr(std::string id){
  if (id=="MDR"){return mdr;}
}
MemoryAdressRegister *ComponentList::getMar(std::string id){
  if (id=="MAR"){return mar;}
}
int fromTwoComp(unsigned int a, unsigned short bits) {
  if (a < pow(2, bits) / 2 ) {
    return a;
  }
  if(a >= pow(2, bits) / 2) {
    return a - pow(2, bits);
  }
}
unsigned int toTwoComp(int a, unsigned short bits) {
  if (a > (pow(2, bits) / 2) - 1){
    if (a < pow(2,bits)){std::cout << "Overflow1 em " << a << std::endl; return a;}
    std::cout << "Overflow2 em " << a << std::endl; return a - pow(2,bits);
  }
  if ( a < -pow(2, bits) / 2){
    if(a > - pow(2,bits)){ std::cout << "Overflow3 em " << a << std::endl; return a - pow(2,bits);}
    std::cout << "Overflow4 em " << a << std::endl; return a + pow(2,bits);
  }
  if ( a >= 0) {
    return a;
  }
  else {
    return a + pow(2,bits);
  }
}
int checkOverflow(int a, unsigned int bits) {
  if (a < - pow(2, bits)/2 || a >=( pow(2, bits)/2) -1 ) {return 1;}
  else { return 0; }
}
int checkEqual(int val1, int val2) {
  if (val1 == val2){return 0;}
  return 1;
}



MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  A(0,BITSVAL),B(0,BITSVAL), ALUx(0,BITSVAL), ALUy(0,BITSVAL),
  MAR(0,BITSVAL),
  MDR(0,BITSVAL),
  SP(0,BITSVAL),
  PC(0,BITSVAL),
  INC(0,BITSVAL),
  IR(0,BITSVAL),
  alu(&ALUx, &ALUy),
  CU(0,BITSVAL),
  cl(A, B, PC, INC, SP, IR, MAR, MDR, ALUx, ALUy, alu, CU),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  loadOpcode(opcode,"");
  opcodeCount = parseAllOpcodes(opcode,Dopcode);
  memCount = loadMem(mem,"");
  clean_gui();
  update_all_components();
  for (int i=0; i<MEMSIZE; i++){
    ui->MEMORY_VIEW->addItem(QString::number(i,16)+ "\t" +  QString::fromStdString(mem[i]));
    std::cout << mem[i] <<std::endl;
    ui->MEMORY_VIEW->item(i)->setFlags(ui->MEMORY_VIEW->item(i)->flags() | Qt::ItemIsEditable);
  }
  //for (int i=0; i<MEMSIZE; i++){ui->MEMORY_DISPLAY->setText(ui->MEMORY_VIEW +"\n" + QString::fromStdString(mem[i]));}
  for (int i=0; i<MEMSIZE; i++){ui->MEMORY_DISPLAY->setText(ui->MEMORY_DISPLAY->toPlainText() +"\n" + QString::fromStdString(mem[i]));}
  for (int i=0; i<opcodeCount; i++){ui->OPCODE_DISPLAY->setText(ui->OPCODE_DISPLAY->toPlainText() +"\n" + QString::fromStdString(opcode[i][0]));}
}

QString format_value(unsigned int x){

  if (QString::number(x,BITSVAL).length() > EXPONENT) { return QString::number(x,BITSVAL).right(EXPONENT);}
  if (QString::number(x,BITSVAL).length() < EXPONENT) {
    QString temp = "00000" + QString::number(x,BITSVAL);
    return temp.right(EXPONENT);}
  return QString::number(x,BITSVAL);
}

void MainWindow::setDisplay(Component *x){x->display->setText(format_value(x->value));}
void MainWindow::setA(unsigned int x){ui->A_DISPLAY->setText(format_value(x));}
void MainWindow::setB(unsigned int x){ui->B_DISPLAY->setText(format_value(x));}
void MainWindow::setALUx(unsigned int x){ui->ALUx_DISPLAY->setText(format_value(x));}
void MainWindow::setALUy(unsigned int x){ui->ALUy_DISPLAY->setText(format_value(x));}
void MainWindow::setALUr(unsigned int x){ui->ALUr_DISPLAY->setText(format_value(x));}
void MainWindow::setIR(unsigned int x){ui->IR_DISPLAY->setText(format_value(x));}
void MainWindow::setPC(unsigned int x){ui->PC_DISPLAY->setText(format_value(x));}
void MainWindow::setSP(unsigned int x){ui->SP_DISPLAY->setText(format_value(x));}
void MainWindow::setCU(unsigned int x){ui->CU_DISPLAY->setText(format_value(x));}
void MainWindow::setINC(unsigned int x){ui->INC_DISPLAY->setText(format_value(x));}
void MainWindow::setMAR(unsigned int x){ui->MAR_DISPLAY->setText(format_value(x));}
void MainWindow::setMDR(unsigned int x){ui->MDR_DISPLAY->setText(format_value(x));}
void MainWindow::setOverflow(unsigned int x){ui->OVERFLOW_DISPLAY->setText(QString::number(x,16));}
void MainWindow::setZero(unsigned int x){ui->ZERO_DISPLAY->setText(QString::number(x,16));}
void MainWindow::setEqual(unsigned int x){ui->EQUAL_DISPLAY->setText(QString::number(x,16));}
void MainWindow::setNegative(unsigned int x){ui->NEGATIVE_DISPLAY->setText(QString::number(x,16));}
MainWindow::~MainWindow()
{
  delete ui;
}


void loadOpcode(std::string opcode[MAXOPCODE][MAXMICROC], std::string path){
  int i = 0, j=0, open=0, close=0;
  std::ifstream opload;
  if (path == ""){opload.open(OPCODE_LOAD_STD);}else{opload.open(path);}
  if(!opload.is_open()){std::cout<< "erro abrindo arquivo de Opcode"<< std::endl;}
  while(getline ( opload, opcode[i][j]) && opcode[i][j] != "" && i < MAXOPCODE && j < MAXMICROC){
    if (opcode[i][j].at(0) != ';'){
      if(opcode[i][j].at(0)=='('){open = 1;}
      if(opcode[i][j].at(0)==')'){close = 1;}
      if (open==1){
	j++;
	if(close == 1){
	  i++;
	  j=0;
	  open = 0;
	  close = 0;
	}
      }
    }
  }
}
int loadMem(std::string mem[MEMSIZE], std::string path) {
  int i = 0, temp = 0;
  std::ifstream memload;

  for (int init = 0; init < MEMSIZE; init++){mem[init] = "0000";}
  if (path == ""){memload.open(MEM_LOAD_STD);}else{memload.open(path);}
  if (!memload.is_open()) { std::cout << "erro abrindo o arquivo de memÃ³ria" <<path<< std::endl; return 0; }
  while (i < MEMSIZE && getline(memload, mem[i])) {
    if (mem[i].at(0) != ';') { i++; }
  }
  int orgJmp = 0;
  for(int j = 0;j<=i;j++){
    if(mem[j].substr(0,3) == "org"){
      //std::cout<<"CHEGUEI 1:"<<mem[j].substr(4)<<"        "<< stoi(mem[j].substr(4), nullptr, 16)<<"   "<<j<<std::endl;
      int k = i-j;
      orgJmp = stoi(mem[j].substr(4), nullptr, 16);
      mem[j] = "0000";
      while(k > 0){
	mem[orgJmp+k-1] = mem[j+k]; mem[j+k] = "0000";
	k--;
      }
    }
  }
  //i = orgJmp+i;
  //  i=0;
  //  while (i < MEMSIZE){
  //      if(mem[i] == ""){mem[i]= "0000";}
  //      i++;}
  return 0;
}



int parseAllOpcodes(std::string opcode[MAXOPCODE][MAXMICROC], std::string Dopcode[100][MAXMICROC]){
  int i= 0;
  for(i; i<MAXOPCODE; i++){
    if (opcode[i][0] == ""){break;}
    parseOpcode(opcode[i], Dopcode[i]);//std::cout<< "AllOpcodes  "<< std::endl;
  }
  return i;
}
void parseOpcode(std::string opcode[MAXMICROC], std::string Dopcode[MAXMICROC]){
  int i = 1, lastIndx = 1;
  Dopcode[0]=opcode[0].substr(1,opcode[0].length());
  while (opcode[i] != ")"){parseOpcodeLine(opcode[i], Dopcode, lastIndx);i++;}//std::cout<<i<<std::endl;}
}
std::string parseOpcodeLine(std::string opcodeLine, std::string Dopcode[MAXMICROC], int &lastIndx){
  std::string UAtom1, LAtom2;
  int i=0, oper = 0;
  while (i < opcodeLine.length()){
    if (opcodeLine == ""){break;}
    else{
      if(opcodeLine.at(i) == '<' && opcodeLine.at(i+1) == '-' ){
	oper = 1;
	UAtom1 = opcodeLine.substr(0,i);
	LAtom2 = parseOpcodeLine(opcodeLine.substr(i+2,opcodeLine.length()-1),Dopcode, lastIndx);
	if(LAtom2 == "end"){
	  LAtom2 = opcodeLine.substr(i+2,opcodeLine.length()-1);
	}
	Dopcode[lastIndx] ="00";
	Dopcode[lastIndx] =Dopcode[lastIndx] + '(' + UAtom1 + ',' + LAtom2 + ')';
	lastIndx++;
	return UAtom1;
      }
      if(opcodeLine.at(i) == '<' && opcodeLine.at(i+1) == '<'){
	oper = 2;
	UAtom1 = opcodeLine.substr(0,i);
	LAtom2 = parseOpcodeLine(opcodeLine.substr(i+2,opcodeLine.length()-1), Dopcode, lastIndx);
	if(LAtom2 == "end"){
	  LAtom2 = opcodeLine.substr(i+2,opcodeLine.length()-1);
	}
	Dopcode[lastIndx] ="01";
	Dopcode[lastIndx] =Dopcode[lastIndx] + '(' + UAtom1 + ',' + LAtom2 + ')';
	lastIndx++;
	return UAtom1;
      }
      if(opcodeLine.at(i) == '>' && opcodeLine.at(i+1) == '>'){
        oper = 3;
        UAtom1 = opcodeLine.substr(0,i);
	LAtom2 = parseOpcodeLine(opcodeLine.substr(i+2,opcodeLine.length()-1), Dopcode, lastIndx);
	if(LAtom2 == "end"){
	  LAtom2 = opcodeLine.substr(i+2,opcodeLine.length()-1);
	}
	Dopcode[lastIndx] ="02";
	Dopcode[lastIndx] =Dopcode[lastIndx] + '(' + UAtom1 + ',' + LAtom2 + ')';
	lastIndx++;
	return UAtom1;
      }
      if(opcodeLine.at(i) == '+'){
        oper = 4;
        UAtom1 = opcodeLine.substr(0,i);
	LAtom2 = parseOpcodeLine(opcodeLine.substr(i+1,opcodeLine.length()-1), Dopcode, lastIndx);
	if(LAtom2 == "end"){
	  LAtom2 = opcodeLine.substr(i+1,opcodeLine.length()-1);
	}
	Dopcode[lastIndx] ="03";
	Dopcode[lastIndx] =Dopcode[lastIndx] + '(' + UAtom1 + ',' + LAtom2 + ')';
	lastIndx++;
	return UAtom1;
      }
      if(opcodeLine.at(i) == '-'){
        oper = 5;
        UAtom1 = opcodeLine.substr(0,i);
	LAtom2 = parseOpcodeLine(opcodeLine.substr(i+1,opcodeLine.length()-1), Dopcode, lastIndx);
	if(LAtom2 == "end"){
	  LAtom2 = opcodeLine.substr(i+1,opcodeLine.length()-1);
	}
	Dopcode[lastIndx] ="04";
	Dopcode[lastIndx] =Dopcode[lastIndx] + '(' + UAtom1 + ',' + LAtom2 + ')';
	lastIndx++;
	return UAtom1;
      }
      if(opcodeLine.at(i) == '='){ //?
        oper = 6;
        UAtom1 = opcodeLine.substr(0,i);
	LAtom2 = parseOpcodeLine(opcodeLine.substr(i+1,opcodeLine.length()-1), Dopcode, lastIndx);
	if(LAtom2 == "end"){
	  LAtom2 = opcodeLine.substr(i+1,opcodeLine.length()-1);
	}
	Dopcode[lastIndx] ="05";
	Dopcode[lastIndx] =Dopcode[lastIndx] + '(' + UAtom1 + ',' + LAtom2 + ')';
	lastIndx++;
	return UAtom1;
      }
      if(opcodeLine.at(i) == 'i' && opcodeLine.at(i+1) == 'f'){
        oper = 7;
        UAtom1 = opcodeLine.substr(3,opcodeLine.find('=')-3);
	LAtom2 = opcodeLine.substr(opcodeLine.find('=')+2, opcodeLine.length());
	Dopcode[lastIndx] ="06";
	Dopcode[lastIndx] =Dopcode[lastIndx] + '(' + UAtom1 + ',' + LAtom2 + ')';
	lastIndx++;
	return UAtom1;
      }
      if(opcodeLine.at(i) == 'H' && opcodeLine.at(i+1) == 'A' && opcodeLine.at(i+2) == 'L' && opcodeLine.at(i+3) == 'T'){
	Dopcode[lastIndx] ="07";
	lastIndx++;
      }
      i++;
    }
  }
  if (oper == 0){return "end";}
}
int executeOpcode(int indx,std::string Dopcode[][MAXMICROC],int opcodeCount, ComponentList cl,std::string memory[]){
  int i=1, flag = 0;
  for(i; i<MAXMICROC; i++){
    std::string componente = "";
    if (Dopcode[indx][i] == ""){break;}
    if (Dopcode[indx][i].at(0) == '0' && Dopcode[indx][i].at(1) == '0') {//X <- Y
      std::string comp2 = Dopcode[indx][i].substr(Dopcode[indx][i].find(',') + 1, Dopcode[indx][i].find(')') - ((Dopcode[indx][i].find(',') + 1)));
      std::string comp1 = Dopcode[indx][i].substr(Dopcode[indx][i].find('(') + 1, Dopcode[indx][i].find(',') - (Dopcode[indx][i].find('(') + 1));
      std::cout << comp1 << " " << comp2 << std::endl;
      //      if (comp2.at(0) =='$'){
      //    cl.getComponent(comp1)->assign(std::stoi(comp2.substr(1, comp2.length()),nullptr, 16));
      //    flag = 1;}
      if (comp2.at(0) == '/') {
	comp2 = comp2.substr(1, comp2.length() - 1);
	if (cl.getComponent(comp2)->type == 1) {//Pegar na memoria o valor, no indice rep pelo valor de comp1
	  cl.getMdr(comp1)->assignFromMemory(memory, cl.getComponent(comp2)->value);
	  componente = comp1;
	  flag = 1;
	  //goto nxt;
	}
	if (cl.getComponent(comp2)->type == 5) {
	  int opc = cl.getComponent(comp2)->value / pow(16, (EXPONENT - 2));
	  std::cout << opc << std::endl;
	  cl.getComponent(comp1)->assign(opc);
	  componente = comp1;
	  flag = 1;
	}
      }
      if (comp2.at(0) == '\'') {
	comp2 = comp2.substr(1, comp2.length() - 1);
	if (cl.getComponent(comp2)->type == 5) {
	  int MSB = cl.getComponent(comp2)->value / pow(2, (cl.getComponent(comp2)->bits) / 2);
	  int MSBEND = (cl.getComponent(comp2)->value - (MSB * pow(2, (cl.getComponent(comp2)->bits) / 2)));
	  cl.getComponent(comp1)->assign(MSBEND);


	  flag = 1;
	}
      }
      if (comp1.at(0) == '/'){
	if(cl.getComponent(comp1.substr(1, comp1.length() - 1))->type == 1 && cl.getComponent(comp2)->type == 2){
          comp1 = comp1.substr(1, comp1.length() - 1);
          cl.getMar(comp1)->assignToMemory(memory, cl.getComponent(comp2)->value);
          flag = 1;
	}
      }
      if (cl.getComponent(comp1)->type == 6) {
	cl.getComponent(comp1)->assign(cl.getComponent(comp2)->value + 1);
	flag = 1;
      }
      if (flag != 1) {
	if (cl.getComponent(comp2)->type == 8) {
	  std::cout << (cl.getAlu(comp2))->r << std::endl;
	  cl.getComponent(comp1)->assign((cl.getAlu(comp2))->r);
	  std::cout << cl.getComponent(comp1)->value << std::endl;
	}
	else {
	  cl.getComponent(comp1)->assign(*cl.getComponent(comp2));
	}
      }
      componente = comp1;
    }
    if (Dopcode[indx][i].at(0) == '0' && Dopcode[indx][i].at(1)  == '1'){//X << Y
      std::string comp2  = Dopcode[indx][i].substr(Dopcode[indx][i].find(',')+1,Dopcode[indx][i].find(')') - ((Dopcode[indx][i].find(',') + 1)));
      std::string comp1  = Dopcode[indx][i].substr(Dopcode[indx][i].find('(')+1,Dopcode[indx][i].find(',') -(Dopcode[indx][i].find('(') + 1));
      if (cl.getComponent(comp1)->type != 8){std::cout << "Tipo Incorreto  " <<comp1 << "<<" << comp2 << std::endl; break;}
      if (comp2.at(0) =='$'){
	cl.getAlu(comp1)->bitShiftL(std::stoi(comp2.substr(1, comp2.length()),nullptr, 16));}
      else{cl.getAlu(comp1)->bitShiftL(cl.getComponent(comp2)->value);}
      componente = comp1;
    }
    if (Dopcode[indx][i].at(0) == '0' && Dopcode[indx][i].at(1)  == '2'){//X >> Y
      std::string comp2  = Dopcode[indx][i].substr(Dopcode[indx][i].find(',')+1,Dopcode[indx][i].find(')') - ((Dopcode[indx][i].find(',') + 1)));
      std::string comp1  = Dopcode[indx][i].substr(Dopcode[indx][i].find('(')+1,Dopcode[indx][i].find(',') -(Dopcode[indx][i].find('(') + 1));
      if (cl.getComponent(comp1)->type != 8){std::cout << "Tipo Incorreto  " <<comp1 << "<<" << comp2 << std::endl; break;}
      if (comp2.at(0) =='$'){
	cl.getAlu(comp1)->bitShiftR(std::stoi(comp2.substr(1, comp2.length()),nullptr, 16));}
      else{cl.getAlu(comp1)->bitShiftR(cl.getComponent(comp2)->value);}
      componente = comp1;
    }
    if (Dopcode[indx][i].at(0) == '0' && Dopcode[indx][i].at(1)  == '3'){cl.alu->add();componente = "ALUr";} //X + Y somente usado em ALUx e ALUy, com output em ALUr
    if (Dopcode[indx][i].at(0) == '0' && Dopcode[indx][i].at(1)  == '4'){cl.alu->sub();componente = "ALUr";} //X - Y somente usado em ALUx e ALUy, com output em ALUr
    if (Dopcode[indx][i].at(0) == '0' && Dopcode[indx][i].at(1)  == '5'){} //X = Y ??
    if (Dopcode[indx][i].at(0) == '0' && Dopcode[indx][i].at(1)  == '6'){ //if(PSRx == Y) somente utilizar em flags
      if (Dopcode[indx][i].at(6) == 'v'){
	std::cout << cl.alu->overflow << "     " << Dopcode[indx][i].at(8) << std::endl;
	if (cl.alu->overflow != Dopcode[indx][i].at(8) - '0'){return 1;}
      }
      if (Dopcode[indx][i].at(6) == 'z'){
	//std::cout << cl.alu->zero << "     " << Dopcode[indx][i].at(8) << std::endl;
	if (cl.alu->zero != Dopcode[indx][i].at(8) - '0'){return 1;}
      }
      if (Dopcode[indx][i].at(6) == 'n'){
	std::cout << (cl.alu->negative == Dopcode[indx][i].at(8)-'0') << std::endl;
	if (cl.alu->negative != Dopcode[indx][i].at(8)-'0'){return 1;}
      }
      if (Dopcode[indx][i].at(6) == 'e'){
	std::cout << cl.alu->equal << "     " << (Dopcode[indx][i].at(8) -'0') << std::endl;
	if (cl.alu->equal != Dopcode[indx][i].at(8) - '0'){return 1;}
      }
    }
    if (Dopcode[indx][i].at(0) == '0' && Dopcode[indx][i].at(1)  == '7'){
      return 2;}
    flag = 0;

    

    //cl.cu->window->clean_gui();
    // cl.cu->window->update_gui(componente);
    qApp->processEvents();
    QThread::msleep(100);
    cl.cu->window->clean_gui();
  }
  return 0;
}

void fetch(std::string memory[], std::string Dopcodes[][MAXMICROC], int opcodeCount, ComponentList cl){
  executeOpcode(0, Dopcodes, opcodeCount, cl, memory);
}
void executeMemory(std::string memory[], std::string Dopcodes[][MAXMICROC], int opcodeCount, ComponentList cl){
  while(1!=0){
    fetch(memory, Dopcodes, opcodeCount, cl);
    if (executeOpcode(cl.getComponent("CU")->value, Dopcodes, opcodeCount, cl, memory) == 2){break;}
  }
}

void MainWindow::on_MEM_GO_clicked(){
  cl.reset_values();
  clean_gui();
  reset_all_comp_values();
  executeMemory(mem,Dopcode,opcodeCount ,cl);
}
//void MainWindow::update_gui(std::string comp){
//  QString propiedades = "border: 2px solid green";
//  if (comp == ""){return;}
//  if (comp == "ALUr" || comp == "ALU"){
//    ALU *current = cl.getAlu(comp);
//    cl.getComponent(comp)->display->setStyleSheet(propiedades);
//    setDisplay(cl.getComponent(comp));
//    //std:: << "zero flag" << cl.getAlu(comp)->zero << std::endl;
//    //cl.getComponent(comp)->window->ui->DEBUG_DISPLAY->setText(QString::number(cl.alu->zero));
//    //cl.alu->window->ui->DEBUG_DISPLAY->setText(QString::number(cl.alu->zero,16));
//    setZero(current->zero);
//    //std::cout << "equal flag" << current->equal << std::endl;
//    setEqual(current->equal);
//    setNegative(current->negative);
//    setOverflow(current->overflow);
//  }else{
//    cl.getComponent(comp)->display->setStyleSheet(propiedades);
//    setDisplay(cl.getComponent(comp));
//  }
//}
void MainWindow::reset_all_comp_values(){
  cl.reset_values();
  update_all_components();
}
void MainWindow::clean_gui(){
  QString propiedades = "border: 1px solid grey";
  ui->A_DISPLAY->setStyleSheet(propiedades);
  ui->B_DISPLAY->setStyleSheet(propiedades);
  ui->ALUx_DISPLAY->setStyleSheet(propiedades);
  ui->ALUy_DISPLAY->setStyleSheet(propiedades);
  ui->ALUr_DISPLAY->setStyleSheet(propiedades);
  ui->ALUr_DISPLAY->setStyleSheet(propiedades);
  ui->IR_DISPLAY->setStyleSheet(propiedades);
  ui->PC_DISPLAY->setStyleSheet(propiedades);
  ui->SP_DISPLAY->setStyleSheet(propiedades);
  ui->CU_DISPLAY->setStyleSheet(propiedades);
  ui->INC_DISPLAY->setStyleSheet(propiedades);
  ui->MAR_DISPLAY->setStyleSheet(propiedades);
  ui->MDR_DISPLAY->setStyleSheet(propiedades);
  ui->OVERFLOW_DISPLAY->setStyleSheet(propiedades);
  ui->ZERO_DISPLAY->setStyleSheet(propiedades);
  ui->EQUAL_DISPLAY->setStyleSheet(propiedades);
  ui->NEGATIVE_DISPLAY->setStyleSheet(propiedades);
}
void MainWindow::update_mem(unsigned short addr){
  ui->MEMORY_VIEW->item(addr)->setText(QString::number(addr,16) + "\t"+  QString::fromStdString(mem[addr]));
  ui->MEMORY_VIEW->item(addr)->setSelected(1);
  //    std::cout << "addr:   " << addr << std::endl;
  //    std::cout << "mem:   " << mem[addr] << std::endl;
  //    ui->MEMORY_DISPLAY->setText("");
  //for (int i=0; i<MEMSIZE; i++){
  //  ui->MEMORY_DISPLAY->setText(ui->MEMORY_DISPLAY->toPlainText() +"\n" + QString::fromStdString(mem[i]));
  //}
}
void MainWindow::update_all_components(){
  setA(A.value);
  setB(B.value);
  setALUx(ALUx.value);
  setALUy(ALUy.value);
  setALUr(alu.value);
  setIR(IR.value);
  setPC(PC.value);
  setSP(SP.value);
  setCU(CU.value);
  setINC(INC.value);
  setMAR(MAR.value);
  setMDR(MDR.value);
  setOverflow(alu.overflow);
  setZero(alu.zero);
  setEqual(alu.equal);
  setNegative(alu.negative);
}
void MainWindow::on_REPL_INPUT_returnPressed(){
  if (ui->REPL_INPUT->text().length() != EXPONENT){ui->REPL_DISPLAY->append(ui->REPL_INPUT->text() +"    erro:Input deve ter exatamente " +QString::number(EXPONENT,10)+" numeros (base 10)"+ "\n"); return;}
  unsigned int input = ui->REPL_INPUT->text().toInt(nullptr, 16);
  IR.value = input;
  CU.value = input/pow(16,EXPONENT -2);
  ui->REPL_DISPLAY->append(ui->REPL_INPUT->text() + ": " + QString::fromStdString(Dopcode[(int)(input/pow(16,EXPONENT -2))][0]));
  //this->update_all_gui();
  ui->REPL_INPUT->setText("");
  qApp->processEvents();
  executeOpcode(input/pow(16,EXPONENT -2),Dopcode,opcodeCount,cl,mem);
}
void MainWindow::on_CLEAR_BUTTON_clicked(){
  reset_all_comp_values();
  qApp->processEvents();
}
void MainWindow::on_MEM_LOCATION_returnPressed(){

  memCount = loadMem(mem,(ui->MEM_LOCATION->text()).toStdString());
  //std::cout << this->ui->MEM_LOCATION->text().toStdString() << std::endl;
  clean_gui();
  ui->MEMORY_DISPLAY->setText("");
  for (int i=0; i<MEMSIZE; i++){
    ui->MEMORY_DISPLAY->setText(ui->MEMORY_DISPLAY->toPlainText() +"\n" + QString::fromStdString(mem[i]));
    ui->MEMORY_VIEW->item(i)->setText(QString::number(i,16)+ "\t" +  QString::fromStdString(mem[i]));
  }
}
void MainWindow::on_OPCODE_LOCATON_returnPressed(){
  clean_gui();
  ui->OPCODE_DISPLAY->setText("");
  loadOpcode(opcode,(ui->OPCODE_LOCATON->text()).toStdString());
  opcodeCount = parseAllOpcodes(opcode,Dopcode);
  //std::cout << "parsed" << std::endl;
  for (int i=0; i<opcodeCount; i++){ui->OPCODE_DISPLAY->setText(ui->OPCODE_DISPLAY->toPlainText() +"\n" + QString::fromStdString(opcode[i][0]));}
}
void MainWindow::on_MEM_FETCH_clicked(){
  fetch(mem, Dopcode, opcodeCount, cl);
}
void MainWindow::on_MEM_EXECUTE_clicked(){
  executeOpcode(CU.value, Dopcode, opcodeCount, cl, mem);
}
void MainWindow::on_MEMORY_VIEW_itemChanged(QListWidgetItem *item)
{
  /*PODE SER CASE SENSITIVE*/
  //int indx= item->text().left(item->text().indexOf("\t")).toInt(nullptr,16);
  mem[item->text().left(item->text().indexOf("\t")).toInt(nullptr,16)] = item->text().right(EXPONENT).toStdString();
  std::cout <<"3:  "<<  mem[item->text().left(item->text().indexOf("\t")).toInt(nullptr,16)] << std::endl;
  //item->setText(QString::number(indx,16)+ "\t" + item->text());
  //for (int i=0; i<MEMSIZE; i++){ui->MEMORY_DISPLAY->setText(ui->MEMORY_DISPLAY->toPlainText() +"\n" + QString::fromStdString(mem[i]));}
}

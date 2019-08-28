/*  
               Driver para expansor i2c MCP23017   
*/
//***********************************************************************************
//                     Libreria MCP23017
//***********************************************************************************
//   MCP23017_init(void);   Llamada para inicialisar MCP23017 con BANK 0 como default   
//   _set_tris_a(int);      Configura puerto GPIOA como entrada o salida, 0:out, 1:in
//   _set_tris_b(int);      Configura puerto GPIOB como entrada o salida, 0:out, 1:in
//   pull_up_a(int);        Configura pull up puerto GPIOA 1: enable, 0: disable
//   pull_up_b(int);        Configura pull up puerto GPIOB 1: enable, 0: disable
//   read_int_a(void);      Lee causa de interrupcion port GPIOA
//   read_int_b(void);      Lee causa de interrupcion port GPIOB
//   _output_a(int);        Saca por el port GPIOA el valor entregado
//   _output_b(int);        Saca por el port GPIOB el valor entregado
//***********************************************************************************   
//   Configuracion de pines por software bus i2c
#ifndef _SDA
#define   _SDA     PIN_B1
#define   _SCL     PIN_B0
#define   _RES     PIN_B2   
#endif
//   Selecciona direccion de chip
#ifndef   CHIP
#define   CHIP_0   0
#define   CHIP_1   1
#define   CHIP_2   2
#define   CHIP_3   3
#define   CHIP_4   4
#define   CHIP_5   5
#define   CHIP_6   6
#define   CHIP_7   7
#endif
//   Reset para MCP23017
#define   RESET_MCP      output_low(_RES)   // Resetea MCP23017
#define   NO_RESET_MCP   output_high(_RES)  // Activa MCP23017
// Banco por defecto
# define    BANK   
//  Si no se elige configuracion de banco 1 se selecciona el banco 0 por default
#ifdef  BANK
# define    IODIRA_ADD      0x00
# define    IODIRB_ADD      0x01
# define   IPOLA_ADD      0x02
# define   IPOLB_ADD      0x03
# define    GPINTENA_ADD   0x04
# define    GPINTENB_ADD   0x05
# define   DEFVALA_ADD      0x06
# define   DEFVALB_ADD      0x07
# define    INTCONA_ADD      0x08
# define    INTCONB_ADD      0x09
# define   IOCONA_ADD      0x0A
# define    IOCONB_ADD      0x0B
# define    GPPUA_ADD      0x0C
# define    GPPUB_ADD      0x0D
# define   INTFA_ADD      0x0E
# define   INTFB_ADD      0x0F
# define    INTCAPA_ADD      0x10
# define    INTCAPB_ADD      0x11
# define   GPIOA_ADD      0x12
# define   GPIOB_ADD      0x13
# define   OLATA_ADD       0x14
# define   OLATB_ADD      0x15
#else
# define    IODIRA_ADD      0x00
# define   IPOLA_ADD      0x01
# define    GPINTENA_ADD   0x02
# define   DEFVALA_ADD      0x03
# define    INTCONA_ADD      0x04
# define   IOCONA_ADD      0x05
# define    GPPUA_ADD      0x06
# define   INTFA_ADD      0x07
# define    INTCAPA_ADD      0x08
# define   GPIOA_ADD      0x09
# define   OLATA_ADD       0x0A
# define    IODIRB_ADD      0x10
# define   IPOLB_ADD      0x11
# define    GPINTENB_ADD   0x12
# define   DEFVALB_ADD      0x13
# define    INTCONB_ADD      0x14
# define   IOCONB_ADD      0x15
# define    GPPUB_ADD      0x16
# define   INTFB_ADD      0x17
# define    INTCAPB_ADD      0x18
# define   GPIOB_ADD      0x19
# define   OLATB_ADD      0x1A
#endif
//    Si no se define configuracion de IOCONA desde principal lo configura por defecto
#ifndef   IOCONA
#define   IOCONA      0b00001011
#endif
//
#use i2c(master,sda=_SDA,scl=_SCL,slow)
// Prototipos de funciones
void MCP23017_init(void);
int device_opcode(int adress, short int rw);
int read_registro(int address);
void write_registro(int reg, int address);
void outporta(int data);
void outportb(int data);
void _set_tris_a(int porta);
void _set_tris_b(int portb);
void select_bank(void);
int read_int_a(void);
int read_int_b(void);
void pull_up_a(int pull);
void pull_up_b(int pull);
void _output_a(int outa);
void _output_b(int outb);
void _set_ipola(int data);
void _set_ipolb(int data);

void MCP23017_init(void){
      output_float(_SDA);
      output_float(_SCL);
      NO_RESET_MCP;
   select_bank();
}

int device_opcode(int adress, short int rw){
int adress_high=4;

   adress=(adress<<1)|rw;
   adress_high=adress_high<<4;
   adress=adress_high|adress;   
   return adress;         
}

int read_registro(int address){
int opc_w, opc_r;
int data;

   opc_w=device_opcode(CHIP_0,0);
   opc_r=device_opcode(CHIP_0,1);
   i2c_start();
   i2c_write(opc_w);
   i2c_write(address);
   i2c_start();
   i2c_write(opc_r);
   data=i2c_read(0);
   i2c_stop();
   return data;
}

void write_registro(int reg, int address){
int opc;
   opc=device_opcode(CHIP_0,0);
   i2c_start();
   i2c_write(opc);
   i2c_write(address);
   i2c_write(reg);
   i2c_stop();
}

void _set_ipola(int data){
   write_registro(data,IPOLA_ADD);
}

void _set_ipolb(int data){
   write_registro(data,IPOLB_ADD);
}

void _set_defvala(int data){
   write_registro(data,DEFVALA_ADD);
}

void _set_defvalb(int data){
   write_registro(data,DEFVALB_ADD);
}

void outporta(int data){
   write_registro(data,OLATB_ADD);
}

void outportb(int data){
   write_registro(data,OLATA_ADD);
}

void _set_tris_a(int porta){
   write_registro(porta,IODIRA_ADD);
   delay_ms(15);
   write_registro(porta,GPINTENA_ADD);
   outporta(porta);   
}

void _set_tris_b(int portb){
   write_registro(portb,IODIRB_ADD);
   delay_ms(15);
   write_registro(portb,GPINTENB_ADD);
   outportb(portb);   
}

void select_bank(void){
   write_registro(IOCONA,0x0A);
   delay_ms(5);
}

int read_int_a(void){
int port;
      port=read_registro(INTCAPA_ADD);
   read_registro(INTFA_ADD);
      return port;
}

int read_int_b(void){
int port;
   port=read_registro(INTCAPB_ADD);
   read_registro(INTFB_ADD);
   return port;
}

void pull_up_a(int pull){
   write_registro(pull,GPPUA_ADD);
}

void pull_up_b(int pull){
   write_registro(pull,GPPUB_ADD);
}

void _output_a(int outa){
   write_registro(outa,GPIOA_ADD);
}

void _output_b(int outb){
   write_registro(outb,GPIOB_ADD);
}
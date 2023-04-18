void serial_init(unsigned short);
void serial_stringout(char *);
unsigned char recv_string(char *);
void finish_recv();

#define SERIAL_START '['
#define SERIAL_END   ']'

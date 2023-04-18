void serial_init(unsigned short ubrr_value);
void serial_txchar(char ch);
void serial_stringout(char *s);
unsigned char recv_string(char *rp);
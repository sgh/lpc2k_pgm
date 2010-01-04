extern void open_serial_port(const char *port_name);
extern int write_serial_port(const void *buf, int num);
extern void input_flush_serial_port(void);
extern int read_serial_port_nb(unsigned char *buf, int bufsize);
extern void close_serial_port(void);
extern void send_break_signal(void);
extern int set_baud(const char *baud_name);
extern int serial_port_fd(void);
extern void set_rts(int val);
extern void set_dtr(int val);


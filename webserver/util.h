#ifndef _UTIL_H
#define _UTIL_H

void sys_error(const char *msg);
int get_line(int sock,char *buf,int size);
int hexit(char c);
void encode_str(char* to, int tosize, const char* from);
void decode_str(char *to, char *from);
void send_respond_head(int cfd, int no, const char* desp, const char* type, long len);
void send_error(int cfd, int status, char *title, char *text);
void send_file(int cfd,const char*file);
const char *get_file_type(const char *name);
void send_dir(int cfd, const char* dirname);
#endif
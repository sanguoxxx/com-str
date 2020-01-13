/*
comstr.h
简单的字符串处理函数

规则
	都以"str_"开头
注意
	由于用到了static char*,
	所以,1个以上的str_函数套用时,可能会出问题
测试
	
修改
    2002-02-14 ver1.0
    2005-02-14 ver1.1
    2005-08-25 ver1.2
    2016-01-27 merge comstr2.h comstr4.h comstr5.h
*/

#ifndef COM_STR_H
#define COM_STR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include "comint.h"

//-----------------------------------------------------------------------------
#ifndef MAX
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#endif

#ifndef MIN
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#endif

#ifndef TSWAP
#define TSWAP(a,b,t) { t=a; a=b; b=t; }
#endif

#ifndef STR_IS_SPACE
#define STR_IS_SPACE(c) ('\n'==(c) || '\r'==(c) || ' '==(c) || '\t'==(c))
#endif

#ifndef STR_IS_RETURN
#define STR_IS_RETURN(c) ('\n'==(c) || '\r'==(c))
#endif

#ifndef STR_IS_UNIX
#define STR_IS_UNIX(p)		('\n'==(*p))
#define STR_IS_WINDOWS(p)	('\r'==(*p) && '\n'==*(p+1))
#endif

#ifndef STR_IS_BWORD
#define STR_IS_BWORD(c)		('_'==c || isalpha(c))
#define STR_IS_MWORD(c)		('_'==c || isalnum(c))
#endif

#ifndef STR_IS_QUOTO
#define STR_IS_QUOTO(c)  	('\''==(c) || '\"'==(c))
#define STR_IS_SQUOTO(c) 	('\''==(c))
#define STR_IS_DQUOTO(c) 	('\"'==(c))
#endif

#ifndef IF_STR_STR
#define IF_STR_STR(s,t)  if(strstr(s,t))
#define IF_STR_CMP(s,t)  if(0==strcasecmp(s,t))
#define IF_STR_NCMP(s,t) if(0==strncasecmp(s,t,strlen(t)))
#endif

#ifndef STR_APPEND
#define STR_APPEND(str,fmt,...) sprintf(str+strlen(str),fmt, ## __VA_ARGS__ );
#endif

#ifndef STR_STDERR
#define STR_STDERR(fmt,...) fprintf(stderr,fmt, ## __VA_ARGS__ );
#endif

typedef int (*df_test_char)(int);

//-----------------------------------------------------------------------------
int   	str_is_space(char c);
int   	str_is_space(const char* s);
int	  	str_is_begin_with(const char* s,const char* t,bool sensitive=1);
int	  	str_is_end_with(const char* s,const char* t,bool sensitive=1);
      	
char* 	str_trim(char *str);
		//return after trim str.not change str itself
char* 	str_trim_self(char str[]);
		//return after trim str.change str itself
char*	str_trim_self_quick(char* str);
char* 	str_trim_left(char str[]);	
		//只去掉左｜右的空格
char* 	str_trim_left_to(char str[],char* dst);
char* 	str_trim_right(char str[]);
char* 	str_trim_cont(char str[],char c=' ');
char* 	str_trim_cont_space(char* str);
char* 	str_trim_cont_real_space(char* buf);
char*   str_trim_quote(char* buf, char c='"');
char* 	str_trim_lines(char* str);
char* 	str_trim_config_line(char* buf,bool balnum=1);
	  	//return 0:fail other:ok
char* 	str_end_space(char *s);
		//去掉首空格并以空格结尾 ' 12 3' -> 12
char* 	str_end_with(char* s,char c);
		//如果s含有c,则以c结尾
int   	str_enclose(char* s,bool ispair=1);
      	
int   	str_strstr(const char* str,const char* pat);
int   	str_strstr(const char *str,const char* pat,int* f);
bool	str_casestr(const char* x,const char* y);
bool  	str_match(const char* input,const char* pattern);
bool 	str_find_word(const char* s,const char* d);
int		str_times(const char* s,char c,bool continious=0);

void  	str_get_fail_string(const char *pat, int* f);
int   	str_freq(const char* str,char c);
int   	str_freq(const char* str,const char* mask);
char* 	str_replace(char *str,const char *str1,const char *str2);
char* 	str_replace(char *str,char c,const char *str2);	
	  //将str中的c换成s,str本身不变
	  //注意:返回的字符串是额外分配的内存,必须被free()掉

char* 	str_replace(char *str,char c,char c2);	
char* 	str_replace(char *str,const char* c,int x,int n);	
	  //将str中的c换成x(格式为%nd)

char* 	str_replace_tab(char* str);
      	
void  	str_free(char* goal[],int n);
void    str_free_array(int n,char** goal);
char** 	str_alloc_array(int row,int collen);

int   	str_parse(const char* str,const char* sep,char* goal[],bool repeat=0);
int   	str_parse(const char* str,char sep, char* goal[],bool repeat=0);
int   	str_strtok(const char* str,const char* sep,char* gaol[],bool method_repeat=0);
int   	str_strtok(const char* str,char sep,char* goal[],bool method_repeat);
int   	str_strtok(const char* buf,char sep,int ngoal, char** goal);
        //here, goal need to call free() after use
int   	str_strtok2(const char *buf,char sep,int ngoal,char rtn[][256],bool ignorecontinous=1,bool combinelast=0);
	  //将str以sep为分隔符，返回字符串数组于goal中
	  //[1] method_repeat:连续2个sep出现,如何计算？
	  //	  0: 不计算  1:计算
	  //    用strdup(),goal可申明为char* goal[?]即可,不用手动分配空间
	  //    但每次调用后,都必须用str_free().
	  //    适于数据小的情况
	  //[2] 用strcpy(),goal必须事先手动分配空间.最后手动删除
	  //    适于频繁调用,大数据的情况
	  //expand for str_strtok2()
	  //when sep is in a string,it should be parsed as one column.
	  //eg. 'welcome,beijing friend'

int   	str_strtok_quote(const char* buf,char sep,int ngoal,char rtn[][256],char quote='"');
int   	str_strtok_self(char buf[],char sep,int ngoal, int rtn[]); //record position in buf
int   	str_strtok_self(char buf[],char sep,int ngoal, char** rtn); //char** rtn=new char* [N];
        //_self: will modify the content of buf
int   	str_strtok_line(const char* str,int nvs,char vs[][256],bool keepempty=0);
int  	str_strtok_scope(const char *buf,char sep1,char sep2,int ngoal,char from[][256],char to[][256]);
char*  	str_field(const char* str,char delim,int col,char* out=0,bool isall=0);
	  	//只将str的第n列取出去
	  	//if out is NULL, return inner buffer
int		str_devide(const char* str,char delim,char* s1=0,char* s2=0,char* s3=0,char* s4=0,char* s5=0,char* s6=0,char* s7=0);
int	  	str_two_value(const char* buf,int &v1, int &v2);
	  //从输入字符串中挑出2个正整数
	  //如: 从scope: 5 - 60 得到v1=5,v2=60

int	  	str_scope_to_index(const char* str,bool* array,int maxdex,bool reverse=0,char delim1='-',char delim2=',');
int	  	str_scope_to_array(const char* str,int* array,int narray,bool reverse=0,char delim1='-',char delim2=',');
int	  	str_scope_to_array_hex(const char* str,int* array,int narray,bool reverse=0,char delim1='-',char delim2=',');
	  	//return the number of array items
	  
char* 	str_trip_begin(char* str,char c);		
char* 	str_trip_end(char* str,const char* c);	
char* 	str_trip_end(char* str,char c='\n');	
char* 	str_trip_line(char* str);
char* 	str_trip_tab(char* str,bool all);		
char* 	str_add_end(char* str,char c='\n');
char* 	str_remove_slant(char *path);
char* 	str_remove(char* s,const char* c);		
char* 	str_remove(char s[],char c=' ');	
char* 	str_remove_any(char* s,const char* c);
char* 	str_remove_space(char* s);
char*	str_remove_between(char* s,char x,char y);
char*	str_remove_note_lines(char* s);
char*	str_remove_begin(char* s,char c);
char*	str_remove_end(char* s,char c);
char*	str_remove_right(char* s,char c,bool includeself=1);
char* 	str_normalize_space(char* s);
char* 	str_normalize_line(char* s);
char* 	str_normalize_config_line(char* s);

char* 	str_join(int num,...);			
char* 	str_join(const char* format,...);
char* 	str_cat(char* s,const char* format,...);
char* 	str_combine(int n, char *words[],char* sep);
	  //注意:额外分配了内存

char* 	str_norm(char* src,char ori,int width);  
	  //mode:对齐方式['r'<-right |'l'<-left |'c'<-center]

char* 	str_append(char* a,const char* b, int n);
char*	str_append(char* s,char c, int n);
	  //重新分配fst的空间，并加上snd

char* 	str_left(const char* str,const char* mid,bool inc=0);	
char* 	str_right(const char* str,const char* mid,bool inc=0);
char* 	str_mid(const char* str,const char* left,const char* right);
char* 	str_left(const char* str,char mid,bool inc=0);
char* 	str_leftn(const char* str,char mid,int n,bool inc=0);
char* 	str_right(const char* str,char mid,bool inc=0);
char* 	str_rleft(const char* str,char mid,bool inc=0);
char* 	str_right(const char* str,char mid,int n,bool inc=0);
char* 	str_rright(const char* str,char mid,bool inc=0);
char* 	str_sub(const char* str,int left,int right);
char* 	str_sub_self_between(char* str,int left,int right);
char* 	str_sub_self_many(char* str,int from,int many);
char* 	str_sub(int x,int n);

int   	str_ncmp(const char* s1,const char* s2);
int   	str_ncasecmp(const char* s1,const char* s2);
int   	str_tail_cmp(const char* s1,const char* s2);
int   	str_rcmp(const char* s1,const char* s2);
int   	str_rcasecmp(const char* s1,const char* s2);
int   	str_rncmp(const char* s1,const char* s2,int len);
int   	str_rncasecmp(const char* s1,const char* s2,int len);
      	
int   	str_case_strstr(const char* s,const char* t);
int   	str_case_compare(const char* s,const char* t,const char* how=0);
      	
char* 	str_upper(char* s);	
char* 	str_lower(char* s);	
char* 	str_upper(const char* s,char* d);
char* 	str_lower(const char* s,char* d);
char* 	str_unix_to_win(char buff[]);	
char* 	str_win_to_unix(char buff[]); 

bool 	str_is_note(const char* s);      	
bool  	str_is_chinese(int ch);	
bool  	str_is_ctrl(int c);		
bool  	str_is_char(int c);		
bool  	str_is_upper(const char* str);
bool  	str_is_lower(const char* str);	
bool  	str_is_comment(const char* str);
bool  	str_is_empty(const char* str);	
int   	str_is_empty(char* s, char* &p); 
bool  	str_is_prefix(const char* s, const char* pre);	
bool  	str_is_suffix(const char* s, const char* suf);	
bool  	str_is_equal(const char* p,const char* q);
bool  	str_is_common(const char* p,const char* q);
bool	str_is_digit(const char* s);
bool	str_is_digit(const char* s,int n);
bool	str_is_date(const char* s);

bool	str_has_digit(const char* s);
bool	str_has_alpha(const char* s);
bool	str_has_alnum(const char* s);
bool	str_has_ascii(const char* s);
bool	str_has_xdigit(const char* s);
bool	str_has_blank(const char* s);
bool	str_has_pair(const char* s,char x,char y);

df_test_char str_get_isfunc(const char* type);
int   	str_meet_type(const char* str,const char* type);
int		str_not_meet_type(const char* str,const char* type);

extern "C" 
{
int   	str_cmp_asc(const void* p1,const void* p2);
int   	str_cmp_desc(const void* p1,const void* p2);
};    	
int   	str_ncmp_byint(const char* s1,const char* s2,int len); 
void  	str_sort(int size,char* array[],bool asc=1);	

void  	str_repeat(int n,const char* c);
void  	str_print(const char* format, ...);
char*	str_nprint(const char* str, int n);
void    str_nprintf(const char* str,int n);
char* 	str_sprint(char* str,const char *format, ...);
void  	str_fprint(FILE* fp,const char *format, ...);

void  	str_error(int errnum);
void  	str_error();
void  	str_die(const char *format, ...);	
void  	str_warn(const char *format, ...);	
void  	str_usage(const char* name,const char* use,bool exitit);	
void  	str_usage(const char* use,bool exitit) ;
void  	str_usage(int num,const char* use[],bool exitit);
void	str_debug(const char* str);
      	
char* 	str_to_time(long data);
char* 	str_file_by_date(const char* prefix,const char* suffix);
char* 	str_file_by_date();
      	
char* 	str_basename(const char *file,char sep='/');
      	
u32 	str_hash(const char* data, int n, u32 seed=0); //quickest
size_t 	str_hash1(const char* s,int len);
size_t 	str_hash2(const char* s,int len);
size_t 	str_hash3(const char* s,int len);

int   	str_len(int n);

int   	str_from_shell(const char* sh,char *buf,int len);
 	  //挺耗费时间, 循环5k -> 用时15s
 	  //结果返回到buf中
 	  	
int   	str_system(int mode,const char* format,...);
	  //mode 0:normal 1:bg 2:fg hide err 3:fg hide output
	  	
void  	str_reverse_part(char str[],int s,int e);
void  	str_reverse_self(char str[]);
void  	str_reverse_word(char str[]);
      	
//for xml
char* 	str_skip_space(char*& str);
int   	str_skip_line(char*& str);
char* 	str_skip_empty(char*& str);
char* 	str_skip_note(char*& str);
char* 	str_meet_space(char*& str);
char* 	str_meet_newline(char*& str);
int   	str_meet_string(char*& str,char* what,int nwhat);
char* 	str_get_word(char* str,char* word);
char* 	str_limit_length(char* str,char* out,int outlen,const char* ends="\n",int maxs=1);

uint32_t str_to_ipaddr(const char* str);
int		str_to_ip_and_port(const char* str, char* ip,int& port);
int		str_is_integer(const char* str);
bool    str_is_ipaddr(const char* str);
int     str_is_ipport(const char* str);

int   	str_is_tolerant_int(const char* str);
int   	str_get_tolerant_int(const char* str);
int   	str_strtok_tolerant_int(const char* str,int nv,int* plist,char sep=',');
      	
int   	str_fill_left(const char* src,char* dst,int len);
int   	str_fill_right(const char* src,char* dst,int len);
char* 	str_between_quota(const char* str,char* ret);
int	  	str_to_int(const char* str);
int   	str_hex_to_int(const char* str);
int64_t	str_hex_to_int64(const char* str);
int   	str_oct_to_int(const char* str);
int   	str_bin_to_int(const char* str);
int   	str_dec_to_int(const char* str);

char*   str_byte_to_mb(u64 x,char* str);
char*   str_byte_to_gb(u64 x,char* str);
char*   str_byte_to_tb(u64 x,char* str);
char*   str_byte_to_pb(u64 x,char* str);

char*   str_sec_to_day(int sec,char* str);
char* 	str_safe_truncate(char* input,int len,char cs=0);

char*	str_swap(char* str,int len);

char*	tostr(char x);
char*	tostr(unsigned char x);
char*	tostr(short x);
char*	tostr(unsigned short x);
char*	tostr(int x);
char*	tostr(unsigned int x);
char*	tostr(int64_t x);
char*	toip(u32 x);
char* 	ftoa(double val,int keep,char* ret,const char* mode=0);
		//mode u:up:进位 d:down:舍弃 r:round:5舍6入
int		str_get_id(const char* s,int where=1);

void 	str_print_hex(const void* input,int len, int colsize=16);
char* 	str_print_hex_to(const void* input,int len, char* str, char delim=' ');

char*	str_print_array_to(const int* parr,int narr,char* str,int len);
char*	str_print_array_to_if(const int* parr,int narr,char* str,int len);
char* 	str_print_array_reverse(const int* parr,int narr,char* str,int len);

#endif

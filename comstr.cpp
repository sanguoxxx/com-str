#include <time.h>
#include <errno.h>

#include "comstr.h"

#define XBUFSIZ 0x100000 //65536
static char str_cb_buf[XBUFSIZ]; 

#define STR_GET_CB_BUF(fmt)	 	\
    va_list vl;					\
   	va_start(vl,fmt);			\
   	vsprintf(str_cb_buf,fmt,vl);\
   	va_end(vl);

//--------------------------------------------------------------
// 1.对str的形式而言:
//   a.char *str =" 123 ",此时str是虚拟地址,不能用strcpy
//   b.char str[]=" 123 ",此时str为物理地址,能用strcpy
//   c.char *str; str=strdup(" 123 ").同2
//   所以,只能用某指针 *p 来进行访问
// 2.关于返回值:
//	 由于返回的是临时地址tmp,如果要保留其内容,必须马上用
//   strcpy等拷贝下来
// 3.测试
//   char *s="  l love my country ";
//   printf("[%s]\n", str_trim_left( s ) );
//   printf("[%s]\n", str_trim_right( s ) );
//   printf("[%s]\n", str_trim_right( str_trim_left(s) ) );
//--------------------------------------------------------------

char* str_trim(char *str)
{
    if(!str) return (char*)"";
    if(!str[0]) return (char*)"";
    
    strcpy(str_cb_buf,"");
    
    char *p1=str;
    char *p2=&str[strlen(str)-1];
    char *p3=str_cb_buf;
    
    while(p1<=p2 && STR_IS_SPACE(*p1)) p1++;
    
    while(p1<=p2 && STR_IS_SPACE(*p2)) p2--;
    p2++;
    
    while( p1 != p2 ) *p3++ = *p1++;
    *p3=0;
    
    return str_cb_buf;
}

char* str_trim_left(char str[])
{
    if(!str[0]) return (char*)"";
    if(!STR_IS_SPACE(str[0])) return str;
    char *p=str;
    char* q=str; 
    while(*p && STR_IS_SPACE(*p)) p++;
    while(*p) *q++=*p++;
    *q=0;
	return str;
}
char* str_trim_left_to(char str[],char* dst)
{
	*dst=0;
    if(!str[0]) return dst;
	char *q=dst;
    char *p=str;
    while(*p && STR_IS_SPACE(*p)) p++;
    while(*p) *q++=*p++;
    *q=0;
	return dst;
}

char* str_trim_right(char str[])
{
    if(!str[0]) return (char*)"";
    int   n=strlen(str)-1;
    char *p=str+n;
	while(p >=str && STR_IS_SPACE(*p)) { p--; }
	*(p+1)=0;
	return str;
}
char* str_trim_self(char str[])
{
    if(0==str[0]) return (char*)"";
    
    int n=strlen(str);
    char *p1=str;
    char *p2=&str[n-1];
    char *p3=str;
    
    while(p1<=p2 && STR_IS_SPACE(*p1)) p1++;
    
    while(p1<=p2 && STR_IS_SPACE(*p2)) p2--;
    p2++;
    
    while( p1 != p2 ) *p3++ = *p1++;
    *p3=0;
    
    return str;
}
char* str_trim_self_quick(char* str)
{
    if(0==str[0]) return str;

    int n=strlen(str);
    char *p1=str;
    char *p2=&str[n-1];

    while(p1<=p2 && STR_IS_SPACE(*p1)) p1++;
    
    while(p1<=p2 && STR_IS_SPACE(*p2)) p2--;
    p2++;
    *p2=0;

    return p1;
}
char *str_trim_cont(char str[],char c)
{
    char *p=str;
    char *q=str;
    while(*p)
    {
        if(*p==c) {
            *q++=*p++; 
            while(*p && *p==c) p++;
        }
        if(*p) *q++=*p++;
    }
    *q=0;
    return str;
}
char* str_trim_lines(char* str)
{
	if(!str) return (char*)"";
	int   n=strlen(str); if(0==n) return str;
	char* p=str;
	char* q=str+n-1;
	//<--right(q)
	if(STR_IS_SPACE(*q))
	{
		q--;
		for(; q>=p && STR_IS_SPACE(*q); q--) ;
		*(q+1)=0;
	}
	//left(p)-->
	if(STR_IS_SPACE(*p))
	{
		p++;
		for(; *p && STR_IS_SPACE(*p); p++) ;
		//equal: strcpy(str,p);
		char* r=str; 
		while(r!=q) *r++=*p++;
		*r=0;
	}
	return str;
}

char* str_trim_config_line(char* buf,bool balnum)
{
	if(!buf) return 0;

	//ltrim
	char* p=buf;
	while(*p && isblank(*p)) { p++; }
	if(0==*p) return 0;
		
	//check-leading
	if(balnum)
	{
		bool valid=0;
		if(isalnum(*p)) valid=1;
		if('_'==*p) valid=1;
		if(!valid) return 0;
	}

	//remove-note
	char* q=strchr(p,'#');
	if(q) *q=0;

	//go to last char
	int n=strlen(p);
	q=p+n-1;

	//end of \r\n?
	if('\n'==*q) { *q=0; --q; --n; }
	if('\r'==*q) { *q=0; --q; --n; }
	if(0==*q) return 0;

	//rtrim
	while(n>0 && isblank(*q)) { *q=0; --q; --n; }
	if(0==*q) return 0;

	return p;
}

int	str_is_begin_with(const char* s,const char* t,bool sensitive)
{
	if(!s || !t) return 0;
	if(0==*s || 0==*t) return 0;
	int n1=strlen(s);
	int n2=strlen(t);
	if(n1 < n2) return 0;
	if(sensitive) return 0==strncmp(s,t,n2);
	else          return 0==strncasecmp(s,t,n2);
}
int	str_is_end_with(const char* s,const char* t,bool sensitive)
{
	if(!s || !t) return 0;
	if(0==*s || 0==*t) return 0;
	int n1=strlen(s);
	int n2=strlen(t);
	if(n1 < n2) return 0;
	int start=n1-n2;
	if(sensitive) return 0==strcmp(s+start,t);
	else          return 0==strcasecmp(s+start,t);
}

int str_is_space(char c)
{
	switch(c)
	{
	case ' ':
	case '\t':
	case '\r':
	case '\n': return 1;
	default  : return 0;
	}
}
int str_is_space(const char* str)
{
	for(const char* p=str; *p; p++) 
	{ 
		if(!str_is_space(*p)) return 0; 
	}
	return 1;
}

char* str_trim_cont_space(char* str)
{
    char *p=str;
    char *q=str;
    while(*p)
    {
        if(str_is_space(*p))
        {
            *q++=' '; //change all space to ' '
            p++;
            while(*p && str_is_space(*p)) p++;
        }
        if(*p) *q++=*p++;
    }
    *q=0;
    return str;	
}

char* str_trim_cont_real_space(char* buf)
{
	char* q=buf;
	int   z=0;
	for(char* p=buf; *p; p++)
	{
		switch(*p)
		{
		case '\t': *p=' '; z++; break;
		case ' ' : z++; break;
		default  : z=0; break;
		}
		if(z>1) continue;
		*q++=*p;
	}
	*q=0;
	return buf;
}
char* str_trim_quote(char* s, char q)
{
	if(!s || 0==*s) return s;
	int  n=strlen(s);
	if(s[n-1] == q) s[n-1]=0;
	if(*s == q) strcpy(s, s+1);
	return s;
}
char* str_end_space(char *s)
{
	if(!s || 0==*s) return s;
	char *p=s, *q=0;
	for(; *p; p++)
	{
		if(STR_IS_SPACE(*p)) 
		{
			if(q) { *q=0; break; }
		}else{
			if(!q) q=s;	
			if(p!=q) *q=*p;	
			q++;	
		}
	}
	if(q) *q=0;
	return s;
}


char* str_end_with(char* s,char c)
{
	char* p=strchr(s,c); 
	if(p) *p=0;
	return s;	
}

int str_enclose(char* s,bool ispair)
{
	/*
	"abc" ==> abc
	(abc) ==> abc
	*/
	if(!s || 0==*s) return 0;
	int  n=strlen(s); if(n<1+ispair) return 0;
	char c1=*s;
	char c2=0;
	switch(c1)
	{
	case '\'':
	case '"' : 	c2=c1; 	break;
	case '(' :  c2=')'; break;
	case '[' :	c2=']'; break;
	case '{' :	c2='}'; break;
	case '<' : 	c2='>'; break;
	}
	if(0==c2) return 0; //not enclosed begin
	if( *(s+n-1) == c2)
	{
		*(s+n-1)=0;
	}else{
		if( ispair ) return 0; //not enclosed end
	}
	strcpy(s, s+1);
	return 1;
}

//find pat in s,if found, return the first matching pos in s,
//else return -1
int str_strstr(const char* str,const char* pat)
{
	const char *s=str;
	const char *p=pat;
	int  i=0;
	
	//朴素的思想
	//i=0 : 看pat是否在str[0]中 
	//i=1 : 看pat是否在str[1]中
	if(*p && *s)
	{
		int el=strlen(s)-strlen(p);
		while( i<=el) 
		{
			if(*p++ == *s++) { if(!*p) return i; }
			else { i++; s=str+i; p=pat; }
		}
	}
	return -1;
}

// find pat in s,if found, return the first matching pos in s,
// else return -1
// here:
//   f is pat's KMP's fail-string
//   f is generated from str_get_fail_string
int str_strstr(const char *str,const char* pat,int* f)
{
	const char *p=pat;
	const char *s=str;
	int  np=strlen(p);
	int  ns=strlen(s);
	int  pp=0;
	int  ps=0;
	
	while( pp<np && ps<ns)
	{
		if(p[pp]==s[ps]) { pp++; ps++; }
		else if(pp==0) ps++;
		else pp=f[pp-1]+1;
	}
	if(pp<np) return -1;
	return ps-np;
}
bool str_casestr(const char* x,const char* y)
{
	char xs[BUFSIZ],ys[BUFSIZ];
	strcpy(xs,x); str_lower(xs);
	strcpy(ys,y); str_lower(ys);
	return strstr(xs,ys) ? 1 : 0;
}

void str_get_fail_string(const char *pat, int* f)
{
	const char *p=pat;
	int n=strlen(p);
	f[0]=-1;	
	for(int j=1;j<n;j++)
	{
		int i=f[i-1];
		while( p[j] != p[i+1] && i>=0) i=f[i];
		if( p[j] == p[i+1] ) f[j]=i+1;
		else f[j]=-1;
	}
}
bool str_match(const char* input,const char* pattern)
{    	
	const char *start=0;
	for (;; input++, pattern++) 
	{	
		switch (*pattern) 
		{	
			case '?': if (!*input) return false; break;
			case '*':
    			pattern++;
				do {	
					if (str_match(pattern, input)) return true;
			   	}while (*input++);
				return false;
			case '[':
    			start=pattern+1;
    			do {
next:
					pattern++;
					if (*pattern == ']')   return false;
					else if (pattern[0] == '-' && pattern > start && pattern[1] != ']')
					{    	
						if (pattern[-1] <= *input && *input <= pattern[1]) break;
				    	else 
				    	{
							start=(++pattern)+1;
							goto next;
				    	}
					}
			    } while (*input != *pattern);
    			while (*pattern != ']')	 {	if (!*pattern++) return false; }
    			break;
			case '\\':
    			if (*(++pattern) != *input) return false;
    			break;
			default:
    			if (*input != *pattern) return false;
		}
		if (!*input)   return true;
	}
}

int str_freq(const char* str,char c)
{	
	int n=0;
	if(str) for(; *str; str++) { if(*str==c) n++; }
	return n;
}
int str_freq(const char* str,const char* mask)
{
    if(!str || !mask) return 0;
    
    int n=0;
    int m=strlen(mask);
    const char *p=strstr(str,mask);
    while(p){ p+=m; p=strstr(p,mask); n++; }
    return n;
}

int str_strtok(const char* str,const char* sep,char* goal[],bool method_repeat)
{ 	
	if(!str || !goal) return 0;
	if( !strstr(str,sep) ) { goal[0]=strdup(str);return 1; }
	
	int slen=strlen(sep);
	if(slen==1) return str_strtok(str,sep[0], goal,method_repeat);
	
	int count=0;
	const char *t=str; 	
		//此处:不能用 strdup(str).
		//一来用不着（因为t不改变str的内容）
		//二来会产生内存得不到释放
		
	while( t )
	{
		const char* t2=strstr(t,sep);
		if(! t2 ) {
			goal[count]=strdup(t);
			count++;
			break;
		}

		if(t2-t>0) { 
			goal[count]=strdup(t);
			goal[count][t2-t]=0;
			count++;
		}else {
			if(method_repeat) count++;
		}
		t=t2+slen;
	}
	return count;
/*	//method 2:
	char* p=strtok(str,sep);
	if( p!=NULL)
	{	goal[count]=strdup(p);
	     	while( (p=strtok(NULL,sep)) !=NULL) goal[++count]=strdup(p);
	}
	return count+1;
	//method 3:
	for (int i=0,char* cp=str; ; cp=NULL)
	{	if (!(goal[i++]=strtok(cp, sep)) ) break;}
*/		

}

int str_strtok(const char* str,char sep,char* goal[],bool method_repeat)
{	
	if(!str || !goal) return 0;
	char s[BUFSIZ];
	s[0]=sep;s[1]=0;
	if( !strstr(str,s) ) { goal[0]=strdup(str);return 1; }

 	int len=0,num=0;
 	s[len++]=*str;
	while(*str++)
	{ 	if( *str== sep) 
		{	if(len>0) 
			{
				s[len]=0;
				goal[num++]=strdup(s);
			}else{
				if(method_repeat) { goal[num++]=strdup(""); }
			}
			len=0;
		}else{ s[len++]=*str; }
	}
	if(len>0) { s[len]=0; goal[num++]=strdup(s); }
	return num;
}

char** str_alloc_array(int row,int len)
{
	char** arr=new char*[row]; if(!arr) return 0;
	memset(arr, 0, sizeof(char*)*row);
	int i=0;
	for(i=0;i<row;i++)
	{
		arr[i]=new char[len+1];
		if(!arr[i]) goto ERR;
	}
	return arr;

ERR:
	for(int j=0;j<i;j++)
	{
		delete arr[j];
	}
	delete [] arr;
	return 0;
}
void str_free_array(int n,char** goal)
{
	if(!goal) return;
	for(int i=0;i<n;i++)
	{
		if(goal[i]) { delete goal[i]; }
	}
	delete [] goal;
	goal=0;
}

//主要是为了方便上述str_strtok中goal参数服务的
void str_free(char* goal[],int n)
{
	if(n>0) 
	{
		for(int i=n-1; i>=0; i--)  { if(goal[i]) free( goal[i] ); }
	}else{
		int i=0;
		while(goal[i]!=NULL) { free(goal[i]); i++; }
	}
}

/* 
** 读大量数据时,替换strtok,且比strparse快1-2倍
** buf:输入的字符串
** rtn:返回的字符串数组,必须有足够的实际物理空间
**     一般可用 rtn=new char* [?];
**     然后 rtn[i]=new char[256];
** 返回分隔符的个数
*/
int str_strtok(const char *buf,char sep,int ngoal,char **rtn)
{
	if(!buf || 0==buf[0]) return 0;
	char *q = rtn[0];
	int  z = 0;
	for(const char *p = buf;  *p; p++)
	{
		if(STR_IS_RETURN(*p))
		{
			break;
		} else if (*p == sep) {
			*q = 0; //make it string
			z++;
			if(z>=ngoal) return z;
			q = rtn[z];
		} else {
			*q++ = *p;
		}
	}
	*q = 0;
	return ++z;
}
int str_strtok2(const char *buf,char sep,int ngoal,char rtn[][256],bool ignorecontinous,bool combinelast)
{
	if(!buf || !*buf) return 0;
	
	int   n = 0; 		//cur-index
	int   z = 0;        //continous-delim
	char *q = rtn[0]; 	//cur-string

	for(const char *p = buf; *p; p++)
	{	
		if(STR_IS_RETURN(*p))
		{
			break;
		} else if (*p == sep) {
			*q = 0;
			if(ignorecontinous)
			{
				if(q == rtn[n]) //no other chars
				{
					++z;
					if(z>1) continue; 	//empty column, ignore it
				}else{
					z=0;
				}
			}
			//too many columns
			++n;
			if(n==ngoal)
			{
				return n;
			}
			//new column
			q = rtn[n];
			if(n == ngoal-1) //last column
			{
				if(combinelast)
				{
					strcpy(q, p+1);
					return n+1;
				}
			}
		} else {
			*q++ = *p;
		}
	}
	*q = 0;
	return n+1;
}

int str_strtok_quote(const char *buf,char sep,int ngoal,char rtn[][256],char quote)
{
	if(!buf || !*buf) return 0;
	const char *p = buf;
	char *q = &(*(rtn[0]));
	int  n = 0;
	bool bs =0; 
	for( ; *p; p++)
	{	
		if(STR_IS_RETURN(*p))
		{
			break;
		} else if (*p == sep) {
			if(bs) { *q++=*p; continue; }
			
			*q = 0;
			n++;
			if(n>=ngoal) return n; 
			q = rtn[n];
		} else if(*p== quote) {
			bs=!bs;
			if(bs) q=rtn[n]; 
		} else {
			*q++ = *p;
			
		}
	}
	*q = 0;
	return ++n;
}
int   str_strtok_self(char buf[],char sep,int ngoal,int rtn[])
{
	if(!buf || !*buf) return 0;
	char *p = buf;
	int  n = 0, last=0, pos=0;

	for(; *p ; p++,pos++)
	{
		if(STR_IS_RETURN(*p))
		{
			break;
		} else if (*p == sep) {
		    *p=0; //update
		    rtn[n++]=last;
			if(n>=ngoal) return n; 
		    last=pos+1;
		}
	}
	*p = 0;
	rtn[n++]=last;
	return n;
}
int   str_strtok_self(char buf[],char sep,int ngoal,char** rtn)
{
	if(!buf || !*buf) return 0;
	char *p    = buf;
	char *last = buf;
	int  n=0;

	for(; *p ; p++)
	{
		if(STR_IS_RETURN(*p))
		{
			break;
		} else if (*p == sep) {
		    *p=0; //update
		    rtn[n++]=last;
			if(n>=ngoal) return n;
		    last=p+1;
		}
	}
	*p = 0;
	rtn[n++]=last;
	return n;
}

/*  get field N's string, same as '|cut -fN'
    NB:if several delim is continous,regarded as one
	params: sput: input-string 
	        n   : input-field(begin from 1)
	        sret: return-string
	return: 0   : failed or not-exist
	        1   : ok
*/
char* str_field(const char* str,char delim,int col,char* out,bool isall)
{
	//1,2,3,4,5
	// 1 2 3 4 5
	if(!str) return 0;
	const char* p=str;
	for(int i=0;i<col-1;i++)
	{
		p=strchr(p,delim);
		if(!p) return 0;
		p++;
	}
	
	char* pout=(out ? out : str_cb_buf);
	if(isall)
	{
		strcpy(pout, p);
	}else{
		const char* q=strchr(p,delim);
		if(!q){
			strcpy(pout,p);
		}else{
			strncpy(pout, p, q-p); //not include delim itself
			pout[q-p]=0;
		}
	}
	return pout;
}

int	str_scope_to_index(const char* str,bool* array,int maxdex,bool reverse,char delim1,char delim2)
{
	if(!str) return 0;
	int  cur=0;
	char vs[256][256];
	int  ncol=str_strtok2(str,delim2,256,vs);
	for(int i=0;i<ncol;i++)
	{
		int a=0,z=0;
		char* p=strchr(vs[i], delim1);
		if(p)
		{
			*p=0;
			a=atoi(vs[i]);
			z=atoi(p+1);
		}else{
			a=z=atoi(vs[i]);
		}
		if(a<0 || z<0) continue; //donot allow negative value
		if(z>=a)
		{
			for(int j=a; j<=z; j++) 
			{
				if(j<maxdex) { array[j]=1; cur++; }
			}
		}else{
			if(!reverse) continue;
			for(int j=a; j>=z; j--)
			{
				if(j<maxdex) { array[j]=1; cur++; }
			}
		}
	}
	return cur;
}
int	str_scope_to_array(const char* str,int* array,int narray,bool reverse,char delim1,char delim2)
{
	if(!str) return 0;
	char vs[256][256];
	int  cur=0;
	int  ncol=str_strtok2(str,delim2,256,vs);
	for(int i=0;i<ncol && cur<narray;i++)
	{
		int a=0,z=0;
		char* p=strchr(vs[i], delim1);
		if(p)
		{
			*p=0;
			a=atoi(vs[i]);
			z=atoi(p+1);
		}else{
			a=z=atoi(vs[i]);
		}
		if(a<0 || z<0) continue; //donot allow negative value
		if(z>=a)
		{
			for(int j=a; j<=z && cur<narray; j++) 
			{
				array[cur++]=j;
			}
		}else{
			if(!reverse) continue;
			for(int j=a; j>=z && cur<narray; j--) 
			{
				array[cur++]=j;
			}
		}
	}
	return cur;
}
int	str_scope_to_array_hex(const char* str,int* array,int narray,bool reverse,char delim1,char delim2)
{
	if(!str) return 0;
	char vs[256][256];
	int  cur=0;
	int  ncol=str_strtok2(str,delim2,256,vs);
	for(int i=0;i<ncol && cur<narray;i++)
	{
		int a=0,z=0;
		char* p=strchr(vs[i], delim1);
		if(p)
		{
			*p=0;
			a=str_hex_to_int(vs[i]);
			z=str_hex_to_int(p+1);
		}else{
			a=z=str_hex_to_int(vs[i]);
		}
		if(a<0 || z<0) continue; //donot allow negative value
		if(z>=a)
		{
			for(int j=a; j<=z && cur<narray; j++) 
			{
				array[cur++]=j;
			}
		}else{
			if(!reverse) continue;
			for(int j=a; j>=z && cur<narray; j--) 
			{
				array[cur++]=j;
			}
		}
	}
	return cur;
}
int str_parse(const char* str,const char* sep,char* goal[],bool repeat)
{ 
	return str_strtok(str,sep,goal, repeat); 
}
int str_parse(const char* str,char sep, char* goal[],bool repeat)
{ 
	return str_strtok(str,sep,goal, repeat); 
}

int str_strtok_line(const char* str,int nvs,char vs[][256],bool keepempty)
{
	const char *pc=str;	
	const char *pp=str; 
	char *pd=vs[0];		
	int  ncol=0;		
	while(*pc)
	{
		if('\n'==*pc || ('\r'==*pc && '\n'==*(pc+1)))
		{
			if(keepempty || pc>pp)
			{
				strncpy(pd, pp, pc-pp); pd[pc-pp]=0; ++ncol;
				if(ncol>=nvs) break;
				pd=vs[ncol];
			}
			if('\r'==*pc) pc++;
			pc++;
			pp=pc;
		}else{
			pc++;
		}
	}
	if(pc>pp && ncol<nvs) { strncpy(pd, pp, pc-pp); pd[pc-pp]=0; ++ncol; }
	return ncol;
}
int  str_strtok_scope(const char *buf,char sep1,char sep2,int ngoal,char from[][256],char to[][256])
{
	int col=str_strtok2(buf, sep1, ngoal, from);
	if(col>0)
	{
		for(int i=0;i<col;i++)
		{
			char* p=strchr(from[i], sep2);
			if(!p) {
				strcpy(to[i], from[i]);
			}else{
				*p=0;
				strcpy(to[i], p+1);
			}
		}
	}
	return col;
}

int str_two_value(const char* buf,int &v1,int &v2)
{
	if(!buf || !*buf) return 0;
	const char* p=buf;
	int where=0;
	/* 	KeyPosition|   *    *  |
		KeyPoint   | 0 1 2  3  |
	    Input      |   5 -- 60 |
	*/
		
	v1=atoi(buf);
	for(; *p ;p++)
	{
		if(*p>='0' && *p<='9') {
			if(where==0)  	  { where=1; v1=atoi(p); }
			else if(where==2) { break; }
			
		} else {
			if(where==1) 	  { where=2;  }
			
		}
	}
	if(p) { v2=atoi(p); return 1; }
	return 0;
}
int str_devide(const char* str,char delim,char* s1,char* s2,char* s3,char* s4,char* s5,char* s6,char* s7)
{
	char vs[8][256];
	int n=str_strtok2(str, delim, 8, vs);
	if(n>=1 && s1) strcpy(s1, vs[0]);
	if(n>=2 && s2) strcpy(s2, vs[1]);
	if(n>=3 && s3) strcpy(s3, vs[2]);
	if(n>=4 && s4) strcpy(s4, vs[3]);
	if(n>=5 && s5) strcpy(s5, vs[4]);
	if(n>=6 && s6) strcpy(s6, vs[5]);
	if(n>=7 && s7) strcpy(s7, vs[6]);
	return n;
}

char* str_join(int num,...)
{ 	
  	strcpy(str_cb_buf,"");
  	va_list list;
  	va_start(list,num);
  	for(int i=0;i<num;i++) {
  		char* p =va_arg(list,char*);
  		if(p) strcat(str_cb_buf, p); 
  	}
   	va_end(list);	
  	return str_cb_buf;
}
char* str_join(const char* format,...)
{
	STR_GET_CB_BUF(format)
    return str_cb_buf;
}
char* str_cat(char* s,const char* format,...)
{
	STR_GET_CB_BUF(format)
	strcat(s, str_cb_buf );
	return s;
}
char* str_combine(int n, char *words[],char* sep)
{  	
	if (n <=1) return (char*)"";		
	
	char *buffer;
  	int i,len =0, t=( sep? strlen(sep) : 0);  	

  	for (i=1; i<n; i++)   len+=strlen(words[i]);
  	len += t* (n-1); 
  	
 	buffer =(char *) malloc(len +1);
  	if (buffer ==NULL) return (char*)"";
  	
  	buffer[0] ='\0';
  	for (i= 1; i< n; i++)  
  	{	
  		if (i>1)  strcat(buffer, sep);
    	strcat (buffer, words[i]);
  	}
  	return (buffer);
}    
char* str_norm(char* src,char ori,int width)
{	
	int len,start;

	memset(str_cb_buf, ' ' ,width);

	if(!src) return str_cb_buf;
	len=strlen(src);
	
	start=0;	
	if(ori=='c') start=(width-len)/2;
	else if(ori=='r') start=width-len;
	if(start<0) start=0;
	
	while( *src && start< width) 
	{	
		
		if(!iscntrl(*src)) str_cb_buf[start++]=*src;
		src++;
	}
	str_cb_buf[width]='\0';	
	return str_cb_buf;
}

char* str_append(char* s, const char* t, int n)
{
	if(!s || !t) return "";
	if(0==n) return s;
	if(n<10) {
		for(int i=0;i<n;i++) 
		{
			strcat(s,t);
		}
	}else{
		int   x=strlen(s);
		int   y=strlen(t);
		char* p=s+x;
		for(int i=0;i<n;i++)
		{
			strcpy(p,t); 
			p+=y;
		}
	}
	return s;
}
char* str_append(char* s, char c, int n)
{
	if(!s) return "";
	int   x=strlen(s);
	char* p=s+x;
	for(int i=0;i<n;i++)
	{
		*p++=c;
	}
	*p=0;
	return s;
}

char* str_left(const char* str,char  mid,bool inc)
{	
	char t[2]={mid, 0 };
	return str_left(str, t, inc);
}
char* str_leftn(const char* str,char mid,int n,bool inc)
{
    if(0==n || 0==mid) return (char*)str;
    const char *p=strchr(str,mid);
    for(int i=0;p && i<n;i++) 
    {
        p=strchr(p,mid);
        if(p) p++;
    }
    if(!p) return (char*)str; //not enough token
    --p; //p is delimeter itself
    int len=p-str; 
    if(inc) len++; //include token itself
    strncpy(str_cb_buf,str, len);
    str_cb_buf[len]=0;
    return str_cb_buf;
}
char* str_right(const char* str,char  mid,bool inc)
{	
	char t[2]={mid, 0 };
	return str_right(str, t, inc);
}
char* str_left(const char* str,const char* mid,bool inc)
{  	
	if(!str) return NULL;
	const char *p=strstr(str, mid); if(!p) return NULL;
	
   	int n=p-str + (inc ? strlen(mid) : 0 );

    strcpy(str_cb_buf,"");
    char* q=str_cb_buf; 
    while( n-- ) *q++=*str++;
    *q=0;
    return str_cb_buf;
}
char* str_right(const char* str,const char* mid,bool inc)
{	
	if(!str) return NULL;
	const char *p=strstr(str, mid); if(!p) return NULL;
    return (char*)p+ (inc ? 0 : strlen(mid) );
} 
char* str_rleft(const char* str,char c,bool inc)
{	
	if(!str) return NULL;
	const char* p=strrchr(str,c); if(!p) return NULL;
   	int n=p-str + (inc ? 1 : 0 );

    strcpy(str_cb_buf,"");
    char* q=str_cb_buf; 
    while( n-- ) *q++=*str++;
    *q=0;
    return str_cb_buf;
}
char* str_right(const char* str,char mid,int n,bool inc)
{
	if(0==n) return (char*)str;
	const char* p=str;
	int x=0;
	for(;*p; p++)
	{
		if(*p==mid) { x++; }
		if(x>=n) break;
	}
	if(x!=n) return 0;
	return (char*)(inc ? p : p+1);
}
char* str_rright(const char* str,char c,bool inc)
{	
	if(!str) return NULL;
	const char* p=strrchr(str,c); if(!p) return NULL;
	return (char*)p+ (inc ? 0 :1);
}
char* str_mid(const char* str,const char* left,const char* right)
{	
	char* s=str_right(str,left,false); if(!s) return NULL;
	return str_left(s,right,false);
}
char* str_sub(const char* str,int left,int right)
{
	if(!str) return NULL;

	strcpy(str_cb_buf,"");
    char *p=str_cb_buf;
    const char *q=str+left;
    int n=right-left+1;
    while(n-- && *q) *p++=*q++;
    *++p=0;
    return str_cb_buf;
}
char* str_sub_self_between(char* str,int left,int right)
{
	int many=right-left+1;
	memcpy(str, str+left, many); //strcpy,strncpy is NOT safe!!!
	str[many]=0;
	return str;
}
char* str_sub_self_many(char* str,int from,int many)
{
	memcpy(str, str+from, many); //strcpy,strncpy is NOT safe!!!
	str[many]=0;
	return str;
}

char* str_sub(int x,int n)
{
	char fmt[32]; sprintf(fmt,"%%0%dd",n);
	sprintf(str_cb_buf, fmt, x);
	if(n>0) str_cb_buf[n]=0;
	return str_cb_buf;
}
int str_ncmp(const char* s1,const char* s2)
{
	const char* p=s1;
	const char* q=s2;
	for(; *p && *q; ++p,++q)
	{
		if(*p!=*q) return (*p>*q ? 1 : -1);
	}
	return 0;
}
int str_ncasecmp(const char* s1,const char* s2)
{
	const char* p=s1;
	const char* q=s2;
	for(; *p && *q; ++p,++q) 
	{
		if(tolower(*p)!=tolower(*q)) return ( tolower(*p)>tolower(*q) ? 1 : -1);
	}
	return 0;
}

int str_tail_cmp(const char* s1,const char* s2)
{
	if(!s1 || !s2) return 0;
	int   n1=strlen(s1);
	int   n2=strlen(s2);
	int   n=(n1>n2 ? n2 : n1);
	const char* q1=s1+n1-1;
	const char* q2=s2+n2-1;
	for(; n>0 && *q1==*q2; q1--,q2--,n--) ;
	return 0==n;
}
int str_rcmp(const char* s1,const char* s2)
{
	return str_rncmp(s1,s2,-1);
}
int str_rncmp(const char* s1,const char* s2,int len)
{
	if(!s1 || !s2)       return (!s1 && !s2) ? 0 : (s1  ? 1 : -1);
	if(0==*s1 || 0==*s2) return (*s1 == *s2) ? 0 : (*s1 ? 1 : -1);
	int n1=strlen(s1);
	int n2=strlen(s2);
	int n3=(len>=0) ? len : (n1>n2 ? n2 : n1);
	const char* p=s1+n1-1;
	const char* q=s2+n2-1;
	for(; n1 && n2 && n3; --n1,--n2,--n3,--p,--q)
	{
		if(*p!=*q) return (*p>*q ? 1 : -1);
	}
	if(0==n3) return 0;
	return (n1==n2) ? 0 : (n1 ? 1 : -1);
}
int str_rcasecmp(const char* s1,const char* s2)
{
	return str_rncasecmp(s1,s2,-1);
}
int str_rncasecmp(const char* s1,const char* s2,int len)
{
	if(!s1 || !s2)       return (!s1 && !s2) ? 0 : (s1  ? 1 : -1);
	if(0==*s1 || 0==*s2) return (*s1 == *s2) ? 0 : (*s1 ? 1 : -1);
	int n1=strlen(s1);
	int n2=strlen(s2);
	int n3=(len>=0) ? len : (n1>n2 ? n2 : n1);
	const char* p=s1+n1-1;
	const char* q=s2+n2-1;
	for(; n1 && n2 && n3; --n1,--n2,--n3,--p,--q)
	{
		if(tolower(*p)!=tolower(*q)) return ( tolower(*p)>tolower(*q) ? 1 : -1);
	}
	if(0==n3) return 0;
	return (n1==n2) ? 0 : (n1 ? 1 : -1);
}

int str_case_strstr(const char* s,const char* t)
{
	if(!s || !t) return 0;
	
	int   flag=0;
    int   ns=strlen(s);
    int   nt=strlen(t);
	char* ss=(char*)malloc(ns+1);
	char* st=(char*)malloc(nt+1);
	if(ss && st)
	{
		memcpy(ss,s,ns); ss[ns]=0;
		memcpy(st,t,nt); st[nt]=0;
		str_lower(ss);
		str_lower(st);
        if( strstr(ss,st) ) flag=1;
   	}
    if(ss) free(ss);
    if(st) free(st);
    return flag;
}
int str_case_compare(const char* s,const char* t,const char* how)
{
	if(!s && !t) return 1;
	if(!s || !t) return 0;
	if(0==*s && 0==*t) return 1;
	if(0==*s || 0==*t) return 0;
	
	IF_STR_CMP(s,t) return 1;		
	if(!how || (how && strstr(how,"strn"))) 
	{
        IF_STR_NCMP(s,t) return 1;
        IF_STR_NCMP(t,s) return 1;
	}
	if(!how || (how && strstr(how,"strstr")))
	{
        IF_STR_STR(s,t)  return 1;
		if(str_case_strstr(s,t)) return 1;
	}
	return 0;
}

char* str_upper(char* s)
{
	if(s) for( ; *s; s++ ) { *s=toupper(*s); }
	return s;
}
char* str_lower(char* s)
{
	if(s) for( ; *s; s++ ) { *s=tolower(*s); }
	return s;
}
char* str_upper(const char* s,char* d)
{
    if(s) while(*s) { *d++=toupper(*s++); }
    *d=0;
    return d;
}
char* str_lower(const char* s,char* d)
{
    if(s) while(*s) { *d++=tolower(*s++); }
    *d=0;
    return d;
}

char* str_unix_to_win(char buff[])
{	
	int i=0;
	while(buff[i])
	{	if (buff[i]=='\n')
		{	
			
			memmove(&buff[i + 1], &buff[i],(strlen(buff)-i+1) );
			buff[i]='\r';
			
			i++;
		} 
		i++;
	}
	return buff;
}

char* str_win_to_unix(char buff[])
{	
	int i=0;
	while(buff[i])
	{	
		if( i>=strlen(buff)-1) break;
		if (buff[i] == '\r' && buff[i+1]=='\n')
		{	
			memmove(&buff[i], &buff[i+1],strlen(buff)-i+1);
			i++;
		} 
		i++;
	}
	return buff;
}

bool str_is_chinese(int ch)	
{ 
	return((unsigned char)ch>160); 
}
bool str_is_ctrl(int c) 	
{ 
	return(iscntrl(c)|| c=='\n'|| c=='\t'|| c=='\f'|| c=='\r'|| c=='\v');
}
bool str_is_char(int c) 	
{ 	
	return ( (c>='A' && c<='Z') || (c>='a' && c<='z') ); 
}
bool str_is_upper(const char* str)
{	
	for(; *str; str++ ){ if(*str>='a' && *str<='z') return false;}
	return true;
}
bool str_is_lower(const char* str)
{	
	for(; *str; str++ ){ if(*str>='A' && *str<='Z') return false; }
	return true;
}	
bool str_is_comment (const char	*str)
{	
	char c= ' ';
	int comment=0,i;
	
	for (i=0; i<strlen(str); i++)
	if ((str[i]!= '\t') && (str[i]!= ' '))	{ c= str[i]; break; }
	if ((strcmp(str, "")== 0) || (c=='#') || (c==' ')) comment=1;
	return (comment);
}
bool str_is_prefix(const char* str, const char* s)
{    	
	const char* p=s, *q=str;
	for (; *p && *q && *p == *q; p++, q++)  ;
	return !*p;
}

bool str_is_suffix(const char* str,const char* s)
{    	
	const char* p=s+strlen(s), *q=str+strlen(str);
	for (; p >s && q >str && *p ==*q;  p--, q--)	;
	return (p == s && *p == *q);
}
bool str_is_digit(const char* s)
{
	for(const char* p=s; *p; p++)
	{
		if(! isdigit(*p) ) return 0;	
	}
	return 1;
}
bool str_is_digit(const char* s,int n)
{
	int z=0;
	for(const char* p=s; *p && z<n; p++,z++)
	{
		if(! isdigit(*p) ) return 0;
	}
	return 1;
}
bool str_is_date(const char* s)
{
	//xxxx.xx.xx
	//xxxx-xx-xx
	int  m=0,n=0,z=0; //m:non-digit n:cont-digit z:all-digit
	for(const char* p=s; *p; p++)
	{
		if(isdigit(*p) ) { if(n>=4 || z>=8) return 0; m=0; n++; z++; }
		else if('-'==*p || '.'==*p) { if(0==n || m>0) return 0; n=0; m++; }
		else { return 0; }
	}
	return 1;
}


bool str_is_empty(const char* str)
{	
	for(const char* p=str; *p; p++)
	{
		if(!STR_IS_SPACE(*p)) { return 0; }
	}
	return 1;
}
int str_is_empty(char *s,char* &p)
{
    if(!s) return 0;
    int n=strlen(s)-1;
    char *q=s+n;
    p=s;
    for( ; *p   && STR_IS_SPACE(*p); p++) ;
    if(0==*p) return 0;
    for( ; n>=0 && STR_IS_SPACE(*q); n--,q--) *q=0;
    return 1;
}
bool str_is_equal(const char* p,const char* q)
{
	if(p==q) return 1; 
    while(*p && *q) { if(*p++!=*q++) return 0; }
    return (0==*q && 0==*p);
}
bool str_is_common(const char* p,const char* q)
{
	if(p==q) return 1; 
    while(*p && *q) { if(*p++!=*q++) return 0; }
    return 1;

}
df_test_char str_get_isfunc(const char* type)
{
	
	if(0==strcasecmp(type,"digit")) return isdigit;
	else if(0==strcasecmp(type,"alnum")) return isalnum;
	else if(0==strcasecmp(type,"alpha")) return isalpha;
	else if(0==strcasecmp(type,"cntrl")) return iscntrl;
	else if(0==strcasecmp(type,"graph")) return isgraph;
	else if(0==strcasecmp(type,"lower")) return islower;
	else if(0==strcasecmp(type,"print")) return isprint;
	else if(0==strcasecmp(type,"punct")) return ispunct;
	else if(0==strcasecmp(type,"space")) return isspace;
	else if(0==strcasecmp(type,"upper")) return isupper;
	else if(0==strcasecmp(type,"xdigit")) return isxdigit;
	else return 0;
}

int str_meet_type(const char* str,const char* type)
{
	df_test_char pf=str_get_isfunc(type); if(!pf) return -1;
	const char* p=str;
	for(; *p; p++) { if( pf(*p) ) break; }
	return p-str;
}
int str_not_meet_type(const char* str,const char* type)
{
	df_test_char pf=str_get_isfunc(type); if(!pf) return -1;
	const char* p=str;
	for(; *p; p++) { if( !pf(*p) ) break; }
	return p-str;
}

int str_cmp_asc(const void* p1,const void* p2)
{
	if(!p1 || !p2)
	{
		if(p1) return 1;
		if(p2) return -1;
		return 0;
	}
	return strcmp( (char*)p1, (char*)p2);
}
int str_cmp_desc(const void* p1,const void* p2)
{
	return -str_cmp_asc(p1,p2);
}

void str_sort(int size,char* array[],bool asc)
{	
	if(size<=1) return;
	qsort( (void*)array, size, sizeof( array[0]), 
			(asc ? str_cmp_asc : str_cmp_desc) );
}
int str_ncmp_byint(const char* s1,const char* s2,int len) 
{
    if(0 != (len & 0x03) ) 
    {
    	if(s1[0]==s2[0]) return strcmp(s1,s2);
    	else             return s1[0]-s2[0];
    }
    
    for(int i=0; *(int*)&s1[i] == *(int*)&s2[i]; )
    {
        i+=4;
        if(i>=len) return 1;
    }
    return 0;
}

void str_print(const char* format, ...)
{
	if(!format) return;
	va_list	args;
	va_start(args,format);
	vfprintf(stderr,format, args);
	va_end(args);
}
void str_error()	{ str_error(errno); }
void str_error(int errnum)
{	
	char* s=strerror( errnum );
	if(s) perror(s);
	else  printf("%d:__unknown error\n", errnum);
}
void str_die(const char *format, ...)
{  	
	if(format)
	{
		va_list args;
	   	va_start(args, format);
	  	vfprintf(stderr, format, args);
	  	fprintf(stderr, "\n");
	  	va_end(args);
	}
  	exit(-1);
}
void str_warn(const char *format, ...)
{  	
	if(format)
	{
		va_list args;
	   	va_start(args, format);
	  	vfprintf(stderr, format, args);
	  	fprintf(stderr, "\n");
	  	va_end(args);
	}
}
char* str_nprint(const char* str, int n)
{
	if(!str || 0==*str) return "";
	strncpy(str_cb_buf,str,n);
	str_cb_buf[n]=0;
	return str_cb_buf;
}
void str_nprintf(const char* str,int n)
{
	int z=0;
	for(const char* p=str; z<n && *p; p++,z++)
	{
		if(isprint(*p)) printf("%c", *p);
		else            printf(".");
	}
}
char* str_sprint(char* str,const char *format, ...)
{
	if(str && format)
	{
		va_list args;
	    va_start(args, format);
	  	vsprintf(str, format, args);
	  	va_end(args);
	}
  	return str;
}
void str_fprint(FILE* fp,const char *format, ...)
{
	if(!format) return;
	va_list args;
    va_start(args, format);
  	vfprintf((fp?fp:stderr), format, args);
  	va_end(args);
}
void str_usage(const char* name,const char* use,bool exitit)
{	
	strcpy(str_cb_buf,name);
	strcat(str_cb_buf," ");
	strcat(str_cb_buf,use);
	str_usage(str_cb_buf,exitit);
}
void str_usage(const char* use,bool exitit) 
{ 
	const char* uu[1]; 
	uu[0]=use; 
	str_usage(1,uu,exitit); 
}
void str_repeat(int n,const char* c)
{	
	for(int i=0;i<n;i++) printf("%s",c);
}
void str_usage(int num,const char* use[],bool exitit)
{	
	int i, k=0, z=strlen("┃ZCTT @2001┃");
	for(i=0;i<num;i++) k=MAX(k, strlen( use[i])+8); 
	if(k<z) k=z+1; 
	
	printf("\n");
	str_repeat( k, "*");
	printf("\n用法[Usage]:\n");
	for( i=0;i<num;i++) printf("    %s\n",use[i]);
	
	str_repeat( k-z, " "); printf("┏━━━━━┓\n");
	str_repeat( k-z, "*"); printf("┃ZCTT @2001┃\n");
	str_repeat( k-z, " "); printf("┗━━━━━┛\n");
 	if(exitit) exit(0);
}

void str_debug(const char* str)
{
#if __STDC__ > 0 && defined(__cplusplus)
	printf("in FILE[%s]: LINE[%d] Function[%s()]",
		__FILE__,
		__LINE__,
		__FUNCTION__);
	if(str) printf("-->%s\n", str);
#endif
}

char* str_trip_begin(char* str,char c)
{
	char *p=str;
	while(*p && *p==c) p++;
	if(p!=str) strcpy(str, (*p) ? p : "");
	return str;
}
char* str_trip_end(char* str,char c)
{	
	if(!str || !*str) return str;
	int   n=strlen(str);
	char* p=str+n-1;
	while(n>0 && c==*p) { *p=0; p--; n--; }
	return str;
}
char* str_trip_end(char* str,const char* c)
{	
	if(!str || !c) return str;
	int len=strlen(str)-strlen(c);
	if(len>0) { if( strcmp( str+len, c)==0) str[len]=0; }
	return str;
}
char* str_trip_line(char* str)
{
	if(!str || !*str) return str;
	int   n=strlen(str);
	char* p=str+n-1;
	while(n>0 && ('\r'==*p || '\n'==*p)) { *p=0; p--; n--; }
	return str;
}

char* str_trip_tab(char* str,bool all)
{	
	int n=strlen(str);
	for(int i=0;i<n;i++) { 	if(str[i]=='\t') str[i]=' '; }
	return str;
}
char* str_add_end(char* str,char c)
{	
	if(!str) return NULL;
	int len=strlen(str);
	if(len>0 && str[len-1]!=c) str[len-1]=c; str[len]=0;
	return str;
}

char* str_remove_slant(char *path)
{
	return str_trip_end(path, '/');
}
char* str_remove(char* s,char c)
{       
	if(!s) return NULL;
	char *p=s;
	char *q=s;
	while(*p)
	{
		if(*p != c) *q++=*p;
		p++;
	}
	*q=0;
	return s;
}
char* str_remove(char* s,const char* c)
{
	if(!c || !s ) return s;
	int lc=strlen(c); if(lc==0) return s;
	char *p=strstr(s,c);
	while(p)
	{
		strcpy(p, p+lc);
		p+=lc;
		p=strstr(p,c);
	}
	return s;
}
char* str_remove_any(char* s,const char* c)
{
	if(!s) return NULL;
	if(!c) return s;
	char *p=s;
	char *q=s;
	while(*p)
	{
		if( !strchr(c, *p) ) *q++=*p;
		p++;
	}
	*q=0;
	return s;
}
char* str_remove_space(char* s)
{
	if(!s) return NULL;
	char *p=s;
	char *q=s;
	while(*p)
	{
		if( !str_is_space(*p) ) *q++=*p;
		p++;
	}
	*q=0;
	return s;
}
char* str_remove_begin(char* s,char c)
{
	if(!s || !c) return NULL;
	if(*s != c) return s;
	char *p=s;
	char *q=s;
	while(*p && *p==c) p++;
	while(*p) *q++=*p++;
	*q=0;
	return s;
}
char* str_remove_end(char* s,char c)
{
	if(!s || !c) return NULL;
	int n=strlen(s);
	char* p=s + n -1;
	if(*p != c) return s;
	while(p>=s && *p==c) p--;
	p++;
	*p=0;
	return s;
}
char* str_remove_right(char* s,char c,bool includeself)
{
	if(!s || !c) return s;
	char* p=strchr(s,c);
	if(p) 
	{
		if(!includeself) p++;
		*p=0;
	}
	return s;
}

bool str_is_note(const char* s)
{
	const char* p=s;
	//skip leading space
	while(*p && isblank(*p)) p++;
	//check the 1st char
	switch(*p)
	{
	case ';':
	case '#': //printf("is-note: %s\n", s); 
			  return 1; 
			  break;
	}
	return 0;
}
char* str_remove_between(char* s,char x,char y)
{
	char* p=strchr(s,   x); if(!p) return s;
	char* q=strchr(p+1, y); if(!q) return s;
	strcpy(p,q+1);
	return s;
}
char* str_remove_note_lines(char* s)
{
	//begin with #
	char* r=s;
	const char* p=s;
	while(1)
	{
		const char* q=strchr(p,'\n'); 
		if(!q)
		{
			if( str_is_note( p ) )
			{
				//ignore whole line
			}else{
				strcpy(r, p);
			}
			break;
		}else{
			if( str_is_note( p ) )
			{
				//ignore whole line
			}else{
				int n=q-p+1;
				strncpy(r, p, n);
				r+=n; *r=0;
			}
			p=q+1;
		}
	}
	return s;
}
char* str_normalize_space(char* s)
{
	if(!s) return NULL;
	str_trim_cont_space(s);
	str_trim_self(s);
	return s;
}
char* str_normalize_line(char* s)
{
	if(!s) return NULL;
	str_trip_line(s);
	str_normalize_space(s);
	return s;
}
char* str_normalize_config_line(char* s)
{
	if(!s) return NULL;
	str_normalize_line(s);
	str_replace(s, '\t', ' ');
	char* p=strchr(s,'#'); if(p) *p=0;
	return s;
}


char* str_replace(char *str,const char *s1,const char *s2)
{
	if(!str || 0==*str) return str;
	if(!s1  || 0==*s1) return str;
	int  n1=strlen(s1);
	int  n2=(s2 ? strlen(s2): 0);

	char *p=str;
	char *q=str_cb_buf;
	int  z=0;
	while(*p)
	{
	    if( 0==strncmp(p,s1,n1) )
	    {
	        if(n2) { memcpy(q, s2, n2); q+=n2; }
	        p+=n1;
	        ++z;
	    }else{
	        *q++=*p++;
	    }
	}
	*q=0;
	//
	if(z) strcpy(str, str_cb_buf);
	return str;
}
char* str_replace(char* str,char c,const char* s2)
{	
	if(!str || 0==*str) return "";
	int  n2=(s2 ? strlen(s2): 0);

	char *q=str_cb_buf;
	int  z=0;
	for(char* p=str; *p; p++)
	{
		if(*p==c)
		{
			if(n2) { memcpy(q, s2, n2);  q+=n2; }
			z++;
		}else{
			*q++=*p;	
		}
	}
	*q=0;
	if(z) strcpy(str,str_cb_buf);
	return str;
}	
char* str_replace(char *str,char c,char c2)
{
    for(char* p=str;*p;p++) { if(*p==c) *p=c2; }
    return str;
}
char* str_replace(char *str,const char* c,int x,int n)
{
	char t[32],fmt[32];
	sprintf(fmt,"%%0%dd", n);
	sprintf(t,fmt,x);
	return str_replace(str,c,t);
}
char* str_replace_tab(char* str)
{
	return str_replace(str, '\t', ' ');
}


int str_hex_to_int(const char* str)
{
	if(!str || 0==str[0]) return 0;
	const char* p=str;
	if(0==strncasecmp(str, "0x", 2)) p+=2;
	int x=0,c=0;
	for(; *p; p++)
	{
		if('0'<=*p && *p<='9')      c=*p-'0';
		else if('a'<=*p && *p<='f') c=*p-'a'+10;
		else if('A'<=*p && *p<='F') c=*p-'A'+10;
		else break;
		x=(x*16)+c;
	}
	return x;
}
int64_t str_hex_to_int64(const char* str)
{
	if(!str || 0==str[0]) return 0;
	const char* p=str;
	if(0==strncasecmp(str, "0x", 2)) p+=2;
	int64_t x=0LL;
	int     c=0;
	for(; *p; p++)
	{
		     if('0'<=*p && *p<='9') c=*p-'0';
		else if('a'<=*p && *p<='f') c=*p-'a'+10;
		else if('A'<=*p && *p<='F') c=*p-'A'+10;
		else break;
		x=(x*16)+c;
	}
	return x;
}

/*
64 bit can hold max 9223PB
0x7FFFFFFFFFFFFFFF =
9.223.372.036.854.775.807
EB PB  TB  GB  MB  KB   B
so EB is needless
*/
char* str_byte_to_pb(u64 x,char* str)
{
	const u64 PB=1LL*1000*1000*1000*1000*1000;
	const u64 TB=1LL*1000*1000*1000*1000;
	const u64 GB=1LL*1000*1000*1000;
	const u64 MB=1LL*1000*1000;
	const u64 KB=1LL*1000;

	int pb=(x/PB);
	int tb=(x%PB)/TB;
	int gb=(x%TB)/GB;
	int mb=(x%GB)/MB;
	int kb=(x%MB)/KB;
	int bb=(x%KB);
	sprintf(str, "%dP.%03dT.%03dG.%03dM.%03dK.%03d",
		pb, tb, gb,
		mb, kb, bb);
	return str;
}
char* str_byte_to_tb(u64 x,char* str)
{
	const u64 TB=1LL*1000*1000*1000*1000;
	const u64 GB=1LL*1000*1000*1000;
	const u64 MB=1LL*1000*1000;
	const u64 KB=1LL*1000;

	int tb=(x/TB);
	int gb=(x%TB)/GB;
	int mb=(x%GB)/MB;
	int kb=(x%MB)/KB;
	int bb=(x%KB);
	sprintf(str, "%03dT.%03dG.%03dM.%03dK.%03d",
		tb, gb,
		mb, kb, bb);
	return str;
}
char* str_byte_to_gb(u64 x,char* str)
{
	const u64 GB=1LL*1000*1000*1000;
	const u64 MB=1LL*1000*1000;
	const u64 KB=1LL*1000;

	int gb=(x/GB);
	int mb=(x%GB)/MB;
	int kb=(x%MB)/KB;
	int bb=(x%KB);
	sprintf(str, "%03dG.%03dM.%03dK.%03d",
		gb,
		mb, kb, bb);
	return str;
}
char* str_byte_to_mb(u64 x,char* str)
{
	const u64 MB=1LL*1000*1000;
	const u64 KB=1LL*1000;

	int mb=(x/MB);
	int kb=(x%MB)/KB;
	int bb=(x%KB);
	sprintf(str, "%03dM.%03dK.%03d",
		mb, kb, bb);
	return str;
}

char*   str_sec_to_day(int sec,char* str)
{
	int d=sec/86400;
	int h=(sec%86400)/3600;
	int m=(sec%3600)/60;
	int s=(sec%60);
	sprintf(str, "%dD.%02d:%02d:%02d", 
		d,
		h,m,s);
	return str;
}


char* str_to_time(long data)
{	
	long x,y,z;
	x=data/3600;
	y=(data-x*3600)/60;
	z=data-x*3600-y*60;
	sprintf(str_cb_buf, "%ld小时%02d分%02d秒", x,y,z );
	return str_cb_buf;
}
char* str_file_by_date(const char * pre,const char* suffix)
{
	long l;
	char tmp[256];
	time_t t=time(&l);
	struct tm * bb=localtime(&t);
	sprintf(tmp,"%4d年%02d月%02d日--%02d时%02d分%02d秒",bb->tm_year+1900,bb->tm_mon,bb->tm_mday,bb->tm_hour,bb->tm_min,bb->tm_sec);
	if(pre)   { strcpy(str_cb_buf,pre);strcat(str_cb_buf,tmp);}
	else      { strcpy(str_cb_buf,tmp); }
	
	if(suffix){ strcat(str_cb_buf,"."); strcat(str_cb_buf,suffix); }
  	return str_cb_buf;
}
char* str_file_by_date()
{	long l;
	char rr[32];
	
	time_t t=time(&l);
	struct tm * bb=localtime(&t);
	sprintf(str_cb_buf,"/tmp/tmp_%4dY%02dM%02dD_%02dh%02dm%02ds_",bb->tm_year+1900,bb->tm_mon,bb->tm_mday,bb->tm_hour,bb->tm_min,bb->tm_sec);
	srandom(bb->tm_sec);
	sprintf(rr,"%ld",random());
	strcat( str_cb_buf, rr);
	return str_cb_buf;
}

char* str_basename(const char* fname,char sep)
{
    const char *p = strrchr(fname, sep);
	if(p) fname=p+1;
    return (char*)fname;
}

size_t str_hash1(const char* s,int len)
{
    size_t h = 0;
    for(int i=0; i<len; i++,s++)
    {
    	h = (31*h) + *s; //sometime: 5
	}
    return h;
}
size_t str_hash2(const char* s,int len)
{
    size_t h = 0, g=0;
    for(int i=0;i<len;i++,s++)
    {
        h = (h << 4) + *s;
        g = (h & 0xF0000000);
        if (g)
        {
            h ^= (g >> 24);
            h ^= g;
        }
    }
    return h;
}
size_t str_hash3(const char* s,int len)
{
    size_t h= 0;
    for(int i=0;i<len;i++,s++)
    {
        h += *s;
        h += (h << 10);
        h ^= (h >> 6);
    }
    h += (h << 3);
    h ^= (h >> 11);
    h += (h << 15); 
    return (0==h) ? 1 : h;
}
u32 str_hash(const char* data, int n, u32 seed)
{
    if(0==seed) seed=0xbc9f1d34;

    // Similar to murmur hash
    const u32 	m = 0xc6a4a793;
    const u32 	r = 24;
    const char* limit = data + n;
    u32       	h = seed ^ (n * m);

    // Pick up four bytes at a time
    while (data + 4 <= limit) 
    {
        u32 w = *(int*)(data);
        data += 4;
        h    += w;
        h    *= m;
        h    ^= (h >> 16);
    }

    // Pick up remaining bytes
    switch (limit - data) 
    {
        case 3: 
            h += data[2] << 16;
            //do 2 then
        case 2: 
            h += data[1] << 8;
            //do 1 then
        case 1:
            h += data[0];
            h *= m;
            h ^= (h >> r);
            break;
        default:
            break;
    }
    return h;
}

int str_len(int n)
{
	int ln=1;
	if(n<0) n=-n;
	while(n>9) { n/=10; ln++; }
	return ln;
}

int str_from_shell(const char* sh,char *buf,int len)
{
	int n=0;
	char* p=buf;
	FILE* fp=popen(sh,"r");
	if(fp)
	{
		
		int ch;
		while(n<len && !feof(fp))
		{
			ch=fgetc(fp); 
			if(EOF != ch) { *p++=(char)ch; n++; }
		}
		pclose(fp);
	}
	*p=0;
	return n;
}

int str_system(int mode,const char* format,...)
{
	STR_GET_CB_BUF(format)
	
   	switch(mode)
   	{
   	case 1: strcat(str_cb_buf, " &"); break;
	case 2: strcat(str_cb_buf, " 2>/dev/null"); break;
   	case 3: strcat(str_cb_buf, " >/dev/null 2>&1"); break;
   	}
	
	return system(str_cb_buf);
}

void str_reverse_part(char str[],int s,int e)
{
	char tmp;
	while(e>s)
	{
		TSWAP(str[s],str[e],tmp)
		s++; e--;
	}
}
void str_reverse_self(char str[])
{
	int len=strlen(str)-1;
	str_reverse_part(str,0,len);
}
void str_reverse_word(char str[])
{
	
	int s=0,e=0,len=0;
	len=strlen(str);
	str_reverse_part(str,s,len-1);
	while(e<len)
	{
		if( !isalpha(str[e]) )
		{
			s=e;
			while(e<len && !isalpha(str[e])) e++;
			e--;
			str_reverse_part(str,s,e);
		}
		e++;
	}
}


char* str_skip_space(char*& str)
{
	while(*str && STR_IS_SPACE(*str)) str++;
	return str;
}
int str_skip_line(char*& str)
{
	if(*str)
	{
		if(STR_IS_UNIX(str)) 	{ str++;  return 1; }
		if(STR_IS_WINDOWS(str)) { str+=2; return 1; }
	}
	return 0;
}
char* str_skip_empty(char*& str)
{
	while(*str)
	{
		str_skip_space(str);
		if(0==str_skip_line(str)) break;
	}
	return str;
}

char* str_skip_note(char*& str)
{
	str_skip_empty(str);
	if('#'==*str || 0==strncmp(str,"//",2)) 
	{
		str_meet_newline(str);
	}
	else if(0==strncmp(str,"/*",2)) 
	{
		for(;*str;str++) 
		{ 
			if(0==strncmp(str,"*/",2)) { str+=2; break;  }
		}
	}
	return str;
}

char* str_meet_space(char*& str)
{
	while(*str && !(STR_IS_SPACE(*str)))  str++;
	return str;
}
char* str_meet_newline(char*& str)
{
	while(*str && '\n'!=*str)  str++;
	if(*str) str++;
	return str;
}
int str_meet_string(char*& str,char* what,int nwhat)
{
	while(*str)
	{
		if('\n'==*str) return 0;
	 	if(0==strcmp(str,what)) { str+=nwhat; return 1; }
		str++;
	}
	return 0;
}

char* str_get_word(char* str,char* word)
{
	char* q=word; *q=0;
	char* p=str;
	
	while(*p && STR_IS_SPACE(*p)) p++;
	
	if(!STR_IS_BWORD(*p)) return 0;
	*q++=*p++;
	
	while(*p)
	{
		if(!STR_IS_MWORD(*p)) break;
		*q++=*p++; 
	}
	*q=0;
	return word;
}
char* str_limit_length(char* str,char* out,int outlen,const char* ends,int maxs)
{
	char* p=str;
	char* q=out;
	char* es=(char*)((ends && *ends) ? ends : "\n");
	int   esn=strlen(es);
	int   cn=0; 
	int   ln=0; 
	while(*p)
	{
		if(0==strncmp(p,es,esn)) { ln++; if(ln>maxs) break; }
		*q++=*p++;
		cn++; if(cn>=outlen) break;
	}
	*q=0;
	return out;
}



//-------------------------------------------------------------------
//comstr4
//-------------------------------------------------------------------
int	str_to_ip_and_port(const char* str, char* ip,int& port)
{
	if(!str || !ip) return 0;
	//x:x
	const char* p=strchr(str, ':'); if(!p) return 0;
	int n=p-str; if(n<1) return 0;
	++p;
	if(0==*p) return 0;
	int x=atoi(p); if(x<1) return 0;
	strncpy(ip, str, n); ip[n]=0;
	port=x;
	return 1;
}
uint32_t str_to_ipaddr(const char* str)
{
	//x.x.x.x
	if(!str) return 0;
	uint32_t z=0;	//ip-addr
	int 	 dot=0;	//the nth domain
	int 	 x=0; 	//the nth value
	int 	 n=0; 	//the nth length
	for(const char* p=str; *p; p++)
	{
		if(*p>='0' && *p<='9')
		{
			++n;
			x=(x*10)+(*p-'0');
			if(x>255) return 0; //zero digit, or XXX > 255
		}
		else if('.'==*p)
		{
			if(0==n) return 0; //no digit
			++dot; if(dot>3) return 0; //x.x.x.x.x: 4 dot
			z=(z<<8)+x;
			x=0;
			n=0;
		}
		else 
		{
			return 0; //not digit
		}
	}
	if(3!=dot) return 0;
	if(0==n || x>255) return 0; //the 4th value
	z=(z<<8)+x;
	return z;
}
bool str_is_ipaddr(const char* str)
{
	for(const char* p=str; *p; p++)
	{
		if( ! ('.'==*p || isdigit(*p)) ) return 0;
	}
	char vs[4][256];
	int  ncol=str_strtok2(str,'.',4,vs);
	if(4 != ncol) return 0;
	for(int i=0;i<4;i++)
	{
		if(0==vs[i][0]) return 0;
		int x=atoi(vs[i]);
		if(x<0 || x>255) return 0;
		if(0==x && 3!=i) return 0;
	}
	return 1;
}
int  str_is_ipport(const char* str)
{
	const char* p=strchr(str,':'); if(!p) return 0;
	char  ip[256]={0};
	char  port[256]={0};
	int   n=p-str;
	strncpy(ip,str,n); ip[n]=0;
	strcpy(port, p+1);
	if(!str_is_ipaddr(ip)) return 0;
	if(!str_is_integer(port)) return 0;
	int x=atoi(port); if(x<1 || x>65535) return 0;
	return 1;
}

int	str_is_integer(const char* str)
{
	for(const char* p=str; *p; p++)
	{
		if(*p<'0' || *p>'9') return 0;
	}
	return 1;
}

int str_is_tolerant_int(const char* str)
{
	for(const char* p=str; *p; p++)
	{
		if(isdigit(*p)) return 1;
	}
	return 0;
}
int str_get_tolerant_int(const char* str)
{
	for(const char* p=str; *p; p++)
	{
		if(isdigit(*p) || '-'==*p) return atoi(p);
	}
	return 0;
}
int str_strtok_tolerant_int(const char* str,int nv,int* plist,char sep)
{
	if(!plist) return 0;
    if(!str || !*str) return 0;
    
    const char *p = str;
    char buf[BUFSIZ];
    char *q = buf;
    int  n = 0;

    for( ; *p; p++)
    {
        if(*p == '\r' || *p == '\n')
        {
            break;
        } else if (*p == sep) {
            *q = 0;
            if( q -buf > 0 && str_is_tolerant_int(buf))
            {
            	plist[n]=str_get_tolerant_int(buf);
            	n++;
            	if(n>=nv) return n;
        	}
            q = buf;
        } else {
            *q++ = *p;
        }
    }
    *q = 0;
    if( q -buf > 0 && str_is_tolerant_int(buf)) 
    {
    	if( n<nv)
    	{
    		plist[n]=str_get_tolerant_int(buf);
    		n++;
    	}
    }
    return n;	
}
	

//-------------------------------------------------------------------
//comstr5
//-------------------------------------------------------------------
int	str_to_int(const char* str)
{
	if(!str || 0==str[0]) return 0;
	if(0==strncasecmp(str, "0x", 2))
	{
		return str_hex_to_int(str+2);
	}
	else if(0==strncasecmp(str, "0b", 2))
	{
		return str_bin_to_int(str+2);
	}
	else if('0'==str[0])
	{
		return str_oct_to_int(str+1);
	}
	else
	{
		return str_dec_to_int(str);	
	}
}

int str_oct_to_int(const char* str)
{
	if(!str || 0==str[0]) return 0;
	int x=0,c=0;
	for(const char* p=str; *p; p++)
	{
		if('0'<=*p && *p<='7') c=*p-'0';
		else break;
		x=(x*8)+c;
	}
	return x;
}

int str_dec_to_int(const char* str)
{
	if(!str || 0==str[0]) return 0;
	int x=0,c=0;
	for(const char* p=str; *p; p++)
	{
		if('0'<=*p && *p<='9') c=*p-'0';
		else break;
		x=(x*10)+c;
	}
	return x;
}
int str_bin_to_int(const char* str)
{
	if(!str || 0==str[0]) return 0;
	int x=0,c=0;
	for(const char* p=str; *p; p++)
	{
		if('0'==*p || '1'==*p) c=*p-'0';
		else break;
		x=(x*2)+c;
	}
	return x;
}

int str_fill_left(const char* src,char* dst,int len)
{
	if(!dst)  { return 0; }
	if(len<1) { *dst=0; return 1; }
	if(!src || 0==*src)  
	{ 
		for(int i=0;i<len;i++) dst[i]=' '; 
		dst[len]=0; 
		return 1;
	}

	int n=strlen(src);
	if(n==len) {
		strcpy(dst, src);
	} else if(n>len) {
		strncpy(dst, src, len); dst[len]=0;
	} else {
		char* q=dst;
		int   left=len-n;
		for(int i=0;i<left;i++) *q++=' ';
		*q=0;
		strcpy(q, src);
	}
	//printf("src[%s] --(%d)--> dst[%s]\n", src, len, dst);
	return 1;
}
int str_fill_right(const char* src,char* dst,int len)
{
	if(!dst)  { return 0; }
	if(len<1) { *dst=0; return 1; }
	if(!src || 0==*src)  
	{ 
		for(int i=0;i<len;i++) dst[i]=' ';
		dst[len]=0;
		return 1; 
	}

	const char* p=src;
	char*       q=dst;
	int         i=0;
	for(;i<len && *p; i++)
	{
		*q++=*p++;
	}
	if(i<len)
	{
		int left=len-i;
		for(i=0;i<left;i++) *q++=' ';	
	}
	*q=0;
	//printf("src[%s] --(%d)--> dst[%s]\n", src, len, dst);
}
char* str_between_quota(const char* str,char* ret)
{
	if(!ret || !str || 0==*str) return 0;
	//first quota
	const char* p=str;
	for(; *p; p++)
	{
		if('"'==*p || '\''==*p) break;
	}
	if(!*p) return 0;
	//second quota
	const char* q=p+1;
	for(; *q; q++)
	{
		if(*q == *p) break;
	}
	if(!*q) return 0;
	//copy
	p++;
	char* r=ret;
	while(p<q) { *r++=*p++; }
	*r=0;
	return ret;
}

bool	str_has_digit(const char* s)
{
	for(const char* p=s; *p; p++) {	if(isdigit(*p)) return 1; }	
	return 0;
}
bool	str_has_alpha(const char* s)
{
	for(const char* p=s; *p; p++) {	if(isalpha(*p)) return 1; }	
	return 0;
}
bool	str_has_alnum(const char* s)
{
	for(const char* p=s; *p; p++) {	if(isalnum(*p)) return 1; }	
	return 0;
}
bool	str_has_xdigit(const char* s)
{
	for(const char* p=s; *p; p++) {	if(isxdigit(*p)) return 1; }	
	return 0;
}
bool	str_has_blank(const char* s)
{
	for(const char* p=s; *p; p++) {	if(isblank(*p)) return 1; }	
	return 0;
}
bool str_has_pair(const char* s,char x,char y)
{
	const char* p=strchr(s,   x); if(!p) return 0;
	const char* q=strchr(p+1, y); if(!q) return 0;
	return 1;
}

//cs/charset: u:utf8 g/0:gbk
//len:column length,as zh-word is regarded as 1, so max=len-2
char* str_safe_truncate(char* input,int len,char cs)
{
	char* p=input;
	int   x=1; //1:ok 2:zh1 3:zh2 4:zh3
	for(int z=0; *p; p++,z++)
	{
		unsigned char c=*p & 0xff;
		int y=x;
		if(c & 0x80)
		{
			switch(cs)
			{
			case 'u':
			case 'U':
				switch(x)
				{
				case 1: x=2; break;
				case 2: x=3; break;
				case 3: x=4; break;
				case 4: x=2; break;
				}
				break;
			default: //gbk,0
				switch(x)
				{
				case 1: x=2; break;
				case 2: x=3; break;
				case 3: x=2; break;
				}
				break;
			}
		}else{
			x=1;
		}
		//printf("%d:%02X:%d:%d\n", z, c, y,x);

		if(z>=len-3) //len-4,len-3,len-2,len-1
		{
			if(x<=2) break;
		}
	}
	*p=0;
	//printf("result[%s]\n", input);
	return input;
}
int	str_times(const char* buf,char c,bool continious)
{
	if(!buf || !*buf) return 0;

	int z=0,a=0,n=0; //z:not-dup-happen a:all-happen n:length
	for(const char *p = buf; *p; p++)
	{
		if(*p == c)
		{
			if(0==n) ++z;
			++n;
			++a;
		}else{
			n=0;
		}
	}
	return continious ? a : z;
}

bool str_find_word(const char* s,const char* d)
{
	if(!s || !d) return 0;

	const int m=strlen(s);
	const int n=strlen(d);
	if(m<1 || n<1) return 0;
	if(m<n)  return 0;
	if(m==n) return (0==strcmp(s,d));
	//now: m must > n
	const char* q=s;
	const char* p=s;
	while(*p)
	{
		if(isalnum(*p) || '_'==*p)
		{
			p++;
		}else{
			int z=p-q;
			if(n==z && 0==strncmp(q,d,n)) return 1;	
			p++;
			//skip following non-word
			while(*p && !(isalnum(*p) || '_'==*p)) 
			{
				p++;
			}
			q=p;
		}
	}
	int z=p-q;
	if(n==z && 0==strncmp(q,d,n)) return 1;
	return 0;
}

char* str_swap(char* r,int len)
{
	if(len<2) return r;
    char *p=r;
    char *q=r+len-1;
    for(; p<q; ++p, --q)
    {
        char c=*p;
        *p=*q;
        *q=c;
    }
	return r;
}

char* tostr(char x) 			{ sprintf(str_cb_buf, "%d", x);   return str_cb_buf; }
char* tostr(unsigned char x)	{ sprintf(str_cb_buf, "%d", x);   return str_cb_buf; }
char* tostr(short x)			{ sprintf(str_cb_buf, "%d", x);   return str_cb_buf; }
char* tostr(unsigned short x)	{ sprintf(str_cb_buf, "%d", x);   return str_cb_buf; }
char* tostr(int x)				{ sprintf(str_cb_buf, "%d", x);   return str_cb_buf; }
char* tostr(unsigned int x)   	{ sprintf(str_cb_buf, "%u", x);   return str_cb_buf; }
char* tostr(int64_t x)			{ sprintf(str_cb_buf, "%lld", x); return str_cb_buf; }

char* toip(u32 x)		
{
	sprintf(str_cb_buf, "%d.%d.%d.%d", (x>>24)&0xff, (x>>16)&0xff, (x>>8)&0xff, x&0xff); 
	return str_cb_buf;
}

void str_print_hex(const void* input,int len, int colsize)
{
	const char* buf=(const char*)input;
	char  str[BUFSIZ];
	char* p=str;

	int   mn=BUFSIZ/4-1;
	if(colsize>mn) { colsize=mn; }
    else           { if(len>mn) len=mn; }

	for(int i=0;i<len;i++)
	{
		sprintf(p, "%02x ", buf[i] & 0xff);
		p+=3;

		if(colsize>0 && 0==((i+1) % colsize))
		{
			for(int j=0;j<colsize;j++)
			{
				char c=buf[i-colsize+j+1];
				if( isprint(c) ) *p++=c;
				else             *p++='.';
			}
			*p=0;
			printf("%s\n", str);
			p=str;
		}
	}
	int odd=len%colsize;
	if(0!=odd)
	{
		int left=colsize-odd;
		for(int j=0;j<left;j++) 
		{
			strcpy(p, "   "); 
			p+=3;
		}
		for(int j=0;j<odd;j++)
		{
			char c=buf[len-odd+j];
			if( isprint(c) ) *p++=c;
			else             *p++='.';
		}
		*p=0;
		printf("%s\n", str);
	}
}
char* str_print_hex_to(const void* input,int len, char* str, char delim)
{
	const u8* p=(const u8*)input;
	char* q=str;
	for(int i=0;i<len;i++,p++)
	{
		if(i>0 && delim) { *q++=delim; }
		u8 c=0xff & *p;
		int n=sprintf(q, "%02x", c); if(n>0) q+=n;
	}
	*q=0;
	return str;
}


char* str_print_array_to(const int* parr,int narr,char* str,int len)
{
	if(!parr || !str || narr<1 || len<12) return "";
	char* p=str;
	for(int i=0;i<narr && len>11;i++)
	{
		if(p > str) { *p++=','; --len; }
		int n=sprintf(p, "%d", parr[i]);
		if(n) { p+=n; len-=n; }
	}
	*p=0;
	return str;
}
char* str_print_array_to_if(const int* parr,int narr,char* str,int len)
{
	if(!parr || !str || narr<1 || len<12) return "";
	char* p=str;
	int z=0;
	for(int i=0;i<narr && len>11;i++)
	{
		if(0==parr[i]) continue;
		++z;
		if(z>1) { *p++=','; --len; }
		int n=sprintf(p, "%d", parr[i]);
		if(n) { p+=n; len-=n; }
	}
	*p=0;
	return str;
}
char* str_print_array_reverse(const int* parr,int narr,char* str,int len)
{
	if(!parr || !str || narr<1 || len<12) return "";
	char* p=str;
	for(int i=0;i<narr && len>11;i++)
	{
		if(p > str) { *p++=','; --len; }
		int n=sprintf(p, "%d", parr[narr-1-i]);
		if(n) { p+=n; len-=n; }
	}
	*p=0;
	return str;
}

//mode u:up:进位 d:down:舍弃 r:round:5舍6入
char* ftoa(double val,int keep,char* ret,const char* mode)
{
   	if(keep > 7) keep=7;
    if(keep < 0) keep=0;
    char fmt[256];
    if(mode && 'u'==mode[0])
    {
        sprintf(fmt, "%%.%df", keep+1);
        sprintf(ret, fmt, val);
        //1.232 -> 1.24
        char* p=strchr(ret, '.');
        if(p)
        {
        	if( *(p+keep+1) < '6')
        	{
        		*(p+keep+1)='9';
        		val=atof(ret);
        	}
        }
		sprintf(fmt, "%%.%df", keep);
		sprintf(ret, fmt, val);
    }else if(mode && 'd'==mode[0]){
        sprintf(ret, "%.7f", val);
        //1.232 -> 1.23
        char* p=strchr(ret, '.');
        if(p)
        {
        	*(p+keep+1)=0;
        }
    }else{
        sprintf(fmt, "%%.%df", keep);
        sprintf(ret, fmt, val); //already round-up: 3.155-->3.16	    		
    }
	return ret;
}

int	str_get_id(const char* s,int where)
{
	int z=0,n=0;
	for(const char* p=s; *p; p++)
	{
		if(isdigit(*p)) {
			if(0==n)
			{
				++z;
				if(z == where) 
				{
					return atoi(p);
				}
			}
			++n;
		}else{
			n=0;
		}
	}
}

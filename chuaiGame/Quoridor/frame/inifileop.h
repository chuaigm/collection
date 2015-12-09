#ifndef __INIFILEOP_H__
#define __INIFILEOP_H__

/*********************************************************************
*
* 文件名称： IniFileOP.h
* 文件标识： 
* 其它说明： INI文件的操作
* 当前版本： V1.1
* 作    者： 廖月旺
* 环    境： VS2005(windows 7)
*
* 修改记录：
*     2005-06-23    V1.0    廖月旺      创建
*     2012-04-19    V1.1    chuaiGM    字符以及变量定义改动，以符合C++编译规则
*     2012-04-26    V1.1    chuaiGM    在linux下，tmpname会报警告(line:518)
*     2013-08-22    V1.2    chuaiGM    更改若干函数的入口参数类型为const
*                   说明：原来里面写的函数入口参数类型多为void*，这里将其全部转化
*                         为const char *型，如果需要调整，函数内部需要做强转
**********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define INI_TEST_FUNC               // 测试函数是否开启
//#define VS_2005_WARN_DISABLEx       // VS2005等windows集成环境下，屏蔽4996号字符警告

#ifdef VS_2005_WARN_DISABLE
#pragma warning (disable : 4996)
#endif

#define SuccessRet 1;
#define FailedRet  0;

#define MAX_CFG_BUF                              512 

#define CFG_OK                                   0 
#define CFG_SECTION_NOT_FOUND                    -1 
#define CFG_KEY_NOT_FOUND                        -2 
#define CFG_ERR                                  -3 
#define CFG_ERR_OPEN_FILE                        -5 
#define CFG_ERR_CREATE_FILE                      -6 
#define CFG_ERR_READ_FILE                        -7 
#define CFG_ERR_WRITE_FILE                       -8 
#define CFG_ERR_FILE_FORMAT                      -9 
#define CFG_ERR_EXCEED_BUF_SIZE                  -10 

#define COPYF_OK                                 0 
#define COPYF_ERR_OPEN_FILE                      -11 
#define COPYF_ERR_CREATE_FILE                    -12 
#define COPYF_ERR_READ_FILE                      -13 
#define COPYF_ERR_WRITE_FILE                     -14 

/*-----mark-----可更改的分隔符-----*/
char CFG_ssl = '[', CFG_ssr = ']';  /* 项标志符Section Symbol --可根据需要更改，如{ }等*/
char CFG_nis = ':';                 /* name 与 index 之间的分隔符 */
char CFG_kvs = '=';                 /* key与value之间的分隔符 */
char CFG_nts = '#';                 /* 注释符 */

int  CFG_section_line_no, CFG_key_line_no, CFG_key_lines;

static char * strtrimr(char * buf);
static char * strtriml(char * buf);
static int  FileGetLine(FILE *fp, char *buffer, int maxlen);
static int  SplitKeyValue(char *buf, char **key, char **val);
// 声明extern，保证外部调用
extern int  FileCopy(const char *source_file, const char *dest_file);
extern int  SplitSectionToNameIndex(char *section, char **name, char **index);
extern int  JoinNameIndexToSection(char **section, const char *name, const char *index);
extern int  ConfigGetKeyValue(const char *CFG_file, const char *section, const char *key, char *buf);
extern int  ConfigSetKeyValue(const char *CFG_file, const char *section, const char *key, const char *buf);
extern int  ConfigGetSections(const char *CFG_file, char *sections[]);
extern int  ConfigGetKeys(const char *CFG_file, const char *section, char *keys[]);
// 为了测试写的测试函数，建立以及读取
extern void test_iniFileCreate(void);
extern void test_iniFileRead(void);

/**********************************************************************
* 函数名称： strtrimr
* 功能描述： 去除字符串右边的空字符
* 访问的表： 无
* 修改的表： 无
* 输入参数： char * buf 字符串指针
* 输出参数： 无
* 返 回 值： 字符串指针
* 其它说明： 无
* 修改日期        版本号     修改人       修改内容
* -----------------------------------------------
* 2005/6/23        V1.0      廖月旺        创建
* 
***********************************************************************/
char * strtrimr(char * buf)
{
    int len,i;
    char * tmp = NULL;
    len = strlen(buf);
    tmp = (char*)malloc(len);
    
    memset(tmp,0x00,len);
    for(i = 0;i < len;i++)
    {
        if (buf[i] !=' ')
            break;
    }
    if (i < len) {
        strncpy(tmp,(buf+i),(len-i));
    }
    strncpy(buf,tmp,len);
    free(tmp);
    return buf;
}

/**********************************************************************
* 函数名称： strtriml
* 功能描述： 去除字符串左边的空字符
* 访问的表： 无
* 修改的表： 无
* 输入参数： char * buf 字符串指针
* 输出参数： 无
* 返 回 值： 字符串指针
* 其它说明： 无
* 修改日期        版本号     修改人       修改内容
* -----------------------------------------------
* 2005/6/23        V1.0      廖月旺        创建
* 
***********************************************************************/
char * strtriml(char * buf)
{
    int len,i;	
    char * tmp=NULL;
    len = strlen(buf);
    tmp = (char*)malloc(len);
    memset(tmp,0x00,len);
    for(i = 0;i < len;i++)
    {
        if (buf[len-i-1] !=' ')
            break;
    }
    if (i < len) {
        strncpy(tmp,buf,len-i);
    }
    strncpy(buf,tmp,len);
    free(tmp);
    return buf;
}

/**********************************************************************
* 函数名称： FileGetLine
* 功能描述： 从文件中读取一行
* 访问的表： 无
* 修改的表： 无
* 输入参数： FILE *fp 文件句柄；int maxlen 缓冲区最大长度
* 输出参数： char *buffer 一行字符串
* 返 回 值： 实际读的长度
* 其它说明： 无
* 修改日期        版本号     修改人       修改内容
* -----------------------------------------------
* 2005/6/23        V1.0      廖月旺        创建
* 
***********************************************************************/
int  FileGetLine(FILE *fp, char *buffer, int maxlen)
{
    int  i, j; 
    char ch1; 
    
    for(i = 0, j = 0; i < maxlen; j++) 
    { 
        if(fread(&ch1, sizeof(char), 1, fp) != 1) 
        { 
            if(feof(fp) != 0) 
            { 
                if(j == 0) return -1;               /* 文件结束 */ 
                else break; 
            } 
            if(ferror(fp) != 0) return -2;        /* 读文件出错 */ 
            return -2; 
        } 
        else 
        { 
            if(ch1 == '\n' || ch1 == 0x00) break; /* 换行 */ 
            if(ch1 == '\f' || ch1 == 0x1A)        /* '\f':换页符也算有效字符 */ 
            { 
                buffer[i++] = ch1; 
                break; 
            } 
            if(ch1 != '\r') buffer[i++] = ch1;    /* 忽略回车符 */ 
        } 
    } 
    buffer[i] = '\0'; 
    return i; 
} 

/**********************************************************************
* 函数名称： FileCopy
* 功能描述： 文件拷贝
* 访问的表： 无
* 修改的表： 无
* 输入参数： char *source_file　源文件　char *dest_file　目标文件
* 输出参数： 无
* 返 回 值： 0 -- OK,非0－－失败
* 其它说明： 无
* 修改日期        版本号     修改人       修改内容
* -----------------------------------------------
* 2005/6/23      V1.0      廖月旺       创建
* 2012-04-19     v1.1     chuaiGM       更改函数输入参数格式
* 2013-08-22     v1.2     chuaiGM       更改参数为const型
***********************************************************************/
int  FileCopy(const char *source_file, const char *dest_file)
{ 
    FILE *fp1, *fp2; 
    char buf[1024+1]; 
    int  ret; 
    
    if((fp1 = fopen(source_file, "r")) == NULL) 
        return COPYF_ERR_OPEN_FILE; 
    ret = COPYF_ERR_CREATE_FILE; 
    
    if((fp2 = fopen(dest_file, "w")) == NULL) goto copy_end; 
    
    while(1) 
    { 
        ret = COPYF_ERR_READ_FILE; 
        memset(buf, 0x00, 1024+1); 
        if(fgets((char *)buf, 1024, fp1) == NULL) 
        { 
            if(strlen(buf) == 0) 
            { 
                if(ferror(fp1) != 0) goto copy_end; 
                break;                                   /* 文件尾 */ 
            } 
        } 
        ret = COPYF_ERR_WRITE_FILE; 
        if(fputs((char *)buf, fp2) == EOF) goto copy_end; 
    } 
    ret = COPYF_OK; 
copy_end: 
    if(fp2 != NULL) fclose(fp2); 
    if(fp1 != NULL) fclose(fp1); 
    return ret; 
}

/**********************************************************************

* 函数名称： SplitSectionToNameIndex
* 功能描述： 分离section为name和index
*            [section]
*              /   \
*            name:index
*            jack  :   12 
*            |     |   | 
*            k1    k2  i 
* 访问的表： 无
* 修改的表： 无
* 输入参数： char *section  
* 输出参数： char **name, char **index
* 返 回 值： 1 --- ok 
*            0 --- blank line 
*           -1 --- no name, ":index" 
*           -2 --- only name, no ':' 
* 其它说明： 无
* 修改日期        版本号     修改人       修改内容
* -----------------------------------------------
* 2005/6/23        V1.0      廖月旺        创建
* 
***********************************************************************/
int  SplitSectionToNameIndex(char *section, char **name, char **index)
{ 
    int  i, k1, k2, n; 
    
    if((n = strlen((char *)section)) < 1) return 0; 
    for(i = 0; i < n; i++) 
        if(section[i] != ' ' && section[i] != '\t') break; 
        if(i >= n) return 0; 
        if(section[i] == CFG_nis) return -1; 
        k1 = i; 
        for(i++; i < n; i++) 
            if(section[i] == CFG_nis) break; 
            if(i >= n) return -2; 
            k2 = i; 
            for(i++; i < n; i++) 
                if(section[i] != ' ' && section[i] != '\t') break; 
                section[k2] = '\0'; 
                *name = section + k1; 
                *index = section + i; 
                return 1; 
} 

/**********************************************************************
* 函数名称： JoinNameIndexToSection
* 功能描述： 合成name和indexsection为section
*            jack  :   12 
*            name:index
*              \   /
*            [section]
* 访问的表： 无
* 修改的表： 无
* 输入参数： char *name, char *index
* 输出参数： char **section  
* 返 回 值： 1 --- ok 
*			 0 --- blank line 
* 其它说明： 无
* 修改日期        版本号     修改人       修改内容
* -----------------------------------------------
* 2005/6/23      V1.0      廖月旺        创建
* 2012-04-19     v1.1     chuaiGM       更改函数输入参数格式
* 2013-08-22     v1.1     chuaiGM       更改后两个参数类型为const
***********************************************************************/
int  JoinNameIndexToSection(char **section, const char *name, const char *index)
{ 
    int n1,n2; 
    
    if((n1 = strlen((char *)name)) < 1) return 0; 
    if((n2 = strlen((char *)index)) < 1) return 0;
    
    strcat(*section,name);
    strcat(*section+n1,":");
    strcat(*section+n1+1,index);
    *(*section+n1+1+n2) = '\0';
    return 1; 
} 
/**********************************************************************
* 函数名称： SplitKeyValue
* 功能描述： 分离key和value
*　　　　　　key=val
*			jack   =   liaoyuewang 
*			|      |   | 
*			k1     k2  i 
* 访问的表： 无
* 修改的表： 无
* 输入参数： char *buf  
* 输出参数： char **key;char **val
* 返 回 值： 1 --- ok 
*			 0 --- blank line 
*			-1 --- no key, "= val" 
*			-2 --- only key, no '=' 
* 其它说明： 无
* 修改日期        版本号     修改人       修改内容
* -----------------------------------------------
* 2005/6/23         V1.0      廖月旺        创建
* 2005-08-22        V1.2     chuaiGM        修改key与value之间的分隔符可自定义CFG_kvs
* 
***********************************************************************/
int  SplitKeyValue(char *buf, char **key, char **val)
{
    int  i, k1, k2, n; 
    
    if((n = strlen((char *)buf)) < 1) return 0; 
    for(i = 0; i < n; i++) 
        if(buf[i] != ' ' && buf[i] != '\t') break; 
        if(i >= n) return 0; 
        if(buf[i] == CFG_kvs) return -1; 
        k1 = i; 
        for(i++; i < n; i++) 
            if(buf[i] == CFG_kvs) break; 
            if(i >= n) return -2; 
            k2 = i; 
            for(i++; i < n; i++) 
                if(buf[i] != ' ' && buf[i] != '\t') break; 
                buf[k2] = '\0'; 
                *key = buf + k1; 
                *val = buf + i; 
                return 1; 
} 

/**********************************************************************
* 函数名称： ConfigGetKeyValue
* 功能描述： 获得key的值
* 访问的表： 无
* 修改的表： 无
* 输入参数： char *CFG_file　文件；char *section　项值；char *key　键值
* 输出参数： char *buf　key的值
* 返 回 值： 0 --- ok 非0 --- error 
* 其它说明： 无
* 修改日期        版本号     修改人       修改内容
* -----------------------------------------------
* 2005-06-23     V1.0     廖月旺     创建
* 2012-04-19     v1.1     chuaiGM    更改函数输入参数格式，函数名修改
* 2013-08-22     v1.2     chuaiGM    更改前三个参数为const类型
***********************************************************************/
int  ConfigGetKeyValue(const char *CFG_file, 
                       const char *section, 
                       const char *key, 
                             char *buf)
{ 
    FILE *fp; 
    char buf1[MAX_CFG_BUF + 1], buf2[MAX_CFG_BUF + 1]; 
    char *key_ptr, *val_ptr; 
    int  line_no, n, ret; 
    
    line_no = 0; 
    CFG_section_line_no = 0; 
    CFG_key_line_no = 0; 
    CFG_key_lines = 0; 
    
    if((fp = fopen(CFG_file, "rb")) == NULL) return CFG_ERR_OPEN_FILE; 
    
    while(1)                                       /* 搜找项section */ 
    { 
        ret = CFG_ERR_READ_FILE; 
        n = FileGetLine(fp, buf1, MAX_CFG_BUF); 
        if(n < -1) goto r_cfg_end; 
        ret = CFG_SECTION_NOT_FOUND; 
        if(n < 0) goto r_cfg_end;                    /* 文件尾，未发现 */ 
        line_no++; 
        n = strlen(strtriml(strtrimr(buf1))); 
        if(n == 0 || buf1[0] == CFG_nts) continue;       /* 空行 或 注释行 */ 
        ret = CFG_ERR_FILE_FORMAT; 
        if(n > 2 && ((buf1[0] == CFG_ssl && buf1[n-1] != CFG_ssr))) 
            goto r_cfg_end; 
        if(buf1[0] == CFG_ssl) 
        { 
            buf1[n-1] = 0x00; 
            if(strcmp(buf1+1, section) == 0) 
                break;                                   /* 找到项section */ 
        } 
    } 
    CFG_section_line_no = line_no; 
    while(1)                                       /* 搜找key */ 
    { 
        ret = CFG_ERR_READ_FILE; 
        n = FileGetLine(fp, buf1, MAX_CFG_BUF); 
        if(n < -1) goto r_cfg_end; 
        ret = CFG_KEY_NOT_FOUND; 
        if(n < 0) goto r_cfg_end;                    /* 文件尾，未发现key */ 
        line_no++; 
        CFG_key_line_no = line_no; 
        CFG_key_lines = 1; 
        n = strlen(strtriml(strtrimr(buf1))); 
        if(n == 0 || buf1[0] == CFG_nts) continue;       /* 空行 或 注释行 */ 
        ret = CFG_KEY_NOT_FOUND; 
        if(buf1[0] == CFG_ssl) goto r_cfg_end; 
        if(buf1[n-1] == '+')                         /* 遇+号表示下一行继续  */ 
        { 
            buf1[n-1] = 0x00; 
            while(1) 
            { 
                ret = CFG_ERR_READ_FILE; 
                n = FileGetLine(fp, buf2, MAX_CFG_BUF); 
                if(n < -1) goto r_cfg_end; 
                if(n < 0) break;                         /* 文件结束 */ 
                line_no++; 
                CFG_key_lines++; 
                n = strlen(strtrimr(buf2)); 
                ret = CFG_ERR_EXCEED_BUF_SIZE; 
                if(n > 0 && buf2[n-1] == '+')            /* 遇+号表示下一行继续 */ 
                { 
                    buf2[n-1] = 0x00; 
                    if(strlen(buf1) + strlen(buf2) > MAX_CFG_BUF) 
                        goto r_cfg_end; 
                    strcat(buf1, buf2); 
                    continue; 
                } 
                if(strlen(buf1) + strlen(buf2) > MAX_CFG_BUF) 
                    goto r_cfg_end; 
                strcat(buf1, buf2); 
                break; 
            } 
        } 
        ret = CFG_ERR_FILE_FORMAT; 
        if(SplitKeyValue(buf1, &key_ptr, &val_ptr) != 1) 
            goto r_cfg_end; 
        strtriml(strtrimr(key_ptr)); 
        if(strcmp(key_ptr, key) != 0) 
            continue;                                  /* 和key值不匹配 */ 
        strcpy(buf, val_ptr); 
        break; 
    } 
    ret = CFG_OK; 
r_cfg_end: 
    if(fp != NULL) fclose(fp); 
    return ret; 
} 

/**********************************************************************
* 函数名称： ConfigSetKeyValue
* 功能描述： 设置key的值
* 访问的表： 无
* 修改的表： 无
* 输入参数： char *CFG_file　文件；char *section　项值；
* 			 char *key　键值；char *buf　key的值
* 输出参数： 无
* 返 回 值：   0 --- ok 非0 --- error 
* 其它说明： 无
* 修改日期        版本号     修改人       修改内容
* -----------------------------------------------
* 2005-06-23     V1.0      廖月旺      创建
* 2012-04-19     v1.1     chuaiGM     更改函数输入参数格式，函数名
* 2012-04-26     v1.1     chuaiGM     tmpnam在linuxG++下会报警告
* 2013-08-22     v1.2     chuaiGM     将所有参数改为const型，写入时分隔符按照用户指定
***********************************************************************/
int  ConfigSetKeyValue(const char *CFG_file, 
                       const char *section, 
                       const char *key, 
                       const char *buf)
{ 
    FILE *fp1, *fp2; 
    char buf1[MAX_CFG_BUF + 1]; 
    int  line_no, line_no1, n, ret, ret2; 
    char *tmpfname; 
    
    ret = ConfigGetKeyValue(CFG_file, section, key, buf1); 
    if(ret <= CFG_ERR && ret != CFG_ERR_OPEN_FILE) return ret; 
    if(ret == CFG_ERR_OPEN_FILE || ret == CFG_SECTION_NOT_FOUND) 
    { 
        if((fp1 = fopen(CFG_file, "a")) == NULL) 
            return CFG_ERR_CREATE_FILE; 
        // write [section]
        if(fprintf(fp1, "%c%s%c\n", CFG_ssl, section, CFG_ssr) == EOF) 
        { 
            fclose(fp1); 
            return CFG_ERR_WRITE_FILE; 
        } 
        // write key=value, the '=' can be customized
        if(fprintf(fp1, "%s%c%s\n", key, CFG_kvs, buf) == EOF) 
        { 
            fclose(fp1); 
            return CFG_ERR_WRITE_FILE; 
        } 
        fclose(fp1); 
        return CFG_OK; 
    } 
    if((tmpfname = tmpnam(NULL)) == NULL) 
    //if((tmpfname = mkstemp(NULL)) == NULL) 
        return CFG_ERR_CREATE_FILE; 
    
    if((fp2 = fopen(tmpfname, "w")) == NULL) 
        return CFG_ERR_CREATE_FILE; 
    
    ret2 = CFG_ERR_OPEN_FILE; 
    
    if((fp1 = fopen(CFG_file, "rb")) == NULL) goto w_cfg_end; 
    
    if(ret == CFG_KEY_NOT_FOUND) 
        line_no1 = CFG_section_line_no; 
    else /* ret = CFG_OK */ 
        line_no1 = CFG_key_line_no - 1; 
    for(line_no = 0; line_no < line_no1; line_no++) 
    { 
        ret2 = CFG_ERR_READ_FILE; 
        n = FileGetLine(fp1, buf1, MAX_CFG_BUF); 
        if(n < 0) goto w_cfg_end; 
        ret2 = CFG_ERR_WRITE_FILE; 
        if(fprintf(fp2, "%s\n", buf1) == EOF) goto w_cfg_end; 
    } 
    if(ret != CFG_KEY_NOT_FOUND) 
        for( ; line_no < line_no1+CFG_key_lines; line_no++) 
        { 
            ret2 = CFG_ERR_READ_FILE; 
            n = FileGetLine(fp1, buf1, MAX_CFG_BUF); 
            if(n < 0) goto w_cfg_end; 
        } 
        ret2 = CFG_ERR_WRITE_FILE; 
        if(fprintf(fp2, "%s%c%s\n", key, CFG_kvs, buf) == EOF) goto w_cfg_end; 
        while(1) 
        { 
            ret2 = CFG_ERR_READ_FILE; 
            n = FileGetLine(fp1, buf1, MAX_CFG_BUF); 
            if(n < -1) goto w_cfg_end; 
            if(n < 0) break; 
            ret2 = CFG_ERR_WRITE_FILE; 
            if(fprintf(fp2, "%s\n", buf1) == EOF) goto w_cfg_end; 
        } 
        ret2 = CFG_OK; 
w_cfg_end: 
        if(fp1 != NULL) fclose(fp1); 
        if(fp2 != NULL) fclose(fp2); 
        if(ret2 == CFG_OK) 
        { 
            ret = FileCopy(tmpfname, CFG_file); 
            if(ret != 0) return CFG_ERR_CREATE_FILE; 
        } 
        remove(tmpfname); 
        return ret2; 
} 

/**********************************************************************
* 函数名称： ConfigGetSections
* 功能描述： 获得所有section
* 访问的表： 无
* 修改的表： 无
* 输入参数： char *CFG_file　文件
* 输出参数： char *sections[]　存放section名字
* 返 回 值： 返回section个数。若出错，返回负数。
* 其它说明： 无
* 修改日期        版本号     修改人       修改内容
* -----------------------------------------------
* 2005/6/23      V1.0      廖月旺        创建
* 2012-04-19     v1.1     chuaiGM       更改函数输入参数格式
* 2013-08-22     v1.1     chuaiGM       更改第一个参数类型为const
***********************************************************************/
int  ConfigGetSections(const char *CFG_file, char *sections[])
{ 
    FILE *fp; 
    char buf1[MAX_CFG_BUF + 1]; 
    int  n, n_sections = 0, ret; 
    
    
    if((fp = fopen(CFG_file, "rb")) == NULL) return CFG_ERR_OPEN_FILE; 
    
    while(1)                                       /*搜找项section */ 
    { 
        ret = CFG_ERR_READ_FILE; 
        n = FileGetLine(fp, buf1, MAX_CFG_BUF); 
        if(n < -1) goto cfg_scts_end; 
        if(n < 0) break;                             /* 文件尾 */ 
        n = strlen(strtriml(strtrimr(buf1))); 
        if(n == 0 || buf1[0] == CFG_nts) continue;       /* 空行 或 注释行 */ 
        ret = CFG_ERR_FILE_FORMAT; 
        if(n > 2 && ((buf1[0] == CFG_ssl && buf1[n-1] != CFG_ssr))) 
            goto cfg_scts_end; 
        if(buf1[0] == CFG_ssl) 
        { 
            buf1[n-1] = 0x00; 
            strcpy(sections[n_sections], buf1+1); 
            n_sections++; 
        } 
    } 
    ret = n_sections; 
cfg_scts_end: 
    if(fp != NULL) fclose(fp); 
    return ret; 
} 

/**********************************************************************
* 函数名称： ConfigGetKeys
* 功能描述： 获得所有key的名字（key=value形式, value可用加号表示续行）
* 访问的表： 无
* 修改的表： 无
* 输入参数： char *CFG_file　文件 char *section 项值
* 输出参数： char *keys[]　存放key名字
* 返 回 值： 返回key个数。若出错，返回负数。
* 其它说明： 无
* 修改日期        版本号     修改人       修改内容
* -----------------------------------------------
* 2005-06-23     V1.0      廖月旺       创建
* 2012-04-19     v1.1     chuaiGM       更改函数输入参数格式
* 2013-08-22     v1.1     chuaiGM       更改前两个参数类型为const
***********************************************************************/
int  ConfigGetKeys(const char *CFG_file, 
                   const char *section, 
                         char *keys[])
{ 
    FILE *fp; 
    char buf1[MAX_CFG_BUF + 1], buf2[MAX_CFG_BUF + 1]; 
    char *key_ptr, *val_ptr; 
    int  n, n_keys = 0, ret; 
    
    
    if((fp = fopen(CFG_file, "rb")) == NULL) return CFG_ERR_OPEN_FILE; 
    
    while(1)                                       /* 搜找项section */ 
    { 
        ret = CFG_ERR_READ_FILE; 
        n = FileGetLine(fp, buf1, MAX_CFG_BUF); 
        if(n < -1) goto cfg_keys_end; 
        ret = CFG_SECTION_NOT_FOUND; 
        if(n < 0) goto cfg_keys_end;                 /* 文件尾 */ 
        n = strlen(strtriml(strtrimr(buf1))); 
        if(n == 0 || buf1[0] == CFG_nts) continue;       /* 空行 或 注释行 */ 
        ret = CFG_ERR_FILE_FORMAT; 
        if(n > 2 && ((buf1[0] == CFG_ssl && buf1[n-1] != CFG_ssr))) 
            goto cfg_keys_end; 
        if(buf1[0] == CFG_ssl) 
        { 
            buf1[n-1] = 0x00; 
            if(strcmp(buf1+1, section) == 0) 
                break;                                   /* 找到项section */ 
        } 
    } 
    while(1) 
    { 
        ret = CFG_ERR_READ_FILE; 
        n = FileGetLine(fp, buf1, MAX_CFG_BUF); 
        if(n < -1) goto cfg_keys_end; 
        if(n < 0) break;                             /* 文件尾 */ 
        n = strlen(strtriml(strtrimr(buf1))); 
        if(n == 0 || buf1[0] == CFG_nts) continue;       /* 空行 或 注释行 */ 
        ret = CFG_KEY_NOT_FOUND; 
        if(buf1[0] == CFG_ssl) 
            break;                                     /* 另一个 section */ 
        if(buf1[n-1] == '+')                         /* 遇+号表示下一行继续 */ 
        { 
            buf1[n-1] = 0x00; 
            while(1) 
            { 
                ret = CFG_ERR_READ_FILE; 
                n = FileGetLine(fp, buf2, MAX_CFG_BUF); 
                if(n < -1) goto cfg_keys_end; 
                if(n < 0) break;                         /* 文件尾 */ 
                n = strlen(strtrimr(buf2)); 
                ret = CFG_ERR_EXCEED_BUF_SIZE; 
                if(n > 0 && buf2[n-1] == '+')            /* 遇+号表示下一行继续 */ 
                { 
                    buf2[n-1] = 0x00; 
                    if(strlen(buf1) + strlen(buf2) > MAX_CFG_BUF) 
                        goto cfg_keys_end; 
                    strcat(buf1, buf2); 
                    continue; 
                } 
                if(strlen(buf1) + strlen(buf2) > MAX_CFG_BUF) 
                    goto cfg_keys_end; 
                strcat(buf1, buf2); 
                break; 
            } 
        } 
        ret = CFG_ERR_FILE_FORMAT; 
        if(SplitKeyValue(buf1, &key_ptr, &val_ptr) != 1) 
            goto cfg_keys_end; 
        strtriml(strtrimr(key_ptr));
        strcpy(keys[n_keys], key_ptr); 
        n_keys++; 
    } 
    ret = n_keys; 
cfg_keys_end: 
    if(fp != NULL) fclose(fp); 
    return ret; 
}

#ifdef INI_TEST_FUNC

/**********************************************************************
* 函数名称： test_iniFileCreate
* 功能描述： 测试函数入口
* 修改日期        版本号     修改人       修改内容
* -----------------------------------------------
* 2005/6/23        V1.0      廖月旺        创建
* 2013-08-22       V1.0     chuaiGM        修改函数名，修改为普通函数接口 
* 
***********************************************************************/
void test_iniFileCreate(void)
{
    char buf[20]="";
    char buf1[20]="";
    char buf2[20]="";
    char buf3[20]="";
    int  ret;
    long abc;
    int i;
    
    char * section;
    char * key;
    char * val;
    char * name;
    char * index;
    section = buf1;
    key = buf2;
    val = buf3;
    
    ret = ConfigSetKeyValue("Config.ini", "Jack:Lio", "Jack", "-12321");
    if(ret!=0) {
        printf("ConfigSetKeyValue -- error, error number=%d\n", ret);
        exit(0);
    }
    
    for(i = 0;i < 20; i++)
    {
        memset(buf,0x00,20);
        memset(buf1,0x00,20);
        memset(buf2,0x00,20);
        memset(buf3,0x00,20);
        sprintf(buf,"%d",i);
        sprintf(buf2,"Lio%d",i);
        sprintf(buf3,"%d",i);
        JoinNameIndexToSection(&section, "Jack", buf);
        ConfigSetKeyValue("Config.ini", section, key, val);
    }
    
    name = buf2;
    index = buf3;
    memset(buf1,0x00,20);
    memset(buf2,0x00,20);
    memset(buf3,0x00,20);
    strcpy(buf1,"Jack:Lio");
    SplitSectionToNameIndex(section, &name, &index);
    printf("\n name=%s,index=%s\n",name,index);
    
    ret = ConfigGetKeyValue("Config.ini", "Jack:Lio", "Jack", buf);
    if(ret!=0) {
        printf("ConfigGetKeyValue -- error, error number=%d\n", ret);
        exit(0);
    }
    if (strcmp(buf,"")!=0) {
        abc = atol(buf);
    }
    
    printf("\n buf=%s\n",buf);	
    printf("\n abc=%ld\n",abc);
    
    ConfigSetKeyValue("Config.ini","Jack:Lio", "Email", "liaoyuewang@163.com");

    //FileCopy("Config.ini", "Configbak.dat");

    printf("\nFile %s line%d\n",__FILE__,__LINE__);

    printf("\n******** This test is created by chuaiGM. ********\n\n");
    
    
}

/**********************************************************************
* 函数名称： test_iniFileRead
* 功能描述： 测试函数入口
* 修改日期        版本号     修改人       修改内容
* -----------------------------------------------
* 2005/6/23        V1.0      廖月旺        创建
* 
***********************************************************************/
void test_iniFileRead(void)
{
    char buf[20]={0};
    
    ConfigSetKeyValue("Config.ini", "system", "rootkey", "000");
    ConfigSetKeyValue("Config.ini", "system", "trankey", "000");
    ConfigGetKeyValue("Config.ini", "system", "rootkey", buf);
    
    printf("取到的rootkey信息如下：%s\n",buf);
    
    ConfigSetKeyValue("Config.ini", "system", "rootkey", "0001");
    ConfigGetKeyValue("Config.ini", "system", "rootkey", buf);
    
    printf("取到的rootkey信息如下：%s\n",buf);
    
    ConfigSetKeyValue("Config.ini", "system", "rootkey", "000");
    ConfigGetKeyValue("Config.ini", "system", "rootkey", buf);

    printf("取到的rootkey信息如下：%s\n",buf);

}
#endif      // INI_TEST_FUNC

#endif      // __INIFILEOP_H__

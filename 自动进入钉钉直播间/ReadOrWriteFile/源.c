#include <stdio.h>
#include "��ͷ.h"
#include <errno.h>
#include <string.h>
#include <wtypes.h>
//#include <Windows.h>
#define MAXSIZE 50
#define MAXPATH 512

extern  _declspec(dllexport) char* Write_File(const int* AutoOpenLive, const int* CheckLive, const int* StopCheckLive, const int* AutoOPenNextLive, const int* OpenLiveTime,
	const int* Time1Start, const char* Time1Time, const int* Time2Start, const char* Time2Time, const  int* Time3Start, const char* Time3Time, const int* Time4Start, const char* Time4Time,
	const int* Time5Start, const char* Time5Time, const int* Time6Start, const char* Time6Time, const int* Time7Start, const char* Time7Time, const int* Time8Start, const char* Time8Time,
	const int* ShowTop, const int* PositionX, const int* PositionY, const int* PositionW, const int* PositionH, const char* config_file_path)
{
	char buff[MAXPATH];
	static char err[MAXSIZE];

	sprintf_s(buff, 512, "�ж�ֱ��ʱ�Զ����� = %d;\n"
		"xx���Ӽ��һ��ֱ���Ƿ��ж� = %d;\n"
		"xx���Ӻ�δ��⵽ֱ���������жϼ�� = %d;\n"
		"�Զ�������һ��ֱ�� = %d;\n"
		"����ֱ������xx�����Զ����� = %d;\n"
		"ʱ��һ���� = %d;\n"
		"ʱ��һ = %s;\n"
		"ʱ������� = %d;\n"
		"ʱ��� = %s;\n"
		"ʱ�������� = %d;\n"
		"ʱ���� = %s;\n"
		"ʱ���Ŀ��� = %d;\n"
		"ʱ���� = %s;\n"
		"ʱ���忪�� = %d;\n"
		"ʱ���� = %s;\n"
		"ʱ�������� = %d;\n"
		"ʱ���� = %s;\n"
		"ʱ���߿��� = %d;\n"
		"ʱ���� = %s;\n"
		"ʱ��˿��� = %d;\n"
		"ʱ��� = %s;\n"
		"����ʼ����ʾ����� = %d;\n"
		"������X���� = %d;\n"
		"������Y���� = %d;\n"
		"�������� = %d;\n"
		"������߶� = %d;\n",
		*AutoOpenLive, *CheckLive, *StopCheckLive, *AutoOPenNextLive, *OpenLiveTime,
		*Time1Start, Time1Time, *Time2Start, Time2Time, *Time3Start, Time3Time, *Time4Start, Time4Time,
		*Time5Start, Time5Time, *Time6Start, Time6Time, *Time7Start, Time7Time, *Time8Start, Time8Time, *ShowTop,
		*PositionX, *PositionY, *PositionW, *PositionH);

	FILE* fp;
	if (fopen_s(&fp, config_file_path, "w") != 0)
	{
		strerror_s(err, MAXSIZE, errno);
		return err;
	}

	if (fwrite(buff, sizeof(char), strlen(buff), fp) == 0)
	{
		strerror_s(err, MAXSIZE, errno);
		return err;
	}
	fclose(fp);

	return NULL;
}


extern  _declspec(dllexport) char* Read_File(int* AutoOpenLive, int* CheckLive, int* StopCheckLive, int* AutoOPenNextLive, int* OpenLiveTime,
	int* Time1Start, char* Time1Time, int* Time2Start, char* Time2Time, int* Time3Start, char* Time3Time, int* Time4Start, char* Time4Time,
	int* Time5Start, char* Time5Time, int* Time6Start, char* Time6Time, int* Time7Start, char* Time7Time, int* Time8Start, char* Time8Time,
	int* ShowTop, int* PositionX, int* PositionY, int* PositionW, int* PositionH, const char* config_file_path)
{
	int ch, i = 0, j = 0;
	int al = 0;//�ж�ֱ��ʱ�Զ�����
	int cl = 5;//ÿ��xx�����Ƿ��ж�ֱ��
	int scl = 5;//xx���Ӻ�δ��⵽ֱ���������жϼ��
	int aonl = 0;//�Զ�����һ��ֱ��
	int olt = 5;//������һ��ֱ������xx����ʱ�Զ�����
	int t1s = 0;//ʱ��һ
	char t1t[MAXSIZE] = { "0,0" };//ʱ��һ����
	int t2s = 0;
	char t2t[MAXSIZE] = { "0,0" };//ʱ�����
	int t3s = 0;
	char t3t[MAXSIZE] = { "0,0" };//ʱ��������
	int t4s = 0;
	char t4t[MAXSIZE] = { "0,0" };//ʱ��������
	int t5s = 0;
	char t5t[MAXSIZE] = { "0,0" };//ʱ��������
	int t6s = 0;
	char t6t[MAXSIZE] = { "0,0" };//ʱ��������
	int t7s = 0;
	char t7t[MAXSIZE] = { "0,0" };//ʱ��������
	int t8s = 0;
	char t8t[MAXSIZE] = { "0,0" };//ʱ�������
	int st = 0;//����ʼ����ʾ�����
	int px = 0;
	int py = 0;
	int pw = 0;
	int ph = 0;

	char config[MAXSIZE];     //������ļ���ȡ�Ĳ���
	static char err[MAXSIZE];//���������Ϣ
	FILE* fp;

	//�������ļ���ȡ����
	if (fopen_s(&fp, config_file_path, "r") != 0)
	{
		strerror_s(err, MAXSIZE, errno);
		return err;
	}

	if ((ch = fgetc(fp)) == EOF)//�ж��ļ��Ƿ�Ϊ��
	{
		strcpy_s(err, MAXSIZE, "�����ļ�Ϊ�գ�");
		return err;
	}

	while ((ch = fgetc(fp)) != EOF)
	{
		if (ch == '=')   //�ж�ch�Ƿ�Ϊ��=��
		{
			while ((ch = fgetc(fp)) != ';')//��ȡ��=������;���ڵĲ���
			{
				if (ch == ' ')//�ж�ch�Ƿ�Ϊ�ո�
				{
					i = 0;
					continue;//�����˴�ѭ����������ȡ�Ŀո�
				}
				config[i] = ch;
				i++;
			}
		}

		if (ch == '\n')       //�ж��Ƿ������һ��
		{
			if (j == 0)
				sscanf_s(config, "%d", &al);  //��һ�е��������ж�ֱ��ʱ�Զ�����
			else if (j == 1)
				sscanf_s(config, "%d", &cl);  //�ڶ��е�������ÿ��xx�����Ƿ��ж�ֱ��
			else if (j == 2)
				sscanf_s(config, "%d", &scl); //�����е�������xx���Ӻ�δ��⵽ֱ���������жϼ��
			else if (j == 3)
				sscanf_s(config, "%d", &aonl);//�����е��������Ƿ��Զ�����һ��ֱ��
			else if (j == 4)
				sscanf_s(config, "%d", &olt); //�����е������Ǿ�����һ��ֱ������xx����ʱ�Զ�����
			else if (j == 5)
				sscanf_s(config, "%d", &t1s); //�����е�������ʱ��һ
			else if (j == 6)
				sscanf_s(config, "%s", &t1t, MAXSIZE); //�����е�������ʱ��һ����
			else if (j == 7)
				sscanf_s(config, "%d", &t2s); //�ڰ��е�������ʱ���
			else if (j == 8)
				sscanf_s(config, "%s", &t2t, MAXSIZE); //�ھ��е�������ʱ��һ����
			else if (j == 9)
				sscanf_s(config, "%d", &t3s); //��ʮ�е�������ʱ����
			else if (j == 10)
				sscanf_s(config, "%s", &t3t, MAXSIZE); //��ʮһ�е�������ʱ��ʱ��������
			else if (j == 11)
				sscanf_s(config, "%d", &t4s); //��ʮ���е�������ʱ����
			else if (j == 12)
				sscanf_s(config, "%s", &t4t, MAXSIZE); //��ʮ���е�������ʱ��ʱ��������
			else if (j == 13)
				sscanf_s(config, "%d", &t5s); //��ʮ���е�������ʱ����
			else if (j == 14)
				sscanf_s(config, "%s", &t5t, MAXSIZE); //��ʮ���е�������ʱ��������
			else if (j == 15)
				sscanf_s(config, "%d", &t6s); //��ʮ���е�������ʱ����
			else if (j == 16)
				sscanf_s(config, "%s", &t6t, MAXSIZE); //��ʮ���е�������ʱ��������
			else if (j == 17)
				sscanf_s(config, "%d", &t7s); //��ʮ���е�������ʱ����
			else if (j == 18)
				sscanf_s(config, "%s", &t7t, MAXSIZE); //��ʮ���е�������ʱ��������
			else if (j == 19)
				sscanf_s(config, "%d", &t8s); //�ڶ�ʮ�е�������ʱ���
			else if (j == 20)
				sscanf_s(config, "%s", &t8t, MAXSIZE); //�ڶ�һ�е�������ʱ�������
			else if (j == 21)
				sscanf_s(config, "%d", &st); //�ڶ�һ�е������Ƕ���ʼ����ʾ�����
			else if (j == 22)
				sscanf_s(config, "%d", &px); //�ڶ����е�������������x����
			else if (j == 23)
				sscanf_s(config, "%d", &py); //�ڶ����е�������������y����
			else if (j == 24)
				sscanf_s(config, "%d", &pw); //�ڶ����е��������������
			else if (j == 25)
				sscanf_s(config, "%d", &ph); //�ڶ����е��������������
			else if (j == 26)
				break;

			j++;
			i = 0;
			memset(config, 0, MAXSIZE);//����ÿ��ѭ��ǰ����
		}

	}

	*AutoOpenLive = al;
	*CheckLive = cl;
	*StopCheckLive = scl;
	*AutoOPenNextLive = aonl;
	*OpenLiveTime = olt;
	*Time1Start = t1s;
	strcpy_s(Time1Time, MAXSIZE, t1t);
	*Time2Start = t2s;
	strcpy_s(Time2Time, MAXSIZE, t2t);
	*Time3Start = t3s;
	strcpy_s(Time3Time, MAXSIZE, t3t);
	*Time4Start = t4s;
	strcpy_s(Time4Time, MAXSIZE, t4t);
	*Time5Start = t5s;
	strcpy_s(Time5Time, MAXSIZE, t5t);
	*Time6Start = t6s;
	strcpy_s(Time6Time, MAXSIZE, t6t);
	*Time7Start = t7s;
	strcpy_s(Time7Time, MAXSIZE, t7t);
	*Time8Start = t8s;
	strcpy_s(Time8Time, MAXSIZE, t8t);
	*ShowTop = st;
	*PositionX = px;
	*PositionY = py;
	*PositionW = pw;
	*PositionH = ph;

	fclose(fp);
	return NULL;
}


extern _declspec(dllexport)  char* Screen_Write_File(const int* DingDingX, const int* DingDingY, const int* DingDingWidth, const int* DingDingHeigth,
	const int* ScreenShotX, const int* ScreenShotY, const int* ScreenShotWidth, const int* ScreenShotHeight, const char* config_file_path)
{
	char buff[MAXPATH];
	static char err[MAXSIZE];

	sprintf_s(buff, MAXPATH, "����X���� = %d;\n����Y���� = %d;\n������� = %d;\n�����߶� = %d;\n��ͼX���� = %d;\n��ͼY���� = %d;\n��ͼ��� = %d;\n��ͼ�߶� = %d;\n",
		*DingDingX, *DingDingY, *DingDingWidth, *DingDingHeigth, *ScreenShotX, *ScreenShotY, *ScreenShotWidth, *ScreenShotHeight);

	//MessageBoxA(NULL, buff, "aaa", MB_OK);
	FILE* fp;
	if (fopen_s(&fp, config_file_path, "w") != 0)
	{
		strerror_s(err, MAXSIZE, errno);
		return err;
	}

	if (fwrite(buff, sizeof(char), strlen(buff), fp) == 0)
	{
		strerror_s(err, MAXSIZE, errno);
		return err;
	}
	fclose(fp);

	return NULL;
}


extern _declspec(dllexport) char* Screen_Read_File(int* DingDingX, int* DingDingY, int* DingDingWidth, int* DingDingHeigth, int* ScreenShotX,
	int* ScreenShotY, int* ScreenShotWidth, int* ScreenShotHeight, const char* config_file_path)
{

	int ch, i = 0, j = 0;
	int Dx = 0;	  //����x����
	int Dy = 0;   //����y����
	int Dw = 0;   //�������ڿ��
	int Dh = 0;   //�������ڸ߶�
	int Sx = 0;   //��ͼx����
	int Sy = 0;   //��ͼy����
	int Sw = 0;   //��ͼ���
	int Sh = 0;   //��ͼ�߶�

	char config[MAXSIZE];     //������ļ���ȡ�Ĳ���
	static char err[MAXSIZE];//���������Ϣ
	FILE* fp;

	//�������ļ���ȡ����
	if (fopen_s(&fp, config_file_path, "r") != 0)
	{
		strerror_s(err, MAXSIZE, errno);
		return err;
	}

	if ((ch = fgetc(fp)) == EOF)//�ж��ļ��Ƿ�Ϊ��
	{
		strcpy_s(err, MAXSIZE, "�����ļ�Ϊ�գ�");
		return err;
	}


	while ((ch = fgetc(fp)) != EOF)
	{
		if (ch == '=')        //�ж�ch�Ƿ�Ϊ��=��
		{
			while ((ch = fgetc(fp)) != ';')//��ȡ��=������;���ڵĲ���
			{
				if (ch == ' ')//�ж�ch�Ƿ�Ϊ�ո�
				{
					i = 0;
					continue;//�����˴�ѭ����������ȡ�Ŀո�
				}
				config[i] = ch;
				i++;
			}
		}

		if (ch == '\n')       //�ж��Ƿ������һ��
		{
			if (j == 0)
				sscanf_s(config, "%d", &Dx);   //�ѵ�һ�е����ݱ��浽Dx����һ�е������Ƕ���x����
			else if (j == 1)
				sscanf_s(config, "%d", &Dy);   //�ѵڶ��е����ݱ��浽Dy���ڶ��е������Ƕ���y����
			else if (j == 2)
				sscanf_s(config, "%d", &Dw);   //�ѵ����е����ݱ��浽Dw�������е������Ƕ������ڸ߶�
			else if (j == 3)
				sscanf_s(config, "%d", &Dh);   //�ѵ����е����ݱ��浽Dh�������е������Ƕ������ڿ��
			else if (j == 4)
				sscanf_s(config, "%d", &Sx);//�ѵ����е����ݱ��浽Sx�������е������ǽ�ͼx����
			else if (j == 5)
				sscanf_s(config, "%d", &Sy);//�ѵ����е����ݱ��浽Sy�������е������ǽ�ͼy����
			else if (j == 6)
				sscanf_s(config, "%d", &Sw);//�ѵ����е����ݱ��浽Sw�������е������ǽ�ͼ���
			else if (j == 7)
				sscanf_s(config, "%d", &Sh); //�ѵڰ��е����ݱ��浽Sh�������е������ǽ�ͼ�߶�
			else if (j == 8)
				break;

			j++;
			i = 0;
			memset(config, 0, MAXSIZE);//����ÿ��ѭ��ǰ����
		}
	}

	fclose(fp);

	*DingDingX = Dx;
	*DingDingY = Dy;
	*DingDingWidth = Dw;
	*DingDingHeigth = Dh;
	*ScreenShotX = Sx;
	*ScreenShotY = Sy;
	*ScreenShotWidth = Sw;
	*ScreenShotHeight = Sh;
	return NULL;
}


//���������
extern _declspec(dllexport) char* Add(const char* Path, const char* KeyName)
{
	HKEY hkey;
	static char err[MAXSIZE];
	/*char path[MAXPATH];

	strcpy_s(path, MAXPATH, Path);*/

	//��ע��������� 
	if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_ALL_ACCESS, &hkey) == ERROR_SUCCESS)
	{
		if (RegSetValueExA(hkey, "�Զ����붤��ֱ����", 0, REG_SZ, (BYTE*)Path, strlen(Path) + 1) == ERROR_SUCCESS)//д��ע���
		{
			RegCloseKey(hkey);//�ر�ע���
			return NULL;
		}
		else
			strcpy_s(err, MAXSIZE, "��ע���д���ֵʧ�ܣ����Թ���ԱȨ������");
	}
	else
		strcpy_s(err, MAXSIZE, "��ע���ʧ�ܣ����Թ���ԱȨ������");

	return err;
}

//ɾ��ע���ʧ��
extern _declspec(dllexport) char* Del(const char* KeyName)
{
	HKEY hkey;
	static char err[MAXSIZE];

	//��ע��������� 
	if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_ALL_ACCESS, &hkey) == ERROR_SUCCESS)
	{
		if (RegQueryValueExA(hkey, "�Զ����붤��ֱ����", NULL, NULL, NULL, NULL) == ERROR_FILE_NOT_FOUND)//�ж��������Ƿ����
		{
			strcpy_s(err, MAXSIZE, "���������");
			return err;
		}
		else
			strcpy_s(err, MAXSIZE, "��ע���ʧ�ܣ����Թ���ԱȨ������");

		if (RegDeleteValueA(hkey, "�Զ����붤��ֱ����") == ERROR_SUCCESS)//ɾ����ֵ
		{
			RegCloseKey(hkey);//�ر�ע���
			return NULL;
		}
		else
			strcpy_s(err, MAXSIZE, "��ע���ʧ�ܣ����Թ���ԱȨ������");
	}
	else
		strcpy_s(err, MAXSIZE, "��ע���ʧ�ܣ����Թ���ԱȨ������");

	return err;
}
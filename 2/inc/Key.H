/**********************************key V2.0 ˵�� ************************************************************************
Keyģ�����ڻ�ȡ��STC-Bѧϰ�塱������������״̬���ṩ����ģ����غ�һ��Ӧ�ú���,һ���������¼���enumEventKey��
  (1)  KeyInit()������ģ����غ�����
  (2)  char GetKeyAct(char Key)����ȡ����״̬��
        ����������Key��ָ��Ҫ��ȡ״̬�İ�����Keyȡֵ��
						enumKey1
						enumKey2
						enumKey3    
��������ȡֵ������Ч��Χ������������fail��
		��������ֵ��
						enumKeyNull���ް���������
						enumKeyPress�����£�
						enumKeyRelease��̧��
						enumKeyFail��ʧ�ܣ� 
	  ����ֵ�Ǿ�����μ�ⰴ��ʵʱ״̬��ͳ�Ƽ��������������������Ч�¼���
    ÿ��������ѯһ�κ�,�¼�ֵ���enumKeyNull���¼�ֵ����ѯһ����Ч��
  (3)  �����¼���enumEventKey
     ������������enumKey1,enumKey2,enumKey3��������һ�������С����¡���̧�𡰶���ʱ��������һ���������¼�������Ӧ�����¼����û������������û���д,����sys���ṩ��SetEventCallBack��������.	 

����˵�������������ADCģ�飬����3��Key3���κβ����ڱ�ģ�鲻�ɼ�⵽�����κ���Ϣ��Ӧ����ʱ����3��Key3���κβ�������ADCģ���м��ͷ�Ӧ��ʹ�÷�����ͬ�������ADCģ��˵����	
	
��д����ɣ��绰18008400450��   2021��3��5����ƣ�2021��8��26�ո���
*/

#ifndef _key_H_
#define _key_H_

extern void KeyInit();
extern unsigned char GetKeyAct(char Key) ;                              //��ȡ����enumKey1,enumKey2,enumKey3�¼�
								 	                                                      //����ֵ��enumKeyNull�����ޣ�enumKeyPress�����½��أ�enumKeyRelease���������أ�enumKeyFail��������
enum KeyName     {enumKey1,enumKey2,enumKey3};                          //������
enum KeyActName  {enumKeyNull,enumKeyPress,enumKeyRelease,enumKeyFail}; //����������

#endif
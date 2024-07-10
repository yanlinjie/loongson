#ifndef EspCommand_H
#define EspCommand_H

#include "sys.h"

/*
ESP8266接收任务码指令：
-- AT 测试指令
-- AT+RST 重启模块
-- ATE0	关闭回显
-- AT+RESTORE 恢复出厂设置功能
-- AT+CWMODE=1 设置为 STA 模式
-- AT+CWJAP="光电科技协会","123guangdian123" 连接路由器
-- AT+CIPSTART="TCP","192.168.1.102",8000	模块连接到 server
-- AT+CIPMODE=1 开启透传模式
-- AT+CIPSEND	开始透传
-- AT+CWDHCP=1,0 关闭STA的DHCP功能
-- AT+CIFSR 获取本地IP
*/

typedef enum{
	ReStart,	//重启模块
	Back_ON,	//开启模块回显功能
	Back_OFF,	//关闭模块回显功能
	ReStore		//恢复出厂设置
}BasicCmdDef;

typedef enum{
	STA,AP,STA_AP,Manual  
}NetTypeDef;

bool ConfigurationEsp8266(void);																		//配置ESP8266接收任务码

bool SendEsp8266Cmd(char *Cmd,char *Ack,u16 Attempts);							//向ESP8266发送AT指令

bool ESP8266_SelfTest(void);																				//ESP8266自检
bool ESP8266_BasicSet(BasicCmdDef Cmd,bool State);								  //配置ESP8266基础设置
bool ESP8266_NetMode(NetTypeDef Mode,bool State);										//配置ESP8266网络模式
bool ESP8266_JoinWifi(char *SSID,char *PassWord,bool State);				//配置ESP8266连接WiFi
bool ESP8266_LinkServe(char *Mode,char *IP,char *Port,bool State);	//配置ESP8266连接服务端
void ESP8266_UnvarnishSend(bool State);															//配置ESP8266透传模式

#endif

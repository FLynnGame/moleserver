#ifndef _GAME_FRAME_MANAGER_H_INCLUDE_
#define _GAME_FRAME_MANAGER_H_INCLUDE_

#include "../include/molnet/Network.h"
#include "../include/Common/defines.h"
#include <string.h>

#include <jsoncpp/json/json.h>

using namespace network;

class GameFrameManager : public Singleton<GameFrameManager>
{
public:
	/// ���캯��
	GameFrameManager();
	/// ��������
	~GameFrameManager();

	/// ���ڴ�����յ���������Ϣ
	void OnProcessNetMes(uint32 connId,CMolMessageIn *mes);

	/// ���ڴ����������������Ϣ
	void OnProcessConnectedNetMes(uint32 connId);

	/// ���ڴ������ڶϿ�����������Ϣ
	void OnProcessDisconnectNetMes(uint32 connId);

private:
	/// �����û���¼ϵͳ��Ϣ
	void OnProcessUserLoginMes(uint32 connId,Json::Value &mes);
	/// �����û���Ϣ������Ϣ
	void OnProcessUserInfoUpdateMes(uint32 connId,Json::Value &mes);
	/// �����û�ע����Ϣ
	void OnProcessUserRegisterMes(uint32 connId,Json::Value &mes);
	/// ����õ���ǰ���߷������б�
	void OnProcessUserGetOnlineRoomMes(uint32 connId);
};

#define ServerGameFrameManager GameFrameManager::getSingleton()

#endif

#ifndef _C_ROBOT_LOGIC_FRAME_H_INCLUDE_
#define _C_ROBOT_LOGIC_FRAME_H_INCLUDE_

#include "cdefines.h"

class CRobotLogicFrame : public RobotLogicFrame
{
public:
	/// ���캯��
	CRobotLogicFrame();
	/// ��������
	~CRobotLogicFrame();

	/// ���õ�ǰ���
	virtual void SetGamePlayer(Player* pPlayer,Room* pRoom) { m_g_myself = pPlayer; m_g_myselfRoom = pRoom;}

	/// ���ڴ����û�׼�������Ϣ
	virtual void OnProcessPlayerReadyMes(int playerId);
	/// ���ڴ����û���ʼ��Ϸ��ʼ��Ϣ
	virtual void OnProcessPlayerGameStartMes();
	/// ���ڴ����û�������Ϸ��Ϣ
	virtual void OnProcessPlayerGameOverMes(void);
	/// ���ڴ����û�������Ϸ��������Ϣ
	virtual void OnProcessPlayerRoomMes(Json::Value &mes);
	/// �����û����뷿����Ϣ
	virtual void OnProcessEnterRoomMsg(int playerId);
	/// �����û��뿪������Ϣ
	virtual void OnProcessLeaveRoomMsg(int playerId);
	/// �����û�����������Ϣ
	virtual void OnProcessReEnterRoomMes(int playerId);
	/// �����û�������Ϣ
	virtual void OnProcessOfflineRoomMes(int playerId);
	/// �����û���ʱ����Ϣ
	virtual void OnProcessTimerMsg(int timerId,int curTimer);

private:
	Player *m_g_myself;
    Room *m_g_myselfRoom;

	int					m_nMinAddId;			//���ټ�ע
	bool				m_bCanThan;				//���Ա���
	int					m_nCurrentUser;			//��ǰ���
	int64				m_nBasicScore;

	PlayStates			m_States[IDD_MAX_ROOM_USERCOUNT];

	int64				m_nUserAdd[IDD_MAX_ROOM_USERCOUNT];
	bool				m_bHasLook[IDD_MAX_ROOM_USERCOUNT];


	int					m_nWinUser;


	//������
	void OnAI();

	void ClearData();
};

#endif
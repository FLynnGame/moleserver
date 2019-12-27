#ifndef _C_GAME_LOGIC_FRAME_H_INCLUDE_
#define _C_GAME_LOGIC_FRAME_H_INCLUDE_

#include "cdefines.h"
#include "GameLogic.h"
#include <map>
#include <vector>

class CServerLogicFrame : public ServerLogicFrame
{
public:
	/// ���캯��
	CServerLogicFrame();
	/// ��������
	~CServerLogicFrame();

	/// ���õ�ǰӦ�÷���
	virtual void SetGameRoom(Room* pRoom) { m_g_GameRoom = pRoom; }
	/// ���ڴ����û���ʼ��Ϸ��ʼ��Ϣ
	virtual void OnProcessPlayerGameStartMes();
	/// ���ڴ����û�������Ϸ��������Ϣ
	virtual void OnProcessPlayerRoomMes(int playerId,Json::Value &mes);
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
	Room *m_g_GameRoom;                                  /**< ��Ϸ���� */

	CzjhGameLogic				m_GameLogic;			//��Ϸ�߼�

	//״̬����
private:
	PlayStates					m_PlayState[IDD_MAX_ROOM_USERCOUNT];							//�Ƿ�����
	bool						m_bLook[IDD_MAX_ROOM_USERCOUNT];								//�Ƿ���
	bool						m_bCanThan;														//�Ƿ���Ա���
	int							m_nCanThanUser;

	//��Ϸ����
private:
	WORD						m_wCardData[IDD_MAX_ROOM_USERCOUNT][IDD_MAX_HAND_COUNT];		//�������
	std::map<uint32,std::string>m_strUserName;							//�������
	int64						m_nUserScore[IDD_MAX_ROOM_USERCOUNT];							//��ҽ�ң������ж�ʲôʱ�������ƣ�
	int64						m_nAddScore[IDD_MAX_ROOM_USERCOUNT];							//��ǰ���½��
	int							m_nBasicAddId;													//�����ע
	int64						m_nBasicScore;													//��λ��ע

	//���Ʊ���
	int							m_nThanUser[IDD_MAX_ROOM_USERCOUNT];

	//���б���
	int							m_nBankUser;													//ׯ��
	int							m_nCurrentUser;													//��ǰ���
	int							m_nAllUserNum;													//��ǰ�����

	bool						m_bGameOver;
	int64						m_nWinScore[IDD_MAX_ROOM_USERCOUNT];
	bool						m_bOffLine[IDD_MAX_ROOM_USERCOUNT];

private:
	//��ע
	void OnUserAddScore(int playerID, int addId);
	//����
	void OnUserWatchCard(int playerID);
	//����
	void OnUserThanCard(int player1,int player2);
	//����
	void OnUserGiveUp(int playerID);
	//��Ϸ����
	void OnUserGameEnd(bool isGiveUp);
	//���Ϳ��Ա�����Ϣ
	void SendCanThan();

private:
	//�Ƿ�������
	bool isMustThan(int playerID);
	//��ȡ��һ���������
	int GetNextUser(int PlayerID);

private:
	//�������
	void ClearData();
	//����
	bool ForTest();
};

#endif

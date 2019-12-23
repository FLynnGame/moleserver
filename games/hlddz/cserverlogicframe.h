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

protected:
	int								 m_bCallScorePhase;			//���ƽ׶�
	int								 m_curGameState;			//��ǰ��Ϸ״̬

	//�������
protected:
	CGameLogic						m_GameLogic;						//��Ϸ�߼�

	//�˿���Ϣ
protected:
	uint8							m_bBackCard[3];						//��Ϸ����
	int								m_bCardCount[GAME_PLAYER];			//�˿���Ŀ
	int								m_bOutCardCount[GAME_PLAYER];		//���ƴ���
	uint8							m_bHandCardData[GAME_PLAYER][20];	//�����˿�

	uint8							m_cbLastTurnOutCard[GAME_PLAYER][MAX_COUNT];	//���ֳ�������
	uint8							m_cbNowTurnOutCard[GAME_PLAYER][MAX_COUNT];		//���ֳ�������
	int								m_nLastTurnOutCount[GAME_PLAYER];				//���ֳ�����Ŀ
	int								m_nNowTurnOutCount[GAME_PLAYER];				//���ֳ�����Ŀ

	//��Ϸ����
protected:
	int							m_cbCount;							//��ֹ�ط�����
	int							m_lSnatchLand;						//����������
	int							m_lBrightCard;						//���Ʊ���
	int							m_wBombTime;						//ը������
	int							m_lThuntian;						//���챶��
	int							m_bLandScore;						//��������
	int							m_wShowTimes;						//��ʾ����
	uint16						m_wFirstUser;						//�׽��û�
	uint16						m_wBankerUser;						//ׯ���û�
	uint16						m_wCurrentUser;						//��ǰ���
	int							m_bScoreInfo[3];					//�з���Ϣ
	uint16						m_wChairID;							//��ʱ�������Ӻ�
	int							m_breSendCardTimes;					//���·��ƴ���
	bool						m_GameOver;							//��Ϸ����
	uint16						m_GameOverChair;					//��Ϸ�������߻�����ҵ����Ӻ�
	bool						m_UserStandUp[GAME_PLAYER];			//�û�������ʶ

	//������Ϣ
protected:
	uint16							m_wTurnWiner;						//ʤ�����
	int								m_bTurnCardCount;					//������Ŀ
	uint8							m_bTurnCardData[20];				//�����б�
	bool							m_bUserTrustee[GAME_PLAYER];		//����й�
	bool                            m_bUserOffLine[GAME_PLAYER];        //��ҵ���״̬
	bool							m_PassTime[GAME_PLAYER];			//���ߡ�������ʱ��
	bool							m_bUserPass[GAME_PLAYER];			//��ҡ�������
	bool							m_bEndFlag;
	bool							m_bYaPaiFlag;
	bool							m_bEndOrReadyFlag;					//�Ŷӻ�����׶�
	//bool							m_FirstUserLife;					//����׶��ǲ��ǵ�һ������뿪
	bool							m_wCloseWarmingFlash;
	int                             m_UserLeaveCount;					//����뿪����
	bool							m_MatchOver;						//����������һ�ֱ�ʶ

	//�зֽ׶α���
protected:
	bool							m_bBrightCard[GAME_PLAYER];			//����ģʽ
	bool							m_bGiveUpBanker[GAME_PLAYER];		//������ʶ
	int							    m_cbLandScoreCount;					//�зִ���

protected:
	int								m_BoomCount;					//�ѳ�ը����(����jackPot)
};

#endif

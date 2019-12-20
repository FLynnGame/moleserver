#ifndef _C_GAME_LOGIC_FRAME_H_INCLUDE_
#define _C_GAME_LOGIC_FRAME_H_INCLUDE_

#include "cdefines.h"
#include "GameLogic.h"
#include <map>
#include <vector>

#include "czhuangmanager.h"

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
	void GetPlayerPanData(void);
	void TongJiUserGamingResult(void);
	void startgame(void);
	void readygame(void);
	std::string getCardTypeName(YaZhuType ptype)
	{
		switch(ptype)
		{
		case YAZHUTYPE_TIAN:
			{
				return "��";
			}
			break;
		case YAZHUTYPE_DI:
			{
				return "��";
			}
			break;
		case YAZHUTYPE_XUAN:
			{
				return "��";
			}
			break;
		case YAZHUTYPE_HUANG:
			{
				return "��";
			}
			break;
		default:
			break;
		}

		return "";
	}
	int64 getuserareajetton(YaZhuType ptype);
	void masterwinorlost(bool iswin);
	void allareawinorlost(bool iswin);
	void thetypewin(YaZhuType ptype,bool isWin=true);
	void copycardtocards(uint8 *pcard,YaZhuType ptype);
	int64 getrealuserjettontotal(void);
	int64 getuserswinandlostresult(void);
	int64 getuserjettontotal(int pChairId);
	int64 getusertotalresult(void);
	/// �������������
	void LoadServerConfig(void);

private:
	Room *m_g_GameRoom;                                  /**< ��Ϸ���� */

	CGameLogic m_CGameLogic;
	CZhuangManager m_ZhuangManager;

	std::map<int32,std::map<YaZhuType,int64> >        m_xiazhus;
	GameState                                         m_GameState;

	uint8                                              m_cards[25];
	uint8                                              m_zhuangcards[5];
	uint8                                              m_othercards[4][5];

	int                            m_timegamestart;
	int                            m_timexiazhu;
	int                            m_timekaipai;
	int                            m_timejiesuan;
	int                            m_timecurlost;

	bool                           m_issupercontrol;
};

#endif

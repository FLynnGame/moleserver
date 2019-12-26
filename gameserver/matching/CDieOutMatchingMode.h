#ifndef _C_DIE_OUT_MATCHING_MODE_H_INCLUDE_
#define _C_DIE_OUT_MATCHING_MODE_H_INCLUDE_

#include "CBasicMatchingMode.h"
#include "../../include/molnet/Mutex.h"

/**
 * �������ṹ
 */
struct MatchStruct
{
	MatchStruct():isMatching(false),matchcount(0),roomIndex(-1),isEnable(false),matchendcount(0) {}
	MatchStruct(bool im,std::vector<CPlayer*> &ppl,int mc,int ri,bool enable,int mec)
		: isMatching(im),playerlist(ppl),matchcount(mc),roomIndex(ri),isEnable(enable),matchendcount(mec) {}

	void clear(void)
	{
		isMatching = false;
		playerlist.clear();
		matchcount = 0;
		matchendcount = 0;
		roomIndex = -1;
		isEnable=false;
		oldroomlist.clear();
		curroomlist.clear();
		oldroomlisttimer.clear();
		oldroomlistString.clear();
	}

	bool isMatching;                 // �Ƿ��ڱ�����
	std::vector<CPlayer*> playerlist; // ������������
	int matchcount;                    // �����ܵ�����
	int roomIndex;                     // ������ʹ�õķ�������
	bool isEnable;                     // �ⳡ�����Ƿ����
	int matchendcount;                 // ��ǰ������������
	std::map<int,int> oldroomlist,curroomlist;        // ��ǰ�����õķ��䣨��Ҫ���ڼ�ص�ǰ��������
	std::map<int,DWORD> oldroomlisttimer;
	std::map<std::string,int> oldroomlistString;
};

/**
 * ��̭��
 */
class CDieOutMatchingMode : public BasicMatchingMode
{
public:
	/// ���캯��
	CDieOutMatchingMode(matchMode pmatchMode);
	/// ��������
	virtual ~CDieOutMatchingMode();

	/// �����������
	virtual void Clear(void);
	/// ���ָ��������Ƿ��ڱ�����
	virtual bool IsExist(CPlayer *pPlayer);
	/// �����ҵ���������
	virtual bool AddPlayer(CPlayer *pPlayer);
	/// ��ȡָ����ұ���������
	virtual void GetPlayerRanking(CPlayer *pPlayer);
	/// �ӱ�������ɾ�����
	virtual bool DeletePlayer(CPlayer *pPlayer,bool isMatching=false);
	/// �õ�������������
	virtual int GetMatchPlayerCount(void);
	/// ��ӱ����õ�����
	virtual bool AddRoom(CRoom *pRoom);

	/// ����������
	virtual void Update(void);

private:
	/// ��ס�������
	inline void LockTableFramePlayerList(void) { m_tableframePlayerListLock.Acquire(); }
	/// �����������
	inline void UnLockTableFramePlayerList(void) { m_tableframePlayerListLock.Release(); }
	/// �õ��������
	inline std::vector<MatchStruct>& GetTablePlayerList(void) { return m_tableframePlayerList; }
	/// �õ�һ���յı�������
	int GetEmptyMatchingRoom(void);
	/// ���ݱ����������õ�������
	MatchStruct* GetMatchStructById(int index);

	/// ��ʼ�������
	void OnProcessMatching(MatchStruct *pMatchStruct);
	/// ��ָ������������ҷ��ͱ�������
	void SendMatchingRanking(MatchStruct *pMatchStruct,bool issort=true);
	/// ��ʼ����
	void StartMatching(MatchStruct *pMatchStruct,bool isConntinue=false);
	/// ���һ�ֱ����Ƿ����
	bool IsOverOneLoopMatching(MatchStruct *pMatchStruct);
	/// �����������û�ȫ���뿪���䣬׼��������һ��
	void AllPlayerLeaveMatchingToNext(MatchStruct *pMatchStruct);
	/// ��������
	void MatchingOver(MatchStruct *pMatchStruct);
	/// ��������
	void MatchingContinue(MatchStruct *pMatchStruct);
	/// ����ָ���������ĵȴ�����
	void SendMatchingWaitingPlayerCount(MatchStruct *pMatchStruct);

private:
	std::vector<MatchStruct> m_tableframePlayerList;             /**< ���ڹ������е���� */
	std::vector<std::vector<CRoom*> > m_tableframeroomlist;      /**< �����õ����� */
	Mutex  m_tableframePlayerListLock;                                    /**< ���ڱ���������� */
	bool m_isAutoMatchingPlayerCount;                            /**< �Ƿ��Զ�ƥ����Ҳ���������һ��ֻ��ƥ��һ�� */
};

#endif

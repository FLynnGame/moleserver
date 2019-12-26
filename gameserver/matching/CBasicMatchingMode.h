#ifndef C_BASIC_MATCHING_MODE_HEAD_FILE
#define C_BASIC_MATCHING_MODE_HEAD_FILE

#include <vector>
#include <list>

#include "../../include/molnet/common.h"
#include "../gameframe/CPlayer.h"
#include "../gameframe/CRoom.h"

class CPlayer;
typedef CPlayer* CPlayerPtr;
class CRoom;

//////////////////////////////////////////////////////////////////////////

/**
 * ����ģʽ
 */
enum matchMode
{
	MATCHMODE_LOOPMATCH = 0,                // ѭ����
	MATCHMODE_GROUPSMATCH,              // ������̭��
	MATCHMODE_NULL
};

/**
 * ������������
 */
class BasicMatchingMode
{
public:
	/// ���캯��
	BasicMatchingMode(matchMode pmatchMode):m_matchMode(pmatchMode) {}
	/// ��������
	virtual ~BasicMatchingMode() {}

	/// ���ñ���ģʽ
	inline void SetMatchingMode(matchMode pmode) { m_matchMode = pmode; }
	/// �õ�����ģʽ
	inline matchMode GetMatchingMode(void) { return m_matchMode; }

	/// �����������
	virtual void Clear(void) {}
	/// ���ָ��������Ƿ��ڱ�����
	virtual bool IsExist(CPlayer *pPlayer) { return true; }
	/// �����ҵ���������
	virtual bool AddPlayer(CPlayer *pPlayer) { return true; }
	/// ��ȡָ����ұ���������
	virtual void GetPlayerRanking(CPlayer *pPlayer) {}
	/// �ӱ�������ɾ�����
	virtual bool DeletePlayer(CPlayer *pPlayer,bool isMatching=false) { return true; }
	/// �õ�������������
	virtual int GetMatchPlayerCount(void) { return 0; }
	/// ��ӱ����õ�����
	virtual bool AddRoom(CRoom *pRoom) { return true; }

	/// ����������
	virtual void Update(void) {}

private:
	matchMode m_matchMode;                         /**< ����ģʽ */
};

/**
 * ���ӹ����࣬���ڹ������е����ӣ�Ȼ��ʵ�ָ��ָ����ı���
 */
class CTabelFrameManager : public Singleton<CTabelFrameManager>
{
public:
	/// ���캯��
	CTabelFrameManager();
	/// ��������
	~CTabelFrameManager();

	/// ���õ�ǰʹ�õı���
	inline void SetMatchingMode(BasicMatchingMode *pBasicMatchingMode) { m_BasicMatchingMode = pBasicMatchingMode; }
	/// �õ���ǰʹ�õı���
	inline BasicMatchingMode* GetMatchingMode(void) { return m_BasicMatchingMode; }

	/// ���ñ�������
	inline void SetMatchingType(matchMode type) { m_matchMode = type; }
	/// �õ���������
	inline matchMode GetMatchingType(void) { return m_matchMode; }
	/// �������������
	inline void SetMaxStartPlayerCount(int count) { m_MaxStartPlayerCount = count; }
	/// �õ����������
	inline int GetMaxStartPlayerCount(void) { return m_MaxStartPlayerCount; }
	/// ���ñ���ѭ������
	inline void SetMatchingLoopCount(int count) { m_MaxMatchingLoopCount = count; }
	/// �õ�����ѭ������
	inline int GetMatchingLoopCount(void) { return m_MaxMatchingLoopCount; }

	/// �����������
	void Clear(void);
	/// ���ָ��������Ƿ��ڱ�����
	bool IsExist(CPlayer *pPlayer);
	/// �����ҵ���������
	bool AddPlayer(CPlayer *pPlayer);
	/// ��ȡָ����ұ���������
	void GetPlayerRanking(CPlayer *pPlayer);
	/// �ӱ�������ɾ�����
	bool DeletePlayer(CPlayer *pPlayer,bool isMatching=false);
	/// �õ�������������
	int GetMatchPlayerCount(void);
	/// ��ӱ����õ�����
	bool AddRoom(CRoom *pRoom);

	/// ����������
	void Update(void);

private:
	matchMode m_matchMode;                                                /**< ����ģʽ */
	int m_MaxStartPlayerCount;                                            /**< ��������� */
	int m_MaxMatchingLoopCount;                                           /**< ����ѭ������ */

	BasicMatchingMode *m_BasicMatchingMode;                               /**< ���ǵ�ǰʹ�õı��� */
};

//////////////////////////////////////////////////////////////////////////

#endif

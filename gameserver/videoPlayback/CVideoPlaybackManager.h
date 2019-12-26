#ifndef _C_VIDEO_PLAYBACK_MANAGER_H_INCLUDE_
#define _C_VIDEO_PLAYBACK_MANAGER_H_INCLUDE_

#include <vector>
#include <string>

#include "../../include/molnet/common.h"
#include "../../include/Common/Player.h"
#include "../../include/Common/Room.h"

#define VIDEO_BUFFER_SIZE 2048

/**
 * ¼���ļ��ṹ
 */
struct FileHeader
{
    char marker[5];             // �ļ���ʶ "X","B","V","O","D"
	char gamelogic[256];        // ��Ϸ�߼�dll
	uint32 useMe;               // ����Լ���ID
	int wUserCount;             // ��Ҹ���
	int wFrameCount;            // ֡��
};

/**
 * ¼��״̬
 */
enum VideoState
{
	VIDEO_RECORDING = 0,                       //¼��
	VIDEO_PLAYBACK,                            //�ط�
	VIDEO_NULL
};

/**
 * ¼������ҽṹ
 */
struct VideoGameStep
{
	VideoGameStep()
		: wTimeSpace(0),wDataSize(0)
	{
	}
	VideoGameStep(DWORD ts,const void *buf,WORD size)
		: wTimeSpace(ts)
	{
		if(buf && size < VIDEO_BUFFER_SIZE)
		{
			memcpy(wBuffer,buf,size);
			wDataSize = size;
		}
	}

	DWORD wTimeSpace;                          // ʱ��
	char wBuffer[VIDEO_BUFFER_SIZE];           // ����
	WORD wDataSize;                            // ���ݳ���
};

/**
 * ¼��ط��õ�����ҽṹ
 */
struct VPPlayer
{
	VPPlayer():m_Id(0),m_ConnectId(0),m_roomId(0),m_ChairIndex(0),m_isLookOn(false),m_Money(0),m_BankMoney(0),
		m_TotalResult(0),m_Revenue(0),m_level(0),m_experience(0),m_totalbureau(0),m_successbureau(0),m_RunawayBureau(0),
		m_runawayrate(0.0f),m_successrate(0.0f),sex(0),gtype(0),ipaddress(0),m_DeviceType(PLAYERDEVICETYPE_PC)
	{}

	VPPlayer(int id,uint32 connectid,int roomid,int chairindex,PlayerState playerstate,PlayerType playertype,
		bool islookon,std::string name,int64 money,int64 bankmoney,int64 totalresult,int64 revenue,int level,
		unsigned int experience,std::string useravatar,int totalbureau,int successbureau,int runawaybureau,
		int failbureau,float successrate,float runawayrate,int psex,std::string prealname,int pgtype,uint32 pipaddress,
		PlayerDeviceType pdt)
		: m_Id(id),m_ConnectId(connectid),m_roomId(roomid),m_ChairIndex(chairindex),m_PlayerState(playerstate),
		  m_PlayerType(playertype),m_isLookOn(islookon),m_Money(money),m_BankMoney(bankmoney),m_TotalResult(totalresult),
		  m_Revenue(revenue),m_level(level),m_experience(experience),m_totalbureau(totalbureau),
		  m_successbureau(successbureau),m_RunawayBureau(runawaybureau),m_failbureau(failbureau),m_successrate(successrate),
		  m_runawayrate(runawayrate),sex(psex),gtype(pgtype),ipaddress(pipaddress),m_DeviceType(pdt)
	{
		strncpy(m_Name,name.c_str(),128);
		strncpy(m_useravatar,useravatar.c_str(),128);
		strncpy(realname,prealname.c_str(),128);
	}

	int m_Id;         /**< ���ID */
	uint32 m_ConnectId;  /**< ��ҵ�����ID */
	int m_roomId;     /**< ������ڵķ���ID */
	int m_ChairIndex;   /**< �û��ڷ����е����� */
	PlayerState m_PlayerState;       /**< ��ҵ�ǰ״̬ */
	PlayerType m_PlayerType;           /**< ������� */
	PlayerDeviceType m_DeviceType;     /**< ����豸���� */
	bool m_isLookOn;                   /**< �Ƿ�Ϊ�Թ� */

	char m_Name[128];  /**< ��ҵ����� */

	int64 m_Money;              /**< ��ҽ�Ǯ���� */
	int64 m_BankMoney;                /**< ���н�Ǯ���� */
	int64 m_TotalResult;              /**< ����ܵ���Ӯֵ */
	int64 m_Revenue;                  /**< ���˰�� */
	int m_level;                       /**< ��ҵȼ� */
	unsigned int m_experience;         /**< ��Ҿ���ֵ */
	char m_useravatar[128];         /**< �û�ͷ�� */
	int m_totalbureau;                 /**< ����ܾ� */
	int m_successbureau;               /**< ���Ӯ�� */
	int m_RunawayBureau;			   /**< ������ܴ��� */
	int m_failbureau;                  /**< ������ */
	float m_successrate;               /**< ���ʤ�� */
	float m_runawayrate;               /**< ��������� */

	int sex;                      //����Ա�
	char realname[128];           //��ʵ����
	int gtype;                    //�������
	uint32 ipaddress;             /**< ���IP */
};

/**
 * ¼��ط����õ��ķ���ṹ
 */
struct VPRoom
{
	VPRoom():m_Id(0),m_MaxCount(0),m_RoomRevenue(0),m_RoomLastMoney(0),m_masterId(0),m_curPlayerId(0)
	{}

	VPRoom(int id,std::string name,int maxcount,RoomType roomtype,RoomState roomstate,
		enStartMode startmode,float revenue,int lastmoney,int master,int curplayer)
		: m_Id(id),m_MaxCount(maxcount),m_RoomType(roomtype),m_RoomState(roomstate),
		  m_RoomGameType(startmode),m_RoomRevenue(revenue),m_RoomLastMoney(lastmoney),m_masterId(master),
		  m_curPlayerId(curplayer)
	{
		strncpy(m_Name,name.c_str(),128);
	}

	int m_Id;                   /**< ����ID */
	char m_Name[128];           /**< �������� */
	int m_MaxCount;             /**< �����������Ҹ��� */
	RoomType m_RoomType;        /**< �������� */
	RoomState m_RoomState;      /**< ���䵱ǰ״̬ */
	enStartMode m_RoomGameType; /**< ������Ϸ���� */
	float m_RoomRevenue;        /**< ����˰�� */
	int m_RoomLastMoney;        /**< ������С��� */

	int m_masterId;             /**< ��ǰ����ID */
	int m_curPlayerId;          /**< ��ǰ���ID */
};
class CVideoPlaybackManager
{
public:
	/// ���캯��
	CVideoPlaybackManager();
	/// ��������
	~CVideoPlaybackManager();

	/// ���浽�ļ�
	bool Sava(std::string filename);
	/// �����¼�ļ�
	bool Load(std::string filename);
	/// �����������
	void Clear(void);

	/// ����¼��״̬
	inline void SetState(VideoState state) { m_VideoState = state; }
	/// �õ�¼��״̬
	inline VideoState GetState(void) { return m_VideoState; }
	/// ����Ҫ���ص���Ϸ���
	inline void SetGameDll(std::string dllname) { m_hDllGameName = dllname; }
	/// �õ�Ҫ���ص���Ϸ���
	inline std::string GetGameDll(void) { return m_hDllGameName; }
	/// ������е�֡
	void ClearAllFrames(void);
	/// �õ�֡��
	inline int GetFrameCount(void) { return (int)m_VideoGameSteps.size(); }
	/// ����һ֡
	inline void AddFrame(VideoGameStep *pGameStep) { m_VideoGameSteps.push_back(pGameStep); }
	/// �õ�ָ��������֡
	VideoGameStep* GetFrame(int index);
	/// �õ���ǰ֡����
	inline int GetCurrentFrame(void) { return m_curFrameIndex; }
	/// ���õ�ǰ֡����
	inline void SetCurrentFrame(int frame) { m_curFrameIndex = frame; }

	/// ����һ�����
	inline void AddUser(VPPlayer pUserData) { m_UserDatas.push_back(pUserData); }
	/// �õ�ָ�����������
	VPPlayer* GetUser(int index);
	/// ���������ڷ���
	inline void AddRoom(VPRoom pRoom) { m_mySelfRoom = pRoom; }
	/// �õ�������ڷ���
	inline VPRoom* GetRoom(void) { return &m_mySelfRoom; }
	/// �õ���Ҹ���
	inline int GetUserCount(void) { return (int)m_UserDatas.size(); }

	/// ����¼���ߵ�ID
	inline void SetMeUserID(uint32 id) { m_mySelfID = id; }
	/// �õ�¼���ߵ�ID
	inline uint32 GetMeUserID(void) { return m_mySelfID; }
	/// ����¼���ߵ����Ӻ�
	inline void SetMeUserChairIndex(int32 index) { m_mySelfChairIndex = index; }
	/// �õ�¼���ߵ����Ӻ�
	inline int32 GetMeUserChairIndex(void) { return m_mySelfChairIndex; }

private:
	std::vector<VideoGameStep*> m_VideoGameSteps;         // ���ڱ���ÿһ֡
	std::vector<VPPlayer> m_UserDatas;                      // �������
	uint32 m_mySelfID;                                    // ����Լ���ID
	int32 m_mySelfChairIndex;                              // ����Լ������Ӻ�
	VPRoom m_mySelfRoom;                                   // ������ڵķ���
	int m_curFrameIndex;                                  // ��ǰ֡
	VideoState m_VideoState;                              // ��ǰ¼��״̬
	std::string  m_hDllGameName;                          /**< Ҫ������Ϸ������ */
};

#endif

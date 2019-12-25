#ifndef _CMDHELPER_H_
#define _CMDHELPER_H_

#include "cdefines.h"
#include <string>
#include <sstream>
//////////////////////////////////////////////////////////////////////////
#define MAX_SELECT_GANG					4									/*!< ����ѡ������� */
#define MAX_FAN_STYLE					20									/*!< ������������� */
//////////////////////////////// ��������Ϣ //////////////////////////////////////////

/*
//! �����齫
/*! ����,ׯ�ҿ��ܷ��ƾ��в���,��ܡ��� */
typedef struct tagCMD_S_DealMJ
{
	int32							iBankerUser;							/*!< ׯ�� */
	int32							iIsRandomBanker;						/*!< �Ƿ����ȷ��ׯ�� */
	int32 							iBaseScore;								/*!< �������� */
	int32							iDiceNum01;								/*!< ������01 ȷ��ׯ�� */
	int32							iDiceNum02;								/*!< ������02 ȷ��ׯ��*/
	int32							iDiceNum11;								/*!< ������11 ȷ������λ��*/
	int32							iDiceNum12;								/*!< ������12 ȷ������λ��*/

	int32							iUserChairID;							/*!< �����λ�� */

	int32							iHandMJCount;							/*!< �齫��Ŀ */
	MJBYTE							bHandMJ[MAX_HAND_MJ];					/*!< �������齫�� */
	MJFLAG							bOperation;
	int32							iStatMJPos;								/*!< ��ʼ����λ�� */

	int32							iGangCount;								/*!< ����Ŀ */
	MJBYTE							bMJGang[MAX_SELECT_GANG];				/*!< �ɸܵ���*/

	int size()
	{
		return sizeof(tagCMD_S_DealMJ);// - (MAX_HAND_MJ-iHandMJCount)*sizeof(MJBYTE) - (MAX_HAND_MJ-iMJGangCount)*sizeof(MJBYTE);
	}
}CMD_S_DealMJ, *PCMD_S_DealMJ;

/*! ����
/*! ���ƺ�,������ҿ����еĲ��� */
typedef struct tagCMD_S_OutMJ
{
	MJBYTE							bOutMJ;									/*!< ������� */
	int32							wOutMJUser;								/*!< ������� */

	MJFLAG							bOperation;
	int32							isOtherPriority;

	int32							isTimeOut;								/*!< �Ƿ�ʱ�ɷ��������� */

	int size()
	{
		return sizeof(tagCMD_S_OutMJ);
	}
}CMD_S_OutMJ,*PCMD_S_OutMJ;

//! ����
/*! ���ƺ�,�����еĲ��� */
typedef struct tagCMD_S_TouchMJ
{
	MJBYTE							bTouchMJ;								/*!< ���� */
	int32							bTouchType;								/*!< 0���������ƣ�1������ǽ�������� */
	int32							wCurrentUser;							/*!< ��ǰ��� */

	MJFLAG							bOperation;
	int32							isOtherPriority;
	MJFLAG							bGangType;								/*!< �ܵ�����,��������,bOperation����GANG����Ч */

	int32							iGangCount;								/*!< ����Ŀ */
	MJBYTE							bMJGang[MAX_SELECT_GANG];				/*!< �ɸܵ���*/

	int size()
	{
		return sizeof(tagCMD_S_TouchMJ);
	}
}CMD_S_TouchMJ, *PCMD_S_TouchMJ;

//! �û�����
/*! �û����ƺ�,������ҿ����еĲ��� */
typedef struct tagCMD_S_PengMJ
{
	int32							wLastOutUser;							/*!< �ϴγ������ */
	int32							wCurrentUser;							/*!< ������� */
	MJBYTE							bMJPeng;								/*!< ������ */

	MJFLAG							bOperation;
	int32							isOtherPriority;

	int size()
	{
		return sizeof(tagCMD_S_PengMJ);
	}
}CMD_S_PengMJ, *PCMD_S_PengMJ;

//! ����
/*! ���ƺ�,������ҿ����еĲ��� */
typedef struct tagCMD_S_GangMJ
{
	MJFLAG							bGangType;								/*!< �������� */
	int32							wCurrentUser;							/*!< ������� */
	int32							wOutMJUser;								/*!< ������� */
	MJBYTE							bMJGang;								/*!< �������� */

	MJFLAG							bOperation;
	int32							isOtherPriority;

	int32							iGangScore;									/*!< �η�����۳��ķ��� */

	int size()
	{
		return sizeof(tagCMD_S_GangMJ);
	}
}CMD_S_GangMJ, *PCMD_S_GangMJ;

//! �������Բ���
/*! �������Բ�����,������ҿ����еĲ��� */
typedef struct tagCMD_S_GiveUp
{
	MJFLAG							bOperation;								/*!< �������˵Ĳ���Ȩ�� */
	MJBYTE							bOutMJ;									/*!< ���Ե������� */
	int32							isOtherPriority;						/*!< �����Ƿ������ȼ��ߵĲ�����ֻ��Ե�ǰ��ң� */
	int32							isOtherHu;								/*!< һ�ڶ������ʱ������������ҿ��Ժ�*/
	int32							iOperUser;								/*!< ��ǰ������� */
	int32							iLastOutUser;							/*!< ��������� */

	int size()
	{
		return sizeof(tagCMD_S_GiveUp);
	}
}CMD_S_GiveUp, *PCMD_S_GiveUp;

//! ��Һ���
/*! ���ƺ���� */
typedef struct tagCMD_S_HU
{
	MJBYTE							bUserHuMJ;										/*!< ���������� */
	int32							iHuUser;										/*!< ���Ƶ��ˣ� ����������ǿ�ˣ���ô�������ǿ����� */
	int32							iDianPaoUser;									/*!< ���ڵ��ˣ� �������������ô�������Ϊ�� */

	enHuType						enhuType;										/*!< ��Ϸ�������� */

	int64							iNomalFan;											/*!< ������ҵ��ܷ���*/
	int64							iExtraFan;										/*!< ������ҵ��ܷ���*/
	int64							iHuScore;										/*!< ���ķ���*/
	int64							iGangScore;										/*!< �η�����ķ���*/
	int64							iDianPaoScore;									/*!< ������ڵķ���*/
	int64							iAllScore;										/*!< ����ܷ�*/

	int32							iFanStyle[MAX_FAN_STYLE];						/*!< ��������*/

	int32							iAlreadyHu;										/*!< �Ѿ�����������*/

	MJFLAG							bOperation;										/*!< �������˵Ĳ���Ȩ�� */
	int32							isOtherPriority;								/*!< �����Ƿ������ȼ��ߵĲ�����ֻ��Ե�ǰ��ң� */
	int32							isNowOper;										/*!< ǿ��������ڲ����������ڳ��� */
	int32							isOtherHu;										/*!< һ�ڶ���ʱ�������Ƿ��ܺ�������Ϊ1 */

	MJFLAG							bGangType;
	int32							iGangCount;
	MJBYTE							bMJGang[MAX_SELECT_GANG];

	int size()
	{
		return sizeof(tagCMD_S_HU);
	}
}CMD_S_HU, *PCMD_S_HU;


//! ��Һ���
/*! ���ƺ���� */
typedef struct tagCMD_S_GameOver
{
	MJBlock							bHUMj[GAME_PLAYER][MAX_BLOCK_COUNT];			/*!< �������ƿ�(���ܵ���) */
	MJBYTE							bHUMjCount[GAME_PLAYER];						/*!< �����ƿ������ */
	MJBYTE							bHandMj[GAME_PLAYER][MAX_HAND_MJ];				/*!< ���е��� */
	MJBYTE							bHandMjCount[GAME_PLAYER];						/*!< ���е��Ƶ����� */
	MJBYTE							bOutMj[GAME_PLAYER][MAX_OUT_MJ];				/*!< �Ѿ������� */
	MJBYTE							bOutMjCount[GAME_PLAYER];						/*!< �Ѿ������Ƶ����� */

	int32							iDianPaoUser[GAME_PLAYER];						/*!< ���ڵ��ˣ� �������������ô�������Ϊ�� */

	enEndGameType					enEndType;										/*!< ��Ϸ�������� */
	enHuType						enhuType[GAME_PLAYER];							/*!< ��Ϸ�������� */

	int64							iNomalFan[GAME_PLAYER];							/*!< ÿ����ҵ��ܷ���*/
	int64							iExtraFan[GAME_PLAYER];
	int64							iFanScore[GAME_PLAYER];							/*!< ÿ����ҷ����÷�*/
	int64							iGangScore[GAME_PLAYER];						/*!< ÿ����ҹη�����÷�*/
	int64							iTotalScore[GAME_PLAYER];						/*!< �����ܷ���*/

	enUserEndType					enUSEndType[GAME_PLAYER];						/*!< ��Ϸ����ʱ��ҵ�״̬��ƽ����������δ��*/

	int32							isChangeTable;									/*!< �Ƿ���*/

	int32							iFanStyle[GAME_PLAYER];							/*!< ��������*/

	int size()
	{
		return sizeof(tagCMD_S_GameOver);
	}
}CMD_S_GameOver, *PCMD_S_GameOver;

//! �й�
/*! �й� */
typedef struct tagCMD_S_Trustee
{
	int32							bUserTrustee;				//�Ƿ��й�	0��ȡ���йܣ�1����ʼ�й�
	int32							iUserChairID;				//���������

	int size()
	{
		return sizeof(tagCMD_S_Trustee);
	}
}CMD_S_Trustee, *PCMD_S_Trustee;


//! ����ػ���Ϸ
/*! ����ػ���Ϸ�������ػ����� */
typedef struct tagCMD_S_ReEnterRoom
{
	int32							iBankerUser;							/*!< ׯ�� */
	int32 							iBaseScore;								/*!< �������� */
	int32							iIsRandomBanker;						/*!< �Ƿ����ȷ��ׯ�� */
	int32							iDiceNum01;								/*!< ������01 ȷ��ׯ�� */
	int32							iDiceNum02;								/*!< ������02 ȷ��ׯ��*/
	int32							iDiceNum11;								/*!< ������11 ȷ������λ��*/
	int32							iDiceNum12;								/*!< ������12 ȷ������λ��*/

	int32							iUserChairID;							/*!< �����λ�� */

	int32							iHandMJCount[GAME_PLAYER];				/*!< �����齫��Ŀ */
	MJBYTE							bHandMJ[GAME_PLAYER][MAX_HAND_MJ];		/*!< �����齫�� */
	int32							iHuMJCount[GAME_PLAYER];				/*!< �����齫��Ŀ */
	MJBlock							bHuMJ[GAME_PLAYER][MAX_BLOCK_COUNT];	/*!< �����齫 */
	int32							iOutMJCount[GAME_PLAYER];				/*!< �����齫��Ŀ */
	MJBYTE							bOutMJ[GAME_PLAYER][MAX_OUT_MJ];		/*!< �����齫 */

	enPlayerStatu					enUserStatu[GAME_PLAYER];				/*!< ���״̬ */

	int32							iOldOperUser;							/*!< ������� */
	int32							iCurrentUser;							/*!< ��ǰ��� */

	int32							isOtherPriority;

	int64							iHuFan;										/*!< ���Ƶ��ܷ���*/
	int64							iGangFan;									/*!< �η�������ܷ���*/
	int64							iHuScore;									/*!< ���ķ���*/
	int64							iGangScore;									/*!< �η�����۳��ķ��� */
	int64							iFanStyle[MAX_FAN_STYLE];					/*!< ��������*/

	enHuType						enhuType;										/*!< ��Ϸ�������� */
	int32							iDianPaoUser;								/*!< ���ڵ��ˣ� �������������ô�������Ϊ�� */
	MJBYTE							bDianPaoMJ[GAME_PLAYER];							/*!< �����ڵ���ʹ�ã�δ��Ϊ0*/

	int32							isForceQuit;								/*< �Ƿ��Ѿ����߳����� */
	int32							isYiPaoDuoXiang;							/*< ������˺��ƣ��ǲ���һ�ڶ��� */
	int32							iReadyPlayer;								/*< �����������ڷ�����֮ǰ����������м������Ѿ�׼������ */

	int size()
	{
		return sizeof(tagCMD_S_ReEnterRoom);
	}
}CMD_S_ReEnterRoom, *PCMD_S_ReEnterRoom;

//�齫����
typedef struct tagCMD_S_MJ
{
	MJBlock							bHUMj[GAME_PLAYER][MAX_BLOCK_COUNT];			/*!< �������ƿ�(���ܵ���) */
	MJBYTE							bHUMjCount[GAME_PLAYER];						/*!< �����ƿ������ */
	MJBYTE							bHandMj[GAME_PLAYER][MAX_HAND_MJ];				/*!< ���е��� */
	MJBYTE							bHandMjCount[GAME_PLAYER];						/*!< ���е��Ƶ����� */
	MJBYTE							bOutMj[GAME_PLAYER][MAX_OUT_MJ];				/*!< �Ѿ������� */
	MJBYTE							bOutMjCount[GAME_PLAYER];						/*!< �Ѿ������Ƶ����� */

	int size()
	{
		return sizeof(tagCMD_S_MJ);
	}
}CMD_S_MJ, *PCMD_S_MJ;

//! ����
/*! ������Ϣ */
typedef struct tagCMD_S_Chat
{
	int32							iChatUser;										/*!< ����������Ϣ��� */
	int32							iFaceID;										/*!< ����ID */
	int32							iTalkID;											/*!< ����ID */
	enChatType						bType;											/*!< ��Ϣ���ͣ����顢���֣� */

	int size()
	{
		return sizeof(tagCMD_S_Chat);
	}
}CMD_S_Chat,*PCMD_S_Chat;

/////////////////////////////�ͻ�����Ϣ/////////////////////////////////////////////
//! ����
/*! �ܳ��Ʋ�������ҳ��� */
typedef struct tagCMD_C_OutMJ
{
	MJBYTE							bOutMJ;									/*!< ������� */
	int size()
	{
		return sizeof(tagCMD_C_OutMJ);
	}
}CMD_C_OutMJ,*PCMD_C_OutMJ;

//! ����
/*! �ܵ��Ƽ����� */
typedef struct tagCMD_C_GangMJ
{
	MJBYTE							bGangMJ;									/*!< ������� */
	enOperationGangType				bGangType;
	int size()
	{
		return sizeof(tagCMD_C_GangMJ);
	}
}CMD_C_GangMJ,*PCMD_C_GangMJ;

//! ����
/*! ������Ϣ */
typedef struct tagCMD_C_Chat
{
	int32							iFaceID;										/*!< ����ID */
	int32							iTalkID;											/*!< ����ID */
	enChatType						bType;											/*!< ��Ϣ���ͣ����顢���֣� */

	int size()
	{
		return sizeof(tagCMD_C_Chat);
	}
}CMD_C_Chat,*PCMD_C_Chat;

//////////////////////////////////////////////////////////////////////////
class CmdHelper
{
public:
	//////���������
	///////////////////////////// CMD_S_DealMJ /////////////////////////////////////////////
	static bool CodeMessage(tagCMD_S_DealMJ &info, Json::Value &msg);

	static bool DeCodeMessage(tagCMD_S_DealMJ &info, Json::Value &msg);

	////////////////////////////// CMD_S_OutMJ ////////////////////////////////////////////
	static bool CodeMessage(CMD_S_OutMJ &info, Json::Value &msg);

	static bool DeCodeMessage(CMD_S_OutMJ &info, Json::Value &msg);

	/////////////////////////////// CMD_S_TouchMJ ///////////////////////////////////////////
	static bool CodeMessage(CMD_S_TouchMJ &info, Json::Value &msg);

	static bool DeCodeMessage(CMD_S_TouchMJ &info, Json::Value &msg);

	///////////////////////////////// CMD_S_PengMJ /////////////////////////////////////////
	static bool CodeMessage(CMD_S_PengMJ &info, Json::Value &msg);

	static bool DeCodeMessage(CMD_S_PengMJ &info, Json::Value &msg);

	///////////////////////////////// CMD_S_GangMJ /////////////////////////////////////////
	static bool CodeMessage(CMD_S_GangMJ &info, Json::Value &msg);

	static bool DeCodeMessage(CMD_S_GangMJ &info, Json::Value &msg);

	/////////////////////////////////// CMD_S_GiveUp ///////////////////////////////////////
	static bool CodeMessage(CMD_S_GiveUp &info, Json::Value &msg);

	static bool DeCodeMessage(CMD_S_GiveUp &info, Json::Value &msg);

	/////////////////////////////////// CMD_S_TRUSTEE ///////////////////////////////////////
	static bool CodeMessage(CMD_S_Trustee &info, Json::Value &msg);

	static bool DeCodeMessage(CMD_S_Trustee &info, Json::Value &msg);

	/////////////////////////////////// CMD_S_HU ///////////////////////////////////////
	static bool CodeMessage(CMD_S_HU &info, Json::Value &msg);

	static bool DeCodeMessage(CMD_S_HU &info, Json::Value &msg);

	/////////////////////////////////// CMD_S_GameOver ///////////////////////////////////////
	static bool CodeMessage(CMD_S_GameOver &info, Json::Value &msg);

	static bool DeCodeMessage(CMD_S_GameOver &info, Json::Value &msg);

	/////////////////////////////////// CMD_S_ReEnterRoom ///////////////////////////////////////
	static bool CodeMessage(CMD_S_ReEnterRoom &info, Json::Value &msg);

	static bool DeCodeMessage(CMD_S_ReEnterRoom &info, Json::Value &msg);

	/////////////////////////////////// CMD_S_MJ ///////////////////////////////////////
	static bool CodeMessage(CMD_S_MJ &info, Json::Value &msg);

	static bool DeCodeMessage(CMD_S_MJ &info, Json::Value &msg);

	/////////////////////////////////// CMD_S_Chat ///////////////////////////////////////
	static bool CodeMessage(CMD_S_Chat &info, Json::Value &msg);

	static bool DeCodeMessage(CMD_S_Chat &info, Json::Value &msg);

	//////�ͻ�������
	//////////////////////////////// CMD_C_OutMJ //////////////////////////////////////////
	static bool CodeMessage(CMD_C_OutMJ &info, Json::Value &msg);

	static bool DeCodeMessage(CMD_C_OutMJ &info, Json::Value &msg);

	//////////////////////////////// CMD_C_GangMJ //////////////////////////////////////////
	static bool CodeMessage(CMD_C_GangMJ &info, Json::Value &msg);

	static bool DeCodeMessage(CMD_C_GangMJ &info, Json::Value &msg);

	//////////////////////////////// CMD_C_Chat //////////////////////////////////////////
	static bool CodeMessage(CMD_C_Chat &info, Json::Value &msg);

	static bool DeCodeMessage(CMD_C_Chat &info, Json::Value &msg);

};


#endif


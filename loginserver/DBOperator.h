#ifndef _DB_OPERATOR_H_INCLUDE_
#define _DB_OPERATOR_H_INCLUDE_

#include "../include/molnet/Network.h"
using namespace network;

#include <string>

/**
 * ����û���Ϣ
 */
struct MemberDataStru
{
	uint32 uid;                   //���ID
	int gtype;                    //�������
	char username[256];           //�������
	char password[256];           //�������
	char bankpassword[256];       //�����������
	char email[256];              //���email
	int sex;                      //����Ա�
	char realname[256];           //��ʵ����
	char homeplace[256];          //��ҳ
	char telephone[256];          //�绰
	char QQ[256];                 //QQ
	char ipaddress[256];          //IP��ַ
	uint32 createtime;            //����ʱ��
	uint32 lastlogintime;         //����¼ʱ��
	char useravatar[256];         //�û�ͷ��
	int state;                      //�Ƿ񱻷��

	int64 money;                  //���ϵ�Ǯ
	int64 bankmoney;              //���е�Ǯ
	uint32 level;                 //�ȼ�
	uint32 experience;            //����

	int glockmachine;             //�Ƿ�����
	char machinecode[256];        //������
};

/**
 * ��Ϸ����ṹ
 */
struct tagGameRoom
{
    int gameid;
    int serverport;
    char serverip[256];
    char processname[256];
    char servername[256];
    int tablecount;
    int tableplayercount;
    int gamingtype;
    int64 lastmoney;
    int64 pielement;
    uint32 roomrevenue;
    int state;                     // 0-������
};

/**
 * �����������Ϸ�����е����ݿ����
 */
class DBOperator : public Singleton<DBOperator>
{
public:
	/// ���캯��
	DBOperator();
	/// ��������
	~DBOperator();

	/// ��ʼ���ݿ�
	bool Initilize(std::string host,std::string user,std::string pass,std::string db,int port);
	/// �ر����ݿ�����
	void Shutdown(void);

	/// ����������ƺ��������������Ƿ����
	unsigned int IsExistUser(std::string name,std::string password);
	/// ����������Ƽ���������Ƿ����
	unsigned int IsExistUser(std::string name);
    /// �õ���ǰע���������
    unsigned int GetTotalUserCount(void);
    /// ����ָ����ҵĻ���
    bool LockMachineByUser(uint32 UserId,int operType);
	/// �������ID�����������
	bool UpdateUserPassword(uint32 UserId,std::string pwd);
	/// �����û�ID�õ��û�����Ϸ����
	bool GetUserData(uint32 UserId,MemberDataStru &UserData);
	/// �����û��˺ŵõ��û�ID
	bool GetUserID(std::string username,uint32 *userId);
	/// ����ָ����ҵ���������
	bool UpdateUserBankPassword(uint32 UserID,std::string decPWD);
	/// ����ָ����ҵĵ�½����
	bool UpdateUserLoginPassword(uint32 UserId,std::string decPWD);
	/// �����û������¼IP�������¼ʱ��
	bool UpdatePlayerLastLogin(uint32 UserID,std::string ipaddress,std::string machinecode);
	/// ����ָ����ҵ���Ϣ
	bool UpdateUserInfo(uint32 UserID,std::string nickname,std::string email,std::string telephone,std::string qq,std::string useravatar,int sex);
	/// ע���û�
	int32 RegisterGameUser(std::string name,std::string password,std::string email,
		int sex,std::string realname,std::string telephone,std::string AvatorIndex,std::string Referrer,std::string ipaddress,std::string cardnumber);
    /// �õ���ǰ���ߵ����з�����
    bool GetOnlineGameRooms(std::vector<tagGameRoom> &pgamerooms);
    /// ���֮��תǮ
    int32 TransferAccounts(uint32 UserID,std::string receiverUser,int64 money);
    /// �õ���ҵĽ��
    bool GetUserMoney(uint32 UserId,int64 *money,int64 *bankmoney);

	/// ����ά����ǰ���ݿ�����
	void Update(void);

private:
	DataProvider *m_DataProvider;                               /**< ���ڷ��ʱ������ݿ� */
};

#define ServerDBOperator DBOperator::getSingleton()

#endif

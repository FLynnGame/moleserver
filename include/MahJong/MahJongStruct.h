#ifndef MAHJONG_STRUCT_HEAD_FILE
#define MAHJONG_STRUCT_HEAD_FILE

#include <time.h>
#include "../molnet/common.h"

typedef int16 MJBYTE;										/*!< �����齫����.������Ҫ8λ,������16λ */
typedef int16 MJFLAG;										/*!< �������ʹ�õı��.������Ҫ16λ */

////////////////////////////////������//////////////////////////////////////////

#define MASK_MAHJONGKIND(value)				((value)>>4)			/*!< ��4λ������ */
#define MASK_MAHJONGNUM(value)				((value)&0x0F)			/*!< ��4λ������ */
#define MASK_MAHJONG(kind,num)				(((kind)<<4)+num)		/*!< ���ͺ�������ϳ��齫 */
//#define MASK_MAHJONGFENGORZI(value)			((value<0x35)?MahJongColor_Feng:MahJongColor_Word)			/*!< 0x31-0x34���� 0x35-0x39���� */

////////////////////////////////��������//////////////////////////////////////////
#define MAX_ARRAY_MJ				38								/*!< �������ʱ��Ÿ����齫����������0��10��λ�ò��š�38Ϊ������  */
#define RANDOM_TIMES				10								/*!< �����齫�Ĵ��� */

#define	MAX_HAND_MJ					14								/*!< �û������������ */
#define MAX_BLOCK_COUNT				((MAX_HAND_MJ + 2) / 3)			/*!< ����齫����� */
#define CONST_MJ_HIDE				0								/*!< �ƶ�����������ʱ����ֵ */

////////////////////////////////�齫�ṹ//////////////////////////////////////////
//! �齫����
/*! �齫�������,���ݺ���ʱ����� */
enum enBlockStyle
{
	BlockStyle_Null	 = 0,											/*!< �� */
	//////////////////////////////////�����еĽṹ////////////////////////////////////////
	BlockStyle_Chi,													/*!< 123���ƽṹ */
	BlockStyle_Peng,	 											/*!< 111���ƽṹ */
	BlockStyle_Gang_Ming,											/*!< 1111���ܽṹ */
	BlockStyle_Gang_ZiMoMing,										/*!< 1111�������ܽṹ */
	BlockStyle_Gang_An,												/*!< 1111���ܽṹ */
	BlockStyle_Gang_XF,												/*!< �������� �з��� ����ܽṹ */
	BlockStyle_FengZiChi,											/*!< �������� �з��� �� */
	//////////////////////////////////�����еĽṹ////////////////////////////////////////
	BlockStyle_Shun,	 											/*!< ˳��ṹ */
	BlockStyle_Ke,													/*!< ���ӽṹ,3����ͬ,���� */
	BlockStyle_Jiang,												/*!< ���ƽṹ */
};

//! �齫��ṹ
/*! �齫��,�ɼ����齫��� */
typedef struct tagMJBlock
{
	MJBYTE					first;									/*!< �齫���һ����ֵ */
	enBlockStyle			style;									/*!< �齫������ */

	/////////////////////////////ֻ������ĳ���(�ϲ��齫)/////////////////////////////////////////////
	MJBYTE					second;									/*!< �齫��ڶ�����ֵ */
	MJBYTE					last;									/*!< �齫���������ֵ */
	//! ��ÿ�������齫
	/*!
		��ø���3���Ƶ�����
		\param mj ��������齫����
		\param iCount ��������齫����
		\return �Ƿ���ȷ��ȡ����
		\sa enBlockStyle
	*/
	bool getMJ(MJBYTE mj[], int &iCount)
	{
		iCount = 0;
		switch(style)
		{
		case BlockStyle_Chi:
		case BlockStyle_Shun:
			{
				mj[0] = first+0;
				mj[1] = first+1;
				mj[2] = first+2;
				iCount = 3;
			}
			break;
		case BlockStyle_Peng:
		case BlockStyle_Ke:
			{
				mj[0] = mj[1] = mj[2] = first;
				iCount = 3;
			}
			break;
		case BlockStyle_Gang_Ming:
		case BlockStyle_Gang_ZiMoMing:
			{
				mj[0] = mj[1] = mj[2] = mj[3] = first;
				iCount = 4;
			}
			break;
		case BlockStyle_Gang_An:
			{
				mj[0] =  mj[1] =  mj[2] = CONST_MJ_HIDE;
				mj[3] = first;
				iCount = 4;
			}
			break;
		case BlockStyle_Gang_XF:
			{
				//���������ƿ�
				if(first == 0x31)
				{
					mj[0] = first+0;
					mj[1] = first+1;
					mj[2] = first+2;
					mj[3] = first+3;
					iCount = 4;
				}
				//�з����ƿ�
				else if (first == 0x35)
				{

					mj[0] = first+0;
					mj[1] = first+1;
					mj[2] = first+2;
					iCount = 3;
				}
			}
			break;
		case BlockStyle_FengZiChi:
			{
				mj[0] = first;
				mj[1] = second;
				mj[2] = last;
				iCount = 3;
			}
			break;
		case BlockStyle_Jiang:
			{
				mj[0] = mj[1] = first;
				iCount = 2;
			}
			break;
		default:
			return false;
		}
		return true;
	}
}MJBlock,*PMJBlock;

//! ����������͵Ľṹ
/*! ���ڴ洢������� */
typedef struct tagHuNode
{
	MJBlock					mjBlock;								/*!< ��ǰ�齫�� */
	struct tagHuNode		*tKe;									/*!< ��һ������ */
	struct tagHuNode		*tShun;									/*!< ��һ��˳�� */
	struct tagHuNode		*tJiang;								/*!< ��һ���� */
}HuNode, *HuTree;

/////////////////////////////�齫��ض���/////////////////////////////////////////////

//! �齫����
/*! �齫����: ��(1-9) ��(1-9) ��(1-9),��,����������ƻ�������������� */
enum enMahJongKind
{
	MahJong_Wan = 0,												/*!< ���� 1~9�� */
	MahJong_Tiao,													/*!< ���� 1~9�� */
	MahJong_Bing,													/*!< Ͳ�� 1~9Ͳ */
	MahJong_Word,													/*!< ���� ���������з��� */
	MahJong_Flower,													/*!< ���� �����ﶬ÷����� */
	MahJong_Invalid = 99											/*!< �Ƿ�ֵ */
};

//! �Բ���������
enum enOperationChiType
{
	Operation_Chi_Type_Null			= 0x00,							/*!< �Ƿ�ֵ */
	Operation_Chi_Type_Left			= 0x01,							/*!< �ԣ�0** */
	Operation_Chi_Type_Midd			= 0x02,							/*!< �ԣ�*0* */
	Operation_Chi_Type_Right		= 0x04							/*!< �ԣ�**0 */
};

//! �ܲ���������
/*! �ܲ��������� */
enum enOperationGangType
{
	Operation_Gang_Type_Null		= 0x00,							/*!< �� */
	Operation_Gang_Type_ZiMoMing	= 0x01,							/*!< ���¸�(��������) */
	Operation_Gang_Type_Ming		= 0x02,							/*!< ֱ�� */
	Operation_Gang_Type_An			= 0x04,							/*!< ���� */
	Operation_Gang_Type_XF			= 0x08,							/*!< ����� */
};
//////////////////////////////////////////////////////////////////////////
#endif

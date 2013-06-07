#pragma once

// ���������-������ ������
#define OSANVE_COUNT 9 // ����� ���������
#define OSANVE_NORM 0 // ��� ���� (�� ���������)
#define OSANVE_CREA 1 // �����
#define OSANVE_GOOD 2 // �������
#define OSANVE_HELP 3 // ��� ������
#define OSANVE_NOTH 4 // ������
#define OSANVE_FEAR 5 // ��������
#define OSANVE_BAD  6 // �������
#define OSANVE_WOUN 7 // �����
#define OSANVE_FGHT 8 // ���� ��� (����� ��� ��� ����)

// ����������� �������� � ������, �
#define OSANVE_MAX_WAIT_TIME 120
// ������ �������� ������ ���������, ��
#define OSANVE_SEND_TIME 10*1000
// ����� ����������� ������, ��
#define PREFIGHT_TIME 10*1000
// ����������� ������� � ������ ������ ��� ����������, ��
#define FIGHT_MAX_WAIT_TIME 30*1000
// ������ ������ ������ ���������, ��
#define BATTERYSTATUS_POLL_TIME 10*1000

// ��������� ��������
#define AL_STATUS_OSANVE  0	//������, �������� �����
#define AL_STATUS_OSSEL   1 // ��������� ������ ������
#define AL_STATUS_COMBAT  2 // ������ �����
#define AL_STATUS_FIGHT   3 // ����� �����
#define AL_STATUS_DEFENSE 4 // ����� ������
#define AL_STATUS_CONSEL  5 // ����� �����������
#define AL_STATUS_DEFEAT  6 // ����������� �����������
#define AL_STATUS_PREPARE 7 // ����������

// ���������-������ ������
extern char* OSANVES[OSANVE_COUNT];

// ������ ���������
#define FORCE_MAX 255
#define FORCE_STEP 5
#define FORCE_SIDE_GOOD 0x00
#define FORCE_SIDE_EVIL 0x10
#define FORCE_HEAL 0x08
#define MAX_ATACK_SEQ 7

// �����������
#define CONS_COUNT  6
#define CONS_TO1HIT 0
#define CONS_MIRAGE 1
#define CONS_CRAZY  2
#define CONS_SAD    3
#define CONS_PAIN   4
#define CONS_BERSERK 5

// �������� �����������
extern char* CONS[CONS_COUNT];


// ����������-����� (�����)
#define AL_CAN_FIGHT  1 // ���������
#define AL_CAN_DEFSEL 2 // �������� �����������
#define AL_CAN_MASTER 4 // ���������

namespace osanve {
	struct player {
//		uword_t _armletId; // ID ��������
		ubyte_t userId; // �����
		ubyte_t groupId; // ������ �������
		ubyte_t enemyId; // ���������
		ubyte_t atackSeq; // ����� ��������� �����
		ubyte_t osanve; // ���������-������ ������
		ubyte_t cons;   // ��������� �����������
		ubyte_t status; // ��������� ��������
		ubyte_t force; // ����
		ubyte_t maxForce; // ���� ����
///		ubyte_t defPress; // ���� ������ ������
        ubyte_t penaltyCons;   // ���������� �����������
		uword_t penalty; // ���� ���������� ����� �� ������ (�� ����� ����� � ������ �����)
		uword_t defeatTime; // ���������� ����� ���������� � �����������
		bool evil; // ������� ����
		ubyte_t fph; // �������� �������������� ���� (force per hour)
		char name[16]; // ��� ���������
		char fname[16]; //������ ��� ���������
		sword_t rights; // ����� �������� (AL_CAN)
	};

	void SetOsanve(ubyte_t no);
	void OnForceTick(void);
  void GetOsanvePacket(char* osanve_packet); // ������������ ������ ������
  void GetAtackPacket(char* a_packet, ubyte_t a); // ������������ ������ �����
  void GetHealPacket(char* h_packet, ubyte_t h); // ������������ ������ �������
	bool PreFightEnd(void);
	void SetFPH(ubyte_t newFPH);
	ubyte_t Atack(ubyte_t a);
	ubyte_t Atacked(ubyte_t a, ubyte_t cons);
	ubyte_t Healed(ubyte_t h);
  void DefenceON(void); // ������ ������ ������
  void DefenceOFF(void); // ������ ������ ������
	void PlayerInit(void);

	struct nick { // ��������� ������ ��������� ������� �� ������
		ubyte_t userId; // �����
		ubyte_t force;
		ubyte_t maxForce;
		ubyte_t osanve; // ���������-������ ������
		int time; // �����������, �� ������� ��� �����
	};
}

using namespace osanve;
// ��� ���� ������
extern struct player Player;
// ������
extern struct nick OSNV[MAX_ARMLET];
// �����
extern struct nick FGHT[MAX_ARMLET];
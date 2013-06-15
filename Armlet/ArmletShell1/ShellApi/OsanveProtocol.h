#pragma once

#define PACKET_LEN	4
#define PACKET_TYPE_ATACK	0x80
#define OSANVE_MASK 0x0F
#define CONS_MASK 0x70
#define FORCE_SIDE_GOOD 0x00
#define FORCE_SIDE_EVIL 0x10
#define FORCE_HEAL 0x08
#define GET_CONS(b) ((b&CONS_MASK)>>4)
#define IS_ATACK_PACKET(p)	(*(p+1)&PACKET_TYPE_ATACK)
#define IS_ATACKHEAL_PACKET(p)	((*(p+1)&PACKET_TYPE_ATACK)&&(*(p+1)&FORCE_HEAL))

namespace OsanveProtocol {

#pragma pack(push,1)

// Пакет осанве (широковещательный)
struct OSANVE_PACKET {
	unsigned char src; // ID отправителя
	unsigned char flags; // собственно осанве и флаг типа пакета
	unsigned char force; // сила отправителя
	unsigned char maxForce; // предел силы отправителя
};

// Пакет атаки (адресный)
struct ATACK_PACKET {
	unsigned char src; // ID отправителя
	unsigned char flags; // флаг типа пакета, флаг лечения и код последствия
	unsigned char dst; // ID получателя
	unsigned char atack; // сила, вложенная в атаку (может быть 0)
};

#pragma pack(pop)

} //namespace

using namespace OsanveProtocol;
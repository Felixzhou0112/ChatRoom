#include "protocol.h"


//产生 PDU 的函数
PDU *makePDU(uint messageLength)
{
    uint PDULength = sizeof(PDU)+messageLength;
    PDU *pdu = (PDU *)malloc(PDULength);
    if(NULL == pdu)
    {
        exit(EXIT_FAILURE);
    }
    memset(pdu,0,PDULength);
    pdu->PDULength = PDULength;
    pdu->messageLength = messageLength;
    return pdu;
}

#include "protocol.h"


//���� PDU �ĺ���
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

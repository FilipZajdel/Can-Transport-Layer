#define TEST_NO_MAIN

#include <fff.h>
#include <acutest.h>
#include <Test_Stub.h>
#include <CanTp.c>

FAKE_VALUE_FUNC(Std_ReturnType, CanIf_Transmit, PduIdType, const PduInfoType *);
FAKE_VALUE_FUNC(BufReq_ReturnType, PduR_CanTpCopyRxData, PduIdType, const PduInfoType *,
                PduLengthType *);
FAKE_VALUE_FUNC(BufReq_ReturnType, PduR_CanTpStartOfReception, PduIdType, const PduInfoType *,
                PduLengthType, PduLengthType *);
FAKE_VOID_FUNC(PduR_CanTpRxIndication, PduIdType, Std_ReturnType);

void CanTp_Transmit_Test_SF_transmission(void)
{
    uint8 sdu[] = {0xF, 0xA, 0xD, 0xE, 0xD};
    PduIdType pduId = 0x01;
    PduInfoType pduInfo = {
        .SduDataPtr = sdu,
        .SduLength = 5,
    };

    CanTp_Transmit(pduId, &pduInfo);
    TEST_CHECK(CanIf_Transmit_fake.call_count == 1);
    TEST_MSG("CanIf_Transmit was called %u/%u times", CanIf_Transmit_fake.call_count, 1);
}

TEST_LINKED_LIST_ENTRY CanTp_Transmit_TEST_LIST[] = {
    {"CanTp_Transmit SF transmission", CanTp_Transmit_Test_SF_transmission}, {NULL, NULL}};

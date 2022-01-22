#include <CanTp.h>
#include <fff.h>

FAKE_VALUE_FUNC(Std_ReturnType, CanIf_Transmit, PduIdType, const PduInfoType);

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

#define CanTp_Transmit_TEST_LIST                                                                   \
    {                                                                                              \
        "CanTp_Transmit SF transmission", CanTp_Transmit_Test_SF_transmission                      \
    }
// ,{ "next_test", next_test } // and so on

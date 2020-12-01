#include "demo/vl53l0x.h"
#include "platform/inc/board_config.h"
#include "sysctl.h"

int main(int argc, char const *argv[])
{
    sysctl_pll_set_freq(SYSCTL_PLL0, 800000000);
    uint64_t core = current_coreid();
    int data;
    printf("Core %ld Hello world\r\n", core);

    vl53l0x_test();
    return 0;
}


#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "cvmx.h"
#include "cvmx-key.h"
#include "cvmx-swap.h"
#include "cvmx-helper.h"
#include "cvmx-helper-board.h"

#include "cvmx-config.h"
#include "cvmx-llm.h"
#include "cvmx-fau.h"
#include "cvmx-sysinfo.h"
#include "cvmx-coremask.h"
#include "cvmx-mdio.h"
#include "cvmx-twsi.h"

#define CVMX_NSDELAY(nsec) { \
    uint64_t timeout_time = cvmx_get_cycle() + 1ull * nsec; \
    while (cvmx_get_cycle() < timeout_time); \
}

int main()
{
  cvmx_sysinfo_t *sysinfo;
  unsigned int core_mask;
  cvmx_helper_link_info_t link_info;

  sysinfo = cvmx_sysinfo_get();
  core_mask = sysinfo->core_mask;

  if (cvmx_coremask_first_core(core_mask)) {
    link_info.s.link_up = 1;
    link_info.s.full_duplex = 1;
    link_info.s.speed = 100;

    cvmx_helper_board_link_set_phy(2,0, link_info);
    cvmx_helper_board_link_set_phy(3,0, link_info);
  }

  return 0;
}


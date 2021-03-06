/*
 * Copyright (C) 2016 Socionext Inc.
 *   Author: Masahiro Yamada <yamada.masahiro@socionext.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <spl.h>

#include "../init.h"
#include "../micro-support-card.h"

int uniphier_ld20_init(const struct uniphier_board_data *bd)
{
	uniphier_sbc_init_savepin(bd);
	uniphier_pxs2_sbc_init(bd);
	/* pins for NAND and System Bus are multiplexed */
	if (spl_boot_device() != BOOT_DEVICE_NAND)
		uniphier_pin_init("system_bus_grp");

	support_card_reset();

	support_card_init();

	led_puts("L0");

	memconf_init(bd);
	uniphier_pxs2_memconf_init(bd);

	led_puts("L1");

	uniphier_ld20_early_clk_init(bd);

	led_puts("L2");

#ifdef CONFIG_SPL_SERIAL_SUPPORT
	preloader_console_init();
#endif

	led_puts("L3");

	uniphier_ld20_dpll_init(bd);

	led_puts("L4");

	{
		int res;

		res = uniphier_ld20_umc_init(bd);
		if (res < 0) {
			while (1)
				;
		}
	}

	led_puts("L5");

	dcache_disable();

	return 0;
}

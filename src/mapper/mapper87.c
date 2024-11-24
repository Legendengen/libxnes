/*
 * Copyright(c) Jianjun Jiang <8192542@qq.com>
 * Mobile phone: +86-18665388956
 * QQ: 8192542
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <xnes.h>

static uint8_t xnes_mapper87_cpu_read(struct xnes_ctx_t * ctx, uint16_t addr)
{
	struct xnes_cartridge_t * c = ctx->cartridge;

	switch(addr >> 13)
	{
	case 0:	/* [0x0000, 0x1FFF] */
		break;
	case 1:	/* [0x2000, 0x3FFF] */
		break;
	case 2:	/* [0x4000, 0x5FFF] */
		break;
	case 3:	/* [0x6000, 0x7FFF] */
		break;
	case 4:	/* [0x8000, 0x9FFF] */
	case 5:	/* [0xA000, 0xBFFF] */
	case 6:	/* [0xC000, 0xDFFF] */
	case 7:	/* [0xE000, 0xFFFF] */
		return c->prg_rom[(addr - 0x8000) & (c->prg_rom_size - 1)];
	default:
		break;
	}
	return 0;
}

static void xnes_mapper87_cpu_write(struct xnes_ctx_t * ctx, uint16_t addr, uint8_t val)
{
	struct xnes_cartridge_t * c = ctx->cartridge;

	switch(addr >> 13)
	{
	case 0:	/* [0x0000, 0x1FFF] */
		break;
	case 1:	/* [0x2000, 0x3FFF] */
		break;
	case 2:	/* [0x4000, 0x5FFF] */
		break;
	case 3:	/* [0x6000, 0x7FFF] */
		c->mapper.m.m87.chr_bank = ((val & 0x1) << 1) | ((val & 0x2) >> 1);
		break;
	case 4:	/* [0x8000, 0x9FFF] */
		break;
	case 5:	/* [0xA000, 0xBFFF] */
		break;
	case 6:	/* [0xC000, 0xDFFF] */
		break;
	case 7:	/* [0xE000, 0xFFFF] */
		break;
	default:
		break;
	}
}

static uint8_t xnes_mapper87_ppu_read(struct xnes_ctx_t * ctx, uint16_t addr)
{
	struct xnes_cartridge_t * c = ctx->cartridge;

	switch(addr >> 13)
	{
	case 0:	/* [0x0000, 0x1FFF] */
		return c->chr_rom[(c->mapper.m.m87.chr_bank << 13) + addr];
	case 1:	/* [0x2000, 0x3FFF] */
		break;
	case 2:	/* [0x4000, 0x5FFF] */
		break;
	case 3:	/* [0x6000, 0x7FFF] */
		break;
	case 4:	/* [0x8000, 0x9FFF] */
		break;
	case 5:	/* [0xA000, 0xBFFF] */
		break;
	case 6:	/* [0xC000, 0xDFFF] */
		break;
	case 7:	/* [0xE000, 0xFFFF] */
		break;
	default:
		break;
	}
	return 0;
}

static void xnes_mapper87_ppu_write(struct xnes_ctx_t * ctx, uint16_t addr, uint8_t val)
{
	struct xnes_cartridge_t * c = ctx->cartridge;

	switch(addr >> 13)
	{
	case 0:	/* [0x0000, 0x1FFF] */
		c->chr_rom[(c->mapper.m.m87.chr_bank << 13) + addr] = val;
		break;
	case 1:	/* [0x2000, 0x3FFF] */
		break;
	case 2:	/* [0x4000, 0x5FFF] */
		break;
	case 3:	/* [0x6000, 0x7FFF] */
		break;
	case 4:	/* [0x8000, 0x9FFF] */
		break;
	case 5:	/* [0xA000, 0xBFFF] */
		break;
	case 6:	/* [0xC000, 0xDFFF] */
		break;
	case 7:	/* [0xE000, 0xFFFF] */
		break;
	default:
		break;
	}
}

/*
 * CNROM
 */
void xnes_mapper87_init(struct xnes_cartridge_t * c)
{
	c->mapper.m.m87.chr_bank = 0;

	c->mapper.cpu_read = xnes_mapper87_cpu_read;
	c->mapper.cpu_write = xnes_mapper87_cpu_write;
	c->mapper.ppu_read = xnes_mapper87_ppu_read;
	c->mapper.ppu_write = xnes_mapper87_ppu_write;
	c->mapper.apu_step = NULL;
	c->mapper.ppu_step = NULL;
}
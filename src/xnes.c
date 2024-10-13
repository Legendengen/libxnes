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

struct xnes_ctx_t * xnes_ctx_alloc(const void * buf, size_t len)
{
	struct xnes_ctx_t * ctx;

	ctx = xnes_malloc(sizeof(struct xnes_ctx_t));
	if(!ctx)
		return NULL;

	xnes_memset(ctx, 0, sizeof(struct xnes_ctx_t));
	ctx->cartridge = xnes_cartridge_alloc(buf, len, ctx);
	if(!ctx->cartridge)
	{
		xnes_free(ctx);
		return NULL;
	}

	xnes_cpu_init(&ctx->cpu, ctx);
	xnes_dma_init(&ctx->dma, ctx);
	xnes_ppu_init(&ctx->ppu, ctx);
	xnes_apu_init(&ctx->apu, ctx);
	xnes_controller_init(&ctx->ctl, ctx);

	return ctx;
}

void xnes_ctx_free(struct xnes_ctx_t * ctx)
{
	if(ctx)
	{
		if(ctx->cartridge)
			xnes_cartridge_free(ctx->cartridge);
		xnes_free(ctx);
	}
}

void xnes_reset(struct xnes_ctx_t * ctx)
{
	if(ctx)
	{
		xnes_cpu_reset(&ctx->cpu);
		xnes_dma_reset(&ctx->dma);
		xnes_ppu_reset(&ctx->ppu);
		xnes_apu_reset(&ctx->apu);
		xnes_controller_reset(&ctx->ctl);
	}
}

void xnes_set_debugger(struct xnes_ctx_t * ctx, int (*debugger)(struct xnes_ctx_t *))
{
	if(ctx)
		ctx->cpu.debugger = debugger;
}

void xnes_set_audio(struct xnes_ctx_t * ctx, void * data, void (*cb)(void *, float), int rate)
{
	if(ctx)
		xnes_apu_set_audio_callback(&ctx->apu, data, cb, rate);
}

void xnes_set_speed(struct xnes_ctx_t * ctx, float speed)
{
	if(ctx && ctx->cartridge)
	{
		ctx->cartridge->cpu_rate_adjusted = ctx->cartridge->cpu_rate * speed;
		ctx->cartridge->cpu_period_adjusted = 1000000000 / ctx->cartridge->cpu_rate_adjusted;
	}
}

static inline int xnes_step(struct xnes_ctx_t * ctx)
{
	int cycles = xnes_cpu_step(&ctx->cpu);

	for(int i = 0; i < cycles; i++)
	{
		xnes_ppu_step(&ctx->ppu);
		xnes_ppu_step(&ctx->ppu);
		xnes_ppu_step(&ctx->ppu);
		xnes_apu_step(&ctx->apu);
	}
	return cycles;
}

uint64_t xnes_step_frame(struct xnes_ctx_t * ctx)
{
	int cycles = 0;
	int frame = ctx->ppu.frame;

	while(frame == ctx->ppu.frame)
	{
		cycles += xnes_step(ctx);
	}
	return (uint64_t)ctx->cartridge->cpu_period_adjusted * cycles;
}

static int xnes_state_length(struct xnes_ctx_t * ctx)
{
	int len = 0;

	len += sizeof(struct xnes_cpu_t);
	len += sizeof(struct xnes_dma_t);
	len += sizeof(struct xnes_ppu_t);
	len += sizeof(struct xnes_apu_t);
	len += sizeof(struct xnes_cartridge_t);
	len += ctx->cartridge->prg_ram_size;
	len += ctx->cartridge->prg_nvram_size;
	len += ctx->cartridge->chr_ram_size;
	len += ctx->cartridge->chr_nvram_size;

	return len;
}

static void xnes_state_save(struct xnes_ctx_t * ctx, void * buf)
{
	char * p = buf;
	int l;

	l = sizeof(struct xnes_cpu_t);
	xnes_memcpy(p, &ctx->cpu, l);
	p += l;

	l = sizeof(struct xnes_dma_t);
	xnes_memcpy(p, &ctx->dma, l);
	p += l;

	l = sizeof(struct xnes_ppu_t);
	xnes_memcpy(p, &ctx->ppu, l);
	p += l;

	l = sizeof(struct xnes_apu_t);
	xnes_memcpy(p, &ctx->apu, l);
	p += l;

	l = sizeof(struct xnes_cartridge_t);
	xnes_memcpy(p, ctx->cartridge, l);
	p += l;

	l = ctx->cartridge->prg_ram_size;
	if(l > 0)
	{
		xnes_memcpy(p, ctx->cartridge->prg_ram, l);
		p += l;
	}

	l = ctx->cartridge->prg_nvram_size;
	if(l > 0)
	{
		xnes_memcpy(p, ctx->cartridge->prg_nvram, l);
		p += l;
	}

	l = ctx->cartridge->chr_ram_size;
	if(l > 0)
	{
		xnes_memcpy(p, ctx->cartridge->chr_ram, l);
		p += l;
	}

	l = ctx->cartridge->chr_nvram_size;
	if(l > 0)
	{
		xnes_memcpy(p, ctx->cartridge->chr_nvram, l);
		p += l;
	}
}

static void xnes_state_restore(struct xnes_ctx_t * ctx, void * buf)
{
	char * p = buf;
	int l;

	xnes_controller_reset(&ctx->ctl);

	l = sizeof(struct xnes_cpu_t);
	xnes_memcpy(&ctx->cpu, p, l);
	p += l;

	l = sizeof(struct xnes_dma_t);
	xnes_memcpy(&ctx->dma, p, l);
	p += l;

	l = sizeof(struct xnes_ppu_t);
	xnes_memcpy(&ctx->ppu, p, l);
	p += l;

	l = sizeof(struct xnes_apu_t);
	xnes_memcpy(&ctx->apu, p, l);
	p += l;

	l = sizeof(struct xnes_cartridge_t);
	xnes_memcpy(ctx->cartridge, p, l);
	p += l;

	l = ctx->cartridge->prg_ram_size;
	if(l > 0)
	{
		xnes_memcpy(ctx->cartridge->prg_ram, p, l);
		p += l;
	}

	l = ctx->cartridge->prg_nvram_size;
	if(l > 0)
	{
		xnes_memcpy(ctx->cartridge->prg_nvram, p, l);
		p += l;
	}

	l = ctx->cartridge->chr_ram_size;
	if(l > 0)
	{
		xnes_memcpy(ctx->cartridge->chr_ram, p, l);
		p += l;
	}

	l = ctx->cartridge->chr_nvram_size;
	if(l > 0)
	{
		xnes_memcpy(ctx->cartridge->chr_nvram, p, l);
		p += l;
	}
}

struct xnes_state_t * xnes_state_alloc(struct xnes_ctx_t * ctx, int count)
{
	if(ctx && (count > 0))
	{
		int length = xnes_state_length(ctx);
		if(length > 0)
		{
			struct xnes_state_t * state = xnes_malloc(sizeof(struct xnes_state_t) + length * count);
			if(state)
			{
				state->ctx = ctx;
				state->buffer = (unsigned char *)state + sizeof(struct xnes_state_t);
				state->length = length;
				state->count = count;
				state->in = 0;
				state->out = 0;
				return state;
			}
		}
	}
	return NULL;
}

void xnes_state_free(struct xnes_state_t * state)
{
	if(state)
		xnes_free(state);
}

void xnes_state_push(struct xnes_state_t * state)
{
	if(state)
	{
		if((state->in - state->out) == state->count)
			++state->out;
		xnes_state_save(state->ctx, state->buffer + state->length * (state->in % state->count));
		state->in++;
	}
}

void xnes_state_pop(struct xnes_state_t * state)
{
	if(state)
	{
		if((state->in - state->out) > 0)
		{
			if(state->in > 0)
			{
				state->in--;
			}
			else
			{
				state->in = (state->in + 0xffffffff - 1) % state->count;
				state->out = state->out % state->count;
			}
			xnes_state_restore(state->ctx, state->buffer + state->length * (state->in % state->count));
		}
	}
}

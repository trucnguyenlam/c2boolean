
#include <stdio.h>
#include <stdlib.h>
#define __cs_safe_malloc malloc
#define assume __CPROVER_assume

extern int __VERIFIER_nondet_int(void);
extern unsigned int __VERIFIER_nondet_uint(void);
unsigned int TRACK_ADDRESS[31];
int TRACK_VALUE[31];
_Bool TRACK_VALID[31];
unsigned int TRACK_POS[31];
unsigned int num_malloc;
void init_MEM(void)
{
		unsigned int TRACK_ADDRESS_tmp[13];
		TRACK_ADDRESS[0] = 0;
		TRACK_ADDRESS[1] = 1;
		TRACK_ADDRESS[2] = 2;
		TRACK_ADDRESS[3] = 3;
		TRACK_ADDRESS[4] = 4;
		TRACK_ADDRESS[5] = 5;
		TRACK_ADDRESS[6] = 6;
		TRACK_ADDRESS[7] = 7;
		TRACK_ADDRESS[8] = 8;
		TRACK_ADDRESS[9] = 9;
		TRACK_ADDRESS[10] = 10;
		TRACK_ADDRESS[11] = 11;
		TRACK_ADDRESS[12] = 12;
		TRACK_ADDRESS[13] = 13;
		TRACK_ADDRESS[14] = 14;
		TRACK_ADDRESS[15] = 15;
		TRACK_ADDRESS[16] = 16;
		num_malloc = 17;
		TRACK_ADDRESS[17] = TRACK_ADDRESS_tmp[0];
		__CPROVER_assume((TRACK_ADDRESS[17] <= 1000) & (TRACK_ADDRESS[17] > TRACK_ADDRESS[16]));
		TRACK_ADDRESS[18] = TRACK_ADDRESS_tmp[1];
		__CPROVER_assume((TRACK_ADDRESS[18] <= 1000) & (TRACK_ADDRESS[18] > TRACK_ADDRESS[17]));
		TRACK_ADDRESS[19] = TRACK_ADDRESS_tmp[2];
		__CPROVER_assume((TRACK_ADDRESS[19] <= 1000) & (TRACK_ADDRESS[19] > TRACK_ADDRESS[18]));
		TRACK_ADDRESS[20] = TRACK_ADDRESS_tmp[3];
		__CPROVER_assume((TRACK_ADDRESS[20] <= 1000) & (TRACK_ADDRESS[20] > TRACK_ADDRESS[19]));
		TRACK_ADDRESS[21] = TRACK_ADDRESS_tmp[4];
		__CPROVER_assume((TRACK_ADDRESS[21] <= 1000) & (TRACK_ADDRESS[21] > TRACK_ADDRESS[20]));
		TRACK_ADDRESS[22] = TRACK_ADDRESS_tmp[5];
		__CPROVER_assume((TRACK_ADDRESS[22] <= 1000) & (TRACK_ADDRESS[22] > TRACK_ADDRESS[21]));
		TRACK_ADDRESS[23] = TRACK_ADDRESS_tmp[6];
		__CPROVER_assume((TRACK_ADDRESS[23] <= 1000) & (TRACK_ADDRESS[23] > TRACK_ADDRESS[22]));
		TRACK_ADDRESS[24] = TRACK_ADDRESS_tmp[7];
		__CPROVER_assume((TRACK_ADDRESS[24] <= 1000) & (TRACK_ADDRESS[24] > TRACK_ADDRESS[23]));
		TRACK_ADDRESS[25] = TRACK_ADDRESS_tmp[8];
		__CPROVER_assume((TRACK_ADDRESS[25] <= 1000) & (TRACK_ADDRESS[25] > TRACK_ADDRESS[24]));
		TRACK_ADDRESS[26] = TRACK_ADDRESS_tmp[9];
		__CPROVER_assume((TRACK_ADDRESS[26] <= 1000) & (TRACK_ADDRESS[26] > TRACK_ADDRESS[25]));
		TRACK_ADDRESS[27] = TRACK_ADDRESS_tmp[10];
		__CPROVER_assume((TRACK_ADDRESS[27] <= 1000) & (TRACK_ADDRESS[27] > TRACK_ADDRESS[26]));
		TRACK_ADDRESS[28] = TRACK_ADDRESS_tmp[11];
		__CPROVER_assume((TRACK_ADDRESS[28] <= 1000) & (TRACK_ADDRESS[28] > TRACK_ADDRESS[27]));
		TRACK_ADDRESS[29] = TRACK_ADDRESS_tmp[12];
		__CPROVER_assume((TRACK_ADDRESS[29] <= 1000) & (TRACK_ADDRESS[29] > TRACK_ADDRESS[28]));
		TRACK_ADDRESS[30] = 1001;
}

unsigned int my_malloc(size_t size)
{
		unsigned int tmp;
		tmp = num_malloc;
		num_malloc += size + 1;
		return tmp + 1;
}

void my_free(unsigned int p)
{
}

int READ(int codevar)
{
		return TRACK_VALUE[codevar];
}

void WRITE(int codevar, int value)
{
		TRACK_VALUE[codevar] = value;
}

unsigned int pos;
int READ_NORMAL(unsigned int address)
{
		pos = __VERIFIER_nondet_uint();
		__CPROVER_assume((pos >= 0) & (pos < 30));
		__CPROVER_assume(TRACK_ADDRESS[pos] == address);
		return TRACK_VALUE[pos];
}

int READ_POS(unsigned int position)
{
		if (TRACK_VALID[position])
		{
				pos = TRACK_POS[position];
				return TRACK_VALUE[pos];
		}

		return READ_NORMAL(TRACK_VALUE[position]);
}

void WRITE_NORMAL(unsigned int address, int value)
{
		pos = __VERIFIER_nondet_uint();
		__CPROVER_assume((pos >= 0) && (pos < 30));
		__CPROVER_assume((address >= TRACK_ADDRESS[pos]) & (address < TRACK_ADDRESS[pos + 1]));
		if (TRACK_ADDRESS[pos] == address)
		{
				TRACK_VALUE[pos] = value;
		}
		else
		{
				__CPROVER_assume(0);
		}

}

void WRITE_POS(unsigned int position, int value)
{
		if (TRACK_VALID[position])
		{
				pos = TRACK_POS[position];
				TRACK_VALUE[pos] = value;
		}
		else
		{
				WRITE_NORMAL(TRACK_VALUE[position], value);
		}

}

void merge_single_node(unsigned int dst, unsigned int data)
{
		TRACK_VALUE[12] = dst;
		TRACK_VALUE[13] = data;
		TRACK_VALUE[14] = READ_NORMAL(TRACK_VALUE[13]);
		TRACK_VALID[14] = 1;
		TRACK_POS[14] = pos;
		WRITE_POS(pos, READ_POS(14));
		WRITE_POS(14, 0);
		WRITE_NORMAL(READ_NORMAL(TRACK_VALUE[12]), TRACK_VALUE[14]);
		WRITE_NORMAL(TRACK_VALUE[12], TRACK_VALUE[14]);
}

void merge_pair(unsigned int dst, unsigned int sub1, unsigned int sub2)
{
		TRACK_VALUE[9] = dst;
		TRACK_VALUE[10] = sub1;
		TRACK_VALUE[11] = sub2;
		if (!(TRACK_VALUE[10] || TRACK_VALUE[11]))
		{
				goto __exit_loop_1;
		}

		{
				if ((!TRACK_VALUE[11]) || (TRACK_VALUE[10] && (READ_NORMAL(TRACK_VALUE[10] + 1) < READ_NORMAL(TRACK_VALUE[11] + 1))))
				{
						merge_single_node(TRACK_ADDRESS[9], TRACK_ADDRESS[10]);
				}
				else
				{
						merge_single_node(TRACK_ADDRESS[9], TRACK_ADDRESS[11]);
				}

		}
		if (!(TRACK_VALUE[10] || TRACK_VALUE[11]))
		{
				goto __exit_loop_1;
		}

		{
				if ((!TRACK_VALUE[11]) || (TRACK_VALUE[10] && (READ_NORMAL(TRACK_VALUE[10] + 1) < READ_NORMAL(TRACK_VALUE[11] + 1))))
				{
						merge_single_node(TRACK_ADDRESS[9], TRACK_ADDRESS[10]);
				}
				else
				{
						merge_single_node(TRACK_ADDRESS[9], TRACK_ADDRESS[11]);
				}

		}
		if (!(TRACK_VALUE[10] || TRACK_VALUE[11]))
		{
				goto __exit_loop_1;
		}

		{
				if ((!TRACK_VALUE[11]) || (TRACK_VALUE[10] && (READ_NORMAL(TRACK_VALUE[10] + 1) < READ_NORMAL(TRACK_VALUE[11] + 1))))
				{
						merge_single_node(TRACK_ADDRESS[9], TRACK_ADDRESS[10]);
				}
				else
				{
						merge_single_node(TRACK_ADDRESS[9], TRACK_ADDRESS[11]);
				}

		}
		if (!(TRACK_VALUE[10] || TRACK_VALUE[11]))
		{
				goto __exit_loop_1;
		}

		{
				if ((!TRACK_VALUE[11]) || (TRACK_VALUE[10] && (READ_NORMAL(TRACK_VALUE[10] + 1) < READ_NORMAL(TRACK_VALUE[11] + 1))))
				{
						merge_single_node(TRACK_ADDRESS[9], TRACK_ADDRESS[10]);
				}
				else
				{
						merge_single_node(TRACK_ADDRESS[9], TRACK_ADDRESS[11]);
				}

		}
		if (!(TRACK_VALUE[10] || TRACK_VALUE[11]))
		{
				goto __exit_loop_1;
		}

		{
				if ((!TRACK_VALUE[11]) || (TRACK_VALUE[10] && (READ_NORMAL(TRACK_VALUE[10] + 1) < READ_NORMAL(TRACK_VALUE[11] + 1))))
				{
						merge_single_node(TRACK_ADDRESS[9], TRACK_ADDRESS[10]);
				}
				else
				{
						merge_single_node(TRACK_ADDRESS[9], TRACK_ADDRESS[11]);
				}

		}
		if (!(TRACK_VALUE[10] || TRACK_VALUE[11]))
		{
				goto __exit_loop_1;
		}

		{
				if ((!TRACK_VALUE[11]) || (TRACK_VALUE[10] && (READ_NORMAL(TRACK_VALUE[10] + 1) < READ_NORMAL(TRACK_VALUE[11] + 1))))
				{
						merge_single_node(TRACK_ADDRESS[9], TRACK_ADDRESS[10]);
				}
				else
				{
						merge_single_node(TRACK_ADDRESS[9], TRACK_ADDRESS[11]);
				}

		}
		if (!(TRACK_VALUE[10] || TRACK_VALUE[11]))
		{
				goto __exit_loop_1;
		}

		{
				if ((!TRACK_VALUE[11]) || (TRACK_VALUE[10] && (READ_NORMAL(TRACK_VALUE[10] + 1) < READ_NORMAL(TRACK_VALUE[11] + 1))))
				{
						merge_single_node(TRACK_ADDRESS[9], TRACK_ADDRESS[10]);
				}
				else
				{
						merge_single_node(TRACK_ADDRESS[9], TRACK_ADDRESS[11]);
				}

		}
		assume(!(TRACK_VALUE[10] || TRACK_VALUE[11]));
		__exit_loop_1:
		;

}

unsigned int seq_sort_core(unsigned int data)
{
		TRACK_VALUE[6] = data;
		TRACK_VALUE[7] = 0;
		if (!TRACK_VALUE[6])
		{
				goto __exit_loop_2;
		}

		{
				TRACK_VALUE[8] = READ_NORMAL(TRACK_VALUE[6] + 1);
				TRACK_VALID[8] = 1;
				TRACK_VALID[8] = pos;
				if (!TRACK_VALUE[8])
				{
						WRITE_NORMAL(TRACK_VALUE[6] + 1, TRACK_VALUE[7]);
						TRACK_VALID[pos] = 1;
						TRACK_POS[pos] = 7;
						TRACK_VALUE[7] = TRACK_VALUE[6];
						TRACK_VALID[7] = 1;
						TRACK_POS[7] = 6;
						goto __exit_loop_2;
				}

				merge_pair(TRACK_VALUE[6], READ_POS(6), READ_POS(8));
				WRITE_NORMAL(TRACK_VALUE[6] + 1, TRACK_VALUE[7]);
				TRACK_VALID[pos] = 1;
				TRACK_POS[pos] = 7;
				TRACK_VALUE[7] = TRACK_VALUE[6];
				TRACK_VALID[7] = 1;
				TRACK_POS[7] = 6;
				TRACK_VALUE[6] = READ_NORMAL(TRACK_VALUE[8] + 1);
				TRACK_VALID[6] = 1;
				TRACK_POS[6] = pos;
				my_free(TRACK_VALUE[8]);
		}
		if (!TRACK_VALUE[6])
		{
				goto __exit_loop_2;
		}

		{
				TRACK_VALUE[8] = READ_NORMAL(TRACK_VALUE[6] + 1);
				TRACK_VALID[8] = 1;
				TRACK_VALID[8] = pos;
				if (!TRACK_VALUE[8])
				{
						WRITE_NORMAL(TRACK_VALUE[6] + 1, TRACK_VALUE[7]);
						TRACK_VALID[pos] = 1;
						TRACK_POS[pos] = 7;
						TRACK_VALUE[7] = TRACK_VALUE[6];
						TRACK_VALID[7] = 1;
						TRACK_POS[7] = 6;
						goto __exit_loop_2;
				}

				merge_pair(TRACK_VALUE[6], READ_POS(6), READ_POS(8));
				WRITE_NORMAL(TRACK_VALUE[6] + 1, TRACK_VALUE[7]);
				TRACK_VALID[pos] = 1;
				TRACK_POS[pos] = 7;
				TRACK_VALUE[7] = TRACK_VALUE[6];
				TRACK_VALID[7] = 1;
				TRACK_POS[7] = 6;
				TRACK_VALUE[6] = READ_NORMAL(TRACK_VALUE[8] + 1);
				TRACK_VALID[6] = 1;
				TRACK_POS[6] = pos;
				my_free(TRACK_VALUE[8]);
		}
		if (!TRACK_VALUE[6])
		{
				goto __exit_loop_2;
		}

		{
				TRACK_VALUE[8] = READ_NORMAL(TRACK_VALUE[6] + 1);
				TRACK_VALID[8] = 1;
				TRACK_VALID[8] = pos;
				if (!TRACK_VALUE[8])
				{
						WRITE_NORMAL(TRACK_VALUE[6] + 1, TRACK_VALUE[7]);
						TRACK_VALID[pos] = 1;
						TRACK_POS[pos] = 7;
						TRACK_VALUE[7] = TRACK_VALUE[6];
						TRACK_VALID[7] = 1;
						TRACK_POS[7] = 6;
						goto __exit_loop_2;
				}

				merge_pair(TRACK_VALUE[6], READ_POS(6), READ_POS(8));
				WRITE_NORMAL(TRACK_VALUE[6] + 1, TRACK_VALUE[7]);
				TRACK_VALID[pos] = 1;
				TRACK_POS[pos] = 7;
				TRACK_VALUE[7] = TRACK_VALUE[6];
				TRACK_VALID[7] = 1;
				TRACK_POS[7] = 6;
				TRACK_VALUE[6] = READ_NORMAL(TRACK_VALUE[8] + 1);
				TRACK_VALID[6] = 1;
				TRACK_POS[6] = pos;
				my_free(TRACK_VALUE[8]);
		}
		if (!TRACK_VALUE[6])
		{
				goto __exit_loop_2;
		}

		{
				TRACK_VALUE[8] = READ_NORMAL(TRACK_VALUE[6] + 1);
				TRACK_VALID[8] = 1;
				TRACK_VALID[8] = pos;
				if (!TRACK_VALUE[8])
				{
						WRITE_NORMAL(TRACK_VALUE[6] + 1, TRACK_VALUE[7]);
						TRACK_VALID[pos] = 1;
						TRACK_POS[pos] = 7;
						TRACK_VALUE[7] = TRACK_VALUE[6];
						TRACK_VALID[7] = 1;
						TRACK_POS[7] = 6;
						goto __exit_loop_2;
				}

				merge_pair(TRACK_VALUE[6], READ_POS(6), READ_POS(8));
				WRITE_NORMAL(TRACK_VALUE[6] + 1, TRACK_VALUE[7]);
				TRACK_VALID[pos] = 1;
				TRACK_POS[pos] = 7;
				TRACK_VALUE[7] = TRACK_VALUE[6];
				TRACK_VALID[7] = 1;
				TRACK_POS[7] = 6;
				TRACK_VALUE[6] = READ_NORMAL(TRACK_VALUE[8] + 1);
				TRACK_VALID[6] = 1;
				TRACK_POS[6] = pos;
				my_free(TRACK_VALUE[8]);
		}
		if (!TRACK_VALUE[6])
		{
				goto __exit_loop_2;
		}

		{
				TRACK_VALUE[8] = READ_NORMAL(TRACK_VALUE[6] + 1);
				TRACK_VALID[8] = 1;
				TRACK_VALID[8] = pos;
				if (!TRACK_VALUE[8])
				{
						WRITE_NORMAL(TRACK_VALUE[6] + 1, TRACK_VALUE[7]);
						TRACK_VALID[pos] = 1;
						TRACK_POS[pos] = 7;
						TRACK_VALUE[7] = TRACK_VALUE[6];
						TRACK_VALID[7] = 1;
						TRACK_POS[7] = 6;
						goto __exit_loop_2;
				}

				merge_pair(TRACK_VALUE[6], READ_POS(6), READ_POS(8));
				WRITE_NORMAL(TRACK_VALUE[6] + 1, TRACK_VALUE[7]);
				TRACK_VALID[pos] = 1;
				TRACK_POS[pos] = 7;
				TRACK_VALUE[7] = TRACK_VALUE[6];
				TRACK_VALID[7] = 1;
				TRACK_POS[7] = 6;
				TRACK_VALUE[6] = READ_NORMAL(TRACK_VALUE[8] + 1);
				TRACK_VALID[6] = 1;
				TRACK_POS[6] = pos;
				my_free(TRACK_VALUE[8]);
		}
		if (!TRACK_VALUE[6])
		{
				goto __exit_loop_2;
		}

		{
				TRACK_VALUE[8] = READ_NORMAL(TRACK_VALUE[6] + 1);
				TRACK_VALID[8] = 1;
				TRACK_VALID[8] = pos;
				if (!TRACK_VALUE[8])
				{
						WRITE_NORMAL(TRACK_VALUE[6] + 1, TRACK_VALUE[7]);
						TRACK_VALID[pos] = 1;
						TRACK_POS[pos] = 7;
						TRACK_VALUE[7] = TRACK_VALUE[6];
						TRACK_VALID[7] = 1;
						TRACK_POS[7] = 6;
						goto __exit_loop_2;
				}

				merge_pair(TRACK_VALUE[6], READ_POS(6), READ_POS(8));
				WRITE_NORMAL(TRACK_VALUE[6] + 1, TRACK_VALUE[7]);
				TRACK_VALID[pos] = 1;
				TRACK_POS[pos] = 7;
				TRACK_VALUE[7] = TRACK_VALUE[6];
				TRACK_VALID[7] = 1;
				TRACK_POS[7] = 6;
				TRACK_VALUE[6] = READ_NORMAL(TRACK_VALUE[8] + 1);
				TRACK_VALID[6] = 1;
				TRACK_POS[6] = pos;
				my_free(TRACK_VALUE[8]);
		}
		if (!TRACK_VALUE[6])
		{
				goto __exit_loop_2;
		}

		{
				TRACK_VALUE[8] = READ_NORMAL(TRACK_VALUE[6] + 1);
				TRACK_VALID[8] = 1;
				TRACK_VALID[8] = pos;
				if (!TRACK_VALUE[8])
				{
						WRITE_NORMAL(TRACK_VALUE[6] + 1, TRACK_VALUE[7]);
						TRACK_VALID[pos] = 1;
						TRACK_POS[pos] = 7;
						TRACK_VALUE[7] = TRACK_VALUE[6];
						TRACK_VALID[7] = 1;
						TRACK_POS[7] = 6;
						goto __exit_loop_2;
				}

				merge_pair(TRACK_VALUE[6], READ_POS(6), READ_POS(8));
				WRITE_NORMAL(TRACK_VALUE[6] + 1, TRACK_VALUE[7]);
				TRACK_VALID[pos] = 1;
				TRACK_POS[pos] = 7;
				TRACK_VALUE[7] = TRACK_VALUE[6];
				TRACK_VALID[7] = 1;
				TRACK_POS[7] = 6;
				TRACK_VALUE[6] = READ_NORMAL(TRACK_VALUE[8] + 1);
				TRACK_VALID[6] = 1;
				TRACK_POS[6] = pos;
				my_free(TRACK_VALUE[8]);
		}
		assume(!TRACK_VALUE[6]);
		__exit_loop_2:
		;

		return TRACK_VALUE[7];
}

void inspect_before(unsigned int shape)
{
		TRACK_VALUE[5] = shape;
		assert(TRACK_VALUE[5]);
		if (!READ_NORMAL(TRACK_VALUE[5] + 1))
		{
				goto __exit_loop_3;
		}

		{
				assert(TRACK_VALUE[5]);
				assert(READ_NORMAL(TRACK_VALUE[5] + 1));
				assert(READ_POS(5));
				assert(READ_NORMAL(READ_POS(5)) == 0);
				TRACK_VALUE[5] = READ_NORMAL(TRACK_VALUE[5] + 1);
				TRACK_VALID[5] = 1;
				TRACK_POS[5] = pos;
		}
		if (!READ_NORMAL(TRACK_VALUE[5] + 1))
		{
				goto __exit_loop_3;
		}

		{
				assert(TRACK_VALUE[5]);
				assert(READ_NORMAL(TRACK_VALUE[5] + 1));
				assert(READ_POS(5));
				assert(READ_NORMAL(READ_POS(5)) == 0);
				TRACK_VALUE[5] = READ_NORMAL(TRACK_VALUE[5] + 1);
				TRACK_VALID[5] = 1;
				TRACK_POS[5] = pos;
		}
		if (!READ_NORMAL(TRACK_VALUE[5] + 1))
		{
				goto __exit_loop_3;
		}

		{
				assert(TRACK_VALUE[5]);
				assert(READ_NORMAL(TRACK_VALUE[5] + 1));
				assert(READ_POS(5));
				assert(READ_NORMAL(READ_POS(5)) == 0);
				TRACK_VALUE[5] = READ_NORMAL(TRACK_VALUE[5] + 1);
				TRACK_VALID[5] = 1;
				TRACK_POS[5] = pos;
		}
		if (!READ_NORMAL(TRACK_VALUE[5] + 1))
		{
				goto __exit_loop_3;
		}

		{
				assert(TRACK_VALUE[5]);
				assert(READ_NORMAL(TRACK_VALUE[5] + 1));
				assert(READ_POS(5));
				assert(READ_NORMAL(READ_POS(5)) == 0);
				TRACK_VALUE[5] = READ_NORMAL(TRACK_VALUE[5] + 1);
				TRACK_VALID[5] = 1;
				TRACK_POS[5] = pos;
		}
		if (!READ_NORMAL(TRACK_VALUE[5] + 1))
		{
				goto __exit_loop_3;
		}

		{
				assert(TRACK_VALUE[5]);
				assert(READ_NORMAL(TRACK_VALUE[5] + 1));
				assert(READ_POS(5));
				assert(READ_NORMAL(READ_POS(5)) == 0);
				TRACK_VALUE[5] = READ_NORMAL(TRACK_VALUE[5] + 1);
				TRACK_VALID[5] = 1;
				TRACK_POS[5] = pos;
		}
		if (!READ_NORMAL(TRACK_VALUE[5] + 1))
		{
				goto __exit_loop_3;
		}

		{
				assert(TRACK_VALUE[5]);
				assert(READ_NORMAL(TRACK_VALUE[5] + 1));
				assert(READ_POS(5));
				assert(READ_NORMAL(READ_POS(5)) == 0);
				TRACK_VALUE[5] = READ_NORMAL(TRACK_VALUE[5] + 1);
				TRACK_VALID[5] = 1;
				TRACK_POS[5] = pos;
		}
		if (!READ_NORMAL(TRACK_VALUE[5] + 1))
		{
				goto __exit_loop_3;
		}

		{
				assert(TRACK_VALUE[5]);
				assert(READ_NORMAL(TRACK_VALUE[5] + 1));
				assert(READ_POS(5));
				assert(READ_NORMAL(READ_POS(5)) == 0);
				TRACK_VALUE[5] = READ_NORMAL(TRACK_VALUE[5] + 1);
				TRACK_VALID[5] = 1;
				TRACK_POS[5] = pos;
		}
		assume(!READ_NORMAL(TRACK_VALUE[5] + 1));
		__exit_loop_3:
		;

		assert(TRACK_VALUE[5]);
		assert(READ_NORMAL(TRACK_VALUE[5] + 1) == 0);
		assert(READ_POS(5));
		assert(READ_NORMAL(READ_POS(5)) == 0);
}

void inspect_after(unsigned int shape)
{
		TRACK_VALUE[15] = shape;
		assert(TRACK_VALUE[15]);
		assert(READ_NORMAL(TRACK_VALUE[15] + 1) == 0);
		assert(READ_POS(15) != 0);
		TRACK_VALUE[16] = READ_NORMAL(TRACK_VALUE[15]);
		TRACK_VALID[16] = 1;
		TRACK_POS[16] = 1;
		if (!READ_POS(16))
		{
				goto __exit_loop_4;
		}

		{
				assert(!READ_NORMAL(TRACK_VALUE[16]));
				TRACK_VALUE[16] = READ_POS(16);
				TRACK_VALID[16] = 1;
				TRACK_POS[16] = pos;
		}
		if (!READ_POS(16))
		{
				goto __exit_loop_4;
		}

		{
				assert(!READ_NORMAL(TRACK_VALUE[16]));
				TRACK_VALUE[16] = READ_POS(16);
				TRACK_VALID[16] = 1;
				TRACK_POS[16] = pos;
		}
		if (!READ_POS(16))
		{
				goto __exit_loop_4;
		}

		{
				assert(!READ_NORMAL(TRACK_VALUE[16]));
				TRACK_VALUE[16] = READ_POS(16);
				TRACK_VALID[16] = 1;
				TRACK_POS[16] = pos;
		}
		if (!READ_POS(16))
		{
				goto __exit_loop_4;
		}

		{
				assert(!READ_NORMAL(TRACK_VALUE[16]));
				TRACK_VALUE[16] = READ_POS(16);
				TRACK_VALID[16] = 1;
				TRACK_POS[16] = pos;
		}
		if (!READ_POS(16))
		{
				goto __exit_loop_4;
		}

		{
				assert(!READ_NORMAL(TRACK_VALUE[16]));
				TRACK_VALUE[16] = READ_POS(16);
				TRACK_VALID[16] = 1;
				TRACK_POS[16] = pos;
		}
		if (!READ_POS(16))
		{
				goto __exit_loop_4;
		}

		{
				assert(!READ_NORMAL(TRACK_VALUE[16]));
				TRACK_VALUE[16] = READ_POS(16);
				TRACK_VALID[16] = 1;
				TRACK_POS[16] = pos;
		}
		if (!READ_POS(16))
		{
				goto __exit_loop_4;
		}

		{
				assert(!READ_NORMAL(TRACK_VALUE[16]));
				TRACK_VALUE[16] = READ_POS(16);
				TRACK_VALID[16] = 1;
				TRACK_POS[16] = pos;
		}
		assume(!READ_POS(16));
		__exit_loop_4:
		;

}

int main()
{
		init_MEM();
		TRACK_VALUE[0] = 0;
		TRACK_VALID[0] = 0;
		if (!__VERIFIER_nondet_int())
		{
				goto __exit_loop_5;
		}

		{
				TRACK_VALUE[1] = my_malloc(1 * 2);
				WRITE_POS(1, TRACK_VALUE[1]);
				TRACK_VALID[pos] = 1;
				TRACK_POS[pos] = 1;
				WRITE_NORMAL(TRACK_VALUE[1] + 1, __VERIFIER_nondet_int());
				TRACK_VALUE[2] = my_malloc(1 * 2);
				WRITE_POS(2, TRACK_VALUE[1]);
				TRACK_VALID[pos] = 1;
				TRACK_POS[pos] = 1;
				WRITE_NORMAL(TRACK_VALUE[2] + 1, TRACK_VALUE[0]);
				TRACK_VALID[pos] = 1;
				TRACK_POS[pos] = 0;
				TRACK_VALUE[0] = TRACK_VALUE[2];
				TRACK_VALID[0] = 1;
				TRACK_POS[0] = 2;
		}
		if (!__VERIFIER_nondet_int())
		{
				goto __exit_loop_5;
		}

		{
				TRACK_VALUE[1] = my_malloc(1 * 2);
				WRITE_POS(1, TRACK_VALUE[1]);
				TRACK_VALID[pos] = 1;
				TRACK_POS[pos] = 1;
				WRITE_NORMAL(TRACK_VALUE[1] + 1, __VERIFIER_nondet_int());
				TRACK_VALUE[2] = my_malloc(1 * 2);
				WRITE_POS(2, TRACK_VALUE[1]);
				TRACK_VALID[pos] = 1;
				TRACK_POS[pos] = 1;
				WRITE_NORMAL(TRACK_VALUE[2] + 1, TRACK_VALUE[0]);
				TRACK_VALID[pos] = 1;
				TRACK_POS[pos] = 0;
				TRACK_VALUE[0] = TRACK_VALUE[2];
				TRACK_VALID[0] = 1;
				TRACK_POS[0] = 2;
		}
		if (!__VERIFIER_nondet_int())
		{
				goto __exit_loop_5;
		}

		{
				TRACK_VALUE[1] = my_malloc(1 * 2);
				WRITE_POS(1, TRACK_VALUE[1]);
				TRACK_VALID[pos] = 1;
				TRACK_POS[pos] = 1;
				WRITE_NORMAL(TRACK_VALUE[1] + 1, __VERIFIER_nondet_int());
				TRACK_VALUE[2] = my_malloc(1 * 2);
				WRITE_POS(2, TRACK_VALUE[1]);
				TRACK_VALID[pos] = 1;
				TRACK_POS[pos] = 1;
				WRITE_NORMAL(TRACK_VALUE[2] + 1, TRACK_VALUE[0]);
				TRACK_VALID[pos] = 1;
				TRACK_POS[pos] = 0;
				TRACK_VALUE[0] = TRACK_VALUE[2];
				TRACK_VALID[0] = 1;
				TRACK_POS[0] = 2;
		}
		if (!__VERIFIER_nondet_int())
		{
				goto __exit_loop_5;
		}

		{
				TRACK_VALUE[1] = my_malloc(1 * 2);
				WRITE_POS(1, TRACK_VALUE[1]);
				TRACK_VALID[pos] = 1;
				TRACK_POS[pos] = 1;
				WRITE_NORMAL(TRACK_VALUE[1] + 1, __VERIFIER_nondet_int());
				TRACK_VALUE[2] = my_malloc(1 * 2);
				WRITE_POS(2, TRACK_VALUE[1]);
				TRACK_VALID[pos] = 1;
				TRACK_POS[pos] = 1;
				WRITE_NORMAL(TRACK_VALUE[2] + 1, TRACK_VALUE[0]);
				TRACK_VALID[pos] = 1;
				TRACK_POS[pos] = 0;
				TRACK_VALUE[0] = TRACK_VALUE[2];
				TRACK_VALID[0] = 1;
				TRACK_POS[0] = 2;
		}
		if (!__VERIFIER_nondet_int())
		{
				goto __exit_loop_5;
		}

		{
				TRACK_VALUE[1] = my_malloc(1 * 2);
				WRITE_POS(1, TRACK_VALUE[1]);
				TRACK_VALID[pos] = 1;
				TRACK_POS[pos] = 1;
				WRITE_NORMAL(TRACK_VALUE[1] + 1, __VERIFIER_nondet_int());
				TRACK_VALUE[2] = my_malloc(1 * 2);
				WRITE_POS(2, TRACK_VALUE[1]);
				TRACK_VALID[pos] = 1;
				TRACK_POS[pos] = 1;
				WRITE_NORMAL(TRACK_VALUE[2] + 1, TRACK_VALUE[0]);
				TRACK_VALID[pos] = 1;
				TRACK_POS[pos] = 0;
				TRACK_VALUE[0] = TRACK_VALUE[2];
				TRACK_VALID[0] = 1;
				TRACK_POS[0] = 2;
		}
		if (!__VERIFIER_nondet_int())
		{
				goto __exit_loop_5;
		}

		{
				TRACK_VALUE[1] = my_malloc(1 * 2);
				WRITE_POS(1, TRACK_VALUE[1]);
				TRACK_VALID[pos] = 1;
				TRACK_POS[pos] = 1;
				WRITE_NORMAL(TRACK_VALUE[1] + 1, __VERIFIER_nondet_int());
				TRACK_VALUE[2] = my_malloc(1 * 2);
				WRITE_POS(2, TRACK_VALUE[1]);
				TRACK_VALID[pos] = 1;
				TRACK_POS[pos] = 1;
				WRITE_NORMAL(TRACK_VALUE[2] + 1, TRACK_VALUE[0]);
				TRACK_VALID[pos] = 1;
				TRACK_POS[pos] = 0;
				TRACK_VALUE[0] = TRACK_VALUE[2];
				TRACK_VALID[0] = 1;
				TRACK_POS[0] = 2;
		}
		if (!__VERIFIER_nondet_int())
		{
				goto __exit_loop_5;
		}

		{
				TRACK_VALUE[1] = my_malloc(1 * 2);
				WRITE_POS(1, TRACK_VALUE[1]);
				TRACK_VALID[pos] = 1;
				TRACK_POS[pos] = 1;
				WRITE_NORMAL(TRACK_VALUE[1] + 1, __VERIFIER_nondet_int());
				TRACK_VALUE[2] = my_malloc(1 * 2);
				WRITE_POS(2, TRACK_VALUE[1]);
				TRACK_VALID[pos] = 1;
				TRACK_POS[pos] = 1;
				WRITE_NORMAL(TRACK_VALUE[2] + 1, TRACK_VALUE[0]);
				TRACK_VALID[pos] = 1;
				TRACK_POS[pos] = 0;
				TRACK_VALUE[0] = TRACK_VALUE[2];
				TRACK_VALID[0] = 1;
				TRACK_POS[0] = 2;
		}
		assume(!__VERIFIER_nondet_int());
		__exit_loop_5:
		;

		inspect_before(TRACK_VALUE[0]);
		if (!READ_NORMAL(TRACK_VALUE[0] + 1))
		{
				goto __exit_loop_6;
		}

		{
				TRACK_VALUE[0] = seq_sort_core(TRACK_VALUE[0]);
				TRACK_VALID[0] = 1;
				TRACK_POS[0] = 7;
		}
		if (!READ_NORMAL(TRACK_VALUE[0] + 1))
		{
				goto __exit_loop_6;
		}

		{
				TRACK_VALUE[0] = seq_sort_core(TRACK_VALUE[0]);
				TRACK_VALID[0] = 1;
				TRACK_POS[0] = 7;
		}
		if (!READ_NORMAL(TRACK_VALUE[0] + 1))
		{
				goto __exit_loop_6;
		}

		{
				TRACK_VALUE[0] = seq_sort_core(TRACK_VALUE[0]);
				TRACK_VALID[0] = 1;
				TRACK_POS[0] = 7;
		}
		if (!READ_NORMAL(TRACK_VALUE[0] + 1))
		{
				goto __exit_loop_6;
		}

		{
				TRACK_VALUE[0] = seq_sort_core(TRACK_VALUE[0]);
				TRACK_VALID[0] = 1;
				TRACK_POS[0] = 7;
		}
		if (!READ_NORMAL(TRACK_VALUE[0] + 1))
		{
				goto __exit_loop_6;
		}

		{
				TRACK_VALUE[0] = seq_sort_core(TRACK_VALUE[0]);
				TRACK_VALID[0] = 1;
				TRACK_POS[0] = 7;
		}
		if (!READ_NORMAL(TRACK_VALUE[0] + 1))
		{
				goto __exit_loop_6;
		}

		{
				TRACK_VALUE[0] = seq_sort_core(TRACK_VALUE[0]);
				TRACK_VALID[0] = 1;
				TRACK_POS[0] = 7;
		}
		if (!READ_NORMAL(TRACK_VALUE[0] + 1))
		{
				goto __exit_loop_6;
		}

		{
				TRACK_VALUE[0] = seq_sort_core(TRACK_VALUE[0]);
				TRACK_VALID[0] = 1;
				TRACK_POS[0] = 7;
		}
		assume(!READ_NORMAL(TRACK_VALUE[0] + 1));
		__exit_loop_6:
		;

		inspect_after(TRACK_VALUE[0]);
		TRACK_VALUE[3] = READ_POS(0);
		my_free(TRACK_VALUE[0]);
		if (!TRACK_VALUE[3])
		{
				goto __exit_loop_7;
		}

		{
				TRACK_VALUE[4] = READ_POS(3);
				TRACK_VALID[4] = 1;
				TRACK_POS[4] = 3;
				my_free(TRACK_VALUE[4]);
				TRACK_VALUE[3] = TRACK_VALUE[4];
				TRACK_VALID[3] = 1;
				TRACK_POS[3] = 1;
		}
		if (!TRACK_VALUE[3])
		{
				goto __exit_loop_7;
		}

		{
				TRACK_VALUE[4] = READ_POS(3);
				TRACK_VALID[4] = 1;
				TRACK_POS[4] = 3;
				my_free(TRACK_VALUE[4]);
				TRACK_VALUE[3] = TRACK_VALUE[4];
				TRACK_VALID[3] = 1;
				TRACK_POS[3] = 1;
		}
		if (!TRACK_VALUE[3])
		{
				goto __exit_loop_7;
		}

		{
				TRACK_VALUE[4] = READ_POS(3);
				TRACK_VALID[4] = 1;
				TRACK_POS[4] = 3;
				my_free(TRACK_VALUE[4]);
				TRACK_VALUE[3] = TRACK_VALUE[4];
				TRACK_VALID[3] = 1;
				TRACK_POS[3] = 1;
		}
		if (!TRACK_VALUE[3])
		{
				goto __exit_loop_7;
		}

		{
				TRACK_VALUE[4] = READ_POS(3);
				TRACK_VALID[4] = 1;
				TRACK_POS[4] = 3;
				my_free(TRACK_VALUE[4]);
				TRACK_VALUE[3] = TRACK_VALUE[4];
				TRACK_VALID[3] = 1;
				TRACK_POS[3] = 1;
		}
		if (!TRACK_VALUE[3])
		{
				goto __exit_loop_7;
		}

		{
				TRACK_VALUE[4] = READ_POS(3);
				TRACK_VALID[4] = 1;
				TRACK_POS[4] = 3;
				my_free(TRACK_VALUE[4]);
				TRACK_VALUE[3] = TRACK_VALUE[4];
				TRACK_VALID[3] = 1;
				TRACK_POS[3] = 1;
		}
		if (!TRACK_VALUE[3])
		{
				goto __exit_loop_7;
		}

		{
				TRACK_VALUE[4] = READ_POS(3);
				TRACK_VALID[4] = 1;
				TRACK_POS[4] = 3;
				my_free(TRACK_VALUE[4]);
				TRACK_VALUE[3] = TRACK_VALUE[4];
				TRACK_VALID[3] = 1;
				TRACK_POS[3] = 1;
		}
		if (!TRACK_VALUE[3])
		{
				goto __exit_loop_7;
		}

		{
				TRACK_VALUE[4] = READ_POS(3);
				TRACK_VALID[4] = 1;
				TRACK_POS[4] = 3;
				my_free(TRACK_VALUE[4]);
				TRACK_VALUE[3] = TRACK_VALUE[4];
				TRACK_VALID[3] = 1;
				TRACK_POS[3] = 1;
		}
		assume(!TRACK_VALUE[3]);
		__exit_loop_7:
		;

		return 0;
}




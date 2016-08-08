
#include <stdio.h>
#include <stdlib.h>
#define __cs_safe_malloc malloc
void __llbmc_assume(int);
void __llbmc_assert(int);

#define assume(x) __llbmc_assume(x)

#define assert(x) __llbmc_assert(x)

#define __VERIFIER_nondet_int() __llbmc_nondef_int()

int __llbmc_nondef_int(void);

unsigned int TRACK_ADDRESS[21];
int TRACK_VALUE[21];
unsigned int num_malloc;
void init_MEM(void)
{
		unsigned int TRACK_ADDRESS_tmp[21];
		TRACK_ADDRESS[0] = 1;
		TRACK_ADDRESS[1] = 2;
		TRACK_ADDRESS[2] = 3;
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
		TRACK_ADDRESS[17] = TRACK_ADDRESS_tmp[14];
		assume((TRACK_ADDRESS[17] <= 10000) & (TRACK_ADDRESS[17] > TRACK_ADDRESS[16]));
		TRACK_ADDRESS[18] = TRACK_ADDRESS_tmp[15];
		assume((TRACK_ADDRESS[18] <= 10000) & (TRACK_ADDRESS[18] > TRACK_ADDRESS[17]));
		TRACK_ADDRESS[19] = TRACK_ADDRESS_tmp[16];
		assume((TRACK_ADDRESS[19] <= 10000) & (TRACK_ADDRESS[19] > TRACK_ADDRESS[18]));
		TRACK_ADDRESS[20] = 10001;
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

int READ_NORMAL(unsigned int address)
{
		int pos;
		assume((pos >= 0) & (pos < 20));
		assume(TRACK_ADDRESS[pos] == address);
		return TRACK_VALUE[pos];
}

void WRITE_NORMAL(unsigned int address, int value)
{
		int pos;
		assume((pos >= 0) & (pos < 20));
		assume((address >= TRACK_ADDRESS[pos]) & (address < TRACK_ADDRESS[pos + 1]));
		if (TRACK_ADDRESS[pos] == address)
		{
				TRACK_VALUE[pos] = value;
		}
		else
		{
				assume(0);
		}

}

void merge_single_node(unsigned int dst, unsigned int data)
{
		TRACK_VALUE[12] = dst;
		TRACK_VALUE[13] = data;
		TRACK_VALUE[14] = READ_NORMAL(TRACK_VALUE[13]);
		WRITE_NORMAL(TRACK_VALUE[13], READ_NORMAL(TRACK_VALUE[14]));
		WRITE_NORMAL(TRACK_VALUE[14], 0);
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
				if (!TRACK_VALUE[8])
				{
						WRITE_NORMAL(TRACK_VALUE[6] + 1, TRACK_VALUE[7]);
						TRACK_VALUE[7] = TRACK_VALUE[6];
						goto __exit_loop_2;
				}

				merge_pair(TRACK_VALUE[6], READ_NORMAL(TRACK_VALUE[6]), READ_NORMAL(TRACK_VALUE[8]));
				WRITE_NORMAL(TRACK_VALUE[6] + 1, TRACK_VALUE[7]);
				TRACK_VALUE[7] = TRACK_VALUE[6];
				TRACK_VALUE[6] = READ_NORMAL(TRACK_VALUE[8] + 1);
				my_free(TRACK_VALUE[8]);
		}
		if (!TRACK_VALUE[6])
		{
				goto __exit_loop_2;
		}

		{
				TRACK_VALUE[8] = READ_NORMAL(TRACK_VALUE[6] + 1);
				if (!TRACK_VALUE[8])
				{
						WRITE_NORMAL(TRACK_VALUE[6] + 1, TRACK_VALUE[7]);
						TRACK_VALUE[7] = TRACK_VALUE[6];
						goto __exit_loop_2;
				}

				merge_pair(TRACK_VALUE[6], READ_NORMAL(TRACK_VALUE[6]), READ_NORMAL(TRACK_VALUE[8]));
				WRITE_NORMAL(TRACK_VALUE[6] + 1, TRACK_VALUE[7]);
				TRACK_VALUE[7] = TRACK_VALUE[6];
				TRACK_VALUE[6] = READ_NORMAL(TRACK_VALUE[8] + 1);
				my_free(TRACK_VALUE[8]);
		}
		if (!TRACK_VALUE[6])
		{
				goto __exit_loop_2;
		}

		{
				TRACK_VALUE[8] = READ_NORMAL(TRACK_VALUE[6] + 1);
				if (!TRACK_VALUE[8])
				{
						WRITE_NORMAL(TRACK_VALUE[6] + 1, TRACK_VALUE[7]);
						TRACK_VALUE[7] = TRACK_VALUE[6];
						goto __exit_loop_2;
				}

				merge_pair(TRACK_VALUE[6], READ_NORMAL(TRACK_VALUE[6]), READ_NORMAL(TRACK_VALUE[8]));
				WRITE_NORMAL(TRACK_VALUE[6] + 1, TRACK_VALUE[7]);
				TRACK_VALUE[7] = TRACK_VALUE[6];
				TRACK_VALUE[6] = READ_NORMAL(TRACK_VALUE[8] + 1);
				my_free(TRACK_VALUE[8]);
		}
		if (!TRACK_VALUE[6])
		{
				goto __exit_loop_2;
		}

		{
				TRACK_VALUE[8] = READ_NORMAL(TRACK_VALUE[6] + 1);
				if (!TRACK_VALUE[8])
				{
						WRITE_NORMAL(TRACK_VALUE[6] + 1, TRACK_VALUE[7]);
						TRACK_VALUE[7] = TRACK_VALUE[6];
						goto __exit_loop_2;
				}

				merge_pair(TRACK_VALUE[6], READ_NORMAL(TRACK_VALUE[6]), READ_NORMAL(TRACK_VALUE[8]));
				WRITE_NORMAL(TRACK_VALUE[6] + 1, TRACK_VALUE[7]);
				TRACK_VALUE[7] = TRACK_VALUE[6];
				TRACK_VALUE[6] = READ_NORMAL(TRACK_VALUE[8] + 1);
				my_free(TRACK_VALUE[8]);
		}
		if (!TRACK_VALUE[6])
		{
				goto __exit_loop_2;
		}

		{
				TRACK_VALUE[8] = READ_NORMAL(TRACK_VALUE[6] + 1);
				if (!TRACK_VALUE[8])
				{
						WRITE_NORMAL(TRACK_VALUE[6] + 1, TRACK_VALUE[7]);
						TRACK_VALUE[7] = TRACK_VALUE[6];
						goto __exit_loop_2;
				}

				merge_pair(TRACK_VALUE[6], READ_NORMAL(TRACK_VALUE[6]), READ_NORMAL(TRACK_VALUE[8]));
				WRITE_NORMAL(TRACK_VALUE[6] + 1, TRACK_VALUE[7]);
				TRACK_VALUE[7] = TRACK_VALUE[6];
				TRACK_VALUE[6] = READ_NORMAL(TRACK_VALUE[8] + 1);
				my_free(TRACK_VALUE[8]);
		}
		if (!TRACK_VALUE[6])
		{
				goto __exit_loop_2;
		}

		{
				TRACK_VALUE[8] = READ_NORMAL(TRACK_VALUE[6] + 1);
				if (!TRACK_VALUE[8])
				{
						WRITE_NORMAL(TRACK_VALUE[6] + 1, TRACK_VALUE[7]);
						TRACK_VALUE[7] = TRACK_VALUE[6];
						goto __exit_loop_2;
				}

				merge_pair(TRACK_VALUE[6], READ_NORMAL(TRACK_VALUE[6]), READ_NORMAL(TRACK_VALUE[8]));
				WRITE_NORMAL(TRACK_VALUE[6] + 1, TRACK_VALUE[7]);
				TRACK_VALUE[7] = TRACK_VALUE[6];
				TRACK_VALUE[6] = READ_NORMAL(TRACK_VALUE[8] + 1);
				my_free(TRACK_VALUE[8]);
		}
		if (!TRACK_VALUE[6])
		{
				goto __exit_loop_2;
		}

		{
				TRACK_VALUE[8] = READ_NORMAL(TRACK_VALUE[6] + 1);
				if (!TRACK_VALUE[8])
				{
						WRITE_NORMAL(TRACK_VALUE[6] + 1, TRACK_VALUE[7]);
						TRACK_VALUE[7] = TRACK_VALUE[6];
						goto __exit_loop_2;
				}

				merge_pair(TRACK_VALUE[6], READ_NORMAL(TRACK_VALUE[6]), READ_NORMAL(TRACK_VALUE[8]));
				WRITE_NORMAL(TRACK_VALUE[6] + 1, TRACK_VALUE[7]);
				TRACK_VALUE[7] = TRACK_VALUE[6];
				TRACK_VALUE[6] = READ_NORMAL(TRACK_VALUE[8] + 1);
				my_free(TRACK_VALUE[8]);
		}
		if (!TRACK_VALUE[6])
		{
				goto __exit_loop_2;
		}

		{
				TRACK_VALUE[8] = READ_NORMAL(TRACK_VALUE[6] + 1);
				if (!TRACK_VALUE[8])
				{
						WRITE_NORMAL(TRACK_VALUE[6] + 1, TRACK_VALUE[7]);
						TRACK_VALUE[7] = TRACK_VALUE[6];
						goto __exit_loop_2;
				}

				merge_pair(TRACK_VALUE[6], READ_NORMAL(TRACK_VALUE[6]), READ_NORMAL(TRACK_VALUE[8]));
				WRITE_NORMAL(TRACK_VALUE[6] + 1, TRACK_VALUE[7]);
				TRACK_VALUE[7] = TRACK_VALUE[6];
				TRACK_VALUE[6] = READ_NORMAL(TRACK_VALUE[8] + 1);
				my_free(TRACK_VALUE[8]);
		}
		if (!TRACK_VALUE[6])
		{
				goto __exit_loop_2;
		}

		{
				TRACK_VALUE[8] = READ_NORMAL(TRACK_VALUE[6] + 1);
				if (!TRACK_VALUE[8])
				{
						WRITE_NORMAL(TRACK_VALUE[6] + 1, TRACK_VALUE[7]);
						TRACK_VALUE[7] = TRACK_VALUE[6];
						goto __exit_loop_2;
				}

				merge_pair(TRACK_VALUE[6], READ_NORMAL(TRACK_VALUE[6]), READ_NORMAL(TRACK_VALUE[8]));
				WRITE_NORMAL(TRACK_VALUE[6] + 1, TRACK_VALUE[7]);
				TRACK_VALUE[7] = TRACK_VALUE[6];
				TRACK_VALUE[6] = READ_NORMAL(TRACK_VALUE[8] + 1);
				my_free(TRACK_VALUE[8]);
		}
		if (!TRACK_VALUE[6])
		{
				goto __exit_loop_2;
		}

		{
				TRACK_VALUE[8] = READ_NORMAL(TRACK_VALUE[6] + 1);
				if (!TRACK_VALUE[8])
				{
						WRITE_NORMAL(TRACK_VALUE[6] + 1, TRACK_VALUE[7]);
						TRACK_VALUE[7] = TRACK_VALUE[6];
						goto __exit_loop_2;
				}

				merge_pair(TRACK_VALUE[6], READ_NORMAL(TRACK_VALUE[6]), READ_NORMAL(TRACK_VALUE[8]));
				WRITE_NORMAL(TRACK_VALUE[6] + 1, TRACK_VALUE[7]);
				TRACK_VALUE[7] = TRACK_VALUE[6];
				TRACK_VALUE[6] = READ_NORMAL(TRACK_VALUE[8] + 1);
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
		;
		if (!READ_NORMAL(TRACK_VALUE[5] + 1))
		{
				goto __exit_loop_3;
		}

		{
				assert(TRACK_VALUE[5]);
				assert(READ_NORMAL(TRACK_VALUE[5] + 1));
				assert(READ_NORMAL(TRACK_VALUE[5]));
				assert(READ_NORMAL(READ_NORMAL(TRACK_VALUE[5])) == 0);
		}
		TRACK_VALUE[5] = READ_NORMAL(TRACK_VALUE[5] + 1);
		if (!READ_NORMAL(TRACK_VALUE[5] + 1))
		{
				goto __exit_loop_3;
		}

		{
				assert(TRACK_VALUE[5]);
				assert(READ_NORMAL(TRACK_VALUE[5] + 1));
				assert(READ_NORMAL(TRACK_VALUE[5]));
				assert(READ_NORMAL(READ_NORMAL(TRACK_VALUE[5])) == 0);
		}
		TRACK_VALUE[5] = READ_NORMAL(TRACK_VALUE[5] + 1);
		if (!READ_NORMAL(TRACK_VALUE[5] + 1))
		{
				goto __exit_loop_3;
		}

		{
				assert(TRACK_VALUE[5]);
				assert(READ_NORMAL(TRACK_VALUE[5] + 1));
				assert(READ_NORMAL(TRACK_VALUE[5]));
				assert(READ_NORMAL(READ_NORMAL(TRACK_VALUE[5])) == 0);
		}
		TRACK_VALUE[5] = READ_NORMAL(TRACK_VALUE[5] + 1);
		if (!READ_NORMAL(TRACK_VALUE[5] + 1))
		{
				goto __exit_loop_3;
		}

		{
				assert(TRACK_VALUE[5]);
				assert(READ_NORMAL(TRACK_VALUE[5] + 1));
				assert(READ_NORMAL(TRACK_VALUE[5]));
				assert(READ_NORMAL(READ_NORMAL(TRACK_VALUE[5])) == 0);
		}
		TRACK_VALUE[5] = READ_NORMAL(TRACK_VALUE[5] + 1);
		if (!READ_NORMAL(TRACK_VALUE[5] + 1))
		{
				goto __exit_loop_3;
		}

		{
				assert(TRACK_VALUE[5]);
				assert(READ_NORMAL(TRACK_VALUE[5] + 1));
				assert(READ_NORMAL(TRACK_VALUE[5]));
				assert(READ_NORMAL(READ_NORMAL(TRACK_VALUE[5])) == 0);
		}
		TRACK_VALUE[5] = READ_NORMAL(TRACK_VALUE[5] + 1);
		if (!READ_NORMAL(TRACK_VALUE[5] + 1))
		{
				goto __exit_loop_3;
		}

		{
				assert(TRACK_VALUE[5]);
				assert(READ_NORMAL(TRACK_VALUE[5] + 1));
				assert(READ_NORMAL(TRACK_VALUE[5]));
				assert(READ_NORMAL(READ_NORMAL(TRACK_VALUE[5])) == 0);
		}
		TRACK_VALUE[5] = READ_NORMAL(TRACK_VALUE[5] + 1);
		if (!READ_NORMAL(TRACK_VALUE[5] + 1))
		{
				goto __exit_loop_3;
		}

		{
				assert(TRACK_VALUE[5]);
				assert(READ_NORMAL(TRACK_VALUE[5] + 1));
				assert(READ_NORMAL(TRACK_VALUE[5]));
				assert(READ_NORMAL(READ_NORMAL(TRACK_VALUE[5])) == 0);
		}
		TRACK_VALUE[5] = READ_NORMAL(TRACK_VALUE[5] + 1);
		if (!READ_NORMAL(TRACK_VALUE[5] + 1))
		{
				goto __exit_loop_3;
		}

		{
				assert(TRACK_VALUE[5]);
				assert(READ_NORMAL(TRACK_VALUE[5] + 1));
				assert(READ_NORMAL(TRACK_VALUE[5]));
				assert(READ_NORMAL(READ_NORMAL(TRACK_VALUE[5])) == 0);
		}
		TRACK_VALUE[5] = READ_NORMAL(TRACK_VALUE[5] + 1);
		if (!READ_NORMAL(TRACK_VALUE[5] + 1))
		{
				goto __exit_loop_3;
		}

		{
				assert(TRACK_VALUE[5]);
				assert(READ_NORMAL(TRACK_VALUE[5] + 1));
				assert(READ_NORMAL(TRACK_VALUE[5]));
				assert(READ_NORMAL(READ_NORMAL(TRACK_VALUE[5])) == 0);
		}
		TRACK_VALUE[5] = READ_NORMAL(TRACK_VALUE[5] + 1);
		if (!READ_NORMAL(TRACK_VALUE[5] + 1))
		{
				goto __exit_loop_3;
		}

		{
				assert(TRACK_VALUE[5]);
				assert(READ_NORMAL(TRACK_VALUE[5] + 1));
				assert(READ_NORMAL(TRACK_VALUE[5]));
				assert(READ_NORMAL(READ_NORMAL(TRACK_VALUE[5])) == 0);
		}
		TRACK_VALUE[5] = READ_NORMAL(TRACK_VALUE[5] + 1);
		assume(!READ_NORMAL(TRACK_VALUE[5] + 1));
		__exit_loop_3:
		;

		assert(TRACK_VALUE[5]);
		assert(READ_NORMAL(TRACK_VALUE[5] + 1) == 0);
		assert(READ_NORMAL(TRACK_VALUE[5]));
		assert(READ_NORMAL(READ_NORMAL(TRACK_VALUE[5])) == 0);
}

void inspect_after(unsigned int shape)
{
		TRACK_VALUE[15] = shape;
		assert(shape);
		assert(READ_NORMAL(shape + 1) == 0);
		assert(READ_NORMAL(shape) != 0);
		TRACK_VALUE[8] = READ_NORMAL(shape);
		if (!READ_NORMAL(TRACK_VALUE[8]))
		{
				goto __exit_loop_4;
		}

		{
				;
		}
		TRACK_VALUE[8] = READ_NORMAL(TRACK_VALUE[8]);
		if (!READ_NORMAL(TRACK_VALUE[8]))
		{
				goto __exit_loop_4;
		}

		{
				;
		}
		TRACK_VALUE[8] = READ_NORMAL(TRACK_VALUE[8]);
		if (!READ_NORMAL(TRACK_VALUE[8]))
		{
				goto __exit_loop_4;
		}

		{
				;
		}
		TRACK_VALUE[8] = READ_NORMAL(TRACK_VALUE[8]);
		if (!READ_NORMAL(TRACK_VALUE[8]))
		{
				goto __exit_loop_4;
		}

		{
				;
		}
		TRACK_VALUE[8] = READ_NORMAL(TRACK_VALUE[8]);
		if (!READ_NORMAL(TRACK_VALUE[8]))
		{
				goto __exit_loop_4;
		}

		{
				;
		}
		TRACK_VALUE[8] = READ_NORMAL(TRACK_VALUE[8]);
		if (!READ_NORMAL(TRACK_VALUE[8]))
		{
				goto __exit_loop_4;
		}

		{
				;
		}
		TRACK_VALUE[8] = READ_NORMAL(TRACK_VALUE[8]);
		if (!READ_NORMAL(TRACK_VALUE[8]))
		{
				goto __exit_loop_4;
		}

		{
				;
		}
		TRACK_VALUE[8] = READ_NORMAL(TRACK_VALUE[8]);
		if (!READ_NORMAL(TRACK_VALUE[8]))
		{
				goto __exit_loop_4;
		}

		{
				;
		}
		TRACK_VALUE[8] = READ_NORMAL(TRACK_VALUE[8]);
		if (!READ_NORMAL(TRACK_VALUE[8]))
		{
				goto __exit_loop_4;
		}

		{
				;
		}
		TRACK_VALUE[8] = READ_NORMAL(TRACK_VALUE[8]);
		if (!READ_NORMAL(TRACK_VALUE[8]))
		{
				goto __exit_loop_4;
		}

		{
				;
		}
		TRACK_VALUE[8] = READ_NORMAL(TRACK_VALUE[8]);
		assume(!READ_NORMAL(TRACK_VALUE[8]));
		__exit_loop_4:
		;

		{
				assert(!READ_NORMAL(TRACK_VALUE[8]));
		}
}

int main()
{
		init_MEM();
		TRACK_VALUE[0] = 0;
		if (!__VERIFIER_nondet_int())
		{
				goto __exit_loop_5;
		}

		{
				TRACK_VALUE[1] = my_malloc(1 * 2);
				WRITE_NORMAL(TRACK_VALUE[1], TRACK_VALUE[1]);
				WRITE_NORMAL(TRACK_VALUE[1] + 1, __VERIFIER_nondet_int());
				TRACK_VALUE[2] = my_malloc(1 * 2);
				WRITE_NORMAL(TRACK_VALUE[2], TRACK_VALUE[1]);
				WRITE_NORMAL(TRACK_VALUE[2] + 1, TRACK_VALUE[0]);
				TRACK_VALUE[0] = TRACK_VALUE[2];
		}
		if (!__VERIFIER_nondet_int())
		{
				goto __exit_loop_5;
		}

		{
				TRACK_VALUE[1] = my_malloc(1 * 2);
				WRITE_NORMAL(TRACK_VALUE[1], TRACK_VALUE[1]);
				WRITE_NORMAL(TRACK_VALUE[1] + 1, __VERIFIER_nondet_int());
				TRACK_VALUE[2] = my_malloc(1 * 2);
				WRITE_NORMAL(TRACK_VALUE[2], TRACK_VALUE[1]);
				WRITE_NORMAL(TRACK_VALUE[2] + 1, TRACK_VALUE[0]);
				TRACK_VALUE[0] = TRACK_VALUE[2];
		}
		if (!__VERIFIER_nondet_int())
		{
				goto __exit_loop_5;
		}

		{
				TRACK_VALUE[1] = my_malloc(1 * 2);
				WRITE_NORMAL(TRACK_VALUE[1], TRACK_VALUE[1]);
				WRITE_NORMAL(TRACK_VALUE[1] + 1, __VERIFIER_nondet_int());
				TRACK_VALUE[2] = my_malloc(1 * 2);
				WRITE_NORMAL(TRACK_VALUE[2], TRACK_VALUE[1]);
				WRITE_NORMAL(TRACK_VALUE[2] + 1, TRACK_VALUE[0]);
				TRACK_VALUE[0] = TRACK_VALUE[2];
		}
		if (!__VERIFIER_nondet_int())
		{
				goto __exit_loop_5;
		}

		{
				TRACK_VALUE[1] = my_malloc(1 * 2);
				WRITE_NORMAL(TRACK_VALUE[1], TRACK_VALUE[1]);
				WRITE_NORMAL(TRACK_VALUE[1] + 1, __VERIFIER_nondet_int());
				TRACK_VALUE[2] = my_malloc(1 * 2);
				WRITE_NORMAL(TRACK_VALUE[2], TRACK_VALUE[1]);
				WRITE_NORMAL(TRACK_VALUE[2] + 1, TRACK_VALUE[0]);
				TRACK_VALUE[0] = TRACK_VALUE[2];
		}
		if (!__VERIFIER_nondet_int())
		{
				goto __exit_loop_5;
		}

		{
				TRACK_VALUE[1] = my_malloc(1 * 2);
				WRITE_NORMAL(TRACK_VALUE[1], TRACK_VALUE[1]);
				WRITE_NORMAL(TRACK_VALUE[1] + 1, __VERIFIER_nondet_int());
				TRACK_VALUE[2] = my_malloc(1 * 2);
				WRITE_NORMAL(TRACK_VALUE[2], TRACK_VALUE[1]);
				WRITE_NORMAL(TRACK_VALUE[2] + 1, TRACK_VALUE[0]);
				TRACK_VALUE[0] = TRACK_VALUE[2];
		}
		if (!__VERIFIER_nondet_int())
		{
				goto __exit_loop_5;
		}

		{
				TRACK_VALUE[1] = my_malloc(1 * 2);
				WRITE_NORMAL(TRACK_VALUE[1], TRACK_VALUE[1]);
				WRITE_NORMAL(TRACK_VALUE[1] + 1, __VERIFIER_nondet_int());
				TRACK_VALUE[2] = my_malloc(1 * 2);
				WRITE_NORMAL(TRACK_VALUE[2], TRACK_VALUE[1]);
				WRITE_NORMAL(TRACK_VALUE[2] + 1, TRACK_VALUE[0]);
				TRACK_VALUE[0] = TRACK_VALUE[2];
		}
		if (!__VERIFIER_nondet_int())
		{
				goto __exit_loop_5;
		}

		{
				TRACK_VALUE[1] = my_malloc(1 * 2);
				WRITE_NORMAL(TRACK_VALUE[1], TRACK_VALUE[1]);
				WRITE_NORMAL(TRACK_VALUE[1] + 1, __VERIFIER_nondet_int());
				TRACK_VALUE[2] = my_malloc(1 * 2);
				WRITE_NORMAL(TRACK_VALUE[2], TRACK_VALUE[1]);
				WRITE_NORMAL(TRACK_VALUE[2] + 1, TRACK_VALUE[0]);
				TRACK_VALUE[0] = TRACK_VALUE[2];
		}
		if (!__VERIFIER_nondet_int())
		{
				goto __exit_loop_5;
		}

		{
				TRACK_VALUE[1] = my_malloc(1 * 2);
				WRITE_NORMAL(TRACK_VALUE[1], TRACK_VALUE[1]);
				WRITE_NORMAL(TRACK_VALUE[1] + 1, __VERIFIER_nondet_int());
				TRACK_VALUE[2] = my_malloc(1 * 2);
				WRITE_NORMAL(TRACK_VALUE[2], TRACK_VALUE[1]);
				WRITE_NORMAL(TRACK_VALUE[2] + 1, TRACK_VALUE[0]);
				TRACK_VALUE[0] = TRACK_VALUE[2];
		}
		if (!__VERIFIER_nondet_int())
		{
				goto __exit_loop_5;
		}

		{
				TRACK_VALUE[1] = my_malloc(1 * 2);
				WRITE_NORMAL(TRACK_VALUE[1], TRACK_VALUE[1]);
				WRITE_NORMAL(TRACK_VALUE[1] + 1, __VERIFIER_nondet_int());
				TRACK_VALUE[2] = my_malloc(1 * 2);
				WRITE_NORMAL(TRACK_VALUE[2], TRACK_VALUE[1]);
				WRITE_NORMAL(TRACK_VALUE[2] + 1, TRACK_VALUE[0]);
				TRACK_VALUE[0] = TRACK_VALUE[2];
		}
		if (!__VERIFIER_nondet_int())
		{
				goto __exit_loop_5;
		}

		{
				TRACK_VALUE[1] = my_malloc(1 * 2);
				WRITE_NORMAL(TRACK_VALUE[1], TRACK_VALUE[1]);
				WRITE_NORMAL(TRACK_VALUE[1] + 1, __VERIFIER_nondet_int());
				TRACK_VALUE[2] = my_malloc(1 * 2);
				WRITE_NORMAL(TRACK_VALUE[2], TRACK_VALUE[1]);
				WRITE_NORMAL(TRACK_VALUE[2] + 1, TRACK_VALUE[0]);
				TRACK_VALUE[0] = TRACK_VALUE[2];
		}
		assume(!__VERIFIER_nondet_int());
		__exit_loop_5:
		;

		if (TRACK_VALUE[0] == 0)
		{
				return 0;
		}

		inspect_before(TRACK_VALUE[0]);
		if (!READ_NORMAL(TRACK_VALUE[0] + 1))
		{
				goto __exit_loop_6;
		}

		{
				TRACK_VALUE[0] = seq_sort_core(TRACK_VALUE[0]);
		}
		if (!READ_NORMAL(TRACK_VALUE[0] + 1))
		{
				goto __exit_loop_6;
		}

		{
				TRACK_VALUE[0] = seq_sort_core(TRACK_VALUE[0]);
		}
		if (!READ_NORMAL(TRACK_VALUE[0] + 1))
		{
				goto __exit_loop_6;
		}

		{
				TRACK_VALUE[0] = seq_sort_core(TRACK_VALUE[0]);
		}
		if (!READ_NORMAL(TRACK_VALUE[0] + 1))
		{
				goto __exit_loop_6;
		}

		{
				TRACK_VALUE[0] = seq_sort_core(TRACK_VALUE[0]);
		}
		if (!READ_NORMAL(TRACK_VALUE[0] + 1))
		{
				goto __exit_loop_6;
		}

		{
				TRACK_VALUE[0] = seq_sort_core(TRACK_VALUE[0]);
		}
		if (!READ_NORMAL(TRACK_VALUE[0] + 1))
		{
				goto __exit_loop_6;
		}

		{
				TRACK_VALUE[0] = seq_sort_core(TRACK_VALUE[0]);
		}
		if (!READ_NORMAL(TRACK_VALUE[0] + 1))
		{
				goto __exit_loop_6;
		}

		{
				TRACK_VALUE[0] = seq_sort_core(TRACK_VALUE[0]);
		}
		if (!READ_NORMAL(TRACK_VALUE[0] + 1))
		{
				goto __exit_loop_6;
		}

		{
				TRACK_VALUE[0] = seq_sort_core(TRACK_VALUE[0]);
		}
		if (!READ_NORMAL(TRACK_VALUE[0] + 1))
		{
				goto __exit_loop_6;
		}

		{
				TRACK_VALUE[0] = seq_sort_core(TRACK_VALUE[0]);
		}
		if (!READ_NORMAL(TRACK_VALUE[0] + 1))
		{
				goto __exit_loop_6;
		}

		{
				TRACK_VALUE[0] = seq_sort_core(TRACK_VALUE[0]);
		}
		assume(!READ_NORMAL(TRACK_VALUE[0] + 1));
		__exit_loop_6:
		;

		inspect_after(TRACK_VALUE[0]);
		TRACK_VALUE[3] = READ_NORMAL(TRACK_VALUE[0]);
		my_free(TRACK_VALUE[0]);
		if (!TRACK_VALUE[3])
		{
				goto __exit_loop_7;
		}

		{
				TRACK_VALUE[4] = READ_NORMAL(TRACK_VALUE[3]);
				my_free(TRACK_VALUE[4]);
				TRACK_VALUE[3] = TRACK_VALUE[4];
		}
		if (!TRACK_VALUE[3])
		{
				goto __exit_loop_7;
		}

		{
				TRACK_VALUE[4] = READ_NORMAL(TRACK_VALUE[3]);
				my_free(TRACK_VALUE[4]);
				TRACK_VALUE[3] = TRACK_VALUE[4];
		}
		if (!TRACK_VALUE[3])
		{
				goto __exit_loop_7;
		}

		{
				TRACK_VALUE[4] = READ_NORMAL(TRACK_VALUE[3]);
				my_free(TRACK_VALUE[4]);
				TRACK_VALUE[3] = TRACK_VALUE[4];
		}
		if (!TRACK_VALUE[3])
		{
				goto __exit_loop_7;
		}

		{
				TRACK_VALUE[4] = READ_NORMAL(TRACK_VALUE[3]);
				my_free(TRACK_VALUE[4]);
				TRACK_VALUE[3] = TRACK_VALUE[4];
		}
		if (!TRACK_VALUE[3])
		{
				goto __exit_loop_7;
		}

		{
				TRACK_VALUE[4] = READ_NORMAL(TRACK_VALUE[3]);
				my_free(TRACK_VALUE[4]);
				TRACK_VALUE[3] = TRACK_VALUE[4];
		}
		if (!TRACK_VALUE[3])
		{
				goto __exit_loop_7;
		}

		{
				TRACK_VALUE[4] = READ_NORMAL(TRACK_VALUE[3]);
				my_free(TRACK_VALUE[4]);
				TRACK_VALUE[3] = TRACK_VALUE[4];
		}
		if (!TRACK_VALUE[3])
		{
				goto __exit_loop_7;
		}

		{
				TRACK_VALUE[4] = READ_NORMAL(TRACK_VALUE[3]);
				my_free(TRACK_VALUE[4]);
				TRACK_VALUE[3] = TRACK_VALUE[4];
		}
		if (!TRACK_VALUE[3])
		{
				goto __exit_loop_7;
		}

		{
				TRACK_VALUE[4] = READ_NORMAL(TRACK_VALUE[3]);
				my_free(TRACK_VALUE[4]);
				TRACK_VALUE[3] = TRACK_VALUE[4];
		}
		if (!TRACK_VALUE[3])
		{
				goto __exit_loop_7;
		}

		{
				TRACK_VALUE[4] = READ_NORMAL(TRACK_VALUE[3]);
				my_free(TRACK_VALUE[4]);
				TRACK_VALUE[3] = TRACK_VALUE[4];
		}
		if (!TRACK_VALUE[3])
		{
				goto __exit_loop_7;
		}

		{
				TRACK_VALUE[4] = READ_NORMAL(TRACK_VALUE[3]);
				my_free(TRACK_VALUE[4]);
				TRACK_VALUE[3] = TRACK_VALUE[4];
		}
		assume(!TRACK_VALUE[3]);
		__exit_loop_7:
		;

		return 0;
}




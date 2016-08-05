
#include <stdio.h>
#include <stdlib.h>
#define __cs_safe_malloc malloc
#define assume __CPROVER_assume

extern int __VERIFIER_nondet_int(void);
struct node
{
		struct node *next;
		int value;
};
struct list
{
		struct node *slist;
		struct list *next;
};
void merge_single_node(struct node ***dst, 
struct node **data)
{
		struct node *node;
		node = *data;
		*data = node->next;
		node->next = 0;
		*(*dst) = node;
		*dst = &node->next;
}

void merge_pair(struct node **dst, 
struct node *sub1, 
struct node *sub2)
{
		if (!(sub1 || sub2))
		{
				goto __exit_loop_1;
		}

		{
				if ((!sub2) || (sub1 && (sub1->value < sub2->value)))
				{
						merge_single_node(&dst, &sub1);
				}
				else
				{
						merge_single_node(&dst, &sub2);
				}

		}
		if (!(sub1 || sub2))
		{
				goto __exit_loop_1;
		}

		{
				if ((!sub2) || (sub1 && (sub1->value < sub2->value)))
				{
						merge_single_node(&dst, &sub1);
				}
				else
				{
						merge_single_node(&dst, &sub2);
				}

		}
		if (!(sub1 || sub2))
		{
				goto __exit_loop_1;
		}

		{
				if ((!sub2) || (sub1 && (sub1->value < sub2->value)))
				{
						merge_single_node(&dst, &sub1);
				}
				else
				{
						merge_single_node(&dst, &sub2);
				}

		}
		if (!(sub1 || sub2))
		{
				goto __exit_loop_1;
		}

		{
				if ((!sub2) || (sub1 && (sub1->value < sub2->value)))
				{
						merge_single_node(&dst, &sub1);
				}
				else
				{
						merge_single_node(&dst, &sub2);
				}

		}
		if (!(sub1 || sub2))
		{
				goto __exit_loop_1;
		}

		{
				if ((!sub2) || (sub1 && (sub1->value < sub2->value)))
				{
						merge_single_node(&dst, &sub1);
				}
				else
				{
						merge_single_node(&dst, &sub2);
				}

		}
		assume(!(sub1 || sub2));
		__exit_loop_1:
		;

}

struct list *seq_sort_core(struct list *data)
{
		struct list *dst;
		dst = 0;
		if (!data)
		{
				goto __exit_loop_2;
		}

		{
				struct list *next;
				next = data->next;
				if (!next)
				{
						data->next = dst;
						dst = data;
						goto __exit_loop_2;
				}

				merge_pair(&data->slist, data->slist, next->slist);
				data->next = dst;
				dst = data;
				data = next->next;
				free(next);
		}
		if (!data)
		{
				goto __exit_loop_2;
		}

		{
				struct list *next;
				next = data->next;
				if (!next)
				{
						data->next = dst;
						dst = data;
						goto __exit_loop_2;
				}

				merge_pair(&data->slist, data->slist, next->slist);
				data->next = dst;
				dst = data;
				data = next->next;
				free(next);
		}
		if (!data)
		{
				goto __exit_loop_2;
		}

		{
				struct list *next;
				next = data->next;
				if (!next)
				{
						data->next = dst;
						dst = data;
						goto __exit_loop_2;
				}

				merge_pair(&data->slist, data->slist, next->slist);
				data->next = dst;
				dst = data;
				data = next->next;
				free(next);
		}
		if (!data)
		{
				goto __exit_loop_2;
		}

		{
				struct list *next;
				next = data->next;
				if (!next)
				{
						data->next = dst;
						dst = data;
						goto __exit_loop_2;
				}

				merge_pair(&data->slist, data->slist, next->slist);
				data->next = dst;
				dst = data;
				data = next->next;
				free(next);
		}
		if (!data)
		{
				goto __exit_loop_2;
		}

		{
				struct list *next;
				next = data->next;
				if (!next)
				{
						data->next = dst;
						dst = data;
						goto __exit_loop_2;
				}

				merge_pair(&data->slist, data->slist, next->slist);
				data->next = dst;
				dst = data;
				data = next->next;
				free(next);
		}
		assume(!data);
		__exit_loop_2:
		;

		return dst;
}

void inspect_before(struct list *shape)
{
		assert(shape);
		;
		if (!shape->next)
		{
				goto __exit_loop_3;
		}

		{
				assert(shape);
				assert(shape->next);
				assert(shape->slist);
				assert(shape->slist->next == 0);
		}
		shape = shape->next;
		if (!shape->next)
		{
				goto __exit_loop_3;
		}

		{
				assert(shape);
				assert(shape->next);
				assert(shape->slist);
				assert(shape->slist->next == 0);
		}
		shape = shape->next;
		if (!shape->next)
		{
				goto __exit_loop_3;
		}

		{
				assert(shape);
				assert(shape->next);
				assert(shape->slist);
				assert(shape->slist->next == 0);
		}
		shape = shape->next;
		if (!shape->next)
		{
				goto __exit_loop_3;
		}

		{
				assert(shape);
				assert(shape->next);
				assert(shape->slist);
				assert(shape->slist->next == 0);
		}
		shape = shape->next;
		if (!shape->next)
		{
				goto __exit_loop_3;
		}

		{
				assert(shape);
				assert(shape->next);
				assert(shape->slist);
				assert(shape->slist->next == 0);
		}
		shape = shape->next;
		assume(!shape->next);
		__exit_loop_3:
		;

		assert(shape);
		assert(shape->next == 0);
		assert(shape->slist);
		assert(shape->slist->next == 0);
}

void inspect_after(struct list *shape)
{
		assert(shape);
		assert(shape->next == 0);
		assert(shape->slist != 0);
		struct node *pos;
		pos = shape->slist;
		if (!pos->next)
		{
				goto __exit_loop_4;
		}

		{
				;
		}
		pos = pos->next;
		if (!pos->next)
		{
				goto __exit_loop_4;
		}

		{
				;
		}
		pos = pos->next;
		if (!pos->next)
		{
				goto __exit_loop_4;
		}

		{
				;
		}
		pos = pos->next;
		if (!pos->next)
		{
				goto __exit_loop_4;
		}

		{
				;
		}
		pos = pos->next;
		if (!pos->next)
		{
				goto __exit_loop_4;
		}

		{
				;
		}
		pos = pos->next;
		assume(!pos->next);
		__exit_loop_4:
		;

		assert(!pos->next);
}

int main()
{
		struct list *data;
		data = 0;
		if (!__VERIFIER_nondet_int())
		{
				goto __exit_loop_5;
		}

		{
				struct node *node;
				node = __cs_safe_malloc(1 * (sizeof(struct node)));
				node->next = node;
				node->value = __VERIFIER_nondet_int();
				struct list *item;
				item = __cs_safe_malloc(1 * (sizeof(struct list)));
				item->slist = node;
				item->next = data;
				data = item;
		}
		if (!__VERIFIER_nondet_int())
		{
				goto __exit_loop_5;
		}

		{
				struct node *node;
				node = __cs_safe_malloc(1 * (sizeof(struct node)));
				node->next = node;
				node->value = __VERIFIER_nondet_int();
				struct list *item;
				item = __cs_safe_malloc(1 * (sizeof(struct list)));
				item->slist = node;
				item->next = data;
				data = item;
		}
		if (!__VERIFIER_nondet_int())
		{
				goto __exit_loop_5;
		}

		{
				struct node *node;
				node = __cs_safe_malloc(1 * (sizeof(struct node)));
				node->next = node;
				node->value = __VERIFIER_nondet_int();
				struct list *item;
				item = __cs_safe_malloc(1 * (sizeof(struct list)));
				item->slist = node;
				item->next = data;
				data = item;
		}
		if (!__VERIFIER_nondet_int())
		{
				goto __exit_loop_5;
		}

		{
				struct node *node;
				node = __cs_safe_malloc(1 * (sizeof(struct node)));
				node->next = node;
				node->value = __VERIFIER_nondet_int();
				struct list *item;
				item = __cs_safe_malloc(1 * (sizeof(struct list)));
				item->slist = node;
				item->next = data;
				data = item;
		}
		if (!__VERIFIER_nondet_int())
		{
				goto __exit_loop_5;
		}

		{
				struct node *node;
				node = __cs_safe_malloc(1 * (sizeof(struct node)));
				node->next = node;
				node->value = __VERIFIER_nondet_int();
				struct list *item;
				item = __cs_safe_malloc(1 * (sizeof(struct list)));
				item->slist = node;
				item->next = data;
				data = item;
		}
		assume(!__VERIFIER_nondet_int());
		__exit_loop_5:
		;

		if (!data)
		{
				return 0;
		}

		inspect_before(data);
		if (!data->next)
		{
				goto __exit_loop_6;
		}

		{
				data = seq_sort_core(data);
		}
		if (!data->next)
		{
				goto __exit_loop_6;
		}

		{
				data = seq_sort_core(data);
		}
		if (!data->next)
		{
				goto __exit_loop_6;
		}

		{
				data = seq_sort_core(data);
		}
		if (!data->next)
		{
				goto __exit_loop_6;
		}

		{
				data = seq_sort_core(data);
		}
		if (!data->next)
		{
				goto __exit_loop_6;
		}

		{
				data = seq_sort_core(data);
		}
		assume(!data->next);
		__exit_loop_6:
		;

		inspect_after(data);
		struct node *node;
		node = data->slist;
		free(data);
		if (!node)
		{
				goto __exit_loop_7;
		}

		{
				struct node *snext;
				snext = node->next;
				free(node);
				node = snext;
		}
		if (!node)
		{
				goto __exit_loop_7;
		}

		{
				struct node *snext;
				snext = node->next;
				free(node);
				node = snext;
		}
		if (!node)
		{
				goto __exit_loop_7;
		}

		{
				struct node *snext;
				snext = node->next;
				free(node);
				node = snext;
		}
		if (!node)
		{
				goto __exit_loop_7;
		}

		{
				struct node *snext;
				snext = node->next;
				free(node);
				node = snext;
		}
		if (!node)
		{
				goto __exit_loop_7;
		}

		{
				struct node *snext;
				snext = node->next;
				free(node);
				node = snext;
		}
		assume(!node);
		__exit_loop_7:
		;

		return 0;
}




#ifndef __RANDOM_DROP_H__
#define __RANDOM_DROP_H__

#define RANDOM_DROP_ACCURACY 10000 // a lower value will decrease the accuracy

typedef struct random_drop_t
{
	unsigned int id;
	float probability; // [0, 1]
} random_drop_t;

void random_drop_init(void);
random_drop_t *random_drop_choose(random_drop_t *drop_list, size_t drop_list_amount);

#endif /* __RANDOM_DROP_H__ */
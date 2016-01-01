#include <stdlib.h>
#include <time.h>

#include "random-drop.h"

/**
 * This function initializes random drops.
 */
void random_drop_init(void)
{
	srand(time(NULL));
}

/**
 * This function chooses a random drop from a drop list. It chooses with care
 * about probabilities.
 * 
 * @param drop_list The drop list.
 * @param drop_list_amount The amount of drops in the drop list.
 * @return A pointer to the choosed drop in the list. This pointer can be a NULL
 *         pointer if no drop was choosed.
 */
random_drop_t *random_drop_choose(random_drop_t *drop_list, size_t drop_list_amount)
{
	unsigned int i = 0;
	float random = 0;
	float drop_list_probability = 0;
	float passed_random = 0;
	
	// generates a random number in [0, 1] with accuracy
	random = (float)(rand() % RANDOM_DROP_ACCURACY) / RANDOM_DROP_ACCURACY;
	
	// retrieve overall probability (adding all probabilities)
	for(i = 0; i < drop_list_amount; i++)
	{
		drop_list_probability += drop_list[i].probability;
	}
	
	// when drop list probability is greater than 1 multiply the random value
	if(drop_list_probability > 1)
	{
		random *= drop_list_probability;
	}
	
	// actual choosing algorithm: Increase the passed_random-value until it is
	// greater than the random value.
	for(i = 0; i < drop_list_amount; i++)
	{
		passed_random += drop_list[i].probability;
		
		if(passed_random > random)
		{
			return &(drop_list[i]);
		}
	}
	
	return NULL;
}

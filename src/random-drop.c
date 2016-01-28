/*
 * Copyright (C) 2015 NIPE-SYSTEMS
 * Copyright (C) 2015 Jonas Krug
 * Copyright (C) 2015 Tim Gevers
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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

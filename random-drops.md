# Random drop reference

The game features a random drop algorithm which is able to pick a random drop with included probabilities.

## Structures

The algorithm consumes a list (a.k.a. *array*) of drops. Each drop has two properties:

* A unique ID for identifying the drop.
* The probability of the drop. `0%` will be `0` and `100%` will be `1.0`. The values are in the range `[0, 1]`.

The structure is defined in `src/random-drop.h`:

```c_cpp
typedef struct random_drop_t
{
    unsigned int id;
    float probability; // [0, 1]
} random_drop_t;
```

A list may be declared as following:

```c_cpp
random_drop_t drop_list[] =
{
    { 1, 0.5 },
    { 2, 0.5 }
};
```

## Choosing algoritm

The algorithm calculates the overall probability by adding all probability-values. When this sum is greater than `1` the random value will be multiplied with this value.

If the sum adds to a number which is lower than `1` it is possible that the algorithm returns an empty drop. This is indicated by a `NULL`-pointer returned by the `random_drop_choose()`-function.

The algorithm will add the probabilities to a value. When this value is then greater than the random picked number the current drop is the choosed one.

## Example code

```c_cpp
random_drop_t drop_list[] =
{
    { 1, 0.5 }, // 50% probability
    { 2, 0.25 } // 25% probability
};
size_t drop_list_amount = sizeof(drop_list) / sizeof(drop_list[0]);
random_drop_t *picked_drop = NULL;

picked_drop = random_drop_choose(drop_list, drop_list_amount);
if(picked_drop != NULL)
{
    printf("Choosed id: %i\n", picked_drop->id);
}
else
{
    printf("No drop choosed.\n");
}
```

## Random value generation accuracy

The accuracy of the generated floating point random value may be changed. The default value is `10000` which results in a accuracy of `1 * 10^-5`. This value can be changed in `src/random-drop.h`.

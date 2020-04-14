## C-step-counter

This is the main repository for the thesis *Open-source algorithm for wearables in healthcare-applications* made by Anna Brondin & Marcus Nordstrom at Malmö University.

## Guide

This is only the algorithm. It is used in [C-step-counter-test](https://github.com/MarcusNordstrom/C-step-counter-test) and [Pinetime-zephyr](https://github.com/MarcusNordstrom/pinetime-zephyr).

To use it in your own project you need to:

 - Collect raw data into a CSV file formated as *time(ms), X, Y, Z*
 - Optimize the constants with [C-optimize-variables](https://github.com/MarcusNordstrom/C-optimize-variables)
 - Insert the new constants into the stages.
 - Build the library with cmake or clone the repo directly into your project

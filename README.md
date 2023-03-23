# Cellular Automata Simulation



<p align="center">
  <img src="https://raw.githubusercontent.com/Alexander-Rus/Cellular-Automata-Simulation/main/Img/output-2023-03-17_11_10_42-converted_AdobeExpress.gif" width="738">
</p>

In this simulation I used C++ to create a cellular interaction between predator organisms and prey organisms. Using SDL to render, organisms are placed on a grid at random. The predator organisms are colored in white, while the prey organisms are colored in green. This is a cellular simulation that simulates predator organisms consuming prey organisms. **This code needs to be re-factored**.

Organisms are contained on a 2D grid, that is specified when executing the program. For this example the grid is 1000 x 1000 however the scaling factor is set to 10, so it appears as a grid of 100 x 100 for easier viewing. When first creating the program, the organisms were not distinguished as either predator or prey, instead they were written to appear on the grid and move around randomly. This is what an early output looked like:

<p align="center">
  <img src="https://raw.githubusercontent.com/Alexander-Rus/Cellular-Automata-Simulation/main/Img/output-2023-03-14_15_23_10-converted_AdobeExpress.gif" width="738">
</p>


Eventually this behavior was scaled up. Each organism is stored in a vector of objects and is then rendered to the screen. Organisms can then randomly choose a direction of travel and move in that direction. In order for organisms to see other organisms, there is an unordered map that keeps track of grid location and who is occupying that grid. So when choosing a new location at random, if that cell is occupied according to the map, then the organisms must chose another unoccupied cell. Adding many organisms yields a result like so:

<p align="center">
  <img src="https://raw.githubusercontent.com/Alexander-Rus/Cellular-Automata-Simulation/main/Img/output-2023-03-14_16_29_05-converted_AdobeExpress.gif" width="738">
</p>

When an organism is created it is randomly assigned to be either a predator or prey organism. Prey organisms will move around and avoid occupied cells, however predator organisms have the added ability to consume prey. They will look at adjacent cells using the unordered map and if a prey is in a near by cell, it will move to that cell. Prey are then removed from the vector of objects.

<p align="center">
  <img src="https://raw.githubusercontent.com/Alexander-Rus/Cellular-Automata-Simulation/main/Img/output-2023-03-17_11_10_42-converted_AdobeExpress.gif" width="738">
</p>


We can see that the organisms will continue moving around and try to interact. If we add a very high number of organisms, they the predators almost instantly eat the prey as there simply is not enough open space for the prey to escape consumption.


<p align="center">
  <img src="https://raw.githubusercontent.com/Alexander-Rus/Cellular-Automata-Simulation/main/Img/output-2023-03-17_12_21_05-converted_AdobeExpress%20(4).gif" width="738">
</p>

## Key take aways
This simulation is a good example of rending multiple objects and having them interact. If the scale is set to 1, and the number of organisms is set very high, then the program can have some performance issues. I added some multi-threading to help render different parts of the screen to improve performance. 

Additionally, in the future, the simulation should incorporate more tasks for both the predators and prey. Prey should be able to breed if they live long enough, and increase the overall number of individuals. Predators should also die should they go long enough without consuming a prey organism.

The code is messy at the moment and does need re-factoring, but it is functional.

## How to run
Clone this repo, and cd into the root directory. 
Run the command `make all` to make the project. In the bin folder there should be an executable, you can run it with the command `./bin/main`

# Game_Of_Life

## Overview
A C implementation of Conway's Game of Life. '.' represent dead cells and '*' represent alive cells. 

## Usage 
To complie run 'make'. To delete all files produced during compilation run make clean.

**-i input_filename:** to specify that the initial generation of the universe read from a file. If
this option is not specified, the user may type in the input. 

**-o output_filename:** to specify a file into which the final generation should be output. If this option
is not specified, output of the final generation is on the screen. 

**-g number_of_generations:** to specify the number of new generations for which the game should be
run. (Set to 5 if this option is not given.)

**-s statistics:** to print statistics after the final generation has been output. 

**-t torus:** to use the torus topology. If this is not specified, cells outside the
universe are permanently dead.

## Example
![Screenshot from 2022-09-05 11-03-47](https://user-images.githubusercontent.com/30124151/188424683-72d6e496-1791-428a-be8d-9f89d55240b7.png)

### Input
![Screenshot from 2022-09-05 11-04-15](https://user-images.githubusercontent.com/30124151/188424834-de09788b-1c92-4c73-93f1-0023a6d10f77.png)

### Output
![Screenshot from 2022-09-05 11-06-35](https://user-images.githubusercontent.com/30124151/188424916-71527b23-ff7d-4ce9-8f9e-54d61ca6a271.png)

![Screenshot from 2022-09-05 11-04-26](https://user-images.githubusercontent.com/30124151/188424876-cb563edd-99e7-4c5a-a87f-40d026906482.png)

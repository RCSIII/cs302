# COSC302 - Data Structures and Algorithms II
# Getting motivated
A retired CS professor has started a gaming company. She needs your help to write a program for creating and solving mazes. Being old school, she is not keen on C++ but will accept the use of classes etc. However, STL is off-limits meaning you have to essentially write a C-style program. You need an array, you create one. You need a stack, you create one. You want to do file I/O, you use fscanf, fprintf, etc. The good news is that you can use what you have learned about disjoint sets, graphs and minimum spanning trees to do this.

# Getting started
See Wikipedia for a general description of how to generate and solve mazes: http://en.wikipedia.org/wiki/Maze_generation_algorithm We will use the Kruskal version that generates mazes which have lots of the usual deadends. Perhaps somewhat unusual, any cell can be reached from any other cell. This would easy to fix but is besides the point of the exercise.
Run the script /home/cosc302/labs/lab7/copy to obtain a shell script called game.sh (that takes the fun out of generating and solving mazes -- sorry), three Hydra exectutables called smazemake, smazesolve and smazeimage, as well as Mazemake.cpp and Mazesolve.cpp (skeleton codes). Your job is to write the code needed to create Mazemake, Mazesolve, and Mazeimage programs which must behave as described next.

# Assignment details

* GENERAL RULES
You are not allowed to use any STL containers or algorithms. Instead you must use the C standard library algorithms defined in cstdlib, cstdio, cstring, ctime, unistd.h, etc. Thus, if you need a vector array or a stack, use a simple array, and if you need to swap two data elements, write a function that does that. File I/O must be based on fopen, fclose, fscanf, fprintf, fwrite, etc. See the Linux man pages for information on how to use these functions.
* Makefile (10 pts)
You are not given a makefile for this assignment. Write your own which must include source code and header file dependencies for all targets. Do not include any of the C standard library header files as dependencies.
* Mazemake (40 pts)
Mazemake generates a random maze. The program takes two commandline arguments, namely, Nrow and Ncol. Your first job is to write this program which must be compatible with the solution executable. See behavior examples below.
Conceptually, imagine an Nrow x Ncol grid where the grid lines have been colored in. Now, randomly erase grid lines between neighboring cells until paths exist that will allow you to walk from any cell to any other cell. That's the maze you will make. You will treat this maze grid as a graph. Cell (i,j) has four neighbors, namely, (i-1,j), (i,j-1), (i+1,j), and (i,j+1). Connectivity is bidirectional. That is, if (i-1,j; i,j) represents a wall, then (i,j; i-1;j) represents the same wall viewed in the opposite direction. Same goes when no wall is present. Cells on the periphery do not have neighbors outside the grid. The easiest way to handle them is to never remove the outer boundary walls thereby making it impossible to step off the grid. Also, we will only consider the walls (i,j; i+1,j) and (i,j; i,j+1). If you view the compass orientation of the four neighbors, then we are only concerned with South and East since North and West can be obtained by symmetry.

As you know, a graph consists of vertices and edges. However, you don't need to store the maze graph vertices since they are merely (i,j) grid cell index pairs which you can traverse using nested loops. You also don't need to store the edges since they are given by the grid. What you do need to store are variables that indicate whether a neighbor is walled off or not. We will do that using pairs of grid cell indices.

Initially, set all cells to be completely walled in. Generate an ordered list of the associated interior walls that you then subject to a random permutation (see code handout). Having done that, you merely have to iterate thru the list while using a disjoint-set (see code handout) to see if a wall should be removed. How so? First, place each cell in its own set. When considering to remove the wall between neighboring cells, do so only if the two cells belong to different sets in which case you also merge the cells into the same set. When all cells belong to the same set, stop the process since all cells are now connected to each other. Not merging cells indicates that the wall is intact in which case you write it to file.

* Mazesolve (50 pts)
Mazesolve finds a path thru the maze. The program reads a maze file and outputs the solution to a path file. Your code must be compatible with the solution executable. See behavior example below.
Read a given maze file and re-create the graph that represents it by doing the opposite of what you did in the mazemake program. Hint: Mazemake initialized the maze to have walls between every cell and then proceeded to knock them down. Mazesolve should initialize the maze to not have any walls and use the maze file to add them.

Consider creating a three-dimensional boolean array that indicates which of the four walls are in place for each grid cell. Thus, when you read (i,j;i+1,j) or (i,j;i,j+1) from the maze file, you must create not just those walls but also (i+1,j;i,j) or (i,j+1;i,j).

Once the graph is in place, apply depth-first search to find a path thru the maze. This is stack based backtracking only you have to create and maintain the stack using an array. Assume the source is grid cell (0,0) and that the sink is grid cell (Nrow-1,Ncol-1). Keep in mind that you don't want to enter into cycles. That is, you don't want to revisit cells already visited. Use vertex coloring or a similar boolean variable for this purpose. Also, you can't walk thru walls. Finally, you need to leave a breadcrumb trail as you go along, so you can find your way back. That is, make a note of who the predecessor is when you move from one cell to the next. This would be done using a link array.

The stack could be a 1D array that holds 2D cell information. The color and the link data structures are more conveniently declared as 2D arrays although you could also implement them as 1D arrays that hold 2D cell information.

The last step is to output the path to stdout for redirection into a file. The output format should be as indicated below. This time, you output the sequence of cells on the path.

# Mazemake example
```
unix> mazemake 23 41 maze.txt
unix> head maze.txt

MAZE 23 41
  6  10   7  10
 21  19  22  19
  6  25   6  26
 21  34  21  35
 11   1  11   2
 11  36  11  37
 19  17  20  17
  3   5   4   5
 15   1  15   2
 ```
# Mazesolve example
```
unix> mazesolve maze.txt path.txt
unix> head path.txt

PATH 23 41
0 0
1 0
2 0
2 1
3 1
3 2
4 2
4 1
5 1
```

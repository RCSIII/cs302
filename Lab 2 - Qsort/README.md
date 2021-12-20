# COSC302 - Data Structures and Algorithms II
# Getting started
Run the script /home/cosc302/labs/lab2/copy to obtain a Hydra executable called sqsort, data files called list1.txt, list2.txt, and list3.txt, a skeleton program called Qsort.cpp, and a makefile for compiling it into a Hydra executable (once properly fleshed out). Your job is to write the missing source code which must behave as described next.

# Assignment details
* Vers 1 Modify Qsort.cpp as follows. The data class has three private data members: firstname, lastname, and phonenumber. Add code for data class input and output operators. The input operator simply reads each of data items when called. The output operator prints a data object to stdout using the format: lastname firstname phonenumber. See examples below. Part of the assignment is for you to figure out how to maintain a fixed width of the name field so that all phonenumbers are left justified to the same column position. Finally, implement printlist() as a templated function that takes generic iterator arguments and steps thru the data while printing to stdout. Do whatever else is needed to make the program compile such as adding missing header files and commenting out pseudo-code. Then test and make sure it reads and writes data from the list files as required.
* Vers 2 Add an overloaded operator< to the data class, so you can impose an order on such objects. Test that it works by applying the std::sort algorithm before writing the list content to stdout. Objects that have the same lastname should use the firstnames to determine which is less than the other. Objects that also have the same firstnames should use the phone numbers to determine which is less than the other. See examples below.
* Vers 3 Add command line argument handling. When completed, any one of the following three ways of calling Qsort should be valid:
unix> Qsort -stl list.txt
unix> Qsort -rpivot list.txt
unix> Qsort -rpivot k0 k1 list.txt
The first version executes the code from above. The second version sorts the data using your own implementation of quicksort. The third version partitions the input data using quickselect below before applying quicksort to sort the data in the range given by integers k0 and k1. Without checking, assume that k0 is less than k1. Do check if k0 is less than 0, in which case it should silently be changed to 0. Likewise, check if k1 is greater than or equal to the number of elements in the data array, say N, and if so, silently change it to N-1.

* Vers 4 Implement quicksort as follows. Copy the partition() and quicksort() functions from the class04_sorting_3 handout. Replace the median-of-three pivot with a randomly chosen index in the left-to-right range and use it to partition the data. This new version forces you to think about how the median-of-three partitioning code works since unlike for it, you may now need to explicitly check and prevent going out-bounds during the search for data pairs to swap. To make you think even more about the algorithm, merge all the functionality into the quicksort() function. That is, do not have an external function that carries out the partitioning. Keep it all inside the quicksort() function which is called from main. Test and compare the output with what you obtained using std::sort. The two output should be identical since they use the same comparison operator.
* Vers 5 Copy the quickselect() function from the class04_sorting_3 handout. Embed the random pivot based partition code that you added to quicksort(). The resulting function will take four arguments, i.e., quickselect(A, left, k, right) where A is a data array, left and right designate the first and the last indices to consider (the range), and k the element that needs to be selected (found and put in its proper place). Make whatever other modifications are needed to get the code to compile. Test the program to make sure you the selection is correct.
Use quickselect(A, 0, k0, N-1) to put the k0-th element in its proper location and partition the data in A accordingly. That is, ensure that all data stored to the left of A[k0] is lexicographically less than or equal to it. All data stored to the right of A[k0] must likewise be greater than or equal to it.

Then use quickselect(A, k0, k1, N-1) to put the k1-th element in its proper location and partition the data in A accordingly. Note that data indexed below k0 is ignored at this point. When this works, use the quicksort() function to sort the data in the k0 thru k1 index range.

This completes the assignment. Submit Qsort.cpp on Canvas after you have cleaned the code up and added a few comments.

# Data and executable output examples
```
unix> cat list1.txt

CANDACE WITT        	250-939-5404
THEODORE PERKINS    	723-668-3397
THAD HULL           	708-807-6757
STEPHAN SALAZAR     	415-413-5058
ISRAEL WILKINS      	938-701-1455
BRUCE PERRY         	540-916-2956
VALENTIN RIVERS     	726-204-2377
WILFRED JOHNSTON    	582-126-8861
JORDAN WILKINS      	938-701-1455
LEVI SPENCE         	985-365-7415
KAYLA NGUYEN        	484-322-1527

unix> ./Qsort -stl list1.txt 

HULL, THAD              708-807-6757	0
JOHNSTON, WILFRED       582-126-8861	1
NGUYEN, KAYLA           484-322-1527	2
PERKINS, THEODORE       723-668-3397	3
PERRY, BRUCE            540-916-2956	4
RIVERS, VALENTIN        726-204-2377	5
SALAZAR, STEPHAN        415-413-5058	6
SPENCE, LEVI            985-365-7415	7
WILKINS, ISRAEL         938-701-1455	8
WILKINS, JORDAN         938-701-1455	9
WITT, CANDACE           250-939-5404	10

unix> ./Qsort -rpivot list1.txt 

[same as above]

unix> ./Qsort -rpivot 3 5 list1.txt

JOHNSTON, WILFRED       582-126-8861	0
HULL, THAD              708-807-6757	1
NGUYEN, KAYLA           484-322-1527	2
PERKINS, THEODORE       723-668-3397	3 **
PERRY, BRUCE            540-916-2956	4 **
RIVERS, VALENTIN        726-204-2377	5 **
SPENCE, LEVI            985-365-7415	6
SALAZAR, STEPHAN        415-413-5058	7
WILKINS, ISRAEL         938-701-1455	8
WITT, CANDACE           250-939-5404	9
WILKINS, JORDAN         938-701-1455	10
```

The line numbers added to right for the last output are for illustrational purposes and not produced by the program. They help you see that the data in lines 3-5 is sorted (by design), that the data lines 0-1, 6-7, and 9-10 are not, and that the data in lines 2 and 8 are (by chance).
```
unix> ./Qsort -stl list3.txt | grep FRANK | head -10

ALLEN, FRANK            315-870-8352
BAKER, FRANK            632-844-8785
BELL, FRANK             822-318-4291
BENNETT, FRANK          936-442-8282
BROWN, FRANK            374-427-9120
BUTLER, FRANK           269-462-6264 **
BUTLER, FRANK           396-862-5765 **
CARTER, FRANK           648-481-5277 **
CARTER, FRANK           673-417-2955 **
DIAZ, FRANK             594-782-3469
```
Notice that entries are sorted by lastname, then firstname and then phonenumber. There may not be similar duplicates in list1.txt and list2.txt.

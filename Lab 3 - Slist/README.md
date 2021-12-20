# COSC302 - Data Structures and Algorithms II
# Getting motivated
In this lab you will continue sorting the multi-column data from Lab 2 which consisted of a firstname, a lastname, and a phone number. The main difference is that the data will now be stored in single linked list. In order to allow sorting by means of STL sorting algorithms (or any of the ones covered in class), you will embed a smart pointer subclass within the single linked list class and write the associated code for setting things up and applying the result.

# Getting started
Run the script /home/cosc302/labs/lab3/copy to obtain a Hydra solution executable called sslist, test data files repeat1.txt and repeat2.txt, skeleton code Slist.h and Slist.cpp, and a makefile for compiling the program into a Hydra executable (once properly fleshed out). Your job is to write the missing source code which must behave as described next.

# Assignment details

* Slist.h Vers 1 Implement the slist::node::constructor that takes a data argument; this functionality is needed by slist::push_back() when it creates a new node.
Slist.cpp Vers 1 Copy and paste the data class definition from the Qsort.cpp you wrote for Lab 2 along with the associated input and output operators and the template based printlist function. Add needed header files and do whatever else it takes for the code to compile (shouldn't be much and may be as simple as commenting incomplete code out), then test and make sure you can read repeat1.txt and print the content back to stdout.

* Slist.h Vers 2 Add an overloaded less-than comparison operator to the slist::node subclass. Add a private slist::sptr subclass; copy the code verbatim from the smart pointer handout, then modify it to explicitly work for slist::node data.
Update the slist::sort() function to sort the single linked list data using the std::sort() function. Determine the number of elements stored in the list. Allocate a vector of smart pointers of that size called Ap. Initialize the individual smart pointers to point to the list nodes. For example, if p = head, then Ap[0] = p->next, etc. Apply std::sort and use the sorted Ap vector to relink the list nodes. Do not simply repeat the array based rearrangement covered in class which used pointers to the data itself. Here, the smart pointers point to nodes that hold the data. Also, keep in mind that operator[] is not available. You will need to use list node pointers. When the sorting is done, step through the smart pointer vector while creating the linked list. For example, if p = head, then p->next = Ap[0], etc. Simple as that (once you figure it out). You may want to draw a picture of what's going on to help visualize the process.

Slist.cpp Vers 2 Update the main function to call slist::sort with an appropriate argument. Compile and test the code using the repeat1.txt and repeat2.txt files.

* Add logic to call std::stable_sort if slist::sort is called with the string "mergesort" and std::sort if the argument is "quicksort".
Slist.cpp Vers 3 Add command line argument handling. Most of this can be copied from Qsort.cpp. When completed, either of the following two ways of executing Slist will be valid:
```
unix> Slist -mergesort repeat.txt
unix> Slist -quicksort repeat.txt
```
The program should print usage information if given any other options or none. Also, update your data reading to be from the specified file instead of stdin.
Finally, add an integer ID to the private member section of the data class. Update the input operator to set this ID to the reflect the order of appearance of a data object in the input stream. Likewise, update the output operator to print this ID. Compare performance for mergesort and quicksort. For some data, you will see that identical data objects appear in the wrong order for quicksort, while mergesort always maintains the correct order.

# Example runs based on repeat1.txt
```
unix> ./Slist -mergesort repeat1.txt 

 ACEVEDO GAVIN           356-488-3678       86
 ACEVEDO GAVIN           356-488-3678      257
 ADKINS NOREEN           218-589-7364      266
 AGUIRRE ROLANDO         232-480-9863       66
 AGUIRRE ROLANDO         232-480-9863       99
 AGUIRRE ROLANDO         232-480-9863      310
 ALLISON CHELSEA         565-133-4367      184
 ALSTON ELOY             625-705-2549      284
 ALSTON ELOY             625-705-2549      291
 ANTHONY EDMOND          498-558-5625      290
 ASHLEY ROSALIND         381-587-7613      175
 ASHLEY ROSALIND         381-587-7613      301
 ATKINSON BETHANY        475-954-1105       92
 AYALA DAMON             376-528-9382       43
 AYALA DAMON             376-528-9382       81
 AYALA DAMON             376-528-9382      117
 AYALA DAMON             376-528-9382      318
 BAILEY ROBERTO          846-171-8639      158
 BAIRD LINWOOD           936-892-1659       27
 BAIRD LINWOOD           936-892-1659       98

unix> ./Slist -quicksort repeat1.txt | head -20

 ACEVEDO GAVIN           356-488-3678       86
 ACEVEDO GAVIN           356-488-3678      257
 ADKINS NOREEN           218-589-7364      266
 AGUIRRE ROLANDO         232-480-9863      310 **
 AGUIRRE ROLANDO         232-480-9863       66 **
 AGUIRRE ROLANDO         232-480-9863       99 **
 ALLISON CHELSEA         565-133-4367      184
 ALSTON ELOY             625-705-2549      284
 ALSTON ELOY             625-705-2549      291
 ANTHONY EDMOND          498-558-5625      290
 ASHLEY ROSALIND         381-587-7613      301 **
 ASHLEY ROSALIND         381-587-7613      175 **
 ATKINSON BETHANY        475-954-1105       92
 AYALA DAMON             376-528-9382      318 **
 AYALA DAMON             376-528-9382       81 **
 AYALA DAMON             376-528-9382       43 **
 AYALA DAMON             376-528-9382      117 **
 BAILEY ROBERTO          846-171-8639      158
 BAIRD LINWOOD           936-892-1659      190 **
 BAIRD LINWOOD           936-892-1659       98 **

unix> ./Slist -qsort_r3 repeat1.txt | head -20

 ACEVEDO GAVIN           356-488-3678      257 **
 ACEVEDO GAVIN           356-488-3678       86 **
 ADKINS NOREEN           218-589-7364      266
 AGUIRRE ROLANDO         232-480-9863      310 **
 AGUIRRE ROLANDO         232-480-9863       99 **
 AGUIRRE ROLANDO         232-480-9863       66 **
 ALLISON CHELSEA         565-133-4367      184
 ALSTON ELOY             625-705-2549      284 
 ALSTON ELOY             625-705-2549      291
 ANTHONY EDMOND          498-558-5625      290
 ASHLEY ROSALIND         381-587-7613      301 **
 ASHLEY ROSALIND         381-587-7613      175 **
 ATKINSON BETHANY        475-954-1105       92
 AYALA DAMON             376-528-9382       81 **
 AYALA DAMON             376-528-9382       43 **
 AYALA DAMON             376-528-9382      318 **
 AYALA DAMON             376-528-9382      117 **
 BAILEY ROBERTO          846-171-8639      158
 BAIRD LINWOOD           936-892-1659      321 **
 BAIRD LINWOOD           936-892-1659       98 **
```
The quicksort and qsort_r3 entries marked with an asterisk are out of order. You will see many more examples, if you look at the full output for repeat1.txt and repeat2.txt.

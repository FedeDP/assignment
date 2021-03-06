[![Build Status](https://travis-ci.org/FedeDP/assignment.svg?branch=master)](https://travis-ci.org/FedeDP/assignment)

# Assignment

## Design
This assignment has been developed with flexibility in mind.  
I tried to be as generic as possible. I therefore used a singly linked list to store data.  

### Linked list
Linked list is completely initialized using a double pointer (a pointer to the head of the list), as i wanted code quality to be as high as possible.  
Double pointer linked list allocation is delegated to its own function; this way it is way more extensible.  
Note that linked list is not needed for the purpose of this exercise, even less storing the [struct tm](https://github.com/FedeDP/assignment/blob/master/inc/assignment.h#L14) inside it.  
In fact, i could have read from file and compute mean impedance on the fly. But again, i think storing data helps extensibility for future uses.  

### Options mapping
Moreover, i had to find a proper way to map our options.conf variables to needed parameters.  
I thought that using an array of struct was the best idea: searching inside the array the correct value, eg: "it", and storing in my conf struct only the index of this struct, was a simple and clean solution.  
I could probably have used an hashmap too, to map eg: "it" with the correct index (for better performance). But i wanted to be as simple as possible, so i guessed that was the right solution for me.  

Talking about flexibility, program can support as many languages as we wish. Developer has only to add new elements to array of structs for formats and languages.  
I could use gettext to translate language strings, but again, i thought that was not the aim of this assignment. Moreover, formats struct would be needed anyway.  
By the way, i found it funny to develop a proper solution without involving gettext.  

### Input time string parsing
The part i struggled more was finding [strptime](http://man7.org/linux/man-pages/man3/strptime.3.html) function (reverse of strftime). At first i tried with [getdate](http://man7.org/linux/man-pages/man3/getdate.3.html), failing over and over, as it requires an env variable (DATEMSK) set to a file that contains the format.  
But thanks to the man-pages of getdate, i found out about strptime.  

### Error checking, valgrind and tests
The program has strict error checking, every issue is found and printed with the help of perror().

I'd like to highlight that program is fully valgrind clean too. Valgrind was ran with:  
```
alias | grep valgrind
alias valgrind='valgrind --tool=memcheck --leak-check=full --track-origins=yes --show-leak-kinds=all -v'
```

Code is tested through simple C asserts. Use "make tests" to create test executable.  
I test for not-existent input/options files, for empty input/options files, and then a normal execution.

### Extra things
As already stated, storing data was not required.  
Moreover, i used getopt to add the possibility to change path to options.conf and input.txt files too. Check with "./assignment -h" (or --help).  

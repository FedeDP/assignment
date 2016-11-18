# Assignment

## Design
This assignment has been developed with flexibility in mind.  
I tried to be as generic as possible. I therefore used a singly linked list to store data.  
Linked list is completely initialized using a double pointer (a pointer to the head of the list), as i wanted code quality to be as high as possible.  
Note that linked list is not mandatory for this exercise, neither is storing the "struct tm" inside it.  
In fact, i could have read from file and compute mean impedance on the fly. But again, i think storing data helps extensibility for future uses.  

Moreover, i had to find a proper way to map our options.conf variables to needed parameters.  
I thought that using an array of struct was the best idea: searching inside the array the correct value, eg: "it", and storing in my conf struct only the index of this struct, was a simple and clean solution.  
I could probably have used an hashmap too, to map eg: "it" with the correct index (for better performance). But i wanted to be as simple as possible, so i guessed that was the right solution for me.  

I used getopt to add the (not required) possibility to change path to options.conf and input.txt files too. Check with "./assignment -h" (or --help).  

The program has strict error checking, every issue is found and printed with the help of perror().

The part i struggled more was finding [strptime](http://man7.org/linux/man-pages/man3/strptime.3.html) function (converse of strftime). At first i tried with [getdate](http://man7.org/linux/man-pages/man3/getdate.3.html), failing over and over, as it requires an env variable (DATEMSK) set to a file that contains the format.  
But thanks to the man-pages of getdate, i found out about strptime.  

Talking about flexibility, program can support as many languages as we wish. Developer has only to add new structs for formats and languages.  
I could use gettext to translate language strings, but again, i thought that was not the aim of this assignment. Moreover, formats struct would be needed anyway.  
By the way, i found it funny to develop a proper solution without involving gettext.  

Finally, i'd like to highlight that program is fully valgrind clean. Valgrind was ran with:  
```
alias | grep valgrind
alias valgrind='valgrind --tool=memcheck --leak-check=full --track-origins=yes --show-leak-kinds=all -v'
```

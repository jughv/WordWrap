# WordWrap
Authors:
Chiu Yin Lau, NetID: cl1309
Julio Johnson Lopez, NetID: jmj227

Our Program uses two methods, our wrapping method as well as our main method.

int wrap(width, input file, output file):
    Our first method in the program returns an integer. The only possible integers are 0(no errors) or -1(there were errors). Furthermore, with no issues our program will "wrap" the given input file. By using the given width this method writes word by word into the output file meanwhile, making sure not to go over the width alotted for the current line. What this entails is if a word at the end of a line causes that line to go over the width, that word is instead pushed down to the next line instead. This way each word is placed one by one without going over the width. Since there is no limit on how many lines, only on the width of the columns this method works out extremely effectively. If there is a word whose length is longer than the necessary width an error message will be returned.


main method:
    Our main method works as a facilitator, by checking to see if the input is a file, a directory, or just stdin quickly written it then decides the proper reaction for each scenario. For a txt file it will send the file into the wrap method and stdout the words with the proper width. In the case of the directory it will go file by file creating new files with the prefix "wrap." which would then contain the files with the necessary width inputted by the user. Lastly for stdinputted text the text is returned instantly with the proper width. One can also make a file using > out and a file called out will be made from the words stdinputted into the terminal.

compiling:
    A makefile is given in our directory, the needed command is "make ww". After using this command the program will compile and be ready for use.

testing:
    Our program takes in 1 - 2 inputs, the first will be the int width, the second would be file or directory. During personal tests we found no issues running code by using "./ww 20 test1.txt" or using "./ww 20 testFolder". Using these commands printed out the text file with a 20 width limit; as well as made new files within the test folder with the prefix "wrap.", which all had the widths set to 20. We tested txt files as well as files with no extensions to see if the programs worked properly. When testing we also found that when using stdin to put in the txt it would instantly return whatever text was entered into the proper width. Lastly when using the > operator we could use the command "./ww 20 > out" and it would create a file with all the txt we inputted with the necessary width we used. Also the number is interchangeable, width can be any integer value we are only using 20 here for ease to read. If there is a word longer than width an error message will occur at the end allowing the user to see one word was longer than width.

Thank you for taking the time to review our project and we hope it is up to par.
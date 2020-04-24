# Text-Predictor
I created a text prediction algorithm that when given a certain word, will tell you what other words co-occur with it. 
Similar to auto-fill on most phones
  - To use the text predictor, you must feed it a body of text (to Text Predict) 
    along with the commands you want to execute. Both files should be .txt and the first file should contain
    the body of text and the second file should be the commands with one command per line.
  
  The text predictor comes with a few different commands:
  - @ str n 
    - will show you n words that have co-occruence with string "str"
  - ! 
    - Will print the full body of text including the amount of each word occurance
   - str
     - Prints the string if the string is contained within the body of the text file

  How to Use the program with linux style command line:
  1. Get on your command line and navigate to the folder which contains the Trieprediction.c & Trieprediction.h files.
  2. From there compile the file and launch using the following two commands.
     gcc TriePrediction.c 
    ./a.out "corpus01.txt" "input01.txt"
      The corpus file should be the body of text you want to predict from and 
      the input file should contain commands. Don't keep the quotations when running the commands.
 
  3. Examples:
    
    corpus03.txt:
      I like cherry pie and chocolate pie.
   
     input03.txt:
      !
      chocolaTE
      apricot
      @ I 11
      @ chocolate 1
      @ persimmon 20
  
  - If passed to your program, those files should result in the following output:
     output03.txt:
      and (1)
      cherry (1)
      chocolate (1)
      i (1)
      like (1)
      pie (2)
      chocolaTE
      - pie (1)
      apricot
      (INVALID STRING)
      I like cherry pie and chocolate pie and chocolate pie and chocolate
      chocolate pie
      persimmon
      
      Have fun text predicting!

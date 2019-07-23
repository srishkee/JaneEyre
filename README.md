# JaneEyre
Repo for the project-gutenberg text analysis 

0. The text analysis is performed using C++.
1. The book (Jane Eyre) is contained within the text file "1260.txt." The user must specify the book filename as an input to the program.
2. The compiled list of the 1000-Most-Commonly-Used-English-Words (same file as specified in project specs) is contained within the text file "1000Words.txt." The user must specify the filename as an input to the program.
3. All functions have been commented thoroughly, with testing performed in the main() block.

Interesting Notes: 

Jane Eyre was written in Modern English. As such, the writing style is much more formal, quite different from the one we are accustomed to today.

1. Bronte's word choice is quite unique at times, sometimes with particular phrases occurring only once. 
Consequently, the sentence returned by the method generateSentence() is surprisingly decipherable. 

2. In the list of "20 Least Frequent Words," many words are separated by a double dash ("--"), also reminiscent of older writing styles.
As such, several of the words include a double-dash, because an empty space was not an a thorough-enough delimiter.
While I have used the '--' as a delimiter in the generateSentence() method, paying more attention to older diction styles would yield more accurate results.




Mastermind is a fun board game. This program makes it possible to play the game against computer or run famous algorithms as playing computer. 

User can select the game mode with console interactions. 
Given user wants to play as computer she can select the algorithm from algoritms avaliable. 
But more importantly she can extend the algorithms easily thanks to the flexibility of the program by override the IStrategy class by just implementing a guess function. 

Currently only two algorihms implemented. 

Donalds Knuth's miniMax algorithm which avarages to 4.81 seconds after 100 trials and 
Swaszek's simple algoritm which always wins but avarages to 5.82. 

For reading the code UnitTests can help a lot which can run by calling the binary with option "-t". 












I example game between human code breaker vs computer code keeper can be seen below: 

/home/erdem/CLionProjects/MasterMindErdemDemır/cmake-build-debug/MasterMindErdemDemr

Codebreaker please select your strategy by pressing: 

(1)-Play as Human

(2)-Minimax algorithm developed by Donald Knuth will be play by computer 

(3)-Swaszek algorithm which requires no bookkeeping and still wins

1

 Please input your guess to konsole in form of integers.1122
 
Code breaker's 0. guess was *1**1**2**2*

Code maker's judgement: 1 black 0 white

 Please input your guess to konsole in form of integers.1133
 
Code breaker's 1. guess was *1**1**3**3*

Code maker's judgement: 0 black 0 white

 Please input your guess to konsole in form of integers.4422
 
Code breaker's 2. guess was *4**4**2**2*

Code maker's judgement: 1 black 0 white

 Please input your guess to konsole in form of integers.5526
 
Code breaker's 3. guess was *5**5**2**6*

Code maker's judgement: 2 black 2 white

 Please input your guess to konsole in form of integers.6525
 
Code breaker's 4. guess was *6**5**2**5*

Code maker's judgement: 2 black 2 white

 Please input your guess to konsole in form of integers.5625
 
Code breaker's 5. guess was *5**6**2**5*

Code maker's judgement: 4 black 0 white

You won!! It took you : 5 rounds 

Process finished with exit code 0



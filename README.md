qcSimulator
--

*This is work in progress.*

Objective: To write a Digital Quantum Computer simulator to test Quantum Simulation algorithms.

Language: C++ 11

Authors: [Kishor Bharti] and [Atul Singh Arora]

Development Phases:

    A. Basic Funtionality
        1. Implement the state initialization   [done]
        2. Implement measurement/readout [done]
        3. Implement gates
            i. Single qBit [done]
            ii. General, N qBit [done]
    B. Implement Usable Interface
        1. Allow for physics like state initialization (cheating method) [done]
        2. Allow for assembly language like gates implementation (look at the codes Neilson used in his book for making those figures) [working on regex]
        3. Implement common things like QFT, 
        (viz. construct the gate sequence on the fly and apply it)
        4. Apply arbitrary unitaries? [same as general N qBit gate]
    C. Extensions
        1. Allow for an easy way of adding algorithms/qbits dependent gate sequences
    D. Test/Write the part for Quantum Simulation

Version
----

infant

Tech
-----------


Installation
--------------
Well its not yet in that stage, but you can clone and compile. This has been written on windows and compiled using visual studio. But you could certainly compile it on linux (will try that soon)

```sh
git clone https://github.com/toAtulArora/qcSimulator.git
```

For compiling, use
```sh
cd qcSimulator
g++ -std=c++11 -I ../eigen qcSimulator.cpp -o Debug/qcSimulator
```
OR simply
```sh
g++ -std=c++11 -I eigen qcSimulator/qcSimulator.cpp -o qcSimulator/Debug/qcSimulator
```

and run using
```sh
./qcSimulator/Debug/qcSimulator
```
TODO: Add a makefile to do this.

Setting up cloud 9
-----
0. Login using your github account and allow the SSH key sharing.
1. Use a new project workspace with c++. (because directly cloning a github repo doesn't result in availability of the latest c compiler.)
2. Then use git clone for getting the repository cloned.
3. Now, ``` git remote set-url origin git@github.com:toAtulArora/qcSimulator.git ```

License
----
There's nothing really there to talk about licensing, but its very likely going to be free for academic use.

[Kishor Bharti]:mailto:kishor.bharti1@gmail.com
[Atul Singh Arora]:mailto:to.AtulArora@gmail.com
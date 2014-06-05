qcSimulator
--

*This is work in progress.*

Objective: To write a Digital Quantum Computer simulator to test Quantum Simulation algorithms.

Language: C++ 11

Authors: [Kishor Bharti] and [Atul Singh Arora]

Development Phases:

    A. Basic Funtionality
        1. Implement the state initialization
        2. Implement measurement/readout
        3. Implement gates
    B. Implement Usable Interface
        1. Allow for physics like state initialization
        2. Allow for assembly language like gates implementation (look at the codes Neilson used in his book for making those figures)
        3. Implement common things like QFT, 
        (viz. construct the gate sequence on the fly and apply it)
        4. Apply arbitrary unitaries?
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
git clone [git-repo-url] dillinger
```

License
----
There's nothing really there to talk about licensing, but its very likely going to be free for academic use.

[Kishor Bharti]:mailto:kishor.bharti1@gmail.com
[Atul Singh Arora]:mailto:to.AtulArora@gmail.com
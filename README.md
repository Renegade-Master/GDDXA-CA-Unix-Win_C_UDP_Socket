# PLEASE README 
GDD4A Networked Games Assignment 01: Hangman

---  

## Team Members  

| Name        | K-Number    |  
| ----------- | ----------- |  
| Ciaran Bent | [K00221230] |  
| Rory Ryan   | [K00218956] |  
| Gary Ryan   | [K00218218] |  

---  

## Repository Structure
```sh
.
├── hangman
│   ├── bin
│   │   └── EXECUTABLE FILES GO HERE
│   ├── rsc
│   │   └── RESOURCE FILES GO HERE
│   └── src
│       └── SOURCE AND HEADER FILES GO HERE
├── LICENSE
└── README.md
```

---

## To Add a New Commit  
1. Create a new Branch with a name that is something to do with the change you intend to make.  (e.g. `feature\myNewFeature`)
2. Work in your branch.
3. When your work is complete, submit a **PULL REQUEST** to `master` branch.
4. At least one other person must approve the PULL REQUEST.
5. Once the PULL REQUEST is approved, the change is automatically merged into `master` branch.
6. Remember that you will be asked to approve other's work, as well as them approve yours.  This is mutual learning.

---

## Compilation Instructions
1. Naviagte to `./hangman/` directory.
    - `cd hangman`
2. Run the compilation script for the binary you wish to create.
    - `./compile___.sh`
    - or `gcc src/___.c -o bin/hangman___ -std=gnu99 -Wall -Wextra -D_FORTIFY_SOURCE=1`

---

## Running Instructions
1. Navigate to `./hangman/bin/` directory.
    - `cd hangman/bin`
2. Run the binary you want to execute.
    - `./___`
3. Provide arguements if you want, but they are optional  
    - Client Arguements:
        - `./client_binary ${IP_ADDRESS}`  
    - Server Arguements [UDP]:  
        - `./server_binary ${MAX_PLAYERS}  `

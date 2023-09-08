
# MultiProgramming Operating System 

This MOS project is divided into 3 phases.



## Assumptions of Phase-1

- Jobs entered without error in input file
- No physical separation between jobs
- Job outputs separated in output file by 2 blank lines
- Program loaded in memory starting at location 00 
- No multiprogramming, load and run one program at a time
- SI interrupt for service request

## Assumptions of Phase-2
-	Jobs may have program errors
-	PI interrupt for program errors introduced
-	No physical separation between jobs
-	Job outputs separated in output file by 2 blank lines
-	Paging introduced, page table stored in real memory
-	Program pages allocated one of 30 memory block using random number generator  
-	Load and run one program at a time
-	Time limit, line limit, out-of-data errors introduced
-	TI interrupt for time-out error introduced
-	2-line messages printed at termination


# Code Documentation

This document provides an overview of the functions and their purpose in the provided code. The code appears to be related to a simulated computer system with a focus on memory management and program execution. Each function plays a specific role in this simulation.

## Table of Contents

1. [Introduction](#introduction)
2. [Function Documentation](#function-documentation)
   - [createMemoryFile](#creatememoryfile)
   - [evalGD](#evalgd)
   - [evalPD](#evalpd)
   - [evalH](#evalh)
   - [MOS1](#mos1)
   - [isValidPF](#isvalidpf)
   - [RectifyPF](#rectifypf)
   - [MOS2](#mos2)
   - [loadRegister](#loadregister)
   - [storeRegister](#storeregister)
   - [compareRegister](#compareregister)
   - [branchOnto](#branchonto)
   - [userProgram](#userprogram)
   - [storeInstruction](#storeinstruction)
   - [startWith](#startswith)
   - [ALLOCATE](#allocate)
   - [addressMap](#addressmap)
   - [appendErrorMess](#appenderrormess)
   - [Terminate](#terminate)
   - [createPageTable](#createpagetable)
   - [INIT](#init)
   - [excuteAMJ](#excuteamj)

---

## Introduction

This code simulates a operating systems operation, focusing on memory management, program execution, and error handling. It appears to be structured as a multi-phase simulation, and it includes functions for tasks like loading, executing, and handling various operations.

---

## Function Documentation

### createMemoryFile

#### Purpose

This function is responsible for creating and appending the contents of memory to a file named "MemoryFile.txt."

### evalGD

#### Purpose

This function simulates the execution of a "GD" (Get Data) operation. It loads data from a buffer into the computer's memory.

### evalPD

#### Purpose

This function simulates the execution of a "PD" (Print Data) operation. It prints data from memory to a line printer file named "linePrinter.txt."

### evalH

#### Purpose

This function simulates the execution of an "H" (Halt) operation. It terminates the program's execution and appends two new lines to the line printer file.

### MOS1

#### Purpose

This function handles various interrupts related to Time Limit Exceeded and Operation Errors during program execution.

### isValidPF

#### Purpose

This function checks if a Page Fault is valid for the current instruction, allowing for page allocation.

### RectifyPF

#### Purpose

This function rectifies a Page Fault by allocating a new page and updating the Page Table Entry (PTE).

### MOS2

#### Purpose

This function handles various interrupts related to Time Limit Exceeded and Operand Errors during program execution.

### loadRegister

#### Purpose

This function loads data from memory into a general-purpose register (R).

### storeRegister

#### Purpose

This function stores data from a general-purpose register (R) back into memory.

### compareRegister

#### Purpose

This function compares data in a general-purpose register (R) with data in memory and sets a toggle (C) accordingly.

### branchOnto

#### Purpose

This function performs a conditional branch operation based on the value of the toggle (C).

### userProgram

#### Purpose

This function simulates the execution of a user program, processing a series of instructions and handling various interrupt conditions.

### storeInstruction

#### Purpose

This function parses and stores individual instructions from a program card into memory for later execution.

### startWith

#### Purpose

This function checks if a given string starts with a specified prefix.

### ALLOCATE

#### Purpose

This function generates a random number to represent the allocation of a memory frame.

### addressMap

#### Purpose

This function maps a virtual address (VA) to a real address (RA) based on the Page Table Entry (PTE) and handles Page Faults.

### appendErrorMess

#### Purpose

This function appends an error message to a given string based on an error code.

### Terminate

#### Purpose

This function handles program termination, appending error messages and program statistics to the line printer file.

### createPageTable

#### Purpose

This function initializes and creates a Page Table in memory, marking unallocated pages with asterisks.

### INIT

#### Purpose

This function initializes the simulation, setting up the Page Table, allocated frames, and other parameters for the simulated job.

### excuteAMJ

#### Purpose

This function executes the "$AMJ" (Job Initiation) control card.

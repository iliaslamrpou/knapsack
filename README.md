
# Knapsack Solver
This repository contains tools and solvers about the knapsack 0-1 problem <br/>
For all the tools the C++ code is included in the repository<br/>

The ***generator folder*** contains two very helpful tools to create one knapsack 0-1 problem or a pack of problems<br/>
The problems can be solved using 6 algorithms: <br/>

- **Greedy approach**<br/>
- **Brute force**<br/>
- **Branch and bound**<br/>
- **Dynamic programming**<br/>
- **Dynamic OR-Tools solver**<br/>
- **Integer OR-Tools solver**<br/>

The solution of each problem is stored in a file<br/>
The solution of a pack of problems is stored in a CSV file for generating statistics<br/>

Web Page: (https://iliaslamrpou.github.io/knapsack)

## Instructions:
1. Download this repository
2. Run the file Knapsack.exe from the command line
3. On the menu select the option (8) :Create all knapsack problems (320 files)
   This selection will create the folder knapsack_problems with 320 knapsack problem files that can be solved using the Knapsack.exe
4. On the menu select the algorithm with which you want to solve all problems or one of them   
   You can create a CSV file with the solutions of all problems for each algorithm separately or for all algorithms     

**Knapsack.exe**          This is the main executable file. Contains the solvers and many features<br/>
**or_tools_dynamic.exe**  This is a utility used by the main file to solve a problem using the OR-Tools Dynamic algorithm<br/>
**or_tools_integer.exe**  This is a utility used by the main file to solve a problem using the OR-Tools Integer algorithm<br/>
**knapsack_gen.exe**      This is a utility used by the main file to create a knapsack problem<br/>

All of these files have to be in the same folder<br/>

## Other folders:
generator folder: <br/>
    **knapsack_gen.c**      : Contains the code of the knapsack generator (C++ code)<br/>
    **knapsackFilesCreator**: This utility calls the knapsack generator to create 320 problems files (C++ code)<br/>
    **oneClickCreator**     : MsDos batch file that calls the knapsackFilesCreator to create all the necessary knapsack problems <br/>

## Required hardware and software:
CPU: 64bit<br/>
Windows 64bit<br/>
Ram: 4Gb<br/>

## Licence: This code is created by Ilias Lamprou
You can use it for educational use<br/>
Contact us for commercial use<br/>

`Dec 31 2019`



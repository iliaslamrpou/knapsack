
<h1 style="color: #5e9ca0;">You can edit <span style="color: #2b2301;">this demo</span> text!</h1>
<h2 style="color: #2e6c80;">How to use the editor:</h2>
<p>Paste your documents in the visual editor on the left or your HTML code in the source editor in the right. <br />Edit any of the two areas and see the other changing in real time.&nbsp;</p>
Instructions:
1. Download this repository
2. Run the file Knapsack.exe
3. On the menu select the option 8:Create all knapsack problems (320 files)
   This selection will create the folder knapsack_problems with 320 knapsack problems files  that can be solved using the Knapsack.exe
4. On the menu select the algorithm with which you want to solve all problems or one of then   
   You can create a CSV files with the solutions of all problems for each algorithm separately or for all algorithms     

==================================
Knapsack.exe          This the main solver with many features
or_tools_dynamic.exe  Util that used by the main file to solve a problem using the OR-Tools Dynamic algorithm
or_tools_integer.exe  Util that used by the main file to solve a problem using the OR-Tools Integer algorithm
knapsack_gen.exe      Util that used by the main file to create a knapsack problem

All of these files has to be to the same folder

Other folders:
generator folder: 
    knapsack_gen.c      : Contains the code the knapsack generator (C++ code)
    knapsackFilesCreator: This utils calls the knapsack generator to create 320 probles files (C++ code)
    oneClickCreator     : MsDos batch file that call the knapsackFilesCreator (C++ code)

//===================================
Required hardware and software:
CPU: 64bit
Windows 64bit
Ram: 10Gb

//===================================
Licence: This code is created by Ilias Lamprou
You can use it for educational use
For commercial use you have get the permission of the owner





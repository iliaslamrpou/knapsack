/* Integer αλγόριθμος OR-Tools
   Created by: Ηλίας Λάμπρου 
   ver 1.0.1  
   Δημιουργήθηκε: 15/12/2019
   Τροποποιήθηκε: 23/12/2019 
   exe file: algorith2.exe 
   σύνταξη: algorith2 inputFilename ouputFilename parameter
   Διαβάζει ένα αρχείο Knapsack και εκτυπώσει το αποτέλεσμα στο αρχείο ouputFilename
   Εάν υπάρχει η τρίτη παράμετρος εκτυπώνει αναλυτική αναφορά μαζί με τα επιλεγμένα αντικείμενα
   διαφορετικά προσθέτει στο τέλος του αρχείου (CSV) το βάρος και την αξία   
*/

#include "../ortools/include/ortools/linear_solver/linear_solver.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <chrono>
namespace operations_research{
std::vector<int> KnapsackInteger(std::vector<int64> &values,std::vector<int64> &weights, int capacity){
    MPSolver solver("IP KNAPSACK SOLVER", MPSolver::CBC_MIXED_INTEGER_PROGRAMMING);
    MPConstraint *const c = solver.MakeRowConstraint(0, capacity, "capacity_constraint");
    MPObjective *const objective = solver.MutableObjective();
    std::vector<MPVariable*> problemVar;                                                  
    for (int i=0;i<values.size();i++){
      std::string s= "x" + std:: to_string((i+1));
      //std::cout<<s<<"\n"; 
      MPVariable *const x = solver.MakeBoolVar(s);
      problemVar.push_back(x);
       c->SetCoefficient(x, weights.at(i));
       objective->SetCoefficient(x, values.at(i));
    }
    objective->SetMaximization();
    solver.Solve();
    //std::cout << "Objective value = " << objective->Value() << std::endl;
    
    //----- Solution
    std::vector<int> packed_items;
    for (int i=0;i<problemVar.size();i++){
       int solValue=problemVar.at(i)->solution_value();
       //std::cout << "x"<<i<<"= " << solValue<<"\n";
       if (solValue==1) packed_items.push_back(i);
    }
    return packed_items;
}
} // namespace operations_research


//=========================================================== main
 int main(int argc, char *argv[]){
  if (argc<3)  {
      std::cout << "Please enter arguments \n";
	    exit(0);
	}
  
  std::cout << "------------------------------\n";
  std::cout << "---Integer OR-Tools Solver ---\n";
  char* inputFilename = argv[1];
  std::cout <<"---Input file: "<< inputFilename<<"\n";
 
   std::ifstream fin(inputFilename);  
   if (!fin.good()) {    
        std::cerr << "Error opening file " << inputFilename << std::endl; 
        system("pause");
        exit(-1);  
    }

    int items_number;
    fin >> items_number; 
    std::cout<<"---Items: "<<items_number<<"\n";
    std::vector<int64> values;
    std::vector<int64> weights;
    int id;    
    for (int i = 0; i < items_number; i++){
        fin >> id;  
        int v;
        fin >> v;
        values.push_back(v);
        int w;
        fin >> w;
        weights.push_back(w);
       // std::cout<<id<<" "<<v<<" "<<w<<"\n";
    }
    int capacity;
    fin >> capacity;   
    std::cout<<"---Capacity: "<<capacity<<"\n";
    
    auto start = std::chrono::high_resolution_clock::now();
    
    std::vector<int> selectedItems = operations_research::KnapsackInteger(values,weights,capacity);
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    unsigned long lastSolutionTime = duration.count();
    
    int maxWeight=0;
    int maxProfit=0;
    std::cout<<"------ Solution ---------\n";
    for (int i=0;i<selectedItems.size();i++){
      int selectedItem = selectedItems.at(i);
      int itemWeight = weights.at(selectedItem);
      int itemProfit = values.at(selectedItem);
      //std::cout<<itemWeight<<" " << itemProfit <<"\n";
      maxProfit+=itemProfit;
      maxWeight+=itemWeight;
      
    }
    std::cout<<"---maxWeight= "<<maxWeight<<"\n";
    std::cout<<"---maxProfit= "<<maxProfit<<"\n";
    std::cout<<"---Time= "<<lastSolutionTime<<"\n";
    
   
     char* outputFilename = argv[2];
     
    
    if (argc>3) {
         std::cout <<"---Output file: "<< outputFilename<<"\n";
         std::fstream fout;
         fout.open(outputFilename, std::ios::out);
         if (!fout.good()) {     
            std::cerr << "--- Error opening file " << outputFilename << std::endl;
            exit(-1);      
          }
            fout <<"------- Integer OR-Tools Solver -------\n\n";
            std::cout <<"---Selected items\n";
            for (int i=0;i<selectedItems.size();i++){
                int selectedItem = selectedItems.at(i);
                int itemWeight = weights.at(selectedItem);
                int itemProfit = values.at(selectedItem);
                fout <<selectedItem;
                fout << " ";
                fout <<itemProfit;
                fout <<" ";
                fout <<itemWeight;
                fout <<"\n";
                std::cout<<selectedItem <<' '<<itemProfit<<' '<<itemWeight<<'\n';
          }
          fout << " " ;
          fout << maxProfit;
          fout << " ";
          fout <<maxWeight;
          fout <<"\n";
          fout.close();
          
          
    
    }
    else  {
      std::fstream fout;
      fout.open(outputFilename, std::ios::app);
      if (!fout.good()) {     
        std::cerr << "--- Error opening file " << outputFilename << std::endl;
        exit(-1);      
      }
      fout <<";"<<maxWeight<<";"<<maxProfit<<";"<<lastSolutionTime;
    
      fout.close();
     }
    
   
  return EXIT_SUCCESS;
}

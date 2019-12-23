/* Δυναμικός αλγόριθμος OR-Tools
   Created by: Ηλίας Λάμπρου 
   ver 1.0.1  
   Δημιουργήθηκε: 13/12/2019
   Τροποποιήθηκε: 23/12/2019 
   exe file: algorith1.exe 
   σύνταξη: algorith1 inputFilename ouputFilename parameter
   Διαβάζει ένα αρχείο Knapsack και εκτυπώσει το αποτέλεσμα στο αρχείο ouputFilename
   Εάν υπάρχει η τρίτη παράμετρος εκτυπώνει αναλυτική αναφορά μαζί με τα επιλεγμένα αντικείμενα
   διαφορετικά προσθέτει στο τέλος του αρχείου (CSV) το βάρος και την αξία   
*/

#include "../ortools/include/ortools/algorithms/knapsack_solver.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <chrono>



namespace operations_research{
   //=========================================================== main
std::vector<int> KnapsackExample(std::vector<int64> &values,std::vector<std::vector<int64>> &weights, std::vector<int64> &capacities ){
    KnapsackSolver solver(KnapsackSolver::KNAPSACK_DYNAMIC_PROGRAMMING_SOLVER, "SimpleKnapsackExample");
    solver.Init(values, weights, capacities);
    int64 computed_value = solver.Solve();
   // std::cout << "Total value: " << computed_value << std::endl;

    std::vector<int> packed_items;
    for (std::size_t i = 0; i < values.size(); ++i){
        if (solver.BestSolutionContains(i)) {
            packed_items.push_back(i);
            }
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
  std::cout << "--------------------------------\n";
  std::cout << "---Dynamic OR-Tools algorithm---\n";
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
    
    std::vector<std::vector<int64>> weightsVectors;
    weightsVectors.push_back(weights);
    std::vector<int64> capacities;
    capacities.push_back(capacity);

    auto start = std::chrono::high_resolution_clock::now();
    
    std::vector<int> selectedItems = operations_research::KnapsackExample(values,weightsVectors,capacities);
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    unsigned long lastSolutionTime = duration.count();
    
    int maxWeight=0;
    int maxProfit=0;
    std::cout << "----------- Solution --------\n";
    for (int i=0;i<selectedItems.size();i++){
      int selectedItem = selectedItems.at(i);
      int itemWeight = weights.at(selectedItem);
      int itemProfit = values.at(selectedItem);
      //std::cout<<itemWeight<<" " << itemWeight <<"\n";
      maxProfit+=itemProfit;
      maxWeight+=itemWeight;
      
    }
    std::cout<<"---maxWeight="<<maxWeight<<"\n";
    std::cout<<"---maxProfit="<<maxProfit<<"\n";
    std::cout<<"---lastSolutionTime="<<lastSolutionTime<<"\n";
    
  
    char* outputFilename = argv[2];
    
  
    
    if (argc>3) {
         std::cout <<"---Output file: "<< outputFilename<<"\n";
         std::fstream fout;
         fout.open(outputFilename, std::ios::out);
         if (!fout.good()) {     
            std::cerr << "--- Error opening file " << outputFilename << std::endl;
            exit(-1);      
          }
            fout <<"------- Dynamic OR-Tools Algorithm -------\n\n";
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
